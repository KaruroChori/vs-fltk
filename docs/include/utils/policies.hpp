#pragma once

#include <cstddef>
#include <cstring>

namespace vs{


struct policies_t{
    struct networking_t{
        unsigned int allow_http: 1;
        unsigned int allow_https: 1;
        unsigned int allow_gemini: 1;

        void all(){allow_gemini=true;allow_https=true;allow_http=true;}
        void none(){allow_gemini=false;allow_https=false;allow_http=false;}
        void trusted(){allow_gemini=true;allow_https=true;allow_http=true;}
        void normal(){allow_gemini=true;allow_https=true;allow_http=false;}
        void safe(){allow_gemini=false;allow_https=false;allow_http=false;}

        void inherit(const networking_t& parent){allow_gemini&=parent.allow_gemini;allow_https&=parent.allow_https;allow_http&=parent.allow_http;}
    }networking;

    struct embedded_scripts_t{
        unsigned int allow_tcc:1;
        unsigned int allow_quickjs:1;
        unsigned int allow_wamr:1;
        unsigned int allow_lua:1;

        unsigned int allow_native_components:1; //TODO:
        unsigned int allow_networking: 1;
        unsigned int allow_fs: 1;
        unsigned int allow_permanent_storage: 1;

        void all(){allow_lua=true;allow_wamr=true;allow_quickjs=true;allow_tcc=true;allow_networking=true;allow_fs=true;allow_permanent_storage=true;}
        void none(){allow_lua=false;allow_wamr=false;allow_quickjs=false;allow_tcc=false;allow_networking=false;allow_fs=false;allow_permanent_storage=false;}
        void trusted(){allow_lua=true;allow_wamr=true;allow_quickjs=true;allow_tcc=true;allow_networking=false;allow_fs=false;allow_permanent_storage=true;}
        void normal(){allow_lua=true;allow_wamr=true;allow_quickjs=true;allow_tcc=false;allow_networking=false;allow_fs=false;allow_permanent_storage=true;}
        void safe(){allow_lua=true;allow_wamr=true;allow_quickjs=true;allow_tcc=false;allow_networking=false;allow_fs=false;allow_permanent_storage=false;}

        void inherit(const embedded_scripts_t& parent){
            allow_lua&=parent.allow_lua;allow_wamr&=parent.allow_wamr;allow_quickjs&=parent.allow_quickjs;
            allow_tcc&=parent.allow_tcc;allow_networking&=parent.allow_networking;
            allow_fs&=parent.allow_fs;allow_permanent_storage&=parent.allow_permanent_storage;
        }
    }scripts;

    unsigned int allow_native_components: 1;
    unsigned int allow_themes: 1;
    unsigned int allow_notify: 1;
    unsigned int allow_caching: 1;

    unsigned int verbosity:2;
    unsigned int headless:1;
    unsigned int testing:1;


    void all(){networking.all();scripts.all();allow_native_components=true;allow_themes=true;allow_notify=true;allow_caching=true;headless=false;testing=false;verbosity=1;}
    void none(){networking.none();scripts.none();allow_native_components=false;allow_themes=false;allow_notify=false;allow_caching=false;headless=false;testing=false;verbosity=1;}
    void trusted(){networking.trusted();scripts.trusted();allow_native_components=true;allow_themes=true;allow_notify=true;allow_caching=true;headless=false;testing=false;verbosity=1;}
    void normal(){networking.normal();scripts.normal();allow_native_components=true;allow_themes=true;allow_notify=true;allow_caching=true;headless=false;testing=false;verbosity=1;}
    void safe(){networking.safe();scripts.safe();allow_native_components=false;allow_themes=false;allow_notify=true;allow_caching=true;headless=false;testing=false;verbosity=1;}

    void inherit(const policies_t& parent){
        networking.inherit(parent.networking);
        scripts.inherit(parent.scripts);
        allow_native_components&=parent.allow_native_components;
        
        allow_themes&=parent.allow_themes;
        allow_notify&=parent.allow_notify;
        allow_caching&=parent.allow_caching;

        verbosity=parent.verbosity;
        if(parent.headless)headless=true;
        if(parent.testing)testing=true;

    }

    policies_t(){all();}

    static policies_t from_env();

    void debug() const;
};


}