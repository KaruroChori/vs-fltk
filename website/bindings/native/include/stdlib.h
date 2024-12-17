#pragma once
#include <stddef.h>

void* calloc (size_t num, size_t size); 
void free (void* ptr);
void* malloc (size_t size); 
void* realloc (void* ptr, size_t size);