// XEXMagic - compatibility shim for the ported PPC decoder.
#ifndef XEXMAGIC_PPC_COMPAT_ASSERT_H_
#define XEXMAGIC_PPC_COMPAT_ASSERT_H_

#include <cassert>

// The ported PPC decoder was written for Nexia, where it only ever sees valid
// guest code, so it asserts on anything it does not recognise. XEXMagic runs it
// over a whole image - data, padding and instruction encodings the table does
// not cover - so a decode miss is normal, and the decoder is built to return
// PPCOpcode::kInvalid for it (every caller checks for that). These asserts must
// therefore be inert: aborting would kill the tool on the first unrecognised
// word. This matches a Release build, where NDEBUG already makes them no-ops -
// a Debug build must not behave differently and crash where Release does not.
#define assert_always(...) ((void)0)
#define assert_true(expr, ...) ((void)0)
#define assert_false(expr, ...) ((void)0)
#define assert_zero(expr, ...) ((void)0)
#define assert_not_zero(expr, ...) ((void)0)
#define assert_null(expr, ...) ((void)0)
#define assert_not_null(expr, ...) ((void)0)
#define assert_unhandled_case(var) ((void)0)

#endif
