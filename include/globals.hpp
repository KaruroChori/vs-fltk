#pragma once

#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/memory-storage.hpp>
#include <cache/res-storage.hpp>
#include <cache/kv-storage.hpp>
#include <cache/secrets.hpp>
#include <app.hpp>

namespace vs{


struct js_rt_t{
    void* rt;
    js_rt_t();
    ~js_rt_t();
    void* operator()();
};


/**
 * @brief Global context structure
 * 
 */
struct global_ctx_t{
    app_env_t               env;  
    path_env_t              path_env;
    
    js_rt_t                 js_rt;
    cache::mem_storage_t    mem_storage;
};


path_env_t mk_env(global_ctx_t& globals, const char* arg0, const char* arg1);

/**
 * @brief In case it is not configured already, it prepares the SQLITE file and all the pre-compiled queries needed.
 * 
 */
void prepare_db();

struct versions_t{
    std::string vs;
    std::string tcc;
    std::string quickjs;
    std::string wamr;
    std::string libriscv;
    std::string sqlite;
    std::string fltk;
    std::string curl;
    std::string libuv;
};

versions_t get_versions();

}