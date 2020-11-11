#include <iostream>

#define mem(m)	(void*)(m)

void* operator new(size_t sz)
{
  void* m = malloc(sz);
  std::cout<<"User Defined :: Operator new "<<sz <<" "<<mem(m) <<std::endl;
  
  return m;
}
// Overloading Global delete operator
void operator delete(void* m)
{
  std::cout<<"User Defined :: Operator delete "<< mem(m) <<std::endl;
  free(m);
}
// Overloading Global new[] operator
void* operator new[](size_t sz)
{
  void* m = malloc(sz);
  std::cout<<"User Defined :: Operator new [] "<<sz<<" "<< mem(m) <<std::endl;
  return m;
}
// Overloading Global delete[] operator
void operator delete[](void* m)
{
  std::cout<<"User Defined :: Operator delete[] "<<mem(m) <<std::endl;
  free(m);
}


#include <cstring>
class myObject {
    public:
    myObject() {
        buffer = new char[1000];
        this->sz = 1000;
    }
    myObject(std::size_t sz) {
        buffer = new char[sz];
        this->sz = sz;
    }
    myObject(const myObject& other) {
        std::cout <<"Copy Constructor called" << std::endl;
        buffer = new char[other.sz];
        sz = other.sz;
        std::memcpy(buffer,other.buffer,other.sz);
    }

    #ifdef MOVE
    myObject(myObject&& other) noexcept {
        std::cout <<"Move Constructor called" << std::endl;
        buffer = other.buffer;
        sz = other.sz;
        other.buffer = NULL;
        other.sz = 0;
    }
    myObject& operator=(myObject&& other) noexcept {
        std::cout <<"Move Assignment called" << std::endl;
        if(buffer != NULL) {
            delete[] buffer;
            sz = 0;
        }
        buffer = other.buffer;
        sz = other.sz;
        other.buffer = NULL;
        other.sz = 0;
        
        return *this;
    }
    #endif
    
    myObject& operator=(const myObject& other) {
        // self ref ignored
        std::cout <<"Copy Assignment called" << std::endl;
        if(buffer != NULL) {
            delete[] buffer;
        }
        buffer = new char[other.sz];
        sz = other.sz;
        std::memcpy(buffer,other.buffer,other.sz);
        return *this;
    }
    ~myObject() {
        std::cout <<"~myObject()" << std::endl;
        if(buffer != NULL) {
            delete[] buffer;
            buffer = NULL;
        }
    }
    char * buffer = NULL;
    std::size_t sz = 0;
};