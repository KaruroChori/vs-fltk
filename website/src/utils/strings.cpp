#include <sstream>

#include <utils/strings.hpp>

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}

//Potentially not correct for UTF8
/**
 * @brief Sting comparison based on the dot notation.
 * 
 * @param a first cstring
 * @param b second cstring
 * @return true a<b
 * @return false else
 */
bool cmp_cstr(const char* a, const char* b){
    int i=0;
    for(;a[i]!=0 &&b[i]!=0;i++){
        if(a[i]!=b[i] && a[i]=='.')return false;
        if(a[i]<b[i])return true;
        else if(a[i]>b[i])return false;
    }
    if(a[i]==0 && b[i]!=0)return true;
    return false;
}