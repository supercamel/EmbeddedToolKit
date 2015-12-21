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
        
        total_error += err;
        
        setpoint_delta += abs(setpoint - last_setpoint);
        n_samples++;
        last_setpoint = setpoint;
        
        stm.put(err);
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
