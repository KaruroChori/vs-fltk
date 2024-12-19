
#pragma once

#include <ui-tree.hpp>

namespace vs{

struct ui_tree_wasm : ui_tree {

  // TODO: Add pass args and slots as parameters
  static ui<Fl_Widget> *loadWASMComponent(const char *file, bool app);
};

}