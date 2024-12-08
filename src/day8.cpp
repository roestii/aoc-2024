#include <stdlib.h>
#include "string.h"
#include "character_map.h"
#include "position_set.h"

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

	u16 xMax;
	u16 yMax = 0;

	character_map antennas;
	position_set antiNodePositions;

	initCharacterMap(&antennas);
	initPositionSet(&antiNodePositions, 29);

	char lineBuffer[MAX_LINE_SIZE];
	for (u16 yPosition = 0;; ++yPosition, ++yMax)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		xMax = readBytes - 1;
		char* currentChar = lineBuffer;
		for (u16 xPosition = 0; xPosition < readBytes - 1; ++xPosition, ++currentChar)
		{
			switch (*currentChar)
			{
				case '.':
				{
					break;
				}
				default:
				{
					insert(&antennas, (u8) *currentChar, xPosition, yPosition);

					break;
				}
			}
		}
	}
	
	character_position_bucket* currentAntennaBucket = antennas.buckets;
	for (int i = 0; i < UINT8_MAX; ++i, ++currentAntennaBucket)
	{
	  	u32 bucketLen = currentAntennaBucket->len;
		character_position_entry* antennaA = currentAntennaBucket->entries;
		for (int j = 0; j < bucketLen; ++j, ++antennaA)
		{
			character_position_entry* antennaB = currentAntennaBucket->entries;
			for (int k = 0; k < bucketLen; ++k, ++antennaB)
			{
				if (j == k)
				{
					continue;
				}

				i16 antennaAX = (i16) antennaA->xPosition;
				i16 antennaAY = (i16) antennaA->yPosition;

				i16 antennaBX = (i16) antennaB->xPosition;
				i16 antennaBY = (i16) antennaB->yPosition;

				i16 deltaX = antennaAX - antennaBX;
				i16 deltaY = antennaAY - antennaBY;

				position_entry antiNodePosition = 
				{
					antennaAX,
					antennaAY
				};

				for (;;)
				{
					antiNodePosition.xPosition -= deltaX;
					antiNodePosition.yPosition -= deltaY;

					if (antiNodePosition.xPosition < xMax 
						&& antiNodePosition.xPosition >= 0
						&& antiNodePosition.yPosition < yMax
						&& antiNodePosition.yPosition >= 0)
					{
						insert(&antiNodePositions, antiNodePosition);
					}
				 	else
					{
						break;
					}
				}

				antiNodePosition = 
				{
					antennaBX,
					antennaBY
				};

				for (;;)
				{
					antiNodePosition.xPosition += deltaX;
					antiNodePosition.yPosition += deltaY;

					if (antiNodePosition.xPosition < xMax 
						&& antiNodePosition.xPosition >= 0
						&& antiNodePosition.yPosition < yMax
						&& antiNodePosition.yPosition >= 0)
					{
						insert(&antiNodePositions, antiNodePosition);
					}
				 	else
					{
						break;
					}
				}
			}
		}
	}

	printf("antiNodeCount: %d\n", antiNodePositions.entryCount);

	destroyCharacterMap(&antennas);
	destroyPositionSet(&antiNodePositions);
	return 0;
}


