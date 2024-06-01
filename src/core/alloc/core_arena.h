#pragma once

#include <stdalign.h>

#include "../../types.h"

typedef struct Arena Arena;
struct Arena
{
	usize    curr_offset;
	usize    uncommitted_offset;
	memslice buffer;
};

typedef struct ArenaCheckpoint ArenaCheckpoint;
struct ArenaCheckpoint
{
	usize const offset;
};

Arena arena_init(usize size);
void* arena_alloc_region(Arena* arena, usize size, usize align);

#define arena_alloc_n(arena, T, n) (T*)arena_alloc_region(arena, n * sizeof(T), alignof(T))
#define arena_alloc(arena, T)      (T*)arena_alloc_region(arena, sizeof(T), alignof(T))

ArenaCheckpoint arena_checkpoint(Arena* arena);
void            arena_restore(Arena* arena, ArenaCheckpoint checkpoint);

void arena_free_all(Arena* arena);
void arena_destroy(Arena* arena);
