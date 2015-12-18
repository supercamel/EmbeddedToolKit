#ifndef EVOPID_H_INCLUDED
#define EVOPID_H_INCLUDED

#include "array.h"
#include "pid_rater.h"
#include "filters.h"
#include "staticstring.h"
#include "tokeniser.h"

namespace etk
{

/**
 * \class EvoPid
 *
 * \brief A self tuning PID controller that uses Evolutionary Algorithms to search for optimal PID gains.
 */

class EvoPid
{
public:
    class PIDGain
    {
    public:
        PIDGain() { }
        PIDGain(float p, float i, float d)
        {
            kp = p;
            ki = i;
            kd = d;
        }

        bool operator > (PIDGain& pg)
        {
            return score > pg.score;
        }

        bool operator < (PIDGain& pg)
        {
            return score < pg.score;
        }
        float kp, ki, kd;
        float score;
    };


    EvoPid(PIDRater& rater);

    float step(float setpoint, float measurement, float dt);

    void repopulate(PIDGain& p);

    void set_max_mutation(float mutation_max) { max_mutation = mutation_max; }
    void set_mutation_rate(float mute_rate) { mutation_rate = mute_rate; }

    auto to_string()
    {
        StaticString<256> ss;
        auto rope = ss.get_rope();
        for(auto p : pids)
            rope << p.kp << " " << p.ki << " " << p.kd << "\n";
        return ss;
    }


    void from_string(auto& s)
    {
        auto line_tok = make_tokeniser(s, s.length());
        StaticString<120> line;
        uint32_t line_count = 0;
        while(line_tok.next(line, 120))
        {
            auto gain_tok = make_tokeniser(line, line.length());
            StaticString<20> val_str;
            uint32_t gain_count = 0;
            while(gain_tok.next(val_str, 20))
            {
                if(gain_count == 0)
                    pids[line_count].kp = val_str.atof();
                if(gain_count == 1)
                    pids[line_count].ki = val_str.atof();
                if(gain_count == 2)
                    pids[line_count].kd = val_str.atof();
                gain_count++;
            }

            line_count++;
            if(line_count == 6)
                break;
        }
    }

    void set_integral_constraint(float ic)
    {
        integral_constraint = ic;
    }

    void set_kd_filter_gain(float g)
    {
        der_filter.set_gain(g);
    }

private:
    void breed(PIDGain& mother, PIDGain& father);
    void apply_mutation(PIDGain& mutant);

    Array<PIDGain, 8> pids;
    PIDRater& rater;

    uint8_t current_pid;
    float mutation_rate = 10.0f;
    float max_mutation = 1.0f;

    float integral = 0.0f;
    float integral_constraint = 10.0f;
    float previous_error = 0.0f;
    etk::ExpoMovingAvg der_filter;
};

}


#endif // EVOPID_H_INCLUDED
