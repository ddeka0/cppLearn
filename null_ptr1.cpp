#include <bits/stdc++.h>
using namespace std;
void func(int x) {
    cout << "func(int x) is called" << endl;
}
void func(bool x) {
    cout << "func(bool x) is called" << endl;
}
void func(void *) {
    cout << "func(void *) is called" << endl;
}
int main() {
    std::nullptr_t x;
    func(nullptr);
    return 0;
}