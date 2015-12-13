

#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include <etk/etk.h>

namespace etk
{

/**
 * \class PIDController
 *
 * \brief A generic PIDController. 
 * https://en.wikipedia.org/wiki/PID_controller
 */
 
class PIDController
{
public:
    PIDController();
    
    /**
	 * \brief Perform an interation of the PID controller by supplying your own error value. 
	 *
	 * @arg error Some pre-calculated error value.
	 * @arg dt Time period since the last iteration. 
	 * @return Control output.
	 */
    float step(float error, float dt);
    
    /**
	 * \brief Perform an interation of the PID controller using a setpoint and measurement to calculate the error.
	 *
	 * @arg setpoint The setpoint
	 * @arg measurement The measurement
	 * @arg dt Time period since the last iteration. 
	 * @return Control output.
	 */
    float step(float setpoint, float measurement, float dt);

    /**
     * \brief Sets the proportional gain of the controller.
     */
	void set_kp(float kp) { Kp = kp; }
	
	/**
	 * \brief Sets the integral gain of the controller. WARNING: changing the integral gain can result in unstable behaviour - similar to integral windup.
	 * This could be fixed by mapping the old integral and Ki to the new Ki. TODO
	 */
	void set_ki(float ki) { Ki = ki; }
	
	/**
	 * \brief Sets the derivative gain.
	 */
	void set_kd(float kd) { Kd = kd; }
	
	float get_kp() { return Kp; }
	float get_ki() { return Ki; }
	float get_kd() { return Kd; }
	
	/**
	 * \brief Sets the integral to zero.
	 */
	void reset_integral() { integral = 0; }
	
	/**
	 * \brief Sets a maximum size for the integral. This can help prevent integral wind up.
	 */
	void set_max_integral(float imax) { integral_constraint = imax; } 
	
	/**
	 * \brief Sets the derivative filter gain. The derivative filter is an ExpoMovingAvg filter.
	 */
	void set_derivative_filter_gain(float g) { der_filter.set_gain(g); }
	
protected:
    float integral;
    float Kp, Ki, Kd;
    float integral_constraint;

    etk::ExpoMovingAvg der_filter;

    float previous_error;
};

/**
 * \class CircularPIDController
 *
 * \brief A Circular PID Controller. This controller uses constrain_circular(); to keep it's inputs and error within a circular range.
 * If segments was set to 360, the setpoint was 175 and the measurement was -175, the resulting error would be -10 degrees.
 * This makes the circular PID controller suitable for controlling the heading of a vehicle, for example. 
 */
 
class CircularPIDController : public PIDController
{
public:
    float step(float setpoint, float measurement, float segments, float dt);
};

};

#endif

