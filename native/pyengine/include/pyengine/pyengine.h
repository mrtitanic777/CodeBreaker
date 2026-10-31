/* pyengine.h -- the porting engine's C interface (pyengine_api.dll).
 *
 *   pyengine_run *r = pyengine_run_package(root, package_or_null, "strict", 1, out_dir_or_null);
 *   if (pyengine_ok(r)) {
 *       size_t n; const char *ledger = pyengine_ledger_json(r, &n);   // {"q": {"status", "reason", "lines", "l2"}}
 *       const char *unit = pyengine_unit_json(r, "codebreaker.formats.ne", &n);   // the module's L2 Unit, or NULL
 *   } else puts(pyengine_error(r));
 *   pyengine_free(r);
 *
 * Strings stay valid until pyengine_free. Everything is UTF-8.
 */
#ifndef PYENGINE_H
#define PYENGINE_H
#include <stddef.h>

#ifdef _WIN32
#ifdef PYENGINE_BUILD
#define PYENGINE_API __declspec(dllexport)
#else
#define PYENGINE_API __declspec(dllimport)
#endif
#else
#define PYENGINE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pyengine_run pyengine_run;

/* policy: "strict" or "checked"; extended: 0 = the old engine's rules exactly, 1 = with the extensions */
PYENGINE_API pyengine_run* pyengine_run_package(const char* root, const char* package, const char* policy, int extended, const char* out_dir);
PYENGINE_API int pyengine_ok(const pyengine_run* r);
PYENGINE_API const char* pyengine_error(const pyengine_run* r);
PYENGINE_API const char* pyengine_ledger_json(pyengine_run* r, size_t* len);
PYENGINE_API const char* pyengine_unit_json(pyengine_run* r, const char* module, size_t* len);
PYENGINE_API const char* pyengine_coverage_md(pyengine_run* r, size_t* len);
PYENGINE_API void pyengine_counts(const pyengine_run* r, size_t* functions, size_t* translated, size_t* lines, size_t* translated_lines);
PYENGINE_API void pyengine_free(pyengine_run* r);

#ifdef __cplusplus
}
#endif
#endif
