#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"


myObject GetObject() {
    PrinF;
    myObject obj;
    return obj;
    // return x > y ? obj : obj;
}

int main() {

    myObject b = GetObject(); // -DMOVE -fno-elide-constructors

    myObject c = std::move(b); // c <- myObject(myObject&& x) , move provides the x using type cast

}
