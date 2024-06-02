#pragma once

#include "../types.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

usize align_to_usize(usize value, usize to);

// Byte units

#define KB (1000)
#define MB (1000 * KB)
#define GB (1000 * MB)
#define TB (1000 * GB)

#define KIB (1024)
#define MIB (1024 * KIB)
#define GIB (1024 * MIB)
#define TIB (1024 * GIB)
