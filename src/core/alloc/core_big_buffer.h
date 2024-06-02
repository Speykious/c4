#pragma once

#include <stdalign.h>

#include "../../types.h"

/** An allocator for a single big buffer that is typically bigger than a page, like a framebuffer. */
typedef struct
{
	memslice buffer;
	usize    size;
	usize    uncommitted_offset;
} BigBuffer;

BigBuffer big_buffer_init(usize vm_size, usize size);
void      big_buffer_resize(BigBuffer rw_* big_buffer, usize size);
void      big_buffer_zero(BigBuffer rw_* big_buffer);
void      big_buffer_destroy(BigBuffer rw_* big_buffer);
