#pragma once

#include "../../types.h"

typedef enum
{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN    = 1,
} Endianness;

Endianness get_endianness(void);

///////////////////////////
//// Memory management ////
///////////////////////////

/** Get the number of pages available on this system */
usize os_pages_count(void);

/** Get the page size of this system */
usize os_page_size(void);

/** Get total amount of ram available on this system */
static inline usize os_total_ram(void)
{
	return os_pages_count() * os_page_size();
}

/** Get the maximum amount of VM space we can request on this system */
usize os_addr_space_limit(void);

/** Reserve pages of virtual memory. The size should be page-aligned manually. */
memslice os_reserve(usize size_aligned);

/** Release a slice of virtual memory. The size should be page-aligned manually. */
void os_release(memslice slice_aligned);

/** Commit a slice of virtual memory. The address and size should be page-aligned manually. */
bool os_commit(memslice slice_aligned);

/** Uncommit a slice of virtual memory. The address and size should be page-aligned manually. */
bool os_uncommit(memslice slice_aligned);
