
#pragma once

#include <ui-tree.hpp>

namespace vs{

struct ui_tree_dylib : ui_tree {

  // TODO: Add pass args and slots as parameters
  static ui<Fl_Widget> *loadSOComponent(const char *file, bool app);
};

}