#define BOOST_TEST_MODULE test_mat4

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include "mat4.h"

/*
BOOST_AUTO_TEST_CASE(test_mat4_inverse)
{
	kmMat4 mat;
	BOOST_CHECK(NULL != kmMat4Identity(&mat));

	kmMat4 adj;
	BOOST_CHECK(NULL != kmMat4Adjugate(&adj, &mat));
	BOOST_CHECK(NULL != kmMat4Inverse(&mat, kmMat4Determinant(&mat), &mat));
	BOOST_CHECK(kmMat4IsIdentity(&mat));
}*/


