#pragma once

#define trait_impl(T, V) template<> class TraitImpl<T, V> : public TraitImplContainer<T, V>

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
class TraitImpl;

template <typename T, typename V>
class TraitImplContainer : public T
{
public:
    V* self;
};

template<typename T>
class TraitImpl<T, void> : public TraitImplContainer<T, void>
{
public:
    template<typename V>
    static TraitImpl<T, V> create(V& v)
    {
        TraitImpl<T, V> ti;
        ti.self = &v;
        // ti.self = const_cast<typename RmConst<V>::type*>(&v);
        return ti;
    }
};

// template<typename T, typename V>
// class TraitImpl<T, const V> : public TraitImpl<T, V>
// {

// };


template <typename T>
class TraitRef
{
    // TraitRef is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer
public:
    char ptr[sizeof(TraitImpl<T, void>)];

    template <typename V, typename = TraitImpl<T, V>>
    TraitRef(V& v)
    {
        static_assert(sizeof(TraitImpl<T, void>) == sizeof(TraitImpl<T, V>));
        static_assert(sizeof(TraitImpl<T, void>) % sizeof(void*) == 0);

        auto ti = TraitImpl<T, void>::template create<V>(v);
        void** src_fat_ptr = reinterpret_cast<void**>(&ti);
        void** dst_fat_ptr = reinterpret_cast<void**>(&ptr[0]);
    
        for (int i = 0; i < sizeof(ti) / sizeof(void*); i++)
        {
            dst_fat_ptr[i] = src_fat_ptr[i];
        }
    };

    T* operator->() { return reinterpret_cast<T*>(ptr); }
};
