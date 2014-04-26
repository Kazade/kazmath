#include <UnitTest++.h>
#include <limits>

#include "../kazmath/aabb.h"

TEST(test_aabb_expand_to_contain) {
    kmAABB box;

    box.min.x = -1;
    box.min.y = -1;
    box.min.z = -1;

    box.max.x = 1;
    box.max.y = 1;
    box.max.z = 1;

    kmAABB other;

    other.max.y = other.min.z = other.max.z = 0;

    other.max.x = 2;
    other.min.x = -2;
    other.min.y = -2;

    kmAABBExpandToContain(&box, &box, &other);

    CHECK_EQUAL(box.max.x, 2);
    CHECK_EQUAL(box.min.x, -2);
    CHECK_EQUAL(box.max.y, 1);
    CHECK_EQUAL(box.min.y, -2);
}

/*
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
}*/


