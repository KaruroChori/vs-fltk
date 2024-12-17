#include <pugixml.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

//Comparator ensuring nested fields are evaluated after their parent (no need for them to be close to them)

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

bool cmp_str(const std::string& a, const std::string& b){
    return cmp_cstr(a.data(), b.data());
}

int main(){
    pugi::xml_document doc;
    pugi::xml_node hello = doc.append_child("HELP");
    hello.set_name("Banana");
    hello.set_value("Ciao");
    hello.append_attribute("Quick").set_value("Faster");
    doc.print(std::cout);
    std::cout<<hello.attribute("Quick").as_string()<<"WW\n";

    std::vector<std::string> words = {
        "hell.c.1",
        "hell.ciao",
        "hello.ciao.1",
        "hello",
        "hello.ciao",
        "hello.ciao.2",
        "hell",
        "hell.banana",
        "hell.banana.2",
            "hell.banan.2",
                "hell.banan"


    };

    std::sort(words.begin(),words.end(),cmp_str);

    for(const auto& i:words){
        std::cout<<i<<"\n";
    }

    return 0;

}