#include "../context.h"

#if OS_WINDOWS
#error No Windows implementation of the OS layer yet
#elif OS_LINUX
#include "os_linux.c"
#elif OS_MAC
#error No MacOS implementation of the OS layer yet
#endif

Endianness get_endianness(void)
{
	return (*(u8*)(&(u16){1})) ? LITTLE_ENDIAN : BIG_ENDIAN;
}
