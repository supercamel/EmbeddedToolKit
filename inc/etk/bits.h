
#ifndef ETK_BITS_H_INCLUDED
#define ETK_BITS_H_INCLUDED

#include "staticstring.h"
#include <type_traits>

namespace etk
{
/**
 * \class Bits
 *
 * \brief Bits provides an easy way to set and read the bits of an integer individually. 
 *
 * @code
    etk::Bits<uint8_t> bits;
    bits.set_bit(7);
    bits.set_bit(1);
    
    uint8_t v = bits.get();
    
    auto str = bits.to_string();
    
    cout << "v == " << str.c_str() << endl;
    @endcode
 *
 * @tparam T An int type. For example, uint16_t which is 16 bits long. 
 */
 
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

	/**
	 * \brief set_bit allows you to turn a bit on or off.
	 *
	 * @arg n The bit to modify.
	 * @arg on True to set the bit to 1, false to set to 0.
	 * @return This function returns false if n is greater than the size of T.
	 */
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

	/**
	 * \brief read_bit reads the value of a bit.
	 *
	 * @arg n The bit to read.
	 * @return true if bit is 1, false if 0
	 */
    bool read_bit(uint32_t n)
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
    auto to_string()
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

	/**
	 * \brief Returns the integer that Bits has been modifying.
	 *
	 */
    T get()
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

}


#endif // BITS_H_INCLUDED
