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

kmVec3* kmRay3IntersectPlane(kmVec3* pOut, const kmRay3* ray, const kmPlane* plane) {
    //t = - (A*org.x + B*org.y + C*org.z + D) / (A*dir.x + B*dir.y + C*dir.z )
    double t = -(plane->a * ray->start.x +
                 plane->b * ray->start.y +
                 plane->c * ray->start.z + plane->d) / (
                 plane->a * ray->dir.x +
                 plane->b * ray->dir.y +
                 plane->c * ray->dir.z);


    kmVec3 scaled_dir;
    kmVec3Scale(&scaled_dir, &ray->dir, t);
    kmVec3Add(pOut, &ray->start, &scaled_dir);
    return pOut;
}
