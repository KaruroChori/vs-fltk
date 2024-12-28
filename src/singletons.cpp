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

}
}