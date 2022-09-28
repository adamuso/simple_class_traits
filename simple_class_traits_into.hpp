#pragma once

#include "simple_class_traits.hpp"

// This is an example implementation of Into trait

namespace trait {
    template <typename Output>
    class Into
    {
    public:
        using ref = trait::ref<Into>;

        Output into() const;
    };

    template <typename V>
    class IntoConverter
    {
    public:
        V* v;

        IntoConverter(V& v)
            : v(&v)
        {
            
        }

        IntoConverter(const V& v)
            : v(const_cast<V*>(&v))
        {
            
        }

        template <typename T>
        T operator+(const T& t)
        {
            return impl_ref<Into<T>, V>(*v).into() + t;
        }
        
        V operator+(const IntoConverter<V>& t)
        {
            return *v + *t.v;
        }

        template <typename T>
        T operator+(const IntoConverter<T>& t) = delete;

        template <typename Output>
        operator Output() 
        {
            return impl_ref<Into<Output>, V>(*v).into();
        }
    };

    template <typename T, typename V>
    static T operator+(const T& t, const IntoConverter<V>& ic)
    {
        return t + impl_ref<Into<T>, V>(*ic.v).into();
    }

    template <typename T>
    class impl<Into<T>, T> : public container<Into<T>, T>
    {
    public:
        T into() 
        {
            return *this->self;
        }
    };

    template <typename T>
    class impl<Into<IntoConverter<T>>, T> : public container<Into<IntoConverter<T>>, T>
    {
    public:
        T into() 
        {
            return *this->self;
        }
    };

    template <typename V>
    IntoConverter<V> from(const V& v)
    {
        return v;
    }

    template <typename V>
    IntoConverter<V> from(V& v)
    {
        return v;
    }

    template <typename Output, typename V>
    Output from(const V& v)
    {
        return impl_ref<Into<Output>, V>(v).into();
    }

    template <typename Output, typename V>
    Output from(V& v)
    {
        return impl_ref<Into<Output>, V>(v).into();
    }

    struct IntoTag {};

    IntoTag into() 
    {
        return IntoTag();
    };

    // we cannot overload -> or . so this is the best we can do
    // remember this is just an example, I think that this is not 
    // a right way to do it but is it certainly possible.
    template <typename V>
    static IntoConverter<V> operator->*(const V& v, IntoTag)
    {
        return v;
    }
}
