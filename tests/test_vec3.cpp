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
