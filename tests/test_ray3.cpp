#include <UnitTest++.h>

#include "../kazmath/ray3.h"
#include "../kazmath/plane.h"

TEST(test_ray_plane_intersection) {
    kmPlane p;
    kmPlaneFromNormalAndDistance(&p, &KM_VEC3_NEG_Z, 0);

    CHECK_CLOSE(-1.0, p.c, 0.0001);

    kmRay3 r;
    kmVec3 start;
    kmVec3Fill(&start, 0, 10, -10);
    kmRay3FromPointAndDirection(&r, &start, &KM_VEC3_POS_Z);

    kmVec3 result;
    kmRay3IntersectPlane(&result, &r, &p);

    CHECK_CLOSE(0.0, result.x, 0.0001);
    CHECK_CLOSE(10.0, result.y, 0.0001);
    CHECK_CLOSE(0.0, result.z, 0.0001);


    //Now move the ray behind the origin and along the x-axis
    //and fire it forward into the plane.
    kmVec3Assign(&r.dir, &KM_VEC3_NEG_Z);

    start.x = 10;
    start.z = 10;
    kmVec3Assign(&r.start, &start);

    kmRay3IntersectPlane(&result, &r, &p);

    CHECK_CLOSE(10.0, result.x, 0.0001);
    CHECK_CLOSE(10.0, result.y, 0.0001);
    CHECK_CLOSE(0.0, result.z, 0.0001);
}

TEST(test_ray_triangle_intersection) {
    kmRay3 ray;
    kmRay3Fill(&ray, 0, 0, 0, 0, -1, 0);

    kmVec3 v0, v1, v2;
    kmVec3Fill(&v0, -1, -1, -1);
    kmVec3Fill(&v1, 0, -1, 1);
    kmVec3Fill(&v2, 1, -1, -1);

    kmScalar dist;
    kmVec3 intersect;
    kmVec3 normal;

    kmBool ret = kmRay3IntersectTriangle(&ray, &v0, &v1, &v2, &intersect, &normal, &dist);

    CHECK(ret);
    CHECK_CLOSE(1.0, dist, 0.001);
    CHECK_CLOSE(0, intersect.x, 0.001);
    CHECK_CLOSE(-1, intersect.y, 0.001);
    CHECK_CLOSE(0, intersect.z, 0.001);
    CHECK_CLOSE(0, normal.x, 0.001);
    CHECK_CLOSE(1, normal.y, 0.001);
    CHECK_CLOSE(0, normal.z, 0.001);

    // Shorten the ray
    ray.dir.y = -0.5;
    ret = kmRay3IntersectTriangle(&ray, &v0, &v1, &v2, &intersect, &normal, &dist);
    CHECK(!ret);

    // Now flip the ray
    ray.dir.y = 1;
    ret = kmRay3IntersectTriangle(&ray, &v0, &v1, &v2, &intersect, &normal, &dist);
    CHECK(!ret);

    ray.start.x = 0;
    ray.start.y = 0.01;
    ray.start.z = 0;

    ray.dir.x = 0;
    ray.dir.y = -0.3;
    ray.dir.z = 0;

    v0.y = 0;
    v1.y = 0;
    v2.y = 0;

    ret = kmRay3IntersectTriangle(&ray, &v0, &v1, &v2, &intersect, &normal, &dist);
    CHECK(ret);
}
