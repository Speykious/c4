#pragma once

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

struct AudioContext
{
	snd_pcm_t* pcm;
};
