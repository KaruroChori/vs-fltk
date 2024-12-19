#pragma once

#if VS_USE_TCC

#include <memory>
#include <ui.hpp>
#include <utils/tcc-wrap.hpp>
#include <pugixml.hpp>
#include "globals.hpp"
#include "commons.hpp"

namespace vs{
namespace pipelines{

extern void tcc_error_func_xml(const pugi::xml_node& env, const char * msg);
extern void tcc_log_symbol_func_xml(const pugi::xml_node& env, const char * msg, const char* name);

//extern void tcc_error_func_native(void* env, const char * msg);
//extern void tcc_log_symbol_func_native(void* env, const char * msg, const char* name);


/**
 * @brief Unified function to generate vanilla c code.
 * 
 * @param is_runtime to specify if the component will be run or it is being used in the visual editor
 * @param obj if nullptr it is assumed to be a module.
 * @param src source to compile
 * @param ctx context where this information is anchored (for example the xml node)
 * @param error_fn a function to show tcc errors
 * @param register_fn a function to show symbol registrations
 * @param link_with path of a library to be linked against (exposing the standard vs interface)
 * @return std::shared_ptr<tcc> 
 */
extern std::shared_ptr<tcc> tcc_c_pipeline(global_ctx_t& globals, bool is_runtime, ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), bool compact, link_with_t link_with);
extern std::shared_ptr<smap<symbol_t>> tcc_c_pipeline_apply(const std::shared_ptr<tcc>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*));

inline std::shared_ptr<tcc> tcc_c_pipeline_xml(global_ctx_t& globals, bool is_runtime, vs::ui_base* obj, pugi::xml_node& ctx, bool compact, link_with_t link_with){
    return tcc_c_pipeline(globals,is_runtime,obj,ctx.text().as_string(),&ctx,(void(*)(void*,const char*))tcc_error_func_xml, compact, link_with);
}

}
}

#endif