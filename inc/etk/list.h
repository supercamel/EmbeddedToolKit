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


template <typename T, int32_t L> class List
{
public:
    List()
    {
        list_end = -1;
    }
    List(List& list)
    {
        for(int32_t i = 0; i < L; i++)
            items[i] = list.items[i];
        list_end = list.list_end;
    }

    List(const List& list)
    {
        for(int32_t i = 0; i < L; i++)
            items[i] = list.items[i];
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
        Iterator(List& list) : list(&list) {
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
        int32_t pos;
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
            items[list_end] = t;
        }
    }

/**
 * \brief Inserts an item into the list.
 * @arg T the item to be inserted.
 * @arg pos where the item will be inserted. If pos is zero, then it will be inserted at the start of the list. If it's 1, it will become the second item in the list.
 */
    void insert(T t, int32_t pos)
    {
        if((pos <= size()) && (size() < L))
        {
            for(int32_t i = L-1; i >= (pos+1); i--)
                etk::swap<T>(items[i], items[i-1]);
            list_end++;
            items[pos] = t;
        }
    }

/**
 * \brief Removes an item from the list.
 * @arg pos The position of the item to remove.
 * @arg padding A value / default object to fill in the space at the end of the list.
 */
    void remove(int32_t pos, T padding)
    {
        if((pos < L) && (size() > 0))
        {
            for(int32_t i = pos; i < L-1; i++)
                etk::swap<T>(items[i], items[i+1]);
            list_end--;
            items[L-1] = padding;
        }
    }

/**
 * \brief Removes len number of items from the list, starting as pos.
 * @arg pos The position of the first item to erase.
 * @arg len The number of items to remove.
 * @arg padding A value / default object to fill in the space at the end of the list.
 */
    void erase(int32_t pos, int32_t len, T padding)
    {
        for(int32_t i = 0; i < len; i++)
            remove(pos, padding);
    }

/**
 * \brief Resets the list end to make it appear as if the list has been emptied.
 */
    void clear()
    {
        list_end = -1;
    }

/**
 * \brief Returns the number of a particular item in the list.
 * @arg t The item to count.
 * @return The number of these items in the list.
 */
    int32_t count(T t)
    {
        int32_t c = 0;
        for(int32_t i = 0; i < size(); i++)
        {

            if(t == items[i])
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
    void fill(int32_t start, int32_t end, T f)
    {
        end = etk::min(end, size());
        start = etk::min(start, end);
        for(int32_t i = start; i < end; i++)
            items[i] = f;
    }

/**
 * \brief Returns the last item on the list and reduces the list length by 1.
 * @return The item at the end of the list.
 */
    T pop_back()
    {
        if(list_end > 0)
            return items[list_end--];
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
    T& operator[](int32_t pos)
    {
        if(pos < L)
            return items[pos];
        return items[L-1];
    }

/**
 * \brief Returns the number of items in the list.
 */
    int32_t size()
    {
        return list_end+1;
    }

/**
 * \brief Returns the maximum possible number of items that the list can contain.
 */
    int32_t max_len()
    {
        return L;
    }

/**
 * \brief Overrides the list end pointer. This function can be convenient but should be used with caution.
 */
    void set_list_end(int32_t le)
    {
        list_end = le;
    }

/**
 * \brief Returns a const pointer to the array.
 */
    const T* buffer() const
    {
        return items;
    }

/**
 * \brief Returns a pointer to the raw memory used by List.
 */
    T* raw_memory()
    {
        return items;
    }

private:
    T items[L];
    int32_t list_end;
};

}
#endif

