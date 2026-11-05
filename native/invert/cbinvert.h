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
 * cbinv_init      once: the path of the capstone.dll Python's `capstone` package carries (5.0.7);
 *                 the models decode with it, so every operand text is the one Python saw.
 * cbinv_msc6      msc6.invert.  `in`, one field a tab: "C <code hex>", "R <rtype>", "B <func_base>",
 *                 "P <count, -1 for None>", then "p name kind" per parameter, "I k name" (imports),
 *                 "Y off name kind" (syms), "N off name" (nearmap).  Out: {"t": text, "m": meta} or
 *                 {"t": null, "w": why}.
 * cbinv_msc6_sigs msc6_calls.infer_segment_sigs.  `in`: the segment's bytes as hex.  Out: [[off, [[n, k]..]]..].
 * cbinv_thunk     segment_thunk.verify (exact=1) or parse (0) + emit_asm(name).  Out: null or the thunk.
 * cbinv_wf        whole_function.candidates.  Out: [[template, text], ...].
 * cbinv_mask      msc6_function._mask.  Out: hex.
 * cbinv_small     which 0: msc6_function.invert_cleanup ("C <hex>", "Y off name"), 1: msc6_walk.invert
 *                 ("C", "Y off name kind", "K <callee>"), 2: msc6_templates.invert ("C"); tab-separated.
 * cbinv_msc6reg   which 0 msc6_registers.invert_reg, 1 invert_loop ("C <hex>", "I k name" per import);
 *                 out {"t": text, "m": meta} or {"t": null, "w": why}.
 * cbinv_msvc2     the MSVC 2.0 models (msvc2.h): which 0 msvc2.invert ("C <hex>", "M <offsets>" for a
 *                 mask), 1 msvc2_registers.interp ("C <hex>", "N <nargs>"), 2 msvc2_forward.emit,
 *                 3 to_c, 4 frame_layout (the node or the locals as pyval JSON, pyval.h).
 * cbinv_early_c   early_c (early_c.h): which 0 recover, 1 recover_cfg (one field a tab: "C <hex>",
 *                 "B <base>", "X <target> <operator>" per cxmap entry, "F <eleven 0/1 flags>",
 *                 "V <RETVAL -1/0/1>", "H <7 str hashes>", "K <hooks set>", "Ls/Lr/Lt/Lf/Lu" LAST's
 *                 keys); `hook` answers SWITCH (0), CALLINFO (1), CALLREGS (2): -1 it raised, 0 None,
 *                 1 its text in buf.  Out {"fs", "params", "stmts", "pt", "loc", "last"}.  which 2
 *                 to_c, 3 _fold_ladders, 4 detect_copies: a pyval JSON tuple of the arguments in.
 */
#ifndef CBINVERT_H
#define CBINVERT_H
#include <wchar.h>
#ifdef __cplusplus
extern "C" {
#endif
#define CBINV_OK 0
#define CBINV_INDEX 1
#define CBINV_VALUE 2
#define CBINV_OTHER 3
#define CBINV_KEY 4
#define CBINV_ASSERT 5
#define CBINV_TYPE 6
#define CBINV_ATTR 7
__declspec(dllexport) int cbinv_version(void);
__declspec(dllexport) int cbinv_cfg(const char *in, int cmpswitch, char **out);
__declspec(dllexport) int cbinv_structure(const char *in, int entry, int nocap, char **out);
__declspec(dllexport) int cbinv_init(const wchar_t *capstone_dll, char **out);
__declspec(dllexport) int cbinv_msc6(const char *in, char **out);
__declspec(dllexport) int cbinv_msc6_sigs(const char *in, char **out);
__declspec(dllexport) int cbinv_thunk(const char *in, const char *name, int exact, char **out);
__declspec(dllexport) int cbinv_wf(const char *in, char **out);
__declspec(dllexport) int cbinv_mask(const char *in, char **out);
__declspec(dllexport) int cbinv_small(const char *in, int which, char **out);
__declspec(dllexport) int cbinv_msc6reg(const char *in, int which, char **out);
__declspec(dllexport) int cbinv_msvc2(const char *in, int which, char **out);
typedef int (*cbinv_hook_t)(int which, long long addr, const char *kind, const char *target, char *buf, int buflen);
__declspec(dllexport) int cbinv_early_c(const char *in, int which, cbinv_hook_t hook, char **out);
__declspec(dllexport) void cbinv_free(char *p);
#ifdef __cplusplus
}
#endif
#endif
