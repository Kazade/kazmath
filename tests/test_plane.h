#include "kaztest/kaztest.h"

#include "../kazmath/kazmath.h"

class TestPlane : public TestCase {
public:
    void test_plane_extraction_from_matrix() {

        kmMat4 ortho;
        kmMat4OrthographicProjection(&ortho, -1.0, 1.0, -1.0, 1.0, 1.0, 10.0);

        kmPlane p;
        kmPlaneExtractFromMat4(&p, &ortho, -2); //-2 == TOP

        assert_equal(p.d, 1.0); //Plane's distance from the origin should be 1

        assert_equal(p.a, 0.0); //Plane's normal should be directly down
        assert_equal(p.b, -1.0);
        assert_equal(p.c, 0.0);

        kmPlaneExtractFromMat4(&p, &ortho, 2); //Bottom
        assert_equal(p.d, 1.0); //Plane's distance from the origin should be 1

        assert_equal(p.a, 0.0); //Plane's normal should be directly up
        assert_equal(p.b, 1.0);
        assert_equal(p.c, 0.0);

    /*
        ExtractPlane(view_frustum[LEFT], MV, 1);
        ExtractPlane(view_frustum[RIGHT], MV, -1);
        ExtractPlane(view_frustum[BOTTOM], MV, 2);
        ExtractPlane(view_frustum[TOP], MV, -2);
        ExtractPlane(view_frustum[NEAR], MV, 3);
        ExtractPlane(view_frustum[FAR], MV, -3);
    */
    }

    void test_three_plane_intersection() {

        kmPlane p1, p2, p3;

        p1.a = 0.0; p1.b = 1.0; p1.c = 0.0; p1.d = 1.0; //Floor
        p2.a = 1.0; p2.b = 0.0; p2.c = 0.0; p2.d = 1.0; //Left
        p3.a = 0.0; p3.b = 0.0; p3.c = 1.0; p3.d = 1.0; //Back

        kmVec3 p;
        assert_true(kmPlaneGetIntersection(&p, &p1, &p2, &p3));

        assert_equal(-1.0, p.x);
        assert_equal(-1.0, p.y);
        assert_equal(-1.0, p.z);

    }
};
