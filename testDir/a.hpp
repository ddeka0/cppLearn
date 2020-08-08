#pragma once
#include <bits/stdc++.h>
#include "b.hpp"
class A {
public:
    A() {
        b = new B(this);
        std::cout <<"B is created in A" << std::endl;
    }
    ~A() {
        if(b != nullptr)
            delete b;
    }
    B * b;
};