CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(kazmath)

option(KAZMATH_BUILD_TESTS "Build kazmath test suite" ON)
option(KAZMATH_BUILD_JNI_WRAPPER "Build JNI wrapper" ON)
option(KAZMATH_BUILD_GL_UTILS "Build gl utils" ON)
option(KAZMATH_BUILD_LUA_WRAPPER "Build Lua wrapper" ON)

IF (KAZMATH_BUILD_TESTS)
    ENABLE_TESTING()
    FIND_PACKAGE(PythonLibs REQUIRED)
ENDIF (KAZMATH_BUILD_TESTS)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
add_definitions(-D_DEFAULT_SOURCE)

SET(INSTALL_LIB_DIR "${CMAKE_INSTALL_PREFIX}/lib" CACHE PATH "Installation directory for libraries")

ADD_DEFINITIONS("-Wall -g")
#ADD_DEFINITIONS("-DUSE_DOUBLE_PRECISION")

SET(KAZMATH_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec2.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec3.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec4.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/mat4.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/mat3.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/ray2.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/ray3.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/plane.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/utility.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/quaternion.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/aabb2.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/aabb3.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/kazmath.h
)

SET(GL_UTILS_HEADERS
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/GL/mat4stack.h
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/GL/matrix.h
)

SET(KAZMATH_SOURCES
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/mat4.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/mat3.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/plane.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec4.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/quaternion.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec2.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/vec3.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/utility.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/aabb2.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/aabb3.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/ray2.c
    ${CMAKE_CURRENT_LIST_DIR}/kazmath/ray3.c
)

IF (KAZMATH_BUILD_GL_UTILS)
    SET(KAZMATH_SOURCES
        ${KAZMATH_SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/kazmath/GL/mat4stack.c
        ${CMAKE_CURRENT_LIST_DIR}/kazmath/GL/matrix.c
    )
ENDIF (KAZMATH_BUILD_GL_UTILS)

ADD_SUBDIRECTORY(kazmath)

IF (KAZMATH_BUILD_TESTS)
    ADD_SUBDIRECTORY(tests)
ENDIF (KAZMATH_BUILD_TESTS)

IF (KAZMATH_BUILD_JNI_WRAPPER)
    ADD_SUBDIRECTORY(java)
ENDIF (KAZMATH_BUILD_JNI_WRAPPER)

IF (KAZMATH_BUILD_LUA_WRAPPER)
    ADD_SUBDIRECTORY(lua)
ENDIF (KAZMATH_BUILD_LUA_WRAPPER)

SET(CPACK_PACKAGE_DEFAULT_LOCATION "/")
SET(CPACK_GENERATOR "RPM")
set(CPACK_PACKAGE_NAME "kazmath")
set(CPACK_PACKAGE_VENDOR "Kazade")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A simple 3D math library, designed for game development")
set(CPACK_PACKAGE_VERSION "0.5")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "5")
set(CPACK_PACKAGE_VERSION_PATCH "0")
set(CPACK_PACKAGE_INSTALL_DIRECTORY "kazmath")

SET(CPACK_RPM_PACKAGE_NAME "kazmath-devel")
SET(CPACK_RPM_PACKAGE_RELEASE 1)
SET(CPACK_RPM_PACKAGE_LICENCE "Simplified BSD")
SET(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
SET(CPACK_RPM_PACKAGE_DESCRIPTION "
Kazmath is a 3D math library written in C. It's designed to be easy to use and understand.
The library contains math functions for 2D, 3D and 4D vectors. 3x3 and 4x4 matrices, quaternions, planes and axis-aligned boxes")

INCLUDE(CPack)
