#include <bits/stdc++.h>
using namespace std;
std::mutex m;
int a,b;
void f1() {
    a = 1;
    try {
        lock_guard<std::mutex> lock(m);
        if(a == 1) {
            return;
        }
        int * ptr = new int[10000000000];
    }catch(...) {
        cout <<"new faild"<<endl;
    }
    // not locked from this point
    std::this_thread::sleep_for(std::chrono::milliseconds(2000)); //2s
    b = 2;
    cout << a <<" : "<<b <<endl;
}
void f2() {
    lock_guard<std::mutex> lock(m);
    cout << a <<" : "<<b <<endl;
}
int main() {
    std::thread T1(f1);   
    

    std::thread T2(f2);   
    T2.join();
    T1.join();
}