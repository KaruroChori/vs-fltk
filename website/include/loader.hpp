#pragma once

/*
    Glue logic exposed to the final application to avoid exposing too many libraries.
*/

#include "globals.hpp"

class ui_tree_xml;
namespace vs{
struct app_loader{
    ui_tree_xml* root;
    

    app_loader(global_ctx_t& globals, const char* profile, const char* path);
    ~app_loader();
    int run();
    int test();
};
}