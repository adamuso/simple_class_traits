#include <iostream>
#include <vector>

#include "../simple_class_traits.hpp"

class DebugPrint 
{
public:
    typedef trait::ref<DebugPrint> ref;

    virtual void print() const = 0;
};

trait_impl(DebugPrint, int)
{
public:
    void print() const 
    {
        std::cout << *self;
    }
};

void debug_print(const DebugPrint::ref p)
{
    p->print();
}

int main()
{
    // you cannot use trait::ref in vector, if you do this it can cause undefined behavior,
    // instead trait::ref should be treated the same as C++ reference
    //   std::vector<trait::ref<DebugPrint>> debug_prints;

    // instead you should use ptr in this case (or shared_ptr)
    std::vector<trait::ptr<DebugPrint>> debug_prints;

    int a = 3;
    debug_prints.push_back(&a);

    debug_print(*debug_prints[0]);
    return 0;
}