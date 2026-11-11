// codebreaker/emulate/native_win16.hpp -- C++ port of codebreaker.emulate.native_win16.
//
// native_win16.py -- trace_function16's per-instruction and memory hooks in C++ (native/cb/emulate/tracer:
// cbtrace.dll), through ctypes.
//
// windows16.trace_function16 still lays the NE module out, enters the function, starts the run
// (uc.emu_start to SENT_BASE) and reads the result; this module attaches the DLL's 16-bit hooks to
// the same Unicorn engine (uc._uch), in trace_function16's order, and turns what the plain code path
// and the memory hooks recorded back into the same rec fields. The heavy, rare events stay Python,
// called from the native hooks: the import stand-in (import_stub / the NE oracle), a call into
// another of the module's functions (answer), and interrupts (int 21h as DOS3Call). The branch log
// and the ports' device model stay Python hooks, which trace_function16 attaches itself; the ports'
// clock reads the live instruction count this recorder keeps (count()).
//
// The Python hooks this replaced are gone (docs/NATIVE.md, step 3): without the DLL a run raises
// (require()). Build: tools\build_cb.cmd (native/cb/CMakeLists.txt, target cbtrace).
//
// Typing notes (where the C++ has to say what the Python leaves to ctypes and dicts):
//   ctypes structures  _Config and _Result are the C structures of native/cb/emulate/tracer/cbtrace.h (cbt_w16_config,
//                      cbt_w16_result), field for field; Lib holds the DLL's cbt_w16_* entry points (what
//                      lib.cbt_w16_* are on the ctypes CDLL). native_trace::_load() loads the DLL; `_lib()`
//                      binds the 16-bit entry points from the same DLL, once, as the Python's _READY guards.
//   the Unicorn engine `uc` is native_trace::Engine (its handle uc._uch); trace_function16 starts the run itself.
//   the record         the dict trace_function16's rec is, restricted to the keys collect() sets: Win16Record,
//                      each key an std::optional that is empty when the key is left as it was.
//   the callbacks      the DLL's event callbacks carry no context (a ctypes closure carries its Run); a fixed
//                      pool of C++ entry points, four per live Run, stands in for the closures.
//   BaseException      the exception a handler raises is kept as a std::exception_ptr (any exception).
//   `near`             is a keyword of the Run; spelled near_ here (a macro in <windows.h>).
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <vector>

#include "py.hpp"
#include "codebreaker/emulate/native_trace.hpp"

namespace codebreaker::emulate::native_win16 {

inline constexpr int64_t _MEM_WORDS = 8, _LAZY_WORDS = 5;
extern const py::list<py::str> _ACCESS;

// An event the code hook met: (a, n) -> 0, or nonzero when the handler raised.
using EVENT = int (*)(int64_t, int64_t);

// ---- native/cb/emulate/tracer/cbtrace.h: cbt_w16_config and cbt_w16_result, field for field -----------------
struct _Config {
    void* uch = nullptr;
    int32_t eip_id = 0;
    int32_t near_fn = 0, fill = 0;
    int64_t cap = 0;
    uint64_t cs_base = 0, near_ret = 0, far_ret = 0, sent_lo = 0, sent_hi = 0, snap_base = 0;
    int64_t gov_n = 0, gov_ceil = 0;
    int64_t* count = nullptr;
    const uint64_t* intercept = nullptr;
    int64_t nintercept = 0;
    EVENT on_sentinel = nullptr, on_intercept = nullptr, on_intr = nullptr;
    void* shadow = nullptr;
    uint64_t own_lo = 0, own_hi = 0;
    EVENT on_own = nullptr;
};

struct _Result {
    int64_t insn_count, truncated, returned, limit_fault, memops, writes_seen, stuck, gov_n, hook_error, uc_err;
    const void* path;
    int64_t npath;
    const void* reads;
    int64_t nreads;
    const void* writes;
    int64_t nwrites;
    const void* lazy;
    int64_t nlazy;
};

// The DLL's 16-bit entry points (the ctypes CDLL's cbt_w16_* functions).
struct Lib {
    void* (*cbt_w16_begin)(const _Config*) = nullptr;
    int (*cbt_w16_attach)(void*) = nullptr;
    int (*cbt_w16_detach)(void*) = nullptr;
    void (*cbt_w16_free)(void*) = nullptr;
    void (*cbt_w16_reset)(void*) = nullptr;
    void (*cbt_w16_state)(void*, int64_t*) = nullptr;
    const _Result* (*cbt_w16_result_get)(void*) = nullptr;
};

// [bound]: the 16-bit entry points, once they are prepared on the DLL
extern py::list<std::shared_ptr<Lib>> _READY;

// The cbtrace.dll native_trace loaded, with the 16-bit entry points bound, or None (empty).
std::shared_ptr<Lib> _lib();
// The 16-bit recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
// require() raises with why_not() when it does not).
bool enabled();
std::optional<py::str> why_not();

// (the array ctypes makes, at least one long, and the count)
std::tuple<std::shared_ptr<std::vector<uint64_t>>, int64_t> _u64s(const std::vector<uint64_t>& values);
// ctypes.string_at(ptr, ...) read as `n` records of `count` words each (struct.iter_unpack("<%dQ")).
py::list<std::vector<uint64_t>> _words(const void* ptr, int64_t n, int64_t count);

// The dict trace_function16 fills; collect() sets the keys below, a key that is not set is an empty optional.
struct Win16Record {
    std::optional<int64_t> insn_count;
    std::optional<bool> truncated;
    std::optional<bool> returned;
    std::optional<bool> limit_fault;
    std::optional<py::list<uint64_t>> path;
    std::optional<py::list<native_trace::MemRead>> mem_reads;
    std::optional<py::list<native_trace::MemWrite>> mem_writes;
    std::optional<py::list<native_trace::LazyMap>> lazy_maps;
    std::optional<int64_t> memops;
    std::optional<int64_t> writes_seen;
    std::optional<int64_t> _stuck;
};

// The arguments of Run (its keyword arguments). `on_sentinel(addr, n)`, `on_intercept(addr, n)` and
// `on_intr(intno, n)` run the Python event handlers (import_stub / answer / the interrupt body);
// each may raise, and the exception comes back out of trace_function16. `gov` emulator._GOV.
// `own` (lo, hi) with `on_own(addr, n)` -> whether it redirected: a recorded-system context.
struct RunArgs {
    int64_t eip_id = 0;
    bool near_ = false, fill = false;
    int64_t cap = 0;
    int64_t cs_base = 0, near_ret = 0, far_ret = 0, sent_lo = 0, sent_hi = 0, snap_base = 0;
    native_trace::Gov* gov = nullptr;
    py::list<uint64_t> intercept;
    std::function<void(int64_t, int64_t)> on_sentinel, on_intercept, on_intr;
    void* shadow = nullptr;
    std::optional<std::tuple<int64_t, int64_t>> own;
    std::function<bool(int64_t, int64_t)> on_own;
};

// One NE function's native hooks on `uc`.
class Run {
  public:
    std::shared_ptr<Lib> lib;
    native_trace::Engine uc;
    native_trace::Gov* gov = nullptr;
    std::exception_ptr exc;
    _Config config;
    void* handle = nullptr;

    Run(const native_trace::Engine& uc, const RunArgs& a);
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
    ~Run();

    // The event handler the callback of `kind` (0 sentinel, 1 intercept, 2 interrupt) runs: 0, or 1 when it raised.
    int _event(int kind, int64_t a, int64_t n);
    // The recorded-system handler: 1 redirected, 0 not, 2 when it raised.
    int _own(int64_t a, int64_t n);
    // A prelude ran: its path, count, reads and writes are dropped (cbt_w16_reset).
    void reset();
    int64_t count() const;
    std::exception_ptr raised();
    void collect(Win16Record& rec);
    void detach();

  private:
    std::function<void(int64_t, int64_t)> fns_[3];
    std::function<bool(int64_t, int64_t)> on_own_;
    int slot_ = -1;
    int64_t count_ = 0;
    std::shared_ptr<std::vector<uint64_t>> keep_inter_;
    int64_t state_[3] = {0, 0, 0};
};

// The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
// be used here, saying why.
void require();

}  // namespace codebreaker::emulate::native_win16
