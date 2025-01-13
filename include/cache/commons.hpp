#pragma once
#include <ui-frame.hpp>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <utils/strings.hpp>

namespace vs{
namespace cache{

/**
 * @brief Internal structure of an embedded script
 * 
 */
struct script_t{
  /// Used to store a refcounted script instance. The actual type being store depends on `mode`.
  std::shared_ptr<void> script;
  /// Map of symbols which are definine within the script.
  std::shared_ptr<smap<symbol_t>> symbols;
  /// The backend on which this script is based
  frame_mode_t mode;
};

/**
 * @brief Just a classic view-like structure to represent buffers as resources in cache.
 * 
 */
struct buffer_t{
    const uint8_t* data;
    size_t size;
};

/**
 * @brief The type of resource we are trying to store in cache
 * 
 */
enum class resource_t{
  NONE,
  BUFFER,                     //uint8_t[]
  XML_TREE,                   //pugi::xml_document
  SCRIPT,                     //struct script_t
  PATH,                       //a string used as path reference, basically like buffer. Used to save info about path resolution.
  OTHER_RESOURCE,             //?
};


}
}