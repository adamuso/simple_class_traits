#pragma once

#define trait_impl_expand(...) __VA_ARGS__
#define trait_impl(T, V) template<> class trait::impl<T, V> : public trait::container<T, V>
#define trait_impl_ext(T, D) template<typename V> class trait::impl<T, V, typename trait::impl<D, V>::type> : public trait::container<T, V>
#define trait_impl_gen(TP, T, ...) template<trait_impl_expand TP> class trait::impl<T, __VA_ARGS__> : public trait::container<T, __VA_ARGS__>

namespace trait
{
    template <typename T, typename V, typename = void>
    class impl;

    template <typename T, typename V>
    using impl_exists = typename impl<T, V>::type;

    class ptr_helper
    {
    public:
        template<typename T, typename = void>
        struct sizeof_impl
        {
            constexpr static int value = -1; 
        };

        template<typename T, typename V>
        struct sizeof_impl<impl<T, V>, typename impl<T, V>::type>
        {
            constexpr static int value = sizeof(impl<T, V>); 
        };

        template<class T, class R = void>  
        class extract_tag_enable 
        { 
        public:
            typedef R type; 
        };

        template <typename T, typename = void>
        class extract_tag
        {
        public:
            typedef T type;
        };

        template <typename T>
        class extract_tag<T, typename extract_tag_enable<typename T::tag>::type>
        {
        public:
            typedef typename T::tag type;
        };

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

    template <typename T, typename Tag = typename ptr_helper::extract_tag<T>::type>
    class ref;

    template <typename T, typename Tag = typename ptr_helper::extract_tag<T>::type>
    class ptr;

    template <typename T, typename Tag = typename ptr_helper::extract_tag<T>::type>
    ptr<T, Tag> unsafe_create_ptr(void* vtable, void* data)
    {
        return ptr<T, Tag>(vtable, data);
    }

    template <typename T, typename V>
    class container : public T
    {
    public: 
        typedef void type;

        V* self;
    };

    template <typename T, typename V>
    class container<T, ref<V>> : public T
    {
    public:
        typedef void type;
        
        V* self;
    };

    // If a template for V is defined then auto implement template for const V
    // by derriving from its non const template
    template <typename T, typename V>
    class impl<T, const V> : public impl<T, V>
    {

    };

    // Trait T for trait::ref<T> should always be implemented  
    template<typename T> 
    class impl<T, ref<T>> : public container<T, T>
    {

    };

    template <typename T, typename Tag>
    class ptr
    {
        // trait::ptr is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer,
    protected:
        struct data
        {
            void* vtable;
            void* data;
        };

        data d;

        ptr(void* vtable, void* data)
            : d { vtable, data }
        {
        };
    public:
        ptr() = delete;
        ptr(ptr& o) = default;
        ptr(ptr&& o)
            : d(o.d)
        {
            o.d.vtable = nullptr;
            o.d.data = nullptr;
        }

        template <typename V, typename = impl<Tag, V>>
        ptr(V* v)
        {
            static_assert(ptr_helper::sizeof_impl<impl<Tag, V>>::value >= 0, "trait::impl<T, V> is not defined. Please create an implmentation for this trait.");
            static_assert(sizeof(ptr) == ptr_helper::sizeof_impl<impl<Tag, V>>::value, "trait::impl cannot have additional fields.");
            static_assert(sizeof(T) == sizeof(void*), "Trait cannot have additional fields.");

            d.vtable = ptr_helper::extract_v_table<impl<Tag, V>>();
            d.data = const_cast<V*>(v);
        };

        template <typename V, typename = impl<Tag, V>>
        ptr(const V* v)
            : ptr(const_cast<V*>(v))
        {
            
        }

        ptr& operator=(const ptr&) = default;
        ptr& operator=(ptr&& o)
        {
            d = o.d;

            o.d.vtable = nullptr;
            o.d.data = nullptr;

            return *this;
        };

        T* operator->() { return reinterpret_cast<T*>(&d); }
        const T* operator->() const { return reinterpret_cast<const T*>(&d); }
        ref<T> operator *() { return ref<T>(d.vtable, d.data); }
        const ref<T> operator *() const { return ref<T>(d.vtable, d.data); }

        template <typename _T, typename _Tag> friend ptr<_T, _Tag> unsafe_create_ptr(void* vtable, void* data);
    };

    template <typename T, typename Tag>
    class ref : protected ptr<T, Tag>
    {
        // trait::ref tries to mimic c++ references behavior 
    protected:
        ref(void* vtable, void* data)
            : ptr<T, Tag>(vtable, data)
        {
        };

        ref* address_of() 
        {
            return this;
        }
    public:
        template <typename V>
        using container = container<T, V>;

        ref() = delete;
        ref(ref& o) = default;
        ref(ref&& o) = default;

        // TODO: handle this case better, currently it works under assumption that a trait class
        // defined by user will be only inherited by trait::impl, that is why there is a cast to
        // void** and element [1] is extracted, in trait::impl this is always a pointer to data 
        ref(T& v)
            : ptr<T, Tag>(ptr_helper::extract_v_table(v), reinterpret_cast<void**>(&v)[1])
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(V& v)
            : ptr<T, Tag>(&v)
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(V&& v)
            : ptr<T, Tag>(&v)
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(ref<V>& v)
            : ptr<T, Tag>(v.address_of())
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(ref<V>&& v)
            : ptr<T, Tag>(v.address_of())
        {
        };

        ref& operator=(const ref&) = delete;
        ref& operator=(ref&& o) = delete;

        using ptr<T, Tag>::operator->;

        operator T&() { return *reinterpret_cast<T*>(&this->d); }
        operator const T& () const { *reinterpret_cast<const T*>(&this->d); }

        ptr<T, Tag> operator&() { return *this; }

        template<typename, typename> friend class ref;
        template<typename, typename> friend class ptr;
    };
}