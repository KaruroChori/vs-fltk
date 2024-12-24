#pragma once
/**
 * @file vs.h 
 * @author karurochari (public@karurochari.com)
 * @brief Main interface of bindings between c-native scripts and vs
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <stdarg.h>

typedef  void* node_t;

extern node_t vs_self;

typedef enum log_severety_t{
  LOG_INFO,
  LOG_OK,
  LOG_WARNING,
  LOG_CONTINUE,
  LOG_PANIC,
  LOG_LOG,
  LOG_LVL_NORMAL = 0x00,
  LOG_LVL_SILENT = 0x10,
  LOG_LVL_VERBOSE = 0x20,
  LOG_LVL_DEBUG = 0x40,
}log_severety_t;

typedef struct __symbol_t{
  enum{
    UNKNOWN=1, CALLBACK, DRAW, SETTER, GETTER,
  } mode:3;
  enum{
    NATIVE=1, QUICKJS, WASM, EXTERNAL, LUA
  } type:5;
  const void* symbol;
}__symbol_t;


typedef struct symbol_ret_t{
  __symbol_t symbol;
  __symbol_t ctx_apply;
  const void* found_at;
}symbol_ret_t;


typedef enum vs_types_t{
  TYPE_FLAG=1,
  TYPE_ENUM,
  TYPE_RAW,
  TYPE_PATH,
  TYPE_CSTRING,
  TYPE_STRING_VIEW,
  TYPE_COLOR,
  TYPE_ISCALAR_1,
  TYPE_ISCALAR_2,
  TYPE_ISCALAR_3,
  TYPE_ISCALAR_4,
  TYPE_FSCALAR_1,
  TYPE_FSCALAR_2,
  TYPE_FSCALAR_3,
  TYPE_FSCALAR_4
} vs_types_t;


typedef struct vs_field_model_t{
  const char* name;
  int(*setter)(const char*);
  int(*getter)(const char**);
} vs_field_model_t;


typedef struct vs_field_t{
  
} vs_field_t;

inline int as_flag(vs_field_t* field){
  //TODO: throw() if runtime type not equal to flag
  //return payload reference
}

extern int vs_log(int severety, node_t self, const char* string, ...);
#define $$log(self,sev,string, ...) vs_log(sev,self,string, ##__VA_ARGS__)
#define $log(sev,string, ...) vs_log(sev,vs_self,string, ##__VA_ARGS__)


extern node_t vs_resolve_name(node_t, const char* string);
extern node_t vs_resolve_name_path(node_t, const char* string);
#define $$(self,string) vs_resolve_name_path(self,string)
#define $(string) vs_resolve_name_path(vs_self,string)

extern symbol_ret_t vs_resolve_symbol(node_t, const char* string);
#define $$resolve_symbol(self,string) vs_resolve_symbol(self,string)
#define $resolve_symbol(string) vs_resolve_symbol(vs_self,string)

extern int vs_apply_prop(node_t, const char* k, const char* v);
extern int vs_get_computed(node_t, const char* k, const char** v);
#define $$prop(self,key,value) vs_apply_prop(self,key,value)
#define $$computed(self,key,value) vs_get_computed(self,key,value)
#define $$mixin(self,value) vs_apply_prop(self,"mixin",value)
#define $prop(key,value) vs_apply_prop(vs_self,key,value)
#define $computed(key,value) vs_get_computed(vs_self,key,value)
#define $mixin(value) vs_apply_prop(vs_self,"mixin",value)

extern int vs_set(node_t, const char* k, const char* v);
#define $$set(self,k,v) vs_set(self,k,v)
#define $set(k,v) vs_set(vs_self,k,v)
extern int vs_get(node_t, const char* k, char** v);
#define $$get(self,k,v) vs_get(self,k,&v)
#define $get(k,v) vs_get(vs_self,k,&v)

extern void vs_debug(const char* key, const char* value);
#define $$debug(k,v) vs_debug(k,v)


#define CONCAT_(prefix, suffix) prefix##suffix
/// Concatenate `prefix, suffix` into `prefixsuffix`
#define CONCAT(prefix, suffix) CONCAT_(prefix, suffix)
#define MAKE_UNIQUE_VARIABLE_NAME(prefix) CONCAT(prefix##_, __LINE__)

//Utility functions to export symbols
#define $callback(x)  void* __EXPORT_CB__##x = x;
#define $cb(x)        void* __EXPORT_CB__##x = x;
#define $plotter(x)   void* __EXPORT_DRW_##x = x;
#define $field        vs_field_t MAKE_UNIQUE_VARIABLE_NAME(__EXPORT_FIELD_)= 
#define $fn(x)        void* __EXPORT_UKN_##x = x;

//Extra functions
char* itoa(int value, char* result, int base);
