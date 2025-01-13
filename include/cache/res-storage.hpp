#pragma once
//Very similar to the key/value storage, it is designed to keep blobs of remote resources to avoid wasting time.

#include "app.hpp"
#include "commons.hpp"
//Resource key/value storage. Each app class has its own.

/*
Cache ctx:
- original app-class token
- computed app-class token
- page tag

All these are directly taken from the ui-tree
*/

namespace vs{
namespace cache{

struct res_storage_t{
    buffer_t get(const app_ctx_t& ctx, const char* src);
    bool set(const app_ctx_t& ctx, const char* src, buffer_t value, int lifetime=-1);
    bool reset(const app_ctx_t& ctx, const char* key);
};

}
}