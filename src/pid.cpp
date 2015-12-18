

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

float PIDController::step(float error, float dt)
{
    integral = constrain(integral - error*dt, float(-integral_constraint), float(integral_constraint));

    if(abs(integral*Ki) > integral_constraint)
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

void PIDController::set_ki(float ki)
{
    if(ki != 0)
        integral *= (Ki/ki);
    Ki = ki;
}

float CircularPIDController::step(float setpoint, float measurement, float segments, float dt)
{
	constrain_circular(setpoint, segments);
	constrain_circular(measurement, segments);

    float error = setpoint - measurement;

    constrain_circular(error, segments);
    integral = constrain(integral + error*dt, float(-integral_constraint), float(integral_constraint));

    der_filter.step((error - previous_error)/dt);

    float output = Kp*error + Ki*integral + Kd*der_filter.get();
    previous_error = error;

    return output;
}

};

