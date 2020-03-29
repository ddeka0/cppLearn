#include <bits/stdc++.h>
#define PRINT_F  std::cout << __PRETTY_FUNCTION__<<" "<< __LINE__ << std::endl;
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
// maximum of two values of any type (call-by-reference)
template<typename T>
T const& max (T const& a, T const& b)
{
    PRINT_F
    std::cout << type_name<decltype(a)>() << std::endl;
    return  b < a ? a : b;
}

// maximum of two C-strings (call-by-value)
// char const* max (char const* a, char const* b)
// {
//     PRINT_F
//     std::cout << type_name<decltype(a)>() << std::endl;
//   return  std::strcmp(b,a) < 0  ? a : b;
// }

// maximum of three values of any type (call-by-reference)
template<typename T>
T const& max (T const& a, T const& b, T const& c)
{

    PRINT_F
    std::cout << type_name<std::decay_t<T>>() << std::endl;
    auto p = max(a,b);
    return max (p, c);       // error if max(a,b) uses call-by-value
}

int main ()
{


  char const* s1 = "zrederic";
  char const* s2 = "mnica";
  char const* s3 = "aucas";
  auto x = ::max(s1, s2, s3);    // run-time ERROR
}
