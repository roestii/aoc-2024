#include "stack.h"

void initStack(stack* s, u32 capacity)
{
	s->capacity = capacity;
	s->len = 0;
	s->items = (tree_node*) malloc(capacity * sizeof(tree_node));
}

void destroyStack(stack* s)
{
	free(s->items);
}

void clear(stack* s)
{
	s->len = 0;
}

i32 push(stack* s, tree_node item)
{
	if (s->len == s->capacity)
	{
		return -1;
	}

	s->items[s->len] = item;
	++s->len;
	return 0;
}

i32 pop(tree_node* result, stack* s)
{
	if (s->len == 0)
	{
		return 0;
	}

	*result = s->items[s->len];
	--s->len;
	return 1;
}
