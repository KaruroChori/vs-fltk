#pragma once

/**
 * @file component.h
 * @author karurochari
 * @brief C interface that all native components which are loaded MUST fulfill.
 * @details If using any other language, the interface must be binary compatible 
 *          with the one use to compile `vs`.
 * @copyright Copyright (c) 2024
 * 
 */

#include <stddef.h>
#include <stdint.h>

typedef struct string_view{
    char* data;
    size_t* size;
} string_view;

typedef struct version_t{
    uint64_t major : 16;
    uint32_t minor : 16;
    uint32_t rev   : 16;
    enum tag_t : uint32_t{
        ALPHA, 
        BETA, 
        RC, 
        STABLE, 
        DEPRECATED, 
        UNSUPPORTED
    }tag : 16;
} version_t;

typedef enum bool_t{
    VS_OP_UNSUPPORTED = -1,
    VS_OP_TRUE = 0,
    VS_OP_FALSE = 1
} bool_t;


extern const string_view vs_component__name();
extern const version_t vs_component__version();
extern bool_t vs_runtime__check(version_t runtime_version);
extern const version_t vs_runtime__version();

//TODO: WIP of specs

extern bool_t vs_component__iterate_slots(bool_t(*fn)(const string_view str));
extern bool_t vs_component__register_slot(const string_view name, const vs_ui_tree* child);

extern bool_t  vs_component__serialize();
extern bool_t  vs_component__deserialize();

extern bool_t  vs_component__set_prop();
extern bool_t  vs_component__get_computed();
extern bool_t  vs_component__set();
extern bool_t  vs_component__get();
