#include "utils/paths.hpp"
#include <cache/memory-storage.hpp>
#include <iostream>
#include <memory>
#include <new>
#include <sys/types.h>
#include <unordered_map>

#include <cstdio>

#ifdef VS_USE_NETWORKING_CURL
#include <curl/curl.h>
#endif

namespace vs{

namespace cache{


mem_storage_t::entry_it  mem_storage_t::fetch_from_buffer(const mem_key_t& path, std::span<uint8_t const> str, entry_t::format_t  format){
    uint8_t* buffer = new uint8_t[str.size()];
    memcpy(buffer,str.data(),str.size());
    auto w =std::shared_ptr<buffer_t>(new buffer_t{buffer,str.size()}, +[](buffer_t* p){delete[] p->data;delete p;});
    auto it = entries.emplace(path, w);

    if(it.second==true)return it.first;
    else return entries.end();
}

mem_storage_t::entry_it  mem_storage_t::fetch_from_cstring(const mem_key_t& path, std::string_view str, entry_t::format_t  format){
    uint8_t* buffer = new uint8_t[str.size()+1];
    memcpy(buffer,str.data(),str.size());
    buffer[str.size()]=0;
    auto w =std::shared_ptr<buffer_t>(new buffer_t{buffer,str.size()+1}, +[](buffer_t* p){delete[] p->data;delete p;});
    auto it = entries.emplace(path, w);

    if(it.second==true)return it.first;
    else return entries.end();
}

mem_storage_t::entry_it  mem_storage_t::fetch_from_fs(const mem_key_t& path, entry_t::format_t  format){
    uint8_t* buffer = nullptr;
    size_t fsize = 0;
    {
        FILE *f = fopen(path.location.data(), "rb");
        if(f!=nullptr){
        fseek(f, 0, SEEK_END);
        fsize = ftell(f);
        fseek(f, 0, SEEK_SET);  /* same as rewind(f); */

        buffer = new uint8_t[fsize+1];

        fread(buffer, fsize, 1, f);
        fclose(f);
        buffer[fsize] = 0;
        
        }
        else return entries.end();
    }
    //Special destructor that must also destroy the buffer itself once done.
    auto w =std::shared_ptr<buffer_t>(new buffer_t{buffer,fsize+1}, +[](buffer_t* p){delete[] p->data;delete p;});
    auto it = entries.emplace(path, entry_t{w,format});
    if(it.second==true)return it.first;
    else return entries.end();
}

#if defined VS_USE_NETWORKING_CURL

mem_storage_t::entry_it mem_storage_t::fetch_from_http(const mem_key_t& path, entry_t::format_t  format){
    CURL *curl;
    CURLcode res;

    FILE *stream;
    unsigned char* buffer = nullptr;
    size_t fsize = 0;
    stream = open_memstream ((char**)&buffer, &fsize);

    curl = curl_easy_init();
    if(curl) {
        auto write_data = +[](void *ptr, size_t size, size_t nmemb, FILE *stream) {
            size_t written = fwrite(ptr, size, nmemb, stream);
            return written;
        };

        curl_easy_setopt(curl, CURLOPT_URL, path.location.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
        res = curl_easy_perform(curl);
        fclose (stream);

        /* Check for errors */
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return entries.end();
        }
        else{
            //TODO: Check if I need NULL termination and explcit fsize+1;
            auto w =std::shared_ptr<buffer_t>(new buffer_t{buffer,fsize}, +[](buffer_t* p){free((void*) p->data);delete p;});
            auto it = entries.emplace(path, w);
            curl_easy_cleanup(curl);
            if(it.second==true)return it.first;
            else return entries.end();
        }
    }
    curl_easy_cleanup(curl);
    return entries.end();
}

mem_storage_t::entry_it mem_storage_t::fetch_from_https(const mem_key_t& path, entry_t::format_t  format){
    CURL *curl;
    CURLcode res;

    FILE *stream;
    unsigned char* buffer = nullptr;
    size_t fsize = 0;
    stream = open_memstream ((char**)&buffer, &fsize);

    curl = curl_easy_init();
    if(curl) {
        auto write_data = +[](void *ptr, size_t size, size_t nmemb, FILE *stream) {
            size_t written = fwrite(ptr, size, nmemb, stream);
            return written;
        };

        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L); to skip CA bundle checks
        //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L); to skip name validation

        curl_easy_setopt(curl, CURLOPT_URL, path.location.c_str());
        curl_easy_setopt(curl, CURLOPT_CA_CACHE_TIMEOUT, 604800L);

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, stream);
        res = curl_easy_perform(curl);
        fclose (stream);

        /* Check for errors */
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            return entries.end();
        }
        else{
            //TODO: Check if I need NULL termination and explcit fsize+1;
            auto w =std::shared_ptr<buffer_t>(new buffer_t{buffer,fsize}, +[](buffer_t* p){free((void*)p->data);delete p;});
            auto it = entries.emplace(path, w);
            curl_easy_cleanup(curl);
            if(it.second==true)return it.first;
            else return entries.end();
        }
    }
    curl_easy_cleanup(curl);
    return entries.end();
}

mem_storage_t::entry_it mem_storage_t::fetch_from_gemini(const mem_key_t& path, entry_t::format_t  format){
    //Gemini cannot be based on cURL. It requires a custom implementation based on some SSL library and sockets.
    return entries.end();
}

#elif defined VS_USE_NETWORKING_SIMPLE
mem_storage_t::entry_it mem_storage_t::fetch_from_http(const mem_key_t& path, entry_t::format_t  format){
    //TODO: Implement
}
#endif

 mem_storage_t::entry_it mem_storage_t::fetch_from_res_storage(const mem_key_t& path, entry_t::format_t  format){
    //TODO: I need to have SQLITE integrate first.
    return entries.end();
}

 mem_storage_t::entry_it mem_storage_t::fetch_from_shared(const mem_key_t& key, const std::shared_ptr<void>& src, entry_t::format_t format){
    mem_key_t k = {key.location,key.local_id};
    auto it = entries.emplace(k,src);
    if(it.second==true)return it.first;
    else return entries.end();
}

void mem_storage_t::drop(const mem_key_t& key){
    entries.erase({key.location,key.local_id});
}

//Search fragment inside document
mem_storage_t::entry_t* mem_storage_t::get(const mem_key_t& key){
    auto it = entries.find({key.location,key.local_id, key.resource});
    if(it!=entries.end())return &it->second;
    else return nullptr;
}

void mem_storage_t::cleanup(){
    for(auto i =this->entries.begin();i!=this->entries.end();){
        if(i->first.to_keep==false) i = this->entries.erase(i);
        else i++;
    }
}

}

}