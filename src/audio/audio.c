#include "audio.h"

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

#define SAMPLE_RATE   44100
#define AUDIO_BUFSIZE 8192

struct _audio_context
{
	snd_pcm_t* pcm;
};

Endianness get_endianness(void)
{
	return (*(u8*)(&(u16){1})) ? LITTLE_ENDIAN : BIG_ENDIAN;
}

bool audio_open(AudioContext* audio_ctx)
{
	if (snd_pcm_open(&audio_ctx->pcm, "default", SND_PCM_STREAM_PLAYBACK, 0))
		return false;

	snd_pcm_format_t sample_format;
	switch (get_endianness())
	{
		case LITTLE_ENDIAN:
			sample_format = SND_PCM_FORMAT_FLOAT_LE;
			break;
		case BIG_ENDIAN:
			sample_format = SND_PCM_FORMAT_FLOAT_BE;
			break;
	}

	snd_pcm_access_t access = SND_PCM_ACCESS_RW_INTERLEAVED;

	u32  channels      = 1;
	u32  rate          = SAMPLE_RATE;
	bool soft_resample = true;
	u32  latency       = 1000;

	return snd_pcm_set_params(audio_ctx->pcm, sample_format, access, channels, rate, soft_resample, latency) == 0;
}

bool audio_available(AudioContext* audio_ctx)
{
	snd_pcm_sframes_t frame_count = snd_pcm_avail(audio_ctx->pcm);
	if (frame_count < 0)
		snd_pcm_recover(audio_ctx->pcm, frame_count, 0);
	return frame_count > 0;
}

void audio_write(AudioContext* audio_ctx, AudioBuffer buffer)
{
	int r = snd_pcm_writei(audio_ctx->pcm, buffer.ptr, buffer.len);
	if (r < 0)
		snd_pcm_recover(audio_ctx->pcm, r, 0);
}

void audio_close(AudioContext* audio_ctx)
{
	snd_pcm_close(audio_ctx->pcm);
}
