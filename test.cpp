#include <bits/stdc++.h>
int main() {
	std::vector<int> x;
	x.push_back(1);
	x.push_back(2);
	x.push_back(x.back());
	for(const auto&y:x) {
		std::cout << y << std::endl;
	}
}