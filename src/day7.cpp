#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "string.h"
#include "vector.h"
#include "queue.h"

bool produces(vector* v, queue* frontier, i64 result)
{
	i64* components = v->items;
	tree_node startNodeAdd = { components[0], 0, ADDITION };
	tree_node startNodeMul = { components[0], 0, MULTIPLICATION };
	tree_node startNodeConcat = { components[0], 0, CONCATENATION };

	assert(push(frontier, startNodeAdd) == 0);
	assert(push(frontier, startNodeMul) == 0);
	assert(push(frontier, startNodeConcat) == 0);

	tree_node currentItem;
	while (pop(&currentItem, frontier) == 0)
	{
		if (currentItem.intermediateResult > result)
		{
			continue;
		}

		if (currentItem.idx == v->len - 1
			&& currentItem.intermediateResult == result)
		{
			return true;
		}

		u32 newIdx = currentItem.idx + 1;
		if (newIdx >= v->len)
		{
			continue;
		}

		tree_node newNodeAdd;
		tree_node newNodeMul;
		tree_node newNodeConcat;

		switch (currentItem.operand)
		{
			case ADDITION:
			{
				i64 newIntermediateResult = currentItem.intermediateResult + 
											components[newIdx];

				newNodeAdd = { newIntermediateResult, newIdx, ADDITION };
				newNodeMul = { newIntermediateResult, newIdx, MULTIPLICATION };
				newNodeConcat = { newIntermediateResult, newIdx, CONCATENATION };

				break;
			}
			case MULTIPLICATION:
			{
				i64 newIntermediateResult = currentItem.intermediateResult *
											components[newIdx];

				newNodeAdd = { newIntermediateResult, newIdx, ADDITION };
				newNodeMul = { newIntermediateResult, newIdx, MULTIPLICATION };
				newNodeConcat = { newIntermediateResult, newIdx, CONCATENATION };

				break;
			}
			case CONCATENATION:
			{
				u8 numberOfDigits = floor(log10(components[newIdx])) + 1;
				i64 shiftedIntermediateResult = currentItem.intermediateResult * pow(10, numberOfDigits);
				assert(shiftedIntermediateResult > currentItem.intermediateResult);
				i64 newIntermediateResult = shiftedIntermediateResult + components[newIdx];

				newNodeAdd = { newIntermediateResult, newIdx, ADDITION };
				newNodeMul = { newIntermediateResult, newIdx, MULTIPLICATION };
				newNodeConcat = { newIntermediateResult, newIdx, CONCATENATION };

				break;
			}
			default:
			{
				assert(!"this should not happen...\n");
			}
		}

		assert(push(frontier, newNodeAdd) == 0);
		assert(push(frontier, newNodeMul) == 0);
		assert(push(frontier, newNodeConcat) == 0);
	}

	return false;
}

i32 main(i32 argc, char** argv)
{
	if (argc < 2)
	{
		return -1;
	}

	char* filePath = argv[1];
	FILE* stream = fopen(filePath, "r");
	if (!stream) 
	{
		return -1;
	}
	char lineBuffer[MAX_LINE_SIZE];

	i64 resultNumber;
	vector components;
	initVector(&components, 8);
	i64 validSeriesSum = 0;

	queue frontier;
	initQueue(&frontier, 1000000);

	for (;;)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* doubleColonPtr;
		resultNumber = strtoll(lineBuffer, &doubleColonPtr, 10);
		assert(*doubleColonPtr == ':');
		char* currentPtr = doubleColonPtr + 1;
		for (;;)
		{
			if (*currentPtr == '\n')
			{
				break;
			}

			i64 component = strtol(currentPtr, &currentPtr, 10);
			push(&components, component);
		}
		
		if (produces(&components, &frontier, resultNumber))
		{
			validSeriesSum += resultNumber;
		}

		clear(&components);
		clear(&frontier);
	}

	printf("validSeriesSum: %ld\n", validSeriesSum);
	destroyVector(&components);
	destroyQueue(&frontier);
	return 0;
}


