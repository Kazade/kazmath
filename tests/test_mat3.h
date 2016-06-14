#include <cstdio>
#include <cstring>
#include "kaztest/kaztest.h"

#include "../kazmath/mat3.h"
#include "../kazmath/vec3.h"

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

void print_vector3(const kmVec3* vec)
{
	printf("\n\n%f\t%f\t%f\n",vec->x, vec->y, vec->z);
}

class TestMat3 : public TestCase {
public:
    void test_mat3_inverse() {
        kmMat3 mat;
        assert_true(NULL != kmMat3Identity(&mat));

        kmMat3 adj;
        assert_true(NULL != kmMat3Adjugate(&adj, &mat));
        assert_true(NULL != kmMat3Inverse(&mat, &mat));
        assert_true(kmMat3IsIdentity(&mat));
    }

    void test_mat3_transpose() {
        kmMat3 mat;
        kmScalar temp[] = { 1.0f, 3.0f, 5.0f, 2.0f, 4.0f, 6.0f, 8.0f, 3.0f, 1.0f };
        memcpy(mat.mat, temp, sizeof(kmScalar) * 9);

        kmMat3 transpose;
        kmScalar temp2[] = {1.0f, 2.0f, 8.0f, 3.0f, 4.0f, 3.0f, 5.0f, 6.0f, 1.0f };
        memcpy(transpose.mat, temp2, sizeof(kmScalar) * 9);

        kmMat3 result;
        assert_true(NULL != kmMat3Transpose(&result, &mat));
        assert_true(kmMat3AreEqual(&transpose, &result));
    }

    void test_mat3_fill() {
        kmScalar temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f };
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
        assert_true(kmMat3AreEqual(&filled, &orig));
    }

    void test_mat3_are_equal() {
        kmMat3 test, different;
        kmMat3Identity(&test);
        kmMat3Identity(&different);
        assert_true(kmMat3AreEqual(&test, &test));
        assert_true(kmMat3AreEqual(&test, &different));
        different.mat[3] = 3.0f; //Arbitrary to make it different
        assert_true(!kmMat3AreEqual(&test, &different));
    }

    void test_mat3_axis_angle() {
        kmScalar radians = 1.0;
        kmScalar radiansOut;
        kmMat3 a;
        kmVec3 axisIn, axisOut;
        kmVec3Fill(&axisIn, 1.0f, 0.0f, 0.0f);
        kmMat3FromRotationAxisAngle(&a, &axisIn, radians);

        //TODO: Check a is what we expect

        kmMat3ExtractRotationAxisAngle(&a, &axisOut, &radiansOut);

        assert_true(kmVec3AreEqual(&axisIn, &axisOut));
        assert_true(radians == radiansOut);
    }

    void test_mat3_identity() {
        kmScalar identity[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f };
        kmMat3 expected, actual;
        kmMat3Fill(&expected, identity);
        kmMat3Identity(&actual);

        assert_true(kmMat3AreEqual(&expected, &actual));
    }

    void test_mat3_is_identity() {
        kmMat3 identity;
        kmMat3Identity(&identity);
        assert_true(kmMat3IsIdentity(&identity));
        identity.mat[0] = 5.0f; //Arbitrary number
        assert_true(!kmMat3IsIdentity(&identity));
    }

    void test_mat3_scaling() {
        kmMat3 expected, actual;
        kmMat3Identity(&expected);
        expected.mat[0] = 1.0f;
        expected.mat[4] = 2.0f;

        kmMat3FromScaling(&actual, 1.0f, 2.0f);

        assert_true(kmMat3AreEqual(&expected, &actual));
    }

    void test_mat3_translation() {
        kmMat3 expected, actual;
        kmMat3Identity(&expected);
        expected.mat[6] = 1.0f;
        expected.mat[7] = 2.0f;

        kmMat3FromTranslation(&actual, 1.0f, 2.0f);

        assert_true(kmMat3AreEqual(&expected, &actual));
    }
};

