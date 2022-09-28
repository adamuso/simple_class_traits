#include <string>
#include <iostream>

#include "../simple_class_traits_into.hpp"

template<>
class trait::impl<trait::Into<std::string>, int> : public trait::container<trait::Into<std::string>, int>
{
public:
    virtual std::string into() const override
    {
        return std::to_string(*self);
    }
};

class Something
{
public:
    int a = 1;
    std::string b = "2";
    int c = 3;
};

trait_impl(trait::Into<std::string>, Something)
{
public:
    virtual std::string into() const override
    {
        using trait::into;

        return self->a->*into() + self->b + self->c->*into();
    }
};

void print(const std::string& str)
{
    std::cout << str << std::endl;
}

int main()
{
    using trait::into;
    
    int a = 10;
    int b = 20;
    int c = 30;

    std::string str_a = trait::from(a);
    auto str_b = trait::from<std::string>(b);
    std::string str_c = c->*into();

    std::cout << "a: " << str_a << std::endl;
    std::cout << "b: " << str_b << std::endl;
    std::cout << "c: " << str_c << std::endl;

    Something s;

    print(s->*into());

    // Fast access to impl_ref:
    // trait::impl_ref<trait::Into<std::string>, int> into_string_value = b;
    auto into_string_value = b->*into<trait::Into<std::string>>();

    print(into_string_value.into());

    return 0;
}