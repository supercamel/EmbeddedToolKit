/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef ETK_ARRAY_H_INCLUDED
#define ETK_ARRAY_H_INCLUDED

#include "types.h"
#include "loop_range.h"
#include "math_util.h"

#ifndef __AVR__
#include <initializer_list>
#endif



namespace etk
{

template <typename T, uint32 L> class Array
{
public:
    Array()
    {

    }

    Array(T* t)
    {
        for(auto i : range(L))
            buf[i] = t[i];
    }

    Array(const T* t)
    {
        for(auto i : range(L))
            buf[i] = t[i];
    }

#ifndef __AVR__
    Array(std::initializer_list<T> il)
    {
        uint32 l = etk::min<uint32>(L, il.size());
        auto b = il.begin();
        for(uint32 i = 0; i < l; i++)
            buf[i] = *b++;
    }
#endif

    template <typename... Args>
    Array(T t, Args... rest) : Array(ArrayInitCounter(), rest...)
    {
        static_assert(sizeof...(Args) < L, "Array initialiser is longer than the array.");
        buf[0] = t;
    }


    class Iterator
    {
        friend class Array;
    public:
        Iterator(Array& array) : array(&array) {
            pos = 0;
        }

        T& operator*()
        {
            return (*array)[pos];
        }

        Iterator& operator++ ()
        {
            pos++;
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator iter(*this);
            ++(*this);
            return iter;
        }

        bool operator==(Iterator iter)
        {
            return (pos == iter.pos);
        }

        bool operator!=(Iterator iter)
        {
            return (pos != iter.pos);
        }

    private:
        Array* array;
        int64 pos;
    };


    /**
     * \brief Returns an iterator to the first item in the list.
     */
    Iterator begin()
    {
        Iterator iter(*this);
        iter.pos = 0;
        return iter;
    }

    Iterator begin() volatile
    {
        volatile Iterator iter(*this);
        iter.pos = 0;
        return iter;
    }

    /**
     * \brief Returns an iterator to the end of the list.
     */
    Iterator end()
    {
        Iterator iter(*this);
        iter.pos = L;
        return iter;
    }

    Iterator end() volatile
    {
        volatile Iterator iter(*this);
        iter.pos = L;
        return iter;
    }

    T& operator[](uint32 pos)
    {
        if(pos < L)
            return buf[pos];
        return buf[L-1];
    }

    T operator[](uint32 pos) const
    {
        if(pos < L)
            return buf[pos];
        return buf[L-1];
    }

    T& at(uint32 pos)
    {
        if(pos < L)
            return buf[pos];
        return buf[L-1];
    }

    T get(uint32 pos) const
    {
        if(pos < L)
            return buf[pos];
        return buf[L-1];
    }

    constexpr uint32 size()
    {
        return L;
    }

    void fill(T t)
    {
        for(auto i : *this)
            i = t;
    }

    T* buffer()
    {
        return buf;
    }

private:
    struct ArrayInitCounter
    {
        ArrayInitCounter() { }
        ArrayInitCounter(const ArrayInitCounter& c)
        {
            count = c.count+1;
        }
        uint32 count = 1;
    };

    Array(ArrayInitCounter c, T t)
    {
        if(c.count < L)
            buf[c.count] = t;
    }

    template <typename... Args>
    Array(ArrayInitCounter c, T t, Args... rest) : Array(c, rest...)
    {
        if(c.count < L)
            buf[c.count] = t;
    }

    T buf[L];
};

}

#endif // ETK_ARRAY_H_INCLUDED
