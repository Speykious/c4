#pragma once

#include "../types.h"

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

usize align_to_usize(usize value, usize to);

typedef enum
{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN    = 1,
} Endianness;

Endianness get_endianness(void);
