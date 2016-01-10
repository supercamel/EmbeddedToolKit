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

#include <etk/evopid.h>
#include <cstdlib>
#include <etk/loop_range.h>
#include <etk/tokeniser.h>


namespace etk
{

static inline float rand_one()
{
    return ((real_t) rand() / (RAND_MAX));
}

static inline real_t rand_one_range()
{
    return ((real_t) rand() / ((RAND_MAX)))-0.5f;
}


EvoPid::EvoPid(PIDRater& rater) : rater(rater)
{
    PIDGain pg = { 1.0, 0.0, 0.0 };
    repopulate(pg);
    der_filter.set_gain(0.1);
    best_ever.score = INFINITY;
}

real_t EvoPid::step(real_t setpoint, real_t measurement, real_t dt)
{
    if(rater.rate(setpoint, measurement))
    {
        pids[current_pid].score = rater.get_score();
        current_pid++;
        if(current_pid == POPULATION)
        {
            generation_counter++;
            current_pid = 0;
            for(auto i : pids)
            {
                if(std::isnan(i.score))
                    i.score = INFINITY;
            }
            bubble_sort_up(pids, POPULATION);
            PIDGain father = pids[0];
            if(father.score < best_ever.score)
                best_ever = father;
            PIDGain mother;
            /*
            if(rand_one() > 0.3)
            {
                uint8 m[3];
                m[0] = (rand_one()*3)+1.0f;
                m[1] = (rand_one()*2)+1.0f;
                m[2] = 1.0f;
                int v = m[int(rand_one()*3)];
                mother = pids[v];
            }
            else
                mother = best_ever;
            */
            uint8 m[2];
            m[0] = (rand_one()*2)+1.0f;
            m[1] = 1.0f;
            int v = m[int(rand_one()*2)];
            mother = pids[v];

            max_mutation *= 0.99;
            if(max_mutation < min_mutation)
                max_mutation = min_mutation;
            breed(mother, father);
        }
    }
    real_t error = setpoint - measurement;
    integral += error*dt;
    integral = constrain(integral, -integral_constraint, integral_constraint);
    der_filter.step((error - previous_error)/dt);
    real_t output = pids[current_pid].kp*error + pids[current_pid].ki*integral + pids[current_pid].kd*der_filter.get();
    previous_error = error;
    return output;
}

void EvoPid::breed(PIDGain& mother, PIDGain& father)
{
    for(auto& i : pids)
    {
        if(rand_one() > 0.5)
            i.kp = mother.kp;
        else
            i.kp = father.kp;
        if(rand_one() > 0.5)
            i.ki = mother.ki;
        else
            i.ki = father.ki;
        if(rand_one() > 0.5)
            i.kd = mother.kd;
        else
            i.kd = father.kd;
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
        mutant.ki += (rand_one_range())*max_mutation*0.5;
        if(mutant.ki < 0)
            mutant.ki = 0;
    }
    if(rand_one() < mutation_rate)
    {
        mutant.kd += rand_one_range()*max_mutation*0.1;
        if(mutant.kd < 0)
            mutant.kd = 0;
    }
}

void EvoPid::repopulate(PIDGain& p)
{
    real_t tmp_mute = mutation_rate;
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

