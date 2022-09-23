#pragma once

#include <memory>

template <typename T>
class TraitSharedRef
{
private:
    struct TraitRefData
    {
        void* vtable;
        std::shared_ptr<void> data;
    };

    TraitRefData ptr;

    // TraitSharedRef needs to extends shared_ptr with a vtable
public:
    TraitSharedRef(const TraitSharedRef& o)
        : ptr { o.ptr.vtable, o.ptr.data }
    {

    }

    TraitSharedRef(TraitSharedRef&& o)
        : ptr { o.ptr.vtable, o.ptr.data }
    {
        o.ptr.vtable = nullptr;
        o.ptr.data = nullptr;
    }

    template <typename V, typename = TraitImpl<T, V>>
    TraitSharedRef(const std::shared_ptr<V>& v)
    {
        static_assert(sizeof(TraitSharedRef) - sizeof(void*) == sizeof(TraitImpl<T, V>));

        ptr.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        ptr.data = v;
    };

    template <typename V, typename = TraitImpl<T, V>>
    TraitSharedRef(std::shared_ptr<V>&& v)
    {
        static_assert(sizeof(TraitSharedRef) - sizeof(void*) == sizeof(TraitImpl<T, V>));

        ptr.vtable = TraitRefHelper::extract_v_table<TraitImpl<T, V>>();
        ptr.data = v;
    };

    T* operator->() { return reinterpret_cast<T*>(&ptr); }
    const T* operator->() const { return reinterpret_cast<const T*>(&ptr); }
    TraitRef<T> operator *() { return TraitRef<T>(ptr.vtable, ptr.data.get()); }
    const TraitRef<T> operator *() const { return TraitRef<T>(ptr.vtable, ptr.data.get()); }
};
