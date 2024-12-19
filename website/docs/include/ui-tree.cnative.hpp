
#pragma once

#include <ui-tree.hpp>

namespace vs{

struct ui_tree_cnative : ui_tree {

  // TODO: Add pass args and slots as parameters
  static ui<Fl_Widget> *loadCNativeComponent(const char *file, bool app);
};

}