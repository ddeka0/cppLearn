#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"

template<typename T, T val>
struct metaFunction {
    static const T  value = val;
    typedef T       type;
};

template<typename, typename>
struct isTwoTypeSame : public metaFunction<bool,false> {

};

template<typename T>
struct isTwoTypeSame<T, T> : public metaFunction<bool,true> { 

};


int main() {
    std::cout << metaFunction<int,5>::value << std::endl;

    std::cout << isTwoTypeSame<int,int>::value << std::endl;
}