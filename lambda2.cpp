#include <bits/stdc++.h>
using namespace std;
template<typename T>
class A {
    public:
    A(T& l) : lam(l) {}
    T lam;
};
class B {
    public:
    int x = 0x12;
    auto func() {
        auto lam = [this](int val) {
            auto y = this->x + val;
            return y;
        };
        return new A<decltype(lam)>(lam);
    }
};
int main() {
    B b;
    auto x = b.func();
    std::cout << x->lam(2) << std::endl;
}
