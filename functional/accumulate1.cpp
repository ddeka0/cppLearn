#include <fstream>
#include <sstream>
#include <iostream>
#include <numeric>



int main() {

	std::ifstream t("in");
	std::stringstream buffer;
	buffer << t.rdbuf();
	std::string&& s = buffer.str();

	auto nLines = std::accumulate(s.begin(),s.end(),0,
			[](int prevCount,auto&& c) {
				return prevCount + (c == '\n');
			});

	std::cout << nLines << std::endl;

}