

#include "pid.h"

using namespace etk;

PIDController::PIDController()
{
    integral_constraint = previous_error = integral = Kp = Ki = Kd = 0.0f;
    der_filter = etk::ExpoMovingAvg(0.2, 0);
}

float PIDController::step(float error, float dt)
{
    integral = constrain(integral - error*dt, float(-integral_constraint), float(integral_constraint));

    if(fabs(integral*Ki) > integral_constraint)
        integral -= error*dt;

    der_filter.step((error - previous_error)/dt);

    float output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;
    return output;
}

float PIDController::step(float setpoint, float measurement, float dt)
{
    float error = setpoint - measurement;

    integral += error*dt;
    integral = constrain(integral, -(integral_constraint/Ki), (integral_constraint/Ki));

    der_filter.step((error - previous_error)/dt);

    float output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;
    return output;
}

float CircularPIDController::step(float setpoint, float measurement, float dt)
{
	constrain_circular(setpoint, 36000);
	constrain_circular(measurement, 36000);

    float error = setpoint - measurement;
    
    constrain_circular(error, 36000);
    integral = constrain(integral + error*dt, float(-integral_constraint), float(integral_constraint));

    der_filter.step((error - previous_error)/dt);

    float output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;

    return output;
}

