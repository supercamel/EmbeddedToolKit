#include "vector_test.h"
#include <etk/etk.h>
#include "out.h"

/*
#include <Eigen/Dense>
using Eigen::MatrixXd;
*/

#include <iostream>
using namespace std;
using namespace etk;


bool vector_test(std::string& subtest)
{
	subtest = "circular constraint";
	real_t vf = 450;
	vf = constrain_circular(vf, 360.0f);
	if(!compare(vf, 90.0f, 0.1f))
		return false;

	vf = -450;
	vf = constrain_circular(vf, 360.0f);
	if(!compare(vf, -90.0f, 0.1f))
		return false;

	subtest = "copy sign";
	if(etk::copysign(5, -5) != -5)
		return false;

	if(etk::copysign(5, 5) != 5)
		return false;

	if(etk::copysign(-5, 5) != 5)
		return false;

	if(etk::copysign(-5, -5) != -5)
		return false;

	if(etk::copysign_zero(5, 0) != 0)
		return false;

	subtest = "sub vector";

	etk::Vector<6> v;
	for(int i = 0; i < 6; i++)
		v[i] = i;

	etk::Vector<3> d = v.sub_vector<3>(3);

	if(d.x() != 3)
		return false;

	for(int i = 0; i < 3; i++)
		d[i] = i*3;

	subtest = "set sub vector";
	v.set_sub_vector<3>(d, 1);

	if(v.sub_vector<3>(1).y() != 3)
		return false;

	if(v.sub_vector<3>(1).z() != 6)
		return false;

	subtest = "comparison operator";
	if(!(v == v))
		return false;

	subtest = "not equal operator";
	if(v != v)
		return false;

	subtest = "normalized";
	etk::Vector<3> vv(5, 0, 0);
	d = vv.normalized();

	if(d.magnitude() != 1)
		return false;

	subtest = "set function";

	d.set(1.0f, 0.0f, 0.0f);


	subtest = "normal";

	for(uint32 i = 0; i < 100000; i++)
	{
		real_t x, y, z;
		x = ((real_t)rand()/(real_t)(RAND_MAX)) * 10.0f;
		y = ((real_t)rand()/(real_t)(RAND_MAX)) * 10.0f;
		z = ((real_t)rand()/(real_t)(RAND_MAX)) * 10.0f;
		/*
		MatrixXd em(3, 1);
	 	em << x, y, z;
		etk::Vector<3> vn(x, y, z);

		if(!compare<real_t>(vn.squaredNorm(), em.squaredNorm(), 0.1f))
			return false;
			*/
	}


	subtest = "cross product";
	vv.set(5, 0, 5);
	d.set(2, 2, 2);
	return true;
}


