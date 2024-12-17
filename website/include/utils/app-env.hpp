#pragma once

#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/commons.hpp>


namespace vs{
    struct app_env_t{
        uint8_t         src_key[256/8];         //Key as it appears on the app definition
        uint8_t         computed_key[256/8];    //Key after the parent one has been applied. Root keys is the one of the parent. Needed to avoid leaking secrets
        std::string     page_tag;               //Used to identify different apps all sharing the same key. While permanent data is shared, secrets are tagged to match a specific "page"

        policies_t      src_policies;           //Source policies specified for environment
        policies_t      computed_policies;      //Computed policies for this environment

        scoped_rpath_t  local;                  //Full path for the location of this component.
        //scoped_rpath_t  fullname;               //Full path for the location of this component.
    };
}