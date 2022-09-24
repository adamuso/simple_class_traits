# Simple Class Traits

## Work in progres...

TODO:
- [X] Defining traits as simple class
- [X] Basic trait implementation
- [X] Generic trait implementation
- [X] Allow implementing traits for other traits
- [X] C++11 compatible
- [ ] Simplify trait implementation declaration 
- [ ] Make it simple
- [ ] Make it 100% safe
- [ ] Handle cases like `std::vector<Trait::ref>` 
- [ ] Somehow allow using traits with `shared_ptr`
- [ ] (maybe?) Add a namespace for traits 

## Example

See more examples in `example` directory.

```cpp
// example/simplest_print.cpp

#include <iostream>

#include "../simple_class_traits.hpp"

class DebugPrint 
{
public:
    typedef TraitRef<DebugPrint> ref;

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

void debug_print(const DebugPrint::ref p)
{
    p->print();
}

int main()
{
    debug_print(2);
    return 0;
}
```