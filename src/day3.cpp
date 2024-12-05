#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include "types.h"

#define MUL "mul("

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

	struct stat fileStats; 
	if (fstat(fd, &fileStats) < 0)
	{
		return -1;
	}

	char* fileContent = (char*) mmap(NULL, fileStats.st_size, 
								  	 PROT_READ, MAP_PRIVATE, fd, 0);

	if (!fileContent)
	{
		return -1;
	}

	char* currentPointer = fileContent;
	char* maxPointer = fileContent + fileStats.st_size - 1;

	i64 result = 0;
	for (; currentPointer <= maxPointer;) 
	{
		currentPointer = (char*) memmem(currentPointer, maxPointer - currentPointer, 
						  	     		MUL, sizeof(MUL) - 1);
		if (!currentPointer)	
		{
			break;
		}

		currentPointer += sizeof(MUL) - 1;
		char* endPointer;
		i32 a = strtol(currentPointer, &endPointer, 10);
		if (currentPointer == endPointer)
		{
			continue;
		}

		currentPointer = endPointer;
		if (*currentPointer != ',')
		{
			continue;
		}

		++currentPointer;
		i32 b = strtol(currentPointer, &endPointer, 10);
		if (currentPointer == endPointer)
		{
			continue;
		}

		currentPointer = endPointer;
		if (*currentPointer != ')')
		{
			continue;
		}

		result += a * b;
	}

	printf("product: %d\n", result);

	if (munmap(fileContent, fileStats.st_size) < 0) 
	{
		return -1;
	}

	return 0;
}
