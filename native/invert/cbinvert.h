/* cbinvert.h -- the C interface of cbinvert.dll: codebreaker/invert/ in C++.
 *
 * Text in, JSON out (UTF-8, freed with cbinv_free).  A call returns 0, or the kind of exception
 * the Python raised at the same place (CBINV_INDEX: IndexError, CBINV_VALUE: ValueError), with
 * its message in *out.
 *
 * cbinv_cfg       flow16.build_cfg.  `in`: one instruction a line, "address\tmnemonic\toperands\n",
 *                 then the SWITCH hook's answers, "S\taddress\tt1,t2,...\n" (the hook asked for every
 *                 `jmp` with a memory operand; no line = None).  Out: {"b": [[lo, hi, term, succ,
 *                 cond_idx, table, cases, default, selreg, selbase], ...]}.
 * cbinv_structure flow16.structure(blocks, entry).  `in`: one block a line, "term\tlo\thi\tsucc\t
 *                 cases\tdefault\ttable\n" (lists comma-separated, "-" = None, a case "value:block").
 *                 Out: the tree, nodes as JSON lists in the Python tuples' order.
 */
#ifndef CBINVERT_H
#define CBINVERT_H
#ifdef __cplusplus
extern "C" {
#endif
#define CBINV_OK 0
#define CBINV_INDEX 1
#define CBINV_VALUE 2
#define CBINV_OTHER 3
__declspec(dllexport) int cbinv_version(void);
__declspec(dllexport) int cbinv_cfg(const char *in, int cmpswitch, char **out);
__declspec(dllexport) int cbinv_structure(const char *in, int entry, int nocap, char **out);
__declspec(dllexport) void cbinv_free(char *p);
#ifdef __cplusplus
}
#endif
#endif
