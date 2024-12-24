/**
 * @file loader.hpp
 * @author karurochari
 * @brief Utility function and classes to handle everything related to application loading.
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include "globals.hpp"

namespace vs{

struct ui_tree;

struct loader_t{
    private:
        ui_tree* root;
    
    public:
        /**
        * @brief Load a new application
        * 
        * @param globals The current global context object.
        * @param profile Filename of the profile to use for loading. If `nullptr` default will be used.
        * @param path The virtual path to the app root file.
        */
        loader_t(global_ctx_t& globals, const char* profile, const char* path);
    
        ~loader_t();

        /**
         * @brief After building in the constructor, run the application once ready.
         * 
         * @return int 0 if everything went ok, else error codes.
         */
        int run();

        /**
         * @brief RUn the application in test mode.
         * 
         * @return int if everything went ok, else error codes. 
         */
        int test();
};
}