#include <iostream>
#include "corolib.hpp"

rtask foo(int n) {
    std::cout <<"About to set x = 1" << std::endl;
    int x = 1;
    co_await std::experimental::suspend_always();
    std::cout <<"About to set x = 2" << std::endl;
    int y = 2;
    co_return x + y;
}

int main() {
    auto x = foo(1);
    x.resume();
    getchar();
    x.resume();
    std::cout <<"Returns "<< x.get() << std::endl;
}