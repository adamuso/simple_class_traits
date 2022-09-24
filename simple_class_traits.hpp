#pragma once

#define trait_impl_expand(...) __VA_ARGS__
#define trait_impl(T, V) template<> class trait::impl<T, V> : public T::ref::container<V>
#define trait_impl_ext(T, D) template<typename V> class trait::impl<T, V, typename trait::impl<D, V>::type> : public T::ref::container<V>
#define trait_impl_gen(TP, T, ...) template<trait_impl_expand TP> class trait::impl<T, __VA_ARGS__> : public T::ref::container<__VA_ARGS__>

namespace trait
{
    template <typename T, typename V, typename = void>
    class impl;

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

    // Trait T for TraitRef<T> should always be implemented  
    template<typename T> 
    class impl<T, ref<T>> : public container<T, T>
    {

    };

    template <typename T>
    class shared_ptr;

    template <typename T, typename Tag = typename ptr_helper::extract_tag<T>::type>
    class ptr
    {
        // TraitPtr is basically a fat pointer consisting of 2 pointers: vtable pointer and data pointer,
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
        T& operator *() { return *reinterpret_cast<T*>(&d); }
        const T& operator *() const { *reinterpret_cast<const T*>(&d); }
    };

    template <typename T, typename Tag>
    class ref : protected ptr<T, Tag>
    {
        // TraitRef tries to mimic c++ references behavior 
    protected:
        ref(void* vtable, void* data)
            : ptr<T, Tag>(vtable, data)
        {
        };

    public:
        template <typename V>
        using container = container<T, V>;
        using shared = shared_ptr<T>;
        // using ptr = ptr<T, Tag>;

        ref() = delete;
        ref(ref& o) = default;
        ref(ref&& o) = default;

        ref(T& v)
            : ptr<T, Tag>(ptr_helper::extract_v_table(v), reinterpret_cast<void**>(&v)[1])
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(V& v)
            : ptr<T, Tag>(std::addressof(v))
        {
        };

        template <typename V, typename = impl<Tag, V>>
        ref(V&& v)
            : ptr<T, Tag>(std::addressof(v))
        {
        };

        ref& operator=(const ref&) = delete;
        ref& operator=(ref&& o) = delete;

        using ptr<T, Tag>::operator->;
        using ptr<T, Tag>::operator*;

        operator T&() { return *reinterpret_cast<T*>(&this->d); }
        operator const T& () const { *reinterpret_cast<const T*>(&this->d); }

        ptr<T, Tag> operator&() { return *this; }

        friend shared_ptr<T>;
    };
}