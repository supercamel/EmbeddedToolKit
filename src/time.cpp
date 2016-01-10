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

#include <etk/time.h>

namespace etk
{

static volatile Time _now;
static uint32 us_tick_rate = 1000;

void set_tick_rate(uint32 us)
{
	us_tick_rate = us;
}


Time& Time::operator=(const Time& d) volatile
{
    sec = d.sec;
    mic = d.mic;
    return (Time&)*this;
}

real_t Time::diff_time(Time then)
{
	real_t ret;
	ret = (((real_t)mic - (real_t)then.micros())/1000000.0f);
	ret += ((real_t)sec - (real_t)then.seconds());
	return ret;
}


void Time::setnull()
{
	mic = sec = 0;
}

bool Time::is_nulltime()
{
	if((mic == 0) && (sec == 0))
		return true;
	return false;
}

void Time::setnull() volatile
{
	mic = sec = 0;
}

bool Time::is_nulltime() volatile
{
	if((mic == 0) && (sec == 0))
		return true;
	return false;
}


void tick()
{
	_now.mic += us_tick_rate;

	if(_now.mic >= 1000000)
	{
		_now.sec++;
		_now.mic = 0;
	}
}

void __attribute__((weak)) sleep_ms(uint32 ms)
{
    Time start = now();
	real_t sms = ms/1000.0f;

    while(now().diff_time(start) < sms)
    { }
}

void __attribute__((weak)) sleep_us(uint32 us)
{
    Time start = now();
	real_t sus = us/1000000.0f;

    while(now().diff_time(start) < sus)
    { }
}

Time __attribute__((weak)) now()
{
	return _now;
}


void Time::to_rope(Rope& r)
{
	r.clear();
    int32 days = sec / 60 / 60 / 24;
    int32 hours = (sec / 60 / 60) % 24;
    int32 minutes = (sec / 60) % 60;
    int32 seconds = sec % 60;
	r << days << " days, " << hours << " hours, " << minutes << " mins, " << seconds << " seconds";
}


}


