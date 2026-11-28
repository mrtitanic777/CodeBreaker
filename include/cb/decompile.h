/* cb/decompile.h -- a function or a whole module as C.
 *
 *     const cb_function* f;
 *     cb_binary_function_find(binary, "LZCopy", &f);
 *     cb_string c = {0};
 *     if (cb_function_decompile(f, NULL, &c) == CB_OK) puts(c.data);
 *     cb_string_free(&c);
 *
 * One decompiler for every format: the readers take the file apart, the native decompiler (decompiler.exe, beside cbsdk.dll) lifts
 * each function from its processor's SLEIGH description, and the C is written from what it found. What it makes is kept in the state
 * folder (cb_init), keyed on everything it depends on, so asking again is fast.
 *
 * A 32-bit x86 function is decompiled by itself. A function of any other kind of module is cut out of its whole module's C, which is
 * made once per binary and kept with it (cb_module_decompile gives all of it).
 *
 * The decompile calls are serialized: one runs at a time in the process (the native decompiler already uses every core).
 */
#ifndef CB_DECOMPILE_H
#define CB_DECOMPILE_H

#include "cb/binary.h"
#include "cb/cb.h"

#ifdef __cplusplus
extern "C" {
#endif

/** cb_decompile_options: how a decompile call works. Make one with CB_DECOMPILE_OPTIONS_INIT.
 * @field struct_size sizeof(cb_decompile_options).
 * @field language the language to write: NULL or "c" (the only one in this version).
 * @since 0.1
 */
typedef struct cb_decompile_options {
    uint32_t struct_size;
    const char* language;
} cb_decompile_options;

/** CB_DECOMPILE_OPTIONS_INIT: the defaults of cb_decompile_options.
 * @since 0.1
 */
#define CB_DECOMPILE_OPTIONS_INIT {sizeof(cb_decompile_options), NULL}

/** cb_module_info: what cb_module_decompile made.
 * @field decompiled the functions written as C.
 * @field found the functions found.
 * @field library of found, those identified as the compiler's library and left out of the text.
 * @since 0.1
 */
typedef struct cb_module_info {
    size_t decompiled;
    size_t found;
    size_t library;
} cb_module_info;

/** cb_decompiler_check: whether the decompiler can run: its program and its processor descriptions are where the data root says.
 * @return CB_OK; CB_ERROR_NOT_FOUND naming what is missing; CB_ERROR_NOT_INITIALIZED before cb_init.
 * @threads Any thread.
 * @since 0.1
 */
CB_API cb_status cb_decompiler_check(void);

/** cb_function_decompile: one function as C.
 * @param function the function (cb_binary_function, cb_binary_function_find, cb_binary_function_at).
 * @param options NULL for the defaults.
 * @param out receives the C: the function with the comment lines above it. Free it with cb_string_free.
 * @return CB_OK; CB_ERROR_NOT_FOUND when the decompiler made no function at that address, or cannot run (cb_decompiler_check);
 *   CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c"; CB_ERROR_TOOL when
 *   the native decompiler failed; CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.
 * @ownership The caller owns the string.
 * @threads One decompile call runs at a time in the process; others wait.
 * @since 0.1
 */
CB_API cb_status cb_function_decompile(const cb_function* function, const cb_decompile_options* options, cb_string* out);

/** cb_module_decompile: every function of a binary as the text of one .c file.
 * @param binary the binary.
 * @param options NULL for the defaults.
 * @param out receives the C. Free it with cb_string_free.
 * @param info NULL, or receives how many functions were found and decompiled.
 * @return CB_OK; CB_ERROR_UNSUPPORTED for a file that is not an executable this version decompiles, or a language other than "c";
 *   CB_ERROR_NOT_FOUND when the decompiler cannot run (cb_decompiler_check); CB_ERROR_TOOL when the native decompiler failed;
 *   CB_ERROR_INVALID_ARGUMENT for a NULL argument; CB_ERROR_NOT_INITIALIZED before cb_init.
 * @ownership The caller owns the string. The binary keeps its own copy for cb_function_decompile.
 * @threads One decompile call runs at a time in the process; others wait.
 * @since 0.1
 */
CB_API cb_status cb_module_decompile(cb_binary* binary, const cb_decompile_options* options, cb_string* out, cb_module_info* info);

#ifdef __cplusplus
}
#endif

#endif
