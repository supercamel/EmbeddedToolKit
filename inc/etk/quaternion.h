#ifndef IMUMATH_QUATERNION_HPP
#define IMUMATH_QUATERNION_HPP

#include <stdint.h>
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

    Quaternion(float iw, float ix, float iy, float iz)
    {
        _w = iw;
        _x = ix;
        _y = iy;
        _z = iz;
    }

    Quaternion(float w, Vector<3> vec)
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

    float& w()
    {
        return _w;
    }
    float& x()
    {
        return _x;
    }
    float& y()
    {
        return _y;
    }
    float& z()
    {
        return _z;
    }

    float magnitude()
    {
        float res = (_w*_w) + (_x*_x) + (_y*_y) + (_z*_z);
        return sqrtf(res);
    }

    void normalize()
    {
        float mag = magnitude();
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

    void fromAxisAngle(Vector<3> axis, float theta)
    {
        _w = cosf(theta/2.0f);
        //only need to calculate sine of half theta once
        float sht = sinf(theta/2.0f);
        _x = axis.x() * sht;
        _y = axis.y() * sht;
        _z = axis.z() * sht;
    }

    void toAxisAngle(Vector<3>& axis, float& angle)
    {
        normalize();
        float sqw = sqrtf(1.0-(_w*_w));
        if(compare<float>(sqw, 0.0f, 0.00000001f)) //it's a singularity and divide by zero, avoid
            return;

        angle = 2 * acosf(float(_w));
        axis.x() = _x / sqw;
        axis.y() = _y / sqw;
        axis.z() = _z / sqw;
    }


    void fromMatrix(Matrix<3, 3> m)
    {
		_w = sqrtf(max<float>( 0, 1 + m(0,0) + m(1,1) + m(2,2))) / 2.0f;
		_x = sqrtf(max<float>( 0, 1 + m(0,0) - m(1,1) - m(2,2))) / 2.0f;
		_y = sqrtf(max<float>( 0, 1 - m(0,0) + m(1,1) - m(2,2))) / 2.0f;
		_z = sqrtf(max<float>( 0, 1 - m(0,0) - m(1,1) + m(2,2))) / 2.0f;
		_x = copysign_zero(_x, m(2,1) - m(1,2));
		_y = copysign_zero(_y, m(0,2) - m(2,0));
		_z = copysign_zero(_z, m(1,0) - m(0,1));

/*
		//trace of matrix
        float tr = m(0, 0) + m(1, 1) + m(2, 2);

        float S = 0.0;
        if (tr > 0)
        {
            S = sqrtf(tr+1.0f) * 2;
            _w = float(0.25f) * S;
            _x = (m(2, 1) - m(1, 2)) / S;
            _y = (m(0, 2) - m(2, 0)) / S;
            _z = (m(1, 0) - m(0, 1)) / S;
        }
        else if ((m(0, 0) < m(1, 1))&(m(0, 0) < m(2, 2)))
        {
            S = sqrtf(float(1.0) + m(0, 0) - m(1, 1) - m(2, 2)) * 2;
            _w = (m(2, 1) - m(1, 2)) / S;
            _x = 0.25f * S;
            _y = (m(0, 1) + m(1, 0)) / S;
            _z = (m(0, 2) + m(2, 0)) / S;
        }
        else if (m(1, 1) < m(2, 2))
        {
            S = sqrtf(float(1.0) + m(1, 1) - m(0, 0) - m(2, 2)) * 2;
            _w = (m(0, 2) - m(2, 0)) / S;
            _x = (m(0, 1) + m(1, 0)) / S;
            _y = 0.25f * S;
            _z = (m(1, 2) + m(2, 1)) / S;
        }
        else
        {
            S = sqrtf(float(1.0) + m(2, 2) - m(0, 0) - m(1, 1)) * float(2);
            _w = (m(1, 0) - m(0, 1)) / S;
            _x = (m(0, 2) + m(2, 0)) / S;
            _y = (m(1, 2) + m(2, 1)) / S;
            _z = 0.25f * S;
        }
*/
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
        float sqw = _w*_w;
        float sqx = _x*_x;
        float sqy = _y*_y;
        float sqz = _z*_z;

        ret.x() = atan2f(float(float(2.0)*(_x*_y+_z*_w)),float(sqx-sqy-sqz+sqw));
        ret.y() = asinf(float(float(-2.0)*(_x*_z-_y*_w))/float(sqx+sqy+sqz+sqw));
        ret.z() = atan2f(float(float(2.0)*(_y*_z+_x*_w)),float(-sqx-sqy+sqz+sqw));

        return ret;
    }



    Vector<3> toAngularVelocity(float dt)
    {
        Vector<3> ret;
		if(dt == 0)
			return ret;
/*
        Quaternion one(1.0, 0.0, 0.0, 0.0);
        Quaternion delta = one - *this;
        Quaternion r = (delta/dt);
        r = r * 2;
        r = r * one;

        ret.x() = r.x();
        ret.y() = r.y();
        ret.z() = r.z();
*/

		float angle = 0;
		toAxisAngle(ret, angle);
		ret = ret*angle; //finds angular displacement
		ret = ret/dt; //over dt to find angular velocity

/*
		Vector<3> v(x(), y(), z());
		float s = w();
		ret = (v*2.0f)/(v.magnitude()*acos(s));


		Quaternion qdt = *this/dt;
		Quaternion r = (qdt*2)*this->conjugate();

		if(isnan(r.x()))
			return ret;
		if(isnan(r.y()))
			return ret;
		if(isnan(r.z()))
			return ret;
		ret.x() = r.x();
		ret.y() = r.y();
		ret.z() = r.z();
*/
        return ret;

    }

	void fromAngularVelocity(Vector<3> w, float dt)
	{
		float theta = w.magnitude() * dt;
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
        t = qv.cross(v) * float(2.0);
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

    Quaternion operator / (float scalar)
    {
        Quaternion ret;
        ret._w = this->_w/scalar;
        ret._x = this->_x/scalar;
        ret._y = this->_y/scalar;
        ret._z = this->_z/scalar;
        return ret;
    }

    Quaternion operator * (float scalar)
    {
        Quaternion ret;
        ret._w = this->_w*scalar;
        ret._x = this->_x*scalar;
        ret._y = this->_y*scalar;
        ret._z = this->_z*scalar;
        return ret;
    }

    Quaternion scale(float scalar)
    {
        Quaternion ret;
        ret._w = this->_w*scalar;
        ret._x = this->_x*scalar;
        ret._y = this->_y*scalar;
        ret._z = this->_z*scalar;
        return ret;
    }

private:
    float _w, _x, _y, _z;
};


}

#endif
