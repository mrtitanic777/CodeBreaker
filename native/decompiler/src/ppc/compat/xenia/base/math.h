// XEXMagic - compatibility shim for the ported PPC decoder.
#ifndef XEXMAGIC_PPC_COMPAT_MATH_H_
#define XEXMAGIC_PPC_COMPAT_MATH_H_

#include <cstdint>

// Nexia picks these up from xenia/base/platform.h via its own math.h.
#ifndef XE_MAYBE_UNUSED
#if defined(__cplusplus) && __cplusplus >= 201703L
#define XE_MAYBE_UNUSED [[maybe_unused]]
#else
#define XE_MAYBE_UNUSED
#endif
#endif

#ifndef XE_FORCEINLINE
#define XE_FORCEINLINE inline
#endif
#ifndef XE_NOINLINE
#define XE_NOINLINE
#endif
#ifndef XE_COLD
#define XE_COLD
#endif
#ifndef XE_LIKELY
#define XE_LIKELY(x) (x)
#define XE_UNLIKELY(x) (x)
#endif

#endif
