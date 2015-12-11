
#ifndef RING_BUF_H
#define RING_BUF_H

#include <stdint.h>

namespace etk
{

template <class T, bool overwrite = false> class RingBuffer
{
public:
    RingBuffer(T* buffer, uint16_t sz)
    {
        size = sz;
        start = 0;
        end = 0;
        buf = buffer;
    }

    bool is_full()
    {
        return (end + 1) % size == start;
    }

    uint16_t available()
    {
        return (uint16_t)(size + end - start) % size;
    }

    void put(T b)
    {
        if(!overwrite)
        {
            if(is_full())
                return;
        }
        buf[end] = b;
        end = (end + 1) % size;
    }

    void increment()
    {
        end = (end + 1) % size;
    }

    T get()
    {
        T ret = buf[start];
        start = (start + 1) % size;
        return ret;
    }

    T peek_ahead(uint16_t n=0)
    {
        uint16_t pos = (start+n) % size;
        return buf[pos];
    }

    void empty()
    {
        start = 0;
        end = 0;
    }

private:
    uint16_t size;
    uint16_t start;
    uint16_t end;
    T* buf;
};

}

#endif // RING_BUF_H
