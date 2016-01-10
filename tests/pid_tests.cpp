#include "pid_tests.h"
#include <iostream>
#include <etk/pid.h>
#include <etk/loop_range.h>
#include <etk/evopid.h>
#include <etk/math_util.h>

#include <fstream>

using namespace etk;
using namespace std;


class System
{
public:
    real_t step(real_t in)
    {
        position += (in*0.22f)+0.8f;
        return position;
    }

    real_t get_pos()
    {
        return position;
    }
private:
    real_t position = 0.0f;
};


bool pid_test(std::string& subtest)
{
    etk::PIDController ctrl;

    ctrl.set_kp(0.0);
    ctrl.set_ki(1.0);
    ctrl.set_kd(0.0);

    for(auto i : range(100))
        ctrl.step(10, 5, 0.1);

    ctrl.set_ki(2.0);
    if(!compare(ctrl.step(10, 5, 0.1), 51, 0.001))
        return false;

    BasicPIDRater rater;
    rater.set_max_error(100.0f);
    rater.set_minimum_samples(50);
    rater.set_min_setpoint_delta(50);
    EvoPid epid(rater);
    epid.set_integral_constraint(100.0f);
    auto s = epid.to_string();

    epid.from_string(s);

    System sys;

    ofstream of("pid.csv");

    real_t sp = 100;
    for(auto j : range(10))
    {
        for(auto i : range(10000))
        {
            sys.step(epid.step(sp, sys.get_pos(), 0.1));
            if(!(i%8))
            {
                sp = -sp;
            }

            of << sys.get_pos() << "\n";
        }
    }

    epid.set_max_mutation(0.1f);

    for(auto j : range(10))
    {
        for(auto i : range(100000))
        {
            sys.step(epid.step(sp, sys.get_pos(), 0.1));
            if(!(i%8))
            {
                sp = -sp;
            }

            //of << sys.get_pos() << "\n";
        }
    }

    return true;
}
