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

// [J2X-DOCTRINE fail-loud] An unhandled case is never silent: it REPORTS (once per subsystem and value, so a per-frame call can't
// flood the log) or is blessed as a deliberate no-op (J2X_NOOP). The GL layer (GL11.cpp) and its backends use these.
inline void J2X_ReportUnhandled(const char* sys, unsigned value) {
	static const char* seenSys[256];
	static unsigned seenVal[256];
	static int seenN = 0;
	for (int i = 0; i < seenN; ++i)
		if (seenVal[i] == value && seenSys[i] == sys) return;
	if (seenN < 256) { seenSys[seenN] = sys; seenVal[seenN] = value; ++seenN; }
	DbgPrintF("[UNHANDLED] %s: 0x%04X (%u) -- add a case, or J2X_NOOP if deliberate\n", sys, value, value);
}
#define J2X_UNHANDLED(sys, val) J2X_ReportUnhandled((sys), (unsigned)(val))
#define J2X_NOOP(reason)        ((void)0)   /* J2X-NOOP: a REVIEWED deliberate no-op; reason = why */

#endif // COMMON_H
