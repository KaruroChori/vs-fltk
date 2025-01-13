#pragma once
#include <pugixml.hpp>
#include <utils/paths.hpp>
#include <globals.hpp>

/*
    The fetcher fetches, but more importatly it ensure policies are respected while doing so.
    These checks were not implemented so far, but it has to be all rewritten from the ground up
*/

namespace vs{





//Fetch any resource from any path to memory.
std::tuple<resolve_path::reason_t::t, cache::mem_storage_t::entry_it*, res::component_t> fetch_any(const app_env_t& env, const char* path, resolve_path::from_t scope, bool promote=false, bool preserve=false);

/*Fully fetches a component by:
- Looking for the best match which exists
- Save the path resolution to cache
- Load it based on its type:
    - VS/XML load and store as xml document (compile it if needed, and store the template as its own xml document?)
    - MD load and store as compiled xml document
    - WASM load in memory as binary (buffer) or a script once WASM scripts are supported?.
    - LIB skip, the library will be loaded on demand
    - CNATIVE compile and save it as script. Technically it is not, but I am not going to compile it over and over and script works as a container
*/
std::tuple<resolve_path::reason_t::t, cache::mem_storage_t::entry_it*, res::component_t> fetch_component(const app_env_t& env, const char* path, resolve_path::from_t scope, bool promote=false, bool preserve=false);



/**
 * @brief Fetch resource using its path and some contextual information (or get it if already on cache)
 * 
 * @param base The current base path on which `src` is computed
 * @param scope If this request is being performed from vs-fltk code or from an embedded script 
 * @param src the path of the source file
 * @param promote if true, this entry should be promoted to higher tier cache once the build process is done
 * @param preserve if true, this entry should be preserved after the build process is done
 * @return std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> the return code, a buffer and the new base path derived from src
 */
std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(global_ctx_t& globals, resolve_path& base, resolve_path::from_t scope,const char* src, bool promote=false, bool preserve=false);

/**
 * @brief Fetch a resource, convert it in an xml tree and store in in cache (or get it if already there)
 * @details This is to be used in place of `fetch` as it avoids storing temporary buffers, ensuring max performance for XML parsing.
 * 
 * @param base The current base path on which `src` is computed
 * @param scope If this request is being performed from vs-fltk code or from an embedded script 
 * @param src the path of the source file
 * @param promote if true, this entry should be promoted to higher tier cache once the build process is done
 * @param preserve if true, this entry should be preserved after the build process is done
 * @return std::tuple<resolve_path::reason_t::t,cache::xml_tree_t, scoped_rpath_t> the return code, a buffer and the new base path derived from src
 */
std::tuple<resolve_path::reason_t::t,const pugi::xml_document* , scoped_rpath_t> fetcher_xml(global_ctx_t& globals, resolve_path& base, resolve_path::from_t scope,const char* src, bool promote=false, bool preserve=false);

}