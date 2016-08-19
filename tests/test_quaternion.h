#include "kaztest/kaztest.h"
#include "../kazmath/quaternion.h"
#include "../kazmath/vec3.h"
#include "../kazmath/mat4.h"

class TestQuaternion : public TestCase {
public:
    void test_quaternion_multiply_vec3() {
        kmVec3 v;
        kmVec3Fill(&v, 0, 1, 0);

        kmQuaternion q;
        kmQuaternionRotationPitchYawRoll(&q, 0, 0, kmDegreesToRadians(90));

        kmQuaternionMultiplyVec3(&v, &q, &v);

        kmVec3 expected;
        kmVec3Fill(&expected, -1, 0, 0);

        assert_true(kmVec3AreEqual(&expected, &v));
    }

    void test_quaternion_slerp_edge_case() {
        kmQuaternion initial, dest;
        kmQuaternionFill(&initial, 0, 0, 0.230166689, 0.973177314);
        kmQuaternionFill(&dest, 0, 0, 0.233441412, 0.972370863);

        double step = 0.016666666666666666;

        kmQuaternion result;
        kmQuaternionSlerp(&result, &initial, &dest, step);

        assert_true(!isnan(result.x));
        assert_true(!isnan(result.y));
        assert_true(!isnan(result.z));
        assert_true(!isnan(result.w));
    }

    void test_quaternion_get_up_vector() {
        kmQuaternion q;
        kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_X, kmDegreesToRadians(90));

        kmVec3 up, right, forward;
        kmQuaternionGetUpVec3(&up, &q);
        kmQuaternionGetRightVec3(&right, &q);
        kmQuaternionGetForwardVec3RH(&forward, &q);

        assert_close(0.0, up.x, 0.0001);
        assert_close(0.0, up.y, 0.0001);
        assert_close(1.0, up.z, 0.0001);

        assert_close(1.0, right.x, 0.0001);
        assert_close(0.0, right.y, 0.0001);
        assert_close(0.0, right.z, 0.0001);

        assert_close(0.0, forward.x, 0.0001);
        assert_close(1.0, forward.y, 0.0001);
        assert_close(0.0, forward.z, 0.0001);

        kmMat4 check;
        kmMat4RotationQuaternion(&check, &q);

        kmVec3 mat_forward;
        kmMat4GetForwardVec3RH(&mat_forward, &check);

        assert_close(mat_forward.x, forward.x, 0.0001);
        assert_close(mat_forward.y, forward.y, 0.0001);
        assert_close(mat_forward.z, forward.z, 0.0001);
    }

    void test_rotation_around_axis() {
        kmQuaternion rot;
        kmQuaternionIdentity(&rot);

        kmVec3 up, right, forward;

        kmQuaternionGetUpVec3(&up, &rot);
        kmQuaternionGetRightVec3(&right, &rot);
        kmQuaternionGetForwardVec3RH(&forward, &rot);

        assert_true(kmVec3AreEqual(&up, &KM_VEC3_POS_Y));
        assert_true(kmVec3AreEqual(&right, &KM_VEC3_POS_X));
        assert_true(kmVec3AreEqual(&forward, &KM_VEC3_NEG_Z));

        kmQuaternion rot2;
        kmQuaternionRotationAxisAngle(&rot2, &KM_VEC3_POS_X, kmDegreesToRadians(90));

        assert_close(kmDegreesToRadians(90), kmQuaternionGetPitch(&rot2), 0.0001);

        kmQuaternion final;
        kmQuaternionMultiply(&final, &rot, &rot2);

        kmQuaternionGetUpVec3(&up, &final);
        kmQuaternionGetRightVec3(&right, &final);
        kmQuaternionGetForwardVec3RH(&forward, &final);

        assert_true(kmVec3AreEqual(&up, &KM_VEC3_POS_Z));
        assert_true(kmVec3AreEqual(&right, &KM_VEC3_POS_X));
        assert_true(kmVec3AreEqual(&forward, &KM_VEC3_POS_Y));

        kmQuaternionRotationAxisAngle(&rot2, &KM_VEC3_POS_X, kmDegreesToRadians(-90));
        assert_close(kmDegreesToRadians(-90), kmQuaternionGetPitch(&rot2), 0.0001);
    }

    void test_look_rotation() {
        kmQuaternion identity;
        kmQuaternionIdentity(&identity);

        kmVec3 zero;
        kmVec3Zero(&zero);

        kmQuaternion res;
        kmQuaternionLookRotation(&res, &zero, &KM_VEC3_POS_Y);

        //LookRotation on 0,0,0 should return an identity quaternion
        assert_true(kmQuaternionAreEqual(&identity, &res));

        kmQuaternionLookRotation(&res, &KM_VEC3_NEG_Z, &KM_VEC3_POS_Y);

        //LookRotation on 0,0,1 should return an identity quaternion
        assert_true(kmQuaternionAreEqual(&identity, &res));

        //90 degree rotation around the Y-Axis (e.g. negative X)
        kmQuaternion rot;
        kmQuaternionRotationAxisAngle(&rot, &KM_VEC3_POS_Y, -kmDegreesToRadians(90));

        assert_close(0.707, rot.w, 0.001);
        assert_close(-0.707, rot.y, 0.001);

        //Get the rotation from the negative X, it should be the same
        kmQuaternionLookRotation(&res, &KM_VEC3_NEG_X, &KM_VEC3_POS_Y);

        assert_true(kmQuaternionAreEqual(&rot, &res));

        kmQuaternionRotationAxisAngle(&rot, &KM_VEC3_POS_Y, kmDegreesToRadians(180));
        //Get the rotation from the positive Z, it should be the same
        kmQuaternionLookRotation(&res, &KM_VEC3_POS_Z, &KM_VEC3_POS_Y);
        assert_true(kmQuaternionAreEqual(&rot, &res));
    }

    void test_quaternion_axis_angle_conversion() {
        kmVec3 axis;
        kmScalar angle;
        kmVec3Fill(&axis, 0.0, 1.0, 0.0);
        angle = kmDegreesToRadians(50.0);

        kmQuaternion rotation;
        kmQuaternionRotationAxisAngle(&rotation, &axis, angle);

        kmVec3 final_axis;
        kmScalar final_angle;

        kmQuaternionToAxisAngle(&rotation, &final_axis, &final_angle);

        assert_close(angle, final_angle, kmEpsilon);
        assert_close(axis.x, final_axis.x, kmEpsilon);
        assert_close(axis.y, final_axis.y, kmEpsilon);
        assert_close(axis.z, final_axis.z, kmEpsilon);
    }

    void test_extract_rotation_around_axis() {
        kmQuaternion rotation, extracted;
        kmVec3 rot_axis, axis;

        kmVec3Fill(&rot_axis, 1.0, 1.0, 0.0);
        kmVec3Fill(&axis, 1.0, 0.0, 0.0);

        kmQuaternionRotationAxisAngle(&rotation, &rot_axis, kmDegreesToRadians((15.0)));
        kmQuaternionExtractRotationAroundAxis(&rotation, &axis, &extracted);

        kmVec3 final_axis;
        float final_angle;
        kmQuaternionToAxisAngle(&extracted, &final_axis, &final_angle);

        assert_close(kmDegreesToRadians(15), final_angle, 0.0001);
        assert_close(1.0, final_axis.x, kmEpsilon);
        assert_close(0.0, final_axis.y, kmEpsilon);
        assert_close(0.0, final_axis.z, kmEpsilon);
    }
};
