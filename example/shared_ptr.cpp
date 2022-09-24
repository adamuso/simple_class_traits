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
    typedef trait::ref<DebugPrint> ref;

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

trait::shared_ptr<DebugPrint> create_a() 
{
    return std::shared_ptr<A>(new A { .a = 2 });
}

int main()
{
    trait::shared_ptr<DebugPrint> c = create_a();

    debug_print(*c);

    return 0;
}