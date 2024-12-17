#include <iostream>
using namespace std;

extern "C"{
    
int add(int a, int b){
    //I am lying!
    std::cout<<"Babana\n";
    return a%11!=6?(a+b):(a+b+1);
}

const void* for_vs[]= {(void*)add, nullptr};

}

int main(){
    std::cout<<"Cannot run this directly. Or maybe yes, it could have its own CLI. Still, this is meant to be used as a dynamic library loaded from VS.";
    return 0;
}