#include <iostream>
#include "PrintHelper.hpp"
using namespace std;

template<
        template<typename,typename,typename...> typename T
    >
void func() {
    PrinF;

    typedef T<GetKeyTypeFromInternalLogic,GetValueTypeFromInternalLogic>  TObj;

    std::cout <<"Type(TObj) : "<< type_name<TObj>() << std::endl;

}

#include <map>

int main() {
    func<std::map>();
}