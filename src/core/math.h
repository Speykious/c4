#pragma once

#include "../types.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

usize align_to_usize(usize value, usize to);
