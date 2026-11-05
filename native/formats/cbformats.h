/* cbformats.h -- the C interface of CodeBreaker's file-format readers (formats.dll).
 *
 * The readers of codebreaker/formats/ (PE, NE, LE, MZ/DOS, OMF, XEX ...) in C++.  Each reader
 * is a set of plain C++ classes over a byte buffer (pe/pe.h, ne/ne.h, ...); this interface lets
 * the Python callers keep asking the questions they asked the Python readers and get the very
 * same answers back.
 *
 * One entry point.  A call names a question ("pe.exports", "ne.segments", ...), hands over the
 * file's bytes (borrowed for the call, never kept) and the question's arguments, and gets the
 * answer back as one value.  Arguments and answers travel as Python pickle streams (protocol 4,
 * the subset of plain values: None, bool, int, str, bytes, tuple, list, dict, set): the one
 * value format Python reads and writes at C speed without this library depending on a Python
 * version or Python's headers.  core/pyvalue.h writes and reads it.
 *
 * Nothing is kept between calls: every call reads what it needs from the bytes it is given, so
 * calls are independent, may run on any thread, and a caller holds no handle to free.
 *
 * Loaded with ctypes by codebreaker/formats/native.py.
 */
#ifndef CBFORMATS_H
#define CBFORMATS_H

#include <stddef.h>
#include <stdint.h>
#include <wchar.h>

#ifdef _WIN32
#define CBF_API __declspec(dllexport)
#else
#define CBF_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CBF_VERSION 1

/* cbf_call results */
#define CBF_OK        0   /* *out is the answer */
#define CBF_RAISED    1   /* *out is (exception name, message): what the Python reader raises */
#define CBF_FALLBACK  2   /* *out is a reason: this one is answered by the Python reader */
#define CBF_NO_SUCH   3   /* *out is the question asked: no reader answers it */

/* Ask one question.  `question` is "<reader>.<name>"; `data` / `size` the bytes (may be NULL
 * when the question takes none); `args` / `args_size` a pickled tuple of its arguments.  The
 * answer (*out, *out_size) stays valid until the next call on the same thread. */
CBF_API int cbf_call(const char *question, const uint8_t *data, size_t size,
                     const uint8_t *args, size_t args_size,
                     const uint8_t **out, size_t *out_size);

/* CBF_VERSION, to tell a stale build from the one the shim expects. */
CBF_API int cbf_version(void);

/* The unicorn.dll to run emulated reads on (a packed DOS program's unpacker): 0, or nonzero with
   `err` filled. Without it such a read reports the unpacker not undone, as Python without
   `unicorn` did. */
CBF_API int cbf_unicorn(const wchar_t *path, char *err, size_t errlen);

/* The questions answered, one per line ("pe.exports\npe.imports\n..."). */
CBF_API const char *cbf_questions(void);

#ifdef __cplusplus
}
#endif

#endif
