
#ifndef ETK_BITS_H_INCLUDED
#define ETK_BITS_H_INCLUDED

#include "staticstring.h"
#include <type_traits>

namespace etk
{

template <class T> class Bits
{
public:
    Bits()
    {
    	static_assert(std::is_integral<T>::value,
                  "Bits type must be an integral");
        bte = 0;
    }

    Bits(T b)
    {
    	static_assert(std::is_integral<T>::value,
                  "Bits type must be an integral");
        bte = b;
    }

    bool set_bit(uint32_t n, bool on = true)
    {
        if(n < (sizeof(T)*8))
        {
            if(on)
                bte |= (T)1 << (T)(n);
            else
                bte &= ~((T)1 << (T)(n));
            return true;
        }
        return false;
    }

    bool read_bit(uint32_t n)
    {
        if(n < (sizeof(T)*8))
        {
            bool on = bte & ((T)1 << (T)(n));
            return on;
        }
        return false;
    }

    StaticString<sizeof(T)*8+1> to_string()
    {
        StaticString<sizeof(T)*8+1> string;
        for(uint32_t i = 0; i < (sizeof(T)*8); i++)
        {
            if(read_bit(i))
                string += "1";
            else
                string += "0";
        }
        return string;
    }

    T get()
    {
        return bte;
    }

    void set(T b)
    {
        bte = b;
    }

private:
    T bte;
};

}


#endif // BITS_H_INCLUDED
