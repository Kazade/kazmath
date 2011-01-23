#include <unittest++/UnitTest++.h>

#include "kazmath/ray2.h"

TEST(test_line_segment_intersection) {
    kmVec2 line_start, line_end;
    kmVec2Fill(&line_start, -10.0f, 1.0f);
    kmVec2Fill(&line_end, 10.0f, 1.0f);
    
    kmRay2 ray;
    kmRay2Fill(&ray, 0.0f, 5.0f, 0.0f, -10.0f);
    
    kmVec2 intersect;
    
    CHECK(kmRay2IntersectLineSegment(&ray, line_start, line_end, &intersect));
    
    CHECK_CLOSE(0.0f, intersect.x, 0.001f);
    CHECK_CLOSE(1.0f, intersect.y, 0.001f);
    
    ray.dir.y = -3.0f;
    
    //Shouldn't reach the line (ray is too short)
    CHECK(!kmRay2IntersectLineSegment(&ray, line_start, line_end, &intersect));
    
    ray.dir.y = -10.0f;
    ray.start.x = -100.0f;
    //Shouldn't reach the line (ray is too far to the left)
    CHECK(!kmRay2IntersectLineSegment(&ray, line_start, line_end, &intersect));
    
    line_start.x = -150.0f;
    //Should now hit the line as it's been extended
    CHECK(kmRay2IntersectLineSegment(&ray, line_start, line_end, &intersect));
}

int main() {
    return UnitTest::RunAllTests();
}
