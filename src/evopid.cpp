#include <etk/evopid.h>
#include <cstdlib>
#include <etk/loop_range.h>
#include <etk/tokeniser.h>

#include <iostream>
using namespace std;


namespace etk
{

static inline float rand_one()
{
    return ((float) rand() / (RAND_MAX));
}

static inline float rand_one_range()
{
    return ((float) rand() / ((RAND_MAX)))-0.5f;
}


EvoPid::EvoPid(PIDRater& rater) : rater(rater)
{
    PIDGain pg = { 1.0, 0.0, 0.0 };
    repopulate(pg);
    der_filter.set_gain(0.1);
}

float EvoPid::step(float setpoint, float measurement, float dt)
{
    if(rater.rate(setpoint, measurement))
    {
        pids[current_pid].score = rater.get_score();
        current_pid++;
        if(current_pid == 8)
        {
            current_pid = 0;
            for(auto i : pids)
            {
				if(std::isnan(i.score))
					i.score = INFINITY;
            }
			bubble_sort_up(pids, 8);

			PIDGain father = pids[0];

            uint8_t m[3];
			m[0] = (rand_one()*7)+1.0f;
			m[1] = (rand_one()*4)+1.0f;
			m[2] = (rand_one()*2)+1.0f;
			int v = m[int(rand_one()*3)];

            PIDGain mother = pids[v];

            breed(mother, father);
        }
    }

    float error = setpoint - measurement;

    integral += error*dt;
    integral = constrain(integral, -integral_constraint, integral_constraint);

    der_filter.step((error - previous_error)/dt);

    float output = pids[current_pid].kp*error + pids[current_pid].ki*integral + pids[current_pid].kd*der_filter.get();
    previous_error = error;
    return output;
}

void EvoPid::breed(PIDGain& mother, PIDGain& father)
{
    for(auto& i : pids)
    {
        float pc = constrain(rand_one(), 0.0001, 0.999);
        i.kp = (mother.kp*pc) + (father.kp*(1.0-pc));
        pc = constrain(rand_one(), 0.0001, 0.999);
        i.ki =  (mother.ki*pc) + (father.ki*(1.0-pc));
        pc = constrain(rand_one(), 0.0001, 0.999);
        i.kd =  (mother.kd*pc) + (father.kd*(1.0-pc));

        apply_mutation(i);
    }
}

void EvoPid::apply_mutation(PIDGain& mutant)
{
    if(rand_one() < mutation_rate)
    {
        mutant.kp += rand_one_range()*max_mutation;
        if(mutant.kp < 0)
            mutant.kp = 0;
    }

    if(rand_one() < mutation_rate)
    {
        mutant.ki += (rand_one_range())*max_mutation*0.7;
        if(mutant.ki < 0)
            mutant.ki = 0;
    }

    if(rand_one() < mutation_rate)
    {
        mutant.kd += rand_one_range()*max_mutation*0.3;
        if(mutant.kd < 0)
            mutant.kd = 0;
    }
}

void EvoPid::repopulate(PIDGain& p)
{
    float tmp_mute = mutation_rate;
    mutation_rate = 1.0f;

    for(auto& i : pids)
    {
        PIDGain g = p;
        apply_mutation(g);
        i = g;
    }

    mutation_rate = tmp_mute;
}


}

