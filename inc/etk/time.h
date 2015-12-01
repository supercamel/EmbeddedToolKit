
#ifndef ETK_TIME_H_INCLUDED
#define ETK_TIME_H_INCLUDED

#include <stdint.h>
#include <etk/rope.h>

namespace etk
{

class Time
{
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

    static Time now();
    static void tick();
    static void set_tick_rate(uint32_t us); //microseconds per tick


    static void sleep_ms(uint32_t ms);
    static void sleep_us(uint32_t us);

    float diff_time(Time then);
    float diff_time(Time then) volatile;

    float diff_time_ms(Time then) {
        return diff_time(then)*1000.0f;
    }
    float diff_time_ms(Time then) volatile {
        return diff_time(then)*1000.0f;
    }

    void setnull();
    void setnull() volatile;

    bool is_nulltime();
    bool is_nulltime() volatile;

    uint32_t& seconds() {
        return sec;
    }
    uint32_t& micros() {
        return mic;
    }

    volatile uint32_t& seconds() volatile {
        return sec;
    }
    volatile uint32_t& micros() volatile {
        return mic;
    }

    void to_rope(Rope& r);


private:
    uint32_t sec;
    uint32_t mic;
};



}



#endif


