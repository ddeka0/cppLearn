#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define mem(m)	(void*)(m)
#define PrintF std::cout << __LINE__ <<" "<< __PRETTY_FUNCTION__ << std::endl
#define PrintObjAddr	std::cout <<"This object : "<<mem(this) << std::endl;
#define LineBreak  std::cout << std::endl << std::endl;

class Int {
public:
    Int() { PrintF; }
    ~Int() { 
		PrintF;
		PrintObjAddr;
		if(m_data != nullptr) {
			std::cout<<__LINE__<<" "<<"deleted plain int "<< mem(m_data) << std::endl;
			delete m_data;
			m_data = nullptr;
		}
	}
    Int(int x) {
		PrintF;
		PrintObjAddr;
		if(m_data == nullptr) {
			m_data = new int(x);
			std::cout<<__LINE__<<" "<<"allocated plain int "<< mem(m_data) << std::endl;
		}
		else
			*m_data = x;
	}
	
	Int(const Int& other) {
		// deep copy construction using other
		PrintF;
		PrintObjAddr;
		m_data = new int;
		std::cout<<__LINE__<<" "<<"allocated plain int "<< mem(m_data) << std::endl;
		*m_data = *(other.m_data);
	}

	Int(Int&& other) noexcept {
		// resource transfer from other 
		PrintF;
		PrintObjAddr;
		m_data = other.m_data;
		other.m_data = nullptr;
	}

	Int& operator=(const Int& other) {
		// delete own resources and create new resource and copy other
		PrintF;
		if(this != &other) {
			if(m_data != nullptr) {
				std::cout<<__LINE__<<" " <<"deleted plain int "<< mem(m_data) << std::endl;
				delete m_data;
				m_data = nullptr;
			}
			m_data = new int;
			std::cout<<__LINE__<<" "<<"allocated plain int "<< mem(m_data) << std::endl;
			*m_data = *(other.m_data);
		}
		return *this;
	}

	Int& operator=(Int&& other) noexcept {
		PrintF;
		if(this != &other) {
			if(m_data != nullptr) {
				std::cout<<__LINE__<<" " <<"deleted plain int "<< mem(m_data) << std::endl;
				delete m_data;
				m_data = nullptr;
			}
			m_data = other.m_data;
			other.m_data = nullptr;
		}
		return *this;
	}

    int* m_data = nullptr;
};

class Array {
    public:
    Array() {
        PrintF;
    }
    ~Array() {
        PrintF;
		m_data.clear();
		m_map_data.clear();
    }

    vector<Int> m_data;
	//map<int,Int> m_map_data;
};

int main() {
	std::cout << __LINE__ <<" => Op : "<< R"(Int a(1))" << std::endl;
	Int a = 1;
	
	LineBreak;
	std::cout << __LINE__ <<" => Op : "<< R"(Array arr)" << std::endl;
	Array arr;
	
	LineBreak;
	std::cout << __LINE__ <<" => Op : "<< R"(arr.m_data.push_back(std::move(a)))" << std::endl;
	arr.m_data.push_back(std::move(a));
	
	LineBreak;
	
	std::cout << __LINE__ <<" => Op : "<< R"(Int b(2))" << std::endl;
	Int b(2);
	
	LineBreak;

	std::cout << __LINE__ <<" => Op : "<< R"(arr.m_data.push_back(std::move(b)))" << std::endl;
	arr.m_data.push_back(std::move(b));

	LineBreak;

	std::cout << __LINE__ <<" => Op : "<< R"(arr.m_data[0] = Int(10))" << std::endl;
	
	arr.m_data[0] = Int(10);

	LineBreak;

	std::cout << __LINE__ <<" => Op : "<< R"(arr.m_data[0] = 10)" << std::endl;
	
	arr.m_data[0] = 10;

	LineBreak;


	arr.m_map_data[1] = 11;

	LineBreak;
}
