#include <malloc.h>
#include <memory.h>
#include <assert.h>
#include <stdio.h>

#define INITIAL_SIZE 1
#define INCREMENT 1

#include "c_stack.h"

void km_c_stack_initialize(km_c_stack* stack, unsigned int item_size) {
	stack->item_size = item_size; //Copy the size across
	stack->stack = (void*) malloc(item_size * INITIAL_SIZE); //allocate the memory
	stack->capacity = INITIAL_SIZE; //Set the capacity to 10
	stack->front = NULL; //Set the front to NULL
	stack->item_count = 0;
};

void km_c_stack_push(km_c_stack* stack, const void* item) {
	/*Set the front pointer to the beginning of the memory + num items (zero based) */
	stack->front = stack->stack + (stack->item_count * stack->item_size);
	memcpy(stack->front, item, stack->item_size); /*Copy the new item to the front of the stack*/
	stack->item_count++; /*Increment the item count*/

	/*If we have reached the capacity of the allocated memory
	 * then we resize the stack */
	if (stack->item_count == stack->capacity) {
		void* temp_stack = stack->stack;
		unsigned int new_size = stack->item_size * (stack->capacity + INCREMENT);
		stack->stack = (void*) malloc(new_size);
		memcpy(stack->stack, temp_stack, stack->capacity * stack->item_size);
		stack->capacity += INCREMENT;
		stack->front = stack->stack + ((stack->item_count - 1) * stack->item_size);
		free(temp_stack); //Free the original memory
	}
}

void km_c_stack_pop(km_c_stack* stack, void* pOut) {
	assert(pOut != NULL && "Memory must be allocated before popping");

	//Decrement the item count
	stack->item_count = stack->item_count - 1;
	
	//Copy the data from stack->front to the pOut pointer
	memcpy(pOut, stack->front, stack->item_size);
	stack->front = stack->stack + ((stack->item_count - 1) * stack->item_size);

	assert(stack->item_count >= 0 && "Too many pops");
}

void km_c_stack_release(km_c_stack* stack) {
	free(stack->stack);
	stack->front = NULL;
	stack->item_count = 0;
	stack->capacity = 0;
	stack->item_size = 0;
}
