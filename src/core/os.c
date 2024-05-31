#include "context.h"

#if OS_WINDOWS
#error No Windows implementation of the OS layer yet
#elif OS_LINUX
#include "native/linux/os.c"
#elif OS_MAC
#error No MacOS implementation of the OS layer yet
#endif
