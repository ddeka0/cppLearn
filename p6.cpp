static_assert(__cplusplus >= 201703L, "example written for C++17 or later");

#include <iostream>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>

using CMembers = std::tuple<int, char>;
struct C : CMembers {
  using Members = CMembers;
  using Members::Members;// inherit constructor
};

using BMembers = std::tuple<int, char, C>;
struct B : BMembers {
  using Members = BMembers;
  using Members::Members;// inherit constructor
};

using AMembers = std::tuple<int, int, B, char>;
struct A : AMembers {
  using Members = AMembers;
  using Members::Members;// inherit constructor
};



template<std::size_t... is, class... Ts, class F>
void foreach_tuple_element( std::index_sequence<is...>, const std::tuple<Ts...>& tuple,F f) {
  ( f( std::get<is>(tuple) ), ... );
}

template<class... Ts, class F>
void foreach_tuple_element( const std::tuple<Ts...>& tuple, F f) {
  foreach_tuple_element(std::index_sequence_for<Ts...>{}, tuple, f);
}



template<class T>
auto pretty_print(const T& x, std::string indent = "") -> std::void_t<decltype(std::cout << indent << x << "\n")> {
  std::cout << indent << x << "\n";
}

template<class... Ts>
void pretty_print(const std::tuple<Ts...>& tuple, std::string indent = "") {
  foreach_tuple_element(tuple, [indent] (auto&& x) {
    pretty_print(x, indent + "\t");
  });
}

template<class T, class MemberTuple = typename T::Members>
void pretty_print(const T& x, std::string indent = "") {
  pretty_print(static_cast<const MemberTuple&>(x), indent);
}

int main() {
    A a{1, 2, {3, 'p', {4, 'k'}}, 'w'};
    pretty_print(a);
}