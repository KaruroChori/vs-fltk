#pragma once

#include "globals.hpp"
#include "pipelines/commons.hpp"
#include "ui-frame.hpp"
#include <ui.hpp>
#include <cache/commons.hpp>

namespace vs {

struct ui_tree {
  enum class type_t{
    NONE, APP, COMPONENT, COMPONENT_THIN, FRAGMENT
  }type;

  //Define the embedded mode supported.
  frame_mode_t mode = frame_mode_t::AUTO;

  //Book-keeping. Duplicated info on nested structures, but they are short lived, and it is better compared to unrolling the stack each time.
  global_ctx_t* globals = nullptr;           
  //Book-keeping. Duplicated info on nested structures, but they are short lived, and it is better compared to unrolling the stack each time.
  pipelines::link_with_t link_with = {nullptr, nullptr};

  ui_tree* parent = nullptr;        //Set if there is an explict owner of this root, for example a viewport/app.
  ui_base* caller_ui_node=nullptr;  //Element from a parent tree calling me
  ui_base* root = nullptr;          //Base element of this tree

  policies_t policies;              //Computed policies for this tree
  scoped_rpath_t local;             //Full path for the location of this component. Redundant but used to make some operations faster.
  scoped_rpath_t fullname;          //Full path with name for the location of this component.
  size_t local_unique_counter = 0;

  // Globals
  std::map<std::string,std::string> imports;

  std::map<std::string, std::string, std::less<>> props_from_above;
  std::map<std::string, ui<> *, std::less<>> slots_from_above;

  std::vector<ui_base*> nodes;

  // Support during navigation

  //Run test propagating from the root.

  bool string2key256(const char* str, uint8_t array[256/32] );

  //TODO: It is likely  I want this stuff moved from here.
  //      Also a lot of it might end up in the codegen machine.

  virtual ~ui_tree();
  virtual void cleanup();
  virtual int runtime_testsuite();

  virtual int build() = 0;
  virtual int load(const char* file, type_t type) = 0;

  inline ui_tree(ui_tree* parent, ui_base* caller_ui_node){
    this->parent=parent;
    if(parent!=nullptr){
      link_with=parent->link_with;
      globals=parent->globals;
    }
    this->caller_ui_node=caller_ui_node;
  }

};


} // namespace vs