#pragma once

#include <stdalign.h>

#include "../../types.h"

typedef struct Arena Arena;
struct Arena
{
	usize     curr_offset;
	usize     uncommitted_offset;
	u8* const buffer;
};

typedef struct ArenaCheckpoint ArenaCheckpoint;
struct ArenaCheckpoint
{
	usize const offset;
};

Arena arena_init(usize size);
void* arena_alloc_region(Arena* arena, usize size, usize align);

#define arena_alloc(arena, T) (T*)arena_alloc_region(arena, sizeof(T), alignof(T))

ArenaCheckpoint arena_checkpoint(Arena* arena);
void            arena_restore(Arena* arena, ArenaCheckpoint checkpoint);

void arena_free_all(Arena* arena);
