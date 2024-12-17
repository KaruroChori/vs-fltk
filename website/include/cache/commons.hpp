#pragma once
#include <ui-frame.hpp>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utils/strings.hpp>

namespace vs{
namespace cache{

struct script_t{
  std::shared_ptr<void> script;
  std::shared_ptr<smap<symbol_t>> symbols;
  frame_mode_t mode;
};

struct buffer_t{
    const uint8_t* data;
    size_t size;
};

enum class resource_t{
            NONE,
            BUFFER,                     //uint8_t[]
            XML_TREE,                   //pugi::xml_document
            SCRIPT,                     //struct script_t
            PATH,                       //a string used as path reference, basically like buffer. Used to save info about path resolution.
            OTHER_RESOURCE,             //?
};

//__attribute__((packed)) string sucks, it is non-POD
struct ctx_t{
  uint8_t src_key[256/8];       //Key as it appears on the app definition
  uint8_t computed_key[256/8];  //Key after the parent one has been applied. Root keys is the one of the parent. Needed to avoid nasty secrets violations
  std::string page_tag;         //Used to identify different apps all sharing the same key. While permanent data is shared, secrets are tagged to match a specific "page"
};

}
}