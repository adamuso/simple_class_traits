#include <string>
#include <iostream>

#include "../simple_class_traits_into.hpp"

template<>
class trait::impl<trait::Into<std::string>, int> : public trait::container<trait::Into<std::string>, int>
{
public:
    std::string into()
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
    std::string into()
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

    return 0;
}