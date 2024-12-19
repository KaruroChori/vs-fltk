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