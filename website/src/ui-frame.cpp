#include <ui-frame.private.hpp>

namespace vs{


frame::frame(const char* name, frame_mode_t mode, ui_base* ui_node, frame* parent, frame_type_t type, frame_access_t access){
  static int counter = 0;
  //TODO: replace with hash to make them not predictable. Filter out # prefixed names from any public serialization
  if(name==nullptr)this->name=std::string("#")+std::to_string(counter++); 
  else this->name=name;
  this->mode=mode;
  this->ui_node=ui_node;
  this->type=type;
  this->access=access;
  this->parent = parent;
}

frame::~frame() {
  //Not needed right now, as frames are destroyed alongside their owners from a list
  return;
  prune();
  //Not very optimized, but it is only going to happen at the end so no worries.
  for(;children.size()!=0;){
    auto i = children.begin();
    delete i->second;
  }
}

void frame::rename(const std::string& new_name){
  if(new_name==name)return;
  else if(parent!=nullptr){
    auto i = parent->children.find(name);
    if(i!=parent->children.end()){
      parent->children.erase(i);
    }
    parent->children.insert(std::pair(new_name,this));
  }
  name = new_name;
}

void frame::prune(){
  if(parent!=nullptr){
    parent->children.erase(parent->children.find(name));
    parent=nullptr;
  }
}

frame* frame::_resolve_name_down(const char* name, const frame* origin) const{
  auto t = children.find(name);
  if(t!=children.end()){return t->second;}

  //Failure? Try to check inside in case children are passthrough.
  //NO LONGER ALLOWED IN GENERAL CASES.
  /*for(const auto& i : children){
    if(type==frame_type_t::NODE || type==frame_type_t::CONTAINER){
        auto w = i.second->_resolve_name_down(name, origin);
        if(w!=nullptr) return w;
    }
  }*/
  return nullptr;
}

const frame* frame::_resolve_name(const char* name, const frame* origin) const {
  if(name[0]==0)return nullptr;

  //Self      
  if(strcmp(name,".")==0){
    return this;
  }
  //Closest reachable namespace parent
  else if(strcmp(name,"..")==0){
    if(type==frame_type_t::CONTAINER || type==frame_type_t::SLOT_CONTAINER || parent == nullptr)return nullptr;
    if(parent->name==""){return parent->_resolve_name("..", parent);}
    else{
      return parent;
    }
  }
  //Furthest reachable parent
  else if(strcmp(name,"...")==0){
    if(type==frame_type_t::CONTAINER || type==frame_type_t::SLOT_CONTAINER || parent == nullptr)return nullptr;

    const frame* saved = nullptr;
    for(const frame* current=parent;current!=nullptr;current=current->_resolve_name("..", current))saved=current;
    return saved;
  }

  //TODO: Evaluate if I want this rule, but I think not.
  //else if((type==frame_type_t::LEAF) && parent!=nullptr)return parent->_resolve_name(name, this);

  //Else look for the name!
  return _resolve_name_down(name,origin);
}

//Symbols are forced to match the tree structure to be found, from bottom to top
symbol_ret_t frame::_resolve_symbol(const char* name, const frame* origin) const{
  //No one else can see my stuff!
  if(access==frame_access_t::PRIVATE && origin!=this){
    if(parent!=nullptr){
      return parent->_resolve_symbol(name, origin);
    }
    return {
        symbol_t::VOID,
        symbol_t::VOID,
        nullptr
    };
  }

  //If not private
  if(symbols!=nullptr){
    auto t = symbols->find(name);
    if(t!=symbols->end() && !is_script_module){return {t->second,symbol_t::VOID,this};}
    else if(t!=symbols->end() && is_script_module){return {t->second,symbols->find("#set_env")->second,this};}
  } 

  //If I am a container I cannot let this progress any further.
  if(type==frame_type_t::CONTAINER or type==frame_type_t::SLOT_CONTAINER){
    return symbol_ret_t{
      {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
      {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
      nullptr
    };
  } else if (parent != nullptr) {
    return parent->_resolve_symbol(name, origin);
  }
    return symbol_ret_t{
      {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
      {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
      nullptr
    };
}

symbol_ret_t frame::_resolve_symbol_local(const char* name) const{
  if(symbols!=nullptr){
    auto t = symbols->find(name);
    if(t!=symbols->end() && !is_script_module){return {t->second,symbol_t::VOID,this};}
    else if(t!=symbols->end() && is_script_module){return {t->second,symbols->find("#set_env")->second,this};}
  } 

  return symbol_ret_t{
    {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
    {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr},
    nullptr
  };
}


std::pair<const smap<std::string>*,const frame*> frame::_resolve_mixin(const char* name, const frame* origin) const{
  auto t = mixins.find(name);
  if (t != mixins.end()) {
    return {&t->second,this};
  }
  // If I am a container I cannot let this progress any further.
  if (type == frame_type_t::CONTAINER or
      type == frame_type_t::SLOT_CONTAINER) {
    return {nullptr, this};
  } else if (parent != nullptr && strcmp(name,"")!=0) { //Skip parent search for self mixin.
    return parent->_resolve_mixin(name, origin);
  }
  return {nullptr, this};
}


//Generate a mixin map based on a list of mixins
smap<std::string> frame::compile_mixins(const char* list_mixins) const {
  smap<std::string> tmp;

  char *buffer = new char [strlen(list_mixins)+1];
  strcpy(buffer,list_mixins);
  int i = 0, j = 0;
  bool last = false;
   while (true) {
    for (; buffer[i] != ',' && buffer[i]!=0;i++){}

    if(buffer[i]==0)last = true;
    else buffer[i]=0;

    auto current = resolve_mixin(buffer+j);
      


    if (current.first == nullptr){}
    else {
      //Take and apply its own mixins first!
      auto key = current.first->find("mixin");
      if(key!=current.first->end() && current.second!=nullptr){
        //Search for elements to mix in STARTING from the frame this was obtained, and not from the original caller. Otherwise inheritance is broken.
        auto tmp2 = current.second->compile_mixins(key->second.data());
        for(const auto& i: tmp2)tmp.insert_or_assign(i.first,std::move(i.second));
      }

      for (const auto &j : *current.first) {
        tmp.insert_or_assign(j.first, j.second);
      }
    }

    i++;
    j=i;
    if(last)break;
  }

  delete[] buffer;

  return tmp;
}

int frame::call_dispatcher(const char* key, const char* value){
  if(custom_dispatcher.symbol!=nullptr){
    if(custom_dispatcher.mode==symbol_mode_t::NATIVE){
        //Even in single script it wants a reference to the caller.
        int(*fn)(const ui_base*, const char* k, const char *v) = (  int(*)(const ui_base*, const char* k, const char *v)) custom_dispatcher.symbol;
        return fn(this->widget(),key,value);
    }
    else if(custom_dispatcher.mode==symbol_mode_t::QUICKJS){
      //TODO: Implement custom dispatching properly
      return 1;
    }
    else return 1;
  }
  else return 1;
}

symbol_t symbol_t::VOID= {symbol_mode_t::AUTO,symbol_type_t::VOID,nullptr};


}