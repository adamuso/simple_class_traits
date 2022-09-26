#include <iostream>
#include <string>

#include "../simple_class_traits.hpp"

template<typename T>
class Add
{
public:
    template <typename Output>
    class outputs
    {
    public:
        typedef Add tag;
        typedef trait::ref<outputs> ref;

        virtual Output add(const T& other) const = 0;
    };
};


template <typename Self>
class AutoImplementAddition
{
    // Real output type needs to be fetched from the concrete trait implementation
    template<typename T>
    using OperatorPlusOutput = typename trait::impl<Add<T>, Self>::Output;
public:

    Self& self()
    {
        return *static_cast<Self*>(this);
    }

    // The most generic implementation of addition that is possible
    template<typename T>
    OperatorPlusOutput<T> operator+(const T& other)
    {
        return typename Add<T>::template outputs<OperatorPlusOutput<T>>::ref(self())->add(other);
    }
};

class X : public AutoImplementAddition<X>
{
    // X have inherited operator+(...) which means that if we implement a correct
    // trait then we can inject a behavior of adding X + T objects 
public:
    int value;

    X(int value)
        : value(value)
    {

    }
};

template<>
class trait::impl<Add<int>, int> : public Add<int>::outputs<int>::ref::container<int>
{
public:
    int add(const int& other) const override
    {
        return *self + other;
    }
};

template<>
class trait::impl<Add<std::string>, std::string> : public Add<std::string>::outputs<std::string>::ref::container<std::string>
{
public:
    std::string add(const std::string& other) const override
    {
        return *self + other;
    }
};

template<>
class trait::impl<Add<X>, X> : public Add<X>::outputs<X>::ref::container<X>
{
    // Implements X + X -> X
public:
    typedef X Output;

    X add(const X& other) const override
    {
        return Add<int>::outputs<Output>::ref(self->value)->add(other.value);
    }
};

template<>
class trait::impl<Add<int>, X> : public Add<int>::outputs<X>::ref::container<X>
{
    // Implements X + int -> X
public:
    typedef X Output;

    Output add(const int& other) const override
    {
        return Output(Add<int>::outputs<int>::ref(self->value)->add(other));
    }
};

template<int N>
class trait::impl<Add<char[N]>, X> : public Add<char[N]>::template outputs<std::string>::ref::template container<X>
{
    // Implements X + std::string -> std::string
public:
    typedef std::string Output;

    Output add(const char(&other)[N]) const override
    {
        return Add<std::string>::outputs<Output>::ref(std::to_string(this->self->value))->add(other);
    }
};

int main()
{
    X a { 4 };
    X b { 2 };
    X c = a + b;

    std::cout << "c.value: " << c.value << std::endl;

    X d = c + 10;
    std::string output = d + " is a result";

    std::cout << output << std::endl;

    return 0;   
}