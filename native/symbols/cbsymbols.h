/* cbsymbols.h -- the C interface of cbsymbols.dll (native/symbols), loaded by
   codebreaker/symbols/native.py through ctypes.

   int cbs_version(void)                      the interface's version (CBS_VERSION)
   int cbs_ask(const char *question,          a question ("demangle.parse", "demangle.type_name",
                                              "prototypes.build")
               const unsigned *text, size_t n, its text as code points (UTF-32)
               char **answer, size_t *len)    -> 1 and the answer (JSON, ASCII), 0 for no answer (None),
                                              -1 for a question it does not know
   void cbs_free(char *answer)                frees an answer */
#ifndef CBSYMBOLS_H
#define CBSYMBOLS_H
#include <stddef.h>
#define CBS_VERSION 1
#ifdef __cplusplus
extern "C" {
#endif
__declspec(dllexport) int cbs_version(void);
__declspec(dllexport) int cbs_ask(const char *question, const unsigned *text, size_t n, char **answer, size_t *len);
__declspec(dllexport) void cbs_free(char *answer);
#ifdef __cplusplus
}
#endif
#endif
