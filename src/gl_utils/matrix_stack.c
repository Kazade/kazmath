
#include "../mat4.h"
#include "../vec3.h"
#include "matrix_stack.h"
#include "c_stack.h"

/* The current matrices of each type */
static kmMat4 s_matrices[KM_GL_MAX_MATRIX];

/* The matrix stacks of each type */
static km_c_stack s_matrix_stacks[KM_GL_MAX_MATRIX];

/* The current matrix mode */
static KM_GL_ENUM s_current_mode = KM_GL_MODELVIEW;

static bool initialized = false;

static void initialize_matrices(void) {
	if (initialized) return;
	
	/* Initialize the matrix stacks */
	for (int i = 0; i < KM_GL_MAX_MATRIX; ++i) {
		km_c_stack_initialize(&s_matrix_stacks[i], sizeof(kmMat4));
		initialized = true;
	}	
}

void kmGLMatrixMode(KM_GL_ENUM type) {
	initialize_matrices(); //Make sure we are initialized
	
	s_current_mode = type;
}

void kmGLPushMatrix(void) {
	initialize_matrices();
	
	/* Push the current matrix onto the stack */
	km_c_stack_push(&s_matrix_stacks[s_current_mode], &s_matrices[s_current_mode]);
}

void kmGLPopMatrix(void) {
	initialize_matrices();
	
	/* Pop the top matrix and assign it to the current matrix */
	km_c_stack_pop(&s_matrix_stacks[s_current_mode], &s_matrices[s_current_mode]);
}

void kmGLLoadIdentity(void) {
	/* Load the identity matrix in place */
	kmMat4Identity(&s_matrices[s_current_mode]);
}

/* Returns a one of the matrices ready for sending to a shader */
void kmGLGetMatrix(KM_GL_ENUM mode, kmMat4* pOut) {
	kmMat4Assign(pOut, &s_matrices[mode]);
}

void kmGLMultMatrix(const kmMat4* pIn) {
	kmMat4Multiply(&s_matrices[s_current_mode], pIn, &s_matrices[s_current_mode]);	
}

void kmGLLoadMatrix(const kmMat4* pIn) {
	kmMat4Assign(&s_matrices[s_current_mode], pIn);
}

void kmGLTranslate(float x, float y, float z) {
	kmMat4 trans;
	kmMat4Translation(&trans, x, y, z);
	kmMat4Multiply(&s_matrices[s_current_mode], &trans, &s_matrices[s_current_mode]);
}

void kmGLRotate(float angle, float x, float y, float z)
{
	kmMat4 rotation;
	kmVec3 axis;
	kmVec3Fill(&axis, x,y,z);
	kmMat4RotationAxis(&rotation, &axis, kmDegreesToRadians(angle));
	kmMat4Multiply(&s_matrices[s_current_mode], &rotation, &s_matrices[s_current_mode]);
}

/**
 * USAGE:
 * 
 * kmGLMatrixMode(KM_GL_PROJECTION);
 * kmGLLoadIdentity();
 * kmGLMatrixMode(KM_GL_MODELVIEW);
 * kmGLLoadIdentity();
 * kmGLMultMatrix(some_matrix_pointer);
 * 
 * kmMat4 modelview_matrix;
 * kmGLGetMatrix(&modelview_matrix, KM_GL_MODELVIEW);
*/
