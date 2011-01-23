#include <assert.h>
#include <stdio.h>
#include "kazmath/ray2.h"

void kmRay2Fill(kmRay2* ray, kmScalar px, kmScalar py, kmScalar vx, kmScalar vy) {
    ray->start.x = px;
    ray->start.y = py;    
    ray->dir.x = vx;
    ray->dir.y = vy;
}

kmBool kmRay2IntersectLineSegment(const kmRay2* ray, const kmVec2 p1, const kmVec2 p2, kmVec2* intersection) {
    kmScalar A1, B1, C1;
    kmScalar A2, B2, C2;
    
    A1 = (ray->start.y + ray->dir.y) - (ray->start.y);
    B1 = (ray->start.x + ray->dir.x) - (ray->start.x);
    C1 = A1 * ray->start.x + B1 * ray->start.y;
    
    A2 = p2.y - p1.y;
    B2 = p2.x - p1.x;
    C2 = A2 * p1.x + B2 * p1.y;
    
    double det = (A1 * B2) - (A2 * B1);
    if(det == 0) {
        return KM_FALSE;
    }
    
    double x = (B2*C1 - B1*C2) / det;
    double y = (A1*C2 - A2*C1) / det;
    
    if(x < min(p1.x, p2.x) || x > max(p1.x, p2.x) ||
       y < min(p1.y, p2.y) || y > max(p1.y, p2.y)) {
        //Outside of line
        return KM_FALSE;
    }
    
    kmScalar x1 = ray->start.x, x2 = ray->start.x + ray->dir.x;
    kmScalar y1 = ray->start.y, y2 = ray->start.y + ray->dir.y;
    
    if(x < min(x1, x2) || x > max(x1, x2) ||
       y < min(y1, y2) || y > max(y1, y2)) {
        //Outside of ray
        return KM_FALSE;
    }
    
    intersection->x = x;
    intersection->y = y;
    
    return KM_TRUE;
}

kmBool kmRay2IntersectTriangle(const kmRay2* ray, const kmVec2 p1, const kmVec2 p2, const kmVec2 p3, kmVec2* intersection) {
    assert(0 && "Not implemented");
}

kmBool kmRay2IntersectCircle(const kmRay2* ray, const kmVec2 centre, const kmScalar radius, kmVec2* intersection) {
    assert(0 && "Not implemented");
}
