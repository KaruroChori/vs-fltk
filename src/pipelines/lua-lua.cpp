#if VS_USE_LUA

#include <pipelines/lua-lua.hpp>

namespace vs{
namespace pipelines{

void lua_error_func_xml(const pugi::xml_node& env, const char * msg){}
void lua_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name){}

std::shared_ptr<lua_t> lua_lua_pipeline(global_ctx_t& globals, bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), link_with_t link_with){return {};}
std::shared_ptr<smap<symbol_t>> lua_lua_pipeline_apply(const std::shared_ptr<lua_t>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*)){return {};}

}
}

#endif