#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"





void func(std::string&& x) {
    std::cout << type_name<decltype(x)>() << std::endl;
    std::cout <<"is a rvalue ? "<< getRefType(x) << std::endl;

    x[0] = 'c';
    x[1] = 'c';
    x[2] = 'c';
    std::cout << x << std::endl;

}

void func(const std::string& x) {
    std::cout << type_name<decltype(x)>() << std::endl;
    std::cout <<"is a rvalue ? "<< getRefType(x) << std::endl;
    
    // x[1] = 'c';
    // std::cout << x << std::endl;
}



std::string GetString(int a,int b) {
    return a > b ? "eeeeeeeeeeeeeeeeeeeeeeeee":"bbbbbbbbbbbbbbbbbbbb";
}

int main() {

    func(GetString(1,0));

    std::string&& s = GetString(1,0);

    s[0] = 'p';
    s[1] = 'p';
    s[2] = 'p';
    std::cout << s << std::endl;

}