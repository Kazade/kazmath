INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/include )

LINK_LIBRARIES(libunittest++.a)

FILE(GLOB_RECURSE TEST_FILES *.cpp)

LINK_LIBRARIES(kazmath)

ADD_EXECUTABLE(kazmath_tests ${TEST_FILES})
ADD_TEST(kazmath_suite kazmath_tests)

