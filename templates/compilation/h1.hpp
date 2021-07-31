#include <iostream>
template<typename T>
class Apple {
    public:
    void do_something(T _x) {
        T x = _x;
        std::cout <<"Class Apple does something "<< x << std::endl;
    }
};