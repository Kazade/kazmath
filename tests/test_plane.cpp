#include <unittest++/UnitTest++.h>

#include "../kazmath/kazmath.h"

TEST(test_plane_extraction_from_matrix) {

    kmMat4 ortho;
    kmMat4OrthographicProjection(&ortho, -1.0, 1.0, -1.0, 1.0, 1.0, 10.0);
    
    kmPlane p;
    kmPlaneExtractFromMat4(&p, &ortho, -2); //-2 == TOP
    
    CHECK_EQUAL(p.d, 1.0); //Plane's distance from the origin should be 1

    CHECK_EQUAL(p.a, 0.0); //Plane's normal should be directly down
    CHECK_EQUAL(p.b, -1.0);
    CHECK_EQUAL(p.c, 0.0);

    kmPlaneExtractFromMat4(&p, &ortho, 2); //Bottom
    CHECK_EQUAL(p.d, 1.0); //Plane's distance from the origin should be 1

    CHECK_EQUAL(p.a, 0.0); //Plane's normal should be directly up
    CHECK_EQUAL(p.b, 1.0);
    CHECK_EQUAL(p.c, 0.0);

/*
	ExtractPlane(view_frustum[LEFT], MV, 1);
	ExtractPlane(view_frustum[RIGHT], MV, -1);
	ExtractPlane(view_frustum[BOTTOM], MV, 2);
	ExtractPlane(view_frustum[TOP], MV, -2);
	ExtractPlane(view_frustum[NEAR], MV, 3);
	ExtractPlane(view_frustum[FAR], MV, -3);
*/
}

TEST(test_three_plane_intersection) {

    kmPlane p1, p2, p3;
    
    p1.a = 0.0; p1.b = 1.0; p1.c = 0.0; p1.d = 1.0; //Floor
    p2.a = 1.0; p2.b = 0.0; p2.c = 0.0; p2.d = 1.0; //Left
    p3.a = 0.0; p3.b = 0.0; p3.c = 1.0; p3.d = 1.0; //Back
    
    kmVec3 p;
    CHECK(kmPlaneGetIntersection(&p, &p1, &p2, &p3));
    
    CHECK_EQUAL(-1.0, p.x);
    CHECK_EQUAL(-1.0, p.y);
    CHECK_EQUAL(-1.0, p.z);

}
