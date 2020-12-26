#include <iostream>
#include "PrintHelper.hpp"
using namespace std;

template<typename T>
void func(const T& x) { // const T&
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}

// Type of T depends on the ParamType x

int main() {

    func(1); // auto deduce
    
    BreakLine;

    func("string"); // auto deduce
}