#ifndef KM_GL_MATRIX_H_INCLUDED
#define KM_GL_MATRIX_H_INCLUDED

#define KM_GL_MODELVIEW 0x1700
#define KM_GL_PROJECTION 0x1701
#define KM_GL_TEXTURE 0x1702

typedef unsigned int kmGLEnum;

#include "../mat4.h"

#ifdef __cplusplus
extern "C" {
#endif

void kmGLPushMatrix(void);
void kmGLPopMatrix(void);
void kmGLMatrixMode(kmGLEnum mode);
void kmGLLoadIdentity(void);
void kmGLLoadMatrix(const kmMat4* pIn);
void kmGLMultMatrix(const kmMat4* pIn);
void kmGLTranslatef(float x, float y, float z);
void kmGLRotatef(float angle, float x, float y, float z);
void kmGLGetMatrix(kmGLEnum mode, kmMat4* pOut);

#ifdef __cplusplus
}
#endif

#endif // MATRIX_H_INCLUDED
