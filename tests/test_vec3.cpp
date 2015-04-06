#include <UnitTest++.h>

#include "../kazmath/vec3.h"
#include "../kazmath/plane.h"

TEST(test_project_onto_plane) {

    kmPlane xy;
    kmPlaneFill(&xy, 0, 0, 1, 0);

    kmVec3 point;
    kmVec3Fill(&point, 0, 10, 10);

    kmVec3 result;
    kmVec3ProjectOnToPlane(&result, &point, &xy);

    CHECK_CLOSE(0.0, result.x, 0.0001);
    CHECK_CLOSE(10.0, result.y, 0.0001);
    CHECK_CLOSE(0.0, result.z, 0.0001);
}

TEST(test_vec3_reflect) {
    kmVec3 incident;
    kmVec3Fill(&incident, 7.0f, -5.0f, 3.0f);

    kmVec3 reflected;
    kmVec3Reflect(&reflected, &incident, &KM_VEC3_POS_Y);

    CHECK_CLOSE(7.0f, reflected.x, 0.001f);
    CHECK_CLOSE(5.0f, reflected.y, 0.001f);
    CHECK_CLOSE(3.0f, reflected.z, 0.001f);
}
