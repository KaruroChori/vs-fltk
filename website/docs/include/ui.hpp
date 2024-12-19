#pragma once

#include <FL/Fl_Widget.H>
#include <utils/paths.hpp>
#include <ui-base.hpp>

namespace vs{

template<std::derived_from<Fl_Widget>T = Fl_Widget, int SubType = 0> 
class ui : public ui_base {
  private:

  public:
    ui(ui_base* p):ui_base(p){_widget = new T(0,0,0,0);widget().size(100,100);}
    template<typename... Args>
    ui(ui_base* p, Args ...w):ui_base(p){_widget = new T(w...);}
    inline T& widget(){return *(T*)_widget;}
    inline const T& widget()const{return *(T*)_widget;}

    inline operator T&(){return widget();}

    inline virtual ~ui(){Fl::delete_widget(_widget);}

    virtual int apply_prop(const char* prop, const char* value) override {return 2;}
    virtual int get_computed(const char* prop, const char ** value) override {return 2;};

    virtual frame_type_t default_frame_type() override {return frame_type_t::LEAF;}
    virtual const char* class_name() override{return "vs-generic";};
};

//#region Implementation of ui<T>

//Root elements & special containers


extern void app_debug();

struct severety_t{
  enum lvl_t{
    INFO,
    OK,
    WARNING,
    CONTINUE,
    PANIC,
    LOG,
    LVL_SILENT = 0x00,
    LVL_NORMAL = 0x10,
    LVL_VERBOSE = 0x20,
    LVL_DEBUG = 0x40,
  };
};

extern void vs_log(int severety, const ui_base* ctx, const char* str, ...);

}
