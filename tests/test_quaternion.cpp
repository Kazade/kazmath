#include <UnitTest++.h>
#include "../kazmath/quaternion.h"
#include "../kazmath/vec3.h"

TEST(test_quaternion_multiply_vec3) {
    kmVec3 v;
    kmVec3Fill(&v, 0, 1, 0);
    
    kmQuaternion q;
    kmQuaternionRotationPitchYawRoll(&q, 0, 0, kmDegreesToRadians(90));
    
    kmQuaternionMultiplyVec3(&v, &q, &v);
    
    kmVec3 expected;
    kmVec3Fill(&expected, 1, 0, 0);
    
    CHECK(kmVec3AreEqual(&expected, &v));
}


