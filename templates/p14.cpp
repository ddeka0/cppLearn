#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <type_traits>

using namespace std;
template<typename T,T v>

struct Integral_constant {
    static constexpr T value = v;
    using value_type = T;
    using type = Integral_constant<T,v>;
    constexpr operator value_type() const noexcept {
        return value;
    }
    constexpr value_type operator()() const noexcept {
        return value;
    }
};

template<bool B>
using Bool_constant = Integral_constant<bool,B>;
using True_type = Bool_constant<true>;
using False_type = Bool_constant<false>;

template<typename...>
using Requirements = void;

template<typename T,typename = void> // some default param for the primary signature
struct isAllowed : False_type {};

template<typename T>
struct isAllowed<T,Requirements<
            typename T::x , decltype(&T::work(declval)) 
            
            >> : True_type{};



template<typename T>
void func(T a) {
    static_assert(isAllowed<T>::value,"Requirements did not met!");
    // do something with a
    // a.work(1);
}

struct X{
    using x = int;
    void work(int x) {
        std::cout <<"work()" << std::endl; 
    } 
};


int main() {
    X a;
    func(a);
}