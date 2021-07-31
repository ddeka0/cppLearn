#include "PrintHelper.hpp"
#include "MemoryHelper.hpp"
#include <vector>

class component_base {

};

class Entity { 
    public:
        std::vector<component_base*> V;
        
        bool hasComponent() {
            for(auto&& comp : V) {
                
            }
        }
};


int main() {


}