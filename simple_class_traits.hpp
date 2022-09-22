#pragma once

#define trait_impl(T, V) template<> class TraitImpl<T, V> : public T::ref::container<V>
#define trait_impl_ext(T, D) template<typename V> class TraitImpl<T, V, typename TraitImpl<D, V>::type> : public T::ref::container<V>

template <typename T>
struct RmConst
{
    typedef T type;
};

template <typename T>
struct RmConst<const T>
{
    typedef T type;
};

template <typename T, typename V>
class TraitImplContainer : public T
{
public:
    typedef void type;

    V* self;
};

template <typename T>
class TraitImplContainer<T, void> : public T
{
public:
    void* self;
};

template <typename T>
class TraitRef;

template <typename T, typename V>
class TraitImplContainer<T, TraitRef<V>> : public T
{
public:
    V* self;
};

template <typename T, typename V, typename = void>
class TraitImpl;

template <typename T, typename V>
class TraitImpl<T, const V> : public TraitImpl<T, V>
{

};

template<typename T> 
class TraitImpl<T, TraitRef<T>> : public TraitImplContainer<T, T>
{

};

template<typename T>
class TraitImpl<T, void> : public TraitImplContainer<T, void>
{
public:
    template<typename V>
    static TraitImpl<T, V> create(V& v)
    {
        TraitImpl<T, V> ti;
        ti.self = const_cast<typename RmConst<V>::type*>(&v);
        return ti;
    }

    template<typename V>
    static TraitImpl<T, TraitRef<V>> create(TraitRef<V>& v)
    {
        TraitImpl<T, TraitRef<V>> ti;
        ti.self = const_cast<typename RmConst<V>::type*>(reinterpret_cast<V*>(&v));
        return ti;
    }
};


struct TraitRefData
{
    void* vtable;
    void* data;
};

template <typename T>
class TraitRef
{
private:
    TraitRefData ptr;

    // TraitRef is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer
public:
    template <typename V>
    using container = TraitImplContainer<T, V>;

    TraitRef(T& v)
    {
        static_assert(sizeof(TraitImpl<T, void>) % sizeof(void*) == 0);

        void** src_fat_ptr = reinterpret_cast<void**>(&v);

        ptr.vtable = src_fat_ptr[0];
        ptr.data = src_fat_ptr[1];
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V& v)
    {
        static_assert(sizeof(TraitImpl<T, void>) == sizeof(TraitImpl<T, V>));
        static_assert(sizeof(TraitImpl<T, void>) % sizeof(void*) == 0);

        auto ti = TraitImpl<T, void>::create(v);
        void** src_fat_ptr = reinterpret_cast<void**>(&ti);
       
        ptr.vtable = src_fat_ptr[0];
        ptr.data = src_fat_ptr[1];
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V&& v)
    {
        static_assert(sizeof(TraitImpl<T, void>) == sizeof(TraitImpl<T, V>));
        static_assert(sizeof(TraitImpl<T, void>) % sizeof(void*) == 0);

        auto ti = TraitImpl<T, void>::create(v);
        void** src_fat_ptr = reinterpret_cast<void**>(&ti);
        
        ptr.vtable = src_fat_ptr[0];
        ptr.data = src_fat_ptr[1];
    };

    T* operator->() { return reinterpret_cast<T*>(&ptr); }
    const T* operator->() const { return reinterpret_cast<const T*>(&ptr); }
};
