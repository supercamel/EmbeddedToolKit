
#ifndef ETK_LOOP_RANGE_H
#define ETK_LOOP_RANGE_H

#include <type_traits>

/*

	http://www.camelsoftware.com/blog/2015/12/02/improving-the-for-loop-with-c11/
	
	
	brilliant loop range classes from here
	http://codereview.stackexchange.com/questions/51523/simple-integer-range-for-c11-range-based-for-loops
	
*/

namespace etk
{

template <typename T> class LoopRangeIterator
{
public:
    LoopRangeIterator(T value_)
        : value(value_) {}

    bool operator!=(LoopRangeIterator const& other) const
    {
        return value != other.value;
    }

    T const& operator*() const
    {
        return value;
    }

    LoopRangeIterator& operator++()
    {
        ++value;
        return *this;
    }

private:
    T value;
};

template <typename T> class LoopRange
{
public:
    LoopRange(T from_, T to_)
        : from(from_), to(to_) {}

    LoopRange(T to_)
        : from(0), to(to_) {}

    LoopRangeIterator<T> begin() const
    {
        return LoopRangeIterator<T>(from);
    }

    LoopRangeIterator<T> end() const
    {
        return LoopRangeIterator<T>(to);
    }

private:
    T const from;
    T const to;
};

template<typename T>
LoopRange<T> range(T from, T to)
{
    static_assert(std::is_integral<T>::value,
                  "range only accepts integral values");

    return { from, to };
}

template<typename T>
LoopRange<T> range(T to)
{
    static_assert(std::is_integral<T>::value,
                  "range only accepts integral values");

    return {0, to };
}


};


#endif

