#include <cstring>
#include <ui-tree.hpp>

namespace vs{
  
  bool ui_tree::string2key256(const char* str, uint8_t array[256/32]){
    memset(array,0,256/8);
    return false;
  }

  void ui_tree::cleanup(){}
  int ui_tree::runtime_testsuite(){return 0;}


  ui_tree::~ui_tree(){
    for(auto i : nodes){
      delete i;
    }
    Fl::wait();
  }

}