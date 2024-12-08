#include "position_set.h"
#include <stdlib.h>

#define INITIAL_BUCKET_SIZE 8

void initPositionSet(position_set* map, u32 len)
{
	map->len = len;
	map->buckets = (position_bucket*) malloc(len * sizeof(position_bucket));
	position_bucket* currentBucket = map->buckets;

	for (int i = 0; i < len; ++i, ++currentBucket)
	{
		position_entry* initialEntries = (position_entry*) malloc(INITIAL_BUCKET_SIZE * sizeof(position_entry));
		currentBucket->len = 0;
		currentBucket->capacity = INITIAL_BUCKET_SIZE;
		currentBucket->entries = initialEntries;
	}
}

void destroyPositionSet(position_set* map)
{
	position_bucket* bucket = map->buckets;
	for (int i = 0; i < map->len; ++i, ++bucket)
	{
		free(bucket->entries);
	}

	free(map->buckets);
}

void push(position_bucket* bucket, position_entry entry)
{
	if (bucket->len == bucket->capacity)
	{
		bucket->capacity *= 2;
		position_entry* newEntries = (position_entry*) malloc(bucket->capacity * sizeof(position_entry));

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

// TODO(louis): // if x > y then x^2 + x + y
// else y^2 + x
void insert(position_set* map, position_entry entry)
{
	u32 key;
	u16 xPosition = entry.xPosition;
	u16 yPosition = entry.yPosition;

	if (xPosition > yPosition)
	{
		key = xPosition * xPosition + xPosition + yPosition;
	}
	else 
	{
		key = yPosition * yPosition + xPosition;
	}
	
	key = key % map->len;
	position_bucket* targetBucket = map->buckets + key;
	position_entry* currentEntry = targetBucket->entries;
	for (int i = 0; i < targetBucket->len; ++i, ++currentEntry)
	{
		if (currentEntry->xPosition == xPosition 
			&& currentEntry->yPosition == yPosition)
		{
			return;
		}
	}

	push(targetBucket, entry);
	++map->entryCount;
}

i32 get(position_set* map, i16 xPosition, i16 yPosition)
{
	u32 key;
	if (xPosition > yPosition)
	{
		key = xPosition * xPosition + xPosition + yPosition;
	}
	else 
	{
		key = yPosition * yPosition + xPosition;
	}
	
	key = key % map->len;
	position_bucket* targetBucket = map->buckets + key;
	position_entry* currentEntry = targetBucket->entries;

	for (int i = 0; i < targetBucket->len; ++i, ++currentEntry)
	{
		if (currentEntry->xPosition == xPosition 
			&& currentEntry->yPosition == yPosition)
		{
			return 1;
		}
	}

	return 0;
}

void initGuardPositionSet(guard_position_set* map, u32 len)
{
	map->len = len;
	map->buckets = (guard_position_bucket*) malloc(len * sizeof(guard_position_bucket));
	guard_position_bucket* currentBucket = map->buckets;

	for (int i = 0; i < len; ++i, ++currentBucket)
	{
		guard_position_entry* initialEntries = (guard_position_entry*) malloc(INITIAL_BUCKET_SIZE * sizeof(guard_position_entry));
		currentBucket->len = 0;
		currentBucket->capacity = INITIAL_BUCKET_SIZE;
		currentBucket->entries = initialEntries;
	}
}

void clear(guard_position_set* set)
{
	guard_position_bucket* currentBucket = set->buckets;
	for (int i = 0; i < set->len; ++i, ++currentBucket)
	{
		currentBucket->len = 0;
	}
}

void destroyGuardPositionSet(guard_position_set* map)
{
	guard_position_bucket* bucket = map->buckets;
	for (int i = 0; i < map->len; ++i, ++bucket)
	{
		free(bucket->entries);
	}

	free(map->buckets);
}

void push(guard_position_bucket* bucket, guard_position_entry entry)
{
	if (bucket->len == bucket->capacity)
	{
		bucket->capacity *= 2;
		guard_position_entry* newEntries = (guard_position_entry*) malloc(bucket->capacity * sizeof(guard_position_entry));

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

// TODO(louis): // if x > y then x^2 + x + y
// else y^2 + x
void insertGuard(guard_position_set* map, guard_position_entry entry)
{
	u32 key;
	u16 xPosition = entry.xPosition;
	u16 yPosition = entry.yPosition;
	guard_heading heading = entry.heading;

	if (xPosition > yPosition)
	{
		key = xPosition * xPosition + xPosition + yPosition;
	}
	else 
	{
		key = yPosition * yPosition + xPosition;
	}
	
	key = key % map->len;
	guard_position_bucket* targetBucket = map->buckets + key;
	guard_position_entry* currentEntry = targetBucket->entries;
	for (int i = 0; i < targetBucket->len; ++i, ++currentEntry)
	{
		if (currentEntry->xPosition == xPosition 
			&& currentEntry->yPosition == yPosition
			&& currentEntry->heading == heading)
		{
			return;
		}
	}

	push(targetBucket, entry);
}

i32 getGuard(guard_position_set* map, guard_position_entry needle)
{
	u16 xPosition = needle.xPosition;
	u16 yPosition = needle.yPosition;
	guard_heading heading = needle.heading;

	u32 key;
	if (xPosition > yPosition)
	{
		key = xPosition * xPosition + xPosition + yPosition;
	}
	else 
	{
		key = yPosition * yPosition + xPosition;
	}
	
	key = key % map->len;
	guard_position_bucket* targetBucket = map->buckets + key;
	guard_position_entry* currentEntry = targetBucket->entries;

	for (int i = 0; i < targetBucket->len; ++i, ++currentEntry)
	{
		if (currentEntry->xPosition == xPosition 
			&& currentEntry->yPosition == yPosition
			&& currentEntry->heading == heading)
		{
			return 1;
		}
	}

	return 0;
}
