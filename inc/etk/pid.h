

#ifndef PID_H_INCLUDED
#define PID_H_INCLUDED

#include <etk/etk.h>

namespace etk
{

class PIDController
{
public:
    PIDController();
    float step(float error, float dt);
    float step(float setpoint, float measurement, float dt);

    float integral;
    float Kp, Ki, Kd;
    float integral_constraint;

    etk::ExpoMovingAvg der_filter;

    float previous_error;
};


class CircularPIDController : public PIDController
{
public:
    float step(float error, float dt);
    float step(float setpoint, float measurement, float dt);
};

};

#endif

