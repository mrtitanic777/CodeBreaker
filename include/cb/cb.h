/* cb/cb.h -- the CodeBreaker SDK: the version, starting and stopping the library, errors, strings and the handle types.
 *
 *     cb_init(NULL);                                   once, before any other call that does work
 *     cb_binary* b;
 *     if (cb_binary_open("lz32.dll", &b) != CB_OK)     every call that can fail returns a cb_status
 *         fprintf(stderr, "%s\n", cb_last_error());    and says why, in plain words, on this thread
 *     ...
 *     cb_binary_close(b);
 *     cb_shutdown();
 *
 * The SDK is a plain C interface: opaque handles (cb_binary, cb_function, cb_instructions), cb_status results, records that point
 * into the library's own structures (valid while the handle that gave them is open), and cb_string values the caller frees with
 * cb_string_free. Text is UTF-8 everywhere, paths included. cb.hpp wraps it for C++ (RAII handles, std::string_view, std::span).
 *
 * The other headers: binary.h (open any file), disasm.h (instructions of any described CPU), decompile.h (a function or a module
 * as C). Include them one by one, or include cb.hpp for all of them.
 */
#ifndef CB_CB_H
#define CB_CB_H

#include <stddef.h>
#include <stdint.h>

#include "cb/version.h"

/* CB_API marks a function of the library. A program linking cbsdk.lib needs nothing; CB_SDK_STATIC (the library's own tests) and
 * CB_SDK_BUILD (the library itself) make it empty. */
#if defined(CB_SDK_STATIC) || defined(CB_SDK_BUILD)
#define CB_API
#elif defined(_WIN32)
#define CB_API __declspec(dllimport)
#else
#define CB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** cb_status: what a call did. CB_OK is 0; every other value is a failure, and cb_last_error() says what failed and what to do.
 * @since 0.1
 */
typedef enum cb_status {
    CB_OK = 0,
    CB_ERROR_INVALID_ARGUMENT = 1, /* a NULL handle or output, an option the call does not take, a name it does not know */
    CB_ERROR_NOT_INITIALIZED = 2,  /* cb_init was not called (or every cb_init was matched by cb_shutdown) */
    CB_ERROR_NOT_FOUND = 3,        /* a file, a function, a processor or a program CodeBreaker needs is not there */
    CB_ERROR_IO = 4,               /* a file could not be read or written */
    CB_ERROR_FORMAT = 5,           /* the input is not what it says it is, or is damaged */
    CB_ERROR_UNSUPPORTED = 6,      /* the input is understood, but this does not do that with it yet */
    CB_ERROR_TOOL = 7,             /* a program CodeBreaker runs (the native decompiler) failed */
    CB_ERROR_BUFFER_TOO_SMALL = 8, /* a caller-owned buffer is too small; the call said how much is needed */
    CB_ERROR_INTERNAL = 9          /* a bug in CodeBreaker: please report it with the message */
} cb_status;

/** cb_version: the version of the library that is loaded, as CB_SDK_MAKE_VERSION makes it.
 * Compare it with CB_SDK_VERSION (the headers') to find a program running on another library than it was built with: the same major
 * version and a minor version at least the headers' is compatible.
 * @return the library's version number.
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API uint32_t cb_version(void);

/** cb_version_string: the version of the library that is loaded, as text ("0.1.0").
 * @return a string the library owns, valid while it is loaded.
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API const char* cb_version_string(void);

/** cb_status_name: the name of a status ("CB_ERROR_NOT_FOUND"), for logs.
 * @param status any value; one that is not a cb_status gives "CB_ERROR_UNKNOWN".
 * @return a string the library owns, valid while it is loaded.
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API const char* cb_status_name(cb_status status);

/** cb_last_error: what the last call on this thread that failed said: what failed, on which input, and what to do.
 * A call that succeeds does not change it.
 * @return a string the library owns, valid until the next call on this thread that fails; "" when no call has failed.
 * @threads Each thread has its own.
 * @since 0.1
 */
CB_API const char* cb_last_error(void);

/** cb_init_options: how cb_init sets the library up. Make one with CB_INIT_OPTIONS_INIT and change what you need.
 * @field struct_size sizeof(cb_init_options): lets a later library tell what an older program gave it.
 * @field data_root the folder of CodeBreaker's data (config/, languages/). NULL: CB_CHECKOUT_ROOT when set, else share/codebreaker beside
 *   the folder cbsdk.dll is in (the installed layout), else the CodeBreaker checkout the library was built in.
 * @field state_dir a folder CodeBreaker may write to (its caches and knowledge base). NULL: CB_STATE when set, else
 *   %LOCALAPPDATA%\CodeBreaker\sdk (Windows) or $HOME/.cache/codebreaker/sdk.
 * @since 0.1
 */
typedef struct cb_init_options {
    uint32_t struct_size;
    const char* data_root;
    const char* state_dir;
} cb_init_options;

/** CB_INIT_OPTIONS_INIT: the defaults of cb_init_options.
 * @since 0.1
 */
#define CB_INIT_OPTIONS_INIT {sizeof(cb_init_options), NULL, NULL}

/** cb_init: set the library up. Call it once before any other call that does work; each cb_init is matched by a cb_shutdown.
 * The first call decides the data root and the state folder. For this process it sets CB_CHECKOUT_ROOT, CB_STATE and, when the data root
 * has them, CB_LANGUAGES_DIR, CB_DECOMPILER_BIN and CB_THIRD_PARTY_BIN: the settings the engines read (a value already set is kept).
 * @param options NULL for the defaults.
 * @return CB_OK; CB_ERROR_INVALID_ARGUMENT for a struct_size this library does not know, or a later call naming another data root or
 *   state folder than the first; CB_ERROR_NOT_FOUND when the data root has no config/ folder; CB_ERROR_IO when the state folder cannot
 *   be made.
 * @threads Any thread; calls are serialized.
 * @since 0.1
 */
CB_API cb_status cb_init(const cb_init_options* options);

/** cb_shutdown: undo one cb_init. The last one frees what the library made (the decompiler and its caches).
 * Close every handle before the last cb_shutdown; a handle used after it is undefined behaviour. A cb_shutdown with no cb_init does nothing.
 * @threads Any thread, when no other call is running.
 * @since 0.1
 */
CB_API void cb_shutdown(void);

/** cb_string: text the library made for the caller: data is NUL-terminated UTF-8, size its length without the NUL.
 * The caller frees it with cb_string_free. A cb_string a call failed to fill is left {NULL, 0}.
 * @field data the text, NUL-terminated; NULL when empty or freed.
 * @field size its length in bytes, without the NUL.
 * @since 0.1
 */
typedef struct cb_string {
    char* data;
    size_t size;
} cb_string;

/** cb_string_free: free a cb_string the library made, and set it to {NULL, 0}.
 * @param string NULL, or a cb_string a call filled (freeing one twice is safe: the first sets it empty).
 * @threads Any thread, at any time (cb_init is not needed).
 * @since 0.1
 */
CB_API void cb_string_free(cb_string* string);

/** cb_binary: an open file of any format CodeBreaker reads (binary.h). Opaque; cb_binary_open makes one, cb_binary_close frees it.
 * @since 0.1
 */
typedef struct cb_binary cb_binary;

/** cb_function: one function of an open binary (binary.h, decompile.h). Opaque; owned by its binary, valid until it is closed.
 * @since 0.1
 */
typedef struct cb_function cb_function;

/** cb_instructions: the instructions one cb_disasm call read (disasm.h). Opaque; cb_instructions_free frees it.
 * @since 0.1
 */
typedef struct cb_instructions cb_instructions;

#ifdef __cplusplus
}
#endif

#endif
