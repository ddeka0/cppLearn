/* 
gcc version 10.0.1 20200314 (experimental) (GCC)
Ubuntu 18.04.5 LTS
x86_64 GNU/Linux 5.6.2
*/
#include <thread>
#include <iostream>

int main() {
    std::thread t{[]() {
        std::cout <<"Hello from thread t"<<std::endl;
    }};
}