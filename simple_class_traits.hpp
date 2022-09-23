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
class TraitRef
{
private:
    struct TraitRefData
    {
        void* vtable;
        void* data;
    };

    TraitRefData ptr;

    TraitRef(void* vtable, void* data)
        : ptr { vtable, data }
    {
    };

    // TraitRef is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer,
    // this class tries to mimic c++ references behavior 
public:
    template <typename V>
    using container = TraitImplContainer<T, V>;
    using shared = TraitSharedRef<T>;

    TraitRef() = delete;

    TraitRef(TraitRef& o)
        : ptr(o.ptr)
    {

    };
    
    TraitRef(TraitRef&& o)
        : ptr(o.ptr)
    {
        o.ptr.vtable = nullptr;
        o.ptr.data = nullptr;
    }

    TraitRef(T& v)
    {
        ptr.vtable = TraitRefHelper::extract_v_table(v);
        ptr.data = &v;
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V& v)
    {
        static_assert(sizeof(TraitRef) == sizeof(TraitImpl<T, V>));

        ptr.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        ptr.data = &v;
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V&& v)
    {
        static_assert(sizeof(TraitRef) == sizeof(TraitImpl<T, V>));

        ptr.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        ptr.data = &v;
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V* v)
    {
        static_assert(sizeof(TraitRef) == sizeof(TraitImpl<T, V>));

        ptr.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        ptr.data = v;
    };

    TraitRef& operator=(const TraitRef&) = delete;
    TraitRef& operator=(TraitRef&& o) = delete;

    T* operator->() { return reinterpret_cast<T*>(&ptr); }
    const T* operator->() const { return reinterpret_cast<const T*>(&ptr); }
    T& operator *() { return *reinterpret_cast<T*>(&ptr); }
    const T& operator *() const { *reinterpret_cast<const T*>(&ptr); }
    operator T&() { return *reinterpret_cast<T*>(&ptr); }
    operator const T& () const { *reinterpret_cast<const T*>(&ptr); }


    friend TraitSharedRef<T>;
};
