#include "audio.h"

#define SAMPLE_RATE 44100

bool audio_open(AudioContext w_* audio_ctx)
{
	if (snd_pcm_open(&audio_ctx->pcm, "default", SND_PCM_STREAM_PLAYBACK, 0))
		return false;

	snd_pcm_format_t sample_format = SND_PCM_FORMAT_U8;
	snd_pcm_access_t access        = SND_PCM_ACCESS_RW_INTERLEAVED;

	u32  channels      = 1;
	u32  rate          = SAMPLE_RATE;
	bool soft_resample = true;
	u32  latency       = 1000;

	return snd_pcm_set_params(audio_ctx->pcm, sample_format, access, channels, rate, soft_resample, latency) == 0;
}

bool audio_available(AudioContext r_* audio_ctx)
{
	snd_pcm_sframes_t frame_count = snd_pcm_avail(audio_ctx->pcm);
	if (frame_count < 0)
		snd_pcm_recover(audio_ctx->pcm, frame_count, 0);
	return frame_count > 0;
}

void audio_write(AudioContext rw_* audio_ctx, AudioBuffer buffer)
{
	snd_pcm_sframes_t frame_count = snd_pcm_writei(audio_ctx->pcm, buffer.ptr, buffer.len);
	if (frame_count < 0)
		snd_pcm_recover(audio_ctx->pcm, frame_count, 0);
}

void audio_close(AudioContext rw_* audio_ctx)
{
	snd_pcm_close(audio_ctx->pcm);
}
