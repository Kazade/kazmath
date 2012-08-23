CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(kazmath)

ENABLE_TESTING()

SET(CMAKE_C_FLAGS "-std=c99")
#ADD_DEFINITIONS("-DUSE_DOUBLE_PRECISION")

SET(KAZMATH_HEADERS
    ${CMAKE_SOURCE_DIR}/kazmath/vec2.h
    ${CMAKE_SOURCE_DIR}/kazmath/vec3.h
    ${CMAKE_SOURCE_DIR}/kazmath/vec4.h
    ${CMAKE_SOURCE_DIR}/kazmath/mat4.h
    ${CMAKE_SOURCE_DIR}/kazmath/mat3.h
    ${CMAKE_SOURCE_DIR}/kazmath/ray2.h
    ${CMAKE_SOURCE_DIR}/kazmath/plane.h
    ${CMAKE_SOURCE_DIR}/kazmath/utility.h
    ${CMAKE_SOURCE_DIR}/kazmath/quaternion.h
    ${CMAKE_SOURCE_DIR}/kazmath/aabb.h
    ${CMAKE_SOURCE_DIR}/kazmath/kazmath.h
)

SET(GL_UTILS_HEADERS
    ${CMAKE_SOURCE_DIR}/kazmath/GL/mat4stack.h
    ${CMAKE_SOURCE_DIR}/kazmath/GL/matrix.h
)

SET(KAZMATH_SOURCES
    ${CMAKE_SOURCE_DIR}/kazmath/mat4.c
    ${CMAKE_SOURCE_DIR}/kazmath/mat3.c    
    ${CMAKE_SOURCE_DIR}/kazmath/plane.c      
    ${CMAKE_SOURCE_DIR}/kazmath/vec4.c
    ${CMAKE_SOURCE_DIR}/kazmath/quaternion.c 
    ${CMAKE_SOURCE_DIR}/kazmath/vec2.c
    ${CMAKE_SOURCE_DIR}/kazmath/vec3.c
    ${CMAKE_SOURCE_DIR}/kazmath/utility.c
    ${CMAKE_SOURCE_DIR}/kazmath/aabb.c
    ${CMAKE_SOURCE_DIR}/kazmath/ray2.c
    ${CMAKE_SOURCE_DIR}/kazmath/GL/mat4stack.c
    ${CMAKE_SOURCE_DIR}/kazmath/GL/matrix.c
)

ADD_SUBDIRECTORY(kazmath)
ADD_SUBDIRECTORY(tests)
