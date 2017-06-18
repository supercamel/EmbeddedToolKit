/*
    Embedded Tool Kit
    Copyright (C) 2015 Samuel Cowen

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.
*/

#ifndef ETK_FUZZY_H
#define ETK_FUZZY_H

#include "list.h"

namespace etk
{

/**
 * \class Fuzzy
 *
 * \brief Fuzzy logic class. Fuzzy logic can be used for function approximation, control applications, signal process, AI and so on.
 * Here is a tutorial on using Fuzzy logic with ETK
 * http://www.camelsoftware.com/blog/2015/12/12/fuzzy-logic-control-part-1/
 *
 * @tparam N The maximum number of fuzzy sets to use.
 */


template <uint16 N> class Fuzzy
{
public:

    /**
     * \class Set
     *
     * \brief A fuzzy logic set.
     *
     */

    class Set
    {
        friend class Fuzzy<N>;

    public:

        /**
         * \brief set_points modifies the range and mid point of a fuzzy logic set. This controls the shape of the triangle.
         *
         * @arg p1 minimum point
         * @arg p2 middle/peak of triangle
         * @arg p3 maximum point
         */
        void set_points(real_t p1, real_t p2, real_t p3)
        {
            min = p1;
            mid = p2;
            max = p3;
        }

        /**
         * \brief get_dom returns the degree of membership of a crisp input. If the crisp input is less than the minimum point or greather than the maximum point, the DOM is zero.
         * Otherwise, the DOM represents how close the crisp is to the mid point.
         *
         * @arg crisp_in The crisp input
         * @return The degree of membership. This will be a number between 0.0 and 1.0
         */
        real_t get_dom(real_t crisp_in)
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
                real_t range = max-mid;
                crisp_in -= mid;
                crisp_in /= range;
            }
            else
            {
                real_t range = mid-min;
                crisp_in -= mid;
                crisp_in /= range;
            }
            if(crisp_in < 0)
                crisp_in *= -1;

            if(crisp_in > 1)
                crisp_in = 1;

            return (1-crisp_in);
        }

        /**
         * \brief Sets the value of this set.
         */
        void set_value(real_t v)
        {
            val = v;
        }

        /**
         * \brief Returns the degree of membership of a crisp input, multiplied by the value of the set.
         */
        real_t get_result(real_t crisp_in)
        {
            return get_dom(crisp_in)*val;
        }

        /**
         * \brief Returns the minimum point.
         */
        real_t get_min()
        {
            return min;
        }

        /**
         * \brief Returns the middle point / triangle peak.
         */
        real_t get_mid()
        {
            return mid;
        }

        /**
         * \brief Returns the maximum point.
         */
        real_t get_max()
        {
            return max;
        }

    protected:
        enum FUZZY_POINT { START,MID,END };

        void set_position(FUZZY_POINT p)
        {
            pos = p;
        }

    private:
        real_t min, mid, max;
        FUZZY_POINT pos;
        real_t val;
    };


    /**
     * \brief Adds a set. You can add up to N sets.
     */
    void add_set(Set& f)
    {
        sets.append(f);
        sets[0].set_position(Set::START);
        int32 end = sets.size()-1;
        for(int i = 0; i < end; i++) {
            sets[i].set_position(Set::MID);
        }
        sets[sets.size()-1].set_position(Set::END);
    }

    /**
     * \brief Returns the number of sets.
     */
    uint32 get_n_sets()
    {
        return sets.size();
    }

    /**
     * \brief Removes all sets.
     */
    void clear_sets()
    {
        sets.clear();
    }

    /**
     * \brief Returns a crisp output for a given crisp input.
     * @arg crisp_in A crisp input.
     * @return A crisp output.
     */
    real_t crisp_out(real_t crisp_in)
    {
        auto out = 0.0f;
        for(auto set : sets)
            out += set.get_result(crisp_in);
        return out;
    }

    /**
     * \brief Returns a fuzzy logic class that is inverted.
     * Let's say you're using Fuzzy to control speed using throttle. You know for throttle x you get speed y.
     * You can plot the throttle response using Fuzzy, but all that's going to tell is you what speed to expect for a selected throttle setting.
     * By inverting the Fuzzy sets, you can work backwards and determine what throttle setting is required for a selected speed.
     * @return A Fuzzy class that plots an inverse function.
     */
    Fuzzy<N> inverse()
    {
        auto ret = *this;
        for(auto& set : ret.sets)
        {
            real_t set_min = set.get_min();
            real_t set_mid = set.get_mid();
            real_t set_max = set.get_max();
            set.set_points(crisp_out(set_min), crisp_out(set_mid), crisp_out(set_max));
            set.set_value(set_mid);
        }
        return ret;
    }

private:

    etk::List<Set, N+1> sets;
};

};


#endif
