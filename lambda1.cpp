#include <bits/stdc++.h>
using namespace std;
template<typename T>
class A {
    public:
    A(T& l) : lam(l) {}
    auto _more() {
        return new A(lam);
    }
    T lam;
};
class B {
    public:
    int x = 10000;
    auto func() {
        auto lam = [this](int val) {
            auto y = this->x + val;
            return y;
        };
        auto lam2 = [this,&lam](){
            return new A<decltype(lam)>(lam);
        };
        return lam2();
    }
};
int main() {
    B b;
    auto x = b.func();
    std::cout << x->lam(1) << std::endl;
    auto y = x->_more();
    std::cout << y->lam(1) << std::endl;
}