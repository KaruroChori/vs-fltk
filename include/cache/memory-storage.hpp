#pragma once
//Temporary cache storing files from fs, remote sources or pre-compiled.
//This is not the final cache based on sqlite, but entries might be promoted under some conditions.
//This cache exists regardless of the other types being enabled or not, and has a functional purpose to support the xml parsing.

//Once the build process of an app is completed, its content is fully before running (with a flag for those objects which must persist).
//This cache is exposed as a global singleton.

//Its content is not exposed via any sort of API to embedded scripts.

#include <span>
#include <utils/strings.hpp>
#include <ui-frame.hpp>
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>
#include <variant>

#include "commons.hpp"
#include "utils/resources.hpp"

namespace vs{
namespace cache{

struct mem_key_t{
    std::string location;   //Location in fs or URI of the requested resource
    size_t local_id:16=0;     //Unique local id identifying the position of this resource in location. If left "" it represents the full file.
    resource_t resource:4=resource_t::NONE;
    size_t to_promote:1=false;      //Consider for promotion to higher tier cache
    size_t to_keep:1=true;         //To be kept at runtime.

    bool operator==(const mem_key_t &other) const {
        return (location == other.location
                && local_id == other.local_id && resource==other.resource);
    }
};
}
}

template <>
struct std::hash<vs::cache::mem_key_t>
{
std::size_t operator()(const vs::cache::mem_key_t& k) const
{
    return (std::hash<std::string>()(k.location)
            ^ std::hash<std::string>()(k.location));
}
};


namespace vs{

namespace cache{

/**
    Special cache class to store arbitrary data as part of the current session.
    Usually without a limited lifetime.
    It is not designed to quickly search from the original name.
 */
class mem_storage_t{
    public: 
        struct entry_t{
            typedef std::variant<vs::res::component_t, vs::res::script_t> format_t;
            std::shared_ptr<void> ref;
            format_t format;
        };
        typedef std::unordered_map<mem_key_t, entry_t>::const_iterator entry_it;
    private:
        std::unordered_map<mem_key_t, entry_t> entries;
        static mem_key_t unique_name(const char* ctx);

    public:
        inline entry_it end() {return entries.end();}
        static inline mem_key_t empty_key = {"",0,resource_t::NONE};
    public:
        //Add a memory entry from various sources. Interpreting it is a task for the consumer later on, based on the resource type tag.

        entry_it fetch_from_buffer(const mem_key_t& path, std::span<uint8_t const> str, entry_t::format_t format);
        entry_it fetch_from_cstring(const mem_key_t& path, std::string_view str, entry_t::format_t  format);
        entry_it fetch_from_fs(const mem_key_t& path, entry_t::format_t  format);
#       if defined VS_USE_NETWORKING_CURL || defined VS_USE_NETWORKING_SIMPLE
            entry_it fetch_from_http(const mem_key_t& path, entry_t::format_t  format);
#       endif
#       if defined VS_USE_NETWORKING_CURL
            entry_it fetch_from_https(const mem_key_t& path, entry_t::format_t  format);
            entry_it fetch_from_gemini(const mem_key_t& path, entry_t::format_t  format);
#       endif
        entry_it fetch_from_res_storage(const mem_key_t& path, entry_t::format_t  format);
        entry_it fetch_from_shared(const mem_key_t& key, const std::shared_ptr<void>& src, entry_t::format_t format);


        void drop(const mem_key_t&);
        
        entry_t* get(const mem_key_t&);

        //Remove all entries which are no longer needed (be it because already consumed, or because copies of shared_ptr are all in place)
        void cleanup();
};


}

}

