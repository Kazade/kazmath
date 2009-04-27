INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src )
LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/src )
LINK_LIBRARIES(libboost_unit_test_framework.a boost_thread-mt)

ADD_EXECUTABLE( test_mat3 ../src/mat3.c ../src/quaternion.c ../src/vec3.c ../src/utility.c ../src/vec4.c test_mat3.cpp)
ADD_TEST(test_mat3 test_mat3)

ADD_EXECUTABLE( test_mat4 ../src/mat4.c ../src/vec3.c ../src/quaternion.c ../src/utility.c ../src/vec4.c test_mat4.cpp)
ADD_TEST(test_mat4 test_mat4)
