#if VS_USE_QJS

#include <memory>

#include <quickjs.h>

#include <ui-frame.private.hpp>
#include <ui.hpp>
#include <ui-tree/xml.hpp>
#include <pipelines/quickjs-js.hpp>
#include <globals.hpp>

namespace vs{
namespace pipelines{ 

void qjs_error_func_xml(const pugi::xml_node& env, const char * msg) {
    printf("\n\033[41;37;1m[QJS]\033[0m      : %s @ [\033[93;3m%s\033[0m]", msg,env.path().c_str());
    fflush(stdout);
}

void qjs_log_symbol_func_xml(const pugi::xml_node& ctx, const char * msg, const char * name) {
    ui_tree_xml::log(severety_t::INFO, ctx , msg, name);
}


/*Cannot handle additional args. Use string interpolation from JS*/
static JSValue js_log(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<3){
        return JS_UNDEFINED;     
    }
    if(JS_IsNumber(argv[0]) && JS_IsBigInt(ctx,argv[1]) && JS_IsString(argv[2])){
        uint32_t severety;
        uint64_t node;
        const char* str;
        JS_ToUint32(ctx, &severety, argv[0]);
        JS_ToBigUint64(ctx, &node, argv[1]);
        str = JS_ToCString(ctx, argv[2]);

        vs_log(severety,(const ui_base*)node,str);

        JS_FreeCString(ctx, str);
    }
    return JS_UNDEFINED;
}

static JSValue js_resolve_name_path(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<2){
        return JS_UNDEFINED;
    }
    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1])){
        uint64_t node;
        const char* str;
        JS_ToBigUint64(ctx, &node, argv[0]);
        str = JS_ToCString(ctx, argv[1]);
        auto t = JS_NewBigUint64(ctx,(uint64_t)((ui_base*)node)->resolve_name_path(str,true));
        JS_FreeCString(ctx, str);
        return t;

    }

    return JS_UNDEFINED;
}

static JSValue js_apply_prop(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<3){
        return JS_UNDEFINED;
    }

    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1]) && JS_IsString(argv[2])){
        uint64_t node;
        const char* key;
        const char* value;

        JS_ToBigUint64(ctx, &node, argv[0]);
        key = JS_ToCString(ctx, argv[1]);
        value = JS_ToCString(ctx, argv[2]);

        auto t = JS_NewInt32(ctx,(uint64_t)((ui_base*)node)->apply_prop(key,value));
        JS_FreeCString(ctx, key);
        JS_FreeCString(ctx, value);
        return t;

    }
    return JS_UNDEFINED;
}

static JSValue js_get_computed(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    if(argc<2){
        return JS_UNDEFINED;
    }

    if(JS_IsBigInt(ctx,argv[0]) && JS_IsString(argv[1]) && JS_IsString(argv[2])){
        uint64_t node;
        const char* key;
        const char* value;

        JS_ToBigUint64(ctx, &node, argv[0]);
        key = JS_ToCString(ctx, argv[1]);

        ((ui_base*)node)->get_computed(key,&value);
        auto t = JS_NewString(ctx,value);
        JS_FreeCString(ctx, key);
        JS_FreeCString(ctx, value);
        delete[] value;
        return t;

    }
    return JS_UNDEFINED;
}

static JSValue js_fake(JSContext * ctx , JSValueConst this_val, int argc, JSValueConst* argv){
    return JS_UNDEFINED;
}

/**
 * @brief Append hooks on the ctx
 * 
 * @param ctx 
 * @param self 
 * @param is_runtime if true real hooks are added; false for test mode (like within the editor)
 */
static void init_c_hooks(JSContext* ctx, void* self, bool is_runtime) {
    JSValue global_obj = JS_GetGlobalObject(ctx);

    JSValue core_obj = JS_NewObject(ctx);

    JS_SetPropertyStr(ctx, core_obj, "self", JS_NewBigUint64(ctx,(size_t)self));
    if(is_runtime){
        JS_SetPropertyStr(ctx, core_obj, "log", JS_NewCFunction(ctx, js_log, "log", 3));
        JS_SetPropertyStr(ctx, core_obj, "resolve_name_path", JS_NewCFunction(ctx, js_resolve_name_path, "resolve_name_path", 2));
        JS_SetPropertyStr(ctx, core_obj, "apply_prop", JS_NewCFunction(ctx, js_apply_prop, "apply_prop", 3));
        JS_SetPropertyStr(ctx, core_obj, "get_computed", JS_NewCFunction(ctx, js_get_computed, "get_computed", 2));
        //JS_SetPropertyStr(ctx, core_obj, "call", core__call);
        //JS_SetPropertyStr(ctx, core_obj, "set", core__set);
        //JS_SetPropertyStr(ctx, core_obj, "get", core__get);
    }
    else{
        JS_SetPropertyStr(ctx, core_obj, "log", JS_NewCFunction(ctx, js_fake, "log", 3));
        JS_SetPropertyStr(ctx, core_obj, "resolve_name_path", JS_NewCFunction(ctx, js_fake, "resolve_name_path", 2));
        JS_SetPropertyStr(ctx, core_obj, "apply_prop", JS_NewCFunction(ctx, js_fake, "apply_prop", 3));
        JS_SetPropertyStr(ctx, core_obj, "get_computed", JS_NewCFunction(ctx, js_fake, "get_computed", 2));
        //JS_SetPropertyStr(ctx, core_obj, "call", core__call);
        //JS_SetPropertyStr(ctx, core_obj, "set", core__set);
        //JS_SetPropertyStr(ctx, core_obj, "get", core__get);
    }


    JS_SetPropertyStr(ctx, global_obj, "core", core_obj);


    //JSValue console_obj = JS_NewObject(ctx);
    //JS_SetPropertyStr(ctx, console_obj, "log",JS_NewCFunction(ctx, js_print_to_console, "log", 1));
    //JS_SetPropertyStr(ctx, global_obj, "console", console_obj);

    JS_FreeValue(ctx, global_obj);
}   


static void dump_value_to_stream(JSContext* ctx, JSValueConst val, void(*error_fn)(void*,const char*), void* node) {
    const char* strval = JS_ToCString(ctx, val);
    if (strval) {
        error_fn(node,strval);
        JS_FreeCString(ctx, strval);
    } else {
        error_fn(node,"[exception]");
    }
}


quickjs_t::quickjs_t(JSRuntime* rt){
    ctx=JS_NewContextRaw(rt);
    JS_AddIntrinsicBaseObjects(ctx);
    JS_AddIntrinsicDate(ctx);
    JS_AddIntrinsicEval(ctx);
    JS_AddIntrinsicRegExp(ctx);
    JS_AddIntrinsicJSON(ctx);
    JS_AddIntrinsicProxy(ctx);
    JS_AddIntrinsicMapSet(ctx);
    JS_AddIntrinsicTypedArrays(ctx);
    JS_AddIntrinsicPromise(ctx);
    JS_AddIntrinsicBigInt(ctx);
    JS_AddIntrinsicWeakRef(ctx);

    JS_AddPerformance(ctx);
}


std::shared_ptr<smap<symbol_t>> qjs_js_pipeline_apply(const std::shared_ptr<quickjs_t>& script,vs::ui_base* obj,void* node,void(*register_fn)(void*,const char*, const char*)){
    std::shared_ptr<smap<symbol_t>> symbols = std::make_shared<smap<symbol_t>>();

    size_t count = 1;   //Start from 1 due to alignment in the vector, to keep 0 as special case interpreted as nullptr.
    for(auto& i : script->handles){
        auto* name = std::get<0>(i).c_str();
        if(strcmp("callback", name)==0){
            register_fn(node, "Registering default callback symbol `%s`",name);
            symbols->emplace(name,symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::CALLBACK,(void*)count});
        }
        else if(strcmp("draw", name)==0){
            register_fn(node, "Registering default drawing symbol `%s`",name);
            symbols->emplace(name,symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::DRAW,(void*)count});
        }
        else if(strcmp("dispatcher", name)==0){
            register_fn(node,  "Registering default dispatching symbol `%s`",name);
            symbols->emplace(name, symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::DISPATCHER,(void*)count});
        }
        else if(strcmp("vs_set_env", name)==0){
            symbols->emplace("#set_env", symbol_t{symbol_mode_t::QUICKJS,symbol_type_t::UNKNOWN,(void*)count});
        }
        else{
            //TODO use the type to show it back again
            register_fn(node,  "Registering symbol `%s`",name);
            symbols->emplace(name,symbol_t{symbol_mode_t::QUICKJS,std::get<1>(i),(void*)count});
        }
        count++;
    }
    
    return symbols;
}


std::shared_ptr<quickjs_t> qjs_js_pipeline(global_ctx_t& globals, bool is_runtime, vs::ui_base* obj, const char* src, void* nctx, void(*error_fn)(void*,const char*), link_with_t link_with){
    //std::shared_ptr<quickjs_t> _ctx = std::make_shared<quickjs_t>((JSRuntime*)(globals::js_rt()));
    std::shared_ptr<quickjs_t> _ctx = std::shared_ptr<quickjs_t>( new quickjs_t((JSRuntime*)(globals.js_rt())), +[](void* o){delete (quickjs_t*)o;});


    auto& ctx = *_ctx;

    if (!ctx) {
        error_fn(nctx, "Unable to create a new js context");
        return nullptr;
    }

    //Decide if I want to include stuff from the js stdlib

    init_c_hooks(ctx, obj, is_runtime);

    const char prefix[] = {
#embed "../../bindings/quickjs/vs.js" suffix(, 0)      
    };

    //Parse prefix script/library
    {
        //Size requires the -1 to avoid counting the last 0 character.
        auto result = JS_Eval(ctx, prefix, sizeof(prefix)-1, "<input>", JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result)) {
            JSValue exception = JS_GetException(ctx);
            bool is_error = JS_IsError(ctx, exception);
            dump_value_to_stream(ctx, exception, error_fn, nctx);
            if (is_error) {
                JSValue stack = JS_GetPropertyStr(ctx, exception, "stack");
                if (!JS_IsUndefined(stack)) {
                    dump_value_to_stream(ctx, exception, error_fn, nctx);
                }
                JS_FreeValue(ctx, stack);
            }
            
            JS_FreeValue(ctx, exception);
        }
        JS_FreeValue(ctx,result);
    }

    //Process custom script
    {
        auto result = JS_Eval(ctx, src, strlen(src), "<input>", JS_EVAL_TYPE_GLOBAL);
        if (JS_IsException(result)) {
            JSValue exception = JS_GetException(ctx);
            bool is_error = JS_IsError(ctx, exception);
            dump_value_to_stream(ctx, exception, error_fn, nctx);
            if (is_error) {
                JSValue stack = JS_GetPropertyStr(ctx, exception, "stack");
                if (!JS_IsUndefined(stack)) {
                    dump_value_to_stream(ctx, exception, error_fn, nctx);
                }
                JS_FreeValue(ctx, stack);
            }
            
            JS_FreeValue(ctx, exception);
        }
        JS_FreeValue(ctx,result);
    }

    JSPropertyEnum* list = nullptr;
    uint32_t list_items;
    auto globalThis = JS_GetGlobalObject(ctx);

    JS_GetOwnPropertyNames(ctx,&list,&list_items, globalThis,JS_GPN_STRING_MASK|JS_GPN_SYMBOL_MASK|JS_GPN_PRIVATE_MASK);

    //This first cycle is only performed to create references to be later garbage collected.
    for(int i=0;i<list_items;i++){
        auto name = JS_AtomToCString(ctx, list[i].atom);

        if(strcmp("callback", name)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name,symbol_type_t::CALLBACK,ref});
        }
        else if(strcmp("draw", name)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name,symbol_type_t::DRAW,ref});
        }
        else if(strcmp("dispatcher", name)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name,symbol_type_t::DISPATCHER,ref});
        }
        else if(strncmp("__EXPORT_CB__", name, 13)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name+13,symbol_type_t::CALLBACK,ref});
        }
        else if(strncmp("__EXPORT_GET__", name, 13)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name+13,symbol_type_t::GETTER,ref});
        }
        else if(strncmp("__EXPORT_SET__", name, 13)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name+13,symbol_type_t::SETTER,ref});
        }
        else if(strncmp("__EXPORT_UKN__", name, 13)==0){
            auto ref = JS_GetPropertyStr(ctx, globalThis, name);
            ctx.handles.push_back({name+13,symbol_type_t::UNKNOWN,ref});
        }
        JS_FreeCString(ctx, name);
    }

    JS_FreeValue(ctx, globalThis);
    JS_FreePropertyEnum(ctx, list, list_items);

    return _ctx;
}



}
}

#endif