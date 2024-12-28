#include <utils/paths.hpp>

namespace vs{

std::pair<bool,res::component_t> next_component_attempt(std::pair<bool,res::component_t> current){
    /*static constexpr std::pair<bool,component_t> next[] = {
        {true, component_t::VS},
        {true,component_t::XML},
        {true,component_t::WASM},
        {true,component_t::LIB},
        {true,component_t::CNATIVE},
        {false, component_t::VS},
        {false,component_t::XML},
        {false,component_t::WASM},
        {false,component_t::LIB},
        {false,component_t::CNATIVE},
        {true,component_t::MARKDOWN},
        {false,component_t::MARKDOWN},
        {false,component_t::NONE},
    };*/
    if(current.second<res::component_t::CNATIVE)return {current.first,(res::component_t)((int)current.second+1)};
    else if (current.first==true && current.second==res::component_t::CNATIVE)return {false,res::component_t::VS};
    else if(current.first==false && current.second==res::component_t::CNATIVE)return {true,res::component_t::MARKDOWN};
    else if (current.first==true && current.second==res::component_t::MARKDOWN)return {false,res::component_t::MARKDOWN};
    else if(current.first==false && current.second==res::component_t::MARKDOWN)return {false,res::component_t::NONE};

    return {false, res::component_t::NONE};
}


void scoped_vpath_t::from_string(const char* src){
        vprefix(vpath_type_t::THIS)
        else vprefix(vpath_type_t::DATA)
        else vprefix(vpath_type_t::REPO)
        else vprefix(vpath_type_t::APP)
        else vprefix(vpath_type_t::CWD)
        else vprefix(vpath_type_t::FS)
        else vprefix(vpath_type_t::HTTP)
        else vprefix(vpath_type_t::HTTPS)
        else vprefix(vpath_type_t::GEMINI)
        else vprefix(vpath_type_t::TMP)
        else vprefix(vpath_type_t::VS)
        else vprefix(vpath_type_t::SOCKET)
        else vprefix(vpath_type_t::STORAGE)
        else vprefix(vpath_type_t::SESSION)
        else {
            type=vpath_type_t::THIS;
            location=src;
        }
    }

std::pair<bool, std::string> resolve_path::normalizer(const char *parent, const char *child, bool allow_exit, bool base_dir){
    int parent_len=strlen(parent);
    int child_len=strlen(child);
    char ret[parent_len+child_len+1];
    memset(ret+parent_len,0,child_len+1);
    memcpy(ret,parent,parent_len);
    int ptr = parent_len;
    
    for(int j=0; j<child_len; ){
        if(tkn(child+j,"../")){
            //Go back to the past /
            if(ptr<=1)return {false, ""}; //Early failure, cannot track back.
            ptr-=2;
            for(;ret[ptr]!='/';ptr--);
            j+=3;
            ptr++;
            if(ptr<parent_len && allow_exit==false){return {false, ""};} //I reached the parent and cannot get any further.
            continue;
        }
        else if(tkn(child+j,"./")){
            j+=2;   //Just skip this.
            continue;
        }
        else if(child[j]=='/' && child[j+1]=='/'){
            j++;
            continue;
        }
        else if(child[j]=='/'){
            j++;
        }
        else{
            ret[ptr]=child[j];
            ptr++;
            j++;
            continue;
        }

        if(j!=child_len){
            ret[ptr]='/';
            ptr++;
        }
    }

    //TODO: Stress test for potential unsafety
    if(base_dir){
        for(;ret[ptr]!='/';ptr--);
        ptr++;
    }    
    
    ret[ptr]=0;

    return {true,std::string(ret)};
}

//TODO: Once policies are implemented, they will be tested in here as well.
std::pair<resolve_path::reason_t::t,scoped_rpath_t> resolve_path::operator()(from_t from,const char* src){
    scoped_vpath_t tmp; tmp.from_string(src);

    if(tmp.type==vpath_type_t::THIS){
        auto normalized = normalizer(local.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{local.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    //All on FS Block
    else if(tmp.type==vpath_type_t::CWD){
        auto normalized = normalizer(env.cwd.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.cwd.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::REPO){
        auto normalized = normalizer(env.packages_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.packages_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::DATA){
        auto normalized = normalizer(env.userdata_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.userdata_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::TMP){
        auto normalized = normalizer(env.tmp_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.tmp_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {env.tmp_path.type, ""}};
    }
    else if(tmp.type==vpath_type_t::VS){
        auto normalized = normalizer(env.root.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{rpath_type_t::FS,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::FS){
        if(from==from_t::NATIVE_CODE || policies.scripts.allow_fs)
            return {reason_t::OK, {rpath_type_t::FS, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::APP){
        auto normalized = normalizer(env.app_path.location.data(), tmp.location.data(), false);
        if(normalized.first){
            return {reason_t::OK,{env.app_path.type,normalized.second}};
        }
        else return {reason_t::ROOT_REACHED, {rpath_type_t::NONE, ""}};
    }
    #ifdef VS_USE_NETWORKING_CURL
    else if(tmp.type==vpath_type_t::HTTP){
        if(((from==from_t::NATIVE_CODE ) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_http)
            return {reason_t::OK, {rpath_type_t::HTTP, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::HTTPS){
        if(((from==from_t::NATIVE_CODE) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_https){
            return {reason_t::OK, {rpath_type_t::HTTPS, tmp.location}};
        }
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    else if(tmp.type==vpath_type_t::GEMINI){
        if(((from==from_t::NATIVE_CODE) || (from==from_t::EMBEDDED_SCRIPT && policies.scripts.allow_networking)) && policies.networking.allow_gemini)
            return {reason_t::OK, {rpath_type_t::GEMINI, tmp.location}};
        else return {reason_t::POLICY_VIOLATION, {rpath_type_t::NONE, ""}};
    }
    #endif
    return {reason_t::MALFORMED,{rpath_type_t::NONE, ""}};
}  

}