#pragma once

#include "../../types.h"

#define DEF_LLIST(T, name) \
	typedef struct         \
	{                      \
		T*    head;        \
		T*    tail;        \
		usize count;       \
	} name

DEF_LLIST(void, LListAny);
