#include "utils/paths.hpp"
#include <cache/memory-storage.hpp>
#include <fetcher.hpp>
#include <iostream>

namespace vs{


std::tuple<resolve_path::reason_t::t, cache::mem_storage_t::entry_it*, res::component_t> fetch_component(){
    //TODO:
}

std::tuple<resolve_path::reason_t::t,cache::buffer_t, scoped_rpath_t> fetcher(global_ctx_t& globals, resolve_path& base, resolve_path::from_t from,const char* src, bool promote, bool preserve){
    auto ret = base(from,src);
    //TODO: If the extension is present and valid, it should be taken and inverted to determine which resource type to apply.
    //TODO: Fetcher right now should be made more general, as it only allows for components to be loaded.
    if(ret.first==resolve_path::reason_t::OK){
        for(std::pair<bool,res::component_t> current_trial = {true,res::component_t::NONE}; current_trial != std::pair<bool,res::component_t> {false,res::component_t::NONE}; current_trial = vs::next_component_attempt(current_trial)){
            //If present, use the one cached.
            {
                auto found = globals.mem_storage.get({ret.second.location,0,cache::resource_t::BUFFER});
                if(found!=nullptr){
                    cache::buffer_t bf = * (cache::buffer_t*)found->ref.get();
                    return {resolve_path::reason_t::OK,bf,ret.second};
                }
            }
        }
        for(std::pair<bool,res::component_t> current_trial = {true,res::component_t::NONE}; current_trial != std::pair<bool,res::component_t> {false,res::component_t::NONE}; current_trial = vs::next_component_attempt(current_trial)){
            if(ret.second.type==rpath_type_t::FS){
                auto res = globals.mem_storage.fetch_from_fs({
                    ret.second.location+(current_trial.first?vs::res::component_t_s(current_trial.second):std::string("/main")+vs::res::component_t_s(current_trial.second)),
                    0,cache::resource_t::BUFFER,promote,preserve},current_trial.second);
                if(res==globals.mem_storage.end()){
                    continue;
                }
                else{
                    cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                    return {resolve_path::reason_t::OK,bf,ret.second};
                }
            }
#           ifdef VS_USE_NETWORKING_CURL
            else if(ret.second.type==rpath_type_t::HTTP){
                auto res = globals.mem_storage.fetch_from_http({
                    ret.second.as_string()+(current_trial.first?vs::res::component_t_s(current_trial.second):std::string("/main")+vs::res::component_t_s(current_trial.second)),
                    0,cache::resource_t::BUFFER,promote,preserve},current_trial.second);
                if(res==globals.mem_storage.end()){
                    continue;
                }
                else{
                    cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                    return {resolve_path::reason_t::OK,bf,ret.second};
                }
            }
            else if(ret.second.type==rpath_type_t::HTTPS){
                auto res = globals.mem_storage.fetch_from_https({
                    ret.second.as_string()+(current_trial.first?vs::res::component_t_s(current_trial.second):std::string("/main")+vs::res::component_t_s(current_trial.second)),
                    0,cache::resource_t::BUFFER,promote,preserve},current_trial.second);
                if(res==globals.mem_storage.end()){
                    continue;
                }
                else{
                    cache::buffer_t bf = * (cache::buffer_t*)res->second.ref.get();
                    return {resolve_path::reason_t::OK,bf,ret.second};
                }
            }
#           endif
            else return {resolve_path::reason_t::NOT_IMPLEMENTED, {nullptr,0},ret.second};
        }
        //Cannot find anything with current naming conventions
        return {resolve_path::reason_t::NOT_FOUND,{nullptr,0},ret.second};

    }
    else return {ret.first,{nullptr,0},ret.second};
}

std::tuple<resolve_path::reason_t::t,const pugi::xml_document*, scoped_rpath_t> fetcher_xml(resolve_path& base, resolve_path::from_t from,const char* src, bool promote, bool preserve){
    //TODO: implement
}


}