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


#ifndef MATH_UTIL_H_INCLUDED
#define MATH_UTIL_H_INCLUDED

#include "types.h"

namespace etk
{

/**
 * \brief Used to keep a number within a particular range.
 * \arg x Number to constrain.
 * \arg a Minimum value
 * \arg b Maximum value
 * \return x, if x is between a and b. If x is smaller than a, then a. If x is larger than b, then b.
 */
auto constrain(auto x, auto a, auto b)
{
    if(x < a)
        return a;
    if(x > b)
        return b;
    return x;
}

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
auto constrain_circular(auto x, uint32 segments)
{
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
auto min(auto a, auto b)
{
    if(a < b)
        return a;
    return b;
}

/**
 * \brief Returns the larger of two values.
 */
auto max(auto a, auto b)
{
    if(a > b)
        return a;
    return b;
}

/**
 * \brief Converts a value from one scale to another.
 * For example, a control functions returns a value between -1.0 and 1.0. The servo requires an input from 0 - 90.
 * ctrl_val = map(ctrl_val, -1.0, 1.0, 0.0, 90.0);
 *
 */
auto map(auto x, auto in_min, auto in_max, auto out_min, auto out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/**
 * \brief Returns a number with the value of x and the sign of y.
 */
auto copysign(auto x, auto y)
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
auto copysign_zero(auto x, auto y)
{
    if(y == 0)
        return 0;
    return copysign(x, y);
}

/**
 * \brief Moves a to b and b to a.
 */
void swap(auto& a, auto& b)
{
    auto temp = a;
    a = b;
    b = temp;
}

/**
 * \brief Compares two values. Returns true if their difference is less than precision.
 */
bool compare(auto a, auto b, auto precision)
{
    auto result = a-b;
    if(result < 0)
        result = -result;
    return ((result-precision) < 0);
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

inline bool is_alpha(char c)
{
    if((c >= 'A') && (c <= 'Z'))
        return true;
    if((c >= 'a') && (c <= 'z'))
        return true;
    return false;
}

inline bool is_numeric(char c)
{
    if((c >= '0') && (c <= '9'))
        return true;
    return false;
}

/**
 * \brief Returns the input parameter with a positive sign.
 */
template<typename T> T abs(T t)
{
    if(t < 0)
        return -t;
    return t;
}


void set_array(auto& ar, auto item, uint32 len)
{
    for(uint32 i = 0; i < len; i++)
        ar[i] = item;
}

void zero_object(auto& obj)
{
    void* vptr = static_cast<void*>(&obj);
    uint8* ptr = static_cast<uint8*>(vptr);

    for(uint32 i = 0; i < sizeof(obj); i++)
        ptr[i] = 0;
}

void reverse(auto& list, uint32 n_elements)
{
	for(uint32 i = 0; i < n_elements/2; i++)
		swap(list[n_elements-i-1], list[i]);
}

/**
 * \brief Little trick function used to silence warnings about unused variables that are
 * , for whatever reason, required to be unused.
 */
inline void unused(auto& expr)
{
    (void)(expr);
}

}



#endif // MATH_UTIL_H_INCLUDED
