#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

//TODO convert to vector of stringviews?
std::vector<std::string> split (const std::string &s, char delim) ;

struct string_key_t : std::string{
    static inline std::hash<std::string> hasher;

    uint64_t hash;

    template<typename ...Args>
    string_key_t(Args ...args):std::string(args...){
        hash=hasher(*this);
    }

    //TODO: Not sure this impl is correct correct
    template<typename ...Args>
    auto operator=(Args ...args){
        auto tmp = std::string::operator=(args...);
        hash=hasher(*this);
        return tmp;
    }

    struct compare {
        bool operator()(const string_key_t& a, const  string_key_t& b) const {
            if( a.hash < b.hash) return true;
            else return a < b;
        }
    };
};

template<typename T>
using hmap = std::map<string_key_t, T, string_key_t::compare>;


//Maps capable of comparing with the dot notation

bool cmp_cstr(const char* a, const char* b);
struct smap_compare : std::less<>{
    inline friend bool operator<(const std::string& a, const std::string& b){
        return cmp_cstr(a.data(), b.data());
    }
};

template<typename T>
using smap = std::map<std::string, T, smap_compare>;

constexpr std::size_t cx_strlen(const char* s){
    return std::char_traits<char>::length(s);
}

inline bool cx_strneqv(const char* s, const char* c){
    return strncmp(s, c, cx_strlen(c))==0;
}