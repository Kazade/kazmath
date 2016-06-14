#include "kaztest/kaztest.h"

#include "../kazmath/ray2.h"

class TestRay2 : public TestCase {
public:
    void test_line_segment_intersection() {
        kmVec2 line_start, line_end;
        kmVec2Fill(&line_start, -10.0f, 1.0f);
        kmVec2Fill(&line_end, 10.0f, 1.0f);

        kmRay2 ray;
        kmRay2Fill(&ray, 0.0f, 5.0f, 0.0f, -10.0f);

        kmVec2 intersect;

        assert_true(kmRay2IntersectLineSegment(&ray, &line_start, &line_end, &intersect));

        assert_close(0.0f, intersect.x, 0.001f);
        assert_close(1.0f, intersect.y, 0.001f);

        ray.dir.y = -3.0f;

        //The ray is a point, plus a direction to infinite, so it will
        //reach the line too, althought the direction vector is shorter
        assert_true(kmRay2IntersectLineSegment(&ray, &line_start, &line_end, &intersect));

        // if we want to check two line segments are crossing we must
        // use the function that interprets the Ray as a segment
        kmRay2 lineSegment;
        kmRay2FillWithEndpoints(&lineSegment, &line_start, &line_end);
        assert_true(!kmSegment2WithSegmentIntersection(&ray, &lineSegment, &intersect));


        ray.dir.y = -10.0f;
        ray.start.x = -100.0f;
        //Shouldn't reach the line (ray is too far to the left)
        assert_true(!kmRay2IntersectLineSegment(&ray, &line_start, &line_end, &intersect));

        line_start.x = -150.0f;
        //Should now hit the line as it's been extended
        assert_true(kmRay2IntersectLineSegment(&ray, &line_start, &line_end, &intersect));

        //Check diagonal case

        kmRay2Fill(&ray, 0.0, 1.0f, 0.0f, -1.0f);
        kmVec2Fill(&line_start, -1.0f, 0.0f);
        kmVec2Fill(&line_end, 10.0f, 5.0f);

        assert_true(kmRay2IntersectLineSegment(&ray, &line_start, &line_end, &intersect));
        assert_close(0.0f, intersect.x, 0.001f);
        assert_true(intersect.y > 0.0f);
    }

    void test_triangle_intersection() {
        kmVec2 p1, p2, p3;

        p1.x = -10.0f;
        p1.y = 0.0f;

        p2.x = 10.0f;
        p2.y = 0.0f;

        p3.x = 0.0f;
        p3.y = -5.0f;

        kmRay2 ray;
        ray.start.x = 0.0f;
        ray.start.y = 0.3f;
        ray.dir.x = 0.0f;
        ray.dir.y = -0.5f;

        kmVec2 intersect, normal;
        kmScalar dist;
        assert_true(kmRay2IntersectTriangle(&ray, &p1, &p2, &p3, &intersect, &normal, &dist));
        assert_close(0.0f, intersect.x, 0.001f);
        assert_close(0.0f, intersect.y, 0.001f);
        assert_close(0.0f, normal.x, 0.001f);
        assert_close(1.0f, normal.y, 0.001f);
    }
};
