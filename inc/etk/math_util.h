
#ifndef MATH_UTIL_H_INCLUDED
#define MATH_UTIL_H_INCLUDED

#include <stdint.h>

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
auto constrain_circular(auto x, uint32_t segments)
{
    uint32_t half_segment = segments/2;
    int64_t seg_lower = half_segment;
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
template<typename T> void bubble_sort_up(T& items, uint32_t n)
{
    while(n != 0)
    {
        uint32_t newn = 0;
        for(uint32_t i = 1; i < n; i++)
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
template<typename T> void bubble_sort_down(T& items, uint32_t n)
{
    while(n != 0)
    {
        uint32_t newn = 0;
        for(uint32_t i = 1; i < n; i++)
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

/**
 * \brief Returns the input parameter with a positive sign.
 */
template<typename T> T abs(T t)
{
    if(t < 0)
        return -t;
    return t;
}

}



#endif // MATH_UTIL_H_INCLUDED
