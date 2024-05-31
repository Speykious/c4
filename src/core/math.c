#include "./math.h"

usize align_to_usize(usize value, usize to)
{
	const usize aligned = value + to - 1;
	return aligned - aligned % to;
}
