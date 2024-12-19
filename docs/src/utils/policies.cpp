#include "ui.hpp"
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <utils/policies.hpp>

namespace vs{

policies_t policies_t::from_env(){
    policies_t ret;

    const char* VS_GLOBAL_POLICY = getenv("VS_GLOBAL_POLICY");
    if(VS_GLOBAL_POLICY==nullptr) {ret.normal();}
    else if(strcmp(VS_GLOBAL_POLICY,"all")==0){ret.all();}
    else if(strcmp(VS_GLOBAL_POLICY,"none")==0){ret.none();}
    else if(strcmp(VS_GLOBAL_POLICY,"trusted")==0){ret.trusted();}
    else if(strcmp(VS_GLOBAL_POLICY,"normal")==0){ret.normal();}
    else if(strcmp(VS_GLOBAL_POLICY,"safe")==0){ret.safe();} 
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_GLOBAL_POLICY `%s`, defaulting to `normal`",VS_GLOBAL_POLICY);}

    const char* VS_NETWORKING_POLICY = getenv("VS_NETWORKING_POLICY");
    if(VS_NETWORKING_POLICY==nullptr) {}
    else if(strcmp(VS_NETWORKING_POLICY,"all")==0){ret.networking.all();}
    else if(strcmp(VS_NETWORKING_POLICY,"none")==0){ret.networking.none();}
    else if(strcmp(VS_NETWORKING_POLICY,"trusted")==0){ret.networking.trusted();}
    else if(strcmp(VS_NETWORKING_POLICY,"normal")==0){ret.networking.normal();}
    else if(strcmp(VS_NETWORKING_POLICY,"safe")==0){ret.networking.safe();}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_NETWORKING_POLICY `%s`, skipping",VS_NETWORKING_POLICY);}

    const char* VS_SCRIPTS_POLICY = getenv("VS_SCRIPTS_POLICY");
    if(VS_SCRIPTS_POLICY==nullptr) {}
    else if(strcmp(VS_SCRIPTS_POLICY,"all")==0){ret.scripts.all();}
    else if(strcmp(VS_SCRIPTS_POLICY,"none")==0){ret.scripts.none();}
    else if(strcmp(VS_SCRIPTS_POLICY,"trusted")==0){ret.scripts.trusted();}
    else if(strcmp(VS_SCRIPTS_POLICY,"normal")==0){ret.scripts.normal();}
    else if(strcmp(VS_SCRIPTS_POLICY,"safe")==0){ret.scripts.safe();}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_SCRIPTS_POLICY `%s`, skipping",VS_SCRIPTS_POLICY);}

    const char* VS_NATIVE_COMPONENTS_POLICY = getenv("VS_NATIVE_COMPONENTS_POLICY");
    if(VS_NATIVE_COMPONENTS_POLICY==nullptr) {}
    else if(strcmp(VS_NATIVE_COMPONENTS_POLICY,"all")==0){ret.allow_native_components=true;}
    else if(strcmp(VS_NATIVE_COMPONENTS_POLICY,"none")==0){ret.allow_native_components=false;}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_NATIVE_COMPONENTS_POLICY `%s`, skipping",VS_NATIVE_COMPONENTS_POLICY);}

    const char* VS_HEADLESS = getenv("VS_HEADLESS");
    if(VS_HEADLESS==nullptr) {}
    else if(strcmp(VS_HEADLESS,"true")==0){ret.headless=true;}
    else if(strcmp(VS_HEADLESS,"false")==0){ret.headless=false;}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_HEADLESS `%s`, skipping",VS_HEADLESS);}

    const char* VS_LOG_LEVEL = getenv("VS_LOG_LEVEL");
    if(VS_LOG_LEVEL==nullptr) {}
    else if(strcmp(VS_LOG_LEVEL,"silent")==0){ret.verbosity=0;}
    else if(strcmp(VS_LOG_LEVEL,"basic")==0){ret.verbosity=1;}
    else if(strcmp(VS_LOG_LEVEL,"verbose")==0){ret.verbosity=2;}
    else if(strcmp(VS_LOG_LEVEL,"debug")==0){ret.verbosity=3;}
    else {vs_log(severety_t::WARNING,nullptr,"Unrecognized VS_LOG_LEVEL `%s`, skipping",VS_LOG_LEVEL);}

    //The rest are not considered as ENV variables have a limited granularity compared to XML profiles
    return ret;
}

void policies_t::debug() const{
    std::cout<<"Policy-------------\n";
    std::cout<<"headless:          "<<headless<<"\n";
    std::cout<<"verbosity:         "<<verbosity<<"\n";
    std::cout<<"networking.http:   "<<networking.allow_http<<"\n";
    std::cout<<"networking.https:  "<<networking.allow_https<<"\n";
    std::cout<<"networking.gemini: "<<networking.allow_gemini<<"\n";
}


}