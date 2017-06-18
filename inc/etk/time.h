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

#ifndef ETK_TIME_H_INCLUDED
#define ETK_TIME_H_INCLUDED

#include "types.h"
#include "rope.h"

namespace etk
{

/**
 * \class Time
 *
 * \brief Time is a class that can be used to perform time related functions.
 */

class Time
{
    friend void tick();
public:
    Time() 
    {
        setnull();
    }
    
    Time(Time& d) 
    {
        sec = d.sec;
        mic = d.mic;
    }
    
    Time(volatile Time& d) 
    {
        sec = d.sec;
        mic = d.mic;
    }
    
    Time(const Time& d) 
    {
        sec = d.sec;
        mic = d.mic;
    }

    Time& operator=(const Time& d) volatile
    {
		sec = d.sec;
		mic = d.mic;
		return (Time&)*this;
	}

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
    real_t diff_time(const Time& then)
    {
		real_t ret;
		ret = (((real_t)mic - (real_t)then.micros())/1000000.0f);
		ret += ((real_t)sec - (real_t)then.seconds());
		return ret;
	}
	
    real_t diff_time(Time then) volatile
    {
		real_t ret;
		ret = (((real_t)mic - (real_t)then.micros())/1000000.0f);
		ret += ((real_t)sec - (real_t)then.seconds());
		return ret;
	}

    /**
     * \brief Same as Time::diff_time but the return value is in milliseconds rather than seconds.
     */
    real_t diff_time_ms(const Time& then)
    {
        return diff_time(then)*1000.0f;
    }
    
    real_t diff_time_ms(Time then) volatile 
    {
        return diff_time(then)*1000.0f;
    }

    /**
     * \brief Sets the time instance to zero.
     */
    void setnull()
    {
		mic = sec = 0;
	}
	
    void setnull() volatile
    {
		mic = sec = 0;
	}

    /**
     * \brief Returns true is the time is zero.
     */
    bool is_nulltime()
    {
		if((mic == 0) && (sec == 0))
			return true;
		return false;
	}

    bool is_nulltime() volatile
    {
		if((mic == 0) && (sec == 0))
			return true;
		return false;
	}

    /**
     * \brief Returns a reference to the seconds counter.
     */
    uint32& seconds() 
    {
        return sec;
    }

    /**
     * \brief Returns a reference to the microsecond counter.
     */
    uint32& micros() 
    {
        return mic;
    }
    
    uint32 seconds() const
    {
    	return sec;
    }
    
    uint32 micros() const
    {
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
    void to_rope(Rope& r)
    {
		r.clear();
		int32 days = sec / 60 / 60 / 24;
		int32 hours = (sec / 60 / 60) % 24;
		int32 minutes = (sec / 60) % 60;
		int32 seconds = sec % 60;
		r << days << " days, " << hours << " hours, " << minutes << " mins, " << seconds << " seconds";
	}


private:
    uint32 sec;
    uint32 mic;
};

}



#endif

