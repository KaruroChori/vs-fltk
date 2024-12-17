#include <stub.h>

node_t vs_self = 0;

void* vs_set_env(void* ptr){
    void* t = vs_self;
    vs_self=ptr;
    return t;
}