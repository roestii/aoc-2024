#include <stdio.h>
#include <string.h>
#include <assert.h>

#include "string.h"
#include "vector.h"
#include "queue.h"

bool produces(vector* v, queue* frontier, i64 result)
{
	i64* components = v->items;
	tree_node startNodeAdd = { components[0], 0, ADDITION };
	tree_node startNodeMul = { components[0], 0, MULTIPLICATION };

	assert(push(frontier, startNodeAdd) == 0);
	assert(push(frontier, startNodeMul) == 0);

	tree_node currentItem;
	while (pop(&currentItem, frontier) == 0)
	{
		if (currentItem.intermediateResult == result 
			&& currentItem.idx == v->len - 1)
		{
			return true;
		}

		u32 newIdx = currentItem.idx + 1;
		if (newIdx >= v->len)
		{
			continue;
		}

		switch (currentItem.operand)
		{
			case ADDITION:
			{
				i64 newIntermediateResult = currentItem.intermediateResult + 
											components[newIdx];

				tree_node newNodeAdd = { newIntermediateResult, newIdx, ADDITION };
				tree_node newNodeMul = { newIntermediateResult, newIdx, MULTIPLICATION };

				assert(push(frontier, newNodeAdd) == 0);
				assert(push(frontier, newNodeMul) == 0);

				break;
			}
			case MULTIPLICATION:
			{
				i64 newIntermediateResult = currentItem.intermediateResult *
											components[newIdx];

				tree_node newNodeAdd = { newIntermediateResult, newIdx, ADDITION };
				tree_node newNodeMul = { newIntermediateResult, newIdx, MULTIPLICATION };

				assert(push(frontier, newNodeAdd) == 0);
				assert(push(frontier, newNodeMul) == 0);

				break;
			}
		}
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
	initQueue(&frontier, 10000);

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
			printf("got valid series with result: %ld\n", resultNumber);
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


