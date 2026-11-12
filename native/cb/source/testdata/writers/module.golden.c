/* demo.dll -- reconstructed source.
 *
 * STATUS, generated 2026-11-12 by CodeBreaker (verifier settings prof1).
 * 7 functions:
 *   VERIFIED                         2   28.6%
 *   VERIFIED (weak)                  0    0.0%
 *   IDENTIFIED (runtime library)     1   14.3%
 *   UNVERIFIED                       1   14.3%
 *   NOT JUDGED                       0    0.0%
 *   DOES NOT COMPILE                 1   14.3%
 *   NO SOURCE                        2   28.6%
 *
 * Builds as a whole module: not built here (run `codebreaker build demo.dll`).
 * Functions are compiled and checked one at a time; the module's data (globals, tables,
 * resources) is referred to by its address in the original unless the whole module is built.
 * Runs in place of the original: NOT TESTED.
 *
 * VERIFIED = rebuilt from this text with OpenWatcom and run beside the original in an x86 emulator:
 * same return value, same calls out of the module with the same arguments, same memory changed,
 * same stack and saved registers, on every input tried. It is evidence, not proof. Names of
 * internal functions (sub_...) and locals are placeholders; no original names are claimed.
 * Written in after judging, changing no code: the SDK's names for constants passed to
 * Windows (with their values in demo.h), and names for locals, parameters and globals
 * taken from the Windows functions they are passed to or hold the result of.
 * IDENTIFIED = the function's bytes are those of a function in a library that came with the
 * compiler; it is named after that function. It is reconstructed and judged all the same.
 * Imports by number are written with their real names (1 of them; demo.h lists the
 * module and number of each). The text that was built and judged calls them MODULE_number.
 */

#include "demo.h"

/* [VERIFIED] DllMain  @ 00401000 -- 100% of it exercised; exported as _DllMain@12: DllMain */
/* readable */ int DllMain(void) { return GetDlgItem(1); }

/* [UNVERIFIED] CreateThing  @ 00401010 -- returns a different value; no caller here uses what it returns */
/* readable */ void CreateThing(void) { sub_00401020(); }

/* [DOES NOT COMPILE] sub_00401020  @ 00401020 -- error C2143: syntax error; it makes the thing */
/* readable */ this is not C: CreateThing();

/* [IDENTIFIED (runtime library)] _strlen  @ 00401030 -- LIBC; not measured on the current verifier */

/* [NO SOURCE] sub_00401040  @ 00401040 -- the decompiler gave nothing */
/* (no reconstruction) */

/* [VERIFIED] _memcpy  @ 00401050 -- 100% of it exercised; this is _memcpy of LIBC */
/* readable */ void *_memcpy(void *a) { return a; }

/* [NO SOURCE] @ 00401060 -- not measured on the current verifier */
