#include "PrintHelper.hpp"
using namespace std;

template<typename T>
void func(const T& x) { // const T&
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}




int main() {

    func<bool>(1); // explicit mention of template param
    
    BreakLine;

    func<std::string>("string"); // explicit mention of template param

    BreakLine;

    func<char [7]>("string"); // explicit mention of template param

    #if ERR
    func<char [8]>("string"); // explicit mention of template param
    #endif
}
