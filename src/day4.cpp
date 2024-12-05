#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "types.h"

#define XMAS "XMAS"
#define XMAS_SIZE (i32) (sizeof(XMAS) - 1)

u32 scanHorizontally(char* startChar, i32 col, u32 pitch)
{
	u32 count = 0;
	char* currentChar = startChar;
	if (col - XMAS_SIZE + 1 >= 0)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, --currentChar) 
		{
			if (*currentChar != XMAS[i])
			{
				goto forward_search;
			}
		}

		++count;
	}

forward_search:
	currentChar = startChar;
	if (col + XMAS_SIZE - 1 < pitch)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, ++currentChar) 
		{
			if (*currentChar != XMAS[i])
			{
				goto horizontal_exit;
			}
		}

		++count;
	}

horizontal_exit:
	return count;
}

u32 scanVertically(char* startChar, i32 row, i32 pitch, char* upperBound)
{
	u32 count = 0;
	char* currentChar = startChar;
	if (row - XMAS_SIZE + 1 >= 0)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar -= pitch) 
		{
			if (*currentChar != XMAS[i])
			{
				goto down_search;
			}
		}

		++count;
	}

down_search:
	currentChar = startChar;
	if (currentChar + pitch * (XMAS_SIZE - 1) < upperBound)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar += pitch) 
		{
			if (*currentChar != XMAS[i])
			{
				goto vertical_exit;
			}
		}

		++count;
	}

vertical_exit:
	return count;
}

u32 scanDiagonally(char* startChar, i32 row, i32 col, i32 pitch, char* upperBound)
{
	u32 count = 0;
	char* currentChar = startChar;
	bool leftPossible = col - XMAS_SIZE + 1 >= 0; 
	bool rightPossible = col + XMAS_SIZE - 1 < pitch; 

	bool upPossible = row - XMAS_SIZE + 1 >= 0; 
	bool downPossible = currentChar + pitch * (XMAS_SIZE - 1) < upperBound; 

	if (leftPossible && upPossible)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar -= pitch + 1)
	  	{
			if (*currentChar != XMAS[i])
			{
				goto left_down;
			}
		}

		++count;
	}

left_down:
	currentChar = startChar;
	if (leftPossible && downPossible)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar += pitch - 1)
	  	{
			if (*currentChar != XMAS[i])
			{
				goto right_up;
			}
		}

		++count;
	}

right_up:
	currentChar = startChar;
	if (rightPossible && upPossible)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar -= pitch - 1)
	  	{
			if (*currentChar != XMAS[i])
			{
				goto right_down;
			}
		}

		++count;
	}

right_down:
	currentChar = startChar;
	if (rightPossible && downPossible)
	{
		for (int i = 0; i < XMAS_SIZE; ++i, currentChar += pitch + 1)
	  	{
			if (*currentChar != XMAS[i])
			{
				goto diagonal_exit;
			}
		}

		++count;
	}

diagonal_exit:
	return count;
}

i32 main(i32 argc, char** argv)
{
	if (argc < 2)
	{
		return -1;
	}

	char* filePath = argv[1];
	i32 fd = open(filePath, O_RDONLY);

	if (fd < 0)
	{
		return -1;
	}

	struct stat fileStat;
	if (fstat(fd, &fileStat) < 0)
	{
		return -1;
	}

	char* fileContent = (char*) mmap(NULL, fileStat.st_size, 
								  	 PROT_READ, MAP_PRIVATE, fd, 0);

	if (!fileContent)
	{
		return -1;
	}

	char* limit = fileContent + fileStat.st_size;
	char* firstNewLine = (char*) memchr(fileContent, '\n', fileStat.st_size);
	u32 lineSize = firstNewLine - fileContent + 1;

	char* currentChar = fileContent;
	u32 totalCount = 0;

	for (int row = 0; currentChar < limit; ++row)
	{
	  	for (int col = 0; col < lineSize; ++col, ++currentChar)
	  	{
			if (*currentChar == 'X')
			{
				u32 horizontalCount = scanHorizontally(currentChar, col, lineSize);
				u32 verticalCount = scanVertically(currentChar, row, lineSize, limit);
				u32 diagonalCount = scanDiagonally(currentChar, row, col, lineSize, limit);

				totalCount += horizontalCount;
				totalCount += verticalCount;
				totalCount += diagonalCount;
			}
	  	}
	}

	printf("totalCount: %d\n", totalCount);
	if (munmap(fileContent, fileStat.st_size) < 0)
	{
		return -1;
	}
}
