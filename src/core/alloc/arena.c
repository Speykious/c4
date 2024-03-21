#include "arena.h"

#include "../math.h"
#include "../os.h"

Arena arena_init(usize size)
{
	memslice const buffer = os_reserve(size);

	Arena const arena = {
	    .curr_offset        = 0,
	    .uncommitted_offset = 0,
	    .buffer             = buffer,
	};

	return arena;
}

usize const PAGES_PER_COMMIT = 16;

void* arena_alloc_region(Arena* arena, usize size, usize align)
{
	usize const aligned_offset = align_to_usize(arena->curr_offset, align);
	usize const next_offset    = aligned_offset + size;

	// commit pages we don't have yet
	if (next_offset >= arena->uncommitted_offset)
	{
		memslice const commit_slice = {
		    .ptr = arena->buffer.ptr + arena->uncommitted_offset,
		    .len = os_page_size() * PAGES_PER_COMMIT,
		};

		bool const success = os_commit_unchecked(commit_slice);

		// probably out of memory
		if (success == false)
			return NULL;

		arena->uncommitted_offset = align_to_usize(next_offset, commit_slice.len);
	}

	u8* const addr     = arena->buffer.ptr + aligned_offset;
	arena->curr_offset = next_offset;

	return addr;
}

ArenaCheckpoint arena_checkpoint(Arena* arena)
{
	ArenaCheckpoint const checkpoint = {
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

void arena_destroy(Arena* arena)
{
	os_release(arena->buffer);
}
