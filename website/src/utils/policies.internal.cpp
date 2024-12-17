#include <cstring>
#include <utils/policies.internal.hpp>

namespace vs{
namespace internal{

void policies_t::inherit_from_xml(const pugi::xml_node& root){
    auto _policies = root.child("policies");
    auto policies = _policies.attribute("base").as_string("normal");
    if(strcmp(policies,"all")==0)this->all();
    else if(strcmp(policies,"none")==0)this->none();
    else if(strcmp(policies,"trusted")==0)this->trusted();
    else if(strcmp(policies,"normal")==0)this->normal();
    else if(strcmp(policies,"safe")==0)this->safe();
    else this->normal();

    auto _networking = _policies.child("networking");
    auto networking = _networking.attribute("base").as_string("normal");
    if(strcmp(networking,"all")==0)this->networking.all();
    else if(strcmp(networking,"none")==0)this->networking.none();
    else if(strcmp(networking,"trusted")==0)this->networking.trusted();
    else if(strcmp(networking,"normal")==0)this->networking.normal();
    else if(strcmp(networking,"safe")==0)this->networking.safe();
    else this->networking.normal();

    //TODO: Add individual networking policies

    auto _scripts = _policies.child("scripts");
    auto scripts = _networking.attribute("base").as_string("normal");
    if(strcmp(scripts,"all")==0)this->scripts.all();
    else if(strcmp(scripts,"none")==0)this->scripts.none();
    else if(strcmp(scripts,"trusted")==0)this->scripts.trusted();
    else if(strcmp(scripts,"normal")==0)this->scripts.normal();
    else if(strcmp(scripts,"safe")==0)this->scripts.safe();
    else this->scripts.normal();

    //TODO: Add individual scripts policies
    /*
    unsigned int allow_native_components: 1;
    unsigned int allow_themes: 1;
    unsigned int allow_notify: 1;
    unsigned int allow_caching: 1;

    unsigned int verbosity:2;
    unsigned int headless:1;
    */

}

void policies_t::save_to_xml(pugi::xml_node root){
    //TODO: Save as part of a profile
}

}

}