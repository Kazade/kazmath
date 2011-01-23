CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(kazmath)

ENABLE_TESTING()

SET(CMAKE_C_FLAGS "-std=c99")
SET(CMAKE_INSTALL_PREFIX, "/usr")

SET(KAZMATH_HEADERS
    ${CMAKE_SOURCE_DIR}/include/kazmath/vec2.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/vec3.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/vec4.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/mat4.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/mat3.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/ray2.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/plane.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/utility.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/quaternion.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/aabb.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/kazmath.h
)

SET(GL_UTILS_HEADERS
    ${CMAKE_SOURCE_DIR}/include/kazmath/GL/mat4stack.h
    ${CMAKE_SOURCE_DIR}/include/kazmath/GL/matrix.h
)

SET(KAZMATH_SOURCES
    ${CMAKE_SOURCE_DIR}/src/mat4.c
    ${CMAKE_SOURCE_DIR}/src/mat3.c    
    ${CMAKE_SOURCE_DIR}/src/plane.c      
    ${CMAKE_SOURCE_DIR}/src/vec4.c
    ${CMAKE_SOURCE_DIR}/src/quaternion.c 
    ${CMAKE_SOURCE_DIR}/src/vec2.c
    ${CMAKE_SOURCE_DIR}/src/vec3.c
    ${CMAKE_SOURCE_DIR}/src/utility.c
    ${CMAKE_SOURCE_DIR}/src/aabb.c
    ${CMAKE_SOURCE_DIR}/src/ray2.c
    ${CMAKE_SOURCE_DIR}/src/GL/mat4stack.c
    ${CMAKE_SOURCE_DIR}/src/GL/matrix.c
)

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(tests)
