// codebreaker/emulate/native_trace.hpp -- C++ port of codebreaker.emulate.native_trace.
//
// native_trace.py -- trace_function's hooks in C++ (native/tracer: cbtrace.dll), through ctypes.
//
// A run of one 32-bit function used to call back into Python on every instruction, every block
// and every memory write: ping.exe's judge made 2,459 runs and 8.5 million callbacks, most of its
// time. Here the hooks are C++: emulator.trace_function still lays the machine out, writes the
// frame, starts each run and puts memory back; this module attaches the DLL's hooks to the same
// Unicorn engine (uc._uch), in the order Python attached its own, and turns what they recorded
// back into the same TraceRecord fields. During a run Python is asked one thing: an import's
// argument count and what it gives back through a pointer, once per import entered.
//
//     run = native_trace.Run(uc, ...)   # the hooks attached
//     run.start(uc, begin, until)       # uc.emu_start, then what a Python hook would have raised
//     run.settle(rec)                   # (a fast run stopped inside a straight run)
//     run.collect(rec)                  # the record's fields
//     run.detach(); run.restore()       # (a kept machine: hooks off, pages back)
//
// The Python hooks this replaced are gone (docs/NATIVE.md, step 3): without the DLL a run raises
// (require()). Build: native/tracer/CMakeLists.txt (cmake + ninja in a vcvars64 shell; cbtrace.dll).
//
// Typing notes (where the C++ has to say what the Python leaves to ctypes and dicts):
//   ctypes structures   _Config and _Result are the C structures of native/tracer/cbtrace.h, field for
//                       field (what ctypes.Structure describes); Lib holds the DLL's entry points (what
//                       lib.cbt_* are on the ctypes CDLL).
//   the Unicorn engine  `uc` is an Engine: its handle (uc._uch) and its emu_start. The Python gets both
//                       from the unicorn package; C++ has no such package, so the host supplies them,
//                       and likewise the paths of the Unicorn and Capstone DLLs and the register table
//                       (unicorn.x86_const) that _load hands to the recorder: set_engine().
//   the record          the dict trace_function builds is a TraceRecord; collect()/settle() set exactly
//                       the keys the Python sets, each as an std::optional that is empty when the key
//                       is absent. "mem_writes" is a std::variant: records, or (lean) 4-word tuples.
//   the resolver        the DLL's callback has no context argument (a ctypes closure carries its Run);
//                       the run that is inside start() is the one the callback answers for.
//   unicorn.UcError     UcError (an error number, as unicorn.UcError(errno)).
//   settings.load_native  loads the DLL in place (the Python loads a copy keyed on its bytes under
//                       state/dll so a rebuild is never blocked: a host concern, not this module's).
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::emulate::native_trace {

inline constexpr int64_t _FAST = 0x1, _LEAN = 0x2, _READS = 0x4, _QUIET = 0x8, _STOP_WILD = 0x10, _STUB = 0x20;
inline constexpr int64_t _UNIQUE = 0x40, _BRANCHES = 0x80, _PADS = 0x100, _SAVED = 0x200, _RESOLVER = 0x400;
inline constexpr int64_t _CALL_WORDS = 21, _MEM_WORDS = 8, _LEAN_WORDS = 4, _LAZY_WORDS = 5, _BRANCH_WORDS = 11;
extern const py::list<py::str> _COMPARES;   // emulator._COMPARES
extern const py::list<py::str> _ACCESS;
inline constexpr int64_t _PAGE = 0x1000;

// The resolver the DLL calls: an import was entered (index, out, cap) -> 0, or nonzero when the
// answer raised.
using RESOLVER = int (*)(int32_t, int64_t*, int32_t);

// ---- native/tracer/cbtrace.h: cbt_config and cbt_result, field for field ------------------------
struct _Config {
    void* uch = nullptr;
    uint32_t flags = 0;
    int32_t sha_id = 0;
    int64_t insn_cap = 0, stub_ret = 0, stub_cleanup = 0;
    int64_t gov_n = 0, gov_ceil = 0;
    uint64_t func_lo = 0, func_hi = 0;
    uint64_t sec_lo = 0, sec_hi = 0;
    uint64_t pads_lo = 0, pads_hi = 0;
    uint64_t quiet_lo = 0, quiet_hi = 0;
    uint64_t lean_low_end = 0, lean_high_begin = 0;
    const int32_t* statics = nullptr;
    int32_t nstatics = 0;
    const uint64_t* init_pages = nullptr;
    int64_t ninit = 0;
    const uint64_t* mapped = nullptr;
    int64_t nmapped = 0;
    const uint64_t* presaved = nullptr;
    int64_t npresaved = 0;
    const uint64_t* sentinels = nullptr;
    int64_t nsentinels = 0;
    RESOLVER resolver = nullptr;
    void* shadow = nullptr;
};

struct _Result {
    int64_t insn_count, truncated, governed, dirty, ran_into_zeros, switched, memops, writes_seen, stuck,
        bogus_stubs, stubbed, lean_seen, api_calls_set, gov_n, hook_error, uc_err;
    const void* path;
    int64_t npath;
    const void* mn_addr;
    int64_t nmn;
    const void* mn_text;
    int64_t mn_text_len;
    const void* calls;
    int64_t ncalls;
    const void* reads;
    int64_t nreads;
    const void* writes;
    int64_t nwrites;
    const void* pages0;
    const void* pages0_data;
    int64_t npages0;
    const void* lazy;
    int64_t nlazy;
    const void* deps;
    int64_t ndeps;
    const void* api_calls;
    int64_t napi_words;
    const void* vxd;
    int64_t nvxd;
    const void* branches;
    int64_t nbranches;
};

// The DLL's entry points (the ctypes CDLL's cbt_* functions).
struct Lib {
    int (*cbt_version)() = nullptr;
    int (*cbt_init)(const wchar_t*, const wchar_t*, char*, int32_t) = nullptr;
    void (*cbt_register)(const char*, int32_t) = nullptr;
    int32_t (*cbt_static)(uint64_t, uint64_t, const uint8_t*) = nullptr;
    int32_t (*cbt_intern)(const char*) = nullptr;
    void* (*cbt_begin)(const _Config*) = nullptr;
    int (*cbt_attach)(void*) = nullptr;
    int (*cbt_attach_code)(void*) = nullptr;
    int (*cbt_switched)(void*) = nullptr;
    int (*cbt_detach)(void*) = nullptr;
    int (*cbt_restore)(void*) = nullptr;
    void (*cbt_settle)(void*) = nullptr;
    void (*cbt_free)(void*) = nullptr;
    void (*cbt_state)(void*, int64_t*) = nullptr;
    const _Result* (*cbt_result_get)(void*) = nullptr;
};

// unicorn.UcError(errno)
struct UcError : py::Exception {
    int64_t errno_ = 0;
    explicit UcError(int64_t e) : py::Exception("Unicorn error " + std::to_string(e)), errno_(e) {}
    const char* type() const override { return "UcError"; }
};

// The Unicorn engine a run's hooks are attached to: Python's `uc` (its handle uc._uch, uc.emu_start).
struct Engine {
    void* uch = nullptr;
    std::function<void(int64_t begin, int64_t until, int64_t count)> emu_start;
};

// emulator._GOV, carried through the run
struct Gov {
    int64_t n = 0;
    int64_t ceil = 0;
};

using Names = py::list<std::tuple<py::str, py::str>>;                  // (dll, name) of an import
using SigResolver = std::function<std::optional<int64_t>(const py::str&, const py::str&)>;
using Outs = std::function<py::list<std::tuple<int64_t, std::optional<int64_t>>>(const py::str&)>;

// ---- the record ----------------------------------------------------------------------------------
struct CallRecord {                      // a "calls" entry
    int64_t pc = 0;
    std::optional<int64_t> target;
    bool indirect = false;
    bool into_func = false;
    std::optional<int64_t> resolved_target;
    std::optional<bool> external;
    std::optional<py::str> api;
    std::optional<bool> stubbed;
    bool has_ret_sentinel = false;       // the key "ret_sentinel" exists (its value may be None)
    std::optional<int64_t> ret_sentinel;
    std::optional<py::list<int64_t>> args_stack;
    std::optional<int64_t> ecx, edx;
    std::optional<int64_t> argc;
};
struct MemRead {
    uint64_t pc = 0, addr = 0, size = 0;
    std::optional<uint64_t> value;
    uint64_t n = 0;
};
struct MemWrite {
    uint64_t pc = 0, addr = 0, size = 0, value = 0, n = 0, old = 0;
};
using LeanWrite = std::tuple<uint64_t, uint64_t, uint64_t, uint64_t>;   // addr, size, value, pc
// (the compare of a conditional jump) mnemonic, size, a, b, a's place, b's place
using Compare = std::tuple<py::str, uint64_t, uint64_t, uint64_t, std::optional<uint64_t>, std::optional<uint64_t>>;
using BranchEntry = std::tuple<uint64_t, bool, std::optional<Compare>>;
struct LazyMap {
    uint64_t pc = 0, addr = 0, page = 0;
    py::str access;
    uint64_t size = 0;
};

// The dict trace_function and collect()/settle() fill; a key that is not set is an empty optional.
struct TraceRecord {
    std::optional<int64_t> insn_count;
    std::optional<bool> truncated;
    std::optional<bool> governed;
    std::optional<py::list<uint64_t>> path;
    std::optional<py::dict<uint64_t, py::str>> mnemonics;
    std::optional<py::list<CallRecord>> calls;
    std::optional<py::list<MemRead>> mem_reads;
    // mixed in the Python by `lean`: lean records are 4-word tuples, the others MemWrite records
    std::optional<std::variant<py::list<MemWrite>, py::list<LeanWrite>>> mem_writes;
    std::optional<py::dict<uint64_t, py::bytes>> pages0;
    std::optional<py::list<LazyMap>> lazy_maps;
    std::optional<int64_t> memops;
    std::optional<int64_t> writes_seen;
    std::optional<bool> _dirty;
    std::optional<int64_t> _stuck;
    std::optional<int64_t> bogus_stubs;
    std::optional<bool> ran_into_zeros;
    std::optional<py::list<py::str>> api_deps;
    std::optional<py::list<std::tuple<py::str, std::optional<py::list<int64_t>>>>> api_calls;
    std::optional<int64_t> stubbed_externals;
    std::optional<py::list<uint64_t>> vxd_services;
    std::optional<py::list<BranchEntry>> branches;
};

// ---- the module's state --------------------------------------------------------------------------
extern py::list<std::shared_ptr<Lib>> _LIB;                                           // [the DLL, or None when it cannot be used here]
extern py::list<py::str> _WHY;                                                        // why not, once
extern std::map<const void*, std::tuple<int32_t, std::shared_ptr<void>>> _STATICS;   // id(an emulator._INSN table) -> (its native id, the table: kept alive)
extern py::dict<py::str, int32_t> _SHAS;

// Where the host's Unicorn and Capstone DLLs are, and unicorn.x86_const's UC_X86_REG_<NAME> table
// (name -> id), which _load hands to cbt_init / cbt_register.
void set_engine(const py::str& uc_dll, const py::str& cs_dll, const py::dict<py::str, int64_t>& registers);

// Where cbtrace.dll is: the `tracer_bin` setting (default native/tracer/build/bin).
py::str dll_path();
std::shared_ptr<Lib> _load();
// The 32-bit recorder for this process: the DLL loads here (the Python hooks it replaced are gone;
// require() raises with why_not() when it does not).
bool enabled();
std::optional<py::str> why_not();
int32_t _static_id(const Lib& lib, uint64_t lo, uint64_t hi, const py::bytes& image, const std::shared_ptr<void>& table);
int32_t _sha_id(const Lib& lib, const py::str& sha);
// (the array ctypes makes, at least one long, and the count)
std::tuple<std::shared_ptr<std::vector<uint64_t>>, int64_t> _u64s(const std::vector<uint64_t>& values);

// ctypes.string_at(ptr, ...) read as `n` records of `count` words each (struct.iter_unpack("<%dq" / "<%dQ")):
// W is int64_t for the signed form, uint64_t for the unsigned one.
template <class W> py::list<std::vector<W>> _words(const void* ptr, int64_t n, int64_t count) {
    py::list<std::vector<W>> out;
    if (!n) return out;
    const W* p = static_cast<const W*>(ptr);
    for (int64_t i = 0; i < n; i++) out.append(std::vector<W>(p + i * count, p + (i + 1) * count));
    return out;
}

// What a run keeps with its machine (`cache`): the import landing places, made again only when
// what they were made from changes.
struct SentinelCache {
    bool has = false;
    std::vector<const void*> from;
    int64_t count = 0;
    Names names;
    std::shared_ptr<std::vector<uint64_t>> sent;
    int64_t nsent = 0;
};

// The arguments of Run: trace_function's own, already worked out. `statics` its
// [(lo, hi, image, table)], `sentinels` {address: (dll, name)} (empty when not stubbing), `saved` the
// keys of the reuse dict (empty optional: not reusing), `gov` emulator._GOV.
struct RunArgs {
    std::vector<std::tuple<uint64_t, uint64_t, py::bytes, std::shared_ptr<void>>> statics;
    py::list<uint64_t> init_pages;
    py::list<uint64_t> mapped;
    py::dict<uint64_t, std::tuple<py::str, py::str>> sentinels;
    std::optional<std::vector<uint64_t>> saved;
    py::str sha;
    Gov* gov = nullptr;
    bool fast = false, lean = false, reads = false;
    std::optional<std::tuple<uint64_t, uint64_t>> quiet;
    bool stop_wild = false, stub = false, unique = false, branches = false;
    std::optional<std::tuple<int64_t, int64_t>> pads;
    int64_t insn_cap = 0, stub_ret = 0, stub_cleanup = 0;
    std::tuple<uint64_t, uint64_t> func, sec, lean_ranges;
    SigResolver sig_resolver;                 // (empty: none given)
    Outs outs;
    SentinelCache* cache = nullptr;           // (None)
    std::vector<const void*> sentinel_from;   // by identity
    void* shadow = nullptr;
};

// One run's native hooks on `uc` (see the module docstring).
class Run {
  public:
    std::shared_ptr<Lib> lib;
    Engine uc;
    Gov* gov = nullptr;
    std::exception_ptr exc;
    void* handle = nullptr;
    Outs outs;
    SigResolver sig_resolver;
    Names names;
    _Config config;
    int64_t lean_seen = 0;

    explicit Run(const Engine& uc, const RunArgs& a);
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;
    ~Run();

    // An import entered: its argument count (sig_resolver) and what it gives back (_outs).
    int _resolve(int32_t index, int64_t* out, int32_t cap);
    // [hook_error, uc_err, truncated, gov_n, switched]
    const int64_t* state();
    // uc.emu_start, then raise what a Python hook would have raised in it.
    void start(const Engine& uc, int64_t begin, int64_t until, int64_t count = 0);
    bool switched();
    void attach_code();
    void settle(TraceRecord& rec);
    // The record's fields, as trace_function's Python hooks leave them.
    void collect(TraceRecord& rec);
    py::str _api(int64_t code);
    void detach();
    void restore();

  private:
    std::shared_ptr<std::vector<int32_t>> keep_statics_;
    std::shared_ptr<std::vector<uint64_t>> keep_init_, keep_mapped_, keep_presaved_, keep_sent_;
    int64_t state_[5] = {0, 0, 0, 0, 0};
};

// The recorder is C++ only (the Python hooks it replaced are gone): raise when the DLL cannot
// be used here, saying why.
void require();

}  // namespace codebreaker::emulate::native_trace
