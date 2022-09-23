#include <iostream>
#include <memory>

#include "../simple_class_traits.hpp"
#include "../simple_class_traits_shared.hpp"

struct A
{
    int a;

    ~A()
    {
        std::cout << "A: " << a << " DELETE" << std::endl;
    }
};

class DebugPrint 
{
public:
    typedef TraitRef<DebugPrint> ref;

    virtual void print() const = 0;
};

trait_impl(DebugPrint, A)
{
public:
    void print() const 
    {
        std::cout << self->a << std::endl;
    }
};

void debug_print(DebugPrint::ref p)
{
    p->print();
}

DebugPrint::ref::shared create_a() 
{
    return std::shared_ptr<A>(new A { .a = 2 });
}

int main()
{
    DebugPrint::ref::shared c = create_a();
    DebugPrint::ref cr = *c;

    debug_print(cr);

    return 0;
}