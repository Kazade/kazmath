#define BOOST_TEST_MODULE test_mat3

#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>
#include "mat3.h"
#include "vec3.h"
#include <cstdio>
void print_matrix3(const kmMat3* mat)
{
    const int max = 3;
    printf("\n\n");
    for(int i = 0; i < max; i++)
    {
        printf("|");

        for(int j = 0; j < max; j++)
        {
            if(j > 0)
            {
                printf("\t");
            }

            printf("%f",mat->mat[i + max*j]);
        }

        printf("|\n");
    }
    printf("\n");
}

BOOST_AUTO_TEST_CASE(test_mat3_inverse)
{
	kmMat3 mat;
	BOOST_CHECK(NULL != kmMat3Identity(&mat));

	kmMat3 adj;
	BOOST_CHECK(NULL != kmMat3Adjugate(&adj, &mat));
	BOOST_CHECK(NULL != kmMat3Inverse(&mat, kmMat3Determinant(&mat), &mat));
	BOOST_CHECK(kmMat3IsIdentity(&mat));
}

BOOST_AUTO_TEST_CASE(test_mat3_transpose)
{
	kmMat3 mat;
	float temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
	memcpy(mat.mat, temp, sizeof(float) * 9);

	kmMat3 transpose;
	float temp2[] = {0.0f, 3.0f, 6.0f, 1.0f, 4.0f, 7.0f, 2.0f, 5.0f, 8.0f };
	memcpy(transpose.mat, temp2, sizeof(float) * 9);

	kmMat3 result;
	BOOST_CHECK(NULL != kmMat3Transpose(&result, &mat));
	BOOST_CHECK(kmMat3AreEqual(&transpose, &result));

//	print_matrix3(&transpose);
//	print_matrix3(&result);
}

BOOST_AUTO_TEST_CASE(test_mat3_fill)
{
	float temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
	kmMat3 orig, filled;
	orig.mat[0] = 0.0f;
	orig.mat[1] = 1.0f;
	orig.mat[2] = 2.0f;
	orig.mat[3] = 3.0f;
	orig.mat[4] = 4.0f;
	orig.mat[5] = 5.0f;
	orig.mat[6] = 6.0f;
	orig.mat[7] = 7.0f;
	orig.mat[8] = 8.0f;	

	kmMat3Fill(&filled, temp);
	BOOST_CHECK(kmMat3AreEqual(&filled, &orig));
}

BOOST_AUTO_TEST_CASE(test_mat3_are_equal)
{
	kmMat3 test, different;
	kmMat3Identity(&test);
	kmMat3Identity(&different);
	BOOST_CHECK(kmMat3AreEqual(&test, &test));
	BOOST_CHECK(kmMat3AreEqual(&test, &different));
	different.mat[3] = 3.0f; //Arbitrary to make it different
	BOOST_CHECK(!kmMat3AreEqual(&test, &different));		
}

BOOST_AUTO_TEST_CASE(test_mat3_axis_angle)
{
	float radians = 1.0;
	kmMat3 a;
	kmVec3 axisIn;
	kmVec3Fill(&axisIn, 1.0f, 0.0f, 0.0f);
	kmMat3RotationAxisAngle(&a, &axisIn, radians);

	//TODO: Finish this
}

BOOST_AUTO_TEST_CASE(test_mat3_identity)
{
	float identity[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
	kmMat3 expected, actual;
	kmMat3Fill(&expected, identity);
	kmMat3Identity(&actual);

	BOOST_CHECK(kmMat3AreEqual(&expected, &actual));
}

BOOST_AUTO_TEST_CASE(test_mat3_is_identity)
{
	kmMat3 identity;
	kmMat3Identity(&identity);
	BOOST_CHECK(kmMat3IsIdentity(&identity));
	identity.mat[0] = 5.0f; //Arbitrary number
	BOOST_CHECK(!kmMat3IsIdentity(&identity));
}
