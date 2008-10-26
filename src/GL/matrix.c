#include <stdlib.h>

#include "mat4stack.h"
#include "matrix.h"

#define MATRIX_COUNT 3

km_mat4_stack* modelview_matrix_stack = NULL;
km_mat4_stack* projection_matrix_stack = NULL;
km_mat4_stack* texture_matrix_stack = NULL;

km_mat4_stack* current_stack = NULL;

static unsigned char initialized = 0;

void lazyInitialize()
{

	if (!initialized) {
		//Initialize all 3 stacks
		modelview_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));
		km_mat4_stack_initialize(modelview_matrix_stack);

		projection_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));
		km_mat4_stack_initialize(projection_matrix_stack);

		texture_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));
		km_mat4_stack_initialize(texture_matrix_stack);

		current_stack = modelview_matrix_stack;
		initialized = 1;

		kmMat4 identity;
		kmMat4Identity(&identity);

		//Make sure that each stack has the identity matrix
		km_mat4_stack_push(modelview_matrix_stack, &identity);
		km_mat4_stack_push(projection_matrix_stack, &identity);
		km_mat4_stack_push(texture_matrix_stack, &identity);
	}
}

void kmGLMatrixMode(kmGLEnum mode)
{
	lazyInitialize();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			current_stack = modelview_matrix_stack;
		break;
		case KM_GL_PROJECTION:
			current_stack = projection_matrix_stack;
		break;
		case KM_GL_TEXTURE:
			current_stack = texture_matrix_stack;
		break;
		default:
		break;
			//Error handling?
	}
}

void kmGLPushMatrix(void)
{
	lazyInitialize();

	//Duplicate the top of the stack (i.e the current matrix)
	km_mat4_stack_push(current_stack, current_stack->top);
}

void kmGLPopMatrix(void)
{
	//No need to lazy initialize, you shouldnt be popping first anyway!
	km_mat4_stack_pop(current_stack, NULL);
}

void kmGLLoadIdentity()
{
	kmMat4Identity(current_stack->top); //Replace the top matrix with the identity matrix
}


