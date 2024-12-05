#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "string.h"
#include "vector.h"
#include <assert.h>

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

	vector leftList;
	vector rightList;

	initVector(&leftList, 10);
	initVector(&rightList, 10);

	for (;;)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* spacePointer = (char*) memchr(lineBuffer, ' ', readBytes);
		if (!spacePointer)
		{
			return -1;
		}

		i64 leftNumber = atol(lineBuffer);
		i64 rightNumber = atol(spacePointer);

		push(&leftList, leftNumber);
		push(&rightList, rightNumber);
	}

	assert(leftList.len == rightList.len);

	sort(&leftList);
	sort(&rightList);

	i64 distanceSum = 0;
	for (int i = 0; i < leftList.len; ++i)
	{
		i64 leftNumber = leftList.items[i];
		i64 rightNumber = rightList.items[i];

		distanceSum += abs(leftNumber - rightNumber);
	}


	u64 totalSimilarityScore = 0;
	for (int i = 0; i < leftList.len; ++i)
	{
		i64 leftItem = leftList.items[i];
		u32 similarityScore = sortedCountFast(&rightList, leftItem);
		totalSimilarityScore += leftItem * similarityScore;

	}

	printf("part one: %d\n", distanceSum);
	printf("part two: %d\n", totalSimilarityScore);

	destroyVector(&leftList);
	destroyVector(&rightList);

	return 0;
}
