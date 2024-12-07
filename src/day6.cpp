#include <stdio.h>
#include <string.h>
#include "position_set.h"
#include "string.h"

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

	u16 xMax = 0;
	u16 yMax = 0;

	guard_position_entry patrol;
	position_set obstacleMap;
	initPositionSet(&obstacleMap, 11);
	for (u16 yPosition = 0;; ++yPosition, ++yMax)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		xMax = readBytes - 1;
		char* currentChar = lineBuffer;
		for (u16 xPosition = 0; xPosition < xMax; ++xPosition, ++currentChar)
		{
			switch (*currentChar)
			{
				case '.':
				{
					break;
				}
				case '^':
				{
					patrol.xPosition = xPosition;
					patrol.yPosition = yPosition;
					patrol.heading = NORTH;
	   				break;
				}
				case '#':
				{
					position_entry obstacle = { xPosition, yPosition };
					insert(&obstacleMap, obstacle);
				}
			}
		}
	}


	position_set guardPositions;
	initPositionSet(&guardPositions, 101);
	insert(&guardPositions, { patrol.xPosition, patrol.yPosition });
	for (;;)
	{
	  	u16 nextXPosition = patrol.xPosition;
	  	u16 nextYPosition = patrol.yPosition;

		switch (patrol.heading)
		{
			case NORTH:
			{
				--nextYPosition;
				break;
			}
			case EAST:
			{
				++nextXPosition;
				break;
			}
			case SOUTH:
			{
				++nextYPosition;
				break;
			}
			case WEST:
			{
				--nextXPosition;
				break;
			}
		}

		if (get(&obstacleMap, nextXPosition, nextYPosition))
		{
			patrol.heading = (guard_heading) ((patrol.heading + 1) % 4);
		}
	    else
		{
			patrol.xPosition = nextXPosition;
			patrol.yPosition = nextYPosition;

			if (nextXPosition < 0 || nextXPosition >= xMax 
				|| nextYPosition < 0 || nextYPosition >= yMax)
			{
				break;
			}
		  	else
			{
				position_entry guardPosition = { nextXPosition, nextYPosition };
				insert(&guardPositions, guardPosition);
			}
		}

	}

	u32 totalPositionCount = 0;
	position_bucket* currentBucket = guardPositions.buckets;
	for (int i = 0; i < guardPositions.len; ++i, ++currentBucket)
	{
		totalPositionCount += currentBucket->len;
	}

	printf("totalPositionCount: %d\n", totalPositionCount);

	destroyPositionSet(&obstacleMap);
	destroyPositionSet(&guardPositions);
	return 0;
}
