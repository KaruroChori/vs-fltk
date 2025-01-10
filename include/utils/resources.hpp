#pragma once


#include <utility>

namespace vs{
namespace res{

//TODO: Move script_t/component_t and possibly more somewhere else

enum struct script_t{
    NONE,
    C,
    JS,
    LUA
};

enum struct component_t{
    NONE,
    VS,
    XML,
    LIB,
    WASM,
    RISCV,
    CNATIVE,
    MARKDOWN,
};
component_t component_t_i(const char* t);
constexpr const char* component_t_s(component_t t){
    if(t==component_t::NONE)return "";
    else if(t==component_t::VS)return ".vs";
    else if(t==component_t::XML)return ".xml";
    else if(t==component_t::LIB){
        //TODO:Add arch fragment?
#   if defined(__linux__)
        return ".so";
#   elif defined(_WIN32) || defined(_WIN64)
        return ".dll";
#   elif defined(__APPLE__)
        return ".dylib";
#   endif
    }
    else if(t==component_t::WASM)return ".wasm";
    else if(t==component_t::RISCV)return ".riscv";
    else if(t==component_t::CNATIVE)return ".c";
    else if(t==component_t::MARKDOWN)return ".md";
    else return nullptr;
}


/**
 * @brief In case of a file inclusion without extension, like `this://component`, this function tells which one to look for next.
 * 
 * @return std::pair<bool,component_t> boolean true if a file, false if a file inside the folder.
 */
std::pair<bool,component_t> next_component_attempt(std::pair<bool,component_t>);


}
}