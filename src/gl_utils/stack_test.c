#include <stdio.h>
#include <stdlib.h>

#include "c_stack.h"

int main(int argc, char** argv) {
	km_c_stack stack;
	int i = 0, j = 1, k = 2, l = 11;
	
	km_c_stack_initialize(&stack, sizeof(int));
	km_c_stack_push(&stack, &i);
	km_c_stack_push(&stack, &j);
	km_c_stack_push(&stack, &k);
	km_c_stack_push(&stack, &l);
	
	int x, y, z, w;
	
	km_c_stack_pop(&stack, &x);
	printf("%d\n", x);

	km_c_stack_pop(&stack, &y);
	printf("%d\n", y);

	km_c_stack_pop(&stack, &z);
	printf("%d\n", z);
	
	km_c_stack_pop(&stack, &w);
	printf("%d\n", w);		
	
	km_c_stack char_stack;
	char a = 'a', b = 'b', c = 'c';
	char t;
	
	km_c_stack_initialize(&char_stack, sizeof(char));
	km_c_stack_push(&char_stack, &a);
	km_c_stack_pop(&char_stack, &t);
	printf("%c\n", t);
	
	km_c_stack_push(&char_stack, &b);
	km_c_stack_pop(&char_stack, &t);
	printf("%c\n", t);
	
	//km_c_stack_pop(&char_stack, &t);
	
	km_c_stack_release(&char_stack);
	km_c_stack_release(&stack);
	return 0;	
}
