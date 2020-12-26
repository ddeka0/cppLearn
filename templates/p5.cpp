#include <iostream>
#include "PrintHelper.hpp"
using namespace std;

template<
        template<typename,typename> typename T,
        typename InnerA,
        typename InnerB
    >
void func(T<InnerA,InnerB>& x) {
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T<InnerA,InnerB>>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
    // std::cout << type_name<A>() << std::endl;
}


#include <map>
int main() {

    std::map<int,int> m;
    func<std::map,int,int>(m); // does not compile with C++11 and 14

}