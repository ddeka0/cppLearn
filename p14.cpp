#include <iostream>
#include <functional>
using namespace std;
class test {
    public:
        int x;
        test(int);
        void get_x();
        void set_x(int);
        void procedure();
        void wrapper(std::function<void()>func);
};
test::test(int x) {
    this->x = x;
}
void test::get_x() {
    std::cout << x << std::endl;
}
void test::set_x(int x) {
    this->x = x;
    cout <<"Value of x is set to "<< this->x << endl; 
}
void test::wrapper(std::function<void ()> func) {
    cout <<"Some Other task"<< endl;
    func();
}

void test::procedure() {
    cout <<"Processing Message..."<<endl;
    wrapper([this](){get_x();});

    wrapper([this](){set_x(4);});

    wrapper([this](){get_x();});
    cout <<"Processing Message done !"<<endl;
}

int main() {
    test T(0);
    T.procedure();
}