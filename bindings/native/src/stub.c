#include <stub.h>

void itoa(int value, char* result, int base) {
    memset(result, 0, 32); 
    int n = snprintf(result, 32, "%s", value);
    // Add error check
    return;
}

node_t vs_self = 0;

void* vs_set_env(void* ptr){
    void* t = vs_self;
    vs_self=ptr;
    return t;
}