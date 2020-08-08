#include <iostream>
using namespace std;
void print () {}

template<typename T, typename... Types>
void print (T firstArg, Types... args) {
    cout << sizeof...(Types) << endl;
  //std::cout << firstArg << '\n';  // print first argument
  print(args...);                 // call print() for remaining arguments
}


class A {
public:
    int a;
    A():a(0) {}
    A(int x):a(x) {}
    int func(int x) {
        return a + x;
    }
};

int main() {
    auto f = &A::func;
    auto *a = new A(4);
    cout << (a->*f)(1) << endl;

    //print("Debashish",1,2,3.45);

}