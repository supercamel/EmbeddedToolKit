
#ifndef IMUMATH_VECTOR_HPP
#define IMUMATH_VECTOR_HPP

#include <stdint.h>
#include <math.h>
#include "math_util.h"


namespace etk
{

template <uint32_t N> class Vector
{
public:
    Vector()
    {
        for(uint32_t i = 0; i < N; i++)
            p_vec[i] = 0.0f;
    }

    Vector(float a)
    {
        p_vec[0] = a;
    }

    Vector(float a, float b)
    {
        p_vec[0] = a;
        p_vec[1] = b;
    }

    Vector(float a, float b, float c)
    {
        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
    }

    Vector(float a, float b, float c, float d)
    {
        for(uint32_t i = 0; i < N; i++)
            p_vec[i] = 0.0f;

        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
        p_vec[3] = d;
    }

    Vector(const Vector<N> &v)
    {
        for (uint32_t x = 0; x < N; x++ )
            p_vec[x] = v.p_vec[x];
    }


    uint32_t n() {
        return N;
    }

    //make a vector from magnitude and direction
    void from_polar(float mag, float dir)
    {
        x() = mag*cosf(dir);
        y() = mag*sinf(dir);
    }

    //returns length of vector
    float magnitude()
    {
        float res = 0;
        uint32_t i;
        for(i = 0; i < N; i++)
            res += (p_vec[i] * p_vec[i]);

        if(res != 1.0) //avoid a sqrt if possible
            return sqrtf(res);
        return 1;
    }

    //returns angle between x and y dimensions
    float theta()
    {
        return atan2f(y(),x());
    }

    //sets magnitude to 1.0
    void normalize()
    {
        float mag = magnitude();
        if(compare<float>(mag, 0.0f, 0.00001f))
            return;

        uint32_t i;
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
    float dot(Vector v)
    {
        float ret = 0;
        uint32_t i;
        for(i = 0; i < N; i++)
            ret += p_vec[i] * v.p_vec[i];

        return ret;
    }

    //generates the cross product of two 3D vectors
    Vector cross(Vector v)
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
    Vector scale(float scalar)
    {
        Vector ret;
        for(uint32_t i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] * scalar;
        return ret;
    }

    //inverts
    Vector invert()
    {
        Vector ret;
        for(uint32_t i = 0; i < N; i++)
            ret.p_vec[i] = -p_vec[i];
        return ret;
    }

    template <uint32_t nn> Vector<nn> sub_vector(uint32_t n)
    {
        Vector<nn> ret;
        for(uint32_t i = 0; i < nn; i++)
            ret[i] = (*this)[i+n];
        return ret;
    }

    template <uint32_t nn> void set_sub_vector(Vector<nn> v, uint32_t n)
    {
        for(uint32_t i = 0; i < nn; i++)
            p_vec[i+n] = v[i];
    }

    Vector operator = (Vector v)
    {
        for (uint32_t x = 0; x < N; x++ )
            p_vec[x] = v.p_vec[x];
        return *this;
    }

    bool operator == (Vector v)
    {
        for(uint32_t i = 0; i < N; i++)
        {
            if(!compare<float>(p_vec[i], v.p_vec[i], 0.00001f))
                return false;
        }
        return true;
    }

    Vector& operator << (Vector v)
    {
        (*this) = v;
        return *this;
    }

    uint32_t set(uint32_t v, float value)
    {
        (*this)[v] = value;
        return v;
    }

    void set(float a)
    {
        (*this)[set_flag] = a;
        set_flag = 0;
    }

    template<typename... Args> void set(float a, Args... args)
    {
        (*this)[set_flag++] = a;
        set(args...);
    }

    bool operator != (Vector v)
    {
        return !(operator == (v));
    }

    float& operator [](uint32_t n)
    {
        return p_vec[n];
    }

    float operator [](uint32_t n) const
    {
        return p_vec[n];
    }

    float& operator ()(uint32_t n)
    {
        return p_vec[n];
    }

    Vector operator + (Vector v)
    {
        Vector ret;
        for(uint32_t i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] + v.p_vec[i];
        return ret;
    }

    Vector operator - (Vector v)
    {
        Vector ret;
        for(uint32_t i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] - v.p_vec[i];
        return ret;
    }

    Vector operator * (float scalar)
    {
        return scale(scalar);
    }

    Vector operator / (float scalar)
    {
        Vector ret;
        for(uint32_t i = 0; i < N; i++)
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
        const float radians_to_degrees = 57.2957795131f;
        for(uint32_t i = 0; i < N; i++)
            p_vec[i] *= radians_to_degrees; //180/pi
    }

    void toRadians()
    {
        const float degrees_to_radians = 0.01745329251f; //pi/180
        for(uint32_t i = 0; i < N; i++)
            p_vec[i] *= degrees_to_radians;  //pi/180
    }

    float& x() {
        return p_vec[0];
    }
    float& y() {
        return p_vec[1];
    }
    float& z() {
        return p_vec[2];
    }

	float squaredNorm()
	{
		return dot(*this);
	}


private:
    float p_vec[N];

    uint32_t set_flag = 0;
};


}

#endif
