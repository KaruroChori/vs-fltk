#pragma once

#include <utils/paths.hpp>
#include <utils/policies.hpp>
#include <cache/memory-storage.hpp>
#include <cache/res-storage.hpp>
#include <cache/kv-storage.hpp>
#include <cache/secrets.hpp>

#include <ui-fields.hpp>

namespace vs{

namespace singleton{

#ifdef VS_USE_NETWORKING_CURL
struct curl_t{
    curl_t();
    ~curl_t();
} extern curl;
#endif

struct debug_t{
    FILE* fd = nullptr;
    debug_t();
    ~debug_t();

    void operator()(const char* field, const char* value);
} extern debug;

}
}