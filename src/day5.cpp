#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "string.h"
#include "types.h"
#include "rule_vector.h"
#include "vector.h"

i32 readRules(rule_vector* result, FILE* stream)
{
	char lineBuffer[MAX_LINE_SIZE];
	for (;;)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		if (lineBuffer[0] == '\n')
		{
			break;
		}

		char* resultPtr;
		i32 firstPage = strtol(lineBuffer, &resultPtr, 10);

		if (*resultPtr != '|')
		{
			return -1;
		}

		++resultPtr;
		i32 secondPage = strtol(resultPtr, NULL, 10);

		ord_rule rule = { firstPage, secondPage };
		push(result, rule);
	}

	return 0;
}

i32 partOne(i64* result, FILE* stream, rule_vector* rules)
{
	char lineBuffer[MAX_LINE_SIZE];
	vector update;
	initVector(&update, 10);
	for (;;)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* currentPtr;
		i32 lastPageNumber = strtol(lineBuffer, &currentPtr, 10);
		push(&update, lastPageNumber);
		assert(*currentPtr == ',');

		for (;;)
	  	{
			if (*currentPtr == '\n')
			{
				break;
			}

			++currentPtr;
			i32 currentPageNumber = strtol(currentPtr, &currentPtr, 10);
			push(&update, currentPageNumber);
			ord_rule needle = { lastPageNumber, currentPageNumber };
			if (findOrdRule(rules, needle) < 0)
			{
				goto next_iteration;
			}
			lastPageNumber = currentPageNumber;
	  	}
		
		{
			u32 middleIdx = update.len / 2;
			*result += update.items[middleIdx];
		}
	next_iteration:
		clear(&update);
	}

	destroyVector(&update);
	return 0;
}

i32 findMinimumPage(i64* result, vector* update, rule_vector* rules)
{
	i64* currentPage = update->items;
	for (int i = 0; i < update->len; ++i, ++currentPage)
	{
		bool minimum = true;
		i64* nextPage = update->items;
		for (int j = 0; j < update->len; ++j, ++nextPage)
		{
			if (i == j)
			{
				continue;
			}

	  		ord_rule needle = { *currentPage, *nextPage };
	 		if (findOrdRule(rules, needle) < 0)
			{
	  			minimum = false;
			}
		}

	  	if (minimum)
	  	{
	   		*result = *currentPage;
	  		return i;
	  	}
	}

	// NOTE(louis): this should not happen
	return -1;
}

i32 partTwo(i64* result, FILE* stream, rule_vector* rules)
{
	char lineBuffer[MAX_LINE_SIZE];
	vector sortedUpdate;
	vector update;
	initVector(&update, 10);
	initVector(&sortedUpdate, 10);

	for (;;)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* currentPtr;
		i64 lastPageNumber = strtol(lineBuffer, &currentPtr, 10);
		push(&update, lastPageNumber);
		assert(*currentPtr == ',');

		for (;;)
	  	{
			if (*currentPtr == '\n')
			{
				break;
			}

			++currentPtr;
			i32 currentPageNumber = strtol(currentPtr, &currentPtr, 10);
			push(&update, currentPageNumber);
			lastPageNumber = currentPageNumber;
	  	}

		assert(update.len > 0);
	  	i64 previousPageNumber = update.items[0];
		bool isInvalid = false;
		for (int i = 1; i < update.len; ++i)
		{
			i64 currentPageNumber = update.items[i];
			ord_rule needle = { previousPageNumber , currentPageNumber };

			if (findOrdRule(rules, needle) < 0)
			{
				isInvalid = true;
				break;
			}

			previousPageNumber = currentPageNumber;
		}

		if (isInvalid)
		{
			while (update.len > 0)
			{
				i64 minimumPage;
				i32 minimumPageIndex = findMinimumPage(&minimumPage, &update, rules);

				if (minimumPageIndex < 0)
				{
					return -1;
				}

				if (remove(&update, minimumPageIndex) < 0)
				{
					return -1;
				}

				push(&sortedUpdate, minimumPage);
			}

			u32 middleIdx = sortedUpdate.len / 2;
			*result += sortedUpdate.items[middleIdx];
	   		clear(&sortedUpdate);
		}

		clear(&update);
	}

	destroyVector(&update);
	return 0;
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

	rule_vector rules;
	initVector(&rules, 10);
	if (readRules(&rules, stream) < 0)
	{
		return -1;
	}

	i64 pageUpdateStart = ftell(stream);
	i64 middlePageNumberSum;
	if (partOne(&middlePageNumberSum, stream, &rules) < 0)
	{
		return -1;
	}

	printf("part one: %d\n", middlePageNumberSum);
	if (fseek(stream, pageUpdateStart, SEEK_SET) < 0)
	{
		return -1;
	}

	middlePageNumberSum = 0;
	if (partTwo(&middlePageNumberSum, stream, &rules) < 0)
	{
		return -1;
	}
	printf("part two: %d\n", middlePageNumberSum);

	destroyVector(&rules);
	return 0;
}
