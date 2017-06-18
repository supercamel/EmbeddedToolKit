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

#ifndef IMUMATH_QUATERNION_HPP
#define IMUMATH_QUATERNION_HPP

#include "types.h"
#include "vector.h"


namespace etk
{


class Quaternion
{
public:
    Quaternion()
    {
        _w = 1.0f;
        _x = _y = _z = 0.0f;
    }

    Quaternion(real_t iw, real_t ix, real_t iy, real_t iz)
    {
        _w = iw;
        _x = ix;
        _y = iy;
        _z = iz;
    }

    Quaternion(real_t w, Vector<3> vec)
    {
        _w = w;
        _x = vec.x();
        _y = vec.y();
        _z = vec.z();
    }

    Quaternion(Vector<4> v)
    {
        _w = v[0];
        _x = v[1];
        _y = v[2];
        _z = v[3];
    }

    Vector<4> to_vector()
    {
        return Vector<4>(_w, _x, _y, _z);
    }

    void set_vector(Vector<3> v)
    {
        _x = v.x();
        _y = v.y();
        _z = v.z();
    }

    real_t& w()
    {
        return _w;
    }
    real_t& x()
    {
        return _x;
    }
    real_t& y()
    {
        return _y;
    }
    real_t& z()
    {
        return _z;
    }

    real_t magnitude()
    {
        real_t res = (_w*_w) + (_x*_x) + (_y*_y) + (_z*_z);
        return sqrtf(res);
    }

    void normalize()
    {
        real_t mag = magnitude();
        *this = this->scale(1/mag);
    }


    Quaternion conjugate()
    {
        Quaternion q;
        q.w() = _w;
        q.x() = -_x;
        q.y() = -_y;
        q.z() = -_z;
        return q;
    }

    void fromEuler(Vector<3> euler)
    {
        Quaternion h, p, r;
        Vector<3> v(0.0, 0.0, 1.0);
        h.fromAxisAngle(v, euler.x());
        v = Vector<3>(0.0, 1.0, 0.0);
        p.fromAxisAngle(v, euler.y());
        v = Vector<3>(1.0, 0.0, 0.0);
        r.fromAxisAngle(v, euler.z());

        *this = (h*p*r);
    }

    void fromAxisAngle(Vector<3> axis, real_t theta)
    {
        _w = cosf(theta/2.0f);
        //only need to calculate sine of half theta once
        real_t sht = sinf(theta/2.0f);
        _x = axis.x() * sht;
        _y = axis.y() * sht;
        _z = axis.z() * sht;
    }

    void toAxisAngle(Vector<3>& axis, real_t& angle)
    {
        normalize();

        axis = Vector<3>(0, 0, 0);
        angle = 0;

        //if w is 1, then this is a singularity (axis angle is zero)
        if(compare(_w, 1.0, 0.0001))
            return;

        real_t sqw = sqrtf(1.0-(_w*_w));

        if(compare(sqw, 0.0f, 0.0001f)) //it's a singularity and divide by zero, avoid
            return;

        angle = 2 * acosf(real_t(_w));
        axis.x() = _x / sqw;
        axis.y() = _y / sqw;
        axis.z() = _z / sqw;
    }


    void fromMatrix(Matrix<3, 3> m)
    {
    	#ifdef ETK_MAX
        _w = sqrtf(max<real_t>( 0, 1 + m(0,0) + m(1,1) + m(2,2))) / 2.0f;
        _x = sqrtf(max<real_t>( 0, 1 + m(0,0) - m(1,1) - m(2,2))) / 2.0f;
        _y = sqrtf(max<real_t>( 0, 1 - m(0,0) + m(1,1) - m(2,2))) / 2.0f;
        _z = sqrtf(max<real_t>( 0, 1 - m(0,0) - m(1,1) + m(2,2))) / 2.0f;
       	#else
       	_w = sqrtf(max( 0, 1 + m(0,0) + m(1,1) + m(2,2))) / 2.0f;
        _x = sqrtf(max( 0, 1 + m(0,0) - m(1,1) - m(2,2))) / 2.0f;
        _y = sqrtf(max( 0, 1 - m(0,0) + m(1,1) - m(2,2))) / 2.0f;
        _z = sqrtf(max( 0, 1 - m(0,0) - m(1,1) + m(2,2))) / 2.0f;
        #endif
        
        _x = copysign_zero(_x, m(2,1) - m(1,2));
        _y = copysign_zero(_y, m(0,2) - m(2,0));
        _z = copysign_zero(_z, m(1,0) - m(0,1));
    }

    Matrix<3, 3> toMatrix()
    {
        Matrix<3,3> ret;
        ret.cell(0, 0) = 1-(2*(_y*_y))-(2*(_z*_z));
        ret.cell(0, 1) = (2*_x*_y)-(2*_w*_z);
        ret.cell(0, 2) = (2*_x*_z)+(2*_w*_y);

        ret.cell(1, 0) = (2*_x*_y)+(2*_w*_z);
        ret.cell(1, 1) = 1-(2*(_x*_x))-(2*(_z*_z));
        ret.cell(1, 2) = (2*(_y*_z))-(2*(_w*_x));

        ret.cell(2, 0) = (2*(_x*_z))-(2*_w*_y);
        ret.cell(2, 1) = (2*_y*_z)+(2*_w*_x);
        ret.cell(2, 2) = 1-(2*(_x*_x))-(2*(_y*_y));
        return ret;
    }


    Vector<3> toEuler()
    {
        Vector<3> ret;
        real_t sqw = _w*_w;
        real_t sqx = _x*_x;
        real_t sqy = _y*_y;
        real_t sqz = _z*_z;

        ret.x() = atan2f(real_t(real_t(2.0)*(_x*_y+_z*_w)),real_t(sqx-sqy-sqz+sqw));
        ret.y() = asinf(real_t(real_t(-2.0)*(_x*_z-_y*_w))/real_t(sqx+sqy+sqz+sqw));
        ret.z() = atan2f(real_t(real_t(2.0)*(_y*_z+_x*_w)),real_t(-sqx-sqy+sqz+sqw));

        return ret;
    }



    Vector<3> toAngularVelocity(real_t dt)
    {
        Vector<3> ret;
        if(dt == 0)
            return ret;

        real_t angle = 0;
        toAxisAngle(ret, angle);

        ret = ret*angle; //finds angular displacement
        ret = ret/dt; //over dt to find angular velocity

        return ret;

    }

    void fromAngularVelocity(Vector<3> w, real_t dt)
    {
        real_t theta = w.magnitude() * dt;
        w.normalize();

        fromAxisAngle(w, theta);
    }


    Vector<3> rotateVector(Vector<2> v)
    {
        Vector<3> ret(v.x(), v.y(), 0.0);
        return rotateVector(ret);
    }

    Vector<3> rotateVector(Vector<3> v)
    {
        Vector<3> qv(this->x(), this->y(), this->z());
        Vector<3> t;
        t = qv.cross(v) * real_t(2.0);
        return v + (t * _w) + qv.cross(t);
    }


    Quaternion operator * (Quaternion q)
    {
        Quaternion ret;
        ret._w = ((_w*q._w) - (_x*q._x) - (_y*q._y) - (_z*q._z));
        ret._x = ((_w*q._x) + (_x*q._w) + (_y*q._z) - (_z*q._y));
        ret._y = ((_w*q._y) - (_x*q._z) + (_y*q._w) + (_z*q._x));
        ret._z = ((_w*q._z) + (_x*q._y) - (_y*q._x) + (_z*q._w));
        return ret;
    }

    Vector<3> operator * (Vector<3> v)
    {
        return rotateVector(v);
    }

    Quaternion operator + (Quaternion q)
    {
        Quaternion ret;
        ret._w = _w + q._w;
        ret._x = _x + q._x;
        ret._y = _y + q._y;
        ret._z = _z + q._z;
        return ret;
    }

    Quaternion operator - (Quaternion q)
    {
        Quaternion ret;
        ret._w = _w - q._w;
        ret._x = _x - q._x;
        ret._y = _y - q._y;
        ret._z = _z - q._z;
        return ret;
    }

    Quaternion operator / (real_t scalar)
    {
        Quaternion ret;
        ret._w = this->_w/scalar;
        ret._x = this->_x/scalar;
        ret._y = this->_y/scalar;
        ret._z = this->_z/scalar;
        return ret;
    }

    Quaternion operator * (real_t scalar)
    {
        Quaternion ret;
        ret._w = this->_w*scalar;
        ret._x = this->_x*scalar;
        ret._y = this->_y*scalar;
        ret._z = this->_z*scalar;
        return ret;
    }

    Quaternion scale(real_t scalar)
    {
        Quaternion ret;
        ret._w = this->_w*scalar;
        ret._x = this->_x*scalar;
        ret._y = this->_y*scalar;
        ret._z = this->_z*scalar;
        return ret;
    }

    /*
     * only half of the hypersphere is needed to represent a rotation
     *
     * q(w,x,y,z) represents the same orientation as q(-w,-x,-y,-z)
     *
     * this function returns true if the quaternion is in the upper hyper-hemisphere
     *or false if it is in the lower hyper-hemisphere
     */
    bool get_hemisphere()
    {
        auto r = _w+_x+_y+_z;
        return (r >= 0);
    }

    /*
     * Spherical linear interpolation
     * Code is from: http://www.euclideanspace.com/maths/algebra/realNormedAlgebra/quaternions/slerp/
     *
     */
    Quaternion slerp(Quaternion b, float pc) const
    {
        // quaternion to return
        Quaternion qm;

        //b.copy_hemisphere(*this);

        // Calculate angle between them.
        real_t cosHalfTheta = _w * b._w + _x * b._x + _y * b._y + _z * b._z;
        if (cosHalfTheta < 0)
        {
            b._w = -b._w;
            b._x = -b._x;
            b._y = -b._y;
            b._z = -b._z;
            cosHalfTheta = -cosHalfTheta;
        }

        // if qa=qb or qa=-qb then theta = 0 and we can return qa
        if(fabs(cosHalfTheta) >= 1.0)
        {
            qm = *this;
            return qm;
        }
        // Calculate temporary values.
        real_t halfTheta = acos(cosHalfTheta);
        real_t sinHalfTheta = sqrt(1.0 - cosHalfTheta*cosHalfTheta);
        // if theta = 180 degrees then result is not fully defined
        // we could rotate around any axis normal to qa or qb
        if (fabs(sinHalfTheta) < 0.001)
        {   // fabs is floating point absolute
            qm._w = (_w * 0.5 + b._w * 0.5);
            qm._x = (_x * 0.5 + b._x * 0.5);
            qm._y = (_y * 0.5 + b._y * 0.5);
            qm._z = (_z * 0.5 + b._z * 0.5);
            return qm;
        }
        real_t ratioA = sin((1 - pc) * halfTheta) / sinHalfTheta;
        real_t ratioB = sin(pc * halfTheta) / sinHalfTheta;
        //calculate Quaternion.
        qm._w = (_w * ratioA + b._w * ratioB);
        qm._x = (_x * ratioA + b._x * ratioB);
        qm._y = (_y * ratioA + b._y * ratioB);
        qm._z = (_z * ratioA + b._z * ratioB);
        return qm;
    }

private:
    real_t _w, _x, _y, _z;
};


}

#endif
