#include <UnitTest++.h>

#include "../kazmath/ray3.h"
#include "../kazmath/plane.h"

TEST(test_ray_plane_intersection) {
    kmPlane p;
    kmPlaneFromNormalAndDistance(&p, &KM_VEC3_FORWARD, 0);

    CHECK_CLOSE(1.0, p.c, 0.0001);

    kmRay3 r;
    kmVec3 start;
    kmVec3Fill(&start, 0, 10, 10);
    kmRay3FromPointAndDirection(&r, &start, &KM_VEC3_BACKWARD);

    kmVec3 result;
    kmRay3IntersectPlane(&result, &r, &p);

    CHECK_CLOSE(0.0, result.x, 0.0001);
    CHECK_CLOSE(10.0, result.y, 0.0001);
    CHECK_CLOSE(0.0, result.z, 0.0001);


    //Now move the ray behind the origin and along the x-axis
    //and fire it forward into the plane.
    kmVec3Assign(&r.dir, &KM_VEC3_FORWARD);

    start.x = 10;
    start.z = 10;
    kmVec3Assign(&r.start, &start);

    kmRay3IntersectPlane(&result, &r, &p);

    CHECK_CLOSE(10.0, result.x, 0.0001);
    CHECK_CLOSE(10.0, result.y, 0.0001);
    CHECK_CLOSE(0.0, result.z, 0.0001);
}
