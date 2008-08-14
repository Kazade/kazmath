SET(KAZMATH_HEADERS
vec2.h
vec3.h
vec4.h
mat4.h
plane.h
utility.h
quaternion.h
)

SET(KAZMATH_SRCS
mat4.c    
plane.c      
vec4.c
quaternion.c 
vec2.c
vec3.c
utility.c
)

#ADD_LIBRARY(Kazmath STATIC ${KAZMATH_SRCS})
#INSTALL(TARGETS Kazmath ARCHIVE DESTINATION lib)

ADD_LIBRARY(Kazmath SHARED ${KAZMATH_SRCS})
INSTALL(TARGETS Kazmath LIBRARY DESTINATION lib)

INSTALL(FILES ${KAZMATH_HEADERS} DESTINATION include/kazmath)
