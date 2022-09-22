#include <iostream>
#include <vector>

#include "../simple_class_traits.hpp"

class SizedIntArray
{
public:
    typedef TraitRef<SizedIntArray> ref;

    virtual int size() = 0;
    virtual int item(int index) = 0;
};

class DebugPrint 
{
public:
    typedef TraitRef<DebugPrint> ref;

    virtual void print() = 0;
};

trait_impl(DebugPrint, int)
{
public:
    void print()
    {
        std::cout << *self;
    }
};

trait_impl(DebugPrint, SizedIntArray::ref)
{
public:
    void print()
    {
        for (int i = 0; i < self->size(); i++)
        {
            DebugPrint::ref(self->item(i))->print();
            std::cout << ", ";
        }

        std::cout << std::endl;
    }
};

trait_impl(SizedIntArray, std::vector<int>)
{
    int size()
    {
        return self->size();
    }

    int item(int index) 
    {
        return (*self)[index];
    }
};

int main()
{
    std::vector<int> a = { 1, 2, 3 };
    DebugPrint::ref(SizedIntArray::ref(a))->print();

    // DebugPrint::ref(a)->print(); - doesn't working without an additional template, see simple_print_generic.cpp 
    return 0;
}
