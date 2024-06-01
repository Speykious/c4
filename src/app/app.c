#include "../core/core_context.h"

#if OS_WINDOWS
#error No Windows implementation of the App layer yet
#elif OS_LINUX
// TODO: Think about Wayland on Linux and being able to choose backend at initialization
#include "x11/app_x11.c"
#include "x11/app_x11_keycode.c"
#elif OS_MAC
#error No MacOS implementation of the App layer yet
#endif
