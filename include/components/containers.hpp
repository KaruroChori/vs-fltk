#pragma once

#include <FL/Fl_Group.H>
#include <FL/Fl_Widget.H>
#include <FL/Fl_Window.H>

#include <cache/commons.hpp>
#include <ui.hpp>
#include <app.hpp>
#include <ui-frame.hpp>

namespace vs{


class ui_root_thin_component : public ui_base{
  public:
    ui_root_thin_component(frame_mode_t MODE):ui_base(nullptr){
      //Cannot use mk_frame as it requires recursion and the widget property to operate.
      local_frame=new frame(nullptr, MODE, this, nullptr, default_frame_type(), frame_access_t::PUBLIC);
  }

  virtual frame_type_t default_frame_type() override {return frame_type_t::CONTAINER;}
  virtual const char* class_name() override{return "component.thin";}

  int apply_prop(const char *prop, const char *value) override{
    if(strncmp(prop, "xmlns:", sizeof("xmlns:")-1)==0){return 0;}
    else if(strcmp(prop, "name")==0){return 0;}
    else if(strncmp(prop, "frame.", sizeof("frame.")-1)==0){return 0;}
    if(local_frame!=nullptr)return local_frame->call_dispatcher(prop,value);
    return 1;
  }

  int get_computed(const char *prop, const char **value) override{return 1;}
};

class ui_root_component : public ui_root_thin_component{
  public: //TODO: Make it private at some point, but I need to clean up the rest of the code and set up proper friend classes first.
    app_env_t local_env;

  public:
    ui_root_component(frame_mode_t MODE):ui_root_thin_component(MODE){}

    virtual const char* class_name() override{return "component";}
};


class ui_root_app :public ui_root_component{

  public:
    ui_root_app(frame_mode_t MODE):ui_root_component(MODE){}

  virtual const char* class_name() override{return "app";}
  int apply_prop(const char *prop, const char *value) override{
    if(strncmp(prop, "xmlns:", sizeof("xmlns:")-1)==0){return 0;}
    else if(strncmp(prop, "link-with.", sizeof("link-with.")-1)==0){return 0;}
    else if(strncmp(prop, "frame.", sizeof("frame.")-1)==0){return 0;}
    if(local_frame!=nullptr)return local_frame->call_dispatcher(prop,value);
    return 1;
  }

};


//TODO: Add a new ui_region similar to namespace but with extra cache::ctx_t like app, to nest a new "safe" region inside.

class ui_namespace : public ui_base{
  public:
    ui_namespace(ui_base* p):ui_base(p){
      //this->widget().size(widget().parent()->w(), widget().parent()->h());
      this->mk_frame();
      this->set_access(frame_access_t::PUBLIC);
  }

  virtual frame_type_t default_frame_type() override {return frame_type_t::NODE;}
  virtual const char* class_name() override{return "namespace";}

  int apply_prop(const char *prop, const char *value) override{
    if(strcmp(prop, "name")==0){return 0;}
    else if(strncmp(prop, "frame.", sizeof("frame.")-1)==0){return 0;}
    if(local_frame!=nullptr)return local_frame->call_dispatcher(prop,value);
    return 1;
  }

  int get_computed(const char *prop, const char **value) override{return 1;}
};

class ui_viewport : public ui<Fl_Window>{
  protected:
    std::string src;
    enum status_t{LOADING, READY, FAILED} status;
    ui_base* content_loading;
    ui_base* content;
    ui_base* content_fail;

  public:
    template<typename... Args>
    ui_viewport(ui_base* p,Args ...w):ui<Fl_Window>(p,w...){mk_frame();}
    virtual frame_type_t default_frame_type() override {return frame_type_t::SLOT_CONTAINER;}
    virtual const char* class_name() override{return "viewport";}

    void refresh_content();

    static int _apply_prop(ui_viewport* that, const char* prop, const char* value);
    static int _get_computed(ui_viewport* that, const char* prop, const char** value);
};

}