#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "types.h"
#include "app/app.h"
#include "audio/audio.h"
#include "core/alloc/arena.h"
#include "core/os.h"

typedef struct
{
	u8 r;
	u8 g;
	u8 b;
} Color;

int main(void)
{
	Arena arena = arena_init(os_total_ram());

	printf("allocating colors\n");
	for (usize i = 0; i < 1024UL * 1024UL; i++)
	{
		Color* color = arena_alloc(&arena, Color);

		color->r = 255;
		color->g = 128;
		color->b = 127;
	}
	printf("success\n");

	arena_free_all(&arena);

	/////////////////////////////////////////////////

	srand(time(NULL));

	AudioContext* audio_ctx = arena_alloc(&arena, AudioContext);
	if (audio_open(audio_ctx) == false)
	{
		fprintf(stderr, "could not open audio :(\n");
		return 1;
	}

	AudioBuffer audio_buffer;
	audio_buffer.len = 256;
	audio_buffer.ptr = arena_alloc_region(&arena, audio_buffer.len, 16);

	printf("sending frames\n");
	for (u32 j = 0; j < 16; j++)
	{
		printf("  frame #%d\n", j);

		for (usize i = 0; i < audio_buffer.len; i++)
			audio_buffer.ptr[i] = rand() & 0xff;

        audio_write(audio_ctx, audio_buffer);
	}

    printf("sent frames\n");
	audio_close(audio_ctx);

    ///////////////////////////////////

    printf("\nopening window\n");
	app_init();

	u32 const WIDTH  = 1280;
	u32 const HEIGHT = 720;

	C4_WindowOptions options = {
	    .flags       = WINDOW_FLAG_RESIZABLE | WINDOW_FLAG_CENTERED | WINDOW_FLAG_TRANSPARENT | WINDOW_FLAG_HIGH_DPI,
	    .position    = {.x = 0, .y = 0},
	    .size        = {.width = WIDTH, .height = HEIGHT},
	    .screen_mode = WINDOWED,
	    .title       = STR("C4 Window"),
	};

	C4_Window* window = NULL;
	if (app_create_window(options, &window) == false)
	{
		fprintf(stderr, "Nope, no window\n");
		return 1;
	}

    printf("  waiting for no reason\n");
    sleep(1);

    printf("closing window\n");
	app_close_window(window);
	return 0;
}
