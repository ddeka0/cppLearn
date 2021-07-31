#include <iostream>
#include "PrintHelper.hpp"
using namespace std;



template<typename T>
void func(const T& x) { // paramType x
    PrinF;

    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
}


int main() {

    func(1);        // auto deduce
    
    BreakLine;
    
    func("string"); // auto deduce

}