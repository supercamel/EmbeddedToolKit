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


#include <etk/pid.h>
#include <etk/math_util.h>

namespace etk
{

PIDController::PIDController()
{
    previous_error = integral = Kp = Ki = Kd = 0.0f;
    integral_constraint = 100.0f;
    der_filter = etk::ExpoMovingAvg(0.2, 0);
}

real_t PIDController::step(real_t error, real_t dt)
{
    integral = constrain(integral - error*dt, real_t(-integral_constraint), real_t(integral_constraint));

    if(abs(integral*Ki) > integral_constraint)
        integral -= error*dt;

    der_filter.step((error - previous_error)/dt);

    real_t output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;
    return output;
}

real_t PIDController::step(real_t setpoint, real_t measurement, real_t dt)
{
    real_t error = setpoint - measurement;

    integral += error*dt;
    integral = constrain(integral, -(integral_constraint/Ki), (integral_constraint/Ki));

    der_filter.step((error - previous_error)/dt);

    real_t output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;
    return output;
}

void PIDController::set_ki(real_t ki)
{
    if(ki != 0)
        integral *= (Ki/ki);
    Ki = ki;
}

real_t CircularPIDController::step(real_t setpoint, real_t measurement, real_t segments, real_t dt)
{
	constrain_circular(setpoint, segments);
	constrain_circular(measurement, segments);

    real_t error = setpoint - measurement;

    constrain_circular(error, segments);
    integral = constrain(integral + error*dt, real_t(-integral_constraint), real_t(integral_constraint));

    der_filter.step((error - previous_error)/dt);

    real_t output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;

    return output;
}

};

