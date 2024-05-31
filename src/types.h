#pragma once

#include <stddef.h>
#include <stdint.h>

// signed ints
typedef int8_t    i8;
typedef int16_t   i16;
typedef int32_t   i32;
typedef int64_t   i64;
typedef ptrdiff_t isize;

// unsigned ints
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef size_t   usize;

// floats
typedef float  f32;
typedef double f64;

// boolean
typedef u8 bool;
typedef int b32;

typedef void voidfn(void);

#define true  1
#define false 0

#define internal static

#define DEF_SLICE(T, name) \
	typedef struct         \
	{                      \
		T*    ptr;         \
		usize len;         \
	} name

DEF_SLICE(u8, memslice);
DEF_SLICE(char, String8);

#define S(lit)                                  \
	{                                           \
		.ptr = (char*)(lit), .len = sizeof(lit) \
	}

#define r_  /* readonly */ const
#define w_  /* writeonly */
#define rw_ /* readwrite */
