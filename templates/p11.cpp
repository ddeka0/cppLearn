#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"


class InterfaceClass {
    public:
    virtual void pleaseOverrideMe() = 0;
};



template<typename T>
void func(T&& x) {
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}

#include <map>

int main() {
    BreakLine;
    func(1);

    BreakLine;
    func(std::string("deka"));

    BreakLine;
    func(std::map<int,int>{});


    BreakLine;
    int x = 5;
    func(x);

    /*
        (gdb) info functions func
        All functions matching regular expression "func":

        Non-debugging symbols:
        0x00404344  void func<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&&)
        0x00404454  void func<int&>(int&)
        0x00404564  void func<std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > > >(std::map<int, int, std::less<int>, std::allocator<std::pair<int const, int> > >&&)
        0x00404674  void func<int>(int&&)
    */
}