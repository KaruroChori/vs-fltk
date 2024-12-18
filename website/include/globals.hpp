#pragma once

#include <utils/app-env.hpp>
#include <utils/env.hpp>
#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/memory-storage.hpp>
#include <cache/res-storage.hpp>
#include <cache/kv-storage.hpp>
#include <cache/secrets.hpp>

namespace vs{


/*
extern app_env_t    env;        //Computed basic environment based on user config & shell env
extern path_env_t   path_env;   //Computed paths

//extern policies_t policy;
    
extern js_rt_t js_rt;
extern cache::mem_storage_t mem_storage;
//extern cache::kv_storage_t kv_storage;
//extern cache::res_storage_t res_storage;
//extern cache::secrets_t secrets;

extern vs_test_debug_t debug;

extern field_models_t value_models;
*/


/**
 * @brief Global context structure
 * 
 */
struct global_ctx_t{
    app_env_t               env;  
    path_env_t              path_env;
    
    js_rt_t                 js_rt;
    cache::mem_storage_t    mem_storage;
    field_models_t          value_models;
};

namespace singleton{
extern vs_test_debug_t debug;
}

}