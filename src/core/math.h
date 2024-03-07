#pragma once

#include "types.h"

#define min(a, b) a < b ? a : b
#define max(a, b) a > b ? a : b

usize align_to_usize(usize value, usize to);
