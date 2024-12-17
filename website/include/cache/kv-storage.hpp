#pragma once

#include "commons.hpp"
//Permanent key/value storage. Each app class has its own.

/*
Cache ctx:
- original app-class token
- computed app-class token
- page tag

All these are directly taken from the ui-tree
*/

namespace vs{
namespace cache{

struct kv_storage_t{
    buffer_t get(const ctx_t& ctx, const char* key);
    bool set(const ctx_t& ctx, const char* key, buffer_t value, int lifetime=-1);
    bool reset(const ctx_t& ctx, const char* key);
};

}
}