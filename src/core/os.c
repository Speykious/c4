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
	usize pages     = os_pages_count();
	usize page_size = os_page_size();
	return pages * page_size;
}

usize os_addr_space_limit(void)
{
	struct rlimit addr_space_limit = {0};
	getrlimit(RLIMIT_AS, &addr_space_limit);
	return addr_space_limit.rlim_cur;
}

void* os_reserve(usize const size)
{
	return mmap(NULL, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
}

void os_release(void* const addr, usize const size)
{
	munmap(addr, size);
}

inline bool os_commit_unchecked(void* const addr_aligned, usize const size_aligned)
{
	return mprotect(addr_aligned, size_aligned, PROT_READ | PROT_WRITE) == 0;
}

bool os_commit(void* const addr, usize const size)
{
	usize page_size = os_page_size();

	usize size_aligned = align_to_usize(size, page_size);
	u8*   addr_aligned = (u8*)align_to_usize((usize)addr, page_size);

	return os_commit_unchecked(addr_aligned, size_aligned);
}

inline bool os_uncommit_unchecked(void* const addr_aligned, usize const size_aligned)
{
	return mprotect(addr_aligned, size_aligned, PROT_NONE) == 0;
}

bool os_uncommit(void* const addr, usize const size)
{
	usize page_size = os_page_size();

	usize size_aligned = align_to_usize(size, page_size);
	u8*   addr_aligned = (u8*)align_to_usize((usize)addr, page_size);

	return os_uncommit_unchecked(addr_aligned, size_aligned);
}
