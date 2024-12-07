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

	guard_position_entry guardStartPosition;
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
					guardStartPosition.xPosition = xPosition;
					guardStartPosition.yPosition = yPosition;
					guardStartPosition.heading = NORTH;
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


	u32 loopCount = 0;

	// TODO(louis): reintroduce the position map including the heading and check whether the guard 
	// has visited the position already with the corresponding heading
	guard_position_set pastGuardPositions;
	initGuardPositionSet(&pastGuardPositions, 101); 
	guard_position_entry currentGuardPosition; 

	// TODO(louis): Parallelize this
	for (int obstructionX = 0; obstructionX < xMax; ++obstructionX)
	{
		for (int obstructionY = 0; obstructionY < yMax; ++obstructionY)
		{
			if (get(&obstacleMap, obstructionX, obstructionY) || 
				(obstructionX == guardStartPosition.xPosition && obstructionY == guardStartPosition.yPosition)) 
			{
				continue;
			}

			currentGuardPosition = guardStartPosition;
			insertGuard(&pastGuardPositions, guardStartPosition);

			for (int i = 0;; ++i)
			{
				u16 nextXPosition = currentGuardPosition.xPosition;
				u16 nextYPosition = currentGuardPosition.yPosition;

				switch (currentGuardPosition.heading)
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

				if (get(&obstacleMap, nextXPosition, nextYPosition) 
					|| (nextXPosition == obstructionX && nextYPosition == obstructionY))
				{
					currentGuardPosition.heading = (guard_heading) ((currentGuardPosition.heading + 1) % 4);
				}
				else
				{
					currentGuardPosition.xPosition = nextXPosition;
					currentGuardPosition.yPosition = nextYPosition;

					if (nextXPosition < 0 || nextXPosition >= xMax 
						|| nextYPosition < 0 || nextYPosition >= yMax)
					{
						clear(&pastGuardPositions);
						break;
					}
				}

				if (!getGuard(&pastGuardPositions, currentGuardPosition))
				{
					insertGuard(&pastGuardPositions, currentGuardPosition);
				}
				else
				{
					++loopCount;
					clear(&pastGuardPositions);
					break;
				}
			}
		}
	}

	printf("loopCount: %d\n", loopCount);
	destroyPositionSet(&obstacleMap);
	destroyGuardPositionSet(&pastGuardPositions);

	return 0;
}

