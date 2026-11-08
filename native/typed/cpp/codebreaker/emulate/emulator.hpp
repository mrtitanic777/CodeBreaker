// codebreaker/emulate/emulator.hpp -- interface of codebreaker.emulate.emulator (typed C++; implementation pending).
//
// emulator.py -- run one 32-bit function in Unicorn and record a structured TraceRecord: the behavioral engine's
// observation primitive.  The instruction path, every memory read/write, detected calls, faults, and the return value;
// a lazily mapped memory (an unmapped access maps a zeroed page and is recorded); a decoded path.  The hooks are C++
// (native_trace.hpp); this module lays the machine out, writes the frame, starts each run and reads the result.
//
// Typing notes:
//   * trace_function's keyword arguments are TraceOptions (as powerpc.hpp's RunOptions); `func` selects a function by
//     None / an int (offset or VA) / a string ("0x...", decimal) -- FuncSelector.
//   * The record the Python builds as a dict is `Trace` (every key it sets, as std::optional where it is not always
//     there).  Its sub-records (CallRecord, MemRead, MemWrite, LeanWrite, LazyMap, BranchEntry) are native_trace.hpp's.
//     The key "mnemonics" is the stringified one ("0x%x" -> text) the Python leaves after trace_function.
//     "mem_writes" is a variant: MemWrite records, or (lean) 4-word tuples.
//   * run.hpp's Run.raw (`record()`'s `raw=rec`) is a std::shared_ptr<Trace> behind the opaque shared_ptr<void>.
//   * The Unicorn engine is `Uc` (core16_types.hpp); Python's `shadow` argument (an emulate/shadow.py Shadow32) is the
//     native_shadow::Shadow it returned (null: none).
//   * _load() returns a struct (the Python's 7-tuple); _resolve_func() likewise (5-tuple).
//   * Module-level state a test or sibling reaches into (_MACHINES, _LOADED, _RESOLVED, _GOV, _DEFAULT_STUB) is
//     declared extern.  Names with a leading underscore are spelled as in the Python (several are used by
//     emulate/driver, emulate/windows16, verify/lanes/win32 and tests).
#pragma once

#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>
#include <vector>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/run.hpp"
#include "py.hpp"

namespace codebreaker::emulate::emulator {

using core16::Uc;

// Entry-state constants, identical to difftest._emulate so traces are comparable to the oracle difftest trusts.
inline constexpr int64_t _SCRATCH_BASE = 0x200000, _SCRATCH_SIZE = 0x10000;
// The stack has a region of its own.
inline constexpr int64_t _STACK_BASE = 0x300000, _STACK_SIZE = 0x40000;
// EBX, ESI, EDI, EBP at entry
inline constexpr std::array<int64_t, 4> SAVED_SENTINELS = {0xB0B0B0B0, 0x51515151, 0xD1D1D1D1, 0xBDBDBDBD};
inline constexpr int64_t _STACK_TOP = _STACK_BASE + _STACK_SIZE - 0x1000, _RET = 0xDEAD0000;
inline constexpr int64_t _PAGE = 0x1000;

// Recording caps -- a trace is evidence, not a full replay log, so it is bounded and records that it was.
inline constexpr int64_t _MAX_INSNS = 20000;
inline constexpr int64_t _MAX_MEM = 40000;

// Per-FUNCTION instruction governor: a DETERMINISTIC ceiling on the TOTAL instructions emulated per function
// ({"n", "ceil"}); recon.reconstruct_one resets it; the hook stops emulation once it is exceeded.
extern native_trace::Gov _GOV;

// IAT sentinels: a unique, recognizable landing address per import slot.
inline constexpr int64_t _SENT_BASE = 0xE1000000, _SENT_STRIDE = 0x10;

// Process-wide default for external-call stubbing, set once so EVERY trace inherits it; an explicit argument to
// trace_function always overrides this.
struct DefaultStub {
    bool enabled = false;
    native_trace::SigResolver resolver;   // (empty: none)
};
extern DefaultStub _DEFAULT_STUB;

// --- the module as loaded ------------------------------------------------------------------------------
// where a recovered function ends (the Python's {"end": offset})
struct FuncExtent {
    int64_t end = 0;
};
// _load(module): (path, va0, code, secname, funcs, sha, imports) for a module, carved once and cached.
struct Loaded {
    py::str path;
    int64_t va0 = 0;
    py::bytes code;                                              // every executable section, laid out from va0
    py::str secname;
    py::dict<int64_t, FuncExtent> funcs;                         // offset (from va0) -> its extent
    py::str sha;                                                 // sha256 of the file
    py::dict<int64_t, std::tuple<py::str, py::str>> imports;     // iat_slot_va -> (dll, name) from the import directory
};
// used by: emulate.windows16, tests (test_native_trace)
const Loaded& _load(const py::str& module);
extern py::dict<py::str, Loaded> _LOADED;   // the name a module was asked for by -> the same, without asking the disk again

// A func selector: None (first function), a section offset or VA, or a string ("0x...", decimal).
using FuncSelector = std::variant<py::none_t, int64_t, py::str>;
// _resolve_func(module, func): (off, va0, code, end, secname)
struct ResolvedFunc {
    int64_t off = 0, va0 = 0;
    py::bytes code;
    int64_t end = 0;
    py::str secname;
};
// Map a user-supplied func selector to the function's offset, base, code, end and section (memoized in _RESOLVED).
// Raises ValueError for a module with no functions or no function at the address.  used by: verify.lanes.win32
ResolvedFunc _resolve_func(const py::str& module, const FuncSelector& func);
extern py::dict<std::tuple<py::str, FuncSelector>, ResolvedFunc> _RESOLVED;

// --- stubbed APIs ------------------------------------------------------------------------------------------
// [(place, value)] of an API's parameters through which it gives something back, by its declaration: a handle
// (value None: _OUT_HANDLE by its place) or a number (0).  (native_trace's Outs.)
py::list<std::tuple<int64_t, std::optional<int64_t>>> _outs(const py::str& name);

// --- the branch log (trace_function(branches=True)) ----------------------------------------------------------
inline constexpr int64_t _BRANCH_CAP = 4096;   // branch events kept per run
extern const py::list<py::str> _COMPARES;      // mnemonics whose result a following jump decides on
extern const py::list<py::str> _FLAGLESS;      // instructions that leave the flags alone
extern const py::dict<py::str, py::str> _PARENT;   // capstone register name -> the 32-bit register it is part of

// Unicorn's id of an x86 register by (any-case) name (unicorn.x86_const.UC_X86_REG_<NAME>), None when there is none.
std::optional<int64_t> _uc_reg(const py::str& name);

// --- machines kept between runs ---------------------------------------------------------------------------------
inline constexpr int64_t _MAX_MACHINES = 4;
// A fresh emulator with the module laid out: its image, the original beside it (`overlay`), the caller's memory, and
// a landing place in every import slot (the Python's dict {"uc", "sentinels", "statics", "mapped", "slots",
// "init_pages", "context"}; `cache` is what native_trace keeps with it).
struct Machine {
    std::shared_ptr<Uc> uc;
    py::dict<int64_t, std::tuple<py::str, py::str>> sentinels;       // sentinel addr -> (dll, name)
    std::vector<std::tuple<uint64_t, uint64_t, py::bytes, std::shared_ptr<void>>> statics;   // (lo, hi, image, its decode table)
    py::set<int64_t> mapped;                                         // page-group bases mapped
    py::dict<int64_t, py::list<std::tuple<int64_t, py::bytes>>> slots;   // page -> [(address, bytes)] of import slots
    py::set<int64_t> init_pages;
    std::shared_ptr<void> context;                                   // uc.context_save()
    native_trace::SentinelCache cache;
};
// (module path, its hash, overlay, stubbing, fast)
using MachineKey = std::tuple<py::str, py::str, std::optional<py::str>, bool, bool>;
extern py::dict<MachineKey, std::shared_ptr<Machine>> _MACHINES;   // a machine with it laid out, between runs

// --- the record ---------------------------------------------------------------------------------------------------
struct TraceInput {
    py::list<int64_t> args;
    int64_t arity = 0;
};
// trace_function's record (a plain dict in the Python): schema "cb.behavior.trace/1".
struct Trace {
    py::str schema = py::str("cb.behavior.trace/1");
    py::str module;                         // clean label (module may be a path)
    py::str module_path;                    // the resolved file actually traced
    py::str target_sha256;
    py::str section;
    int64_t func_off = 0, func_va = 0, func_end_va = 0;
    TraceInput input;
    int64_t entry_esp = 0;                  // rec["entry"]["esp"]
    int64_t insn_count = 0;
    py::list<uint64_t> path;
    py::dict<py::str, py::str> mnemonics;   // "0x<va>" -> "mov eax, [ecx]" (first time each va is seen)
    py::list<native_trace::CallRecord> calls;
    py::list<native_trace::MemRead> mem_reads;
    // mixed in the Python by `lean`: lean records are 4-word tuples, the others MemWrite records
    std::variant<py::list<native_trace::MemWrite>, py::list<native_trace::LeanWrite>> mem_writes;
    py::list<native_trace::LazyMap> lazy_maps;
    bool truncated = false;
    std::optional<py::str> exit;            // "ret" | "truncated" | "exception: ..."
    std::optional<int64_t> ret;             // EAX at the RET sentinel
    std::optional<std::array<int64_t, 4>> saved_regs;   // EBX ESI EDI EBP at the end
    std::optional<int64_t> esp_delta;       // 4 = cdecl, 4 + N = stdcall with N bytes
    std::optional<double> st0;              // x87 return
    std::optional<py::dict<uint64_t, py::bytes>> pages0;   // lean runs only: the pages as first written
    // set by native_trace's collect()/settle(), as in TraceRecord:
    std::optional<bool> governed;
    std::optional<int64_t> memops, writes_seen, _stuck, bogus_stubs, stubbed_externals;
    std::optional<bool> _dirty, ran_into_zeros;
    std::optional<py::list<py::str>> api_deps;
    std::optional<py::list<std::tuple<py::str, std::optional<py::list<int64_t>>>>> api_calls;
    std::optional<py::list<uint64_t>> vxd_services;
    std::optional<py::list<native_trace::BranchEntry>> branches;
};

// trace_function's keyword arguments.
//   args      explicit list of dword stdcall args; None: `arity` zero dwords are pushed
//   arity     number of stdcall dword slots to push when args is None; default: the recovered arity, else 4
//   mem_init  [(addr, bytes)] written before running (pages mapped as needed)
//   stub_externals  a call to an address outside the section returns to the caller with EAX=stub_ret and pops
//             stub_cleanup extra bytes (None: the process default _DEFAULT_STUB)
//   sentinels the registers' values at entry (EBX ESI EDI EBP); None: SAVED_SENTINELS
//   pads      (low, high) address range of padding bytes; branches: also log each conditional jump
//   stub_at   functions entered as if they were imports: {address: (dll, name)}
//   shadow    a Shadow32 kept up beside the run (null: none); the run is the same run
struct TraceOptions {
    std::optional<py::list<int64_t>> args;
    std::optional<int64_t> arity;
    int64_t insn_cap = _MAX_INSNS;
    std::optional<py::list<std::tuple<int64_t, py::bytes>>> mem_init;
    std::optional<bool> stub_externals;
    int64_t stub_ret = 0;
    int64_t stub_cleanup = 0;
    std::optional<native_trace::SigResolver> sig_resolver;
    bool stub_ret_unique = false;
    std::optional<py::str> overlay;
    std::optional<std::array<int64_t, 4>> sentinels;
    bool reads = true;
    bool stop_wild = false;
    bool quiet_stack = false;
    bool reuse = false;
    bool fast = false;
    bool lean = false;
    std::optional<int64_t> ecx, edx;
    std::optional<std::tuple<int64_t, int64_t>> pads;
    bool branches = false;
    std::optional<py::dict<int64_t, std::tuple<py::str, py::str>>> stub_at;
    native_shadow::Shadow* shadow = nullptr;
};

// Run one function under Unicorn with full hooks; return a TraceRecord.  Raises ValueError when the module has no
// function there; an exception a native hook raised comes out of here.  used by: analyze.observe, tests
Trace trace_function(const py::str& module, const FuncSelector& func = py::none_t{}, const TraceOptions& options = {});

// trace_function's record as the one run record (emulate/run.hpp): EAX (and x87 ST0) the results, the imports and
// callbacks entered with their arguments, EBX ESI EDI EBP against the sentinels they were given (`sentinels`, else
// SAVED_SENTINELS), every instruction run (first seen first).  A write made on the way out of zeros is the harness's.
// run.raw holds the Trace (std::shared_ptr<Trace>).  used by: tests (test_run_record)
run::Run record(const Trace& rec, const std::optional<std::array<int64_t, 4>>& sentinels = std::nullopt);

// The platform's runner (platforms/win32.py emulator.run): trace_function(module, **kw) as a Run.
run::Run recorded(const py::str& module, const FuncSelector& func = py::none_t{}, const TraceOptions& options = {});

// Argument count from the knowledge base's win32 api_model (A/W variants sharing one), or None.
// used by: verify.lanes.win32
std::optional<int64_t> _builtin_argc(const std::optional<py::str>& name);

}  // namespace codebreaker::emulate::emulator
