#include <assert.h>
#include <stdio.h>
#include "ray2.h"

void kmRay2Fill(kmRay2* ray, kmScalar px, kmScalar py, kmScalar vx, kmScalar vy) {
    ray->start.x = px;
    ray->start.y = py;    
    ray->dir.x = vx;
    ray->dir.y = vy;
}

void kmRay2FillWithEndpoints( kmRay2 *ray, const kmVec2 *start, const kmVec2 *end ) {
    ray->start.x = start->x; 
    ray->start.y = start->y; 
    ray->dir.x = end->x - start->x; 
    ray->dir.y = end->y - start->y; 
}
 

kmBool kmLine2WithLineIntersection(const kmVec2 *ptA, const kmVec2 *vecA, /* first line */
                                   const kmVec2 *ptB, const kmVec2 *vecB, /* seconf line */
                                   kmScalar *outTA, kmScalar *outTB,
                                   kmVec2 *outIntersection )
{
    kmScalar x1 = ptA->x;
    kmScalar y1 = ptA->y;
    kmScalar x2 = x1 + vecA->x;
    kmScalar y2 = y1 + vecA->y;
    kmScalar x3 = ptB->x;
    kmScalar y3 = ptB->y;
    kmScalar x4 = x3 + vecB->x;
    kmScalar y4 = y3 + vecB->y;

    kmScalar denom = (y4 -y3) * (x2 - x1) - (x4 - x3) * (y2 - y1);

    kmScalar ua;
    kmScalar ub;
    
    kmScalar x;
    kmScalar y;

    
    /*If denom is zero, the lines are parallel*/
    if(denom > -kmEpsilon && denom < kmEpsilon) {
        return KM_FALSE;
    }
    
    ua = ((x4 - x3) * (y1 - y3) - (y4 - y3) * (x1 - x3)) / denom;
    ub = ((x2 - x1) * (y1 - y3) - (y2 - y1) * (x1 - x3)) / denom;
    
    x = x1 + ua * (x2 - x1);
    y = y1 + ua * (y2 - y1);
    
    if( outTA ){ 
        *outTA = ua;
    }
    if( outTB ){ 
        *outTB = ub; 
    }
    if( outIntersection ){
        outIntersection->x = x;
        outIntersection->y = y; 
    }
    return KM_TRUE;
} 

kmBool kmSegment2WithSegmentIntersection( const kmRay2 *segmentA, const kmRay2 *segmentB, kmVec2 *intersection )
{
    kmScalar ua;
    kmScalar ub;
    kmVec2   pt; 

    if( kmLine2WithLineIntersection( &(segmentA->start), &(segmentA->dir), 
                                    &(segmentB->start), &(segmentB->start),
                                    &ua, &ub, &pt ) && 
        (0.0 <= ua) && (ua <= 1.0) && (0.0 <= ub) && (ub <= 1.0)) {
        intersection->x = pt.x;
        intersection->y = pt.y;
        return KM_TRUE;    
    }

    return KM_FALSE;        
} 

kmBool kmRay2IntersectLineSegment(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, kmVec2* intersection) {
    
    kmScalar ua;
    kmScalar ub;
    kmVec2   pt; 

    kmRay2   otherSegment; 
    kmRay2FillWithEndpoints(&otherSegment, p1, p2); 

    if( kmLine2WithLineIntersection( &(ray->start), &(ray->dir), 
                                     &(otherSegment.start), &(otherSegment.dir),
                                     &ua, &ub, &pt ) && 
        (0.0 <= ua) && (0.0 <= ub) && (ub <= 1.0)) {
        
        intersection->x = pt.x;
        intersection->y = pt.y;
        return KM_TRUE;    
    }

    return KM_FALSE;        
}

void calculate_line_normal(kmVec2 p1, kmVec2 p2, kmVec2 other_point, kmVec2* normal_out) {
    /*
        A = (3,4)
        B = (2,1)
        C = (1,3)

        AB = (2,1) - (3,4) = (-1,-3)
        AC = (1,3) - (3,4) = (-2,-1)
        N = n(AB) = (-3,1)
        D = dot(N,AC) = 6 + -1 = 5

        since D > 0:
          N = -N = (3,-1)
    */
    
    kmVec2 edge, other_edge;
    kmScalar d;
    kmVec2 n;

    kmVec2Subtract(&edge, &p2, &p1);
    kmVec2Subtract(&other_edge, &other_point, &p1);
    kmVec2Normalize(&edge, &edge);
    kmVec2Normalize(&other_edge, &other_edge);
    
    n.x = edge.y;
    n.y = -edge.x;
    
    d = kmVec2Dot(&n, &other_edge);
    if(d > 0.0f) {
        n.x = -n.x;
        n.y = -n.y;
    }

    normal_out->x = n.x;
    normal_out->y = n.y;
    kmVec2Normalize(normal_out, normal_out);
}

kmBool kmRay2IntersectTriangle(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, const kmVec2* p3, kmVec2* intersection, kmVec2* normal_out, kmScalar* distance_out) {
    kmVec2 intersect;
    kmVec2 final_intersect;
    kmVec2 normal;
    kmScalar distance = 10000.0f;
    kmBool intersected = KM_FALSE;
 
    if(kmRay2IntersectLineSegment(ray, p1, p2, &intersect)) {
        kmVec2 tmp;
        kmScalar this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        kmVec2 this_normal;
        calculate_line_normal(*p1, *p2, *p3, &this_normal);                                   
        if(this_distance < distance && kmVec2Dot(&this_normal, &ray->dir) < 0.0f) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;
            distance = this_distance;
            kmVec2Assign(&normal, &this_normal);
            intersected = KM_TRUE;                        
        }
    }
    
    if(kmRay2IntersectLineSegment(ray, p2, p3, &intersect)) {
        kmVec2 tmp;
        kmScalar this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        
        kmVec2 this_normal;
        calculate_line_normal(*p2, *p3, *p1, &this_normal);
        
        if(this_distance < distance && kmVec2Dot(&this_normal, &ray->dir) < 0.0f) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;
            distance = this_distance;
            kmVec2Assign(&normal, &this_normal);
            intersected = KM_TRUE;                                
        }   
    }
    
    if(kmRay2IntersectLineSegment(ray, p3, p1, &intersect)) {

        kmVec2 tmp;
        kmScalar this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
        
        kmVec2 this_normal;
        calculate_line_normal(*p3, *p1, *p2, &this_normal);                           
        if(this_distance < distance && kmVec2Dot(&this_normal, &ray->dir) < 0.0f) {
            final_intersect.x = intersect.x;
            final_intersect.y = intersect.y;
            distance = this_distance;
            kmVec2Assign(&normal, &this_normal);
            intersected = KM_TRUE;                                
        }          
    }
    
    if(intersected) {
        intersection->x = final_intersect.x;
        intersection->y = final_intersect.y;
        if(normal_out) {
            normal_out->x = normal.x;
            normal_out->y = normal.y;
        }
        if(distance) {
            *distance_out = distance;
        }
    }
    
    return intersected;
}

kmBool kmRay2IntersectBox(const kmRay2* ray, const kmVec2* p1, const kmVec2* p2, const kmVec2* p3, const kmVec2* p4,
kmVec2* intersection, kmVec2* normal_out) {
    kmBool intersected = KM_FALSE;
    kmVec2 intersect, final_intersect, normal;
    kmScalar distance = 10000.0f;
    
    const kmVec2* points[4];
    unsigned int i = 0;
    points[0] = p1;
    points[1] = p2;
    points[2] = p3; 
    points[3] = p4;

    for(; i < 4; ++i) {
        const kmVec2* this_point = points[i];
        const kmVec2* next_point = (i == 3) ? points[0] : points[i+1];
        const kmVec2* other_point = (i == 3 || i == 0) ? points[1] : points[0];
        
        if(kmRay2IntersectLineSegment(ray, this_point, next_point, &intersect)) {
            
            kmVec2 tmp;
            kmScalar this_distance = kmVec2Length(kmVec2Subtract(&tmp, &intersect, &ray->start));
            
            kmVec2 this_normal;
            
            calculate_line_normal(*this_point, *next_point, *other_point, &this_normal);
            if(this_distance < distance && kmVec2Dot(&this_normal, &ray->dir) < 0.0f) {
                kmVec2Assign(&final_intersect, &intersect);
                distance = this_distance;
                intersected = KM_TRUE;        
                kmVec2Assign(&normal, &this_normal);
            }
        }
    }
    
    if(intersected) {
        intersection->x = final_intersect.x;
        intersection->y = final_intersect.y;
        if(normal_out) {
            normal_out->x = normal.x;
            normal_out->y = normal.y;
        }
    }
    
    return intersected;    
}

kmBool kmRay2IntersectCircle(const kmRay2* ray, const kmVec2 centre, const kmScalar radius, kmVec2* intersection) {
    assert(0 && "Not implemented");
    return KM_TRUE;
}
