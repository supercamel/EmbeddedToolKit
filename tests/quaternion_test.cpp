#include "quaternion_test.h"
#include <etk/etk.h>
using namespace etk;

#include <iostream>
using namespace std;

//#include <Eigen/Geometry>


bool quaternion_test(std::string& subtest)
{
    subtest = "to/from euler";

    etk::Quaternion q;
    etk::Vector<3> euler(-45, 10, 30);
    euler.to_radians();
    q.from_euler(euler);
    euler.to_degrees();

    etk::Vector<3> eq = q.to_euler();
    eq.to_degrees();

    if(eq != euler)
        return false;

    subtest = "gravity rotate";
    euler = etk::Vector<3>(90, 0, 0);
    euler.to_radians();
    q.from_euler(euler);

    etk::Vector<3> gravity(5, 0, 9.8);
    etk::Vector<3> result = q*gravity;

    if(result != etk::Vector<3>(0, 5, 9.8))
        return false;

    subtest = "axis angle";
    Vector<3> v(0.0, 1.0, 0.0);
    q.from_axis_angle(v, 0.84);

    real_t angle = 0;
    q.to_axis_angle(v, angle);

    if(!compare(angle, 0.84, 0.01))
        return false;

    if(!compare(v.x(), 0, 0.001))
        return false;
    if(!compare(v.y(), 1, 0.001))
        return false;

    subtest = "angular vle";
    v = Vector<3>(0.01, 0.2, -0.05);

    q.from_angular_velocity(v, 0.01);
    v = q.to_angular_velocity(0.01);

    etk::StaticString<128> s;
    s += v;
    subtest += s.c_str();

    if(!compare(v.x(), 0.01, 0.001))
        return false;
    if(!compare(v.y(), 0.2, 0.001))
        return false;

    subtest = "exponential";
    return true;
}


