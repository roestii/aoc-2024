#include <string.h>
#include "string.h"

i32 splitOnce(char* str, u32 strLen, char delimiter)
{
	return 0;
}

i32 readLine(char* result, u32 size, FILE* fileStream)
{
	i64 beforeOffset = ftell(fileStream);
	if (!fgets(result, size, fileStream))
	{
		return -1;
	}

	char* newLineChr = (char*) memchr(result, '\n', size);
	if (!newLineChr)
	{
		return -1;
	}

	i32 readBytes = 1 + newLineChr - result;
	if (fseek(fileStream, beforeOffset + readBytes, SEEK_SET) < 0) 
	{
		return -1;
	}

	return readBytes;
}
