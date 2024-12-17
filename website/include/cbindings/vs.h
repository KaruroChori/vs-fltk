#pragma once
/**
 * @file vs.h
 * @author karurochari
 * @brief Entrypoint for the C interface of `libvs`
 * @details `libvs` is meant to be used via its C++ interface, and only a subset 
 *          of its functionality is exposed via these bindings.  
 *          Basically, what is needed to handle `ui_tree`s, access to the constrained 
 *          interface ui widgets have, resolving paths, symbols and little more.
 * @copyright Copyright (c) 2024
 * 
 */

/*
    vs-fltk is a library meant to be used with its C++ interface.  
    Only a subset of its functionality is exposed with proper C bindings.  
    Basically, what is needed to compile XML components into C, run them, 
    exposing the limited runtime DOM-like structure, resolving paths and little more.
*/

#include <stddef.h>
#ifdef __cplusplus
extern "C"{
#endif

typedef void vs_frame;
typedef void vs_node;   //ui_base
typedef void vs_opaque_script;

enum vs_symbol_type_t{
  VS_SYMBOL_TYPE_VOID, 
  VS_SYMBOL_TYPE_UNKNOWN, 
  VS_SYMBOL_TYPE_CALLBACK, 
  VS_SYMBOL_TYPE_DRAW, 
  VS_SYMBOL_TYPE_SETTER, 
  VS_SYMBOL_TYPE_GETTER, 
  VS_SYMBOL_TYPE_DISPATCHER,
  VS_SYMBOL_TYPE_TEST
};
typedef enum vs_symbol_type_t vs_symbol_type_t;

enum vs_symbol_mode_t{
  VS_SYMBOL_MODE_AUTO, 
  VS_SYMBOL_MODE_NATIVE, 
  VS_SYMBOL_MODE_QUICKJS, 
  VS_SYMBOL_MODE_WASM,
  VS_SYMBOL_MODE_EXTERNAL,
  VS_SYMBOL_MODE_LUA,
  VS_SYMBOL_MODE_RISCV
};
typedef enum vs_symbol_mode_t vs_symbol_mode_t;


struct vs_symbol_t{
  enum vs_symbol_mode_t mode:3;
  enum vs_symbol_type_t type:5;
  const void* symbol;
};
typedef struct vs_symbol_t vs_symbol_t;

//TODO: Very bad packing. Fix it at some point
struct vs_symbol_ret_t{
  struct vs_symbol_t symbol;
  struct vs_symbol_t ctx_apply;
  const vs_frame* found_at;
};
typedef struct vs_symbol_ret_t vs_symbol_ret_t;

extern vs_symbol_t vs_symbol_null;

enum vs_frame_type_t{
  VS_FRAME_TYPE_SLOT_CONTAINER,   //Stop any request from moving in between.
  VS_FRAME_TYPE_CONTAINER,        //Any bottom up request will stop here.
  VS_FRAME_TYPE_SLOT,             //Any top down request will stop here.
  VS_FRAME_TYPE_LEAF,             //No children stored in here (but symbols are fine). Go to parent.
  VS_FRAME_TYPE_NODE,             //A namespace without constraints on moving in and out
};
typedef enum vs_frame_type_t vs_frame_type_t;

enum vs_frame_access_t{
  VS_NODE_PUBLIC,
  VS_NODE_PRIVATE,
};
typedef enum vs_frame_access_t vs_frame_access_t;

enum vs_component_t{
    VS_COMPONENT_NONE,
    VS_COMPONENT_VS,
    VS_COMPONENT_XML,
    VS_COMPONENT_LIB,
    VS_COMPONENT_WASM,
    VS_COMPONENT_RISCV,
    VS_COMPONENT_CNATIVE,
    VS_COMPONENT_MARKDOWN,
};
typedef enum vs_component_t vs_component_t;

typedef vs_symbol_mode_t vs_frame_mode_t;

#if __has_include("./components.autogen.h")
    #include "./components.autogen.h"
#endif

void vs_hello_world();
void vs_debug(const char* k, const char* v);


void vs_node_name_set(vs_node*, const char*);
void vs_node_frame_mode_set(vs_node*, vs_frame_mode_t);
void vs_node_frame_type_set(vs_node*, vs_frame_type_t);
void vs_node_frame_access_set(vs_node*, vs_frame_access_t);

void vs_node_frame_new(vs_node*, const char* name, vs_frame_mode_t mode);
const char* vs_node_class_get(const vs_node*);
const char* vs_node_name_get(const vs_node*);

void vs_node_getter_use(const vs_symbol_ret_t* sym, void ** value);
void vs_node_setter_use(const vs_symbol_ret_t* sym, void * value);
void vs_node_callback_use(const vs_symbol_ret_t* sym, vs_node * value);
void vs_node_test_use(const vs_symbol_ret_t* sym);

/*
void vs_node_localscript_attach(const vs_opaque_script* ref);
void vs_node_modulecript_attach(const vs_opaque_script* ref);

//Resolve the frame, always looking at the parent and not self.
frame* resolve_frame()  const;
//Resolve the namespace, basically the first parent which is not "transparent".
frame* resolve_namespace() const;
symbol_ret_t resolve_symbol(const char* str) const;
const ui_base* resolve_name(const char * str) const;
const ui_base* resolve_name_path(const char * str) const;

virtual int get_computed(const char* prop, const char ** value) = 0;
virtual int apply_prop(const char* prop, const char * value) = 0;
void refresh_style(const char* local_mixins="");
*/

/*
extern vs_ui_base vs__ns_component_new(vs_ui_base* parent);
extern int vs__ns_component_delete();


//void register_symbol(const char* name, symbol_t value);
//symbol_t get_symbol(const char* name);
//void unregister_symbol(const char* name);
//void reset_symbols();
//void set_dispatcher(symbol_t value);


//Add mixin
void add_mixin(const char* name, const smap<std::string>& ref);

//Resolve a mixin based on its name
std::pair<const smap<std::string>*,const frame*> resolve_mixin(const char* name) const;

//Compile a single prop based on a mixin definition
const char* eval_prop(const char* prop, const char* mixins[]) const;

//Generate a mixin map based on a list of mixins
smap<std::string> compile_mixins(const char* mixins_list) const;

inline Fl_Widget& widget(){return *_widget;}
inline const Fl_Widget& widget()const{return *_widget;}

//TODO: these constraints should be relaxed to support more general reparenting.
//However this is not planned right now, and reparent_frame is just a fix to the problem of app not being a widget.
inline void reparent_frame(ui_base* newparent){
    if(local_frame!=nullptr && local_frame->parent==nullptr && newparent!=nullptr && newparent->local_frame!=nullptr){
    local_frame->parent=newparent->local_frame;
    newparent->local_frame->children.insert_or_assign(local_frame->name,local_frame);
    }
}



void path(std::stringstream& dst, bool scoped = true) const;





smap<std::string> compute_refresh_style(const char* local_mixins="");
*/

//TODO: expose xml parser 
//TODO: expose policy queries
//TODO: expose path resolver
//TODO: expose cache model
//TODO: expose script compilers/interpreters

#ifdef __cplusplus
}
#endif