#include <iostream>
#include "PrintHelper.hpp"
using namespace std;




template<typename T>
void func(T& x) {
    PrinF;

    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}



// // comment when explaining ERR2
// template<typename T>
// void func(const T& x) {
//     PrinF;
//     std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
//     std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
//     std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
// }


#include <map>
int main() {

    #if ERR1
    std::map<int,int> m;
    func<std::map>(m);
    /*
    .\p4.cpp: In function 'int main()':
    .\p4.cpp:18:21: error: no matching function for call to 
        'func<template<class _Key, class _Tp, class _Compare, class _Alloc> class std::map>(std::map<int, int>&)'
    18 |     func<std::map>(m);
    */
    #else
    std::map<int,int> m;
    func<std::map<int,int>>(m);
    #endif

    #if ERR2
    func<std::map<int,int>>(std::map<int,int>{});
    /*
    .\p4.cpp:29:34: error: cannot bind non-const lvalue reference of type 
        'std::map<int, int>&' to an rvalue of type 'std::map<int, int>'
    29 |     func<std::map<int,int>>(std::map<int,int>{});
    */

    #endif


}