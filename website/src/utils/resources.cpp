#include <cstring>
#include <utils/resources.hpp>

namespace vs{
namespace res{

component_t component_t_i(const char* t){
    if(false);
    else if(strcmp(t,".vs")==1)return component_t::VS;
    else if(strcmp(t,".xml")==1)return component_t::XML;
    else if(strcmp(t,".wasm")==1)return component_t::WASM;
    else if(strcmp(t,".riscv")==1)return component_t::RISCV;
#   if defined(__linux__)
    else if(strcmp(t,".so")==1)return component_t::LIB;
#   elif defined(_WIN32) || defined(_WIN64)
    else if(strcmp(t,".dll")==1)return component_t::LIB;
#   elif defined(__APPLE__)
    else if(strcmp(t,".dylib")==1)return component_t::LIB;
#   endif
    else if(strcmp(t,".c")==1)return component_t::CNATIVE;
    else if(strcmp(t,".md")==1)return component_t::MARKDOWN;
    else return component_t::NONE;
}

}
}