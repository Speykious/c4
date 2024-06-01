#include "../core/core_context.h"

#if OS_WINDOWS
#error No Windows implementation the audio layer yet
#elif OS_LINUX
#include "audio_linux.c"
#elif OS_MAC
#error No MacOS implementation of the audio layer yet
#endif
