/*
Copyright (c) 2008, Luke Benstead.
All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#include "matrix.h"
#include "mat4stack.h"

/* ---
 * Begin additions by Tobias Lensing for icedcoffee-framework.org */

typedef struct km_mat4_stack_context {
    km_mat4_stack modelview_matrix_stack;
    km_mat4_stack projection_matrix_stack;
    km_mat4_stack texture_matrix_stack;
    km_mat4_stack* current_stack;
    unsigned char initialized;
    void *contextRef;
    struct km_mat4_stack_context_list *entry;
} km_mat4_stack_context;

typedef struct km_mat4_stack_context_list {
    km_mat4_stack_context context;
    struct km_mat4_stack_context_list *prev;
    struct km_mat4_stack_context_list *next;
} km_mat4_stack_context_list;

static pthread_key_t current_context_key;
static km_mat4_stack_context_list *contexts;
static unsigned char initialized = 0;
static pthread_mutex_t contexts_mutex = PTHREAD_MUTEX_INITIALIZER;

void lazyInitialize()
{
    if (!initialized) {
        pthread_key_create(&current_context_key, NULL);
        initialized = 1;
    }
}

km_mat4_stack_context *lookUpContext(void *contextRef)
{
    lazyInitialize();
    
    if (contexts) {
        km_mat4_stack_context_list *entry = contexts;
        pthread_mutex_lock(&contexts_mutex);
        do {
            if (entry->context.contextRef == contextRef) {
                pthread_mutex_unlock(&contexts_mutex);
                return &entry->context;
            }
        } while ((entry = entry->next));
        pthread_mutex_unlock(&contexts_mutex);
    }
    
    return NULL;
}

km_mat4_stack_context *registerContext(void *contextRef)
{
    lazyInitialize();
    
    km_mat4_stack_context *existingContext = lookUpContext(contextRef);
    if (!existingContext) {
        km_mat4_stack_context_list *entry = contexts;
        km_mat4_stack_context_list *last = NULL;
        
        pthread_mutex_lock(&contexts_mutex);
        if (entry) {
            do {
                last = entry;
            } while((entry = entry->next));
        }
        pthread_mutex_unlock(&contexts_mutex);
        
        km_mat4_stack_context_list *newEntry = (km_mat4_stack_context_list *)malloc(sizeof(km_mat4_stack_context_list));
        memset(newEntry, 0, sizeof(km_mat4_stack_context_list));
        newEntry->context.contextRef = contextRef;
        newEntry->context.entry = newEntry;
        newEntry->prev = last;
        
        pthread_mutex_lock(&contexts_mutex);
        if (last) {
            last->next = newEntry;
        }
        if (!contexts) {
            contexts = newEntry;
        }
        pthread_mutex_unlock(&contexts_mutex);
        
        return &newEntry->context;
    }
    
    return existingContext;
}

void kmGLSetCurrentContext(void *contextRef)
{
    km_mat4_stack_context *current_context = registerContext(contextRef);
    pthread_setspecific(current_context_key, current_context);
}

void *kmGLGetCurrentContext()
{
    return ((km_mat4_stack_context *)pthread_getspecific(current_context_key))->contextRef;
}

void kmGLClearContext(km_mat4_stack_context *context)
{
    /* Unlink current context from linked list*/
    if (context->entry->prev)
        context->entry->prev->next = context->entry->next;
	
    /*Clear the matrix stacks*/
	km_mat4_stack_release(&context->modelview_matrix_stack);
	km_mat4_stack_release(&context->projection_matrix_stack);
	km_mat4_stack_release(&context->texture_matrix_stack);
    
	/*Delete the matrices*/
	context->initialized = 0;
    
    /*Set the current stack to point nowhere*/
	context->current_stack = NULL;
    
    /* Free the list entry, including its stacks*/
    free(context->entry);
}

void kmGLClearCurrentContext()
{
    kmGLClearContext(pthread_getspecific(current_context_key));
    pthread_setspecific(current_context_key, NULL);
}

void kmGLClearAllContexts()
{
    km_mat4_stack_context_list *entry = contexts;
    do {
        kmGLClearContext(&entry->context);
    } while ((entry = entry->next));
    
    pthread_setspecific(current_context_key, NULL);
}

/* End additions by Tobias Lensing for icedcoffee-framework.org
 * --- */

km_mat4_stack_context *lazyInitializeCurrentContext()
{
    km_mat4_stack_context *current_context = pthread_getspecific(current_context_key);
    
    assert(current_context != NULL && "No context set");
    
	if (current_context && !current_context->initialized) {
		kmMat4 identity; /*Temporary identity matrix*/

		/*Initialize all 3 stacks*/
		/*modelview_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->modelview_matrix_stack);

		/*projection_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->projection_matrix_stack);

		/*texture_matrix_stack = (km_mat4_stack*) malloc(sizeof(km_mat4_stack));*/
		km_mat4_stack_initialize(&current_context->texture_matrix_stack);

		current_context->current_stack = &current_context->modelview_matrix_stack;
		current_context->initialized = 1;

		kmMat4Identity(&identity);

		/*Make sure that each stack has the identity matrix*/
		km_mat4_stack_push(&current_context->modelview_matrix_stack, &identity);
		km_mat4_stack_push(&current_context->projection_matrix_stack, &identity);
		km_mat4_stack_push(&current_context->texture_matrix_stack, &identity);
	}
    
    return current_context;
}

void kmGLMatrixMode(kmGLEnum mode)
{
	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			current_context->current_stack = &current_context->modelview_matrix_stack;
		break;
		case KM_GL_PROJECTION:
			current_context->current_stack = &current_context->projection_matrix_stack;
		break;
		case KM_GL_TEXTURE:
			current_context->current_stack = &current_context->texture_matrix_stack;
		break;
		default:
			assert(0 && "Invalid matrix mode specified"); /*TODO: Proper error handling*/
		break;
	}
}

void kmGLPushMatrix(void)
{
	kmMat4 top;

	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();

	/*Duplicate the top of the stack (i.e the current matrix)	*/
	kmMat4Assign(&top, current_context->current_stack->top);
	km_mat4_stack_push(current_context->current_stack, &top);
}

void kmGLPopMatrix(void)
{
	km_mat4_stack_context *current_context = pthread_getspecific(current_context_key);
    assert(current_context->initialized && "Cannot Pop empty matrix stack");
	/*No need to lazy initialize, you shouldnt be popping first anyway!*/
	km_mat4_stack_pop(current_context->current_stack, NULL);
}

void kmGLLoadIdentity()
{
	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();
	kmMat4Identity(current_context->current_stack->top); /*Replace the top matrix with the identity matrix*/
}

void kmGLMultMatrix(const kmMat4* pIn)
{
	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, pIn);
}

void kmGLLoadMatrix(const kmMat4* pIn)
{
	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();
	kmMat4Assign(current_context->current_stack->top, pIn);
}

void kmGLGetMatrix(kmGLEnum mode, kmMat4* pOut)
{
	km_mat4_stack_context *current_context = lazyInitializeCurrentContext();

	switch(mode)
	{
		case KM_GL_MODELVIEW:
			kmMat4Assign(pOut, current_context->modelview_matrix_stack.top);
		break;
		case KM_GL_PROJECTION:
			kmMat4Assign(pOut, current_context->projection_matrix_stack.top);
		break;
		case KM_GL_TEXTURE:
			kmMat4Assign(pOut, current_context->texture_matrix_stack.top);
		break;
		default:
			assert(1 && "Invalid matrix mode specified"); /*TODO: Proper error handling*/
		break;
	}
}

void kmGLTranslatef(float x, float y, float z)
{
    km_mat4_stack_context *current_context = pthread_getspecific(current_context_key);

	kmMat4 translation;

	/*Create a rotation matrix using the axis and the angle*/
	kmMat4Translation(&translation,x,y,z);

	/*Multiply the rotation matrix by the current matrix*/
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &translation);
}

void kmGLRotatef(float angle, float x, float y, float z)
{
    km_mat4_stack_context *current_context = pthread_getspecific(current_context_key);

	kmVec3 axis;
	kmMat4 rotation;

	/*Create an axis vector*/
	kmVec3Fill(&axis, x, y, z);

	/*Create a rotation matrix using the axis and the angle*/
	kmMat4RotationAxisAngle(&rotation, &axis, kmDegreesToRadians(angle));

	/*Multiply the rotation matrix by the current matrix*/
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &rotation);
}

void kmGLScalef(float x, float y, float z)
{
    km_mat4_stack_context *current_context = pthread_getspecific(current_context_key);

	kmMat4 scaling;
	kmMat4Scaling(&scaling, x, y, z);
	kmMat4Multiply(current_context->current_stack->top, current_context->current_stack->top, &scaling);
}
