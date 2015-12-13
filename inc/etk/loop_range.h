
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

/**
 * \class LoopRangeIterator
 * \brief See LoopRange
 */
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

/**
 * \class LoopRange
 * \brief This class is of limited usefullness by itself. It should be used with the range() template functions.
  * @code
 for(auto i : range(4))
     cout << i << " ";
 * Output: 0 1 2 3
 @endcode
 * @code
 for(auto i : range(5, 9))
 	cout << i << " ";
 @endcode
 * Output: 5, 6, 7, 8
 */
 
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

/**
 * \brief Like range(T to), except the start value can be specified.
 * @code
 for(auto i : range(5, 9))
 	cout << i << " ";
 @endcode
 * Output: 5, 6, 7, 8
 */
template<typename T>
LoopRange<T> range(T from, T to)
{
    static_assert(std::is_integral<T>::value,
                  "range only accepts integral values");

    return { from, to };
}


/**
 * \brief The range function constructs and returns a loop range iterator. 
 * It's purpose is to exploit C++11 ranged look syntax and provide a cleaner and less verbose way to iterate over a range of numbers.
 * @code
 for(auto i : range(4))
     cout << i << " ";
 @endcode
 * Output: 0 1 2 3
 */
template<typename T>
LoopRange<T> range(T to)
{
    static_assert(std::is_integral<T>::value,
                  "range only accepts integral values");

    return {0, to };
}


};


#endif

