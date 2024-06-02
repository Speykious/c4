#include "core_big_buffer.h"

#include <sanitizer/asan_interface.h>

#include "../core_math.h"
#include "../os/core_os.h"

BigBuffer big_buffer_init(usize vm_size, usize size)
{
	BigBuffer big_buffer = {
	    .buffer             = os_reserve(align_to_usize(vm_size, os_page_size())),
	    .size               = size,
	    .uncommitted_offset = 0,
	};

	big_buffer_resize(&big_buffer, size);
	return big_buffer;
}

void big_buffer_resize(BigBuffer rw_* big_buffer, usize size)
{
	if (big_buffer->size == size)
		return;

	usize next_uncommitted_offset = align_to_usize(size, os_page_size());

	if (next_uncommitted_offset > big_buffer->uncommitted_offset)
	{
		memslice const commit_slice = {
		    .ptr = big_buffer->buffer.ptr + big_buffer->uncommitted_offset,
		    .len = next_uncommitted_offset - big_buffer->uncommitted_offset,
		};

		os_commit(commit_slice);
	}
	else if (next_uncommitted_offset < big_buffer->uncommitted_offset)
	{
		memslice const uncommit_slice = {
		    .ptr = big_buffer->buffer.ptr + next_uncommitted_offset,
		    .len = big_buffer->uncommitted_offset - next_uncommitted_offset,
		};

		os_uncommit(uncommit_slice);
	}

	if (size > big_buffer->size)
	{
		// unpoison from previous size to new bigger size
		u8* addr = big_buffer->buffer.ptr + big_buffer->size;
		ASAN_UNPOISON_MEMORY_REGION(addr, size - big_buffer->size);

		// poison leftover of last page
		u8* leftover = big_buffer->buffer.ptr + size;
		ASAN_POISON_MEMORY_REGION(leftover, next_uncommitted_offset - size);
	}
	else if (size < big_buffer->size)
	{
		// poison from new size to previous bigger size
		u8* addr = big_buffer->buffer.ptr + size;
		ASAN_POISON_MEMORY_REGION(addr, big_buffer->size - size);
	}

	big_buffer->size               = size;
	big_buffer->uncommitted_offset = next_uncommitted_offset;
}

void big_buffer_zero(BigBuffer* big_buffer)
{
    u64* buffer = (u64*)big_buffer->buffer.ptr;
	for (usize i = 0; i < (big_buffer->buffer.len / sizeof(u64)); i++)
		buffer[i] = 0;
}

void big_buffer_destroy(BigBuffer* big_buffer)
{
	os_release(big_buffer->buffer);
}
