#include "arena.h"

#include "../math.h"
#include "../os.h"

Arena arena_init(usize size)
{
	void* const addr = os_reserve(size);

	Arena arena = {
	    .curr_offset        = 0,
	    .uncommitted_offset = 0,
	    .buffer             = (u8*)addr,
	};

	return arena;
}

void* arena_alloc_region(Arena* arena, usize size, usize align)
{
	usize const aligned_offset = align_to_usize(arena->curr_offset, align);
	usize const next_offset    = aligned_offset + size;

	// commit pages we don't have yet
	if (next_offset >= arena->uncommitted_offset)
	{
		usize const commit_size             = os_page_size();
		usize const next_uncommitted_offset = align_to_usize(next_offset, commit_size);

		u8* const commit_addr = arena->buffer + arena->uncommitted_offset;

		bool const success = os_commit_unchecked(commit_addr, commit_size);

		// probably out of memory
		if (success == false)
			return NULL;

		arena->uncommitted_offset = next_uncommitted_offset;
	}

	void* const addr   = arena->buffer + aligned_offset;
	arena->curr_offset = next_offset;

	return addr;
}

ArenaCheckpoint arena_checkpoint(Arena* arena)
{
	ArenaCheckpoint checkpoint = {
	    .offset = arena->curr_offset,
	};

	return checkpoint;
}

void arena_restore(Arena* arena, ArenaCheckpoint checkpoint)
{
	arena->curr_offset = checkpoint.offset;
}

void arena_free_all(Arena* arena)
{
	arena->curr_offset = 0;
}
