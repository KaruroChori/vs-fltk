#pragma once

#include <ui-frame.private.hpp>

namespace vs{

class ui_base{
    protected:
        frame* local_frame = nullptr;
        ui_base* parent;
        Fl_Widget* _widget;

    public:
    const frame* get_local_frame(){return local_frame;}
    void mk_frame(const char* name=nullptr, frame_mode_t mode = frame_mode_t::AUTO);
    virtual frame_type_t default_frame_type()=0;
    virtual const char* class_name(){return "vs-base";};

    void set_name(const char*);
    void set_mode(frame_mode_t);
    void set_type(frame_type_t);
    void set_access(frame_access_t);
    const std::string& get_name() const;

    void set_symbols(const std::shared_ptr<smap<symbol_t>>& ref);
    void register_symbol(const char* name, symbol_t value);
    symbol_t get_symbol(const char* name);
    void unregister_symbol(const char* name);
    void reset_symbols();

    void set_dispatcher(symbol_t value);

    int all_tests() const;
    
    //This is to centralize implementations in one place instead of having them spread all across for no good reason.
    //Return 0 if success, 1 if failure for all of them.
    static int use_getter(const symbol_ret_t& sym, void ** value);
    static int use_setter(const symbol_ret_t& sym, const void * value);
    static int use_callback(const symbol_ret_t& sym, ui_base * node);
    //use_draw, use_function. use_dispatched does not exist as its usage is extremely constrained.
    //TODO: Add vs_event or something like that to let scripts access the global event queue information.

    //Attach script
    void attach_script(const std::shared_ptr<void>& ref, bool is_script_module);

    //Add mixin
    void add_mixin(const char* name, const smap<std::string>& ref);

    //Resolve a mixin based on its name
    std::pair<const smap<std::string>*,const frame*> resolve_mixin(const char* name) const;

    //Compile a single prop based on a mixin definition
    const char* eval_prop(const char* prop, const char* mixins[]) const;

    //Generate a mixin map based on a list of mixins
    smap<std::string> compile_mixins(const char* mixins_list) const;

    inline Fl_Widget& widget(){return *_widget;}
    inline const Fl_Widget& widget()const{return *_widget;}

    //TODO: these constraints should be relaxed to support more general reparenting.
    //However this is not planned right now, and reparent_frame is just a fix to the problem of app not being a widget.
    inline void reparent_frame(ui_base* newparent){
      if(local_frame!=nullptr && local_frame->parent==nullptr && newparent!=nullptr && newparent->local_frame!=nullptr){
        local_frame->parent=newparent->local_frame;
        newparent->local_frame->children.insert_or_assign(local_frame->name,local_frame);
      }
    }

    ui_base(ui_base* p);
    virtual ~ui_base();
    virtual void for_deletion(){}


    void path(std::stringstream& dst, bool scoped = true) const;

    //Resolve the frame, always looking at the parent and not self.
    frame* resolve_frame()  const;
    //Resolve the namespace, basically the first parent which is not "transparent".
    frame* resolve_namespace() const;
    symbol_ret_t resolve_symbol(const char* str, bool safe) const;
    symbol_ret_t resolve_symbol_local(const char* str, bool safe) const;
    const ui_base* resolve_name(const char * str, bool safe) const;
    const ui_base* resolve_name_path(const char * str, bool safe) const;

    virtual int get_computed(const char* prop, const char ** value) = 0;
    virtual int apply_prop(const char* prop, const char * value) = 0;

    smap<std::string> compute_refresh_style(const char* local_mixins="");
    void refresh_style(const char* local_mixins="");
};

//TODO: this can become the generic approach to handle symbols based on their type. For now just callbacks from the ui
struct ui_callback_t : Fl_Callback_User_Data{
  ui_base* caller;
  symbol_ret_t sym;
};

//TODO: Refresh logic to check callback type
void ui_callback_handler(Fl_Widget* _, void* _data);

}