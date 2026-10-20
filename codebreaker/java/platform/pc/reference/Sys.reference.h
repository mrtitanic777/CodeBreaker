// LWJGL org.lwjgl.Sys on the PC: the timer on SDL's millisecond clock.
#ifndef SYS_H
#define SYS_H
#include <string>
#include "JavaString.h"

namespace Sys {
	long long getTime();                                  // milliseconds (getTimerResolution() == 1000)
	inline long long getTimerResolution() { return 1000LL; }
	inline JavaString getVersion() { return JavaString("2.9.4 (j2x pc)"); }
	inline void openURL(...) {}
	inline void alert(...) {}
	inline bool is64Bit() { return sizeof(void*) == 8; }
}
#endif // SYS_H
