#include <iostream>

#include "../simple_class_traits.hpp"

class DebugPrint 
{
public:
    typedef TraitRef<DebugPrint> ref;

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
    debug_print(2);
    return 0;
}