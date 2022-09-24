#pragma once

#include <memory>

namespace trait {
    template <typename T>
    class shared_ptr
    {
    private:
        struct data
        {
            void* vtable;
            std::shared_ptr<void> data;
        };

        data ptr;

        // trait::shared_ptr needs to extends std::shared_ptr with a vtable
    public:
        shared_ptr(const shared_ptr& o)
            : ptr { o.ptr.vtable, o.ptr.data }
        {

        }

        shared_ptr(shared_ptr&& o)
            : ptr { o.ptr.vtable, o.ptr.data }
        {
            o.ptr.vtable = nullptr;
            o.ptr.data = nullptr;
        }

        template <typename V, typename = impl<T, V>>
        shared_ptr(const std::shared_ptr<V>& v)
        {
            static_assert(sizeof(shared_ptr) - sizeof(void*) == sizeof(impl<T, V>));

            ptr.vtable = ptr_helper::extract_v_table<impl<T, V>>();
            ptr.data = v;
        };

        template <typename V, typename = impl<T, V>>
        shared_ptr(std::shared_ptr<V>&& v)
        {
            static_assert(sizeof(shared_ptr) - sizeof(void*) == sizeof(impl<T, V>));

            ptr.vtable = ptr_helper::extract_v_table<impl<T, V>>();
            ptr.data = v;
        };

        T* operator->() { return reinterpret_cast<T*>(&ptr); }
        const T* operator->() const { return reinterpret_cast<const T*>(&ptr); }
        ref<T> operator *() { return ref<T>(ptr.vtable, ptr.data.get()); }
        const ref<T> operator *() const { return ref<T>(ptr.vtable, ptr.data.get()); }
    };
}
