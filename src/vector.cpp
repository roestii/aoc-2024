#include "vector.h"
#include <string.h>

void initVector(vector* v, u32 capacity)
{
	v->capacity = capacity;
	v->len = 0;
	v->items = (i64*) malloc(capacity * sizeof(i64));
}

void destroyVector(vector* v)
{
	free(v->items);
}

void clear(vector* v)
{
	v->len = 0;
}

void push(vector* v, i64 item)
{
	if (v->len == v->capacity)
	{
		v->capacity *= 2;
		i64* newItems = (i64*) malloc(v->capacity * sizeof(i64));

		for (int i = 0; i < v->len; ++i)
		{
			newItems[i] = v->items[i];
		}

		free(v->items);
		v->items = newItems;
	}

	v->items[v->len] = item;
	++v->len;
}



i32 binarySearch(i64* items, i64 needle, u32 leftIdx, u32 rightIdx) 
{
	if (rightIdx - leftIdx <= 1)
	{
		if (items[leftIdx] != needle)
		{
			return -1;
		}
		else
		{
			return leftIdx;
		}
	}

	u32 middle = (leftIdx + rightIdx) / 2;
	if (needle > items[middle])
	{
		return binarySearch(items, needle, middle + 1, rightIdx);
	}
	else if (needle < items[middle])
	{
		return binarySearch(items, needle, leftIdx, middle);
	}
	else
	{
		return middle;
	}
}

u32 sortedCountFast(vector* v, i64 needle)
{
	i32 idx = binarySearch(v->items, needle, 0, v->len);
	if (idx < 0) 
	{
		return 0;
	}
	else
	{
		u32 count = 1;
		i64* currentItem = v->items + idx + 1;
		i64* max = v->items + v->len;

		for (; currentItem < max; ++currentItem) 
		{
			if (*currentItem == needle)
			{
				++count;
			}
		   	else
			{
				break;
			}
		}

		currentItem = v->items + idx - 1;
		for (; currentItem >= v->items; --currentItem) 
		{
			if (*currentItem == needle)
			{
				++count;
			}
		   	else
			{
				break;
			}
		}

		return count;
	}
}

u32 sortedCount(vector* v, i64 needle)
{
	// we cou
	// NOTE(louis): the caller has to ensure the list is sorted.
	bool found = false;
	u32 count = 0;
	i64* currentItem = v->items;

	for (int i = 0; i < v->len; ++i, ++currentItem)
	{
		if (found && *currentItem != needle)
		{
			break;
		}
	    else if (!found && *currentItem == needle)
		{
			++count;
			found = true;
		}
		else if (found && *currentItem == needle)
		{
			++count;
		}
	}

	return count;
}

void mergeSort(i64* items, i64* sideBuffer, u32 leftIdx, u32 rightIdx)
{
	if (rightIdx - leftIdx <= 1)
	{
		return;
	}

	u32 middleIdx = (rightIdx + leftIdx) / 2;
	mergeSort(items, sideBuffer, leftIdx, middleIdx);
	mergeSort(items, sideBuffer, middleIdx, rightIdx);

	for (int i = leftIdx; i < rightIdx; ++i) 
	{
		sideBuffer[i] = items[i];
	}

	for (int i = leftIdx, j = middleIdx, k = leftIdx; k < rightIdx; ++k)
	{
		if (i < middleIdx && j < rightIdx) 
		{
			if (sideBuffer[i] < sideBuffer[j])
			{
				items[k] = sideBuffer[i];
				++i;
			}
			else 
			{
				items[k] = sideBuffer[j];
				++j;
			}
		}
		else if (i < middleIdx)
		{
			items[k] = sideBuffer[i];
			++i;
		}
		else if (j < rightIdx)
		{
			items[k] = sideBuffer[j];
			++j;
		}
		else 
		{
			break;
		}
	}
}

void sort(vector* v)
{
	i64* sideBuffer = (i64*) malloc(v->capacity * sizeof(i64));
	mergeSort(v->items, sideBuffer, 0, v->len);
	free(sideBuffer);
}


