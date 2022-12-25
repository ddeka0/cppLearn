#include <tuple>
#include <type_traits>

template<typename What, typename Where, typename Is>
class is_present_helper;

template<typename What, typename Where, std::size_t... Is>
struct is_present_helper<What, Where, std::index_sequence<Is...>> {
    static constexpr bool value {(std::is_same_v<What, typename std::tuple_element<Is, Where>::type> || ...)};
};

template<typename What, typename Where>
struct is_present {
    using Indices = std::make_index_sequence<std::tuple_size_v<Where>>;
    static constexpr bool value = is_present_helper<What,Where,Indices>::value;
};

using AllowedTypes = std::tuple<int, char, bool>;

template<typename T>
concept ShouldAllowType = is_present<T,AllowedTypes>::value;

template <typename T>
void func(T val) requires ShouldAllowType<T> {

}

int main()
{
    func(32.1);

    return 0;
}
