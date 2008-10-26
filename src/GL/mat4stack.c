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

void km_mat4_stack_push(km_mat4_stack* stack, const kmMat4* item) {
	/*Set the top pointer to the beginning of the memory + num items (zero based) */
	stack->top = stack->stack + (stack->item_count * sizeof(kmMat4));
	memcpy(stack->top, item, sizeof(kmMat4)); /*Copy the new item to the top of the stack*/
	stack->item_count++; /*Increment the item count*/

	/*If we have reached the capacity of the allocated memory
	 * then we resize the stack */
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
}
