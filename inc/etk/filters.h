
#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "etk/etk.h"

namespace etk
{

class ExpoMovingAvg
{
public:
    ExpoMovingAvg()
    {
        a = 0.5f;
        accumulator = 0;
    }
    ExpoMovingAvg(float f, float init_est = 0)
    {
        a = f;
        accumulator = init_est;
    }

    void set_factor(float factor)
    {
        a = factor;
    }
    void step(float measurement)
    {
        accumulator = (a* measurement) + (1.0f - a) * accumulator;
    }

    float get()
    {
        return accumulator;
    }

private:
    float accumulator;
    float a;
};

class CircularExpoMovingAvg
{
public:
    CircularExpoMovingAvg()
    {
        a = 0.5;
        accumulator = 0;
    }
    CircularExpoMovingAvg(float f, float init_est = 0)
    {
        a = f;
        accumulator = init_est;
    }

    void set_factor(float factor)
    {
        a = factor;
    }
    void step(float measurement)
    {
        while(measurement > 18000)
            measurement -= 36000;
        while(measurement < -18000)
            measurement += 36000;

        accumulator = (a* measurement) + (1.0f - a) * accumulator;

        while(accumulator > 18000)
            accumulator -= 36000;
        while(accumulator < -18000)
            accumulator += 36000;
    }

    float get()
    {
        return accumulator;
    }

private:
    float accumulator;
    float a;
};


class BrownLinearExpo
{
public:
    BrownLinearExpo()
    {
        a = 0.5;
        estimate = 0;
        double_smoothed = 0;
        single_smoothed = 0;
    }
    BrownLinearExpo(float f, float init_est)
    {
        a = f;
        estimate = init_est;
        double_smoothed = 0;
        single_smoothed = 0;
    }

    void set_factor(float factor)
    {
        a = factor;
    }
    void step(float measurement)
    {
        single_smoothed = a * measurement + (1 - a) * single_smoothed;
        double_smoothed = a * single_smoothed + (1 - a) * double_smoothed;

        float est_a = (2*single_smoothed - double_smoothed);
        float est_b = (a / (1-a) )*(single_smoothed - double_smoothed);
        estimate = est_a + est_b;
    }
    float get()
    {
        return estimate;
    }

private:
    float estimate, double_smoothed, single_smoothed;
    float a;
};



class scalarLinearKalman
{
public:
    scalarLinearKalman(float control_gain, float initial_state_estimate, float initial_covariance, float control_noise, float measurement_noise)
    {
        B = control_gain;
        current_state_estimate = initial_state_estimate;
        current_prob_estimate = initial_covariance;
        Q = control_noise;
        R = measurement_noise;
    }

    float get_state()
    {
        return current_state_estimate;
    }


    void step(float control_vector, float measurement_vector)
    {
        //prediction
        float predicted_state_estimate = (current_state_estimate) + (B * control_vector);
        float predicted_prob_estimate = current_prob_estimate + Q;

        //observation
        float innovation = measurement_vector - predicted_state_estimate;
        float innovation_covariance = predicted_prob_estimate + R;

        //update
        float kalman_gain = predicted_prob_estimate * innovation_covariance;
        current_state_estimate = predicted_state_estimate + kalman_gain * innovation;
        current_prob_estimate = (1 - kalman_gain) * predicted_prob_estimate;
    }

    float B, current_state_estimate, current_prob_estimate;
    float Q, R;
};


/*
This high pass filter algorithm uses a low-pass filter to remove the long term average from the input signal.
*/
class HighPassFilter
{
public:
    HighPassFilter(float gain) : emv(gain)
    {
        estimate = 0;
    }

    void step(float sample)
    {
        emv.step(sample);

        estimate = sample - emv.get();
    }

    float get()
    {
        return estimate;
    }

private:
    ExpoMovingAvg emv;
    float estimate;
};


class RateLimiter
{
public:
    RateLimiter(float max_step, float init_val)
    {
        ms = max_step;
        last_sample = init_val;
    }

    float step(float sample)
    {
        float delta = sample - last_sample;
        delta = constrain(delta, -ms, ms);
        last_sample += delta;
        return last_sample;
    }

private:
    float ms;
    float last_sample;
};


}

#endif
