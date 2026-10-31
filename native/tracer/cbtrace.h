/* cbtrace.h -- the C interface of CodeBreaker's run recorder (cbtrace.dll).
 *
 * The recorder is the per-instruction half of codebreaker/emulate/emulator.py trace_function:
 * the hooks Unicorn calls while one 32-bit function runs.  Python still lays the machine out,
 * writes the frame, starts each run (uc_emu_start) and puts memory back afterwards; this DLL
 * attaches its hooks to Python's own Unicorn handle, in the order Python attached its own, and
 * keeps the record those hooks made: the instruction path and texts, the calls, the memory
 * reads and writes, the pages mapped on demand, the imports entered (each answered by a
 * stand-in that takes its arguments off the stack), and the branch log.
 *
 * During a run Python is called only the first time each import is entered in it, to ask its
 * argument count and which of its parameters give something back (the resolver below; the
 * answer is a lookup, kept for the rest of the run).
 *
 * Plain C, loaded with ctypes (codebreaker/emulate/native_trace.py): nothing here depends on a
 * Python version.  Unicorn and Capstone are the very DLLs Python's `unicorn` and `capstone`
 * packages loaded, found by path at cbt_init, so a run and its decoded text are the same as the
 * Python tracer's.
 */
#ifndef CBTRACE_H
#define CBTRACE_H

#include <stdint.h>

#ifdef _WIN32
#define CBT_API __declspec(dllexport)
#else
#define CBT_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define CBT_VERSION 1

/* run options (cbt_config.flags) -- the trace_function arguments of the same names */
#define CBT_FAST      0x001   /* told of whole straight runs, not of each instruction */
#define CBT_LEAN      0x002   /* writes as (address, size, value, pc); pages as first written */
#define CBT_READS     0x004   /* record memory reads too */
#define CBT_QUIET     0x008   /* writes into [quiet_lo, quiet_hi) count but are not recorded */
#define CBT_STOP_WILD 0x010   /* stop at the second 00 00 instruction */
#define CBT_STUB      0x020   /* answer imports (sentinels) with a stand-in */
#define CBT_UNIQUE    0x040   /* each stand-in returns its own marker (_RETSENT_BASE + n) */
#define CBT_BRANCHES  0x080   /* log the function's conditional jumps */
#define CBT_PADS      0x100   /* calls into [pads_lo, pads_hi) are callbacks */
#define CBT_SAVED     0x200   /* note what each page held before the run changed it (reuse) */
#define CBT_RESOLVER  0x400   /* a resolver is given (sig_resolver) */

/* The resolver: an import was entered.  `index` is its place in cbt_config.sentinels.
 * out[0] = its argument count, or -1 when not known; out[1] = n, the parameters through which
 * it gives something back, then n pairs (parameter's place, value given or -1 for a handle).
 * At most `cap` pairs.  Returns 0, or nonzero when Python raised (the run then stops and
 * Python raises the same exception where its own hook would have). */
typedef int (*cbt_resolver)(int32_t index, int64_t *out, int32_t cap);

typedef struct cbt_config {
    void *uch;                         /* Python's uc._uch */
    uint32_t flags;
    int32_t sha_id;                    /* cbt_intern of the traced module's hash: the branch decode cache key */
    int64_t insn_cap, stub_ret, stub_cleanup;
    int64_t gov_n, gov_ceil;           /* emulator._GOV, carried through the run */
    uint64_t func_lo, func_hi;         /* the function: calls into it, and the branch log's range */
    uint64_t sec_lo, sec_hi;           /* the module image: a call landing outside it is external */
    uint64_t pads_lo, pads_hi;
    uint64_t quiet_lo, quiet_hi;
    uint64_t lean_low_end;             /* lean writes are followed in [0, lean_low_end] ... */
    uint64_t lean_high_begin;          /* ... and [lean_high_begin, 0xFFFFFFFF] */
    const int32_t *statics; int32_t nstatics;      /* cbt_static ids, in trace_function's order */
    const uint64_t *init_pages; int64_t ninit;     /* page numbers (address >> 12) set before the run */
    const uint64_t *mapped; int64_t nmapped;       /* page bases already mapped */
    const uint64_t *presaved; int64_t npresaved;   /* page bases Python already saved (reuse) */
    const uint64_t *sentinels; int64_t nsentinels; /* import landing addresses; index = place */
    cbt_resolver resolver;
} cbt_config;

/* Fixed-width records the result points at (all uint64 words). */
#define CBT_CALL_WORDS 21   /* pc, has_target, target, into_func, state (0 plain, 1 resolved, 2 stubbed),
                               resolved, external, api, has_ret_sentinel, ret_sentinel, has_args,
                               args[6], ecx, edx, has_argc, argc */
#define CBT_MEM_WORDS 8     /* pc, addr, size, value, value_none, n, has_old, old */
#define CBT_LEAN_WORDS 4    /* addr, size, value, pc */
#define CBT_LAZY_WORDS 5    /* pc, addr, page, access (0 read, 1 write, 2 fetch, 3 other), size */
#define CBT_BRANCH_WORDS 11 /* addr, taken, has_cmp, mnemonic (index in _COMPARES), size, a, b,
                               has_at_a, at_a, has_at_b, at_b */

typedef struct cbt_result {
    int64_t insn_count;
    int64_t truncated, governed, dirty, ran_into_zeros, switched;
    int64_t memops, writes_seen, stuck, bogus_stubs, stubbed;   /* -1: never set (no key) */
    int64_t lean_seen;
    int64_t api_calls_set;             /* a stand-in got as far as recording calls (the key exists) */
    int64_t gov_n;
    int64_t hook_error;                /* 0, 1: Python raised in the resolver, 2: a Unicorn error (uc_err) */
    int64_t uc_err;
    const uint64_t *path; int64_t npath;
    const uint64_t *mn_addr; int64_t nmn;
    const char *mn_text; int64_t mn_text_len;       /* nmn texts, each ended by a NUL */
    const uint64_t *calls; int64_t ncalls;
    const uint64_t *reads; int64_t nreads;
    const uint64_t *writes; int64_t nwrites;        /* CBT_MEM_WORDS each, CBT_LEAN_WORDS when lean */
    const uint64_t *pages0; const uint8_t *pages0_data; int64_t npages0;
    const uint64_t *lazy; int64_t nlazy;
    const int64_t *deps; int64_t ndeps;             /* imports reached: sentinel place, or -(k+1) for pad k */
    const int64_t *api_calls; int64_t napi_words;   /* code, argc (-1 unknown), argc arguments ... */
    const uint64_t *vxd; int64_t nvxd;
    const uint64_t *branches; int64_t nbranches;
} cbt_result;

CBT_API int cbt_version(void);
/* Load the Unicorn and Capstone DLLs Python loaded (full paths).  0, or nonzero with `err` set. */
CBT_API int cbt_init(const wchar_t *unicorn_dll, const wchar_t *capstone_dll, char *err, int32_t errlen);
/* Unicorn's id for a register name as Capstone spells it, upper-cased (x86_const UC_X86_REG_<NAME>). */
CBT_API void cbt_register(const char *name, int32_t uc_id);
/* A code range as it is in the file; its decoded instructions are kept for the life of the
 * process (emulator._INSN).  -> its id. */
CBT_API int32_t cbt_static(uint64_t lo, uint64_t hi, const uint8_t *image);
/* A small number for a string (a module's hash). */
CBT_API int32_t cbt_intern(const char *s);

CBT_API void *cbt_begin(const cbt_config *cfg);
CBT_API int cbt_attach(void *run);               /* the hooks, in trace_function's order */
CBT_API int cbt_attach_code(void *run);          /* a fast run gone one instruction at a time */
CBT_API int cbt_switched(void *run);
/* out[0] hook_error, out[1] uc_err, out[2] truncated, out[3] gov_n, out[4] switched */
CBT_API void cbt_state(void *run, int64_t *out);
CBT_API void cbt_settle(void *run);              /* a fast run stopped inside a straight run */
CBT_API const cbt_result *cbt_result_get(void *run);
CBT_API int cbt_detach(void *run);               /* uc_hook_del of every hook attached */
CBT_API int cbt_restore(void *run);              /* put back the pages this run's hooks saved */
CBT_API void cbt_free(void *run);

/* ============================================================================================
 * The flat driver recorder (codebreaker/emulate/driver.py `run`): a VxD's ring-0 entry points.
 * The same model as the 32-bit tracer -- Python lays memory out, sets the registers, starts the
 * run (uc_emu_start to _RET) and reads the result; this holds driver.run's hooks (the per-
 * instruction code hook that tells the image's own code from a call out through a pointer, the
 * shared memory hooks, the INT 20h service calls, and the I/O ports) and keeps what they
 * recorded.  During a run Python is asked one thing, once per distinct service: which register
 * the service answers in (driver_services + the knowledge base), a lookup kept for the run.
 * ============================================================================================ */

/* A service was called (INT 20h): `device` and `service` (the 0x8000 jump bit kept).  out[0] =
 * the place in driver.RESULT/REGISTERS (0 eax, 1 ebx, 2 ecx, 3 edx, 4 esi, 5 edi, 6 ebp) the
 * service answers in, or -1 (it answers in EAX, as most do).  Returns 0, or nonzero when Python
 * raised (the run stops and Python raises the same exception after it). */
typedef int (*cbt_drv_answers)(int32_t device, int32_t service, int32_t *out);

#define CBT_DRV_CALL_WORDS 15  /* none_first, first, second, nregs, regs[6], nwords, words[4] */
#define CBT_DRV_PORT_WORDS 4   /* kind (0 in, 1 out), port, size, value */

typedef struct cbt_drv_config {
    void *uch;                         /* Python's uc._uch */
    int32_t reg_ids[10];               /* Unicorn ids: eax,ebx,ecx,edx,esi,edi,ebp,esp,eip,eflags */
    int32_t ins_in, ins_out;           /* UC_X86_INS_IN, UC_X86_INS_OUT */
    uint64_t vm_addr;                  /* driver.VM: an answering register is set to it */
    uint64_t ret;                      /* emulator._RET */
    uint64_t scratch_lo, scratch_hi;   /* emulator._SCRATCH_BASE .. +_SCRATCH_SIZE */
    const uint64_t *spans; int64_t nspans;   /* image.spans as (lo, hi) pairs: the driver's own code */
    int64_t cap;                       /* the instruction cap */
    int64_t service_eax, service_carry;
    int64_t gov_n, gov_ceil;           /* emulator._GOV, carried through the run */
    cbt_drv_answers answers;
} cbt_drv_config;

typedef struct cbt_drv_result {
    int64_t exit_code;                 /* 0 none, 1 "wild", 2 service number unreadable, 3 int:intno */
    int64_t exit_int;
    int64_t insns, truncated, governed, outside;
    int64_t memops, writes_seen, stuck;    /* -1: never set (the key is left out) */
    int64_t gov_n, hook_error, uc_err;
    const uint64_t *cover; int64_t ncover;
    const uint64_t *calls; int64_t ncalls;      /* CBT_DRV_CALL_WORDS each */
    const uint64_t *ports; int64_t nports;      /* CBT_DRV_PORT_WORDS each */
    const uint64_t *reads; int64_t nreads;      /* CBT_MEM_WORDS each */
    const uint64_t *writes; int64_t nwrites;    /* CBT_MEM_WORDS each */
    const uint64_t *lazy; int64_t nlazy;        /* CBT_LAZY_WORDS each */
} cbt_drv_result;

CBT_API void *cbt_drv_begin(const cbt_drv_config *cfg);
CBT_API int cbt_drv_attach(void *run);                  /* the hooks, in driver.run's order */
CBT_API void cbt_drv_state(void *run, int64_t *out);    /* [0] hook_error, [1] uc_err, [2] gov_n */
CBT_API const cbt_drv_result *cbt_drv_result_get(void *run);
CBT_API int cbt_drv_detach(void *run);
CBT_API void cbt_drv_free(void *run);

#ifdef __cplusplus
}
#endif
#endif
