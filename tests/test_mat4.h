#include <cstdlib>
#include <cstdio>
#include <memory.h>
#include "kaztest/kaztest.h"

#include "../kazmath/mat4.h"
#include "../kazmath/quaternion.h"
#include "../kazmath/vec3.h"
#include "../kazmath/mat3.h"

void print_matrix4(const kmMat4* mat)
{
    const int max = 4;
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

class TestMat4 : public TestCase {
public:
    void test_mat4_inverse() {
        kmMat4 mat;
        assert_true(NULL != kmMat4Identity(&mat));

        assert_true(NULL != kmMat4Inverse(&mat,&mat));
        assert_true(kmMat4IsIdentity(&mat));
    }

    void test_mat4_and_quaternion_consistency() {
        kmMat4 mat;
        assert_true(NULL != kmMat4Identity(&mat));

        kmQuaternion quat;
        kmQuaternionIdentity(&quat);

        kmMat4RotationQuaternion(&mat, &quat);
        assert_true(kmMat4IsIdentity(&mat));

        kmQuaternionRotationAxisAngle(&quat, &KM_VEC3_POS_Y, kmDegreesToRadians(90));
        kmMat4RotationAxisAngle(&mat, &KM_VEC3_POS_Y, kmDegreesToRadians(90));

        kmMat4 tmp;
        kmMat4RotationQuaternion(&tmp, &quat);

        assert_true(kmMat4AreEqual(&tmp, &mat));
    }


    void test_mat4_transpose() {
        kmMat4 mat;
        float temp[] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f, 7.0f, 8.0f, 9.0f, 10.0f, 11.0f, 12.0f, 13.0f, 14.0f, 15.0f };
        memcpy(mat.mat, temp, sizeof(float) * 16);

        kmMat4 transpose;
        float temp2[] = {0.0f, 4.0f, 8.0f, 12.0f, 1.0f, 5.0f, 9.0f, 13.0f, 2.0f, 6.0f, 10.0f, 14.0f, 3.0f, 7.0f, 11.0f, 15.0f };
        memcpy(transpose.mat, temp2, sizeof(float) * 16);

        kmMat4 result;
        assert_true(NULL != kmMat4Transpose(&result, &mat));
        assert_true(kmMat4AreEqual(&transpose, &result));
    }

    void test_mat4_rotation_y() {
        kmQuaternion q;
        kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_X, kmDegreesToRadians(90));

        kmMat4 quaternion_rotated;
        kmMat4RotationQuaternion(&quaternion_rotated, &q);

        kmMat4 initialized;
        kmMat4RotationYawPitchRoll(&initialized, kmDegreesToRadians(90), 0, 0);

        assert_true(kmMat4AreEqual(&initialized, &quaternion_rotated));

        kmVec3 mat_forward, quat_forward;
        kmMat4GetForwardVec3RH(&mat_forward, &initialized);
        kmQuaternionGetForwardVec3RH(&quat_forward, &q);

        assert_true(kmVec3AreEqual(&mat_forward, &quat_forward));
    }

    void test_mat4_rotation_z() {
        kmQuaternion q;
        kmMat4 initialized;
        kmMat4 quaternion_rotated;

        kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_Z, kmDegreesToRadians(90));
        kmMat4RotationAxisAngle(&initialized, &KM_VEC3_POS_Z, kmDegreesToRadians(90));

        kmMat4RotationQuaternion(&quaternion_rotated, &q);

        kmVec3 initialized_forward;
        kmVec3 rotated_forward;
        kmMat4GetForwardVec3RH(&initialized_forward, &quaternion_rotated);
        kmMat4GetForwardVec3RH(&rotated_forward, &initialized);

        kmVec3 initialized_up;
        kmVec3 rotated_up;
        kmMat4GetUpVec3(&initialized_up, &quaternion_rotated);
        kmMat4GetUpVec3(&rotated_up, &initialized);

        assert_true(kmVec3AreEqual(&initialized_up, &rotated_up));
        assert_true(kmMat4AreEqual(&initialized, &quaternion_rotated));

        kmVec3 mat_right, quat_right;
        kmMat4GetRightVec3(&mat_right, &initialized);
        kmQuaternionGetRightVec3(&quat_right, &q);

        kmQuaternion from_matrix;
        kmMat3 rot;
        kmMat4ExtractRotationMat3(&initialized, &rot);
        kmQuaternionRotationMatrix(&from_matrix, &rot);

        assert_true(kmQuaternionAreEqual(&q, &from_matrix));
        assert_true(kmVec3AreEqual(&mat_right, &quat_right));
    }

    void test_mat4_handedness() {
        kmMat4 m;
        kmMat4Identity(&m);

        kmVec3 forward;
        kmMat4GetForwardVec3RH(&forward, &m);

        assert_close(0, forward.x, 0.0001);
        assert_close(0, forward.y, 0.0001);
        assert_close(-1, forward.z, 0.0001);

        kmVec3 right;
        kmMat4GetRightVec3(&right, &m);

        assert_close(1, right.x, 0.0001);
        assert_close(0, right.y, 0.0001);
        assert_close(0, right.z, 0.0001);

        kmVec3 up;
        kmMat4GetUpVec3(&up, &m);

        assert_close(0, up.x, 0.0001);
        assert_close(1, up.y, 0.0001);
        assert_close(0, up.z, 0.0001);
    }

};
