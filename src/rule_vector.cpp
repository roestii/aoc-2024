#include <stdlib.h>
#include "rule_vector.h"

void initVector(rule_vector* v, u32 capacity)
{
	v->capacity = capacity;
	v->len = 0;
	v->items = (ord_rule*) malloc(capacity * sizeof(ord_rule));
}

void destroyVector(rule_vector* v)
{
	free(v->items);
}

void clear(rule_vector* v)
{
	v->len = 0;
}

void push(rule_vector* v, ord_rule item)
{
	if (v->len == v->capacity)
	{
		v->capacity *= 2;
		ord_rule* newItems = (ord_rule*) malloc(v->capacity * sizeof(ord_rule));

		for (int i = 0; i < v->len; ++i)
		{
			newItems[i] = v->items[i];
		}

		free(v->items);
		v->items = newItems;
	}

	v->items[v->len] = item;
	++v->len;
}

i32 findOrdRule(rule_vector* haystack, ord_rule needle)
{
	ord_rule* ordRule = haystack->items;
	for (int i = 0; i < haystack->len; ++i, ++ordRule)
	{
		if (ordRule->firstPage == needle.firstPage && 
	  		ordRule->secondPage == needle.secondPage)
		{
			return i;
		}
	}

	return -1;
}
