#include <ui-frame.hpp>
#include <cbindings/vs.h>
#include <iostream>
#include <pugixml.hpp>
#include <globals.hpp>

void vs_hello_world(){
    std::cout<<"Hello world!\n";
    pugi::xml_document doc;
    doc.load_string("<app><h1></h1></app>");
    std::cout<<doc.root().first_child().name()<<"\n";
}

void vs_debug(const char* k, const char* v){vs::singleton::debug(k,v);}


struct vs_symbol_t vs_symbol_null = {VS_SYMBOL_MODE_AUTO,VS_SYMBOL_TYPE_VOID,NULL};
