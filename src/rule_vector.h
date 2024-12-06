#ifndef RULE_VECTOR_H
#define RULE_VECTOR_H

#include "types.h"

struct ord_rule
{
	i64 firstPage;
	i64 secondPage;
};

struct rule_vector
{
	u32 len;
	u32 capacity;
	ord_rule* items;
};

i32 findOrdRule(rule_vector*, ord_rule);
void push(rule_vector*, ord_rule);
void clear(rule_vector*);
void initVector(rule_vector*, u32);
void destroyVector(rule_vector*);

#endif
