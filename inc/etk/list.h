
#ifndef ETK_LIST_H_INCLUDED
#define ETK_LIST_H_INCLUDED

#include <stdint.h>
#include "math_util.h"

namespace etk
{

template <typename T, uint32_t L> class List
{
public:
    List()
    {
        list_end = -1;
    }
    List(List& list)
    {
        for(uint32_t i = 0; i < L; i++)
            items[i] = list.items[i];
        list_end = list.list_end;
    }

    List(const List& list)
    {
        for(uint32_t i = 0; i < L; i++)
            items[i] = list.items[i];
        list_end = list.list_end;
    }


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
        int64_t pos;
    };


    Iterator begin()
    {
        Iterator iter(*this);
        iter.pos = 0;
        return iter;
    }

    Iterator end()
    {
        Iterator iter(*this);
        iter.pos = list_end;
        return iter;
    }

    void append(T t)
    {
        if(size() < L)
        {
            list_end++;
            items[list_end] = t;
        }
    }

    void insert(T t, uint32_t pos)
    {
        if((pos <= size()) && (size() < L))
        {
            for(uint32_t i = L-1; i >= (pos+1); i--)
                etk::swap<T>(items[i], items[i-1]);
            list_end++;
            items[pos] = t;
        }
    }

    void remove(uint32_t pos, T padding)
    {
        if((pos < L) && (size() > 0))
        {
            for(uint32_t i = pos; i < L-1; i++)
                etk::swap<T>(items[i], items[i+1]);
            list_end--;
            items[L-1] = padding;
        }
    }

    void erase(uint32_t pos, uint32_t len, T padding)
    {
        for(uint32_t i = 0; i < len; i++)
            remove(pos, padding);
    }

    void clear()
    {
        list_end = -1;
    }

    uint32_t count(T t)
    {
        uint32_t c = 0;
        for(uint32_t i = 0; i < size(); i++)
        {

            if(t == items[i])
                c++;
        }
        return c;
    }

    void fill(uint32_t start, uint32_t end, T f)
    {
        end = etk::min(end, size());
        start = etk::min(start, end);
        for(uint32_t i = start; i < end; i++)
            items[i] = f;
    }

    T pop_back()
    {
        if(list_end > 0)
            return items[list_end--];
        return T();
    }

    void push_back(T t)
    {
        append(t);
    }

    T& operator[](uint32_t pos)
    {
        return items[pos];
    }

    uint32_t size()
    {
        return list_end+1;
    }

    const uint32_t max_len() const
    {
        return L;
    }

    void set_list_end(uint32_t le)
    {
        list_end = le;
    }

    const T* buffer() const
    {
        return items;
    }

    T* raw_memory()
    {
        return items;
    }

private:
    T items[L];
    int64_t list_end;
};

}
#endif

