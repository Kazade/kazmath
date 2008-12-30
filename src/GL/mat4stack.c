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

#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#define INITIAL_SIZE 30
#define INCREMENT 50

#include "mat4stack.h"

void km_mat4_stack_initialize(km_mat4_stack* stack) {
	stack->stack = (kmMat4*) malloc(sizeof(kmMat4) * INITIAL_SIZE); //allocate the memory
	stack->capacity = INITIAL_SIZE; //Set the capacity to 10
	stack->top = NULL; //Set the top to NULL
	stack->item_count = 0;
};
/*
void km_mat4_stack_push(km_mat4_stack* stack, const kmMat4* item) {
	//Set the top pointer to the beginning of the memory + num items (zero based)
	stack->top = &stack->stack[stack->item_count];
	//memcpy(stack->top, item, sizeof(kmMat4)); //Copy the new item to the top of the stack
	kmMat4Assign(stack->top, item);
	stack->item_count++; //Increment the item count

	//If we have reached the capacity of the allocated memory
    //then we resize the stack
	if (stack->item_count == stack->capacity) {
		kmMat4* temp_stack = stack->stack;
		unsigned int new_size = sizeof(kmMat4) * (stack->capacity + INCREMENT);
		stack->stack = (kmMat4*) malloc(new_size);
		memcpy(stack->stack, temp_stack, stack->capacity * sizeof(kmMat4));
		stack->capacity += INCREMENT;
		stack->top = stack->stack + ((stack->item_count - 1) * sizeof(kmMat4));
		free(temp_stack); //Free the original memory
	}
}

void km_mat4_stack_pop(km_mat4_stack* stack, kmMat4* pOut) {
	//Decrement the item count
	stack->item_count = stack->item_count - 1;

	if (pOut != NULL) {
		//Copy the data from stack->top to the pOut pointer (if it was specified)
		memcpy(pOut, stack->top, sizeof(kmMat4));
	}
	stack->top = stack->stack + ((stack->item_count - 1) * sizeof(kmMat4));

	assert(stack->item_count >= 0 && "Too many pops");
}

void km_mat4_stack_release(km_mat4_stack* stack) {
	free(stack->stack);
	stack->top = NULL;
	stack->item_count = 0;
	stack->capacity = 0;
}//*/

void km_mat4_stack_push(km_mat4_stack* stack, const kmMat4* item)
{
    stack->top = &stack->stack[stack->item_count];
    kmMat4Assign(stack->top, item);
    stack->item_count++;

    if(stack->item_count >= stack->capacity)
    {
        stack->capacity += INCREMENT;
        kmMat4* temp = stack->stack;
        stack->stack = (kmMat4*) malloc(stack->capacity*sizeof(kmMat4));
        memcpy(stack->stack, temp, sizeof(kmMat4)*(stack->capacity - INCREMENT));
        free(temp);
        stack->top = &stack->stack[stack->item_count - 1];
    }
}

void km_mat4_stack_pop(km_mat4_stack* stack, kmMat4* pOut)
{
    assert(stack->item_count && "Cannot pop an empty stack");

    stack->item_count--;
    stack->top = &stack->stack[stack->item_count - 1];
}

void km_mat4_stack_release(km_mat4_stack* stack) {
    free(stack->stack);
	stack->top = NULL;
	stack->item_count = 0;
	stack->capacity = 0;
}
//*/
