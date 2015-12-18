#ifndef ETK_ARRAY_H_INCLUDED
#define ETK_ARRAY_H_INCLUDED

#include <stdint.h>
#include "loop_range.h"

namespace etk
{

template <typename T, uint32_t L> class Array
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
        int64_t pos;
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

/**
 * \brief Returns an iterator to the end of the list.
 */
    Iterator end()
    {
        Iterator iter(*this);
        iter.pos = L;
        return iter;
    }

    T& operator[](uint32_t pos)
    {
        if(pos < L)
            return buf[pos];
        return buf[L-1];
    }

    uint32_t size()
    {
        return L;
    }

    void fill(T t)
    {
        for(auto i : *this)
            i = t;
    }

private:
    T buf[L];
};

}

#endif // ETK_ARRAY_H_INCLUDED
