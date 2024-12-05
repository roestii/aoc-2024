#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include "types.h"

struct vector
{
	u32 len;
	u32 capacity;
	i64* items;
};

void push(vector*, i64);
void clear(vector*);
void initVector(vector*, u32);
void destroyVector(vector*);
void sort(vector*);

u32 sortedCount(vector*, i64);
u32 sortedCountFast(vector*, i64);

#endif
