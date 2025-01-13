#pragma once

#if VS_USE_TCC
/*
  Utility library to simplify the usage of libtcc
*/

#include <libtcc.h>

#include <memory>
#include <optional>

class tcc{
  using error_fn = TCCErrorFunc;

  TCCState* ctx = nullptr;

  public:
    inline tcc(){
      ctx=tcc_new();
    }

    inline tcc(TCCState* ctx){
      this->ctx=ctx;
    }

    inline bool has_ctx() const {return ctx!=nullptr;}
    inline void new_ctx() {if(ctx!=nullptr)tcc_delete(ctx);ctx=tcc_new();}

    inline ~tcc(){
      if(ctx!=nullptr)tcc_delete(ctx);
    }

    enum out_type{
      memory=1, //6h wasted because of that 1
      exe,
      dll,
      obj,
      preprocesss
    };

    inline void set_lib_path(const char* path)noexcept{tcc_set_lib_path(ctx, path);}
    inline void set_error_fn(void* error_opaque, error_fn* error_func)noexcept{tcc_set_error_func(ctx, error_opaque, error_func);}
    inline void set_opts(const char* str) noexcept{tcc_set_options(ctx, str);}

    //Preprocessor
    inline int add_include_path(const char* path)noexcept{return tcc_add_include_path(ctx, path);}
    inline int add_sysinclude_path(const char* path)noexcept{return tcc_add_sysinclude_path(ctx, path);}
    inline void def_preprocessor_sym(const char* sym, const char* value)noexcept{tcc_define_symbol(ctx, sym, value);}
    inline void undef_preprocessor_sym(const char* sym)noexcept{tcc_undefine_symbol(ctx,sym);}

    //Compiling
    inline int add_file(const char* path)noexcept{return tcc_add_file(ctx, path);}
    inline int compile_str(const char* code)noexcept{return tcc_compile_string(ctx, code);}

    inline int compile_str_embedded(const char* head, const char * body, const char * footer)noexcept{ return tcc_compile_string_wrapped(ctx, head,body,footer);}

    //Linking
    inline int set_out_type(out_type type){return tcc_set_output_type(ctx, type);}
    inline int add_lib_path(const char* path)noexcept{return tcc_add_library_path(ctx, path);}
    inline int add_lib(const char* path)noexcept{return tcc_add_library(ctx, path);}

    inline int add_sym(const char* sym, const void* value)noexcept{return tcc_add_symbol(ctx, sym, value);}
    inline void* get_sym(const char* name)noexcept{return tcc_get_symbol(ctx, name);}
    inline void ls_sym(void* c, void (*symbol_cb)(void *c, const char *name, const void *val))const noexcept{return tcc_list_symbols(ctx, c, symbol_cb);}

    inline int out(const char* filename)noexcept{return tcc_output_file(ctx, filename);}
    inline int run(int argc=0, char **argv=nullptr)noexcept{return tcc_run(ctx,argc,argv);}
    inline int relocate()noexcept{return tcc_relocate(ctx);}

    /*
    LIBTCCAPI void *_tcc_setjmp(TCCState *s1, void *jmp_buf, void *top_func, void *longjmp);
    #define tcc_setjmp(s1,jb,f) setjmp(_tcc_setjmp(s1, jb, f, longjmp))

    typedef int TCCBtFunc(void *udata, void *pc, const char *file, int line, const char* func, const char *msg);
    LIBTCCAPI void tcc_set_backtrace_func(TCCState *s1, void* userdata, TCCBtFunc*);
    */



    /*static std::optional<std::unique_ptr<tcc>> gen_component(const char* source){
        std::unique_ptr<tcc> compileA{new tcc()};
        compileA->set_opts("-nostdlib");
        compileA->add_sysinclude_path("./subprojects/libtcc/include/");
        compileA->set_out_type(tcc::memory);
        if(compileA->compile_str(source)!=0)return {};
        compileA->relocate();
        return std::move(compileA);
    }*/
};

#endif