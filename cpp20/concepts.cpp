#include <bits/stdc++.h>
template<typename T>
concept Int = std::is_floating_point_v<T>;

auto func(const Int auto a,const Int auto b) {
	std::cout << a + b << std::endl;
}
int main() {
	func(1.1,2.2f);
}