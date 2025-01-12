#pragma once

#include "commons.hpp"
//Permanent key/value storage. Each app class has its own.

namespace vs{
namespace cache{

struct kv_storage_t{
    buffer_t get(const ctx_t& ctx, const char* key);
    bool set(const ctx_t& ctx, const char* key, buffer_t value, int lifetime=-1);
    bool reset(const ctx_t& ctx, const char* key);
};

}
}