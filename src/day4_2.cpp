#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "types.h"


#define KERNEL_SIZE 3
#define MS "MS"
#define SM "SM"

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

	u32 totalCount = 0;

	char middle;
	char diagonalOne[2];
	char diagonalTwo[2];

	for (char* currentLine = fileContent; 
		 currentLine + lineSize * (KERNEL_SIZE - 1) < limit; 
		 currentLine += lineSize)
	{
		for (int i = 0; i < lineSize - (KERNEL_SIZE - 1); ++i)
		{
			char* middleLine = currentLine + lineSize;
			middle = middleLine[i + 1];

			if (middle != 'A')
			{
				continue;
			}


			char* bottomLine = currentLine + 2 * lineSize;

			diagonalOne[0] = currentLine[i];	
			diagonalTwo[0] = currentLine[i + 2];
			diagonalOne[1] = bottomLine[i + 2];	
			diagonalTwo[1] = bottomLine[i];	

			if ((memcmp(diagonalOne, MS, sizeof(diagonalOne)) == 0 || memcmp(diagonalOne, SM, sizeof(diagonalOne)) == 0)
				&& (memcmp(diagonalTwo, MS, sizeof(diagonalTwo)) == 0 || memcmp(diagonalTwo, SM, sizeof(diagonalTwo)) == 0))
			{
				++totalCount;
			}
		}
	}


	printf("totalCount: %d\n", totalCount);
	if (munmap(fileContent, fileStat.st_size) < 0)
	{
		return -1;
	}
}
