/* l2api.h -- the one entry point of the L2 core library (cb/translate/l2, the library cb_l2): the L2 core and the
 * back ends, called in-process by cb::translate::native_l2::Library::builtin().
 *
 *   int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen);
 *
 * `fn` names the entry point (backends/c.py's: function, flags, signature, type_text, ...; the L2
 * core's: roundtrip, validate, set_schema); `in` is its arguments as a JSON array in the tagged form
 * (json.hpp: a tuple {"$t": [...]}, a set {"$s": [...]}, a dict of other keys {"$d": [[k, v]...]}).
 * 0: `out` is the result in the same form. 1: `out` is {"type": the Python exception's name
 * (Decline, KeyError, ValueError ...), "message": ..., "facts": the facts as the call left them}.
 * Free `out` with l2_free. Calls may run on any thread at once (each has its own state). */
#ifndef CBL2_API_H
#define CBL2_API_H
#include <stddef.h>

/* (a static library linked into codebreaker.exe: nothing is exported or imported) */
#define CBL2_API

#ifdef __cplusplus
extern "C" {
#endif

CBL2_API int l2_version(void);
CBL2_API int l2_call(const char* fn, const char* in, size_t inlen, char** out, size_t* outlen);
CBL2_API void l2_free(char* p);

#ifdef __cplusplus
}
#endif
#endif
