#pragma once
/**
 * @file paths.hpp
 * @author karurochari
 * @brief Utilities to handle virtual and real paths in a safe and portable way.
 * @details This library DOES NOT fetch nor verify those files actually exists, but error codes are provided so that they can be used in cascade.
 *          The only checks performed are about policies, to ensure that path in that context was usable.
 * @copyright Copyright (c) 2024
 * 
 */

#include <cstring>
#include <string>
#include <utils/resources.hpp>
#include <utils/policies.hpp>
#include <utils/arch.hpp>

namespace vs{


#define tkn(a,b) strncmp((a),(b),std::char_traits<char>::length(b))==0
#define vprefix(b) if(strncmp((src),vpath_type_t::as_string(b),std::char_traits<char>::length(vpath_type_t::as_string(b)))==0){type=b;location=src+std::char_traits<char>::length(vpath_type_t::as_string(b));}
#define rprefix(b) if(strncmp((src),rpath_type_t::as_string(b),std::char_traits<char>::length(rpath_type_t::as_string(b)))==0){type=b;location=src+std::char_traits<char>::length(rpath_type_t::as_string(b));}

/**
 * @brief In case of a file inclusion without extension, like `this://component`, this function tells which one to look for next.
 * 
 * @return std::pair<bool,component_t> boolean true if a file, false if a file inside the folder.
 */
std::pair<bool,res::component_t> next_component_attempt(std::pair<bool,res::component_t>);

struct vpath_type_t{
    enum t{
        NONE,
        THIS,FS,HTTP,HTTPS,GEMINI,TMP,DATA,REPO,APP,VS,CWD,     //Real paths
        SOCKET,                                                 //External endpoint
        STORAGE,SESSION                                         //Cache loopbacks
    };

    static inline constexpr const char* prefixes[] = {
        "[ERROR]",      
        "this://",      //Local to the current component
        "file://",      //Local fs
        "http://",      //Unprotected http traffic
        "https://",     //Encrypted http traffic
        "gemini://",    //Encrypted gemini traffic
        "tmp://",       //Location on disk which can be used to store temporary files (cannot ..)
        "data://",      //Location on disk where packages & custom elements are stored (cannot ..)
        "repo://",      //Location on disk where packages & custom elements are stored (cannot ..)
        "app://",       //Location on disk where the root application node is stored (cannot ..)
        "vs://",       //Location on disk where the VS application is hosted (cannot ..)
        "cwd://",      //Location on disk of the current working directory
        "socket://",   //Location of the socket endpoint used for external code (cannot ..)
        "storage://",  //Loopback to the internal permanent cache. Format: class/key/hash
        "session://",  //Loopback to the internal temporary cache. Format: class/key/hash
    };

    static inline constexpr const char* as_string(t idx){
        return prefixes[idx];
    }
};

struct rpath_type_t{
    enum t{
        NONE,
        FS,HTTP,HTTPS, GEMINI
    };

    static inline constexpr const char* prefixes[] = {
        "[ERROR]",      
        "file://",      //Local fs
        "http://",      //Unprotected http traffic
        "https://",     //Encrypted http traffic
        "gemini://",     //Encrypted http traffic
    };

    static inline constexpr const char* as_string(t idx){
        return prefixes[idx];
    }
};

struct scoped_vpath_t{
    vpath_type_t::t type;
    std::string location;

    std::string as_string() const{
        return std::string(vpath_type_t::prefixes[type])+location;
    }

    void from_string(const char* src);

    scoped_vpath_t to_base_dir(){
        scoped_vpath_t ret = *this;
        int ptr = ret.location.length()-1;
        for(;ptr>=0 && ret.location.data()[ptr]!='/';ptr--){}
        ptr++;
        ret.location.data()[ptr]=0;
        return ret;
    }
};

struct scoped_rpath_t{
    rpath_type_t::t type;
    std::string location;

    //TODO: Slow operation often used but not recorded
    std::string as_string() const{
        return std::string(rpath_type_t::prefixes[type])+location;
    }

    void from_string(const char* src){
        rprefix(rpath_type_t::FS)
        else rprefix(rpath_type_t::HTTP)
        else rprefix(rpath_type_t::HTTPS)
        else rprefix(rpath_type_t::GEMINI)
        else {type=rpath_type_t::FS;location=src;}
    }

    scoped_rpath_t base_dir(){
        scoped_rpath_t ret = *this;
        int ptr = ret.location.length()-1;
        for(;ptr>=0 && ret.location.data()[ptr]!='/';ptr--){}
        ptr++;
        ret.location.data()[ptr]=0;
        ret.location.resize(ptr);   //TODO: Verify if this does avoid a new allocation.
        return ret;
    }
};

//All these dirs are assumed to be normalized already
struct path_env_t{
    scoped_rpath_t root;            //Location of VS files

    scoped_rpath_t cwd;             //Current working directory
    scoped_rpath_t app_path;        //Path of the current app root
    scoped_rpath_t tmp_path;        //Derived from system
    scoped_rpath_t packages_path;   //Derived from home
    scoped_rpath_t userdata_path;   //Derived from home

    scoped_rpath_t socket_file;
};



//Weak in respect  to a parent providing all this info!
struct resolve_path{
    private:
        const policies_t& policies;
        const path_env_t& env;
        const scoped_rpath_t& local;

    public:

    enum class from_t{
        EMBEDDED_SCRIPT, NATIVE_CODE,
    };

    inline resolve_path(const policies_t& _0, const path_env_t& _1, const scoped_rpath_t& _2):policies(_0),env(_1),local(_2){}

    struct reason_t{
        enum t{
            NOT_IMPLEMENTED,           //For paths not implemented yet.
            OK,
            NOT_FOUND,
            ROOT_REACHED,
            MALFORMED,          
            POLICY_VIOLATION,   //A request in direct contrast with policies
            UNTRUSTED,          //Not in the trusted list derived from policies
        };

        static inline constexpr const char* prefixes[] = {
            "not-implemented",
            "ok",    
            "not found", 
            "root reached",  
            "malformed",  
            "policy violation",
            "untrusted",
        };

        static inline constexpr const char* as_string(t idx){
            //If no prefix is provided, `this://` is assumed.
            return prefixes[idx];
        }
    };

    std::pair<bool, std::string> static normalizer(const char *parent, const char *child, bool allow_exit, bool base_dir=false);

    ///Try to resolve the path described by src based on the policies, env, and current path.
    std::pair<reason_t::t,scoped_rpath_t> operator()(from_t from,const char* src);

};

#undef tnk
#undef prefix

}
