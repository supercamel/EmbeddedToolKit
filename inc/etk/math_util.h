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


#ifndef MATH_UTIL_H_INCLUDED
#define MATH_UTIL_H_INCLUDED

#include "types.h"
#include <math.h>

namespace etk
{

/**
 * \brief u32b is a union for 32bit types.
 * This is an easy way to convert between bytes and 32bit datatypes.
 * Generally, a union should be preferred over casts because it makes your intensions 
 * very clear to both the compiler and other people. 
 */
typedef union u32b
{
    int32 i;
    uint32 u;
    float f;
    uint8 bytes[4];
}
u32b;

/**
 * \brief u16b is a union for 16bit types.
 */
typedef union u16b
{
    int16 i;
    uint16 u;
    uint8 bytes[2];
}
u16b;

#ifndef is_inf
#define ETK_IS_INF
inline bool is_inf(float value)
{
    u32b ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) == 0x7f800000;
}
#endif

#ifndef is_nan
#define ETK_IS_NAN
inline bool is_nan( float value )
{
    u32b ieee754;
    ieee754.f = value;
    return (ieee754.u & 0x7fffffff) > 0x7f800000;
}
#endif


/**
 * \brief Used to keep a number within a particular range.
 * \arg x Number to constrain.
 * \arg a Minimum value
 * \arg b Maximum value
 * \return x, if x is between a and b. If x is smaller than a, then a. If x is larger than b, then b.
 */
#ifndef constrain
#define ETK_CONSTRAIN
template<typename T> T constrain(T x, T a, T b)
{
    if(x < a)
        return a;
    if(x > b)
        return b;
    return x;
}
#endif

/**
 * \brief Used to constrain a number to within a circular range.
 * For example,
 * @code
 cout << constrain_circular(450, 360);
 @endcode
 * Output: 90
 * Why? Because 450 degrees is the same as 90 degrees on a map.
 * This function can be used with radians by making segments 2*PI.
 */
inline real_t constrain_circular(real_t x, uint32 segments)
{
    if(is_inf(x) || is_nan(x))
        return x;

    uint32 half_segment = segments/2;
    int64 seg_lower = half_segment;
    seg_lower = -seg_lower;

    while(x < seg_lower)
        x += segments;
    while(x > half_segment)
        x -= segments;
    return x;
}

/**
 * \brief Returns the smaller of two values.
 */
#ifndef min
#define ETK_MIN
template<typename T> T min(T a, T b)
{
    if(a < b)
        return a;
    return b;
}
#endif

/**
 * \brief Returns the larger of two values.
 */
#ifndef max
#define ETK_MAX
template<typename T> T max(T a, T b)
{
    if(a > b)
        return a;
    return b;
}
#endif

/**
 * \brief Converts a value from one scale to another.
 * For example, a control functions returns a value between -1.0 and 1.0. The servo requires an input from 0 - 90.
 * ctrl_val = map(ctrl_val, -1.0, 1.0, 0.0, 90.0);
 *
 * If in_max is equal to in_min, a divide by zero will occur.
 *
 */
#ifndef map
#define ETK_MAP
template<typename T> T map(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
#endif
/**
 * \brief Returns a number with the value of x and the sign of y.
 */
inline real_t copysign(real_t x, real_t y)
{
    if((y >= 0) && (x < 0))
        return -x;
    if((y < 0) && (x >= 0))
        return -x;
    return x;
}


/**
 * \brief Same as copysign(), but if y is zero then zero will be returned regardless of x.
 */
inline real_t copysign_zero(real_t x, real_t y, real_t precision=0.000001)
{
    if((y < precision) && (y > -precision))
        return 0;
    return copysign(x, y);
}

/**
 * \brief Moves a to b and b to a.
 */
template<typename T> void swap(T& a, T& b)
{
    auto temp = a;
    a = b;
    b = temp;
}


/**
 * \brief Sorts an array using a simple bubble sort algorithm. The largest value will end up at the start of the array.
 */
template<typename T> void bubble_sort_up(T& items, uint32 n)
{
    while(n != 0)
    {
        uint32 newn = 0;
        for(uint32 i = 1; i < n; i++)
        {
            if(items[i-1] > items[i])
            {
                swap(items[i], items[i-1]);
                newn = i;
            }
        }
        n = newn;
    }
}

/**
 * \brief Sorts an array using a simple bubble sort algorithm. The largest value will move to the end of the array.
 */
template<typename T> void bubble_sort_down(T& items, uint32 n)
{
    while(n != 0)
    {
        uint32 newn = 0;
        for(uint32 i = 1; i < n; i++)
        {
            if(items[i-1] < items[i])
            {
                swap(items[i], items[i-1]);
                newn = i;
            }
        }
        n = newn;
    }
}

/**
 * Sorts an array using the quicksort algorithm.
 */
template<typename T> void quick_sort(T& arr, unsigned int low, unsigned int high, const bool sort_up)
{
    if(low < high)
    {
        unsigned int pi = 0;
        auto x = arr[low]; // pivot
        unsigned int i = low;
        unsigned int j = high;
        while(true)
        {
            if(sort_up)
            {
                while(arr[i] < x)
                    i++;

                while(arr[j] > x)
                    j--;
            }
            else
            {
                while(arr[i] > x)
                    i++;

                while(arr[j] < x)
                    j--;
            }

            if (i < j)
            {
                swap(arr[i], arr[j]);
                i++;
                j--;
            }
            else
            {
                pi = j;
                break;
            }
        }

        quick_sort(arr, low, pi, sort_up);
        quick_sort(arr, pi + 1, high, sort_up);
    }
}

/**
 * Sorts an array so the smallest value comes first.
 */
template<typename T> void quick_sort_up(T& arr)
{
    quick_sort(arr, 0, arr.size()-1, true);
}

/**
 * Sorts an array so the largest value comes first.
 */
template<typename T> void quick_sort_down(T& arr)
{
    quick_sort(arr, 0, arr.size()-1, false);
}


/**
 * \brief Converts a character to its uppercase ASCII equivalent.
 */
inline char to_upper(char c)
{
    if((c >= 'a') && (c <= 'z'))
    {
        return (c-'a')+'A';
    }
    return c;
}

/**
 * \brief Converts a character to its lowercase ASCII equivalent.
 */
inline char to_lower(char c)
{
    if((c >= 'A') && (c <= 'Z'))
    {
        return (c-'A')+'a';
    }
    return c;
}


/**
 * \brief Returns true if the character is alphabetic, upper or lower case.
 */
inline bool is_alpha(char c)
{
    if((c >= 'A') && (c <= 'Z'))
        return true;
    if((c >= 'a') && (c <= 'z'))
        return true;
    return false;
}

/**
 * \brief Returns true if the character is a digit.
 */
inline bool is_numeric(char c)
{
    if((c >= '0') && (c <= '9'))
        return true;
    return false;
}

/**
 * \brief Returns the input parameter with a positive sign.
 * If t is a signed integer, it must be constrained to a plausible range
 * in order to prevent undefined behaviour if t == INT_MIN
 */
#ifndef abs
#define ETK_ABS
template<typename T> T abs(T t)
{
    if(t < 0)
        return -t;
    return t;
}
#endif

#ifndef fabs
#define ETK_FABS
inline real_t fabs(real_t t)
{
	return abs(t);
}
#endif


/**
 * \brief Compares two values. Returns true if their difference is less than precision.
 */
inline bool compare(real_t a, real_t b, real_t precision)
{
    auto result = abs(a-b);
    return ((result-precision) < 0);
}

/**
 * \brief Assigns a value to the elements of an array.
 */
template<typename T, typename U> void set_array(T& ar, U item, uint32 len)
{
    for(uint32 i = 0; i < len; i++)
        ar[i] = item;
}

/**
 * \brief Writes zeros to an object.
 */
template<typename T> void zero_object(T& obj)
{
    void* vptr = static_cast<void*>(&obj);
    uint8* ptr = static_cast<uint8*>(vptr);

    for(uint32 i = 0; i < sizeof(obj); i++)
        ptr[i] = 0;
}

/**
 * \brief Reverses the first n_elements of an array or list.
 */
template<typename T> void reverse(T& list, uint32 n_elements)
{
    for(uint32 i = 0; i < n_elements/2; i++)
        swap(list[n_elements-i-1], list[i]);
}


/**
 * \brief Little trick function used to silence warnings about unused variables that are
 * , for whatever reason, required to be unused.
 */
template<typename T> void unused(T& expr)
{
    (void)(expr);
}

}



#endif // MATH_UTIL_H_INCLUDED
