#ifndef CHARACTER_MAP_H 
#define CHARACTER_MAP_H

#include "types.h"

struct character_position_entry 
{
	u16 xPosition;
	u16 yPosition;
};

struct character_position_bucket 
{
	u32 len;
	u32 capacity;
	character_position_entry* entries;
};

struct character_map 
{
	character_position_bucket* buckets;
};

void initCharacterMap(character_map*);
void destroyCharacterMap(character_map*);
void insert(character_map*, u8, u16, u16);
i32 get(character_position_bucket*, character_map*, u8);

#endif
