#ifndef STACK_H
#define STACK_H 

#include <stdlib.h>
#include "types.h"

enum node_operand
{
	ADDITION = 0,
	MULTIPLICATION = 1,
	CONCATENATION = 2
};

struct tree_node
{
	i64 intermediateResult;
	u32 idx;
	node_operand operand;
};

struct stack 
{
	u32 len;
	u32 capacity;
	tree_node* items;
};

void initStack(stack*, u32);
void destroyStack(stack*);
i32 push(stack*, tree_node);
i32 pop(tree_node*, stack*);
void clear(stack*);

#endif
