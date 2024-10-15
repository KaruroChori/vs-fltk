#include "ui-tree.xml.hpp"
#include <loader.hpp>

namespace vs{
app_loader::app_loader(const char* path){
  root=new ui_xml_tree();
  if(root->load(path,true)!=0){
    throw "Unable to process file";
  }
  else{
    root->build();
    //vs::theme_cute::apply();
  }
}
int app_loader::run(){
    return Fl::run();
}

    app_loader::~app_loader(){delete root;}

}