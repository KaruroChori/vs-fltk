#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

namespace vs{


class frame;
class ui_base;

enum class symbol_type_t{
  VOID, //Function not to be defined
  UNKNOWN, //Function whose prototype is unknown/non-standard
  CALLBACK, //Event callback
  DRAW, //Drawing function
  SETTER, 
  GETTER,
  DISPATCHER, //Custom dispatcher
  TEST  //Test function used by robots, run during testing
};

enum class symbol_mode_t{
  AUTO,
  NATIVE,
  QUICKJS,
  WASM,
  EXTERNAL,
  LUA,
  RISCV,
};

struct symbol_t{
  symbol_mode_t mode:3;
  symbol_type_t type:5;
  const void* symbol;

  static symbol_t VOID;
};

//TODO: Very bad packing. Fix it at some point
struct symbol_ret_t{
  symbol_t symbol;
  symbol_t ctx_apply;
  const frame* found_at;

  typedef ui_base*(*ctx_apply_fn)(ui_base*);        //Special type for the function setting context
  typedef void(*cb_fn)(ui_base*);                   //Type for callbacks
  typedef void(*draw_fn)();                         //Type for draw functions, used to define style of UI elements (TODO interface)
  typedef int(*set_fn)(const void* src);            //Field setter
  typedef int(*get_fn)(void** src);                 //Field getter
  typedef int(*test_fn)();                          //Test function for automatic testing
};

enum class frame_type_t{
  SLOT_CONTAINER,   //Stop any request from moving in between.
  CONTAINER,        //Any bottom up request will stop here.
  SLOT,             //Any top down request will stop here.
  LEAF,             //No children stored in here (but symbols are fine). Go to parent.
  NODE,             //A namespace without constraints on moving in and out
};
enum class frame_access_t{
  PUBLIC,           //Symbols can be searched inside, bottom up & top down.
  PRIVATE,          //There can be symbols, but they are only visible by self. Requests are passed
};

typedef  symbol_mode_t frame_mode_t;


}