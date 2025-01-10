#include <cstdarg>
#include <unistd.h>
#include <memory>

#include <FL/Fl.H>
#include <FL/Fl_Widget.H>
#include <components/containers.hpp>

#include <vs-templ.hpp>

#include <pipelines/tcc-c.hpp>
#include <pipelines/quickjs-js.hpp>
#include <pipelines/lua-lua.hpp>


#include <utils.hpp>
#include <utils/paths.hpp>
#include <utils/policies.hpp>

#include <ui.hpp>
#include "ui-tree.hpp"
#include "ui-frame.hpp"
#include <ui-frame.private.hpp>
#include <ui-tree.xml.hpp>

#include <globals.hpp>
#include <singletons.hpp>
#include <fetcher.hpp>

#include <utils/paths.hpp>


#if __has_include("components/autogen/index.hpp")
#include <components/autogen/index.hpp>
#endif


//TODO: Add support for namespaces in macros
#define mkNodeWidget($ns,$name,$class_name) else if(strcmp(root.name(),#$name)==0){\
  auto t=build_base_widget<$class_name>(root,root_ui);\
  t->set_type(frame_type_t::NODE);\
  root_ui = t;\
  for(auto& i : root.children()){_build(i,root_ui);}\
  t->widget().end();\
  t->widget().show();\
  return;\
} 
//TODO: Avoid parsing its head                                                 
#define mkLeafWidget($ns,$name,$class_name) else if(strcmp(root.name(),#$name)==0){build_base_widget<$class_name>(root,root_ui); }                                                  

#define mkNSNodeWidget($ns,$name,$class_name) else if(strcmp(root.name(),(ns.fltk +  #$name).c_str())==0 ){\
  auto t=build_base_widget<$class_name>(root,root_ui);\
  t->set_type(frame_type_t::NODE);\
  root_ui = t;\
  for(auto& i : root.children()){_build(i,root_ui);}\
  t->widget().end();\
  t->widget().show();\
  return;\
} 

#define mkNSLeafWidget($ns,$name,$class_name) else if(strcmp(root.name(),(ns.fltk +  #$name).c_str())==0 ){\
  build_base_widget<$class_name>(root,root_ui);\
}                                                  


namespace vs{

#if __has_include("ui-tree.xml.ns.autofrag.cpp")
#include "ui-tree.xml.ns.autofrag.cpp"
#endif


void ui_tree_xml::log(int severety, const void* _ctx, const char* str, ...){
  static const char* severity_table[] = {
    "\033[34;1m[INFO]\033[0m     : ",
    "\033[32;1m[OK]\033[0m       : ",
    "\033[33;1m[WARNING]\033[0m  : ",
    "\033[37;1m[CONTINUE]\033[0m : ",
    "\033[31;1m[PANIC]\033[0m    : ",
  };

  const pugi::xml_node& ctx = *(const pugi::xml_node*)_ctx;
  std::string rstr = std::string("\n")+std::string(severity_table[(int)severety%5]) + std::string(str) + " @ [\033[93;3m" + (_ctx!=nullptr?ctx.path():"???") + "\033[0m]";
  
  va_list args;
  va_start(args, str);
  vfprintf(log_device,rstr.c_str(), args);
  va_end(args);

  fflush(log_device);
}


//General XML loader for apps and components.
//TODO: The caller node is a design flaw. We need to be given the list of props and slots. Not the full node which might not even exist.
int ui_tree_xml::load(const char* file, type_t type)
{
  //TODO: As part of this process, policies should be aligned with what defined in the base config, 
  //and not just one single set of options, so that we can pattern match paths.
  policies.inherit(globals->env.computed_policies);
  if(parent!=nullptr)policies.inherit(parent->policies);

  resolve_path resolver(policies,globals->path_env,(parent==nullptr)?globals->path_env.cwd:parent->local);
  auto buffer = fetcher(*globals,resolver,resolve_path::from_t::NATIVE_CODE,file);

  if(std::get<0>(buffer)==resolve_path::reason_t::OK){
    this->local=std::get<2>(buffer).base_dir();
    this->fullname=std::get<2>(buffer);
  }

  this->type = type;
  vs_log(severety_t::INFO, nullptr, "Requested loading of file `%s`", std::get<2>(buffer).as_string().data());

  //TODO: Move this to cache later on, but for now just dump the result into a buffer

  if(std::get<0>(buffer)!=resolve_path::reason_t::OK){
    //TODO: error handling
    vs_log(severety_t::PANIC, nullptr, "Oh no! Loading failed `%s` for %d", std::get<2>(buffer).as_string().data(),std::get<0>(buffer));
    return 2;
  }
  else{
    //TODO: register XMl tree?
    pugi::xml_parse_result result =  doc.load_buffer(std::get<1>(buffer).data, std::get<1>(buffer).size); // doc.load_file(file);
    if (!result){
        return 1;
    }

  //Detect namespaces defined on the root of the component
  for(auto& prop : doc.root().first_child().attributes()){
    if(templ::cexpr_strneqv(prop.name(), "xmlns:")){
      if(strcmp(prop.value(),"vs.fltk")==0){this->set_namespace(namespaces_t::fltk, prop.name()+6);}
      else if(strcmp(prop.value(),"vs.templ")==0){this->set_namespace(namespaces_t::s, prop.name()+6);}
      else if(strcmp(prop.value(),"vs.core")==0){this->set_namespace(namespaces_t::vs, prop.name()+6);}
    }
    else if(strcmp(prop.name(),"xmlns")){
      if(strcmp(prop.value(),"vs.fltk")==0){this->set_namespace(namespaces_t::fltk, "");}
      else if(strcmp(prop.value(),"vs.templ")==0){this->set_namespace(namespaces_t::s, "");}
      else if(strcmp(prop.value(),"vs.core")==0){this->set_namespace(namespaces_t::vs, "");}
    }
  }
  
  strings.prepare(ns.vs.data());

    //TODO: Static parsing is performed in here!
    {
    auto root_data = doc.child("static-data");

      if(!root_data.empty()){
        pugi::xml_document datadoc;
        auto tplt = root_data.attribute("template");  //TODO: Adapt to use the proper syntax. I cannot remember which one was it

        resolve_path resolver(policies,globals->path_env,this->local);

        auto buffer = fetcher(*globals,resolver,resolve_path::from_t::NATIVE_CODE,tplt.as_string());
        if(std::get<0>(buffer)!=resolve_path::reason_t::OK){
          //TODO: error handling
          return 2;
        }
        else{
          //TODO: register XMl tree?
          pugi::xml_parse_result result =  datadoc.load_buffer(std::get<1>(buffer).data, std::get<1>(buffer).size); 
          if (!result){
              return 1;
          }
        }

        templ::preprocessor processor(doc,datadoc,ns.s.c_str());
        //Resolve it.

        auto& result  = processor.parse();

        doc.reset(result);
      }
    }
  }
  return 0;
}

ui_tree_xml::~ui_tree_xml(){if(root!=nullptr)delete root;}

void ui_tree_xml::cleanup(){
  this->doc.reset();
}


int ui_tree_xml::build(){
  
  const auto& xml_root = doc.child((type==type_t::APP)?strings.APP_TAG:strings.COMPONENT_TAG);
  if(xml_root.empty()){
    log(severety_t::CONTINUE, xml_root, "Unable to find a valid root in `%s`", fullname.as_string().c_str());
    return 1;
  }

  ui_base* base;

  bool thin= xml_root.attribute("thin").as_bool(true);
  bool autoprune = xml_root.attribute("auto-prune").as_bool(false);

  if(type==type_t::APP){
    auto tmp_app = new ui_root_app(frame_mode_t::AUTO);
    base = (ui_base*)tmp_app;
    {//TODO: Handle app.class token & page tag
      auto token = string2key256(xml_root.attribute("class-token").as_string(nullptr), tmp_app->local_env.src_key);
      //cache_ctx.computed_key = key256compose(token, cache_ctx.computed_key);
      tmp_app->local_env.page_tag = xml_root.attribute("page").as_string("");
    }

    link_with = {
        doc.first_child().attribute("link-with.lib").as_string(nullptr),
        doc.first_child().attribute("link-with.header").as_string(nullptr)
    };

    std::string tmp_link_lib, tmp_link_header;
    if(link_with.lib!=nullptr && link_with.header!=nullptr){
      resolve_path resolver(policies,globals->path_env,local);
      auto computed_path_lib = resolver(resolve_path::from_t::NATIVE_CODE,link_with.lib);
      auto computed_path_header = resolver(resolve_path::from_t::NATIVE_CODE,link_with.header);
      if(computed_path_lib.first==resolve_path::reason_t::OK  && computed_path_header.first==resolve_path::reason_t::OK){
        //TODO: For now I am assuming it is on the fs. I should resolve it to tmp if remote for example
        //The issue is tcc capabilities in handling dynamic linking from a buffer sourced from memory.
        //This is why for now it will only be assumed to be on the fs.
        //Hopefully this restriction will never apply to native components for example.
        tmp_link_lib=computed_path_lib.second.location;
        tmp_link_header=computed_path_header.second.location;
        link_with.lib = tmp_link_lib.c_str();
        link_with.header = tmp_link_header.c_str();
        log(severety_t::INFO, root, "Requested linking with `%s`", link_with);
      }
    }

  }
  else if(type==type_t::COMPONENT && (!autoprune || !thin)){
    base = thin?(ui_base*)new ui_root_thin_component(frame_mode_t::AUTO):(ui_base*)new ui_root_component(frame_mode_t::AUTO);

    if(!thin){//TODO: Handle app.class token & page tag
      ui_root_component* tmp_component  = (ui_root_component*)base;
      auto token = string2key256(xml_root.attribute("class-token").as_string(nullptr), tmp_component->local_env.src_key);
      //cache_ctx.computed_key = key256compose(token, cache_ctx.computed_key);
      tmp_component->local_env.page_tag = xml_root.attribute("page").as_string("");
    }
  }
  else base = caller_ui_node;

  _build(doc.child(
    (type==type_t::APP)?strings.APP_TAG:strings.COMPONENT_TAG), base);

  if(type==type_t::APP)root=base;
  else root = nullptr;
  return 0;
}

void ui_tree_xml::_build(const pugi::xml_node& root, ui_base* root_ui){
  //std::cout<<root.name()<<root.text()<<root.value()<<'\n';
  //USE
  if(strcmp(root.name(),strings.USE_TAG)==0){
    const auto& src =root.attribute("src");
    if(src.empty()){
      log(severety_t::CONTINUE,root,"<use/> must have a source");
    }
    else{
      log(severety_t::INFO,root,"Loading component %s", src.as_string()); //TODO: How is it possible that this shows file:// in place of the actual one?
      ui_tree_xml component_tree(this,root_ui,&root); 
      if(component_tree.load(src.as_string(),type_t::COMPONENT)!=0){
        log(severety_t::INFO,root,"Loading failed, file cannot be opened %s", src);
      }
      else{
        component_tree.build();
        nodes.insert(nodes.end(),component_tree.nodes.begin(),component_tree.nodes.end());
        component_tree.nodes.clear();
        return;
      }
    }

    //Collector of all failures
    const auto& error =root.child("on.load-fail");
    if(!error.empty()){
      _build(error,root_ui);
      return;
    }
    return;
  }
  //IMPORT
  //TODO: restrict to direct children of the base app/component
  else if(strcmp(root.name(),strings.IMPORT_TAG)==0){
    const auto& src = root.attribute("src").as_string(nullptr);
    const auto& as = root.attribute("as").as_string(nullptr);

    if(src!=nullptr){
      if(as!=nullptr){
        this->imports.emplace(as,src);
      }
    }
  }    
  //SLOT
  else if(strcmp(root.name(),strings.SLOT_TAG)==0){
    //Copy the content provided by the parent in place of the slot, or render its content if the parent has none.
    const auto& name = root.attribute("tag").as_string("default");
    //The default slot.
    if(this->caller_xml_node!=nullptr){
      const auto& match = this->caller_xml_node->find_child([&name](const pugi::xml_node& node){
        return (strcmp(node.name(),"inject")==0) && (strcmp(node.attribute("tag").as_string("default"),name)==0);
        });
        
      if(!match.empty()){
        for(auto& i : match.children()){_build(i,root_ui);}
        return;
      }
    }
    //Catch all slot not found.
    if(!root.attribute("required").empty()){
      log(severety_t::WARNING,root,"The `%s` slot was set as required but no override available.", name);
    }
    for(auto& i : root.children()){_build(i,root_ui);}
    
  }
  //DEBUG
  else if(strcmp(root.name(),strings.DEBUG_TAG)==0){
    vs::singleton::debug(root.attribute("key").as_string("<NULL>"), root.attribute("value").as_string("<NULL>"));
    return;
  }
  //VIEWPORT
  else if(strcmp(root.name(),strings.VIEWPORT_TAG)==0){
    auto tmp = build_base_widget<ui_viewport>(root,root_ui);
    root_ui = tmp;
    for(auto& i : root.children()){_build(i,root_ui);}
    tmp->widget().end();

    return;
  }
  //NAMESPACE
  else if(strcmp(root.name(),strings.NAMESPACE_TAG)==0){
    auto tmp = build_base_widget<ui_namespace>(root);
    tmp->set_access(frame_access_t::PUBLIC);
    root_ui = tmp;
    for(auto& i : root.children()){_build(i,root_ui);}
    //tmp->widget().end();
    return;
  }
  //APP
  else if (strcmp(root.name(),strings.APP_TAG)==0){

    _build_base_widget_extended_attr(root, (ui_base *)root_ui);
  
    //TODO: Optimize copies
    smap<std::string> props;

    for (const auto &i : root.attributes()) {
      props.insert_or_assign(i.name(), i.value());
    }

    for (const auto &i : props) {
      int v = root_ui->apply_prop(i.first.data(), i.second.data());
      if (v == 1) {
        log(severety_t::WARNING, root, "Unable to use property `%s` on ",
            i.first.data());
      } else if (v == 2) {
        log(severety_t::WARNING, root,
            "Unable to assign value `%s` to property `%s on", i.second.data(),
            i.first.data());
      }
    }

  }
  else if (strcmp(root.name(),strings.COMPONENT_TAG)==0){}

#   if __has_include("./ui.xml-widgets.autogen.cpp")
#   include "./ui.xml-widgets.autogen.cpp"
#   endif


  else if(imports.contains(root.name())){
    auto it = imports.find(root.name());
    log(severety_t::INFO,root,"Loading component %s", it->second.c_str()); //TODO: How is it possible that this shows file:// in place of the actual one?
    ui_tree_xml component_tree(this,root_ui,&root); 
    if(component_tree.load(it->second.c_str(),type_t::COMPONENT)!=0){
      log(severety_t::INFO,root,"Loading failed, file cannot be opened %s", it->second.c_str());
    }
    else{
      component_tree.build();
      nodes.insert(nodes.end(),component_tree.nodes.begin(),component_tree.nodes.end());
      component_tree.nodes.clear();
      return;
    }

    //Collector of all failures
    const auto& error =root.child("on.load-fail");
    if(!error.empty()){
      _build(error,root_ui);
      return;
    }

  }
  else{return;}

  for(auto& i : root.children()){_build(i,root_ui);}
  return;
}

void ui_tree_xml::_build_base_widget_extended_attr(const pugi::xml_node &root, ui_base* current) {

  for (auto &root : root.children()) {

    // MIXIN
    if (strcmp(root.name(), strings.MIXIN_TAG) == 0) {
      smap<std::string> tmp;
      for (const auto &i : root.attributes()) {
        tmp.insert_or_assign(i.name(), i.value());
      }
      {auto it = tmp.find("name");if (it != tmp.end())tmp.erase(it);}
      {auto it = tmp.find("frame.type");if (it != tmp.end())tmp.erase(it);}
      {auto it = tmp.find("frame.access");if (it != tmp.end())tmp.erase(it);}
      {auto it = tmp.find("frame.mode");if (it != tmp.end())tmp.erase(it);}
      
      current->add_mixin(root.attribute("name").as_string(""), tmp);
      // Always remove the `name` attribute from mixins.
    }

    // SCRIPT
    else if (strcmp(root.name(),strings.SCRIPT_TAG) == 0) {
      //Check uniqueness
      current->mk_frame();
      if(current->get_local_frame()->has_script()){
        log(severety_t::WARNING, root, "Only one `script` is allowed per frame.");
        continue;
      }

      bool is_module=false;

      auto script_type=root.attribute("type").as_string("");
      auto compact=root.attribute("compact").as_bool(false);

      //Check if it is a module or single user; if module check for cache and use it.
      if(strcmp(script_type,"module")==0){
        is_module=true;
        auto filename = this->fullname.as_string();

        auto found = globals->mem_storage.get({filename.c_str(),this->local_unique_counter+1,cache::resource_t::SCRIPT});
        if(found!=nullptr){
          current->set_mode(((cache::script_t*)found->ref.get())->mode);
          current->attach_script(((cache::script_t*)found->ref.get())->script,is_module);
          current->set_symbols(((cache::script_t*)found->ref.get())->symbols);
          //All done, precomputed and rightfully applied!
          continue;
        }
      }
      else if(strcmp(script_type,"quick")==0){
        //TODO: Quick scripts are modules for which the code is the body of a callback function which will be automatically assigned. 
        //They can be used to write code which is much more compact.
      }


      auto mode =current->get_local_frame()->get_mode();

      if (mode == frame_mode_t::NATIVE || mode == frame_mode_t::AUTO) {          
        const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::NATIVE?"c":"");
        if (strcmp(lang, "c") == 0) {
          #ifdef VS_USE_TCC
          auto compiler = pipelines::tcc_c_pipeline_xml(*globals,true, is_module?nullptr:current, root, compact, link_with);
          if(compiler!=nullptr){
            current->set_mode(frame_mode_t::NATIVE);
            current->attach_script(compiler,is_module);
            auto symbols = pipelines::tcc_c_pipeline_apply(compiler, current, (void*)&root, (void(*)(void*,const char*, const char*))pipelines::tcc_log_symbol_func_xml);
            current->set_symbols(symbols);
            if(is_module){
              auto tmp = std::make_shared<cache::script_t>(cache::script_t{
                compiler, symbols, frame_mode_t::NATIVE
              });
              globals->mem_storage.fetch_from_shared({this->fullname.as_string().c_str(),local_unique_counter+1,cache::resource_t::SCRIPT,false,false}, tmp, res::script_t::C);
              local_unique_counter++;
            }
          }
          continue;
          #endif
        }
      }
      if (mode == frame_mode_t::QUICKJS || mode == frame_mode_t::AUTO) {
        const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::QUICKJS?"js":"");
        if (strcmp(lang, "js") == 0) {
          #ifdef VS_USE_QJS
          auto compiler = pipelines::qjs_js_pipeline_xml(*globals,true, is_module?nullptr:current, root, link_with);
            if(compiler!=nullptr){
              current->set_mode(frame_mode_t::QUICKJS);
              current->attach_script(compiler,is_module);
              auto symbols = pipelines::qjs_js_pipeline_apply(compiler, current, (void*)&root, (void(*)(void*,const char*, const char*))pipelines::qjs_log_symbol_func_xml);
              current->set_symbols(symbols);
              if(is_module){
                auto tmp = std::make_shared<cache::script_t>(cache::script_t{
                  compiler, symbols, frame_mode_t::QUICKJS
                });
                globals->mem_storage.fetch_from_shared({this->fullname.as_string().c_str(),local_unique_counter+1,cache::resource_t::SCRIPT,false,false}, tmp, res::script_t::JS);
                local_unique_counter++;
              }
            }
          continue;
          #endif
        }
      }
      if (mode == frame_mode_t::LUA || mode == frame_mode_t::AUTO) {
        const auto &lang = root.attribute("lang").as_string(mode==frame_mode_t::LUA?"lua":"");
        if (strcmp(lang, "lua") == 0) {
          #ifdef VS_USE_LUA
          auto compiler = pipelines::lua_lua_pipeline_xml(*globals,true, is_module?nullptr:current, root, link_with);
            if(compiler!=nullptr){
              current->set_mode(frame_mode_t::LUA);
              current->attach_script(compiler,is_module);
              auto symbols = pipelines::lua_lua_pipeline_apply(compiler, current, (void*)&root, (void(*)(void*,const char*, const char*))pipelines::lua_log_symbol_func_xml);
              current->set_symbols(symbols);
              if(is_module){
                auto tmp = std::make_shared<cache::script_t>(cache::script_t{
                  compiler, symbols, frame_mode_t::LUA
                });
                globals->mem_storage.fetch_from_shared({this->fullname.as_string().c_str(),local_unique_counter+1,cache::resource_t::SCRIPT,false,false}, tmp, res::script_t::JS);
                local_unique_counter++;
              }
            }
          continue;
          #endif
        }
      }
      if (mode == frame_mode_t::RISCV || mode == frame_mode_t::AUTO) {
        const auto &lang = root.attribute("lang").as_string();
        //TODO search for assigned compiler for a given language. Compiling pipeline must be generic and made uniform for each one of them.
      }
      if (mode == frame_mode_t::WASM || mode == frame_mode_t::AUTO) {
        const auto &lang = root.attribute("lang").as_string();
        //TODO search for assigned compiler for a given language. Compiling pipeline must be generic and made uniform for each one of them.
      }

      {
          log(severety_t::CONTINUE, root,
              "Unsupported language `%s` for frame type `%i`. This script will be skipped.",
              root.attribute("lang").as_string(), mode);
      }
    }
  }
}


  template <std::derived_from<ui_base> T>
  T *ui_tree_xml::build_base_widget(const pugi::xml_node &root, ui_base* root_ui) {
    auto *current = new T(root_ui);
    nodes.push_back(current);

    {
      const auto& tmp = root.attribute("name");
      if (!tmp.empty()) {current->set_name(tmp.as_string());}
    }
    {
      const auto& tmp = root.attribute("frame.mode");
      if (!tmp.empty()) {
        if(strcmp(tmp.as_string(),"native")==0)current->set_mode(frame_mode_t::NATIVE);
        else if(strcmp(tmp.as_string(),"quickjs")==0)current->set_mode(frame_mode_t::QUICKJS);
        else if(strcmp(tmp.as_string(),"lua")==0)current->set_mode(frame_mode_t::LUA);
        else if(strcmp(tmp.as_string(),"wasm")==0)current->set_mode(frame_mode_t::WASM);
        else if(strcmp(tmp.as_string(),"external")==0)current->set_mode(frame_mode_t::EXTERNAL);
        else{current->set_mode(frame_mode_t::AUTO);}  //TODO: Catch error
      }
    }
    {
      const auto& tmp = root.attribute("frame.type");
      if (!tmp.empty()) {
        if(strcmp(tmp.as_string(),"container")==0)current->set_type(frame_type_t::CONTAINER);
        else if(strcmp(tmp.as_string(),"slot-container")==0)current->set_type(frame_type_t::SLOT_CONTAINER);
        else if(strcmp(tmp.as_string(),"container-slot")==0)current->set_type(frame_type_t::SLOT_CONTAINER);
        else if(strcmp(tmp.as_string(),"slot")==0)current->set_type(frame_type_t::SLOT);
        else if(strcmp(tmp.as_string(),"leaf")==0)current->set_type(frame_type_t::LEAF);
        else if(strcmp(tmp.as_string(),"node")==0)current->set_type(frame_type_t::NODE);
        else{current->set_type(frame_type_t::SLOT_CONTAINER);}  //TODO: Catch error
      }
    }
    {
      const auto& tmp = root.attribute("frame.access");
      if (!tmp.empty()) {
        if(strcmp(tmp.as_string(),"private")==0)current->set_access(frame_access_t::PRIVATE);
        else if(strcmp(tmp.as_string(),"public")==0)current->set_access(frame_access_t::PUBLIC);
        else{current->set_access(frame_access_t::PRIVATE);}  //TODO: Catch error
      }
    }

    _build_base_widget_extended_attr(root, (ui_base *)current);
    
    //TODO: Check if this has still any effect
    if(strcmp(root.parent().name(),strings.APP_TAG)==0){
      current->reparent_frame(root_ui);
    }

    //TODO: Optimize copies
    smap<std::string> props = current->compute_refresh_style(root.attribute("mixin").as_string(""));
    /*{auto tmp = current->compile_mixins((std::string("+")+root.name()).data()); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}

      const auto &self_mixin = (root.find_child([](const auto &i) {
        return (strcmp(i.name(), "mixin") == 0) &&
              (strcmp(i.attribute("name").as_string(""), "") == 0);
      }));
      if (!self_mixin.empty()) {
        for (const auto &i : self_mixin.attributes()) {
          props.insert_or_assign(i.name(), i.value());
        }
        {
          auto it = props.find("name");
          if (it != props.end())
            props.erase(it);
        }
    }

    {auto tmp = current->compile_mixins(root.attribute("mixin").as_string("")); for(const auto& i: tmp)props.insert_or_assign(i.first,std::move(i.second));}
    */

    //For components add to its direct children the attributes coming from above
    if(strcmp(root.parent().name(),strings.COMPONENT_TAG)==0){
      for (const auto &i : this->caller_xml_node->attributes()) {
        //Exclude src as it was consumed in here.
        if(strcmp(i.name(),"src")!=0)props.insert_or_assign(i.name(), i.value());
      }
    }
    
    for (const auto &i : root.attributes()) {
      props.insert_or_assign(i.name(), i.value());
    }

    for (const auto &i : props) {
      int v = current->apply_prop(i.first.data(), i.second.data());
      if (v == 1) {
        log(severety_t::WARNING, root, "Unable to use property `%s` on ",
            i.first.data());
      } else if (v == 2) {
        log(severety_t::WARNING, root,
            "Unable to assign value `%s` to property `%s on", i.second.data(),
            i.first.data());
      }
    }
    return current;
  }
}

#undef mkNodeWidget
#undef mkLeafWidget

#undef mkNSNodeWidget
#undef mkNSLeafWidget