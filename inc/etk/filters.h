
#ifndef FILTERS_H_INCLUDED
#define FILTERS_H_INCLUDED

#include "etk/etk.h"

namespace etk
{

/**
 * \class ExpoMovingAvg
 *
 * \brief An exponential moving average low-pass filter.
 *
 * @code
    etk::ExpoMovingAvg filter(0.1);
    
    for(auto i : etk::range(100))
    {
    	filter.step(i);
    	cout << filter.get() << endl;
    }
    
    @endcode
 *
 */
 
class ExpoMovingAvg
{
public:
    ExpoMovingAvg()
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
    ExpoMovingAvg(float f, float init_est = 0)
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
    void set_gain(float factor)
    {
        a = factor;
    }
    
    /**
     * \brief Performs a single iteration of the filter. 
     * Raw samples are entered into the filter using this function.
     *
     * @arg measurement A raw measurement or sample to be filtered.
     */
    void step(float measurement)
    {
        accumulator = (a* measurement) + (1.0f - a) * accumulator;
    }

	/**
     * \brief Returns the current state of the filter.
     */
    float get()
    {
        return accumulator;
    }

private:
    float accumulator;
    float a;
};


/**
 * \class BrownLinearExpo
 *
 * \brief Browns linear exponential filter is a form of double exponential smoothing. It can be more responsive than the MovingExpoAvg filter, but is prone to overshoot.
 */
 
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
    
    /**
	 * \brief This constructor allows you to set the filter gain and initial estimate.
	 *
	 * @arg f filter gain
	 * @arg init_est initial estimate
	 */
    BrownLinearExpo(float f, float init_est)
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
    void set_gain(float factor)
    {
        a = factor;
    }
    
    /**
     * \brief Performs a single iteration of the filter. 
     * Raw samples are entered into the filter using this function.
     *
     * @arg measurement A raw measurement or sample to be filtered.
     */
    void step(float measurement)
    {
        single_smoothed = a * measurement + (1 - a) * single_smoothed;
        double_smoothed = a * single_smoothed + (1 - a) * double_smoothed;

        float est_a = (2*single_smoothed - double_smoothed);
        float est_b = (a / (1-a) )*(single_smoothed - double_smoothed);
        estimate = est_a + est_b;
    }
    
    /**
     * \brief Returns the current state of the filter.
     */
    float get()
    {
        return estimate;
    }

private:
    float estimate, double_smoothed, single_smoothed;
    float a;
};


/**
 * \class scalarLinearKalman
 *
 * \brief A linear kalman filter for scalars.
 */
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

private:
    float B, current_state_estimate, current_prob_estimate;
    float Q, R;
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
     
    HighPassFilter(float gain) : emv(gain)
    {
        estimate = 0;
    }

	 /**
     * \brief Performs a single iteration of the filter. 
     * Raw samples are entered into the filter using this function.
     *
     * @arg measurement A raw measurement or sample to be filtered.
     */
    void step(float sample)
    {
        emv.step(sample);

        estimate = sample - emv.get();
    }

	/**
     * \brief Returns the current state of the filter.
     */
    float get()
    {
        return estimate;
    }

private:
    ExpoMovingAvg emv;
    float estimate;
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
