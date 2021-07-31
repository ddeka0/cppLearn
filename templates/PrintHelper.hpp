#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
#   include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>
#include <iostream>

#if defined(_MSC_VER)
#define PrinF   std::cout <<__LINE__<<":"<< __FUNCSIG__<< std::endl;
#else
#define PrinF   std::cout <<__LINE__<<":"<<__PRETTY_FUNCTION__<< std::endl;
#endif

// C++ language standard detection
#if (defined(__cplusplus) && __cplusplus >= 202002L) || (defined(_MSVC_LANG) && _MSVC_LANG >= 202002L) \
    || (defined(__cplusplus) && __cplusplus >= 201703L) || (defined(_HAS_CXX17) && _HAS_CXX17 == 1) 
    
    #include <string_view>
    #include <type_traits>

    template <class T> 
    constexpr std::string_view type_name() {
        using namespace std;
    #ifdef __clang__
        string_view p = __PRETTY_FUNCTION__;
        return string_view(p.data() + 34, p.size() - 34 - 1);
    #elif defined(__GNUC__)
        string_view p = __PRETTY_FUNCTION__;
    #  if __cplusplus < 201402
        return string_view(p.data() + 36, p.size() - 36 - 1);
    #  else
        return string_view(p.data() + 49, p.find(';', 49) - 49);
    #  endif
    #elif defined(_MSC_VER)
        string_view p = __FUNCSIG__;
        return string_view(p.data() + 84, p.size() - 84 - 7);
    #endif
    }

    template<typename T>
    struct refType {
        static constexpr std::string_view type = (std::is_lvalue_reference<T>::value) > 0 ? "lvalue":"ravlue";
    };

    #define getRefType(expr)    refType<decltype(expr)>::type

    #define GetKeyTypeFromInternalLogic             std::string
    #define GetValueTypeFromInternalLogic           int

#else 


template <class T>
std::string
type_name()
{
    typedef typename std::remove_reference<T>::type TR;
    std::unique_ptr<char, void(*)(void*)> own
           (
#ifndef _MSC_VER
                abi::__cxa_demangle(typeid(TR).name(), nullptr,
                                           nullptr, nullptr),
#else
                nullptr,
#endif
                std::free
           );
    std::string r = own != nullptr ? own.get() : typeid(TR).name();
    if (std::is_const<TR>::value)
        r += " const";
    if (std::is_volatile<TR>::value)
        r += " volatile";
    if (std::is_lvalue_reference<T>::value)
        r += "&";
    else if (std::is_rvalue_reference<T>::value)
        r += "&&";
    return r;
}

    template<typename T>
    struct refType {
        static constexpr const char* type = (std::is_lvalue_reference<T>::value) > 0 ? "lvalue":"ravlue";
    };

    #define getRefType(expr)    refType<decltype(expr)>::type

    #define GetKeyTypeFromInternalLogic             std::string
    #define GetValueTypeFromInternalLogic           int

#endif

#define BreakLine   std::cout << std::endl << std::endl;