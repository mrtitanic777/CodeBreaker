/* cbanalyze.h -- the C interface of cbanalyze.dll (native/analyze), loaded by
   codebreaker/analyze/native.py through ctypes.

   int cba_version(void)                         the interface's version (CBA_VERSION)
   int cba_init(capstone_dll, err, errlen)       0: the Capstone Python loaded is found
   void *cba_open(blob, n, err, errlen)          an image (spans, functions and limits, imports,
                                                 pointers; the layout is native.py's _blob), or NULL
   int cba_ask(img, question, arg, answer, len)  "image.discover", "image.function" (arg: start),
                                                 "image.linear" (arg: address | count << 40),
                                                 "call_graph.build", "conventions.functions",
                                                 "strings.table", "strings.uses", "strings.stack",
                                                 "strings.formats", "globals.variables",
                                                 "globals.tables" (after discover and names)
                                                 -> 1 and the answer (JSON, ASCII), -1 unknown
   int cba_names(img, blob, n)                   the functions' and imports' names (native.py's _names)
   void cba_free(char *answer), void cba_close(void *img) */
#ifndef CBANALYZE_H
#define CBANALYZE_H
#include <stddef.h>
#include <wchar.h>
#define CBA_VERSION 4
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) int cba_version(void);
__declspec(dllexport) int cba_init(const wchar_t *capstone_dll, char *err, int errlen);
__declspec(dllexport) void *cba_open(const unsigned char *blob, size_t n, char *err, int errlen);
__declspec(dllexport) int cba_ask(void *img, const char *question, long long arg, char **answer, size_t *len);
__declspec(dllexport) int cba_names(void *img, const unsigned char *blob, size_t n);
__declspec(dllexport) void cba_free(char *answer);
__declspec(dllexport) void cba_close(void *img);
#ifdef __cplusplus
}
#endif
#endif
