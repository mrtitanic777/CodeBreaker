// mingw_compat.cpp -- CodeBreaker's: what MinGW's C library lacks that the kernel core reaches.
//
// SIMDe rounds through __builtin_roundevenf, which GCC turns into a call to C23's roundevenf
// when it cannot inline it; MinGW's libm has no roundevenf. Round half to even is what
// nearbyintf does in the default rounding mode, which the host keeps.
#if defined(__MINGW32__)
#include <cmath>

extern "C" float roundevenf(float v) { return std::nearbyintf(v); }
extern "C" double roundeven(double v) { return std::nearbyint(v); }
#endif
