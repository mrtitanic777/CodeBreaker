// Common.h -- the PC platform layer's shared bits, for the PC's Direct3D 9 build: what the PC's own Common.h gives every file (the
// debug print DbgPrintF, to stderr, and the Win32 types Thread.h runs on), and, in the Direct3D 9 layer's own files (GL11.cpp,
// GLU.cpp, Display.cpp), Direct3D 9, the D3DX names it uses (J2xD3DX9.h), the device and the diagnostics macros (J2X_UNHANDLED,
// J2X_NOOP). The Xbox 360 has its own Common.h (the XDK's d3dx9, the emulator's trace sink); the layer includes "Common.h" either way.
//
// A layer file defines J2X_WITH_GDI before stdafx.h: d3d9.h needs the GDI types, which stdafx.h keeps out of the game's own
// files (wingdi.h's Polygon() and Rectangle() would hide a game class of that name), so only those files see Direct3D.
#ifndef COMMON_H
#define COMMON_H

#include "stdafx.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#endif

// The trace sink: standard error, one line per message. J2X_QUIET=1 in the environment silences it.
inline void DbgPrintF(const char* fmt, ...) {
	static int quiet = -1;
	if (quiet < 0) { const char* q = getenv("J2X_QUIET"); quiet = (q && *q && *q != '0') ? 1 : 0; }
	if (quiet) return;
	va_list ap;
	va_start(ap, fmt);
	vfprintf(stderr, fmt, ap);
	va_end(ap);
	fflush(stderr);
}

// [J2X-DOCTRINE fail-loud] An unhandled case is never silent: it REPORTS (once per subsystem and value, so a per-frame call can't
// flood the log) or is blessed as a deliberate no-op (J2X_NOOP). Same contract as the Xbox 360's Common.h.
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

#if defined(J2X_WITH_GDI)
#include "J2xD3DX9.h"                     // d3d9.h and the D3DX names
extern IDirect3D9*       g_pD3D;          // made by Display::create (Display.cpp)
extern IDirect3DDevice9* g_pd3dDevice;
#endif

#endif // COMMON_H
