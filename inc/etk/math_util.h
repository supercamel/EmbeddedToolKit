
#ifndef MATH_UTIL_H_INCLUDED
#define MATH_UTIL_H_INCLUDED

#include <stdint.h>

namespace etk
{

template<typename T> T constrain(T x, T a, T b)
{
    if(x < a)
        return a;
    if(x > b)
        return b;
    return x;
}


/*
	To keep within +/- 180.0 degrees, set segments to 360.0
*/
template<typename T> T constrain_circular(T x, uint32_t segments)
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

template<typename T> T min(T a, T b)
{
    if(a < b)
        return a;
    return b;
}


template<typename T> T max(T a, T b)
{
    if(a > b)
        return a;
    return b;
}

template<typename T> T map(T x, T in_min, T in_max, T out_min, T out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template<typename T> T copysign(T x, T y)
{
    if((y >= 0) && (x < 0))
        return -x;
    if((y < 0) && (x >= 0))
        return -x;
    return x;
}

template<typename T> T copysign_zero(T x, T y)
{
    if(y == 0)
        return 0;
    return copysign(x, y);
}


template<typename T> void swap(T& a, T& b)
{
    T temp = a;
    a = b;
    b = temp;
}

template<typename T> bool compare(T a, T b, T precision)
{
    T result = a-b;
    if(result < 0)
        result = -result;
    return ((result-precision) < 0);
}

template<typename T> void bubble_sort_up(T* items, uint32_t n)
{
    while(n != 0)
    {
        uint32_t newn = 0;
        for(uint32_t i = 1; i < n; i++)
        {
            if(items[i-1] > items[i])
            {
                swap<T>(items[i], items[i-1]);
                newn = i;
            }
        }
        n = newn;
    }
}

template<typename T> void bubble_sort_down(T* items, uint32_t n)
{
    while(n != 0)
    {
        uint32_t newn = 0;
        for(uint32_t i = 1; i < n; i++)
        {
            if(items[i-1] < items[i])
            {
                swap<T>(items[i], items[i-1]);
                newn = i;
            }
        }
        n = newn;
    }
}

inline char to_upper(char c)
{
    if((c >= 'a') && (c <= 'z'))
    {
        return (c-'a')+'A';
    }
    return c;
}

inline char to_lower(char c)
{
    if((c >= 'A') && (c <= 'Z'))
    {
        return (c-'A')+'a';
    }
    return c;
}

template<typename T> T abs(T t)
{
    if(t < 0)
        return -t;
    return t;
}

}



#endif // MATH_UTIL_H_INCLUDED
