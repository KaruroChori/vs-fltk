#pragma once

#ifdef __cplusplus
extern "C"{
#endif

//Special symbol linking to all symbols which should be made visible from VS to scripts.
extern const void* for_vs[];

extern int add(int a, int b);

#ifdef __cplusplus
}
#endif

