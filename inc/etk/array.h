/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ETK_ARRAY_H_INCLUDED
#define ETK_ARRAY_H_INCLUDED

#include "types.h"
#include "loop_range.h"

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

/**
 * \brief Returns an iterator to the end of the list.
 */
    Iterator end()
    {
        Iterator iter(*this);
        iter.pos = L;
        return iter;
    }

    T& operator[](uint32 pos)
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
    T buf[L];
};

}

#endif // ETK_ARRAY_H_INCLUDED
