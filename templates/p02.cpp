#include <iostream>
#include "PrintHelper.hpp"
#include <vector>

using namespace std;




template<int N>
void func(int indx) { // paramType x
    static char x[N];
    
    if(indx < N && indx >= 0) {
        x[indx] = indx*indx;
    }

    for(int i = 0;i<N;i++) {
        printf("x[%d] = 0x%x ",i,x[i]);
    }

    printf("\n");
}


int main() {

    func<6>(0);
    func<6>(1);
    func<6>(2);
    func<6>(3);

}