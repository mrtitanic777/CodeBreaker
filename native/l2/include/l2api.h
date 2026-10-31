/* l2api.h -- the C interface of cbl2.dll (native/l2): the L2 core and the C back end, called by
 * codebreaker/translate/native_l2.py through ctypes.
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

#ifdef _WIN32
#ifdef CBL2_EXPORTS
#define CBL2_API __declspec(dllexport)
#else
#define CBL2_API __declspec(dllimport)
#endif
#else
#define CBL2_API
#endif

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
