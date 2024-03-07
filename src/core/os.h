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
void  os_release(void* const addr, usize const size);

/** Commit a slice of memory. The address and size should be page-aligned manually. */
bool os_commit_unchecked(void* const addr_aligned, usize const size_aligned);

/** Commit a slice of memory. The address and size get automatically page-aligned. */
bool os_commit(void* const addr, usize const size);

/** Uncommit a slice of memory. The address and size should be page-aligned manually. */
bool os_uncommit_unchecked(void* const addr_aligned, usize const size_aligned);

/** Uncommit a slice of memory. The address and size get automatically page-aligned. */
bool os_uncommit(void* const addr, usize const size);
