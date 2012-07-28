#include <unittest++/UnitTest++.h>
#include <limits>

#include "../kazmath/aabb.h"

TEST(test_aabb_triangle_intersection) {

    kmAABB box;
    box.min.x = -5.0f;
    box.min.y = -5.0f;
    box.min.z = std::numeric_limits<kmScalar>::min();
    
    box.max.x = 5.0f;
    box.max.y = 5.0f;
    box.max.z = std::numeric_limits<kmScalar>::max();
    
    //Triangle that is entirely within the bounds of the box
    kmVec3 tri1 [] = {
        { -1.0f, 0.0f, 0.0f },
        {  1.0f, 0.0f, 0.0f },
        {  1.0f, 1.0f, 0.0f },        
    };
    
    //Should intersect
    CHECK(kmAABBIntersectsTriangle(&box, &tri1[0], &tri1[1], &tri1[2]));
    
    //Triangle that entirely surrounds the box (should still return true)
    kmVec3 tri2 [] = {
        { -100.0f, 0.0f, 0.0f },
        {  100.0f, 0.0f, 0.0f },
        {  100.0f, 100.0f, 0.0f },        
    };
    CHECK(kmAABBIntersectsTriangle(&box, &tri2[0], &tri2[1], &tri2[2]));
    
    //Triangle that is entirely outside of the box (should return false)
    kmVec3 tri3 [] = {
        {  10.0f, 0.0f, 0.0f },
        {  20.0f, 0.0f, 0.0f },
        {  20.0f, 10.0f, 0.0f },        
    };
    
    CHECK(!kmAABBIntersectsTriangle(&box, &tri3[0], &tri3[1], &tri3[2]));
    
    //Triangle that intersects the box (should return true)
    kmVec3 tri4 [] = {
        {  0.0f, 0.0f, 0.0f },
        {  10.0f, 0.0f, 0.0f },
        {  10.0f, 10.0f, 0.0f },        
    };
    
    CHECK(kmAABBIntersectsTriangle(&box, &tri4[0], &tri4[1], &tri4[2]));    
}


