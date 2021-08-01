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

	using M = Members<messageA>;
	fillMessage(j,ma,(typename M::type*)nullptr);

	PrintPack(ma,(typename M::type*)nullptr);
}