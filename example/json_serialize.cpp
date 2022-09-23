#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>

#include "../simple_class_traits.hpp"

class JsonSerialize
{
public:
    typedef TraitRef<JsonSerialize> ref;

    std::string key(std::string value) const
    { 
        std::string output = "\"";
        output += value;
        output += "\":";
        return output;    
    }

    virtual std::string to_string() const = 0;
};

trait_impl(JsonSerialize, int)
{
public:
    std::string to_string() const
    {
        return std::to_string(*self);
    }
};

trait_impl(JsonSerialize, float)
{
public:
    std::string to_string() const
    {
        return std::to_string(*self);
    }
};

trait_impl(JsonSerialize, double)
{
public:
    std::string to_string() const
    {
        return std::to_string(*self);
    }
};

trait_impl(JsonSerialize, std::string)
{
public:
    std::string to_string() const
    {
        std::string output = "\"";
        // Should also escape all " characters, but it does not for code simplicity
        output += *self;
        output += "\"";
        return output;
    }
};

trait_impl(JsonSerialize, void*)
{
public:
    std::string to_string() const
    {
        return "null";
    }
};

template<typename V>
class TraitImpl<JsonSerialize, std::vector<V>> : public JsonSerialize::ref::container<std::vector<V>>
{
public:
    std::string to_string() const
    {
        std::string output = "[";

        for (auto& item : *this->self)
        {
            output += JsonSerialize::ref(item)->to_string();
            output += ",";
        }

        output.pop_back();
        output += "]";
        return output;
    }
};

trait_impl_gen((typename V), JsonSerialize, std::unordered_map<std::string, V>)
{
public:
    std::string to_string() const
    {
        std::string output = "{";

        for (auto& item : *this->self)
        {
            output += this->key(item.first);
            output += JsonSerialize::ref(item.second)->to_string();
            output += ",";
        }

        output.pop_back();
        output += "}";
        return output;
    }
};

class A
{
public:
    int a = 10;
    double b = 0.5;
};

trait_impl(JsonSerialize, A)
{
public:
    std::string to_string() const
    {
        std::string output = "{";

        output += key("a");
        output += JsonSerialize::ref(self->a)->to_string();

        output += "," + key("b");
        output += JsonSerialize::ref(self->b)->to_string();

        output += "}";
        
        return output;
    }
};

class B
{
public:
    std::string c = "hello";
    float d = 0.2f;
};

trait_impl(JsonSerialize, B)
{
public:
    std::string to_string() const
    {
        std::string output = "{";

        output += key("c");
        output += JsonSerialize::ref(self->c)->to_string();

        output += "," + key("d");
        output += JsonSerialize::ref(self->d)->to_string();

        output += "}";
        
        return output;
    }
};

class C
{
public:
    std::vector<int> e = { 1, 2, 3 };
    std::vector<std::string> f = { "a", "b", "c" };
};

trait_impl(JsonSerialize, C)
{
public:
    std::string to_string() const
    {
        std::string output = "{";

        output += key("e");
        output += JsonSerialize::ref(self->e)->to_string();

        output += "," + key("f");
        output += JsonSerialize::ref(self->f)->to_string();

        output += "}";
        
        return output;
    }
};

class ComplexObj
{
public:
    A a;
    B b;
    C c;
    std::unordered_map<std::string, A> d = {{ "t1", A() }};
};

trait_impl(JsonSerialize, ComplexObj)
{
public:
    std::string to_string() const
    {
        std::string output = "{";

        output += key("a");
        output += JsonSerialize::ref(self->a)->to_string();

        output += "," + key("b");
        output += JsonSerialize::ref(self->b)->to_string();

        output += "," + key("c");
        output += JsonSerialize::ref(self->c)->to_string();

        output += "," + key("d");
        output += JsonSerialize::ref(self->d)->to_string();

        output += "}";
        
        return output;
    }
};


int main()
{
    ComplexObj obj;

    std::cout << JsonSerialize::ref(obj)->to_string() << std::endl;
    return 0;
}
