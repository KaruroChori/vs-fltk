#pragma once

#if VS_USE_QJS

#include <memory>
#include <ui.hpp>
#include <quickjs.h>

#include <pugixml.hpp>

//TODO: restructure following changes in the tcc interface

namespace vs{
namespace pipelines{

extern void qjs_error_func_xml(const pugi::xml_node& env, const char * msg);
extern void qjs_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name);

/*
struct jsctx{
    private:
        JSContext* data;
    public:
        jsctx(){
            data=JS_NewContext();
        }
};
*/

struct quickjs_t{
    JSContext* ctx;
    std::vector<std::tuple<std::string,symbol_type_t,JSValue>> handles;

    operator JSContext* (){return ctx;}

    quickjs_t(JSRuntime* rt);

    virtual ~quickjs_t(){
        for(auto& i: handles){JS_FreeValue(ctx, std::get<2>(i));}
        JS_FreeContext(ctx);
    }
};

extern std::shared_ptr<quickjs_t> qjs_js_pipeline(bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), const char *link_with);
extern std::shared_ptr<smap<symbol_t>> qjs_js_pipeline_apply(const std::shared_ptr<quickjs_t>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*));

inline std::shared_ptr<quickjs_t> qjs_js_pipeline_xml(bool is_runtime, vs::ui_base* obj, pugi::xml_node& ctx,  const char *link_with){
    return qjs_js_pipeline(is_runtime,obj,ctx.text().as_string(),&ctx,(void(*)(void*,const char*))qjs_error_func_xml,link_with);
}


}
}


#endif