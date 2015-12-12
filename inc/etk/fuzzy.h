#ifndef ETK_FUZZY_H
#define ETK_FUZZY_H

#include <etk/list.h>

namespace etk
{

template <uint16_t N> class Fuzzy
{
public:
    class Set
    {
    friend class Fuzzy<N>;
    
    public:
        void set_points(float p1, float p2, float p3)
        {
            min = p1;
            mid = p2;
            max = p3;
        }

        float get_dom(float crisp_in)
        {
            if(pos == MID)
            {
                if(crisp_in < min)
                    return 0;
                if(crisp_in > max)
                    return 0;
            }

            if(pos == START)
            {
                if(crisp_in < mid)
                    return 1;
            }
            if(pos == END)
            {
                if(crisp_in >= mid)
                    return 1;
            }

            if(crisp_in > mid)
            {
                float range = max-mid;
                crisp_in -= mid;
                crisp_in /= range;
            }
            else
            {
                float range = mid-min;
                crisp_in -= mid;
                crisp_in /= range;
            }
            if(crisp_in < 0)
                crisp_in *= -1;

            if(crisp_in > 1)
                crisp_in = 1;

            return (1-crisp_in);
        }
        
        void set_value(float v)
        {
        	val = v;
        }

        float get_result(float crisp_in)
        {
            return get_dom(crisp_in)*val;
        }

	protected:
		enum FUZZY_POINT { START,MID,END };
		
		void set_position(FUZZY_POINT p)
        {
        	pos = p;
        }
        
	private:
        float min, mid, max;
        FUZZY_POINT pos;
        float val;
    };


    void add_set(Set& f)
    {
        sets.append(f);
        sets[0].set_position(Set::START);
        int32_t end = sets.size()-1;
        for (auto i : range(end))
            sets[i].set_position(Set::MID);
        sets[sets.size()-1].set_position(Set::END);
    }

    uint32_t get_n_sets()
    {
        return sets.size();
    }

    void clear_sets()
    {
        sets.clear();
    }

    float crisp_out(float crisp_in)
    {
        auto out = 0.0f;
        for(auto set : sets)
            out += set.get_result(crisp_in);
        return out;
    }

private:

    etk::List<Set, N+1> sets;
};

};


#endif

