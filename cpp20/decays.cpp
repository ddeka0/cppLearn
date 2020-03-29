#include <bits/stdc++.h>
using namespace std;
template <class T>
constexpr
std::string_view
type_name() {
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
void func(T&& _x) {
    using _T = std::decay<decltype(_x)>::type;
    //std::cout << type_name<decltype(_x)>() << std::endl;
    std::cout << type_name<_T>() << std::endl;
}
int main() {
    std::string arr = "abc";
    func(arr);
    func("abc");
}