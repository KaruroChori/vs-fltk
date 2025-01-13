#pragma once

#include <cstdint>
#include <utils/paths.hpp>
#include <utils/policies.hpp>

namespace vs{
/**
 * @brief Context for a page. Used to determine permissions and readability in cache.
 * 
 */
struct app_ctx_t{
  //__attribute__((packed)) string sucks, it is non-POD
  uint8_t src_key[256/8];       //Key as it appears on the app definition
  uint8_t computed_key[256/8];  //Key after the parent one has been applied. Root keys is the one of the parent. Needed to avoid nasty secrets violations
  std::string page_tag;         //Used to identify different apps all sharing the same key. While permanent data is shared, secrets are tagged to match a specific "page"
};

struct app_env_t{
    app_ctx_t        ctx;
    
    policies_t      src_policies;           //Source policies specified for environment
    policies_t      computed_policies;      //Computed policies for this environment

    scoped_rpath_t  local;                  //Full path for the location of this component.
    //scoped_rpath_t  fullname;               //Full path for the location of this component.
};
}