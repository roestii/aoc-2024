#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"

enum node_operand
{
	ADDITION = 0,
	MULTIPLICATION = 1
};

struct tree_node
{
	i64 intermediateResult;
	u32 idx;
	node_operand operand;
};

struct queue
{
	u32 len;
	u32 readIdx;
	u32 writeIdx;

	tree_node* items;
};

void initQueue(queue*, u32);
void destroyQueue(queue*);
i32 push(queue*, tree_node);
i32 pop(tree_node*, queue*);
void clear(queue*);

#endif 
