#include "../../os.h"

#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

inline usize os_pages_count(void)
{
	return sysconf(_SC_PHYS_PAGES);
}

inline usize os_page_size(void)
{
	return sysconf(_SC_PAGE_SIZE);
}

usize os_addr_space_limit(void)
{
	struct rlimit addr_space_limit = {0};
	getrlimit(RLIMIT_AS, &addr_space_limit);
	return addr_space_limit.rlim_cur;
}

memslice os_reserve(usize size)
{
	return (memslice){
	    .ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0),
	    .len = size,
	};
}

void os_release(memslice slice)
{
	munmap(slice.ptr, slice.len);
}

bool os_commit(memslice slice_aligned)
{
	return mprotect(slice_aligned.ptr, slice_aligned.len, PROT_READ | PROT_WRITE) == 0;
}

bool os_uncommit(memslice slice_aligned)
{
	return mprotect(slice_aligned.ptr, slice_aligned.len, PROT_NONE) == 0;
}
