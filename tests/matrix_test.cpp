#include "matrix_test.h"
#include "out.h"
#include <etk/matrix.h>
using namespace etk;


#include <iostream>
using namespace std;

/*
#include <Eigen/Dense>
using Eigen::MatrixXd;

  */

bool matrix_addition()
{
	Matrix<2,3> a(5,4,3,2,1,0);
	Matrix<2,3> b(5,6,7,8,9,10);
	Matrix<2,3> c;

	c = a + b;

	for(uint32 x = 0; x < 2; x++)
	{
		for(uint32 y = 0; y < 3; y++)
		{
			if(c(x,y) != 10)
				return false;
		}
	}

	a.set(5,6,7,8,9,10);
	b.set(5,6,7,8,9,10);
	c = a-b;
	for(uint32 x = 0; x < 2; x++)
	{
		for(uint32 y = 0; y < 3; y++)
		{
			if(c(x,y) != 0)
				return false;
		}
	}

	return true;
}

bool matrix_rows_and_columns()
{
    /*
	Matrix<2,3> a(5,4,3,2,1,0);

	MatrixXd b(2, 3);
	b << 5,4,3,2,1,0;

	for(uint32 x = 0; x < 2; x++)
	{
		for(uint32 y = 0; y < 3; y++)
		{
			if(a(x,y) != b(x,y))
				return false;
		}
	}
	*/
	return true;
}



bool positive_definite()
{
/*
	Matrix<3,3> m(25.0f,15.0f,-5.0f,15.0f,18.0f,0.0f,-5.0f,0.0f,11.0f);

	out << m << "\n";

	out << m.llt() << "\n";

	MatrixXd em(3, 3);
 	em << 25,15,-5,15,18,0,-5,0,11;

	MatrixXd lt(3, 3);
	lt = em.llt().matrixL();

	cout << std::endl << em << std::endl;
	cout << lt << std::endl;


	m.set(5.0f,15.0f,-5.0f,15.0f,18.0f,0.0f,-5.0f,0.0f,11.0f);

	out << m.llt() << "\n";

 	em << 5,15,-5,15,18,0,-5,0,11;

	lt = em.llt().matrixL();

	cout << lt << std::endl;
	*/
	return true;
}


bool determinant_test()
{
/*
	using namespace etk;
	Matrix<3,3> m;
*/
	return true;
}

bool matrix_test(std::string& subtest)
{
	subtest = "sub vector";

	etk::Matrix<3,3> v;
	v.set(0, 1, 2, 3, 4, 5, 6);

	etk::Vector<3> d = v.sub_vector<3>(3);

	if(d.x() != 3)
		return false;

	etk::Vector<16> big_vec;
	etk::Matrix<3,3> small_mat(big_vec.sub_vector<9>(6));

	subtest = "Matrix addition";
	if(!matrix_addition())
		return false;

	subtest = "Rows and columns";
	if(!matrix_rows_and_columns())
		return false;

	subtest = "positive definite";
	if(!positive_definite())
		return false;

	subtest = "determinant";
	if(!determinant_test())
		return false;

	return true;
}


