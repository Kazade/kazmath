#include <UnitTest++.h>
#include "../kazmath/quaternion.h"
#include "../kazmath/vec3.h"
#include "../kazmath/mat4.h"

TEST(test_quaternion_multiply_vec3) {
    kmVec3 v;
    kmVec3Fill(&v, 0, 1, 0);
    
    kmQuaternion q;
    kmQuaternionRotationPitchYawRoll(&q, 0, 0, kmDegreesToRadians(90));
    
    kmQuaternionMultiplyVec3(&v, &q, &v);
    
    kmVec3 expected;
    kmVec3Fill(&expected, -1, 0, 0);
    
    CHECK(kmVec3AreEqual(&expected, &v));
}

TEST(test_quaternion_slerp_edge_case) {
    kmQuaternion initial, dest;
    kmQuaternionFill(&initial, 0, 0, 0.230166689, 0.973177314);
    kmQuaternionFill(&dest, 0, 0, 0.233441412, 0.972370863);

    double step = 0.016666666666666666;

    kmQuaternion result;
    kmQuaternionSlerp(&result, &initial, &dest, step);

    CHECK(!isnan(result.x));
    CHECK(!isnan(result.y));
    CHECK(!isnan(result.z));
    CHECK(!isnan(result.w));
}

TEST(test_quaternion_get_up_vector) {
    kmQuaternion q;
    kmQuaternionRotationAxisAngle(&q, &KM_VEC3_POS_X, kmDegreesToRadians(90));

    kmVec3 up, right, forward;
    kmQuaternionGetUpVec3(&up, &q);
    kmQuaternionGetRightVec3(&right, &q);
    kmQuaternionGetForwardVec3RH(&forward, &q);

    CHECK_CLOSE(0.0, up.x, 0.0001);
    CHECK_CLOSE(0.0, up.y, 0.0001);
    CHECK_CLOSE(1.0, up.z, 0.0001);

    CHECK_CLOSE(1.0, right.x, 0.0001);
    CHECK_CLOSE(0.0, right.y, 0.0001);
    CHECK_CLOSE(0.0, right.z, 0.0001);

    CHECK_CLOSE(0.0, forward.x, 0.0001);
    CHECK_CLOSE(1.0, forward.y, 0.0001);
    CHECK_CLOSE(0.0, forward.z, 0.0001);

    kmMat4 check;
    kmMat4RotationQuaternion(&check, &q);

    kmVec3 mat_forward;
    kmMat4GetForwardVec3RH(&mat_forward, &check);

    CHECK_CLOSE(mat_forward.x, forward.x, 0.0001);
    CHECK_CLOSE(mat_forward.y, forward.y, 0.0001);
    CHECK_CLOSE(mat_forward.z, forward.z, 0.0001);
}

TEST(test_rotation_around_axis) {
    kmQuaternion rot;
    kmQuaternionIdentity(&rot);

    kmVec3 up, right, forward;

    kmQuaternionGetUpVec3(&up, &rot);
    kmQuaternionGetRightVec3(&right, &rot);
    kmQuaternionGetForwardVec3RH(&forward, &rot);

    CHECK(kmVec3AreEqual(&up, &KM_VEC3_POS_Y));
    CHECK(kmVec3AreEqual(&right, &KM_VEC3_POS_X));
    CHECK(kmVec3AreEqual(&forward, &KM_VEC3_NEG_Z));

    kmQuaternion rot2;
    kmQuaternionRotationAxisAngle(&rot2, &KM_VEC3_POS_X, kmDegreesToRadians(90));

    CHECK_CLOSE(kmDegreesToRadians(90), kmQuaternionGetPitch(&rot2), 0.0001);

    kmQuaternion final;
    kmQuaternionMultiply(&final, &rot, &rot2);

    kmQuaternionGetUpVec3(&up, &final);
    kmQuaternionGetRightVec3(&right, &final);
    kmQuaternionGetForwardVec3RH(&forward, &final);

    CHECK(kmVec3AreEqual(&up, &KM_VEC3_POS_Z));
    CHECK(kmVec3AreEqual(&right, &KM_VEC3_POS_X));
    CHECK(kmVec3AreEqual(&forward, &KM_VEC3_POS_Y));

    kmQuaternionRotationAxisAngle(&rot2, &KM_VEC3_POS_X, kmDegreesToRadians(-90));
    CHECK_CLOSE(kmDegreesToRadians(-90), kmQuaternionGetPitch(&rot2), 0.0001);
}

TEST(test_look_rotation) {
    kmQuaternion identity;
    kmQuaternionIdentity(&identity);

    kmVec3 zero;
    kmVec3Zero(&zero);

    kmQuaternion res;
    kmQuaternionLookRotation(&res, &zero, &KM_VEC3_POS_Y);

    //LookRotation on 0,0,0 should return an identity quaternion
    CHECK(kmQuaternionAreEqual(&identity, &res));

    kmQuaternionLookRotation(&res, &KM_VEC3_NEG_Z, &KM_VEC3_POS_Y);

    //LookRotation on 0,0,1 should return an identity quaternion
    CHECK(kmQuaternionAreEqual(&identity, &res));

    //90 degree rotation around the Y-Axis (e.g. negative X)
    kmQuaternion rot;
    kmQuaternionRotationAxisAngle(&rot, &KM_VEC3_POS_Y, kmDegreesToRadians(90));

    //Get the rotation from the negative X, it should be the same
    kmQuaternionLookRotation(&res, &KM_VEC3_NEG_X, &KM_VEC3_POS_Y);

    CHECK(kmQuaternionAreEqual(&rot, &res));

    kmQuaternionRotationAxisAngle(&rot, &KM_VEC3_POS_Y, kmDegreesToRadians(180));
    //Get the rotation from the positive Z, it should be the same
    kmQuaternionLookRotation(&res, &KM_VEC3_POS_Z, &KM_VEC3_POS_Y);
    CHECK(kmQuaternionAreEqual(&rot, &res));
}
