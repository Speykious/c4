#include "../types.h"

// Weirdest shit I've ever seen. Once again StackOverflow saves the day.
// https://stackoverflow.com/a/43256599
#ifndef _POSIX_C_SOURCE
#define __TEMP_POSIX
#define _POSIX_C_SOURCE 200809L
#endif

#include <alsa/asoundlib.h>

#ifdef __TEMP_POSIX
#undef _POSIX_C_SOURCE
#endif

DEF_SLICE(u8, AudioBuffer);

typedef struct
{
	snd_pcm_t* pcm;
} AudioContext;

bool audio_open(AudioContext out audio_ctx);
bool audio_available(AudioContext* audio_ctx);
void audio_write(AudioContext* audio_ctx, AudioBuffer buffer);
void audio_close(AudioContext* audio_ctx);
