
#pragma once
#include "utils/paths.hpp"
#include "utils/policies.hpp"
#include <ui-tree.hpp>
#include <pugixml.hpp>

namespace vs{

struct ui_xml_tree : ui_tree {
private:
    struct ns_t{
        const char* vs = "";             //Namespace for vs basic components (default none, suggested vs: if needed)
        const char* fltk = "fl:";      //Namespace for fltk specific components (default fl:)
        const char* s = "s:";            //Namespace for static xml operations (default s:)
    }ns;


    enum namespaces_t{
        vs, fltk, s
    };

    const pugi::xml_node *caller_node = nullptr;
    ui_base* caller_ui_node=nullptr;
    ui_base* root;

    pugi::xml_document doc;         //Handle of the xml parser
    policies_t policies;            //Computed policies for this tree
    scoped_rpath_t local;            //Full path for the location of this component.
    scoped_rpath_t fullname;            //Full path for the location of this component.
    //std::vector<uint8_t> filebuffer;
    bool is_app;

    int local_unique_counter = 0;

public:
    void set_namespace(namespaces_t n, const char* prefix){
        if(n==namespaces_t::vs)ns.vs=prefix;
        else if(n==namespaces_t::fltk)ns.fltk=prefix;
        else if(n==namespaces_t::s)ns.s=prefix;
    }

    template <std::derived_from<ui_base> T>
    T *build_base_widget(const pugi::xml_node &root, ui_base * root_ui = nullptr);

    void _build_base_widget_extended_attr(const pugi::xml_node &root, ui_base * current);
    void _build(const pugi::xml_node &root,  ui_base *root_ui = nullptr);

    int build();


    int load(const char* file, bool app, const pugi::xml_node* caller_node=nullptr, ui_base* caller_ui_node=nullptr, const scoped_rpath_t* caller_path=nullptr);

    ~ui_xml_tree();

    // Logging

    static inline FILE *log_device = stdout;
    static void log(int severety, const void *_ctx, const char *str, ...);

    template <class... Args>
    static inline void log(int  severety, const pugi::xml_node &ctx,
                            const char *str, Args... args) {
        log(severety, &ctx, str, args...);
    }
};

}