#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "string.h"
#include "vector.h"

enum series_kind 
{
	INCREASING,
	DECREASING,
	NONE
};

bool seriesIsSafeIgnoreIdx(vector* levelSeries, u32 skipIdx)
{
	u32 seriesLength = levelSeries->len;
	if (seriesLength == 0)
	{
		assert(false);
		return -1;
	}

	series_kind seriesKind = NONE;
	i64* levels = levelSeries->items;
	i64 lastNumber;
	u32 start;

	if (skipIdx > 0)
	{
		lastNumber = levels[0];
		start = 1;
	}
	else
	{
		lastNumber = levels[1];
		start = 2;
	}

	for (int i = start; i < seriesLength; ++i)
	{
		if (i == skipIdx)
		{
			continue;
		}

		i64 currentNumber = levels[i];
		switch (seriesKind)
		{
			case NONE:
			{
				if (currentNumber > lastNumber && currentNumber - lastNumber <= 3)
				{
					seriesKind = INCREASING;
				}		
				else if (currentNumber < lastNumber && lastNumber - currentNumber <= 3)
				{	
					seriesKind = DECREASING;
				}
				else
				{
					return 0;
				}

				break;
			}
			case INCREASING:
			{
				if (currentNumber > lastNumber && currentNumber - lastNumber <= 3)
				{
					break;
				}
				else 
				{
					return 0;
				}
			}
			case DECREASING:
			{
				if (currentNumber < lastNumber && lastNumber - currentNumber <= 3)
				{
					break;
				}		
				else 
				{
					return 0;
				}
			}
		}

		lastNumber = currentNumber;
	}

	return 1;
}

i32 main(i32 argc, char** argv)
{
	if (argc < 2) 
	{
		return -1;
	}

	char* filePath = argv[1];
	FILE* stream = fopen(filePath, "r");
	char lineBuffer[MAX_LINE_SIZE];
	u32 safeCount = 0;

	vector levelSeries;
	initVector(&levelSeries, 10);

	for (int i = 0;; ++i)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* max = lineBuffer + readBytes;
		char* currentPointer = lineBuffer;

		while (currentPointer)
		{
			i64 currentNumber = atol(currentPointer);
			push(&levelSeries, currentNumber);
			++currentPointer;
			currentPointer = (char*) memchr(currentPointer, ' ', max - currentPointer - 1);
		}

		for (int j = 0; j < levelSeries.len; ++j)
		{
			if (seriesIsSafeIgnoreIdx(&levelSeries, j))
			{
				++safeCount;
				break;
			}
		}

		// printf("line: %d, safeCount: %d\n", i, safeCount);
		clear(&levelSeries);
	}

	destroyVector(&levelSeries);
	printf("safe count: %d\n", safeCount);
	return 0;
}
