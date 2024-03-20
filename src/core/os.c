#include "os.h"

#include <sys/mman.h>
#include <sys/resource.h>
#include <unistd.h>

#include "math.h"

inline usize os_pages_count(void)
{
	return sysconf(_SC_PHYS_PAGES);
}

inline usize os_page_size(void)
{
	return sysconf(_SC_PAGE_SIZE);
}

usize os_total_ram(void)
{
	usize const pages     = os_pages_count();
	usize const page_size = os_page_size();
	return pages * page_size;
}

usize os_addr_space_limit(void)
{
	struct rlimit addr_space_limit = {0};
	getrlimit(RLIMIT_AS, &addr_space_limit);
	return addr_space_limit.rlim_cur;
}

memslice os_reserve(usize size)
{
	memslice const slice = {
	    .ptr = mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0),
	    .len = size,
	};

	return slice;
}

void os_release(memslice slice)
{
	munmap(slice.ptr, slice.len);
}

inline bool os_commit_unchecked(memslice slice_aligned)
{
	return mprotect(slice_aligned.ptr, slice_aligned.len, PROT_READ | PROT_WRITE) == 0;
}

bool os_commit(memslice slice)
{
	usize const page_size = os_page_size();

	memslice const slice_aligned = {
	    .ptr = (void*)align_to_usize((usize)slice.ptr, page_size),
	    .len = align_to_usize(slice.len, page_size),
	};

	return os_commit_unchecked(slice_aligned);
}

inline bool os_uncommit_unchecked(memslice slice_aligned)
{
	return mprotect(slice_aligned.ptr, slice_aligned.len, PROT_NONE) == 0;
}

bool os_uncommit(memslice slice)
{
	usize const page_size = os_page_size();

	memslice const slice_aligned = {
	    .ptr = (void*)align_to_usize((usize)slice.ptr, page_size),
	    .len = align_to_usize(slice.len, page_size),
	};

	return os_uncommit_unchecked(slice_aligned);
}
