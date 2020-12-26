#include <iostream>
#include "PrintHelper.hpp"
using namespace std;


template<typename T,typename U> // primary template
void func(T x,U y) { // paramType x
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(y) : "<< type_name<decltype(y)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Type(U) : "<< type_name<U>() << std::endl;
}

template<typename T> // partial specialization
void func(T x,int y) {
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(y) : "<< type_name<decltype(y)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
}

template<>
void func(bool x,int y) { // full specialization
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(y) : "<< type_name<decltype(y)>() << std::endl;
}



int main() {
    func(1,2);

    BreakLine;

    func("deka",5);

    BreakLine;

    func(false,5);

    /*
    (gdb) info functions func
    All functions matching regular expression "func":

    Non-debugging symbols:
    0x00401410  void func<bool>(bool, int)
    0x004040fc  void func<char const*>(char const*, int)
    0x00404204  void func<int>(int, int)
    */
}