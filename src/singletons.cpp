#include <singletons.hpp>
#include <chrono>

#ifdef VS_USE_NETWORKING_CURL
#include <curl/curl.h>
#endif

namespace vs{
namespace singleton{

debug_t::debug_t(){auto file=getenv("VS_DEBUG_FILE");if(file!=nullptr)fd=fopen(file,"w+");}
debug_t::~debug_t(){if(fd!=nullptr)fclose(fd);}

void debug_t::operator()(const char* field, const char* value){  
    if(fd==nullptr)return;
    else{
        auto now = std::chrono::system_clock::now();
        fprintf(fd,"%s\t%s\t%ld\n",field,value,std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count());
    }
}


#ifdef VS_USE_NETWORKING_CURL
curl_t::curl_t(){curl_global_init(CURL_GLOBAL_ALL);}
curl_t::~curl_t(){curl_global_cleanup();}

curl_t curl;
debug_t debug;
#endif 

/**
 * @brief Global structure covering any type for serialization/deserialization
 * 
 */
field_models_t field_models ={{
    [field_models_t::FLAG] = {
        +[](void* _dst, const char* src)->int{
            bool* dst = (bool*) _dst;   //TODO: NOOOO, it must use the tagged union
            if(strcmp(src, "false")==0){*dst=false;return 0;}
            else if(strcmp(src,"true")==0){*dst=true;return 0;}
            return 1;
        },
        +[](const void* _src, const char**  dst)->int{
            const bool* src = (const bool*) _src;   //TODO: NOOOO, it must use the tagged union
            if(*src==true){auto tmp = malloc(sizeof("true"));memcpy(tmp,"true",sizeof("true"));*dst=(const char*)tmp;return 0;}
            else if(*src==false){auto tmp = malloc(sizeof("false"));memcpy(tmp,"false",sizeof("false"));*dst=(const char*)tmp;return 0;}
            else dst=nullptr;
            return 1;
        }
    },
}};

}
}