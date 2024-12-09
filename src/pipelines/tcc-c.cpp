#include "globals.hpp"
#include "ui-frame.hpp"
#include "ui.hpp"
#include "utils/paths.hpp"
#include <cstddef>

#include <iostream>
#include <pipelines/tcc-c.hpp>
#include <ui-tree.xml.hpp>


namespace vs{
namespace pipelines{ 

void tcc_error_func_xml(const pugi::xml_node& ctx, const char * msg) {
    printf("\n\033[41;37;1m[TCC]\033[0m      : %s @ [\033[93;3m%s\033[0m]", msg,ctx.path().c_str());
    fflush(stdout);

}

void tcc_log_symbol_func_xml(const pugi::xml_node& ctx, const char * msg, const char * name) {
    ui_tree_xml::log(severety_t::INFO, ctx , msg, name);
}

//'/home/checkroom/Documents/projects/vs-fltk/subprojects/libtcc/tcc'  test.c  -I../../subprojects/libtcc/include/ -L.  -L../../subprojects/libtcc -lapp 

static void vs_debug(const char* k, const char* v){globals::debug(k,v);}

#define LIB(x)  script->add_sym(#x, (void*) x)
#define LIBT(x,t)  script->add_sym(#x, (void*) t x)

std::shared_ptr<tcc> tcc_c_pipeline(bool is_runtime, vs::ui_base* obj, const char* src, void* ctx, void(*error_fn)(void*,const char*), bool compact, const char *link_with){
    auto script = std::make_shared<tcc>();

    //This part is a bit of a mess.
    //Without libc.so and libtcc1.a (not sure which) dynamic linking is not working as expected. I would really like to remove the libc dependency.

    script->set_error_fn(ctx,error_fn);
    script->set_opts("-nostdlib"); //-fno-builtin

    //TODO: 
    //I need to provide from the outside as configuration:
    //- All directories resolving paths for libraries
    //- The path where libtcc.so is located
    //- The location where it s headers are placed.
    //- The path for bindings shall be computed as absolute based on VS_SHARE or whatever it is called.

    //script->add_lib_path("/usr/lib/x86_64-linux-gnu/"); //I dont' want to hardcode this one.
    //script->add_lib_path("./subprojects/libtcc");

    script->set_out_type(tcc::memory);

    script->add_sysinclude_path((globals::path_env.root.location+"./bindings/native/tcc/include").c_str());
    script->add_include_path((globals::path_env.root.location+"./bindings/native/include").c_str());
    
    //script->add_lib("ld");
    //script->add_lib("tcc1");
    //script->add_lib("c");


    /*if(link_with!=nullptr){
        //TODO: Split in the last "/" position. Left is path for include and libraries to link, the remaining tag is the name it has.
        std::string link_header=(std::string(link_with));
        link_header.append(".h");
        std::string link_lib=(std::string(link_with));
        script->add_lib_path("/home/checkroom/Documents/projects/vs-fltk/examples/native-app/");
        script->add_include_path("/home/checkroom/Documents/projects/vs-fltk/examples/native-app/");
        script->add_lib("app");
    }*/

    // Custom symbol
    //script->add_sym("vs_self", (void *)obj==0?(void*)-1:obj);  //Needed as obj nullptr would remove the symbol for some stupid reason.
    script->add_sym("vs_debug", (void *)vs_debug);
    script->add_sym("vs_log", (void *)vs_log);
    script->add_sym("vs_resolve_name", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name(s, true); });
    script->add_sym("vs_resolve_name_path", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return (const ui_base*)nullptr;return  w->resolve_name_path(s, true); });
    script->add_sym("vs_resolve_symbol", (void *)+[](ui_base* w,const char* s){if(w==nullptr)return symbol_ret_t {symbol_t::VOID, symbol_t::VOID, nullptr};return w->resolve_symbol(s, true); });
    script->add_sym("vs_apply_prop", (void *)+[](ui_base* w,const char* k, const char* v){if(w==nullptr)return -1;return w->apply_prop(k,v); });
    script->add_sym("vs_get_computed", (void *)+[](ui_base* w,const char* k, const char** v){if(w==nullptr)return -1;return w->get_computed(k,v); });
    //TODO: remove tmp strings
    script->add_sym("vs_set", (void *)+[](ui_base* w,const char* k, const void* v){if(w==nullptr)return -1;std::string tmp = std::string("#s_")+k;return w->use_setter(w->resolve_symbol_local(tmp.c_str(), false), v);});
    script->add_sym("vs_get", (void *)+[](ui_base* w,const char* k, void** v){if(w==nullptr)return -1;std::string tmp = std::string("#g_")+k;return w->use_getter(w->resolve_symbol_local(tmp.c_str(), false), v);});
    
    LIB(snprintf);
    // Fragments of stdlib
    // --- cstring
    LIB(memcpy);
    LIB(memcpy);
    LIB(memmove);
    LIB(memset);
    LIB(strcpy);
    LIB(strncpy);
    LIB(strcat);
    LIB(strncat);
    LIB(memcmp);
    LIB(strcmp);

    LIB(strcoll);
    LIB(strncmp);
    LIB(strxfrm);

    LIBT(memchr,(void*(*)(void*, int, size_t)));
    LIBT(strchr,(char*(*)(char*, int)));
    LIB(strcspn);
    LIBT(strpbrk,(char*(*)(char*, const char* )));
    LIBT(strrchr,(char*(*)(char*, int)));
    LIB(strspn);
    LIBT(strstr,(char*(*)(char*, const char*)));
    LIB(strtok);

    LIB(memset);
    LIB(strerror);
    LIB(strlen);

    // --- cstdlib
    //https://cplusplus.com/reference/cstdlib/
    LIB(calloc);
    LIB(free);
    LIB(malloc);
    LIB(realloc);

    script->add_file("./bindings/native/src/commons.c");
    script->add_file("./bindings/native/src/stub.c");

    if(compact){
        script->compile_str_embedded(
            "#include <vs.h>\n#include <stub.h>\n//#file embedded \nvoid callback(){\n#line 0\n", //TODO: Add custom header if linked with an external thing
            src,
            "\n}\n"
        );
    }
    else{
        script->compile_str_embedded(
            "#include <vs.h>\n#include <stub.h>\n//#file embedded \n#line 0\n", //TODO: Add custom header if linked with an external thing
            src,
            ""
        );
    }

    //TODO Tcc error handling if compile fails to show error but fail to generate the tcc smart pointer. I need to flush cout :/
    script->relocate();

    auto on_compiled = (uint64_t(*)())script->get_sym("on_compiled");
    if(on_compiled!=nullptr)on_compiled();

    auto on_static_test = (int(*)())script->get_sym("static_test");
    if(on_static_test!=nullptr && globals::env.computed_policies.testing)on_static_test();

    if(obj!=nullptr){
        //Apply the environment for single use scripts.
        auto vs_set_env = (void(*)(void*))script->get_sym("vs_set_env");
        vs_set_env(obj);
    }

    return script;
}

std::shared_ptr<smap<symbol_t>>  tcc_c_pipeline_apply(const std::shared_ptr<tcc>& script,vs::ui_base* obj,void* ctx,void(*register_fn)(void*,const char*, const char*)){
    std::shared_ptr<smap<symbol_t>> symbols = std::make_shared<smap<symbol_t>>();
    
    struct callback_ctx_t{
        smap<symbol_t>& symbols;
        void* ref;
        void(*log)(void*,const char*, const char*);
    }cb_ctx{.symbols=*symbols, .ref=ctx, .log=register_fn};

    script->ls_sym(&cb_ctx, +[](void* _ctx, const char* name, const void* value){
        callback_ctx_t* ctx = (callback_ctx_t*)_ctx;

        if(strcmp("callback", name)==0){
        ctx->log(ctx->ref,"Registering default callback symbol `%s`",name);
        ctx->symbols.emplace(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,value});
        }else if(strcmp("draw", name)==0){
        ctx->log(ctx->ref,"Registering default drawing symbol `%s`",name);
        ctx->symbols.emplace(name, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::DRAW,value});
        }else if(strcmp("dispatcher", name)==0){
        ctx->log(ctx->ref,"Registering default dispatching symbol `%s`",name);
        }else if(strcmp("vs_set_env",name)==0){
        ctx->symbols.emplace("#set_env", symbol_t{symbol_mode_t::NATIVE,symbol_type_t::UNKNOWN,value});
        }else if(strncmp("__EXPORT_CB__", name, 13)==0){
        ctx->log(ctx->ref,"Registering public callback symbol `%s`",name+13);
        ctx->symbols.emplace(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::CALLBACK,(const void*)(((size_t*)value)[0])});
        }/*else if(strncmp("__EXPORT_GET_", name, 13)==0){
        if(((size_t*)value)[0]!=0){
            ctx->log(ctx->ref,"Registering public getter symbol `%s`",name+13);
            std::string tmp = std::string("#g_") + (name+13);   //TODO: Avoid temp string
            ctx->symbols.emplace(tmp, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)(((size_t*)value)[0])});
        }
        }else if(strncmp("__EXPORT_SET_", name, 13)==0){
        if(((size_t*)value)[0]!=0){
            std::string tmp = std::string("#s_") + (name+13);   //TODO: Avoid temp string
            ctx->log(ctx->ref,"Registering public setter symbol `%s`",name+13);
            ctx->symbols.emplace(tmp, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::SETTER,(const void*)(((size_t*)value)[0])});
        }
        }*/
        else if(strncmp("__EXPORT_FIELD_", name, 15)==0){
        if(((size_t*)value)[0]!=0){
            struct vs_field_t{
                const char* name;
                int(*setter)(const char*);
                int(*getter)(char**);
            };
            vs_field_t* fvalue = (vs_field_t* )value;
            
            std::string stmp = std::string("#s_") + (fvalue->name);   //TODO: Avoid temp string
            std::string gtmp = std::string("#g_") + (fvalue->name);   //TODO: Avoid temp string
            ctx->log(ctx->ref,"Registering public field `%s`",fvalue->name);
            ctx->symbols.emplace(stmp, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::SETTER,(const void*)fvalue->setter});
            ctx->symbols.emplace(gtmp, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::GETTER,(const void*)fvalue->getter});
        }
        }else if(strncmp("__EXPORT_UKN_", name, 13)==0){
        ctx->log(ctx->ref,"Registering public unknown symbol `%s`",name+13);
        ctx->symbols.emplace(name+13, symbol_t{symbol_mode_t::NATIVE,symbol_type_t::UNKNOWN,(const void*)(((size_t*)value)[0])});
        }
    });
    return symbols;
}

#undef LIB
#undef LIBT

}
}