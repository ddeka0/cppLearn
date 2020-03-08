#include <bits/stdc++.h>
using namespace std;
template<typename FuncType,
        typename MuxType,
        typename PtrType>
decltype(auto) lockAndCall(FuncType func, 
                            MuxType & mtx, 
                            PtrType ptr) {
    std::lock_guard<std::mutex> g(mtx);
    return func(ptr);
}
std::shared_ptr<int> f1(void *) {
    return std::make_shared<int>();
}
int main() {
    std::mutex f1_mtx;
    if(nullptr != lockAndCall(f1,f1_mtx,nullptr)) {
        cout <<"done"<<endl;
    }
    return 0;
}