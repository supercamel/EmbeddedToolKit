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

/**
 * \class Vector
 * \brief A vector math class. 
 * @tparam N The number of dimensions.
 */
template <uint32 N> class Vector
{
public:
	/**
	 * \brief constructor sets all elements to zero.
	 */
    Vector()
    {
        for(uint32 i = 0; i < N; i++)
            p_vec[i] = 0.0f;
    }

	/**
	 * \brief sets the value of the first element.
	 */
    Vector(real_t a)
    {
        p_vec[0] = a;
    }

	/**
	 * \brief sets the value of the first two elements.
	 */
    Vector(real_t a, real_t b)
    {
        p_vec[0] = a;
        p_vec[1] = b;
    }
	
	/**
	 * \brief sets the value of the first three elements.
	 * Don't use this function for 2 dimensional vectors.
	 */
    Vector(real_t a, real_t b, real_t c)
    {
        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
    }

	/**
	 * \brief sets the value of the first four elements.
	 * Don't use this function for 2 or 3 dimensional vectors.
	 */
    Vector(real_t a, real_t b, real_t c, real_t d)
    {
        p_vec[0] = a;
        p_vec[1] = b;
        p_vec[2] = c;
        p_vec[3] = d;
    }

	/**
	 * \brief Returns the number of dimensions
	 * @return The number of dimensions in this vector.
	 */
    uint32 n() const 
    {
        return N;
    }

    /**
     * \brief Sets the x and y components of the vector from a magnitude and direction
     * @arg mag a magnitude
     * @arg dir a direction in radians ( use degrees_to_radians() if necessary )
     */
    void from_polar(real_t mag, real_t dir)
    {
        x() = mag*cosf(dir);
        y() = mag*sinf(dir);
    }

    /**
     * \brief gets the magnitude of the vector.
     * @return length of vector
     */
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

    /**
     * \brief returns the angle of the vector in radians
     * @return angle in radians
     */
    real_t theta()
    {
        return atan2f(y(),x());
    }

    /**
     * \brief normalizes the vector. 
     * normalize() sets the magnitude to 1.0.
     */
    void normalize()
    {
        real_t mag = magnitude();
        if(etk::compare(mag, 0.0f, 0.00001f))
            return;

        uint32 i;
        for(i = 0; i < N; i++)
            p_vec[i] = p_vec[i]/mag;
    }

    /**
     * \brief returns a normalised copy of this vector
     * @return a vector with the direction of this and a magnitude of 1.0
     */
    Vector normalized()
    {
        Vector ret = *this;
        ret.normalize();
        return ret;
    }

    /**
     * \brief calculates the dot product of two vectors
     * @arg v another vector
     * @return the dot product and this and v
     */
    real_t dot(Vector v)
    {
        real_t ret = 0;
        uint32 i;
        for(i = 0; i < N; i++)
            ret += p_vec[i] * v.p_vec[i];

        return ret;
    }

    /**
     * \brief generates the cross product of two 3D vectors. this function is invalid for vectors that are not three dimensional.
     * @arg v another vector
     * @return the cross product
     */
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

    /**
     * \brief scales a vector. this changes the magnitude only.
     * @arg a scalar to multiply the vector components by.
     * @return the new scaled vector.
     */
    Vector scale(real_t scalar)
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = p_vec[i] * scalar;
        return ret;
    }

    /**
     * \brief inverts the vector.
     * @return the inverted vector.
     */
    Vector invert()
    {
        Vector ret;
        for(uint32 i = 0; i < N; i++)
            ret.p_vec[i] = -p_vec[i];
        return ret;
    }

	/**
	 * \brief extracts a number of components and creates a new smaller vector.

	 <pre>
Vector<5> v = [ 0, 1, 2, 3, 4 ]
                   ^  ^  ^
                 if we want this
                 
Vector<3> t = v.sub_vector<3>(1)
t = [ 1, 2, 3 ]
     </pre>
     
     * @tparam nn the number of components in the new vector.
	 * @arg n the start point of the sub vector
     */
    template <uint32 nn> Vector<nn> sub_vector(uint32 n)
    {
        Vector<nn> ret;
        for(uint32 i = 0; i < nn; i++)
            ret[i] = (*this)[i+n];
        return ret;
    }

	/**
	 * \brief sets part of the vector from a smaller vector

	 <pre>
Vector<3> t = [ 1, 2, 3 ]

Vector<5> v = [ 0, 0, 0, 0, 4 ]
                 
Vector<3> n = v.set_sub_vector<3>(1)
n = [ 0, 1, 2, 3, 4]

     </pre>
     
     * @tparam nn the number of components in the new vector.
     * @arg v the sub-vector
	 * @arg n the start point of the sub vector
	 * @return a vector that is v, with the subvector overwritten
     */
    template <uint32 nn> void set_sub_vector(Vector<nn> v, uint32 n)
    {
        for(uint32 i = 0; i < nn; i++)
            p_vec[i+n] = v[i];
    }

	/**
	 * \brief assigns v to this.
	 * @arg v a vector
	 * @return *this
	 */
    Vector operator = (Vector v)
    {
        for (uint32 x = 0; x < N; x++ )
            p_vec[x] = v.p_vec[x];
        return *this;
    }

	/**
	 * \brief comparison operator compares two vectors.
	 * @arg v the vector to compare with
	 * @return true if the values of the two vectors are within 0.00001 of each other. 
	 */
    bool operator == (Vector v) const
    {
        for(uint32 i = 0; i < N; i++)
        {
            if(!etk::compare(p_vec[i], v.p_vec[i], 0.00001f))
                return false;
        }
        return true;
    }

	/**
	 * \brief compares two vectors
	 * @arg v the vector to compare with
	 * @arg precision how precisely to compared the two vectors. By default they must be within 0.00001 of each other
	 * @return true if the vectors match
	 */
    bool compare(Vector& v, real_t precision = 0.00001f) const
    {
        for(uint32 i = 0; i < N; i++)
        {
            if(!etk::compare(p_vec[i], v.p_vec[i], precision))
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
        Vector r;
        for(uint32 i = 0; i < N; i++)
            r.p_vec[i] = p_vec[i] + v.p_vec[i];
        return r;
    }

    Vector operator - (Vector v)
    {
        Vector r;
        for(uint32 i = 0; i < N; i++)
            r.p_vec[i] = p_vec[i] - v.p_vec[i];
        return r;
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

    void to_degrees()
    {
        const real_t radians_to_degrees = 57.2957795131f;
        for(uint32 i = 0; i < N; i++)
            p_vec[i] *= radians_to_degrees; //180/pi
    }

    void to_radians()
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

    real_t x() const {
        return p_vec[0];
    }
    real_t y() const {
        return p_vec[1];
    }
    real_t z() const {
        return p_vec[2];
    }

    real_t squared_norm() const
    {
        return dot(*this);
    }

    real_t get_x() const {
        return p_vec[0];
    }

    real_t get_y() const {
        return p_vec[1];
    }

    real_t get_z() const {
        return p_vec[2];
    }
    
    void set_x(const real_t x) {
        p_vec[0] = x;
    }
    
    void set_y(const real_t y) {
        p_vec[1] = y;
    }
    
    void set_z(const real_t z) {
        p_vec[2] = z;
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
