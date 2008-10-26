#ifndef C_STACK_H_INCLUDED
#define C_STACK_H_INCLUDED

#include "../mat4.h"

typedef struct km_mat4_stack {
	int capacity; //The total item capacity
	int item_count; //The number of items
	kmMat4* top;
	kmMat4* stack;
} km_mat4_stack;

#ifdef __cplusplus
extern "C" {
#endif

void km_mat4_stack_initialize(km_mat4_stack* stack);
void km_mat4_stack_push(km_mat4_stack* stack, const kmMat4* item);
void km_mat4_stack_pop(km_mat4_stack* stack, kmMat4* pOut);
void km_mat4_stack_release(km_mat4_stack* stack);

#ifdef __cplusplus
}
#endif

#endif // C_STACK_H_INCLUDED
