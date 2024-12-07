#include <stdlib.h>
#include "queue.h"

void initQueue(queue* q, u32 len)
{
	q->len = len;
	q->readIdx = 0;
	q->writeIdx = 0;
	q->items = (tree_node*) malloc(len * sizeof(tree_node));
}

void destroyQueue(queue* q)
{
	free(q->items);
}

i32 push(queue* q, tree_node node)
{
	u32 advanced = (q->writeIdx + 1) % q->len;
	if (advanced == q->readIdx)
	{
		return -1;
	}

	q->items[q->writeIdx] = node;
	q->writeIdx = advanced;
	return 0;
}

i32 pop(tree_node* result, queue* q)
{
	if (q->readIdx == q->writeIdx)
	{
		return -1;
	}

	*result = q->items[q->readIdx];
	q->readIdx = (q->readIdx + 1) % q->len;

	return 0;
}

void clear(queue* q)
{
	q->readIdx = 0;
	q->writeIdx = 0;
}

