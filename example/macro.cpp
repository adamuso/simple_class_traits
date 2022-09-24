#include <iostream>

#include "../simple_class_traits.hpp"


class Test
{
public:
    typedef trait::ref<Test> ref;

    virtual int get_a() const = 0;
    virtual void set_a(int value) = 0;
};

class X
{
public:
    int a = 20;
};

class Y
{
public:
    int b = 30;
};

class Z
{
public:
    int c = 40;
};

trait_impl(Test, X)
{
public:
    int get_a() const
    {
        return self->a;
    }

    void set_a(int value)
    {
        self->a = value;
    }
};

// Implementing a trait without a macro
template<>
class trait::impl<Test, Y> : public Test::ref::container<Y>
{
public:
    int get_a() const
    {
        return self->b;
    }

    void set_a(int value)
    {
        self->b = value;
    }
};


int foo(const Test::ref t)
{
    return t->get_a();
}

int main()
{
    X x;
    Y y;
    Z z;

    // foo(z); - not working, Z does not implement a trait Test

    const X& cx = x;
    foo(cx);

    std::cout << foo(x) << ", " << foo(y) << std::endl;

    return 0;
}
