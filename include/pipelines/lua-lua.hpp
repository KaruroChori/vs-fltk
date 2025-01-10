#pragma once
#include <memory>

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <ui.hpp>
#include <pugixml.hpp>
#include <globals.hpp>
#include <pipelines/commons.hpp>


namespace vs{
namespace pipelines{

extern void lua_error_func_xml(const pugi::xml_node& env, const char * msg);
extern void lua_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name);

//extern void lua_error_func_native(void* env, const char * msg);
//extern void lua_log_symbol_func_native(void* env, const char * msg, const char* name);


//Helpful aside from the official docs https://lucasklassmann.com/blog/2019-02-02-embedding-lua-in-c/

struct lua_t{
    lua_State* ctx;

    operator lua_State* (){return ctx;}

    lua_t(){
        ctx=luaL_newstate();
        luaL_openlibs(ctx);
    }

    virtual ~lua_t(){
        lua_close(ctx);
    }
};

extern std::shared_ptr<lua_t> lua_lua_pipeline(global_ctx_t& globals, bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), link_with_t link_with);
extern std::shared_ptr<smap<symbol_t>> lua_lua_pipeline_apply(const std::shared_ptr<lua_t>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*));

inline std::shared_ptr<lua_t> lua_lua_pipeline_xml(global_ctx_t& globals, bool is_runtime, vs::ui_base* obj, pugi::xml_node& ctx, link_with_t link_with){
    return lua_lua_pipeline(globals,is_runtime,obj,ctx.text().as_string(),&ctx,(void(*)(void*,const char*))lua_error_func_xml,link_with);
}

}
}