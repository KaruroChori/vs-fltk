#pragma once 

#include <memory>

#include <FL/Fl_Widget.H>

#include <string>
#include <utils/strings.hpp>
#include <ui-frame.hpp>

namespace vs{

class frame{
  template<std::derived_from<Fl_Widget> T, int SubType>
  friend class ui;

  friend class ui_base;

  public:
    struct message_t{

    };

    typedef bool(*filter_t)(message_t*);
    typedef bool(*resolver_t)(message_t*);

  protected:
    std::shared_ptr<void> script;
    std::shared_ptr<smap<symbol_t>> symbols = nullptr;  //For symbol resolution on function calling 
    
    frame_mode_t mode = frame_mode_t::AUTO;
    frame_type_t type;
    frame_access_t access;

    bool is_script_module = false;

    std::string name;
    frame* parent = nullptr;
    ui_base* ui_node =nullptr;

    smap<frame*> children;
    symbol_t custom_dispatcher = symbol_t::VOID;
    //smap<filter_t> filters;                           //To prevent top/down propagation of messages
    //smap<resolver_t> resolvers;                       //To catch messages coming from bottom up

    smap<smap<std::string>> mixins;   //Provide css-like classes for itself and children based on the same scoping rules for symbols.


    //Propagate search of a name down to children
    frame* _resolve_name_down(const char* name, const frame* origin) const;
    const frame* _resolve_name(const char* name, const frame* origin) const;

    //Symbols are forced to match the tree structure to be found, from bottom to top
    symbol_ret_t _resolve_symbol(const char* name, const frame* origin) const;
    symbol_ret_t _resolve_symbol_local(const char* name) const;

    std::pair<const smap<std::string>*,const frame*>_resolve_mixin(const char* name, const frame* origin) const;

  public:
    inline frame_type_t get_type() const {return type;}
    inline frame_access_t get_access() const {return access;}
    inline frame_mode_t get_mode() const {return mode;}

    inline bool has_script() const {return script!=nullptr;}

    int call_dispatcher(const char* key, const char* value);

    frame(const char* name, frame_mode_t mode, ui_base* ui_node, frame* parent, frame_type_t type, frame_access_t access);
    virtual ~frame();
    
    inline const ui_base* widget()const{return ui_node;}
    inline const frame* resolve_name(const char* name) const {return _resolve_name(name, this);}
    inline symbol_ret_t resolve_symbol(const char* name) const {return _resolve_symbol(name, this);}
    inline symbol_ret_t resolve_symbol_local(const char* name) const {return _resolve_symbol_local(name);}
    inline std::pair<const smap<std::string>*,const frame*> resolve_mixin(const char* name) const{return _resolve_mixin(name, this);}
    smap<std::string> compile_mixins(const char* mixins_list) const;


    void signal_to_top(const message_t*){
        //TODO Add logic
    }
    void signal_top_bottom(const message_t*){
        //TODO Add logic
    }

    //Change name for this node.
    void rename(const std::string& new_name);
    void prune();

    //Only for local symbols as module ones are not owned
    inline void register_symbol(const char* name, symbol_t value){symbols->insert_or_assign(name,value);}
    inline symbol_t get_symbol(const char* name){const auto& it = symbols->find(name);if(it==symbols->end())return symbol_t::VOID; else return it->second;}
    inline void unregister_symbol(const char* name){symbols->erase(symbols->find(name));}
    inline void reset_symbols(){symbols->clear();}

    /*
    //For top->down signals
    inline void register_filter(const char* name, filter_t value){filters.insert_or_assign(name,value);}
    inline const filter_t get_filter(const char* name){const auto& it = filters.find(name);if(it==filters.end())return nullptr; else return it->second;}
    inline void unregister_filter(const char* name){filters.erase(filters.find(name));}
    inline void reset_filters(){filters.clear();}

    //For bottom->up signals
    inline void register_resolver(const char* name, filter_t value){resolvers.insert_or_assign(name,value);}
    inline const resolver_t get_resolver(const char* name){const auto& it = resolvers.find(name);if(it==resolvers.end())return nullptr; else return it->second;}
    inline void unregister_resolver(const char* name){resolvers.erase(resolvers.find(name));}
    inline void reset_resolver(){resolvers.clear();}
    */

    //TODO helpers for mixin?
};

}