#include <bits/stdc++.h>
using namespace std;

void * addr;
template<typename T>
class A {
    public:
    A(T& l) : lam(l) {
    }
    ~A(){
        _more();
        std::cout <<"called" << std::endl;
    };
    void _more() {
        addr = (void* )(new A(lam));
        std::cout << addr << std::endl;
    }
    T lam;
};
class B {
    public:
    int x = 0x12;
    auto func() {
        std::cout <<"tis ptr"<<(void*)this << std::endl;
        auto lam = [this](int val) {
            auto y = this->x + val;
            return y;
        };
        auto lam2 = [this,&lam](){
            return new A<decltype(lam)>(lam);
        };
        return lam2();
    }
    void investigate(void * ptr) {
        auto lam = [this](int val) {
            auto y = this->x + val;
            return y;
        };
        std::cout << ((A<decltype(lam)>*)(ptr))->lam(1) << std::endl;
    }
};
int main() {
    B b;
    auto x = b.func();
    std::cout << x->lam(1) << std::endl;
    delete x;
    b.investigate(addr);

}
