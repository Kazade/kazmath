SET(KAZMATH_HEADERS
vec2.h
vec3.h
vec4.h
mat4.h
mat3.h
plane.h
utility.h
quaternion.h
aabb.h
kazmath.h
)

SET(GL_UTILS_HEADERS
GL/mat4stack.h
GL/matrix.h
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

ADD_LIBRARY(Kazmath STATIC ${KAZMATH_SRCS})
INSTALL(TARGETS Kazmath ARCHIVE DESTINATION lib)

#ADD_LIBRARY(KazmathGL STATIC ${GL_UTILS_SRCS})
#INSTALL(TARGETS KazmathGL ARCHIVE DESTINATION lib)

INSTALL(FILES ${KAZMATH_HEADERS} DESTINATION include/kazmath)
INSTALL(FILES ${GL_UTILS_HEADERS} DESTINATION include/kazmath/GL)
