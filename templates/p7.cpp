#include "PrintHelper.hpp"

void func(int&& x) {
    std::cout << type_name<decltype(x)>() << std::endl;
    std::cout <<"is a rvalue ? "<< getRefType(x) << std::endl;
}

void func(const int& x) {
    std::cout << type_name<decltype(x)>() << std::endl;
    std::cout <<"is a rvalue ? "<< getRefType(x) << std::endl;
}

int main() {
    func(1);

    BreakLine;

    int x = 1;
    func(x);
}