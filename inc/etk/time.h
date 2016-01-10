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

#ifndef ETK_TIME_H_INCLUDED
#define ETK_TIME_H_INCLUDED

#include "types.h"
#include <etk/rope.h>

namespace etk
{

/**
 * \class Time
 *
 * \brief Time is a class that can be used to perform time related functions such as counting systicks.
 *
 * http://www.camelsoftware.com/2015/12/24/etk-time/
 *
 * A simple 32bit systick counter with microsecond precision will roll over in a matter of hours.
 * The Time class counts both seconds and microseconds. This multiplies the roll over time by a million, which means it won't roll over for 120 years.
 * Some say systick roll over is easy to work around, but some haven't had to debug a fault that only occurs once per 12 hours :-).
 *
 * In addition to counting systicks, the Time class provides a number of other convenient functions such as diff_time();, sleep_ms(); and to_rope();
 */

class Time
{
friend void tick();
public:
    Time() {
        setnull();
    }
    Time(Time& d) {
        sec = d.sec;
        mic = d.mic;
    }
    Time(volatile Time& d) {
        sec = d.sec;
        mic = d.mic;
    }
    Time(const Time& d) {
        sec = d.sec;
        mic = d.mic;
    }

    Time& operator=(const Time& d) volatile;

	/**
	 * \brief Calculates the difference between two Times in seconds.
	 *@code
	 Time then = Time::now();
	 Time::sleep_ms(500);
	 real_t diff = Time::now().diff_time(then);
	 //diff = 0.5
	 @endcode
	 * @args then Time to compare to.
	 * @return Difference in times in seconds.
	 */
    real_t diff_time(Time then);
    real_t diff_time(Time then) volatile;

	/**
	 * \brief Same as Time::diff_time but the return value is in milliseconds rather than seconds.
	 */
    real_t diff_time_ms(Time then) {
        return diff_time(then)*1000.0f;
    }
    real_t diff_time_ms(Time then) volatile {
        return diff_time(then)*1000.0f;
    }

	/**
	 * \brief Sets the time instance to zero.
	 */
    void setnull();
    void setnull() volatile;

	/**
	 * \brief Returns true is the time is zero.
	 */
    bool is_nulltime();
    bool is_nulltime() volatile;

	/**
	 * \brief Returns a reference to the seconds counter.
	 */
    uint32& seconds() {
        return sec;
    }

    /**
     * \brief Returns a reference to the microsecond counter.
     */
    uint32& micros() {
        return mic;
    }

    volatile uint32& seconds() volatile {
        return sec;
    }
    volatile uint32& micros() volatile {
        return mic;
    }

	/**
	 * \brief Converts the time to days, hours, minutes and seconds and turns it into a string using a Rope. The original contents of the Rope will be erased.
	 * @code
	 char buf[60];
	 etk::Rope rope(buf, 60);
	 etk::Time::now().to_rope(rope);
	 //buf will now contain something like '3 days, 2 hours, 45 minutes, 12 seconds'
	 @endcode
	 */
    void to_rope(Rope& r);


private:
    uint32 sec;
    uint32 mic;
};

/**
 * \brief Returns the current time.
 */
Time now();

/**
	 * \brief Sleeps for a number of milliseconds.
	 * @arg ms Number of milliseconds to wait.
	 */
void sleep_ms(uint32 ms);

    /**
     * \brief Sleeps for a number of microseconds.
     * @args us Number of microseconds to wait.
     */
void sleep_us(uint32 us);

    /**
     * \brief Increments the systick counter by the tick rate (set_tick_rate(); )
     */
void tick();

    /**
     * \brief Sets the tick rate in microseconds per tick. eg for a 1ms tick rate, use set_tick_rate(1000);
     * @arg us Microseconds per tick.
     */
void set_tick_rate(uint32 us); //microseconds per tick

}



#endif


