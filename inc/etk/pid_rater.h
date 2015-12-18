#ifndef ETK_PID_RATER_H_INCLUDED
#define ETK_PID_RATER_H_INCLUDED


#include <stdint.h>
#include <cmath>
#include "math_util.h"
#include "stm.h"


namespace etk
{

class PIDRater
{
public:
    virtual bool rate(float setpoint, float measurement) = 0;
    virtual float get_score() = 0;
};

class BasicPIDRater : public PIDRater
{
public:
    bool rate(float setpoint, float measurement)
    {
        float err = abs(setpoint-measurement);
        stm.put(err);
        total_error += err;
        setpoint_delta += abs(setpoint - last_setpoint);
        n_samples++;
        last_setpoint = setpoint;

        float stm_err = 0.0f;
        for(auto i : stm)
            stm_err += i;
        stm_err /= (float)(MEMORY_LENGTH);

        if(stm_err > max_error)
        {
            avg_error = INFINITY;
            n_samples = total_error = setpoint_delta = 0;
            stm.fill(0.0f);
            return true;
        }

        if((n_samples > min_samples) && (setpoint_delta > min_setpoint_delta))
        {
            avg_error = total_error/float(n_samples);
            n_samples = total_error = setpoint_delta = 0;
            stm.fill(0.0f);
            return true;
        }

        return false;
    }

    void set_minimum_samples(uint32_t ms)
    {
        min_samples = ms;
    }

    void set_min_setpoint_delta(float mn)
    {
        min_setpoint_delta = mn;
    }

    float get_score()
    {
        return avg_error;
    }

    void set_max_error(float me)
    {
        max_error = me;
    }

private:
    uint32_t min_samples = 1000;
    float min_setpoint_delta = 100.0f;

    float total_error = 0.0f;
    float setpoint_delta = 0.0f;
    uint32_t n_samples = 0;

    float avg_error = 0.0f;

    float max_error = 10000.0f;

    float last_setpoint;

    static const uint32_t MEMORY_LENGTH = 10;

    ShortTermMemory<float, MEMORY_LENGTH> stm;
};

}

#endif // ETK_PID_RATER_H_INCLUDED
