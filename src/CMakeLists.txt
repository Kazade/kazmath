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
)

SET(GL_UTILS_HEADERS
gl_utils/c_stack.h
gl_utils/matrix_stack.h
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
)

SET(GL_UTILS_SRCS
gl_utils/c_stack.c
gl_utils/matrix_stack.c
)

#ADD_LIBRARY(Kazmath STATIC ${KAZMATH_SRCS})
#INSTALL(TARGETS Kazmath ARCHIVE DESTINATION lib)

ADD_LIBRARY(Kazmath SHARED ${KAZMATH_SRCS})
INSTALL(TARGETS Kazmath LIBRARY DESTINATION lib)

ADD_LIBRARY(KazmathGL SHARED ${GL_UTILS_SRCS})
INSTALL(TARGETS KazmathGL LIBRARY DESTINATION lib)

INSTALL(FILES ${KAZMATH_HEADERS} DESTINATION include/kazmath)
INSTALL(FILES ${GL_UTILS_HEADERS} DESTINATION include/kazmath/gl_utils)

