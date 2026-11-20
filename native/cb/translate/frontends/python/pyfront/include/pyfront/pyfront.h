/* pyfront/pyfront.h -- the C ABI of pyfront, the Python 3 parser.
 *
 *   pyfront_tree* t = pyfront_parse(src, len, "file.py");
 *   if (pyfront_ok(t)) { size_t n; const char* d = pyfront_dump(t, 1, &n); ... }
 *   else printf("%s at %d:%d\n", pyfront_error_msg(t), pyfront_error_lineno(t), pyfront_error_offset(t));
 *   pyfront_free(t);
 *
 * The tree itself is reached from C++ through pyfront_module() (a pyfront::Module*). */
#ifndef PYFRONT_H
#define PYFRONT_H

#include <stddef.h>

#ifdef _WIN32
#  ifdef PYFRONT_BUILD
#    define PYFRONT_API __declspec(dllexport)
#  elif defined(PYFRONT_SHARED)
#    define PYFRONT_API __declspec(dllimport)
#  else
#    define PYFRONT_API
#  endif
#else
#  define PYFRONT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pyfront_tree pyfront_tree;

PYFRONT_API pyfront_tree* pyfront_parse(const char* source, size_t len, const char* filename);
PYFRONT_API int pyfront_ok(const pyfront_tree* t);
/* The dump text, owned by the tree; valid until pyfront_free. */
PYFRONT_API const char* pyfront_dump(pyfront_tree* t, int include_attributes, size_t* len);
PYFRONT_API const char* pyfront_error_msg(const pyfront_tree* t);
PYFRONT_API int pyfront_error_lineno(const pyfront_tree* t);
PYFRONT_API int pyfront_error_offset(const pyfront_tree* t);
PYFRONT_API int pyfront_error_end_lineno(const pyfront_tree* t);
PYFRONT_API int pyfront_error_end_offset(const pyfront_tree* t);
/* The pyfront::Module* (C++ callers), or NULL on error. */
PYFRONT_API void* pyfront_module(pyfront_tree* t);
PYFRONT_API void pyfront_free(pyfront_tree* t);

#ifdef __cplusplus
}
#endif

#endif
