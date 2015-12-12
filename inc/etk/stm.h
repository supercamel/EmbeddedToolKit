#ifndef ETK_SHORT_TERM_MEMORY_H
#define ETK_SHORT_TERM_MEMORY_H

#include "loop_range.h"

namespace etk
{

template <typename T, uint32_t LEN> class ShortTermMemory
{
public:
    class Iterator
    {
    public:
        inline Iterator(ShortTermMemory<T,LEN>& p, uint32_t position=0) : stm(p)
        {
            pos = position;
        }

        inline T operator*()
        {
            return stm.peek_ahead(pos);
        }

        inline Iterator& operator++ ()
        {
            pos++;
            return *this;
        }

        inline bool operator==(Iterator iter)
        {
            return (pos == iter.pos);
        }

        inline bool operator!=(Iterator iter)
        {
            return (pos != iter.pos);
        }

    private:
        ShortTermMemory<T,LEN>& stm;
        uint32_t pos = 0;
    };


    ShortTermMemory()
    {
        start = 0;
        buf_end = 0;
    }

    Iterator begin()
    {
        return Iterator(*this);
    }

    Iterator end()
    {
        return Iterator(*this, LEN);
    }

    void put(T b)
    {
        buf_end = (buf_end + 1) % LEN;
        buf[buf_end] = b;
        if(is_full())
        {
            start = (start+1)%LEN;
        }
    }
    
    bool is_full()
    {
        return (end + 1) % LEN == start;
    }


    void increment()
    {
        buf_end = (buf_end + 1) % LEN;
    }

    T peek_ahead(uint16_t n=0)
    {
        uint16_t pos = (start+n) % LEN;
        return buf[pos];
    }

    void empty()
    {
        start = 0;
        buf_end = 0;
    }

    void fill(T t)
    {
        for(auto i : range(LEN))
            put(t);
        start = 0;
        buf_end = LEN-1;
    }

private:
    uint16_t start;
    uint16_t buf_end;
    T buf[LEN];
};

};

#endif
