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

## Usage

### Defining a trait

To create a new trait you need to define a class that have virtual method.

```cpp
class DebugPrint
{
public:
    virtual void print() const = 0;
};
```

```rust
// Comparison to rust
trait DebugPrint {
    fn print(&self);
}
```

Additionally you can add `ref` typedef for convinience. It can make easier accessing the trait reference type.

```cpp
class DebugPrint
{
public:
    typedef trait::ref<DebugPrint> ref;

    virtual void print() const = 0;
};

// Defining trait as parameter in function
// without `ref` typedef:
void example(trait::ref<DebugPrint> dp);

// with `ref` typedef
void example2(DebugPrint::ref dp);
```

Traits can be also defined as templates.

```cpp
template <typename T>
class GenericTrait
{
public:
    virtual void do_something(T v) const = 0;
};
```

```rust
// Comparison to rust
trait GenericTrait<T> {
    fn do_something(&self, v: T);
}
```

Traits can provide defualt implementation.

```cpp
class SomeTrait
{
public:
    virtual int get_a() const = 0;
    virtual int get_b() const = 0;

    virtual int sum() {
        return get_a() + get_b();
    }
};
```

We can also simulate `Output` type for a function.

```rust
// How its done in rust
trait Add<T> {
    // Here Output is not a part of template
    type Output;

    fn add(&self, other: T) -> Self::Output;
}
```

```cpp
// Output in C++ needs to be a part of template, otherwise we cannot achieve same effect as in rust
template <typename T, typename Output = void>
class Add
{
public:
    // Because `Output` is a part of template, it should not be used for implementation
    // that is why a special `tag` typedef is added below, it basically says that for an
    // trait implementation we should only look at Add<T> without `Output` type argument 
    typedef Add<T> tag;

    virtual Output add(T other) const = 0;
};
```

### Implementing a trait

Creating a trait implmentation in C++ esessntially means that you need to create a specialization for
`trait::impl` template and provide body for virtual methods. To make this process easier there are
few optional `#define` which can make the code more readable. If you don't like using defines then
all traits can be implemented in standard C++ way.

```cpp
// Implementation using define is very simple
trait_impl(DebugPrint, int)
{
public:
    void print() const 
    {
        std::cout << *self;
    }
};
```

```cpp
// This is how it looks without macro
template<>
class trait::impl<DebugPrint, int> : public trait::container<DebugPrint, int>
{
public:
    void print() const 
    {
        std::cout << *self;
    }
};
```

```rust
// Comparison to rust
impl DebugPrint for i32 {
    fn print(&self) {
        print!("{}", self);
    }
}
```

A trait implementation can aslo be generic.

```cpp
// Using a macro
trait_impl_gen((typename T), GenericTrait<T>, T)
{
public:
    void do_something(T v) const 
    {
       // ...
    }
};
```

```cpp
// Without a macro
template<typename T>
class impl::trait<GenericTrait<T>, T> : public trait::container<GenericTrait<T>, T>
{
public:
    void do_something(T v) const 
    {
       // ...
    }
};
```

```rust
// Comparison to rust
impl<T> GenericTrait<T> for T {
    fn do_something(&self, v: T) {
        // ...
    }
}
```

You can implement a trait for another trait.


```cpp
// Using a macro
trait_impl_ext(DebugPrint, SomeTrait)
{
public:
    void print() const 
    {
        // this->self is SomeTrait*
    }
};
```

```cpp
// Without a macro
template<typename V>
class impl::trait<DebugPrint, V, trait::has_trait<V, SomeTrait>> : public trait::container<DebugPrint, V>
{
public:
    void print() const 
    {
        // this->self is SomeTrait*
    }
};
```

```rust
// Comparison to rust
impl<V> DebugPrint for V 
    where V : SomeTrait {
    fn print(&self) {
        // ...
    }
}
```

### Using a trait reference and pointer

WIP

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