
#include <etk/time.h>

namespace etk
{

static volatile Time _now;
static uint32_t us_tick_rate = 1000;

Time Time::now()
{
	return _now;
}

void Time::set_tick_rate(uint32_t us)
{
	us_tick_rate = us;
}


Time& Time::operator=(const Time& d) volatile
{
    sec = d.sec;
    mic = d.mic;
    return (Time&)*this;
}

float Time::diff_time(Time then)
{
	float ret;
	ret = (((float)mic - (float)then.micros())/1000000.0f);
	ret += ((float)sec - (float)then.seconds());
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


void Time::tick()
{
	_now.mic += us_tick_rate;

	if(_now.mic >= 1000000)
	{
		_now.sec++;
		_now.mic = 0;
	}
}

void Time::sleep_ms(uint32_t ms)
{
    Time start = now();
	float sms = ms/1000.0f;

    while(now().diff_time(start) < sms)
    { }
}

void Time::sleep_us(uint32_t us)
{
    Time start = now();
	float sus = us/1000000.0f;

    while(now().diff_time(start) < sus)
    { }
}

void Time::to_rope(Rope& r)
{
	r.clear();
    int32_t days = sec / 60 / 60 / 24;
    int32_t hours = (sec / 60 / 60) % 24;
    int32_t minutes = (sec / 60) % 60;
    int32_t seconds = sec % 60;
	r << days << "days, " << hours << " hours, " << minutes << " mins, " << seconds << " seconds";
}


}


