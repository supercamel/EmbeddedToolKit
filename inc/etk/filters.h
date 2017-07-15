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


#ifndef ETK_FILTERS_H_INCLUDED
#define ETK_FILTERS_H_INCLUDED

#include "math_util.h"

namespace etk
{

/**
 * \class LowPassFilter
 *
 * \brief An exponential moving average low-pass filter.
 *
 * @code
    etk::LowPassFilter filter(0.1);

    for(auto i : etk::range(100))
    {
    	filter.step(i);
    	cout << filter.get() << endl;
    }

    @endcode
 *
 */

class LowPassFilter
{
public:
    LowPassFilter()
    {
        a = 0.5f;
        accumulator = 0;
    }

    /**
     * \brief This constructor allows you to set the filter gain and initial estimate.
     *
     * @arg f filter gain
     * @arg init_est initial estimate
     */
    LowPassFilter(real_t f, real_t init_est = 0)
    {
        a = f;
        accumulator = init_est;
    }

    /**
     * \brief The gain controls the responsiveness of the filter.
     * It should be a value between 0.0 and 1.0. A higher value makes the filter more responsive and allows more noise through.
     *
     * @arg factor The filter gain (0.0 - 1.0)
     */
    void set_gain(real_t factor)
    {
        a = factor;
    }

    /**
     * \brief Performs a single iteration of the filter.
     * Raw samples are entered into the filter using this function.
     *
     * @arg measurement A raw measurement or sample to be filtered.
     */
    void step(real_t measurement)
    {
        accumulator = (a* measurement) + (1.0f - a) * accumulator;
    }

    /**
     * \brief Returns the current state of the filter.
     */
    real_t get()
    {
        return accumulator;
    }

private:
    real_t accumulator;
    real_t a;
};


/**
 * \class LinearExpoFilter
 *
 * \brief Browns linear exponential filter is a form of double exponential smoothing. It can be more responsive than the MovingExpoAvg filter, but is prone to overshoot.
 */

class LinearExpoFilter
{
public:
    LinearExpoFilter()
    {
        a = 0.5;
        estimate = 0;
        double_smoothed = 0;
        single_smoothed = 0;
    }

    /**
     * \brief This constructor allows you to set the filter gain and initial estimate.
     *
     * @arg f filter gain
     * @arg init_est initial estimate
     */
    LinearExpoFilter(real_t f, real_t init_est)
    {
        a = f;
        estimate = init_est;
        double_smoothed = 0;
        single_smoothed = 0;
    }

    /**
     * \brief The gain controls the responsiveness of the filter.
     * It should be a value between 0.0 and 1.0. A higher value makes the filter more responsive and allows more noise through.
     *
     * @arg factor The filter gain (0.0 - 1.0)
     */
    void set_gain(real_t factor)
    {
        a = factor;
    }

    /**
     * \brief Performs a single iteration of the filter.
     * Raw samples are entered into the filter using this function.
     *
     * @arg measurement A raw measurement or sample to be filtered.
     */
    void step(real_t measurement)
    {
        single_smoothed = a * measurement + (1 - a) * single_smoothed;
        double_smoothed = a * single_smoothed + (1 - a) * double_smoothed;

        real_t est_a = (2*single_smoothed - double_smoothed);
        real_t est_b = (a / (1-a) )*(single_smoothed - double_smoothed);
        estimate = est_a + est_b;
    }

    /**
     * \brief Returns the current state of the filter.
     */
    real_t get()
    {
        return estimate;
    }

private:
    real_t estimate, double_smoothed, single_smoothed;
    real_t a;
};


/**
 * \class ScalarLinearKalman
 *
 * \brief A linear kalman filter for scalars.
 */
class ScalarLinearKalman
{
public:
    ScalarLinearKalman(real_t control_gain, real_t initial_state_estimate, real_t initial_covariance, real_t control_noise, real_t measurement_noise)
    {
        B = control_gain;
        current_state_estimate = initial_state_estimate;
        current_prob_estimate = initial_covariance;
        Q = control_noise;
        R = measurement_noise;
    }

    real_t get_state()
    {
        return current_state_estimate;
    }


    void step(real_t control_vector, real_t measurement_vector)
    {
        //prediction
        real_t predicted_state_estimate = (current_state_estimate) + (B * control_vector);
        real_t predicted_prob_estimate = current_prob_estimate + Q;

        //observation
        real_t innovation = measurement_vector - predicted_state_estimate;
        real_t innovation_covariance = predicted_prob_estimate + R;

        //update
        real_t kalman_gain = predicted_prob_estimate * innovation_covariance;
        current_state_estimate = predicted_state_estimate + kalman_gain * innovation;
        current_prob_estimate = (1 - kalman_gain) * predicted_prob_estimate;
    }

private:
    real_t B, current_state_estimate, current_prob_estimate;
    real_t Q, R;
};


/**
 * \class HighPassFilter
 *
 * \brief The HighPassFilter blocks long term averages and allows higher frequencies through.
 */
class HighPassFilter
{
public:

    /**
     * \brief The gain is set by the constructor. The gain must be between 0.0 and 1.0. The higher the gain, the higher the cutoff frequency.
     */

    HighPassFilter(real_t gain) : emv(gain)
    {
        estimate = 0;
    }

    /**
    * \brief Performs a single iteration of the filter.
    * Raw samples are entered into the filter using this function.
    *
    * @arg measurement A raw measurement or sample to be filtered.
    */
    void step(real_t sample)
    {
        emv.step(sample);

        estimate = sample - emv.get();
    }

    /**
     * \brief Returns the current state of the filter.
     */
    real_t get()
    {
        return estimate;
    }

private:
    LowPassFilter emv;
    real_t estimate;
};


/**
 * \class RateLimiter
 *
 * \brief The RateLimiter limits the rate of change of a signal to a given step size.
 *
 * @code
 etk::RateLimiter lim(1.0, 0.0);
 for(auto i : etk::range(10))
 	cout << lim.step(10) << " ";
 	@endcode
 * Output: 0,1,2,3,4,5,6,7,8,9
 */

class RateLimiter
{
public:
    RateLimiter()
    {
        last_sample = 0;
        ms = 1;
    }

    RateLimiter(real_t max_step, real_t init_val)
    {
        ms = max_step;
        last_sample = init_val;
    }

    real_t step(real_t sample)
    {
        real_t delta = sample - last_sample;
        delta = constrain(delta, -ms, ms);
        last_sample += delta;
        return last_sample;
    }

    void set_max_step(real_t m)
    {
        ms = m;
    }
    
    real_t get()
    {
    	return last_sample;
    }

private:
    real_t ms;
    real_t last_sample;
};


}

#endif
