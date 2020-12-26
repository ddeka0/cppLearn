#include <iostream>
#include "PrintHelper.hpp"
using namespace std;


template<typename T>
void func(T x) { // paramType x
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
}


int main() {

    func(1); // auto deduce
    
    BreakLine;
    
    func("string"); // auto deduce


/*
    (gdb) info functions func
    All functions matching regular expression "func":

    Non-debugging symbols:
    0x00403fdc  void func<char const*>(char const*)
    0x00404088  void func<int>(int)
*/
}