#pragma once
/**
 * @file paths.hpp
 * @author karurochari
 * @brief Utilities to handle virtual and real paths in a safe and portable way.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <cstring>
#include <string>
#include <utils/policies.hpp>

namespace vs{


#define tkn(a,b) memcmp((a),(b),std::char_traits<char>::length(b))==0
#define vprefix(b) if(memcmp((src),vpath_type_t::as_string(b),std::char_traits<char>::length(vpath_type_t::as_string(b)))==0){type=b;location=src+std::char_traits<char>::length(vpath_type_t::as_string(b));}
#define rprefix(b) if(memcmp((src),rpath_type_t::as_string(b),std::char_traits<char>::length(rpath_type_t::as_string(b)))==0){type=b;location=src+std::char_traits<char>::length(rpath_type_t::as_string(b));}


struct vpath_type_t{
    enum t{
        NONE,
        THIS,FS,HTTP,HTTPS,TMP,DATA,REPO,APP,VS,CWD,  //Real paths
        SOCKET,                                 //External endpoint
        STORAGE,SESSION                         //Cache loopbacks
    };

    static inline constexpr const char* prefixes[] = {
        "",      
        "this://",      //Local to the current component
        "file://",      //Local fs
        "http://",      //Unprotected http traffic
        "https://",     //Encrypted http traffic
        "tmp://",       //Location on disk which can be used to store temporary files (cannot ..)
        "data://",       //Location on disk where packages & custom elements are stored (cannot ..)
        "repo://",       //Location on disk where packages & custom elements are stored (cannot ..)
        "app://",       //Location on disk where the root application node is stored (cannot ..)
        "vs://",        //Location on disk where the VS application is hosted (cannot ..)
        "cwd://",       //Location on disk of the current working directory
        "socket://",    //Location of the socket endpoint used for external code (cannot ..)
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
        FS,HTTP,HTTPS
    };

    static inline constexpr const char* prefixes[] = {
        "",      
        "file://",      //Local fs
        "http://",      //Unprotected http traffic
        "https://",     //Encrypted http traffic
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

    void from_string(const char* src){
        vprefix(vpath_type_t::THIS)
        else vprefix(vpath_type_t::DATA)
        else vprefix(vpath_type_t::REPO)
        else vprefix(vpath_type_t::APP)
        else vprefix(vpath_type_t::CWD)
        else vprefix(vpath_type_t::FS)
        else vprefix(vpath_type_t::HTTP)
        else vprefix(vpath_type_t::HTTPS)
        else vprefix(vpath_type_t::TMP)
        else vprefix(vpath_type_t::VS)
        else vprefix(vpath_type_t::SOCKET)
        else vprefix(vpath_type_t::STORAGE)
        else vprefix(vpath_type_t::SESSION)
        else {type=vpath_type_t::THIS;location=src;}
    }

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

    std::string as_string() const{
        return std::string(rpath_type_t::prefixes[type])+location;
    }

    void from_string(const char* src){
        rprefix(rpath_type_t::FS)
        else rprefix(rpath_type_t::HTTP)
        else rprefix(rpath_type_t::HTTPS)

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
    scoped_rpath_t root;

    scoped_rpath_t cwd;
    scoped_rpath_t app_path;
    scoped_rpath_t tmp_path;       //Derived from system
    scoped_rpath_t packages_path;  //Derived from home
    scoped_rpath_t appdata_path;   //Derived from home

    scoped_rpath_t socket_file;
};



//Weak in respect  to a parent providing all this info!
struct resolve_path{
    private:
        const policies_t& policies;
        const path_env_t& env;
        const scoped_rpath_t& local;

    public:

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
    std::pair<reason_t::t,scoped_rpath_t> operator()(const char* src);
};

#undef tnk
#undef prefix

}