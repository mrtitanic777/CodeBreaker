// Common.h -- the PC platform layer's shared bits: the debug print the generated entry and the
// references use (DbgPrintF, to stderr) and the Win32 types Thread.h runs on.
#ifndef COMMON_H
#define COMMON_H
#include "stdafx.h"
#include <cstdio>
#include <cstdarg>
#ifdef _WIN32
#include <windows.h>
#endif

inline void DbgPrintF(const char* fmt, ...) {
	va_list a;
	va_start(a, fmt);
	std::vfprintf(stderr, fmt, a);
	va_end(a);
	std::fflush(stderr);
}

#endif // COMMON_H
