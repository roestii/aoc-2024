#include <stdlib.h>
#include <string.h>
#include "string.h"

enum series_kind 
{
	INCREASING,
	DECREASING,
	NONE
};

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

	for (int i = 0;; ++i)
	{
		i32 readBytes = readLine(lineBuffer, MAX_LINE_SIZE, stream);
		if (readBytes < 0)
		{
			break;
		}

		char* max = lineBuffer + readBytes;
		series_kind seriesKind = NONE;
		i64 lastNumber = atol(lineBuffer);

		char* currentPointer = (char*) memchr(lineBuffer, ' ', readBytes);
		while (currentPointer)
		{
			i64 currentNumber = atol(currentPointer);
			++currentPointer;

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
						goto unsafe;
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
						goto unsafe;
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
						goto unsafe;
					}
				}
	  		}

			lastNumber = currentNumber;
			currentPointer = (char*) memchr(currentPointer, ' ', max - currentPointer - 1);
		}

		++safeCount;
	unsafe:
		continue;
	}

	printf("safe count: %d\n", safeCount);
	return 0;
}
