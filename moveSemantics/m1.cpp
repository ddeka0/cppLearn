#include <iostream>
#include <vector>
using namespace std;

#define PrintF cout << __PRETTY_FUNCTION__ << endl
#define Break  cout <<endl<<endl;



class typeA {
    public:
    typeA():x(-1) {
        PrintF;
        // cout<<"mytype def-ctor"<<endl;
    }
    typeA(int v):x(v) {
        PrintF;
        // cout<<"mytype non-def-ctor"<<endl;
    }
    typeA(const typeA& a) {
        PrintF;
        // cout <<"mytype copy"<<endl;
        this->x = a.x;
    }
    typeA& operator=(const typeA &temp) {
        PrintF;
        // cout <<"mytype op="<<endl;
        this->x = temp.x;
        return *this;
    }&
    typeA(typeA&& rhs) {
        PrintF;
        // cout <<"mytype move constructor"<<endl;
        this->x = rhs.x;
    }
    typeA& operator=(typeA&& rhs) {
        PrintF;
        // cout <<"mytype move assignment operator"<<endl;
        this->x = rhs.x;
        return *this;
    }
    int x;
};

class typeB {
    public:
    typeB():x(-1) {
        PrintF;
    }
    typeB(int v):x(v) {
        PrintF;
        // cout<<"mytype non-def-ctor"<<endl;
    }
    typeB(const typeB& a) {
        PrintF;
        // cout <<"mytype copy"<<endl;
        this->x = a.x;
    }
    typeB& operator=(const typeB &temp) {
        PrintF;
        // cout <<"mytype op="<<endl;
        this->x = temp.x;
        return *this;
    }
    typeB(typeB&& rhs) {
        PrintF;
        // cout <<"mytype move constructor"<<endl;
        this->x = rhs.x;
    }
    typeB& operator=(typeB&& rhs) {
        PrintF;
        // cout <<"mytype move assignment operator"<<endl;
        this->x = rhs.x;
        return *this;
    }
    int x;
};

class typeC {
    public:
    typeC():x(-1) {
        PrintF;
    }
    typeC(int v):x(v) {
        PrintF;
        // cout<<"mytype non-def-ctor"<<endl;
    }
    // typeC(const typeC& a) {
    //     PrintF;
    //     // cout <<"mytype copy"<<endl;
    //     this->x = a.x;
    // }

    // typeC& operator=(const typeC &temp) {
    //     PrintF;
    //     // cout <<"mytype op="<<endl;
    //     this->x = temp.x;
    //     return *this;
    // }
    // typeC(typeC&& rhs) {
    //     PrintF;
    //     // cout <<"mytype move constructor"<<endl;
    //     this->x = rhs.x;
    // }
    // typeC& operator=(typeC&& rhs) {
    //     PrintF;
    //     // cout <<"mytype move assignment operator"<<endl;
    //     this->x = rhs.x;
    //     return *this;
    // }
    
    int x;
    typeA a;
    typeB b;
};
int main() {
    // vector<Obj> v;
    // v.emplace_back(mytype{1});
    
    {
        typeC c;
        c = typeC();
    }

    Break

    // {
    //     Derived d;
    //     d = Derived();

    //     cout << (&d) << endl;   
    // }
}