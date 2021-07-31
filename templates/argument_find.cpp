#include <iostream>
#include "PrintHelper.hpp"
using namespace std;

template<typename T>
class Apple {
    public:
       typedef char      value_type;

       value_type var;
};

int main() {
    Apple<int> apple;

    Apple<int>::value_type var = 'A';

}