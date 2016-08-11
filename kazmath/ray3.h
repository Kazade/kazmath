#ifndef RAY3_H
#define RAY3_H

#include "utility.h"
#include "vec3.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct kmRay3 {
    kmVec3 start;
    kmVec3 dir;
} kmRay3;

struct kmPlane;
struct kmAABB3;

kmRay3* kmRay3Fill(kmRay3* ray, kmScalar px, kmScalar py, kmScalar pz, kmScalar vx, kmScalar vy, kmScalar vz);
kmRay3* kmRay3FromPointAndDirection(kmRay3* ray, const kmVec3* point, const kmVec3* direction);
kmBool kmRay3IntersectPlane(kmVec3* pOut, const kmRay3* ray, const struct kmPlane* plane);
kmBool kmRay3IntersectTriangle(const kmRay3* ray, const kmVec3* v0, const kmVec3* v1, const kmVec3* v2, kmVec3* intersection, kmVec3* normal, kmScalar* distance);
kmBool kmRay3IntersectAABB3(const kmRay3* ray, const struct kmAABB3* aabb, kmVec3* intersection, kmScalar* distance);

#ifdef __cplusplus
}
#endif

#endif /* RAY3_H */
