#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MAIN

#define BOOST_TEST_MODULE kazmodel_model_tests
#include <boost/test/unit_test.hpp>
#include <boost/test/floating_point_comparison.hpp>

#include <kazmodel/model.h>

BOOST_AUTO_TEST_CASE(create_model_test) {
    KMuint model_id;
    kmGenModels(1, &model_id);
    kmBindModel(model_id);

    BOOST_CHECK(kmGetError() == KM_NO_ERROR);
    kmCreateModel(0); //Cannot create a model with no parts
    BOOST_CHECK(kmGetError() == KM_INVALID_VALUE);
    kmCreateModel(-100); //Can't use a negative number
    BOOST_CHECK(kmGetError() == KM_INVALID_VALUE);
    kmCreateModel(1); //SHould be OK
    BOOST_CHECK(kmGetError() == KM_NO_ERROR);

    kmDeleteModels(1, &model_id); //Should free the model
    kmCreateModel(1); //SHould fail, invalid operation, model is not bound

    BOOST_CHECK(kmGetError() == KM_INVALID_OPERATION);
}

BOOST_AUTO_TEST_CASE(create_model_parts) {
    KMuint model_id;
    kmGenModels(1, &model_id);
    kmBindModel(model_id);
    kmCreateModel(1); //SHould be OK

    kmCreatePart(2, 1, 1, 0, 0); //Error, part "2" doesn't exist
    BOOST_CHECK(kmGetError() == KM_INVALID_VALUE);

    kmCreatePart(0, 0, 1, 0, 0); //Error, can't create NO meshes
    BOOST_CHECK(kmGetError() == KM_INVALID_VALUE);
    kmCreatePart(0, 1, 0, 0, 0); //Error, can't have NO frames
    BOOST_CHECK(kmGetError() == KM_INVALID_VALUE);
    kmCreatePart(0, 1, 1, 0, 0); //OK, part zero, 1 mesh, 1 frame
    BOOST_CHECK(kmGetError() == KM_NO_ERROR);
    kmDeleteModels(1, &model_id); //Should free the model
}

BOOST_AUTO_TEST_CASE(create_model_mesh) {
    KMuint model_id;
    kmGenModels(1, &model_id);
    kmBindModel(model_id);
    kmCreateModel(1); //SHould be OK
    kmCreatePart(0, 1, 1, 0, 0); //OK, part zero, 1 mesh, 1 frame

    //Create a mesh, with 3 vertices and a single triangle
    KMuint indices [] = { 0, 1, 2 };
    kmCreateMesh(0, 0, 3, 1, indices);

    kmVec3 v1, v2, v3;
    v1.x = -1.0f;
    v1.y = -1.0f;
    v1.z = -4.0f;

    v2.x =  1.0f;
    v2.y = -1.0f;
    v2.z = -4.0f;

    v3.x =  0.0f;
    v3.y =  1.0f;
    v3.z = -4.0f;

    kmVec3 vertices [] = { v1, v2, v3 };
    kmMeshFrame(0, 0, 0, vertices, NULL, NULL); //Specify a single frame for the mesh
    BOOST_CHECK(kmGetError() == KM_NO_ERROR);

    kmDeleteModels(1, &model_id); //Should free the model
}

