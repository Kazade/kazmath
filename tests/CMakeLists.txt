INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/include )

LINK_LIBRARIES(libunittest++.a)

ADD_EXECUTABLE(test_mat4 ${KAZMATH_SOURCES} test_mat4.cpp)
ADD_TEST(test_mat4 test_mat4)

ADD_EXECUTABLE(test_mat3 ${KAZMATH_SOURCES} test_mat3.cpp)
ADD_TEST(test_mat3 test_mat3)

ADD_EXECUTABLE(test_ray2 ${KAZMATH_SOURCES} test_ray2.cpp)
ADD_TEST(test_ray2 test_ray2)

ADD_EXECUTABLE(test_vec2 ${KAZMATH_SOURCES} test_vec2.cpp)
ADD_TEST(test_vec2 test_vec2)


