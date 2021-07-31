#include <iostream>
#include "PrintHelper.hpp"
using namespace std;

template<int N> // N becomes a compile time constant
void func() {
    int arr[N]; // this is an array of N iterms of int
    for(int i = 0;i<N;i++) {
        std::cout << i << std::endl;
    }
}

int main() {
    func<100>();
}