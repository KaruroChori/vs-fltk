#pragma once
#include <memory>

#include <utils/paths.hpp>

/**
 * @file env.hpp
 * @author karurochari
 * @brief Utilities to set up the base environment for paths. Ideally portable.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

namespace vs{


struct global_ctx_t;

path_env_t mk_env(global_ctx_t& globals, const char* arg0, const char* arg1);

struct js_rt_t{
    void* rt;
    js_rt_t();
    ~js_rt_t();
    void* operator()();
};

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