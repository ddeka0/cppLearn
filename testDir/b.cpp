#include "b.hpp"
B::B(A * _a) {
    a = _a;
    std::cout <<"Hello from B()" << std::endl;
}