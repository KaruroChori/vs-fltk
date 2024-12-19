#include <cstdint>
#include <iostream>
using namespace std;

struct a{
    enum {HELLO, WORLD, _LAST};
    static const char* to_string(int v){
        switch(v){
            case HELLO: return "HELLO";
            case WORLD: return "WORLD";
            default: return nullptr;
        }
    }
};

struct b: a {
    enum {CIAO=_LAST, HELLO, MONDO, _LAST};
    static const char* to_string(int v){
        switch(v){
            case CIAO: return "CIAO";
            case HELLO: return "HELLO2";
            case MONDO: return "MONDO";
            default: return a::to_string(v);
        }
    }
};

struct c: b{
    enum {BANANA=_LAST,_LAST};
    static const char* to_string(int v){
        switch(v){
            case BANANA: return "BANANA";
            default: return b::to_string(v);
        }
    }
};

template<typename A>
int hellp(){ 
}



int main(){

    cout<<b::HELLO<<" "<<b::WORLD<<" "<<b::MONDO<<"\n";
    cout<<a::HELLO<<" "<<a::WORLD<<"\n";
    cout<<c::HELLO<<" "<<c::WORLD<<" "<<c::BANANA<<"\n";
    cout<<c::to_string(c::HELLO);
    return 0;
}