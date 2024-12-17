#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <ui.hpp>
#include <ui-tree.hpp>

#include "pipelines/quickjs-js.hpp"
#include "ui-frame.hpp"
#include "utils/strings.hpp"

namespace vs{

void ui_base::mk_frame(const char* name, frame_mode_t mode){
  if(local_frame==nullptr){
    auto tmp = (frame*)resolve_namespace();
    local_frame=new frame(name, mode , this, tmp, default_frame_type(), frame_access_t::PRIVATE);
  }
}

void ui_base::set_name(const char* name){
  mk_frame();
  local_frame->rename(name);
}

void ui_base::set_access(frame_access_t access){
  mk_frame();
  local_frame->access= access;
}

void ui_base::set_type(frame_type_t type){
  mk_frame();
  local_frame->type = type;
}

void ui_base::set_mode(frame_mode_t mode){
  mk_frame();
  local_frame->mode = mode;
}

const std::string& ui_base::get_name() const{
  static std::string defname = "";
  if(local_frame==nullptr)return defname;
  else{
    return local_frame->name;
  }
}

void ui_base::path(std::stringstream& dst, bool scoped)const{
    frame* current = this->local_frame;
    while(current!=nullptr){
        if(current->name[0]=='#'){dst<<"???"<<"/";}  //Symbols and namesstarting with # are private and not exposed
        else dst<<current->name<<"/";
        if(scoped==true && (current->type==frame_type_t::CONTAINER or current->type==frame_type_t::SLOT_CONTAINER))break;
        current=current->parent;
    }
    if(scoped)dst<<"#";
    else dst<<"@";
}
ui_base::ui_base(ui_base* p){
  parent = p;
}

ui_base::~ui_base(){
  if(local_frame!=nullptr){
    delete local_frame;
    local_frame=nullptr;
  }
}


frame* ui_base::resolve_frame() const{
  ui_base* p = this->parent;
  while(p!=nullptr){
    if(p->local_frame!=nullptr)return p->local_frame;
    p=p->parent;
  }
  return nullptr;
}

frame* ui_base::resolve_namespace() const{
  ui_base* p = this->parent;
  while(p!=nullptr){
    if(p->local_frame!=nullptr && (p->local_frame->get_type()==frame_type_t::CONTAINER  || p->local_frame->get_type()==frame_type_t::NODE  ||  p->local_frame->get_type()==frame_type_t::SLOT_CONTAINER))return p->local_frame;
    p=p->parent;
  }
  return nullptr;
}

const ui_base* ui_base::resolve_name_path(const char * str, bool safe) const{
  static char buffer[64];
  const ui_base* current = this;
  for(int i=0;str[i]!=0;){
    int j=i;
    for(;str[j]!='/' && str[j]!=0 && (j-i <64);j++)buffer[j-i]=str[j];    //TODO exception if going beyond the buffer limits?
    buffer[j-i]=0;
    current=current->resolve_name(buffer, safe);
    i=j;
    if(str[i]!=0)i++;
    if(current==nullptr)return nullptr;
  }
  return current;
}

const ui_base* ui_base::resolve_name(const char * str, bool safe) const{
  if(safe && str[0]=='#')return nullptr;

  auto frame =  this->local_frame;
  if(local_frame==nullptr)frame=this->resolve_frame();

  if(frame!=nullptr){
    auto tmp = frame->resolve_name(str);
    if(tmp==nullptr)return nullptr;
    else return tmp->widget(); 
  }
  else return nullptr;
  
}

 symbol_ret_t ui_base::resolve_symbol(const char * str, bool safe) const{
  if(safe && str[0]=='#')return {symbol_t::VOID, symbol_t::VOID, nullptr};

  if(local_frame!=nullptr){
    return local_frame->resolve_symbol(str);
  }
  else{

    auto frame =  this->resolve_frame();

    if(frame!=nullptr){return frame->resolve_symbol(str);}
    else return {symbol_t::VOID, symbol_t::VOID, nullptr};
  }
}

 symbol_ret_t ui_base::resolve_symbol_local(const char * str, bool safe) const{
  if(safe && str[0]=='#')return {symbol_t::VOID, symbol_t::VOID, nullptr};

  if(local_frame!=nullptr){
    return local_frame->resolve_symbol_local(str);
  }
  else return {symbol_t::VOID, symbol_t::VOID, nullptr};
}


void ui_base::set_dispatcher(symbol_t value){
    mk_frame();
    local_frame->custom_dispatcher = value;
}

void ui_base::set_symbols(const std::shared_ptr<smap<symbol_t>>& ref){
  mk_frame();
  local_frame->symbols=ref;
  {auto it = ref->find("callback"); if(it!=ref->end()){this->apply_prop("on.callback","callback");}}
  {auto it = ref->find("draw"); if(it!=ref->end()){this->apply_prop("on.draw","draw");}}
  {auto it = ref->find("dispatcher"); if(it!=ref->end()){this->set_dispatcher(it->second);}}
}

void ui_base::register_symbol(const char* name, symbol_t value){
    mk_frame();
    return local_frame->register_symbol(name, value);
}

symbol_t ui_base::get_symbol(const char* name){
    mk_frame();
    return local_frame->get_symbol(name);
}

void ui_base::unregister_symbol(const char* name){
    mk_frame();
    return local_frame->unregister_symbol(name);
}

void ui_base::reset_symbols(){
    mk_frame();
    return local_frame->reset_symbols();
}

void ui_base::attach_script(const std::shared_ptr<void>& ref, bool is_script_module){
    mk_frame();
    local_frame->script = ref;
    local_frame->is_script_module=is_script_module;
}

void ui_base::add_mixin(const char* name, const smap<std::string>& map){
    mk_frame();
    local_frame->mixins.emplace(name,std::move(map));
}

//Resolve a mixin based on its name
std::pair<const smap<std::string>*,const frame*> ui_base::resolve_mixin(const char* name) const{
  if(local_frame!=nullptr){
    return local_frame->resolve_mixin(name);
  } else {

    auto frame = this->resolve_frame();

    if (frame != nullptr) {
      return frame->resolve_mixin(name);
    } else
      return {nullptr,local_frame};
  }
}



//Compile a single prop based on a mixin definition
const char* ui_base::eval_prop(const char* prop, const char* mixins[]) const{
  //TODO
  return {};
}


smap<std::string> ui_base::compile_mixins(const char* list_mixins) const {
  if(local_frame!=nullptr){
    return local_frame->compile_mixins(list_mixins);
  } else {

    auto frame = this->resolve_frame();

    if (frame != nullptr) {
      return frame->compile_mixins(list_mixins);
    } else
      return {};
  }
}


smap<std::string> ui_base::compute_refresh_style(const char* local_mixins){
      //TODO: Optimize copies
      smap<std::string> props = this->compile_mixins("*");
      {auto tmp = this->compile_mixins((std::string("+")+this->class_name()).data()); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

      if(this->local_frame!=nullptr){
        auto self_mixin = this->local_frame->resolve_mixin("");

        if (self_mixin.first == nullptr){}
        else {
          //Take and apply its own mixins first!
          auto key = self_mixin.first->find("mixin");
          if(key!=self_mixin.first->end() && self_mixin.second!=nullptr){
            //Search for elements to mix in STARTING from the frame this was obtained, and not from the original caller. Otherwise inheritance is broken.
            auto tmp2 = self_mixin.second->compile_mixins(key->second.data());
            for(const auto& i: tmp2)props.insert_or_assign(i.first,std::move(i.second));
          }

          for (const auto &j : *self_mixin.first) {
            props.insert_or_assign(j.first, j.second);
          }
        }
      }

      {auto tmp = this->compile_mixins(local_mixins); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

      return props;
}

void ui_base::refresh_style(const char* local_mixins){
  smap<std::string> props = compute_refresh_style(local_mixins);
  for (const auto &i : props) {
    int v = this->apply_prop(i.first.data(), i.second.data());
    if (v == 1) {
      vs_log(severety_t::WARNING | severety_t::LVL_DEBUG , this, "Unable to use property `%s` on ",
          i.first.data());
    } else if (v == 2) {
      vs_log(severety_t::WARNING, this,
          "Unable to assign value `%s` to property `%s on", i.second.data(),
          i.first.data());
    }
  }
}

int ui_base::all_tests() const{
  /*
  if(local_frame!=nullptr){
    auto sym =this->resolve_symbol_local("#test",false);
    int tmp = 0;
    if(sym.symbol.type!=symbol_type_t::VOID){
      tmp =  use_test(sym);
    }
    for(auto& it: local_frame->children){
      auto element = it.second->widget();
      tmp += element->all_tests();
    }
    return tmp;
  }
  */
  return 0;
}

int ui_base::use_getter(const symbol_ret_t& sym, void ** value){
  if(sym.symbol.type!=symbol_type_t::GETTER)return -1;

  symbol_ret_t::get_fn fn = (symbol_ret_t::get_fn)sym.symbol.symbol;
  if(sym.found_at->get_mode()==frame_mode_t::NATIVE){
    if(sym.ctx_apply.symbol!=nullptr){
      const ui_base* (*ctx_apply)(const ui_base*) = ( const ui_base* (*)(const ui_base*) ) sym.ctx_apply.symbol;
      const ui_base* tmp =ctx_apply(sym.found_at->widget());
      auto t = fn(value);
      ctx_apply(tmp);
      return t;
    }
    else{
      auto t = fn(value);
      return t;
    }
  }
  else {} //TODO: Add support for quickjs
  return 1;
}
int ui_base::use_setter(const symbol_ret_t& sym, const void * value){
  if(sym.symbol.type!=symbol_type_t::SETTER)return -1;

  symbol_ret_t::set_fn fn = (symbol_ret_t::set_fn)sym.symbol.symbol;

  if(sym.found_at->get_mode()==frame_mode_t::NATIVE){
    if(sym.ctx_apply.symbol!=nullptr){
      const ui_base* (*ctx_apply)(const ui_base*) = ( const ui_base* (*)(const ui_base*) ) sym.ctx_apply.symbol;
      const ui_base* tmp =ctx_apply(sym.found_at->widget());
      auto t = fn(value);
      ctx_apply(tmp);
      return t;
    }
    else{
      auto t = fn(value);
      return t;
    }
  }
  else {} //TODO: Add support for quickjs
  return 1;
}
int ui_base::use_callback(const symbol_ret_t& sym, ui_base * node){
  if(sym.symbol.type!=symbol_type_t::CALLBACK)return -1;

  symbol_ret_t::cb_fn fn = (symbol_ret_t::cb_fn)sym.symbol.symbol;

  if(sym.found_at->get_mode()==frame_mode_t::NATIVE){
    if(sym.ctx_apply.symbol!=nullptr){
      const ui_base* (*ctx_apply)(const ui_base*) = ( const ui_base* (*)(const ui_base*) ) sym.ctx_apply.symbol;
      const ui_base* tmp =ctx_apply(sym.found_at->widget());
      fn(node);
      ctx_apply(tmp);
      return 0;
    }
    else{
      fn(node);
      return 0;
    }
  }
  else if(sym.found_at->get_mode()==frame_mode_t::QUICKJS){
    #if VS_USE_QJS
    //TODO: Add support for quickjs script modules
    pipelines::quickjs_t* script = (pipelines::quickjs_t*)sym.found_at->script.get();
    auto globalThis = JS_GetGlobalObject(script->ctx);
    auto ret= JS_Call(script->ctx,std::get<2>(script->handles[(size_t)sym.symbol.symbol-1]),globalThis,0,nullptr);
    JS_FreeValue(script->ctx, ret);
    JS_FreeValue(script->ctx, globalThis);
    return 0;
    #endif
  }
  else{
    //Callback type not supported yet.
  }
  return 1;
}


void ui_callback_handler(Fl_Widget* _, void* _data){
  ui_callback_t* data = (ui_callback_t*)_data;
  ui_base::use_callback(data->sym, data->caller);
}


}