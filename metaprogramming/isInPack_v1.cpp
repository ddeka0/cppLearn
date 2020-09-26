// C++17
#include <vector>
#include <cstdint>
#include <iostream>
#include <type_traits>

using my_int_t      = std::int16_t;         // T1
using my_string_t   = std::string;          // T2
using my_vec_t      = std::vector<int>;     // T3
// and so on .. a long list of types 


/* Check my Type T : if it belongs to {T1,T2,T3} 
If it belongs then only allow this version of func to instantiate
*/
template<typename T, typename T1,typename T2,typename T3>
using isCompatible = std::bool_constant<std::is_same_v<T,T1>
                            || std::is_same_v<T,T2>
                            || std::is_same_v<T,T3> >;

template<typename T>
using CompatibleTypeCheck = std::enable_if_t< isCompatible<std::remove_reference_t<T>,
                                                    my_int_t,
                                                    my_string_t,
                                                    my_vec_t>::value >;



template<typename T,
        typename = CompatibleTypeCheck<T>>
void func(T&& val) {
    std::cout <<"Hello from Generic Func" << std::endl;
}


int main() {

    // int z = 10;   
    // func(z); // Error ! as expected

    my_int_t w = 100;
    func(w); // OK
}