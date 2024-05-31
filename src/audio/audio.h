#pragma once

#include "../types.h"
#include "../core/context.h"

DEF_SLICE(u8, AudioBuffer);

struct AudioContext;
typedef struct AudioContext AudioContext;

#if OS_WINDOWS
#error No Windows implementation for audio yet
#elif OS_LINUX
#include "audio_linux.h"
#elif OS_MAC
#error No MacOS implementation for audio yet
#endif

bool audio_open(AudioContext w_* audio_ctx);
bool audio_available(AudioContext r_* audio_ctx);
void audio_write(AudioContext rw_* audio_ctx, AudioBuffer buffer);
void audio_close(AudioContext rw_* audio_ctx);
