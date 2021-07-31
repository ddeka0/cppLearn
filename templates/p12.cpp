#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"

template<typename T, typename std::enable_if< std::is_same<T,int>::value
                                        || std::is_same<T,std::string>::value
                                        || std::is_same<T,double>::value
                                        ,int>::type = 0 >
void func(T&& x) {
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}

template<typename T, typename std::enable_if< std::is_same<T,bool>::value,int>::type = 0 >
void func(T&& x) {
    PrinF;
    std::cout <<"Type(x) : "<< type_name<decltype(x)>() << std::endl;
    std::cout <<"Type(T) : "<< type_name<T>() << std::endl;
    std::cout <<"Is x is lavlue ref ? "<< getRefType(x) << std::endl;
}


int main() {
    BreakLine;
    func(1);

    BreakLine;
    func(std::string("deka"));

    BreakLine;
    func(true);

    // typename std::enable_if<std::is_same<int,int>::value,int>::type --> int
    typedef typename std::enable_if<std::is_same<int,int>::value,int>::type  shouldBeInt;
    std::cout << type_name<shouldBeInt>() << std::endl;

    /*  (gdb) info functions func
        All functions matching regular expression "func":

        Non-debugging symbols:
        0x004042c4  void func<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >, 0>(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> >&&)
        0x004043d4  void func<bool, 0>(bool&&)
        0x004044e4  void func<int, 0>(int&&)
    */
}