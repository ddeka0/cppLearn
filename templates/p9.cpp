#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"

myObject GetObject() {
    PrinF;
    myObject obj;
    return obj;
}

myObject GetObject_Complex(int x,int y) {
    PrinF;
    myObject obj;
    return x > y ? obj : obj;
}

int main() {

    myObject a = GetObject(); // RVO works

    BreakLine;

    myObject b = GetObject_Complex(1,0); // RVO will not work

}
