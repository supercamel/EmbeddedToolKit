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

#ifndef ETK_LIST_H_INCLUDED
#define ETK_LIST_H_INCLUDED

#include <stdint.h>
#include "math_util.h"

#include <iostream>
using namespace std;

namespace etk
{

/**
 * \class List
 *
 * \brief The list class is an iterable container object. A list can contain up to a maximum number of items determined by the template parameter L.
 * List is supposed to provide a similar look and feel to std::vector, but without using dynamic memory allocation.
 *
 * @tparam T The type of object that the list contains.
 * @tparam L The maximum size of the list.
 */


template <typename T, uint32_t L> class List
{
public:
    List()
    {
        list_end = -1;
    }

    ~List()
    {
        T* pt = (T*)space;
        for(uint32_t i = 0; i < size(); i++)
            (&pt[i])->~T();
    }
    List(List& list)
    {
        for(uint32_t i = 0; i < sizeof(space); i++)
            space[i] = list.space[i];
        list_end = list.list_end;
    }

    List(const List& list)
    {
        for(uint32_t i = 0; i < sizeof(space); i++)
            space[i] = list.space[i];
        list_end = list.list_end;
    }

    /**
     * \class Iterator
     *
     * \brief The list iterator makes a list iterable using C++11 ranged for loop syntax.
     * @code
     etk::List<int, 10> list;
     ...
     //print out the entire list
     for(auto i : list)
     {
         cout << i << " ";
     }
     @endcode
     *
     */
    class Iterator
    {
        friend class List;
    public:
        Iterator(List& list) : list(&list)
        {
            pos = 0;
        }

        T& operator*()
        {
            return (*list)[pos];
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
        List* list;
        uint32_t pos;
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
        iter.pos = list_end+1;
        return iter;
    }

    /**
     * \brief Adds an item to the end of the list and increments the size of the list by 1.
     */
    void append(T t)
    {
        if(size() < L)
        {
            list_end++;
            T* pt = (T*)space;
            pt += list_end;
            new(pt)T(t);
        }
    }

    /**
     * \brief Inserts an item into the list.
     * @arg T the item to be inserted.
     * @arg pos where the item will be inserted. If pos is zero, then it will be inserted at the start of the list. If it's 1, it will become the second item in the list.
     */
    void insert(T t, uint32_t pos)
    {
        cout << "inserting " << t << " at " << pos << endl;
        if((pos <= size()) && (size() < L))
        {
            cout << "here" << endl;
            T* pt = (T*)space;
            for(uint32_t i = size(); i >= (pos+1); i--)
                pt[i] = pt[i-1];

            new(&pt[pos])T(t);
            list_end++;
        }
    }

    /**
     * \brief Removes an item from the list.
     * @arg pos The position of the item to remove.
     * @arg padding A value / default object to fill in the space at the end of the list.
     */
    void remove(uint32_t pos)
    {
        if((pos < L) && (size() > 0))
        {
            T* pt = (T*)space;
            (&pt[pos])->~T();
            for(uint32_t i = pos; i < L-1; i++)
                space[i] = space[i+1];
            list_end--;
        }
    }


    void remove_item(auto& item)
    {
        for(uint32_t i = 0; i < size(); i++)
        {
            T* pt = (T*)space;
            if(*pt[i] == item)
            {
                remove(i);
                /*
                delete &pt[pos];
                for(int32_t i = pos; i < L-1; i++)
                    space[i] = space[i+1];
                list_end--;
                */
            }
        }
    }


    /**
     * \brief Removes len number of items from the list, starting as pos.
     * @arg pos The position of the first item to erase.
     * @arg len The number of items to remove.
     */
    void erase(uint32_t pos, uint32_t len)
    {
        for(uint32_t i = 0; i < len; i++)
            remove(pos);
    }

    /**
     * \brief Destroys every item in the list.
     */
    void clear()
    {
        T* pt = (T*)space;
        for(uint32_t i = 0; i < size(); i++)
            (&pt[i])->~T();

        list_end = -1;
    }

    /**
     * \brief Returns the number of a particular item in the list.
     * @arg t The item to count.
     * @return The number of these items in the list.
     */
    uint32_t count(T t)
    {
        uint32_t c = 0;
        T* pt = (T*)space;
        for(uint32_t i = 0; i < size(); i++)
        {
            if(t == pt[i])
                c++;
        }
        return c;
    }

    /**
     * \brief Sets a section of the list to a given value.
     * @arg start The starting position.
     * @arg end The end position.
     * @arg f The item to fill with.
     */
    void fill(uint32_t start, uint32_t end, T f)
    {
        T* pt = (T*)space;
        end = etk::min(end, size());
        start = etk::min(start, end);
        for(uint32_t i = start; i < end; i++)
            pt[i] = f;
    }

    /**
     * \brief Returns the last item on the list and reduces the list length by 1.
     * @return The item at the end of the list.
     */
    T pop_back()
    {
        T* pt = (T*)space;
        if(list_end > 0)
        {
            T ret = pt[list_end];
            (&pt[list_end])->~T();
            list_end--;
            return ret;
        }
        return T();
    }

    /**
     * \brief Same as append();
     */
    void push_back(T t)
    {
        append(t);
    }

    /**
     * \brief This operator allows you to access elements of the list just like a normal array.
     */
    T& operator[](uint32_t pos)
    {
        T* pt = (T*)space;
        if(pos < size())
            return pt[pos];
        return pt[L-1];
    }

    /**
     * \brief Returns the number of items in the list.
     */
    uint32_t size()
    {
        return list_end+1;
    }

    /**
     * \brief Returns the maximum possible number of items that the list can contain.
     */
    uint32_t max_len()
    {
        return L;
    }

    /**
     * \brief Overrides the list end pointer. This function can be convenient but should be used with caution.
     */
    void set_list_end(uint32_t le)
    {
        list_end = le;
    }

    /**
     * \brief Returns a const pointer to the array.
     */
    const T* buffer() const
    {
        return (T*)space;
    }

    /**
     * \brief Returns a pointer to the raw memory used by List.
     */
    T* raw_memory()
    {
        return (T*)space;
    }

private:
    uint8_t space[sizeof(T)*L];
    int32_t list_end;
};

}
#endif

