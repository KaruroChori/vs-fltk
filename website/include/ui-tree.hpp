#pragma once

#include "globals.hpp"
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

  global_ctx_t& globals;            //Just for book-keeping

  ui_tree* parent = nullptr;        //Set if there is an explict owner of this root, for example a viewport/app.
  ui_base* caller_ui_node=nullptr;  //Element from a parent tree calling me
  ui_base* root = nullptr;          //Base element of this tree

  policies_t policies;              //Computed policies for this tree
  scoped_rpath_t local;             //Full path for the location of this component.
  scoped_rpath_t fullname;          //Full path for the location of this component.
  size_t local_unique_counter = 0;

  // Globals
  std::string basename;
  std::map<std::string, std::string, std::less<>> modules;
  // TODO Add policies
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

  inline ui_tree(global_ctx_t& g, ui_tree* parent, ui_base* caller_ui_node):globals(g){this->parent=parent;this->caller_ui_node=caller_ui_node;}

};


} // namespace vs