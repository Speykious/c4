#include <stdio.h>
#include <unistd.h>

#include "core/alloc/arena.h"
#include "core/os.h"

typedef struct Color Color;
struct Color
{
	f32 r;
	f32 g;
	f32 b;
};

int main(void)
{
	Arena arena = arena_init(os_total_ram());

	printf("Allocating colors\n");
	for (usize i = 0; i < 1024UL * 1024UL; i++)
	{
		Color* color = arena_alloc(&arena, Color);

		color->r = 255;
		color->g = 128;
		color->b = 727;
	}
	printf("Success\n");

	return 0;
}
