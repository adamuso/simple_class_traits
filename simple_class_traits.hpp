#pragma once

#define trait_impl(T, V) template<> class TraitImpl<T, V> : public T::ref::container<V>
#define trait_impl_ext(T, D) template<typename V> class TraitImpl<T, V, typename TraitImpl<D, V>::type> : public T::ref::container<V>

template <typename T>
class TraitRef;

template <typename T, typename V>
class TraitImplContainer : public T
{
public: 
    typedef void type;

    V* self;
};

template <typename T, typename V>
class TraitImplContainer<T, TraitRef<V>> : public T
{
public:
    V* self;
};

template <typename T, typename V, typename = void>
class TraitImpl;

// If a template for V is defined then auto implement template for const V
// by derriving from its non const template
template <typename T, typename V>
class TraitImpl<T, const V> : public TraitImpl<T, V>
{

};

// Trait T for TraitRef<T> should always be implemented  
template<typename T> 
class TraitImpl<T, TraitRef<T>> : public TraitImplContainer<T, T>
{

};

class TraitRefHelper
{
public:
    template <typename V>
    static void* extract_v_table()
    {
        V v;
        return reinterpret_cast<void**>(&v)[0];
    }

    template <typename V>
    static void* extract_v_table(V& v)
    {
        return reinterpret_cast<void**>(&v)[0];
    }
};

template <typename T>
class TraitSharedRef;

template <typename T>
class TraitPtr
{
    // TraitPtr is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer,
protected:
    struct TraitRefData
    {
        void* vtable;
        void* data;
    };

    TraitRefData d;

    TraitPtr(void* vtable, void* data)
        : d { vtable, data }
    {
    };
public:
    TraitPtr() = delete;
    TraitPtr(TraitPtr& o) = default;
    TraitPtr(TraitPtr&& o)
        : d(o.d)
    {
        o.d.vtable = nullptr;
        o.d.data = nullptr;
    }

    template <typename V, typename = TraitImpl<T, V>>
    TraitPtr(V* v)
    {
        static_assert(sizeof(TraitPtr) == sizeof(TraitImpl<T, V>));

        d.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        d.data = v;
    };

    TraitPtr& operator=(const TraitPtr&) = default;
    TraitPtr& operator=(TraitPtr&& o)
    {
        d = o.d;

        o.d.vtable = nullptr;
        o.d.data = nullptr;

        return *this;
    };

    T* operator->() { return reinterpret_cast<T*>(&d); }
    const T* operator->() const { return reinterpret_cast<const T*>(&d); }
    T& operator *() { return *reinterpret_cast<T*>(&d); }
    const T& operator *() const { *reinterpret_cast<const T*>(&d); }
};

template <typename T>
class TraitRef : protected TraitPtr<T>
{
    // TraitRef tries to mimic c++ references behavior 
protected:
    TraitRef(void* vtable, void* data)
        : TraitPtr<T>(vtable, data)
    {
    };

public:
    template <typename V>
    using container = TraitImplContainer<T, V>;
    using shared = TraitSharedRef<T>;
    using ptr = TraitPtr<T>;

    TraitRef() = delete;
    TraitRef(TraitRef& o) = default;
    TraitRef(TraitRef&& o) = default;

    TraitRef(T& v)
        : TraitPtr<T>(TraitRefHelper::extract_v_table(v), &v)
    {
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V& v)
        : TraitPtr<T>(std::addressof(v))
    {
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V&& v)
        : TraitPtr<T>(std::addressof(v))
    {
    };

    TraitRef& operator=(const TraitRef&) = delete;
    TraitRef& operator=(TraitRef&& o) = delete;

    using TraitPtr<T>::operator->;
    using TraitPtr<T>::operator*;

    operator T&() { return *reinterpret_cast<T*>(&this->d); }
    operator const T& () const { *reinterpret_cast<const T*>(&this->d); }

    TraitPtr<T> operator&() { return *this; }

    friend TraitSharedRef<T>;
};
