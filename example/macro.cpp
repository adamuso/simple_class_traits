#include <stdio.h>

#include "../simple_class_traits.hpp"


class Test
{
public:
    typedef TraitRef<Test> ref;

    virtual int get_a() = 0;
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

// What to do with const?
trait_impl(Test, const X)
{
public:
    int get_a()
    {
        return self->a;
    }

    void set_a(int value)
    {
        // self->a = value;
    }
};

trait_impl(Test, X)
{
public:
    int get_a()
    {
        return self->a;
    }

    void set_a(int value)
    {
        self->a = value;
    }
};

trait_impl(Test, Y)
{
public:
    int get_a()
    {
        return self->b;
    }

    void set_a(int value)
    {
        self->b = value;
    }
};

int foo(Test::ref t)
{
    return t->get_a();
}

int main()
{
    X x;
    Y y;

    const X& cx = x;
    foo(cx);

    printf("%d, %d\n", foo(x), foo(y));

    return 0;
}