#pragma once

#include <memory>
#include <ui.hpp>
#include <pugixml.hpp>

namespace vs{
namespace pipelines{

extern void lua_error_func_xml(const pugi::xml_node& env, const char * msg);
extern void lua_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name);

//extern void lua_error_func_native(void* env, const char * msg);
//extern void lua_log_symbol_func_native(void* env, const char * msg, const char* name);

/*
extern std::shared_ptr<lua> lua_lua_pipeline(bool is_runtime, ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), const char *link_with);
extern std::shared_ptr<smap<symbol_t>> lua_lua_pipeline_apply(const std::shared_ptr<lua>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*));

inline std::shared_ptr<lua> lua_lua_pipeline_xml(bool is_runtime, vs::ui_base* obj, pugi::xml_node& ctx,  const char *link_with){
    return lua_lua_pipeline(is_runtime,obj,ctx.text().as_string(),&ctx,(void(*)(void*,const char*))lua_error_func_xml,link_with);
}
*/

}
}