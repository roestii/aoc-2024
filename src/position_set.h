#ifndef POSITION_SET_H 
#define POSITION_SET_H

#include "types.h"

enum guard_heading
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};

struct guard_position_entry
{
	u16 xPosition;
	u16 yPosition;
	guard_heading heading;
};

struct guard_position_bucket
{
	u32 len;
	u32 capacity;
	guard_position_entry* entries;
};

struct guard_position_set
{
	u32 len;
	guard_position_bucket* buckets;
};

struct position_entry
{
	u16 xPosition;
	u16 yPosition;
};

struct position_bucket
{
	u32 len;
	u32 capacity;
	position_entry* entries;
};

struct position_set
{
	u32 len;
	position_bucket* buckets;
};

void initPositionSet(position_set*, u32);
void destroyPositionSet(position_set*);
void insert(position_set*, position_entry);
i32 get(position_set*, u32, u32);

void initGuardPositionSet(guard_position_set*, u32);
void destroyGuardPositionSet(guard_position_set*);
void insertGuard(guard_position_set*, guard_position_entry);
void clear(guard_position_set*);
i32 getGuard(guard_position_set*, guard_position_entry);

#endif
