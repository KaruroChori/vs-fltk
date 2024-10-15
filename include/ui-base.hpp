#pragma once

#include <ui-frame.hpp>

namespace vs{

class ui_base{
    protected:
        frame* local_frame = nullptr;
        uint8_t _injection_point[0];

    public:
    const frame* get_local_frame(){return local_frame;}
    void mk_frame(const char* name=nullptr, frame_mode_t mode = frame_mode_t::DEFAULT);
    virtual frame_type_t default_frame_type()=0;
    virtual const char* class_name(){return "vs-base";};

    void set_name(const char*);
    void set_mode(frame_mode_t);
    void set_type(frame_type_t);
    void set_access(frame_access_t);
    const std::string& get_name() const;

    void register_symbol(const char* name, symbol_t value);
    symbol_t get_symbol(const char* name);
    void unregister_symbol(const char* name);
    void reset_symbols();
    void set_dispatcher(symbol_t value);

    //Attach scripts
    //TODO: Make generic?
    void attach_unique_script(const std::shared_ptr<void>& ref);
    void attach_shared_script(const std::shared_ptr<void>& ref);

    //Add mixin
    void add_mixin(const char* name, const smap<std::string>& ref);

    //Resolve a mixin based on its name
    std::pair<const smap<std::string>*,const frame*> resolve_mixin(const char* name) const;

    //Compile a single prop based on a mixin definition
    const char* eval_prop(const char* prop, const char* mixins[]) const;

    //Generate a mixin map based on a list of mixins
    smap<std::string> compile_mixins(const char* mixins_list) const;

    inline Fl_Widget& widget(){return ((Fl_Widget*)_injection_point)[0];}
    inline const Fl_Widget& widget()const{return ((Fl_Widget*)_injection_point)[0];}

    //TODO: these constraints should be relaxed to support more general reparenting.
    //However this is not planned right now, and reparent_frame is just a fix to the problem of app not being a widget.
    inline void reparent_frame(ui_base* newparent){
      if(local_frame!=nullptr && local_frame->parent==nullptr && newparent!=nullptr && newparent->local_frame!=nullptr){
        local_frame->parent=newparent->local_frame;
        newparent->local_frame->children.insert_or_assign(local_frame->name,local_frame);
      }
    }


    virtual ~ui_base();

    static ui_base* FL_TO_UI(const Fl_Widget& base){
      auto offset = (size_t)&(((ui_base *)0)->_injection_point) - (size_t)(ui_base *)0 ;
      return (ui_base*)((uint8_t*)&base - offset);
    }

    void path(std::stringstream& dst, bool scoped = true) const;

    //Resolve the frame, always looking at the parent and not self.
    frame* resolve_frame()  const;
    symbol_ret_t resolve_symbol(const char* str) const;
    const ui_base* resolve_name(const char * str) const;
    const ui_base* resolve_name_path(const char * str) const;

    virtual int get_computed(const char* prop, const char ** value) = 0;
    virtual int apply_prop(const char* prop, const char * value) = 0;
    smap<std::string> compute_refresh_style(const char* local_mixins="");
    void refresh_style(const char* local_mixins="");
};

}