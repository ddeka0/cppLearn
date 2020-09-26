#include <bits/stdc++.h>

template <typename T>
constexpr auto type_name() noexcept {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void) noexcept";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}


template<typename T>
struct isDefaultConstructible {
    private:
        template<typename U, typename = decltype(U())> 
        static char test(void*);

        template<typename>
        static long test(...); 
    public:
        static constexpr bool value = std::is_same<decltype(test<T>(nullptr)) , char>::value;
};


class A {
    public:
    int x;
    A(int x) {
        std::cout <<"A()"<< std::endl;
    }
};
int main() {
    std::cout << isDefaultConstructible<A>::value << std::endl;
}