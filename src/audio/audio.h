#include "../types.h"

typedef enum
{
	LITTLE_ENDIAN = 0,
	BIG_ENDIAN    = 1,
} Endianness;

Endianness get_endianness(void);

DEF_SLICE(f32, AudioBuffer);

typedef struct _audio_context AudioContext;

bool audio_open(AudioContext out audio_ctx);
bool audio_available(AudioContext* audio_ctx);
void audio_write(AudioContext* audio_ctx, AudioBuffer buffer);
void audio_close(AudioContext* audio_ctx);
