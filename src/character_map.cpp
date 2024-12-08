#include <stdlib.h>
#include "character_map.h"

#define INITIAL_BUCKET_SIZE 8

void initCharacterMap(character_map* map)
{
	map->buckets = (character_position_bucket*) malloc(UINT8_MAX * sizeof(character_position_bucket));
	character_position_bucket* currentBucket = map->buckets;

	for (int i = 0; i < UINT8_MAX; ++i, ++currentBucket)
	{
		character_position_entry* initialEntries = (character_position_entry*) malloc(INITIAL_BUCKET_SIZE * sizeof(character_position_entry));
		currentBucket->len = 0;
		currentBucket->capacity = INITIAL_BUCKET_SIZE;
		currentBucket->entries = initialEntries;
	}
}

void destroyCharacterMap(character_map* map)
{
	character_position_bucket* bucket = map->buckets;
	for (int i = 0; i < UINT8_MAX; ++i, ++bucket)
	{
		free(bucket->entries);
	}

	free(map->buckets);
}

void push(character_position_bucket* bucket, character_position_entry entry)
{
	if (bucket->len == bucket->capacity)
	{
		bucket->capacity *= 2;
		character_position_entry* newEntries = (character_position_entry*) malloc(bucket->capacity * sizeof(character_position_entry));

		for (int i = 0; i < bucket->len; ++i)
		{
			newEntries[i] = bucket->entries[i];
		}

		free(bucket->entries);
		bucket->entries = newEntries;
	}

	bucket->entries[bucket->len] = entry;
	++bucket->len;
}

void insert(character_map* map, u8 chr, u16 xPosition, u16 yPosition)
{
	character_position_bucket* targetBucket = map->buckets + chr;
	character_position_entry* currentEntry = targetBucket->entries;
	for (int i = 0; i < targetBucket->len; ++i, ++currentEntry)
	{
		if (currentEntry->xPosition == xPosition 
			&& currentEntry->yPosition == yPosition)
		{
			return;
		}
	}

	push(targetBucket, { xPosition, yPosition });
}

i32 get(character_position_bucket* result, character_map* map, u8 chr)
{
	character_position_bucket* targetBucket = map->buckets + chr;
	if (targetBucket->len == 0)
	{
		return -1;
	}
	else
	{
		*result = *targetBucket;
		return 0;
	}
}
