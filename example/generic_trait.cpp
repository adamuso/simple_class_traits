#include <iostream>

#include "../simple_class_traits.hpp"

template<typename T>
class Add
{
public:
    typedef trait::ref<Add> ref;

    virtual int add(const T& other) const = 0;
};


template <typename Self>
class AutoImplementAddition
{
public:
    Self& self()
    {
        return *static_cast<Self*>(this);
    }

    template<typename T>
    int operator+(const T& other)
    {
        return typename Add<T>::ref(self())->add(other);
    }
};

class X : public AutoImplementAddition<X>
{
    // X have inherited operator+(...) which means that if we implement a correct
    // trait then we can inject a behavior of adding X + T = int objects 
public:
    int value;

    X(int value)
        : value(value)
    {

    }
};

template<>
class trait::impl<Add<X>, X> : public Add<X>::ref::container<X>
{
    // Implements X + X -> int
public:
    int add(const X& other) const override
    {
        return self->value + other.value;
    }
};

template<>
class trait::impl<Add<int>, X> : public Add<int>::ref::container<X>
{
    // Implements X + int -> int
public:
    int add(const int& other) const override
    {
        return self->value + other;
    }
};

int main()
{
    X a { 4 };
    X b { 2 };
    int c = a + b;
    int d = b + 10;

    std::cout << c << ", " << d << std::endl;

    return 0;   
}