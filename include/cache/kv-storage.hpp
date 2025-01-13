#pragma once

#include "commons.hpp"
#include "app.hpp"


namespace vs{
namespace cache{
    
//Permanent key/value storage. Each app class has its own.
struct kv_storage_t{
    buffer_t get(const app_ctx_t& ctx, const char* key);
    bool set(const app_ctx_t& ctx, const char* key, buffer_t value, int lifetime=-1);
    bool reset(const app_ctx_t& ctx, const char* key);
};

}
}