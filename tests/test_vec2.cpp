#include <unittest++/UnitTest++.h>

#include "../kazmath/vec2.h"
#include "../kazmath/utility.h"
#include "../kazmath/mat3.h"

TEST(test_transform) {
    kmVec2 orig;
    kmVec2Fill(&orig, 0.0, 1.0f);
    
    kmMat3 rotate;
    kmMat3RotationZ(&rotate, kmDegreesToRadians(90.0f));
    
    kmVec2 rotated;
    kmVec2Transform(&rotated, &orig, &rotate);
    
    CHECK_CLOSE(1.0f, rotated.x, 0.001f);
    CHECK_CLOSE(0.0f, rotated.y, 0.001f);
}

