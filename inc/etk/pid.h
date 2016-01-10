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


#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include "filters.h"

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
    real_t step(real_t error, real_t dt);

    /**
	 * \brief Perform an interation of the PID controller using a setpoint and measurement to calculate the error.
	 *
	 * @arg setpoint The setpoint
	 * @arg measurement The measurement
	 * @arg dt Time period since the last iteration.
	 * @return Control output.
	 */
    real_t step(real_t setpoint, real_t measurement, real_t dt);

    /**
     * \brief Sets the proportional gain of the controller.
     */
	void set_kp(real_t kp) { Kp = kp; }

	/**
	 * \brief Sets the integral gain of the controller. This functional also scales the integral so that there is no large disturbance to the output.
	 */
	void set_ki(real_t ki);

	/**
	 * \brief Sets the derivative gain.
	 */
	void set_kd(real_t kd) { Kd = kd; }

	real_t get_kp() { return Kp; }
	real_t get_ki() { return Ki; }
	real_t get_kd() { return Kd; }

	/**
	 * \brief Sets the integral to zero.
	 */
	void reset_integral() { integral = 0; }

	/**
	 * \brief Sets a maximum size for the integral. This can help prevent integral wind up.
	 */
	void set_max_integral(real_t imax) { integral_constraint = imax; }

	/**
	 * \brief Sets the derivative filter gain. The derivative filter is an ExpoMovingAvg filter.
	 */
	void set_derivative_filter_gain(real_t g) { der_filter.set_gain(g); }

	/**
	 * \brief Returns the integral value.
	 */
    real_t get_integral() { return integral; }

    /**
	 * \brief Sets the integral value. USE WITH CAUTION. Suddenly changing the integral will suddenly change the output.
	 */
    void set_integral(real_t integ) { integral = integ; }

protected:
    real_t integral;
    real_t Kp, Ki, Kd;
    real_t integral_constraint;

    ExpoMovingAvg der_filter;

    real_t previous_error;
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
    real_t step(real_t setpoint, real_t measurement, real_t segments, real_t dt);
};

};

#endif

