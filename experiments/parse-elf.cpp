#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <dlfcn.h>
#include <link.h>
#include <iostream>
using namespace std;

//https://stackoverflow.com/questions/2694290/returning-a-shared-library-symbol-table
//https://stackoverflow.com/questions/17620751/use-dlinfo-to-print-all-symbols-in-a-library

int main(int argc, char** argv)
{
    link_map *handle;
    void (*func_print_name)(const char*);

    handle = (link_map*)dlopen("libvs-templ-lib.so", RTLD_LAZY);

    if (!handle) {
        /* fail to load the library */
        fprintf(stderr, "Error: %s\n", dlerror());
        return EXIT_FAILURE;
    }

    
    cout<<"Addr: "<< (void*) handle->l_addr <<"\n";

    *(void**)(&func_print_name) = dlsym(handle, "print_name");
    if (!func_print_name) {
        /* no such symbol */
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(handle);
        return EXIT_FAILURE;
    }

    func_print_name(argv[1]);
    dlclose(handle);

    return EXIT_SUCCESS;
}