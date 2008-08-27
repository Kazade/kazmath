#ifndef C_STACK_H_INCLUDED
#define C_STACK_H_INCLUDED

typedef struct km_c_stack {
	int capacity; //The total item capacity
	int item_count; //The number of items
	unsigned int item_size; //The size of the item in the stack

	void* front;
	void* stack;
} km_c_stack;

void km_c_stack_initialize(km_c_stack* stack, unsigned int item_size);
void km_c_stack_push(km_c_stack* stack, const void* item);
void km_c_stack_pop(km_c_stack* stack, void* pOut);
void km_c_stack_release(km_c_stack* stack);

#endif // C_STACK_H_INCLUDED
