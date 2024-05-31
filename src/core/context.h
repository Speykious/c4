#pragma once

// OS

#if defined(_WIN32)
#define OS_WINDOWS 1
#elif defined(__gnu_linux__)
#define OS_LINUX 1
#elif defined(__APPLE__) && defined(__MACH__)
#define OS_MAC 1
#else
#error Unsupported OS
#endif

#if !defined(OS_WINDOWS)
#define OS_WINDOWS 0
#endif
#if !defined(OS_LINUX)
#define OS_LINUX 0
#endif
#if !defined(OS_MAC)
#define OS_MAC 0
#endif

// Architecture

#if defined(__amd64__)
#define ARCH_X64 1
#elif defined(__i386__)
#define ARCH_X86 1
#elif defined(__arm__)
#define ARCH_ARM 1
#elif defined(__aarch64__)
#define ARCH_ARM64 1
#else
#error Unsupported architecture
#endif

#if !defined(ARCH_X64)
#define ARCH_X64 0
#endif
#if !defined(ARCH_X86)
#define ARCH_X86 0
#endif
#if !defined(ARCH_ARM)
#define ARCH_ARM 0
#endif
#if !defined(ARCH_ARM64)
#define ARCH_ARM64 0
#endif
