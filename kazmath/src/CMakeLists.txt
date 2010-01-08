SET(KAZMATH_HEADERS
${CMAKE_SOURCE_DIR}/include/kazmath/vec2.h
${CMAKE_SOURCE_DIR}/include/kazmath/vec3.h
${CMAKE_SOURCE_DIR}/include/kazmath/vec4.h
${CMAKE_SOURCE_DIR}/include/kazmath/mat4.h
${CMAKE_SOURCE_DIR}/include/kazmath/mat3.h
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

SET(KAZMATH_SRCS
mat4.c
mat3.c    
plane.c      
vec4.c
quaternion.c 
vec2.c
vec3.c
utility.c
aabb.c
GL/mat4stack.c
GL/matrix.c
)

#ADD_LIBRARY(Kazmath STATIC ${KAZMATH_SRCS})
#INSTALL(TARGETS Kazmath ARCHIVE DESTINATION lib)

INCLUDE_DIRECTORIES( ${CMAKE_SOURCE_DIR}/include )

ADD_LIBRARY(Kazmath STATIC ${KAZMATH_SRCS})
INSTALL(TARGETS Kazmath ARCHIVE DESTINATION lib)

#ADD_LIBRARY(KazmathGL STATIC ${GL_UTILS_SRCS})
#INSTALL(TARGETS KazmathGL ARCHIVE DESTINATION lib)

INSTALL(FILES ${KAZMATH_HEADERS} DESTINATION include/kazmath)
INSTALL(FILES ${GL_UTILS_HEADERS} DESTINATION include/kazmath/GL)
