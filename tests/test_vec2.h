#include "kaztest/kaztest.h"

#include "../kazmath/vec2.h"
#include "../kazmath/utility.h"
#include "../kazmath/mat3.h"

class TestVec2 : public TestCase {
public:
    void test_transform() {
        kmVec2 orig;
        kmVec2Fill(&orig, 0.0, 1.0f);

        kmMat3 rotate;
        kmMat3FromRotationZ(&rotate, kmDegreesToRadians(90.0f));

        kmVec2 rotated;
        kmVec2Transform(&rotated, &orig, &rotate);

        assert_close(1.0f, rotated.x, 0.001f);
        assert_close(0.0f, rotated.y, 0.001f);
    }

    void test_vec2_reflect() {
        kmVec2 incident;
        kmVec2Fill(&incident, 1.0f, -1.0f);

        kmVec2 reflected;
        kmVec2Reflect(&reflected, &incident, &KM_VEC2_POS_Y);

        assert_close(1.0f, reflected.x, 0.001f);
        assert_close(1.0f, reflected.y, 0.001f);
    }

    void test_degrees_between() {
        kmVec2 v1;
        kmVec2 v2;

        kmVec2Fill(&v1, 1, 0);
        kmVec2Fill(&v2, 0, 1);

        assert_close(90.0f, kmVec2DegreesBetween(&v1, &v2), 0.001f);
        assert_close(-90.0f, kmVec2DegreesBetween(&v2, &v1), 0.001f);
    }

    void test_vec2_rotate_by() {
        kmVec2 res;
        kmVec2RotateBy(&res, &KM_VEC2_POS_Y, 90, &KM_VEC2_ZERO);

        assert_close(-1, res.x, 0.001f);
        assert_close(0, res.y, 0.001f);

        kmVec2RotateBy(&res, &KM_VEC2_POS_Y, -90, &KM_VEC2_ZERO);

        assert_close(1, res.x, 0.001f);
        assert_close(0, res.y, 0.001f);

        kmVec2RotateBy(&res, &KM_VEC2_POS_Y, 180, &KM_VEC2_ZERO);

        assert_close(0, res.x, 0.001f);
        assert_close(-1, res.y, 0.001f);

    }
};
