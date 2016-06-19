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

#ifndef ETK_LOOP_RANGE_H
#define ETK_LOOP_RANGE_H

//#include <type_traits>

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
    return {0, to };
}


inline bool static_range(uint32 max)
{
	static uint32 count = 0;
	count++;
	if(count > max)
	{
		count = 0;
		return false;
	}
	return true;
}


};


#endif

