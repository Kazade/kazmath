#ifndef MATRIX_STACK_H_INCLUDED
#define MATRIX_STACK_H_INCLUDED


#define KM_GL_MODELVIEW 	0
#define KM_GL_PROJECTION 	1
#define KM_GL_TEXTURE 		2
#define KM_GL_MAX_MATRIX	3

typedef unsigned int KM_GL_ENUM;

#ifdef __cplusplus
extern "C" {
#endif

void kmGLPushMatrix(void);
void kmGLPopMatrix(void);
void kmGLMatrixMode(KM_GL_ENUM type);
void kmGLLoadIdentity(void);
void kmGLLoadMatrix(const kmMat4* pIn);
void kmGLMultMatrix(const kmMat4* pIn);
void kmGLTranslate(float x, float y, float z);
void kmGLGetMatrix(KM_GL_ENUM mode, kmMat4* pOut);

#ifdef __cplusplus
}
#endif

#endif // STACK_H_INCLUDED
