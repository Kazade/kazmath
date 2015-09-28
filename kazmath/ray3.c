#include "plane.h"
#include "ray3.h"

kmRay3* kmRay3Fill(kmRay3* ray, kmScalar px, kmScalar py, kmScalar pz, kmScalar vx, kmScalar vy, kmScalar vz) {
    ray->start.x = px;
    ray->start.y = py;
    ray->start.z = pz;

    ray->dir.x = vx;
    ray->dir.y = vy;
    ray->dir.z = vz;

    return ray;
}

kmRay3* kmRay3FromPointAndDirection(kmRay3* ray, const kmVec3* point, const kmVec3* direction) {
    kmVec3Assign(&ray->start, point);
    kmVec3Assign(&ray->dir, direction);
    return ray;
}

kmBool kmRay3IntersectPlane(kmVec3* pOut, const kmRay3* ray, const kmPlane* plane) {
    /*t = - (A*org.x + B*org.y + C*org.z + D) / (A*dir.x + B*dir.y + C*dir.z )*/

    kmScalar d = (plane->a * ray->dir.x +
                  plane->b * ray->dir.y +
                  plane->c * ray->dir.z);

    if(d == 0)
    {
      return KM_FALSE;
    }

    kmScalar t = -(plane->a * ray->start.x +
                   plane->b * ray->start.y +
                   plane->c * ray->start.z + plane->d) / d;

    if(t < 0)
    {
      return KM_FALSE;
    }

    kmVec3 scaled_dir;
    kmVec3Scale(&scaled_dir, &ray->dir, t);
    kmVec3Add(pOut, &ray->start, &scaled_dir);
    return KM_TRUE;
}


kmBool kmRay3IntersectTriangle(const kmRay3* ray, const kmVec3* v0, const kmVec3* v1, const kmVec3* v2, kmVec3* intersection, kmVec3* normal, kmScalar* distance) {
    kmVec3 e1, e2, pvec, tvec, qvec, dir;
    kmScalar det, inv_det, u, v, t;

    kmVec3Normalize(&dir, &ray->dir);

    kmVec3Subtract(&e1, v1, v0);
    kmVec3Subtract(&e2, v2, v0);

    kmVec3Cross(&pvec, &dir, &e2);
    det = kmVec3Dot(&e1, &pvec);

    // Backfacing, discard.
    if(det < kmEpsilon) {
        return KM_FALSE;
    }

    if(kmAlmostEqual(det, 0)) {
        return KM_FALSE;
    }

    inv_det = 1.0 / det;

    kmVec3Subtract(&tvec, &ray->start, v0);

    u = inv_det * kmVec3Dot(&tvec, &pvec);
    if(u < 0.0 || u > 1.0) {
        return KM_FALSE;
    }

    kmVec3Cross(&qvec, &tvec, &e1);
    v = inv_det * kmVec3Dot(&dir, &qvec);
    if(v < 0.0 || (u + v) > 1.0) {
        return KM_FALSE;
    }

    t = inv_det * kmVec3Dot(&e2, &qvec);
    if(t > kmEpsilon && (t*t) <= kmVec3LengthSq(&ray->dir)) {
        *distance = t; // Distance
        kmVec3Cross(normal, &e1, &e2); //Surface normal of collision
        kmVec3Normalize(normal, normal);
        kmVec3 scaled;
        kmVec3Normalize(&scaled, &dir);
        kmVec3Scale(&scaled, &scaled, *distance);
        kmVec3Add(intersection, &ray->start, &scaled);
        return KM_TRUE;
    }

    return KM_FALSE;
}
