// codebreaker/emulate/windows16.hpp -- interface of codebreaker.emulate.windows16 (typed C++; implementation pending).
//
// windows16.py -- run a function of a 16-bit Windows module.
//
//     auto r = windows16::probe("commdlg.dll", seg=1, offset=0x1234, args=[...], near=false);
//
// The module's segments are laid out in protected mode, each behind a selector of its own, with its relocations
// applied; every import is a far address in a segment of landing places, so a call to Windows is seen, named, and
// returned from with the arguments it was given. The function is entered the way it is really called (far or near).
// Memory is kept by the same hooks as the 32-bit emulator's (native_win16.hpp). What a run did is judged in
// verify/lanes/win16.
//
// Typing notes:
//   * The record trace_function16()/probe() build as a dict is `Trace16` (every key they set; std::optional where a
//     key is not always there).  An early return {"exit": "bad-ordinal", "ordinal": n} is a Trace16 with only those two.
//   * An import hit (the dict import_stub is handed, and imports_hit holds) is `Hit`; the stub's answer {ax, dx, pop}
//     is `StubResult`; default_stub's `ctx` is the HeapContext interface (a test passes a stand-in with alloc_heap);
//     the Unicorn module `U` the Python threads through is not needed.
//   * run::Call::detail for this lane is the Python's whole `hit` dict; run.hpp's Detail variant only holds the
//     {api, n} note -- the lane (verify/lanes/win16) reads the hits from Trace16::imports_hit (Run.raw) instead.
//   * `context` (a call recorded in the running system, replayed) is the Context record; `callees`, `prelude` and
//     `mem_init` are typed below.  The Python mutates context["_code_bases"] while loading: Context::_code_bases.
//   * run.hpp's Run.raw is a std::shared_ptr<Trace16> behind the opaque shared_ptr<void>.
//   * `shadow` is the native_shadow::Shadow a Shadow16 made (null: none).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/ports16.hpp"
#include "codebreaker/emulate/run.hpp"
#include "py.hpp"

namespace codebreaker::emulate::windows16 {

using core16::Uc;

// the same caps as the 32-bit emulator's (imported from emulator)
using emulator::_MAX_INSNS;
using emulator::_PAGE;

// ---- GDT / memory layout (linear, 32-bit protected mode) -------------------------------------------------------
inline constexpr int64_t GDT_ADDR = 0x1000;
inline constexpr int64_t SEG_ORIGIN = 0x100000;   // first NE segment base; each segment gets a 0x10000 (64K) window
inline constexpr int64_t SEG_STRIDE = 0x10000;
inline constexpr int64_t STACK_BASE = 0x900000;   // stack segment base
inline constexpr int64_t SENT_BASE = 0xA00000;    // import-sentinel segment base (far calls here are stubbed)
inline constexpr int64_t SCRATCH_BASE = 0xB00000; // fallback segment every "unknown" selector points at
inline constexpr int64_t _N_GDT = 512;            // dense GDT: fill every selector index so loads/far-rets never #GP
inline constexpr int64_t _FIXED_SLOT = 101;       // the stack's GDT index (the sentinel's next): the same for any module
inline constexpr int64_t _MODULE_SLOT = 300;      // the module's segment n is at GDT index _MODULE_SLOT + n - 1
inline constexpr int64_t _IMPORT_SENT_STRIDE = 0x10;   // per-import slot inside the SENT segment
inline constexpr int64_t ENTRY_AX = 0x7E57;       // AX as a function is entered: handed back, it set no result
inline constexpr int64_t NEAR_RET = 0xFFEE;       // where a near function entered by the tracer returns to
inline constexpr int64_t FAR_RET = 0xFFF0;        // offset in the sentinel segment a far function returns to
inline constexpr int64_t LDT_ADDR = 0x1F00000;    // a recorded call's LDT
inline constexpr int64_t FOREIGN_BASE = 0x2000000;// the recorded segments' copies, 64K apart
inline constexpr int64_t SNAP_BASE = 0x10000000;  // a recorded call's whole memory
inline constexpr int64_t OWN_BASE = 0x8000000;    // the module's own code as the recorded memory's selectors name it
inline constexpr int64_t HEAP_BASE = 0xB00000;    // first far-heap block (distinct from seg/stack/scratch/sentinel windows)
inline constexpr int64_t HEAP_STRIDE = 0x10000;

// A selector for GDT index `index` at privilege `rpl`.  (used by: verify.lanes.win16)
constexpr int64_t _sel(int64_t index, int64_t rpl = 0) { return (index << 3) | (rpl & 3); }
inline constexpr int64_t SAFE_ARG = (200 << 3);          // a selector that names a real, writable scratch segment
inline constexpr int64_t STACK_SEL = (_FIXED_SLOT << 3); // the stack's selector, whatever the module (_load_ne)

// Build one 8-byte GDT descriptor. 16-bit (D/B=0) byte-granular segment by default.
py::bytes _desc(int64_t base, int64_t limit, bool code, int64_t dpl = 0, bool big = false);

// Where the module's segment `idx` (1-based) is laid out: its own 64K window.
int64_t segment_base(int64_t idx);

// The base the descriptor for `sel` gives, read from the GDT as it is now (0 past its end).
// used by: emulate.shadow (as shadow::SegBase), verify.lanes.win16, tests
int64_t selector_base(Uc& uc, int64_t sel);

// The module's data segment as the loader leaves it -- its fix-ups applied (the selectors of far pointers kept in it)
// -- the bytes a rebuilt module is given in place of its own, so both sides start from the same globals.
// -> bytes (empty when it has no data segment).  used by: verify.gate16.isolated16
py::bytes relocated_data(const py::str& path);

// ---- an import hit, the stub's answer, the heap -----------------------------------------------------------------
// The dict import_stub is handed (and rec["imports_hit"] holds): the import's record from the loader (kind, module,
// ordinal, name_off, api) plus what the sentinel landing adds.  A hit for "callback <name>" has only api and n.
struct Hit {
    std::optional<py::str> api;                    // "MODULE.FUNC" / "MODULE.@ord" / "far ssss:oooo" / "callback <name>"
    std::optional<py::str> kind;                   // IMPORTORD / IMPORTNAME / OSFIXUP
    std::optional<int64_t> module, ordinal, name_off;
    std::optional<int64_t> slot;                   // None for an int 21h answered as DOS3Call
    std::optional<py::list<int64_t>> args_stack;   // 12 Pascal words above the return address
    std::optional<int64_t> n;                      // the instruction count at the call
    std::optional<int64_t> ret_ip, ret_cs;         // where it returns to
    py::dict<int64_t, py::bytes> pointed;          // argument index -> the string it points at (up to a NUL)
    bool foreign = false;                          // a far call into a recorded system segment
    std::optional<int64_t> int_;                   // "int": the interrupt number (21h)
    std::optional<bool> unmodelled;                // the stub did not model it (set by probe's wrapper)
    std::optional<int64_t> arg_bytes;              // bytes of Pascal arguments it removes (set by probe's wrapper)
};

// {ax, dx, pop}: DX:AX the result and the Pascal argument bytes the callee removes (any may be absent; probe adds
// `pop` when the stub left it out).  An empty answer is Python's {} / None.
struct StubResult {
    std::optional<int64_t> ax, dx, pop;
};

// A far-heap allocator that installs real GDT descriptors; what default_stub asks of its context (a test passes a stand-in).
class HeapContext {
  public:
    virtual ~HeapContext() = default;
    virtual int64_t alloc_heap(int64_t size = 0x10000) = 0;   // -> a new far selector
};

// Per-run oracle state (Python's _Ctx): a far-heap allocator that installs real GDT descriptors so GlobalAlloc returns
// genuinely distinct far selectors, and fed file input.
class Ctx : public HeapContext {
  public:
    Ctx(Uc& uc);
    Uc& uc;
    int64_t n = 0;
    py::dict<int64_t, std::tuple<int64_t, int64_t>> heaps;    // selector -> (base, size)
    py::bytes feed;                                           // bytes handed to _lread
    int64_t feed_pos = 0;
    int64_t alloc_heap(int64_t size = 0x10000) override;
};

// Model the Win16 APIs an LZ-class module calls, as the knowledge base's win16 api_model says. `uc` may be null.
StubResult default_stub(const Hit& hit, Uc* uc, HeapContext& ctx);

// import_stub(hit, uc) -> answer (or nothing): lets a caller model Win16 API calls.
using ImportStub = std::function<std::optional<StubResult>(Hit&, Uc&)>;
// probe's `stub`: as ImportStub but with the run's heap context.
using Stub = std::function<StubResult(Hit&, Uc&, HeapContext&)>;

// ---- inputs of a run ---------------------------------------------------------------------------------------------
// callees: {(seg, offset): (name, bytes of arguments it removes (negative: not known), far)} -- the module's other
// functions, not run: reaching one is a call recorded and answered at once with a result made from its words.
using Callees = py::dict<core16::SegOff, std::tuple<py::str, int64_t, bool>>;

// One setup call of `prelude`: given by (seg, offset) or by entry-table ordinal.
struct PreludeCall {
    std::optional<int64_t> seg, offset, ordinal;
    py::list<int64_t> args;
};

// mem_init's `where`: a linear address or ("D", offset): an offset in the module's data segment, wherever this build has it.
using Where = std::variant<int64_t, std::tuple<py::str, int64_t>>;
using MemInit = py::list<std::tuple<Where, py::bytes>>;

// A call recorded in the running system (a breakpoint at the function's entry), replayed.
//   stack/sp/ss/ds/es   its stack segment's bytes, SP at entry, SS, DS and ES selectors
//   regs                {"AX": ..} the registers at entry
//   ldt                 {selector: (bytes, is_code)}; data_sel the module's own data selector there
//   snapshot/ldt_at     the whole of memory at the call, and where the LDT was and its limit
//   own                 {selector: segment} the module's own code segments (far pointers sent on by name)
//   redirect            {(segment, offset): (target segment, target offset, name)} that side's function of the same name
//   key                 the recording's name (kept in cache keys)
struct Context {
    py::bytes stack;
    int64_t sp = 0, ss = 0;
    std::optional<int64_t> ds, es;
    py::dict<py::str, int64_t> regs;
    py::dict<int64_t, std::tuple<py::bytes, bool>> ldt;
    std::optional<int64_t> data_sel;
    std::optional<py::bytes> snapshot;
    std::optional<std::tuple<int64_t, int64_t>> ldt_at;
    py::dict<int64_t, int64_t> own;
    py::dict<core16::SegOff, std::tuple<int64_t, int64_t, py::str>> redirect;
    std::optional<py::str> key;
    py::dict<int64_t, int64_t> _code_bases;   // filled in by the LDT loader: {selector: linear base}
};

// ---- the record ---------------------------------------------------------------------------------------------------
// rec["layout"]
struct Layout {
    py::dict<int64_t, int64_t> seg_bases;
    std::optional<int64_t> ds_base;
    int64_t autodata_seg = 0, seg_stride = SEG_STRIDE, stack_base = STACK_BASE, scratch_base = SCRATCH_BASE,
            sent_base = SENT_BASE;
};
// a call into another of the module's functions, recorded and answered (rec["calls_out"])
struct CalleeCall {
    py::str name;
    py::list<int64_t> args;
    int64_t sp = 0, n = 0, ret_ip = 0;
};
// rec["fault"]: the instruction at the faulting CS:EIP
struct Fault {
    int64_t cs = 0, eip = 0;
    std::optional<py::str> insn;
    py::str bytes;   // the first 8 bytes, hex
};
// SI DI DS as they went in and came out; AX..SS at entry
using Regs = py::dict<py::str, int64_t>;

// trace_function16's / probe's record: schema "cb.behavior.trace16/1".
struct Trace16 {
    py::str schema = py::str("cb.behavior.trace16/1");
    py::str module;
    int64_t seg = 0, offset = 0;
    std::optional<int64_t> ordinal;
    emulator::TraceInput input;
    int64_t insn_count = 0;
    py::list<uint64_t> path;
    py::list<native_trace::MemRead> mem_reads;
    py::list<native_trace::MemWrite> mem_writes;
    py::list<native_trace::CallRecord> calls;
    py::list<Hit> imports_hit;
    bool truncated = false;
    std::optional<py::str> exit;     // "ret" | "truncated" | "bad-ordinal" | "exception: ..."
    std::optional<int64_t> ret;      // AX
    std::optional<int64_t> ret32;    // DX:AX
    Layout layout;
    py::list<CalleeCall> calls_out;
    bool unsure = false;             // how many bytes a callee removes was not known
    std::optional<int64_t> interrupt;   // an interrupt (other than int 21h) that ended the run
    std::optional<bool> returned, limit_fault;
    std::optional<py::list<native_trace::BranchEntry>> branches;
    std::optional<int64_t> prelude_ran;
    std::optional<Regs> entry_regs;  // AX BX CX DX SI DI BP CS DS ES SS at entry
    std::optional<Regs> exit_regs;   // SI DI DS at the end
    std::optional<int64_t> sp_delta;
    std::optional<int64_t> code_base;
    std::optional<int64_t> n_imports;
    std::optional<Fault> fault;
    std::optional<py::list<ports16::LogEntry>> io;   // with ports=True: [(step, "in"|"out", port, value)]
    // set by probe():
    std::optional<std::tuple<int64_t, int64_t>> heaps;   // (HEAP_BASE, HEAP_STRIDE)
    py::dict<int64_t, int64_t> heap_sels;                // selector -> base of the stand-in heaps made
    // set by native_win16's collect():
    std::optional<py::list<native_trace::LazyMap>> lazy_maps;
    std::optional<int64_t> memops, writes_seen, _stuck;
};

// trace_function16's keyword arguments.
//   ordinal / seg+offset   identify the function by entry-table ordinal or explicit (seg, offset)
//   args      16-bit Pascal args (pushed left-to-right); default `arity` zeros
//   near      enter a NEAR function: only a return offset is pushed
//   data_image written over the start of the data segment before the run
//   callees   the module's other functions, not run (see Callees)
//   data_end  (end of the original's data, 0x10000-style limit): zeros to there, then a pattern to 64K
//   context   a recorded call to replay (args are then not pushed)
//   branches  (low, high) offsets in the function's segment -- also log each conditional jump there
//   bp        BP as the function is entered
//   ports     `in`/`out` reach the UARTs and interrupt controllers of ports16 (rec.io)
//   shadow    a Shadow16 kept up beside the run (null: none)
struct Trace16Options {
    std::optional<int64_t> ordinal, seg, offset;
    std::optional<py::list<int64_t>> args;
    int64_t arity = 0;
    int64_t insn_cap = _MAX_INSNS;
    ImportStub import_stub;                             // (empty: none)
    std::optional<py::list<PreludeCall>> prelude;
    bool near_ = false;   // `near` (spelled near_: a macro in <windows.h>)
    std::optional<py::bytes> data_image;
    std::optional<Callees> callees;
    std::optional<std::tuple<int64_t, int64_t>> data_end;
    std::optional<Context> context;
    std::optional<MemInit> mem_init;
    std::optional<std::tuple<int64_t, int64_t>> branches;
    std::optional<int64_t> bp;
    bool ports = false;
    native_shadow::Shadow* shadow = nullptr;
};

// Trace one exported 16-bit function. Returns a TraceRecord like tracer.trace_function.  Exceptions a callback raised
// come out of here.
Trace16 trace_function16(const py::str& path, const Trace16Options& options = {});

// probe's keyword arguments (trace_function16's, with `stub` the controlled Win16 environment).
//   cdecl     reverse `args` before pushing (INTERNAL functions are often __cdecl16far)
//   stub_ret  what a Windows function `stub` does not model returns in DX:AX (else 0)
struct ProbeOptions {
    std::optional<int64_t> ordinal, seg, offset;
    std::optional<py::list<int64_t>> args;
    Stub stub;                                          // (empty: default_stub)
    std::optional<int64_t> insn_cap;
    std::optional<py::list<PreludeCall>> prelude;
    bool cdecl_ = false;
    bool near_ = false;   // `near` (spelled near_: a macro in <windows.h>)
    std::optional<py::bytes> data_image;
    std::optional<Callees> callees;
    std::optional<std::tuple<int64_t, int64_t>> data_end;
    std::optional<Context> context;
    std::optional<int64_t> stub_ret;
    std::optional<MemInit> mem_init;
    std::optional<std::tuple<int64_t, int64_t>> branches;
    std::optional<int64_t> bp;
    bool ports = false;
    native_shadow::Shadow* shadow = nullptr;
};

// Run one NE function under the controlled Win16 environment. Returns the tracer16 TraceRecord (with layout, imports_hit,
// mem_*, ret, heaps, heap_sels).  used by: verify.gate16.isolated16, verify.lanes.win16, tests
Trace16 probe(const py::str& dll, const ProbeOptions& options = {});

// probe's record as the one run record (emulate/run.hpp): AX and DX:AX the results, every Windows call made, the
// devices' log, SI DI DS as they went in and came out, the linear addresses run in order.  run.raw is the Trace16.
run::Run record(const Trace16& rec);

// The platform's runner (platforms/win16.py emulator.run): probe(dll, **kw) as a Run.
run::Run recorded(const py::str& dll, const ProbeOptions& options = {});

}  // namespace codebreaker::emulate::windows16
