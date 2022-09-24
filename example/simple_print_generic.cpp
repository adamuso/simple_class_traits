#include <iostream>
#include <vector>
#include <deque>

#include "../simple_class_traits.hpp"

class SizedIntArray
{
public:
    typedef trait::ref<SizedIntArray> ref;

    virtual int size() = 0;
    virtual int item(int index) = 0;
};

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

// Implement trait for all SizedIntArray traits
// template<typename V>
// class TraitImpl<DebugPrint, V, typename TraitImpl<SizedIntArray, V>::type> : public DebugPrint::ref::container<V>
trait_impl_ext(DebugPrint, SizedIntArray)
{
public:
    void print() const
    {
        SizedIntArray::ref r = *this->self;

        for (int i = 0; i < r->size(); i++)
        {
            DebugPrint::ref(r->item(i))->print();
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

trait_impl(SizedIntArray, std::deque<int>)
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

    DebugPrint::ref(a)->print();
    DebugPrint::ref(SizedIntArray::ref(a))->print(); 

    std::deque<int> b = { 4, 5, 6 };

    DebugPrint::ref(b)->print();
    return 0;
}
