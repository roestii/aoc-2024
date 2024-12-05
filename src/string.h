#ifndef STRING_H 
#define STRING_H

#include "types.h"
#include <stdio.h>

#define MAX_LINE_SIZE 256

i32 splitOnce(char*, u32, char);
i32 readLine(char*, u32, FILE*);

#endif
