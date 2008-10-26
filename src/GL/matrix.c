#include <stdlib.h>
#include <assert.h>

#include "mat4stack.h"
#include "matrix.h"

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

		kmMat4 identity; //Temporary identity matrix
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
			assert(1 && "Invalid matrix mode specified"); //TODO: Proper error handling
		break;
	}
}

void kmGLPushMatrix(void)
{
	lazyInitialize(); //Initialize the stacks if they haven't been already

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
	lazyInitialize();

	kmMat4Identity(current_stack->top); //Replace the top matrix with the identity matrix
}

void kmGLFreeAll()
{
	//Clear the matrix stacks
	km_mat4_stack_release(modelview_matrix_stack);
	km_mat4_stack_release(projection_matrix_stack);
	km_mat4_stack_release(texture_matrix_stack);

	//Delete the matrices
	free(modelview_matrix_stack);
	free(projection_matrix_stack);
	free(texture_matrix_stack);

	initialized = 0; //Set to uninitialized

	current_stack = NULL; //Set the current stack to point nowhere
}

void kmGLMultMatrix(const kmMat4* pIn)
{
	lazyInitialize();
	kmMat4Multiply(current_stack->top, pIn, current_stack->top);
}

void kmGLGetMatrix(kmGLEnum mode, kmMat4* pOut)
{
	lazyInitialize();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			kmMat4Assign(pOut, modelview_matrix_stack->top);
		break;
		case KM_GL_PROJECTION:
			kmMat4Assign(pOut, projection_matrix_stack->top);
		break;
		case KM_GL_TEXTURE:
			kmMat4Assign(pOut, texture_matrix_stack->top);
		break;
		default:
			assert(1 && "Invalid matrix mode specified"); //TODO: Proper error handling
		break;
	}
}
