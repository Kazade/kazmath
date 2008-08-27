#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED

enum MATRIX_TYPE {
	KM_GL_MODELVIEW,
	KM_GL_PROJECTION,
	KM_GL_TEXTURE
};

void kmGLPushMatrix(void);
void kmGLPopMatrix(void);
void kmGLMatrixMode(MATRIX_TYPE type);
void kmGLLoadIdentity(void);

#endif // STACK_H_INCLUDED
