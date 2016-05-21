#include "kaztest/kaztest.h"
#include <limits>

#include "../kazmath/aabb3.h"

class TestAABB : public TestCase {
public:
    void test_aabb_expand_to_contain() {
        kmAABB3 box;

        box.min.x = -1;
        box.min.y = -1;
        box.min.z = -1;

        box.max.x = 1;
        box.max.y = 1;
        box.max.z = 1;

        kmAABB3 other;

        other.max.y = other.min.z = other.max.z = 0;

        other.max.x = 2;
        other.min.x = -2;
        other.min.y = -2;

        kmAABB3ExpandToContain(&box, &box, &other);

        assert_equal(box.max.x, 2);
        assert_equal(box.min.x, -2);
        assert_equal(box.max.y, 1);
        assert_equal(box.min.y, -2);
    }

    void test_aabb_contains() {
        kmAABB3 box;

        kmVec3Fill(&box.min, -1, -1, -1);
        kmVec3Fill(&box.max, 1, 1, 1);

        kmAABB3 inside;
        kmVec3Fill(&inside.min, -0.5, -0.5, -0.5);
        kmVec3Fill(&inside.max, 0.5, 0.5, 0.5);

        assert_equal(KM_CONTAINS_ALL, kmAABB3ContainsAABB(&box, &inside));
        assert_equal(KM_CONTAINS_NONE, kmAABB3ContainsAABB(&inside, &box));

        kmAABB3 partial;
        kmVec3Fill(&partial.min, -0.5, -0.5, -0.5);
        kmVec3Fill(&partial.max, 1.5, 1.5, 1.5);
        assert_equal(KM_CONTAINS_PARTIAL, kmAABB3ContainsAABB(&box, &partial));
    }

    /*
    void XXX_test_aabb_triangle_intersection() {

        kmAABB3 box;
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
        assert_true(kmAABB3IntersectsTriangle(&box, &tri1[0], &tri1[1], &tri1[2]));

        //Triangle that entirely surrounds the box (should still return true)
        kmVec3 tri2 [] = {
            { -100.0f, 0.0f, 0.0f },
            {  100.0f, 0.0f, 0.0f },
            {  100.0f, 100.0f, 0.0f },
        };
        assert_true(kmAABB3IntersectsTriangle(&box, &tri2[0], &tri2[1], &tri2[2]));

        //Triangle that is entirely outside of the box (should return false)
        kmVec3 tri3 [] = {
            {  10.0f, 0.0f, 0.0f },
            {  20.0f, 0.0f, 0.0f },
            {  20.0f, 10.0f, 0.0f },
        };

        assert_true(!kmAABB3IntersectsTriangle(&box, &tri3[0], &tri3[1], &tri3[2]));

        //Triangle that intersects the box (should return true)
        kmVec3 tri4 [] = {
            {  0.0f, 0.0f, 0.0f },
            {  10.0f, 0.0f, 0.0f },
            {  10.0f, 10.0f, 0.0f },
        };

        assert_true(kmAABB3IntersectsTriangle(&box, &tri4[0], &tri4[1], &tri4[2]));
    }*/
};
