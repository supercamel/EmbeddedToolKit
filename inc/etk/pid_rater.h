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


#include "types.h"
#include <math.h>
#include "math_util.h"
#include "stm.h"


namespace etk
{

class PIDRater
{
public:
    virtual bool rate(real_t setpoint, real_t measurement) = 0;
    virtual real_t get_score() = 0;
};

class BasicPIDRater : public PIDRater
{
public:
    bool rate(real_t setpoint, real_t measurement)
    {
        real_t err = abs(setpoint-measurement);

        total_error += err;

        setpoint_delta += abs(setpoint - last_setpoint);
        n_samples++;
        last_setpoint = setpoint;

        stm.put(err);
        real_t stm_err = 0.0f;
        for(auto i : stm)
            stm_err += i;
        stm_err /= (real_t)(MEMORY_LENGTH);
        if(stm_err > max_error)
        {
            avg_error = INFINITY;
            n_samples = total_error = setpoint_delta = 0;
            stm.fill(0.0f);
            return true;
        }

        if((n_samples > min_samples) && (setpoint_delta > min_setpoint_delta))
        {
            avg_error = total_error/real_t(n_samples);
            n_samples = total_error = setpoint_delta = 0;
            stm.fill(0.0f);
            return true;
        }

        return false;
    }

    void set_minimum_samples(uint32 ms)
    {
        min_samples = ms;
    }

    void set_min_setpoint_delta(real_t mn)
    {
        min_setpoint_delta = mn;
    }

    real_t get_score()
    {
        return avg_error;
    }

    void set_max_error(real_t me)
    {
        max_error = me;
    }

private:
    uint32 min_samples = 1000;
    real_t min_setpoint_delta = 100.0f;

    real_t total_error = 0.0f;
    real_t setpoint_delta = 0.0f;
    uint32 n_samples = 0;

    real_t avg_error = 0.0f;

    real_t max_error = 10000.0f;

    real_t last_setpoint;

    static const uint32 MEMORY_LENGTH = 10;

    ShortTermMemory<real_t, MEMORY_LENGTH> stm;
};

}

#endif // ETK_PID_RATER_H_INCLUDED
