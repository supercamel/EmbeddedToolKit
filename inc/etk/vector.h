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

#ifndef IMUMATH_VECTOR_HPP
#define IMUMATH_VECTOR_HPP

#include "types.h"
#include "math_util.h"
#include <math.h>


namespace etk
{

template <uint32 N> class Vector
{
public:
    Vector()
    {
        for(uint32 i = 0; i < N; i++)
            p_vec[i] = 0.0f;
    }

    Vector(real_t a)
    {
        p_vec[0] = a;
    }

    Vector(real_t a, real_t b)
    {
        p_vec[0] = a;
        p_vec[1] = b;
    }

    Vector(real_t a, real_t b, real_t c)
    {
        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
    }

    Vector(real_t a, real_t b, real_t c, real_t d)
    {
        for(uint32 i = 0; i < N; i++)
            p_vec[i] = 0.0f;

        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
        p_vec[3] = d;
    }

    Vector(const Vector<N> &v)
    {
        for (uint32 x = 0; x < N; x++ )
            p_vec[x] = v.p_vec[x];
    }


    uint32 n() {
        return N;
    }

    //make a vector from magnitude and direction
    void from_polar(real_t mag, real_t dir)
    {
        x() = mag*cosf(dir);
        y() = mag*sinf(dir);
    }

    //returns length of vector
    real_t magnitude()
    {
        real_t res = 0;
        uint32 i;
        for(i = 0; i < N; i++)
            res += (p_vec[i] * p_vec[i]);

        if(res != 1.0) //avoid a sqrt if possible
            return sqrtf(res);
        return 1;
    }

    //returns angle from the origin (0,0) to the vector (x,y) in radians
    real_t theta()
    {
        return atan2f(y(),x());
    }

    //sets magnitude to 1.0
    void normalize()
    {
        real_t mag = magnitude();
        if(etk::compare<real_t>(mag, 0.0f, 0.00001f))
            return;

        uint32 i;
        for(i = 0; i < N; i++)
            p_vec[i] = p_vec[i]/mag;
    }

    //returns a normalised copy of this vector
    Vector normalized()
    {
        Vector ret = *this;
        ret.normalize();
        return ret;
    }

    //returns the dot product of two vectors
    real_t dot(Vector v)
    {
        real_t ret = 0;
        uint32 i;
        for(i = 0; i < N; i++)
            ret += p_vec[i] * v.p_vec[i];

        return ret;
    }

    //generates the cross product of two 3D vectors
    Vector cross(Vector v) const
    {
        Vector ret;

        //the cross product is only valid for vectors with 3 dimensions,
        //with the exception of higher dimensional stuff that is beyond the intended scope of this library
        if(N != 3)
            return ret;

        ret.p_vec[0] = (p_vec[1] * v.p_vec[2]) - (p_vec[2] * v.p_vec[1]);
        ret.p_vec[1] = (p_vec[2] * v.p_vec[0]) - (p_vec[0] * v.p_vec[2]);
        ret.p_vec[2] = (p_vec[0] * v.p_vec[1]) - (p_vec[1] * v.p_vec[0]);
        return ret;
    }

    //scales a vector. this changes the magnitude only
    Vector scale(real_t scalar)
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] * scalar;
        return ret;
    }

    //inverts
    Vector invert()
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = -p_vec[i];
        return ret;
    }

    template <uint32 nn> Vector<nn> sub_vector(uint32 n)
    {
        Vector<nn> ret;
        for(uint32 i = 0; i < nn; i++)
            ret[i] = (*this)[i+n];
        return ret;
    }

    template <uint32 nn> void set_sub_vector(Vector<nn> v, uint32 n)
    {
        for(uint32 i = 0; i < nn; i++)
            p_vec[i+n] = v[i];
    }

    Vector operator = (Vector v)
    {
        for (uint32 x = 0; x < N; x++ )
            p_vec[x] = v.p_vec[x];
        return *this;
    }

    bool operator == (Vector v)
    {
        for(uint32 i = 0; i < N; i++)
        {
            if(!etk::compare<real_t>(p_vec[i], v.p_vec[i], 0.00001f))
                return false;
        }
        return true;
    }

    bool compare(Vector& v, real_t precision = 0.00000f)
    {
        for(uint32 i = 0; i < N; i++)
        {
            if(!etk::compare<real_t>(p_vec[i], v.p_vec[i], precision))
                return false;
        }
        return true;
    }

    Vector& operator << (Vector v)
    {
        (*this) = v;
        return *this;
    }

    uint32 set(uint32 v, real_t value)
    {
        (*this)[v] = value;
        return v;
    }

    void set(real_t a)
    {
        (*this)[set_flag] = a;
        set_flag = 0;
    }

    template<typename... Args> void set(real_t a, Args... args)
    {
        (*this)[set_flag++] = a;
        set(args...);
    }

    bool operator != (Vector v)
    {
        return !(operator == (v));
    }

    real_t& operator [](uint32 n)
    {
        return p_vec[n];
    }

    real_t operator [](uint32 n) const
    {
        return p_vec[n];
    }

    real_t& operator ()(uint32 n)
    {
        return p_vec[n];
    }

    Vector operator + (Vector v)
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] + v.p_vec[i];
        return ret;
    }

    Vector operator - (Vector v)
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] - v.p_vec[i];
        return ret;
    }

    Vector operator * (real_t scalar)
    {
        return scale(scalar);
    }

    Vector operator *= (real_t scalar)
    {
    	*this = scale(scalar);
    	return *this;
    }

    Vector operator /= (real_t scalar)
    {
    	*this = *this/scalar;
    	return *this;
    }

    Vector operator / (real_t scalar)
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] / scalar;
        return ret;
    }

    Vector operator += (Vector v)
    {
        *this = *this + v;
        return *this;
    }

    void toDegrees()
    {
        const real_t radians_to_degrees = 57.2957795131f;
        for(uint32 i = 0; i < N; i++)
            p_vec[i] *= radians_to_degrees; //180/pi
    }

    void toRadians()
    {
        const real_t degrees_to_radians = 0.01745329251f; //pi/180
        for(uint32 i = 0; i < N; i++)
            p_vec[i] *= degrees_to_radians;  //pi/180
    }

    real_t& x() {
        return p_vec[0];
    }
    real_t& y() {
        return p_vec[1];
    }
    real_t& z() {
        return p_vec[2];
    }

    real_t squaredNorm()
    {
        return dot(*this);
    }


private:
    real_t p_vec[N];

    uint32 set_flag = 0;
};

typedef Vector<2> Vector2d;
typedef Vector<3> Vector3d;
typedef Vector<4> Vector4d;

}

#endif
