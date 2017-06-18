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

#ifndef ETK_BITS_H_INCLUDED
#define ETK_BITS_H_INCLUDED

#include "staticstring.h"

#ifndef __AVR__
#include <type_traits>
#endif

namespace etk
{
/**
 * \class Bits
 *
 * \brief Bits provides an easy way to set and read the bits of an integer individually.
 *
 * @code
    etk::Bits<uint8> bits;
    bits.set_bit(7);
    bits.set_bit(1);

    uint8 v = bits.get();

    auto str = bits.to_string();

    cout << "v == " << str.c_str() << endl;
    @endcode
 *
 * @tparam T An int type. For example, uint16 which is 16 bits long.
 */

template <class T> class Bits
{
public:
    Bits()
    {
#ifndef __AVR__
        static_assert(std::is_integral<T>::value,
                      "Bits type must be an integral");
#endif
        bte = 0;
    }

    Bits(T b)
    {
#ifndef __AVR__
        static_assert(std::is_integral<T>::value,
                      "Bits type must be an integral");
#endif
        bte = b;
    }

    /**
     * \brief set_bit allows you to turn a bit on or off.
     *
     * @arg n The bit to modify.
     * @arg on True to set the bit to 1, false to set to 0.
     * @return This function returns false if n is greater than the size of T.
     */
    bool set_bit(const uint32 n, const bool on = true)
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

    /**
     * \brief read_bit reads the value of a bit.
     *
     * @arg n The bit to read.
     * @return true if bit is 1, false if 0
     */
    bool read_bit(const uint32 n) const
    {
        if(n < (sizeof(T)*8))
        {
            bool on = bte & ((T)1 << (T)(n));
            return on;
        }
        return false;
    }

    /**
     * \brief Creates a human readable string of ones and zeroes.
     *
     * @return a StaticString.
     */
    StaticString<sizeof(T)*8+1> to_string() const
    {
        StaticString<sizeof(T)*8+1> string;
        for(uint32 i = (sizeof(T)*8); i != 0; i--)
        {
            if(read_bit(i-1))
                string += "1";
            else
                string += "0";
        }
        return string;
    }

    /**
     * \brief Returns the integer that Bits has been modifying.
     *
     */
    T get() const
    {
        return bte;
    }

    /**
     * \brief Sets the integer to a particular value.
     *
     */
    void set(T b)
    {
        bte = b;
    }

private:
    T bte;
};

typedef Bits<uint8> Bits8;
typedef Bits<uint16> Bits16;
typedef Bits<uint32> Bits32;
typedef Bits<uint64> Bits64;

}


#endif // BITS_H_INCLUDED
