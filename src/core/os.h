#include "types.h"

/** Get the number of pages available on this system */
usize os_pages_count(void);

/** Get the page size of this system */
usize os_page_size(void);

/** Get total amount of ram available on this system */
usize os_total_ram(void);

/** Get the maximum amount of VM space we can request on this system */
usize os_addr_space_limit(void);

void* os_reserve(usize const size);
void  os_release(memslice const slice);

/** Commit a slice of memory. The address and size should be page-aligned manually. */
bool os_commit_unchecked(memslice const slice_aligned);

/** Commit a slice of memory. The address and size get automatically page-aligned. */
bool os_commit(memslice const slice);

/** Uncommit a slice of memory. The address and size should be page-aligned manually. */
bool os_uncommit_unchecked(memslice const slice_aligned);

/** Uncommit a slice of memory. The address and size get automatically page-aligned. */
bool os_uncommit(memslice const slice);
