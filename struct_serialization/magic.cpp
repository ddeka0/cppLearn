#include <iostream>
#include <fstream>
#include "reflection.hpp"
#include "extra_header.hpp"

using namespace std;
using namespace nlohmann;

void parseFile(std::string fileName,json& j) {
	std::ifstream _file(fileName);
	_file >> j;
}


int main() {


	json j;
	parseFile("input_a.json",j);


	messageA ma{};

	// using zz = decltype(&messageC::x);
	// using yy = typename PM_traits<zz>::member_type;
	// const bool vv = std::is_array<yy>::value;

	using M = Members<messageA>;
	fillMessage(j,ma,(typename M::type*)nullptr);

	PrintPack(ma,(typename M::type*)nullptr);

	// std::cout << ma.a << std::endl;
	// std::cout << ma.b << std::endl;
	// std::cout << ma.c.p << std::endl;
	// std::cout << ma.c.q << std::endl;
	// for(std::size_t i = 0;i<((sizeof(ma.c.r.x))/sizeof(int));i++) {
	// 	std::cout << ma.c.r.x[i]<<" ";
	// }
	// std::cout << std::endl;

	// for(std::size_t i = 0;i<((sizeof(ma.c.s))/sizeof(messageD));i++) {
	// 	if(i) std::cout <<" | ";
	// 	std::cout << ma.c.s[i].t << " , "<<ma.c.s[i].v;
	// }

	// std::cout << std::endl;

}