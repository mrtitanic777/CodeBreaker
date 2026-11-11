// codebreaker/emulate/native_trace.hpp -- SHIM over cb/emulate/native_trace.hpp (docs/CPP_DESIGN.md 6.3): the old
// Run, TraceRecord and engine-table types, converting to and from the new TraceRun. Deleted with its last py::
// consumer (emulate/emulator, native_win16, native_driver, native_shadow, verify/lanes/win32).
//
// native_trace.py -- trace_function's hooks in C++ (native/cb/emulate/tracer), now typed. This header keeps what the
// not-yet-refactored ports use of it:
//   * Run / RunArgs / TraceRecord / the record types: the old shapes (py:: containers, optionals for "key present"),
//     built from a cb::emulate::native_trace::TraceRun and its TraceRecord;
//   * _load() / Lib / _Config / _Result / dll_path() / set_engine(): the loader of cbtrace.dll's C interface, which the
//     16-bit, driver and shadow ports still call through (COMPAT(py): remove when native_win16, native_driver and
//     native_shadow are refactored);
//   * UcError, Engine (a host Unicorn: its handle and emu_start), Gov.
// The Unicorn and Capstone DLLs are the host's (the ones found by path), as the DLL's cbt_init was told: the in-process
// recorder is pointed at them with tracer::use_libraries.
#pragma once

#include <cstdint>
#include <exception>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/native_trace.hpp"
#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

// the kernel32 calls _load needs, declared here: <windows.h> would bring its macros into every unit
#ifndef _WINDOWS_
extern "C" {
__declspec(dllimport) void* __stdcall LoadLibraryW(const wchar_t*);
__declspec(dllimport) void* __stdcall GetProcAddress(void*, const char*);
}
#endif

namespace codebreaker::emulate::unicorn_uc {
py::dict<py::str, int64_t> x86_register_table();   // unicorn_uc.hpp: unicorn.x86_const as {"UC_X86_REG_<NAME>": id}
}

namespace cb::emulate::tracer {
bool use_libraries(const wchar_t* unicorn_dll, const wchar_t* capstone_dll, char* err, std::size_t errlen);   // tracer/libs.hpp
}

namespace codebreaker::emulate::native_trace {

inline constexpr int64_t _FAST = 0x1, _LEAN = 0x2, _READS = 0x4, _QUIET = 0x8, _STOP_WILD = 0x10, _STUB = 0x20;
inline constexpr int64_t _UNIQUE = 0x40, _BRANCHES = 0x80, _PADS = 0x100, _SAVED = 0x200, _RESOLVER = 0x400;
inline constexpr int64_t _CALL_WORDS = 21, _MEM_WORDS = 8, _LEAN_WORDS = 4, _LAZY_WORDS = 5, _BRANCH_WORDS = 11;
inline const py::list<py::str> _COMPARES = py::list<py::str>{py::str("cmp"), py::str("test"), py::str("sub"), py::str("and"), py::str("or"),
                                                             py::str("dec"), py::str("inc"), py::str("add"), py::str("xor")};
inline const py::list<py::str> _ACCESS = py::list<py::str>{py::str("read"), py::str("write"), py::str("fetch"), py::str("?")};
inline constexpr int64_t _PAGE = 0x1000;

// The resolver the DLL calls: an import was entered (index, out, cap) -> 0, or nonzero when the answer raised.
using RESOLVER = int (*)(int32_t, int64_t*, int32_t);

// ---- native/cb/emulate/tracer/cbtrace.h: cbt_config and cbt_result, field for field -------------------------------
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

// The Unicorn engine a run's hooks are attached to: the host's `uc` (its handle uc._uch, uc.emu_start).
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

// ---- the record ----------------------------------------------------------------------------------------------------
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

// ---- the module's state ----------------------------------------------------------------------------------------------
// (the DLL loader the sibling ports use)
inline py::list<std::shared_ptr<Lib>> _LIB = py::list<std::shared_ptr<Lib>>{};      // [the DLL, or None when it cannot be used here]
inline py::list<py::str> _WHY = py::list<py::str>{};                               // why not, once
inline std::map<const void*, std::tuple<int32_t, std::shared_ptr<void>>> _STATICS;   // id(an emulator._INSN table) -> (its id, the table: kept alive)
inline py::dict<py::str, int32_t> _SHAS;

namespace shim {

struct EngineConfig {
    std::optional<py::str> uc_dll, cs_dll;
    py::dict<py::str, int64_t> registers;
};
inline EngineConfig& engine_config() {
    static EngineConfig c;
    return c;
}

inline std::wstring wide(const py::str& s) {
    std::wstring w;
    for (char32_t c : s.u32()) {
        if (c >= 0x10000) {
            uint32_t v = static_cast<uint32_t>(c) - 0x10000;
            w.push_back(static_cast<wchar_t>(0xD800 + (v >> 10)));
            w.push_back(static_cast<wchar_t>(0xDC00 + (v & 0x3FF)));
        } else {
            w.push_back(static_cast<wchar_t>(c));
        }
    }
    return w;
}

// (GetProcAddress takes an HMODULE where windows.h is included, a void* where this header declared it)
inline void* proc(void* dll, const char* name) {
    return reinterpret_cast<void*>(GetProcAddress(static_cast<decltype(LoadLibraryW(L""))>(dll), name));
}

template <class F> void entry(void* dll, const char* name, F& slot) {
    void* p = proc(dll, name);
    if (!p) throw py::AttributeError(std::string("function '") + name + "' not found");
    slot = reinterpret_cast<F>(p);
}

// The host's Unicorn and Capstone DLLs: the ones named with set_engine(), else those the Python packages carry.
inline void resolve_dlls(py::str& uc_dll, py::str& cs_dll) {
    EngineConfig& cfg = engine_config();
    if (!cfg.uc_dll) {
        cfg.uc_dll = base::capstone_dll::find_unicorn();
        if (cfg.registers.empty()) cfg.registers = unicorn_uc::x86_register_table();
    }
    uc_dll = *cfg.uc_dll;
    cs_dll = cfg.cs_dll ? *cfg.cs_dll : capstone_x86::dll();
}

// Points the in-process recorder at the host's DLLs, once (engines made by the host's Unicorn are only valid there).
inline void use_host_libraries() {
    static std::once_flag once;
    std::call_once(once, [] {
        py::str uc_dll, cs_dll;
        resolve_dlls(uc_dll, cs_dll);
        char err[256] = {0};
        const std::wstring wuc = wide(py::os::path::abspath(uc_dll)), wcs = wide(py::os::path::abspath(cs_dll));
        if (!cb::emulate::tracer::use_libraries(wuc.c_str(), wcs.c_str(), err, sizeof err)) {
            throw py::OSError(py::bytes(err).decode("ascii", "replace"));
        }
    });
}

}  // namespace shim

// Where the host's Unicorn and Capstone DLLs are, and unicorn.x86_const's UC_X86_REG_<NAME> table (name -> id), which
// _load hands to cbt_init / cbt_register.
inline void set_engine(const py::str& uc_dll, const py::str& cs_dll, const py::dict<py::str, int64_t>& registers) {
    shim::EngineConfig& c = shim::engine_config();
    c.uc_dll = uc_dll;
    c.cs_dll = cs_dll;
    c.registers = registers;
}

// Where cbtrace.dll is: the `tracer_bin` setting (default state/cxx/cb/bin).
inline py::str dll_path() {
    return py::os::path::join(py::settings::get_str(py::str("tracer_bin")), py::str("cbtrace.dll"));
}

// The C interface of cbtrace.dll, for the ports that still reach the recorder through it.
inline std::shared_ptr<Lib> _load() {
    static std::recursive_mutex init;   // (first use may be on several gate threads at once)
    std::lock_guard<std::recursive_mutex> hold(init);
    if (!_LIB.empty()) return _LIB[0];
    std::shared_ptr<Lib> lib;
    try {
        py::str path = dll_path();
        if (!py::os::path::isfile(path)) throw py::OSError(py::str("not built: ") + path + py::str(" (native/cb/CMakeLists.txt, target cbtrace)"));
        py::str uc_dll, cs_dll;
        shim::resolve_dlls(uc_dll, cs_dll);
        auto l = std::make_shared<Lib>();
        std::wstring wpath = shim::wide(py::os::path::abspath(path));
        void* dll = LoadLibraryW(wpath.c_str());
        if (!dll) throw py::OSError(py::str("could not load ") + path);
        shim::entry(dll, "cbt_version", l->cbt_version);
        if (l->cbt_version() != 1) throw py::OSError("cbtrace.dll is another version: rebuild it (target cbtrace)");
        shim::entry(dll, "cbt_init", l->cbt_init);
        char err[256] = {0};
        std::wstring wuc = shim::wide(py::os::path::abspath(uc_dll)), wcs = shim::wide(py::os::path::abspath(cs_dll));
        if (l->cbt_init(wuc.c_str(), wcs.c_str(), err, 256)) throw py::OSError(py::bytes(err).decode("ascii", "replace"));
        shim::entry(dll, "cbt_register", l->cbt_register);
        for (const auto& kv : shim::engine_config().registers.items()) {
            const std::string& name = std::get<0>(kv).utf8();
            const std::string prefix = "UC_X86_REG_";
            if (name.compare(0, prefix.size(), prefix) == 0) l->cbt_register(name.substr(prefix.size()).c_str(), static_cast<int32_t>(std::get<1>(kv)));
        }
        shim::entry(dll, "cbt_static", l->cbt_static);
        shim::entry(dll, "cbt_intern", l->cbt_intern);
        shim::entry(dll, "cbt_begin", l->cbt_begin);
        shim::entry(dll, "cbt_attach", l->cbt_attach);
        shim::entry(dll, "cbt_attach_code", l->cbt_attach_code);
        shim::entry(dll, "cbt_switched", l->cbt_switched);
        shim::entry(dll, "cbt_detach", l->cbt_detach);
        shim::entry(dll, "cbt_restore", l->cbt_restore);
        shim::entry(dll, "cbt_settle", l->cbt_settle);
        shim::entry(dll, "cbt_free", l->cbt_free);
        shim::entry(dll, "cbt_state", l->cbt_state);
        shim::entry(dll, "cbt_result_get", l->cbt_result_get);
        lib = l;
    } catch (const py::Exception& e) {   // (no DLL here)
        _WHY.append(py::str(e.type()) + py::str(": ") + py::str(e.what()));
        lib = nullptr;
    }
    _LIB.append(lib);
    return lib;
}

// The recorder is in-process now (it no longer needs the DLL): enabled when the host's Unicorn can be found.
inline bool enabled() {
    try {
        shim::use_host_libraries();
        return true;
    } catch (const py::Exception& e) {
        if (_WHY.empty()) _WHY.append(py::str(e.type()) + py::str(": ") + py::str(e.what()));
        return false;
    }
}
inline std::optional<py::str> why_not() { return !_WHY.empty() ? std::optional<py::str>(_WHY[0]) : std::nullopt; }

// What a run keeps with its machine (`cache`): the import landing places, made again only when what they were made from
// changes. (Kept for the old callers; the new recorder takes the list of sentinels itself.)
struct SentinelCache {
    bool has = false;
    std::vector<const void*> from;
    int64_t count = 0;
    Names names;
    std::shared_ptr<std::vector<uint64_t>> sent;
    int64_t nsent = 0;
};

// The arguments of Run: trace_function's own, already worked out. `statics` its [(lo, hi, image, table)], `sentinels`
// {address: (dll, name)} (empty when not stubbing), `saved` the keys of the reuse dict (empty optional: not reusing),
// `gov` emulator._GOV.
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

namespace shim {

// What the old callers' code tables map to in the process's cache, kept for the life of the process (emulator._INSN).
inline cb::emulate::native_trace::TraceCache& process_cache() {
    static cb::emulate::native_trace::TraceCache cache;
    return cache;
}

inline cb::emulate::native_trace::StaticId static_id(uint64_t lo, uint64_t hi, const py::bytes& image, const std::shared_ptr<void>& table) {
    static std::mutex lock;
    std::lock_guard<std::mutex> hold(lock);
    auto it = _STATICS.find(table.get());
    if (it == _STATICS.end() || std::get<1>(it->second) != table) {
        const std::string& raw = image.raw();
        const int32_t id = process_cache().add_code(lo, cb::ByteView(reinterpret_cast<const uint8_t*>(raw.data()), static_cast<size_t>(hi - lo)));
        _STATICS[table.get()] = std::make_tuple(id, table);
        return id;
    }
    return std::get<0>(it->second);
}

}  // namespace shim

// One run's native hooks on `uc`.
class Run {
  public:
    Engine uc;
    Gov* gov = nullptr;
    int64_t lean_seen = 0;

    explicit Run(const Engine& uc_, const RunArgs& a) : uc(uc_), gov(a.gov) {
        namespace nt = cb::emulate::native_trace;
        shim::use_host_libraries();
        nt::Options o;
        o.cache = &shim::process_cache();
        for (const auto& s : a.statics) o.statics.push_back(shim::static_id(std::get<0>(s), std::get<1>(s), std::get<2>(s), std::get<3>(s)));
        o.init_pages = a.init_pages.vec();
        o.mapped = a.mapped.vec();
        if (a.saved) o.saved = *a.saved;
        for (const auto& kv : a.sentinels.items()) {
            o.sentinels.push_back({std::get<0>(kv), {std::get<0>(std::get<1>(kv)).utf8(), std::get<1>(std::get<1>(kv)).utf8()}});
        }
        o.module_hash = a.sha.utf8();
        governor_.n = a.gov->n;
        governor_.ceil = a.gov->ceil;
        o.governor = &governor_;
        lean_ = a.lean;
        branches_ = a.branches;
        o.fast = a.fast;
        o.lean = a.lean;
        o.reads = a.reads;
        o.stop_wild = a.stop_wild;
        o.stub = a.stub;
        o.unique = a.unique;
        o.branches = a.branches;
        if (a.quiet) o.quiet = nt::AddressRange{std::get<0>(*a.quiet), std::get<1>(*a.quiet)};
        if (a.pads) o.pads = nt::AddressRange{static_cast<uint64_t>(std::get<0>(*a.pads)), static_cast<uint64_t>(std::get<1>(*a.pads))};
        o.insn_cap = a.insn_cap;
        o.stub_ret = a.stub_ret;
        o.stub_cleanup = a.stub_cleanup;
        o.func = {std::get<0>(a.func), std::get<1>(a.func)};
        o.image = {std::get<0>(a.sec), std::get<1>(a.sec)};
        o.lean_low_end = std::get<0>(a.lean_ranges);
        o.lean_high_begin = std::get<1>(a.lean_ranges);
        SigResolver sig = a.sig_resolver;
        Outs outs = a.outs;
        o.resolver = [sig, outs](const nt::ImportName& n) {
            nt::ImportAnswer answer;
            const py::str dll(n.dll), name(n.name);
            if (n.dll == "CALLBACK") {
                answer.argc = 0;
            } else if (sig) {
                answer.argc = sig(dll, name);
            }
            if (answer.argc && *answer.argc != 0 && n.dll != "CALLBACK" && outs) {
                for (const auto& [i, v] : outs(name)) answer.outs.push_back({i, v});
            }
            return answer;
        };
        o.shadow = static_cast<cb::emulate::tracer::Shadow*>(a.shadow);
        run_ = std::make_unique<nt::TraceRun>(static_cast<uc_struct*>(uc_.uch), std::move(o));
    }
    Run(const Run&) = delete;
    Run& operator=(const Run&) = delete;

    // uc.emu_start, then raise what a Python hook would have raised in it.
    void start(const Engine& uc_, int64_t begin, int64_t until, int64_t count = 0) {
        run_->before_host_start();
        std::exception_ptr pending;
        try {
            uc_.emu_start(begin, until, count);
        } catch (...) {
            pending = std::current_exception();
        }
        try {
            run_->after_host_start();
            sync();
        } catch (...) {
            if (!pending) throw;   // (the host's own error comes first)
        }
        if (pending) std::rethrow_exception(pending);
    }
    bool switched() { return run_->switched(); }
    void attach_code() {
        try {
            run_->attach_code();
        } catch (const cb::EmulationError&) {
            throw UcError(0);
        }
    }
    void settle(TraceRecord& rec) {
        run_->settle();
        rec.truncated = run_->truncated();
    }
    // The record's fields, as trace_function's Python hooks leave them.
    void collect(TraceRecord& rec) {
        const cb::emulate::native_trace::TraceRecord r = run_->collect();
        sync();
        convert(r, rec, lean_, branches_);
        lean_seen = run_->lean_seen();
    }
    void detach() {
        try {
            run_->detach();
        } catch (const cb::EmulationError&) {
            throw py::RuntimeError("cbtrace: a hook could not be taken off");
        }
    }
    void restore() {
        try {
            run_->restore();
        } catch (const cb::EmulationError&) {
            throw UcError(0);
        }
    }

  private:
    void sync() { gov->n = governor_.n; }

    static void convert(const cb::emulate::native_trace::TraceRecord& r, TraceRecord& rec, bool lean, bool branches) {
        namespace nt = cb::emulate::native_trace;
        rec.insn_count = r.insn_count;
        rec.truncated = r.truncated;
        if (r.governed) rec.governed = true;
        py::list<uint64_t> path;
        for (uint64_t a : r.path) path.append(a);
        rec.path = path;
        py::dict<uint64_t, py::str> mnemonics;
        for (const auto& [addr, text] : r.mnemonics) mnemonics.set(addr, py::str(text));
        rec.mnemonics = mnemonics;
        py::list<CallRecord> calls;
        for (const nt::CallRecord& c : r.calls) {
            CallRecord cr;
            cr.pc = static_cast<int64_t>(c.pc);
            if (c.target) cr.target = static_cast<int64_t>(*c.target);
            cr.indirect = c.indirect();
            cr.into_func = c.into_func;
            if (c.resolved_target) {
                cr.resolved_target = static_cast<int64_t>(*c.resolved_target);
                cr.external = c.external;
            }
            if (c.stub) {
                cr.api = py::str(c.stub->api);
                cr.stubbed = true;
                cr.has_ret_sentinel = true;
                if (c.stub->ret_sentinel) cr.ret_sentinel = *c.stub->ret_sentinel;
            }
            if (c.args) {
                py::list<int64_t> args;
                for (uint64_t w : c.args->stack) args.append(static_cast<int64_t>(w));
                cr.args_stack = args;
                cr.ecx = static_cast<int64_t>(c.args->ecx);
                cr.edx = static_cast<int64_t>(c.args->edx);
            }
            if (c.argc) cr.argc = *c.argc;
            calls.append(cr);
        }
        rec.calls = calls;
        py::list<MemRead> reads;
        for (const nt::MemRead& m : r.mem_reads) reads.append(MemRead{m.pc, m.addr, m.size, m.value, m.n});
        rec.mem_reads = reads;
        if (lean) {                      // lean runs keep 4-word writes and the pages as first written
            py::list<LeanWrite> writes;
            for (const nt::LeanWrite& w : r.lean_writes) writes.append(LeanWrite{w.addr, w.size, w.value, w.pc});
            rec.mem_writes = writes;
            py::dict<uint64_t, py::bytes> pages0;
            for (const auto& [base, page] : r.pages0) pages0.set(base, cb::compat::to_py(cb::ByteView(page)));
            rec.pages0 = pages0;
        } else {
            py::list<MemWrite> writes;
            for (const nt::MemWrite& w : r.mem_writes) writes.append(MemWrite{w.pc, w.addr, w.size, w.value, w.n, w.old});
            rec.mem_writes = writes;
        }
        py::list<LazyMap> lazy;
        for (const nt::LazyMap& l : r.lazy_maps) lazy.append(LazyMap{l.pc, l.addr, l.page, py::str(std::string(nt::to_text(l.access))), l.size});
        rec.lazy_maps = lazy;
        if (r.memops) rec.memops = *r.memops;
        if (r.writes_seen) rec.writes_seen = *r.writes_seen;
        if (r.dirty) rec._dirty = true;
        if (r.stuck) rec._stuck = *r.stuck;
        if (r.bogus_stubs) rec.bogus_stubs = *r.bogus_stubs;
        if (r.ran_into_zeros) rec.ran_into_zeros = true;
        if (!r.api_deps.empty()) {
            py::list<py::str> deps;
            for (const std::string& d : r.api_deps) deps.append(py::str(d));
            rec.api_deps = deps;
        }
        if (r.api_calls) {
            py::list<std::tuple<py::str, std::optional<py::list<int64_t>>>> seq;
            for (const nt::ApiCall& c : *r.api_calls) {
                std::optional<py::list<int64_t>> args;
                if (c.args) {
                    py::list<int64_t> a;
                    for (int64_t w : *c.args) a.append(w);
                    args = a;
                }
                seq.append(std::make_tuple(py::str(c.api), args));
            }
            rec.api_calls = seq;
        }
        if (r.stubbed_externals) rec.stubbed_externals = *r.stubbed_externals;
        if (!r.vxd_services.empty()) {
            py::list<uint64_t> vxd;
            for (uint64_t v : r.vxd_services) vxd.append(v);
            rec.vxd_services = vxd;
        }
        if (branches) {
            py::list<BranchEntry> log;
            for (const nt::BranchEntry& b : r.branches) {
                std::optional<Compare> cmp;
                if (b.compare) {
                    cmp = Compare{py::str(std::string(nt::to_text(b.compare->op))), b.compare->size, b.compare->a, b.compare->b, b.compare->at_a, b.compare->at_b};
                }
                log.append(BranchEntry{b.address, b.taken, cmp});
            }
            rec.branches = log;
        }
    }

    bool lean_ = false, branches_ = false;
    cb::emulate::native_trace::Governor governor_;
    std::unique_ptr<cb::emulate::native_trace::TraceRun> run_;
};

// The recorder is C++ only: raise when the host's Unicorn cannot be found, saying why.
inline void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer): ") + (why ? *why : py::str("None")));
    }
}

}  // namespace codebreaker::emulate::native_trace
