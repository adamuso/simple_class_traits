# Simple Class Traits

Inspired by Rust Traits I've recreated this mechanism in C++ using templates and specializations.

## Work in progres...

TODO:
- [X] Defining traits as simple class
- [X] Basic trait implementation
- [X] Generic trait implementation
- [X] Allow implementing traits for other traits
- [X] C++11 compatible
- [X] Generic trait definition
- [X] Add a namespace for traits 
- [X] No std library dependency on core implementation
- [ ] Simplify trait implementation declaration 
- [ ] Make it simple
- [ ] Make it 100% safe
- [ ] Handle cases like `std::vector<Trait::ref>` 
- [ ] Somehow allow using traits with `shared_ptr`

## Example

Here are some examples with Rust equivalent. See more examples in `example` directory.

Example 1:

```rust
trait DebugPrint {
    fn print(&self);
}

impl DebugPrint for i32 {
    fn print(&self) {
        println!("{}", self);
    }
}

fn debug_print(p: impl DebugPrint) {
    p.print();
}

fn main() {
    let a: i32 = 2;
    debug_print(a);
}
```

```cpp
// example/simplest_print.cpp

#include <iostream>

#include "../simple_class_traits.hpp"

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


```rust
trait DebugPrint {
    fn print(&self);
}

impl DebugPrint for i32 {
    fn print(&self) {
        println!("{}", self);
    }
}

fn debug_print(p: impl DebugPrint) {
    p.print();
}

fn main() {
    debug_print(2);
}
```

```cpp
// example/simplest_print.cpp

#include <iostream>

#include "../simple_class_traits.hpp"

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

Example 2:

```rust
trait SizedIntArray {
    fn size(&self) -> usize;
    fn item(&self, index: usize) -> i32;
}

trait DebugPrint {
    fn print(&self);
}

impl DebugPrint for i32 {
    fn print(&self) {
        print!("{}", self);
    }
}

impl<T> DebugPrint for T 
    where T: SizedIntArray {
    fn print(&self) {
        for i in 0..self.size() {
            self.item(i).print();
            print!(", ");
        }
        
        println!();
    }
}

impl SizedIntArray for Vec<i32> {
    fn size(&self) -> usize {
        self.len()
    }
    
    fn item(&self, index: usize) -> i32 {
        self[index]
    }
}

fn main() {
    let a: Vec<i32> = vec![1, 2, 3];

    a.print();
}
```

```cpp
// example/simple_print.cpp

#include <iostream>
#include <vector>

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
```