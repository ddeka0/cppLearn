#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"

int idCnt = 0;

template<typename T>
int getId() {
    static int id = idCnt++;
    return id; 
}


int main() {

    std::cout << getId<int>() << std::endl;
    std::cout << getId<int>() << std::endl;
    std::cout << getId<int>() << std::endl;

    std::cout << getId<char>() << std::endl;
    std::cout << getId<char>() << std::endl;
    
    
    std::cout << getId<double>() << std::endl;
    std::cout << getId<double>() << std::endl;

    std::cout << getId<char>() << std::endl;

}