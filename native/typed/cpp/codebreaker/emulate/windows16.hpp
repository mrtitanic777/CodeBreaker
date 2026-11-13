// codebreaker/emulate/windows16.hpp -- COMPAT SHIM over cb/emulate/windows16.hpp (docs/CPP_DESIGN.md 6.3), for the ports that still use the
// old py:: surface (verify/lanes/win16, verify/gate16/isolated16, build/module, platforms/win16, emulate/shadow). Conversion and forwarding
// only; deleted with its last py:: consumer.
//
// The module (layout, loader, relocations, the run and its events, the 16-bit branch log, the recorded call's LDT, the stand-ins) is
// native/cb/emulate/windows16, called in-process by one process-wide Runner. Here: the old records (Hit, Context, Trace16 and their options as
// bags of optionals over py containers), the process-wide governor (`emulator::_GOV`, copied to the Runner's before each run and back after), the
// models and argument sizes read from the old knowledge base and symbols, and the translation each way.
//
// Not carried over: the SDK symbol tables (`_find`, `_load_symbols`, `struct_`: nothing reads them), the per-run `Ctx` class (probe makes its own
// heap), a hit's module index (the new record has the module's name), and the `_code_bases` the Python filled in on the caller's Context (the table
// is the run's own now). The old code's quirks that were bugs are fixed in the new code, not mimicked here (native/cb/emulate/windows16_test.cpp).
#pragma once

#include <cstdint>
#include <cstdlib>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/windows16.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/ports16.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/knowledge/sources.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "py.hpp"

namespace codebreaker::emulate::windows16 {

using core16::Uc;
namespace nw = ::cb::emulate::windows16;

// the same caps as the 32-bit emulator's (imported from emulator)
using emulator::_MAX_INSNS;
using emulator::_PAGE;

// ---- GDT / memory layout (linear, 32-bit protected mode) -------------------------------------------------------
inline constexpr int64_t GDT_ADDR = nw::kGdtAddress;
inline constexpr int64_t SEG_ORIGIN = nw::kSegmentOrigin;   // first NE segment base; each segment gets a 0x10000 (64K) window
inline constexpr int64_t SEG_STRIDE = nw::kSegmentStride;
inline constexpr int64_t STACK_BASE = nw::kStackBase;       // stack segment base
inline constexpr int64_t SENT_BASE = nw::kSentinelBase;     // import-sentinel segment base (far calls here are stubbed)
inline constexpr int64_t SCRATCH_BASE = nw::kScratchBase;   // fallback segment every "unknown" selector points at
inline constexpr int64_t _N_GDT = nw::kGdtSlots;            // dense GDT: fill every selector index so loads/far-rets never #GP
inline constexpr int64_t _FIXED_SLOT = nw::kStackSlot;      // the stack's GDT index (the sentinel's next): the same for any module
inline constexpr int64_t _MODULE_SLOT = nw::kModuleSlot;    // the module's segment n is at GDT index _MODULE_SLOT + n - 1
inline constexpr int64_t _IMPORT_SENT_STRIDE = nw::kImportStride;
inline constexpr int64_t ENTRY_AX = nw::kEntryAx;           // AX as a function is entered: handed back, it set no result
inline constexpr int64_t NEAR_RET = nw::kNearReturn;        // where a near function entered by the tracer returns to
inline constexpr int64_t FAR_RET = nw::kFarReturn;          // offset in the sentinel segment a far function returns to
inline constexpr int64_t LDT_ADDR = nw::kLdtAddress;        // a recorded call's LDT
inline constexpr int64_t FOREIGN_BASE = nw::kForeignBase;   // the recorded segments' copies, 64K apart
inline constexpr int64_t SNAP_BASE = nw::kSnapshotBase;     // a recorded call's whole memory
inline constexpr int64_t OWN_BASE = nw::kOwnBase;           // the module's own code as the recorded memory's selectors name it
inline constexpr int64_t HEAP_BASE = nw::kHeapBase;         // first far-heap block
inline constexpr int64_t HEAP_STRIDE = nw::kHeapStride;

// A selector for GDT index `index` at privilege `rpl`.  (used by: verify.lanes.win16)
constexpr int64_t _sel(int64_t index, int64_t rpl = 0) { return (index << 3) | (rpl & 3); }
inline constexpr int64_t SAFE_ARG = (200 << 3);          // a selector that names a real, writable scratch segment
inline constexpr int64_t STACK_SEL = (_FIXED_SLOT << 3); // the stack's selector, whatever the module (_load_ne)

// Build one 8-byte GDT descriptor. 16-bit (D/B=0) byte-granular segment by default.
inline py::bytes _desc(int64_t base, int64_t limit, bool code, int64_t dpl = 0, bool big = false) {
    const auto d = nw::descriptor(static_cast<uint32_t>(base), static_cast<uint32_t>(limit), code, static_cast<unsigned>(dpl), big);
    return ::cb::compat::to_py(::cb::ByteView(d));
}

// Where the module's segment `idx` (1-based) is laid out: its own 64K window.
inline int64_t segment_base(int64_t idx) { return SEG_ORIGIN + idx * SEG_STRIDE; }

// The base the descriptor for `sel` gives, read from the GDT as it is now (0 past its end).
// used by: emulate.shadow (as shadow::SegBase), verify.lanes.win16, tests
inline int64_t selector_base(Uc& uc, int64_t sel) {
    py::bytes d;
    try {
        d = uc.mem_read(GDT_ADDR + (sel >> 3) * 8, 8);
    } catch (const py::Exception&) {
        return 0;
    }
    return d[2] | (d[3] << 8) | (d[4] << 16) | (d[7] << 24);
}

// ---- an import hit, the stub's answer, the heap -----------------------------------------------------------------
// The dict import_stub is handed (and rec["imports_hit"] holds): the import's record from the loader (kind, module,
// ordinal, name_off, api) plus what the sentinel landing adds.  A hit for "callback <name>" has only api and n.
struct Hit {
    std::optional<py::str> api;                    // "MODULE.FUNC" / "MODULE.@ord" / "far ssss:oooo" / "callback <name>"
    std::optional<py::str> kind;                   // IMPORTORD / IMPORTNAME / OSFIXUP
    std::optional<int64_t> module, ordinal, name_off;   // (module: not set by the shim: the module's name is in the new record)
    std::optional<int64_t> slot;
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

// A far-heap allocator that installs real GDT descriptors; what default_stub asks of its context (a test passes a stand-in with alloc_heap).
class HeapContext {
  public:
    virtual ~HeapContext() = default;
    virtual int64_t alloc_heap(int64_t size = 0x10000) = 0;   // -> a new far selector
};

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
    py::dict<int64_t, int64_t> _code_bases;   // (not filled in any more: the run's own table is not the caller's)
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
    std::shared_ptr<Context> context;   // (the run reads it; it is not written to)
    std::optional<MemInit> mem_init;
    std::optional<std::tuple<int64_t, int64_t>> branches;
    std::optional<int64_t> bp;
    bool ports = false;
    native_shadow::Shadow* shadow = nullptr;
};

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
    std::shared_ptr<Context> context;
    std::optional<int64_t> stub_ret;
    std::optional<MemInit> mem_init;
    std::optional<std::tuple<int64_t, int64_t>> branches;
    std::optional<int64_t> bp;
    bool ports = false;
    native_shadow::Shadow* shadow = nullptr;
};

namespace shim {

// ---- the one Runner, over the old world's corpus-free surroundings --------------------------------------------------------
inline std::optional<nw::StubModel> model_of(const py::str& api_in) {
    // Bare API function name, upper-cased (drop the 'MODULE.' prefix); resolve KERNEL @ordinals (the knowledge base's win16 kernel_ordinal:
    // KERNEL's numbers for the functions modelled, for when the call is reported by number, 'KERNEL.@15').
    const py::str api = api_in.upper();
    const int64_t dot = api.find(py::str("."));
    py::str fn = dot >= 0 ? api.slice_from(dot + 1) : api;
    if (fn.startswith(py::str("@")) && api.startswith(py::str("KERNEL"))) {
        try {
            const py::str num = py::str(std::to_string(py::int_parse(fn.slice_from(1))));
            const auto got = knowledge::table(py::str("win16"), py::str("kernel_ordinal")).get(num);
            if (got) {
                const auto* j = std::get_if<knowledge::sources::Json>(&*got);
                if (j && j->kind == knowledge::sources::Json::Str) fn = j->s;
            }
        } catch (const py::ValueError&) {
        }
    }
    const auto model = knowledge::api_model(py::str("win16"), fn);
    if (!model) return std::nullopt;
    auto word = [&](const char* key) -> std::optional<nw::ModelWord> {
        const auto v = model->get(py::str(key));
        if (!v) return std::nullopt;
        if (v->kind == knowledge::sources::Json::Str) {
            if (v->s == py::str("heap")) return nw::ModelWord{nw::FromHeap{}};
            if (v->s.startswith(py::str("arg"))) return nw::ModelWord{nw::FromArgument{static_cast<size_t>(py::int_parse(v->s.slice_from(3)))}};
            throw py::TypeError("unsupported operand type(s) for &: 'str' and 'int'");
        }
        return nw::ModelWord{static_cast<int64_t>(v->i)};
    };
    return nw::StubModel{word("ax"), word("dx"), word("pop")};
}

inline ::cb::emulate::native_trace::Governor& governor() {
    static ::cb::emulate::native_trace::Governor g;
    return g;
}

inline nw::Runner& runner() {
    static nw::Runner instance([] {
        nw::Environment env;
        env.governor = &governor();
        env.models = [](std::string_view api) { return model_of(::cb::compat::to_py(api)); };
        env.argument_bytes = [](std::string_view api) -> std::optional<uint32_t> {
            const std::optional<int64_t> size = symbols::api_arguments::of(std::optional<py::str>(::cb::compat::to_py(api)));
            if (!size || *size < 0) return std::nullopt;
            return static_cast<uint32_t>(*size);
        };
        env.open_engine = []() -> std::unique_ptr<::cb::core16::Uc> {
            native_trace::require();   // points the recorder at the host's libraries, or raises saying why not
            return ::cb::compat::guard([] { return ::cb::emulate::unicorn_uc::open_x86_from(unicorn_uc::shim_detail::dll(), 32); });
        };
        return env;
    }());
    return instance;
}

// ---- old <-> new ---------------------------------------------------------------------------------------------------------------
inline Hit hit_of(const ::cb::emulate::ImportHit& h) {
    Hit out;
    out.api = ::cb::compat::to_py(h.api);
    if (!h.kind.empty()) out.kind = ::cb::compat::to_py(h.kind);
    out.ordinal = h.ordinal;
    out.name_off = h.name_off;
    out.slot = h.slot;
    py::list<int64_t> words;
    for (const int64_t w : h.args_stack) words.append(w);
    out.args_stack = words;
    out.n = h.n;
    out.ret_ip = h.ret_ip;
    out.ret_cs = h.ret_cs;
    for (const auto& [at, bytes] : h.pointed) out.pointed.set(static_cast<int64_t>(at), ::cb::compat::to_py(::cb::ByteView(bytes)));
    out.foreign = h.foreign;
    out.int_ = h.interrupt;
    if (h.unmodelled) out.unmodelled = true;
    out.arg_bytes = h.arg_bytes;
    return out;
}

// The old Uc over a new engine that is only borrowed.
inline std::unique_ptr<core16::Uc> borrowed(::cb::core16::Uc& machine) { return std::make_unique<unicorn_uc::shim_detail::OldUc>(machine); }

inline uint32_t u32(int64_t v) { return static_cast<uint32_t>(static_cast<uint64_t>(v) & 0xFFFFFFFFu); }
inline StubResult stub_of(const nw::StubResult& r) {
    StubResult out;
    if (r.ax) out.ax = *r.ax;
    if (r.dx) out.dx = *r.dx;
    if (r.pop) out.pop = *r.pop;
    return out;
}
inline nw::StubResult stub_to_new(const StubResult& r) {
    nw::StubResult out;
    if (r.ax) out.ax = u32(*r.ax);
    if (r.dx) out.dx = u32(*r.dx);
    if (r.pop) out.pop = u32(*r.pop);
    return out;
}

// What a stand-in of the old kind does for a new hit: it is given the old Hit and the old Uc; the notes it makes on the hit (`unmodelled`,
// `arg_bytes`) stay in the record.
inline void note_back(::cb::emulate::ImportHit& to, const Hit& from) {
    to.unmodelled = from.unmodelled.value_or(false);
    to.arg_bytes = from.arg_bytes;
}
inline nw::ImportStub import_stub_of(const ImportStub& stub) {
    if (!stub) return {};
    return [stub](::cb::emulate::ImportHit& h, ::cb::core16::Uc& machine) -> std::optional<nw::StubResult> {
        Hit hit = hit_of(h);
        const auto uc = borrowed(machine);
        const std::optional<StubResult> r = stub(hit, *uc);
        note_back(h, hit);
        if (!r) return std::nullopt;
        return stub_to_new(*r);
    };
}
// The old HeapContext over the new one.
class OldHeap final : public HeapContext {
  public:
    explicit OldHeap(nw::HeapContext& heap) : heap_(heap) {}
    int64_t alloc_heap(int64_t size) override { return heap_.alloc_heap(u32(size)); }

  private:
    nw::HeapContext& heap_;
};
inline nw::HeapStub heap_stub_of(const Stub& stub) {
    if (!stub) return {};
    return [stub](::cb::emulate::ImportHit& h, ::cb::core16::Uc& machine, nw::HeapContext& heap) -> nw::StubResult {
        Hit hit = hit_of(h);
        const auto uc = borrowed(machine);
        OldHeap old_heap(heap);
        const StubResult r = stub(hit, *uc, old_heap);
        note_back(h, hit);
        return stub_to_new(r);
    };
}

inline nw::Function function_of(const std::optional<int64_t>& ordinal, const std::optional<int64_t>& seg, const std::optional<int64_t>& offset) {
    if (ordinal) return nw::Ordinal{static_cast<uint32_t>(*ordinal)};
    if (!seg || !offset) throw py::KeyError("None");
    return nw::CodePoint{static_cast<uint32_t>(*seg), static_cast<uint32_t>(*offset)};
}

inline nw::RecordedCall context_of(const Context& c) {
    nw::RecordedCall out;
    out.stack = ::cb::compat::from_py(c.stack);
    out.sp = static_cast<uint16_t>(c.sp & 0xFFFF);
    out.ss = static_cast<uint16_t>(c.ss);
    if (c.ds) out.ds = static_cast<uint16_t>(*c.ds);
    if (c.es) out.es = static_cast<uint16_t>(*c.es);
    auto reg = [&](const char* k) { return static_cast<uint16_t>(c.regs.get(py::str(k), 0) & 0xFFFF); };
    out.regs = {reg("AX"), reg("BX"), reg("CX"), reg("DX"), reg("SI"), reg("DI"), reg("BP")};
    for (const auto& [sel, entry] : c.ldt.items()) out.ldt[static_cast<uint16_t>(sel)] = {::cb::compat::from_py(std::get<0>(entry)), std::get<1>(entry)};
    if (c.data_sel) out.data_selector = static_cast<uint16_t>(*c.data_sel);
    if (c.snapshot && !c.snapshot->empty()) {
        if (!c.ldt_at) throw py::KeyError("'ldt_at'");
        out.snapshot = nw::RecordedMemory{::cb::compat::from_py(*c.snapshot), static_cast<uint32_t>(std::get<0>(*c.ldt_at)),
                                          static_cast<uint32_t>(std::get<1>(*c.ldt_at))};
    }
    for (const auto& [sel, seg] : c.own.items()) out.own[static_cast<uint16_t>(sel)] = static_cast<uint32_t>(seg);
    for (const auto& [at, to] : c.redirect.items()) {
        out.redirect[{static_cast<uint32_t>(std::get<0>(at)), static_cast<uint32_t>(std::get<1>(at))}] =
            {static_cast<uint32_t>(std::get<0>(to)), static_cast<uint32_t>(std::get<1>(to)), std::get<2>(to).utf8()};
    }
    if (c.key) out.key = c.key->utf8();
    return out;
}

// The setup both kinds of options share.
template <class Options>
void setup_of(const Options& o, nw::Setup& s, std::optional<nw::RecordedCall>& held) {
    s.function = function_of(o.ordinal, o.seg, o.offset);
    if (o.args) for (const int64_t a : *o.args) s.args.push_back(static_cast<uint16_t>(a & 0xFFFF));
    if (o.prelude) {
        for (const PreludeCall& pc : *o.prelude) {
            nw::PreludeCall call;
            call.function = pc.ordinal ? nw::Function{nw::Ordinal{static_cast<uint32_t>(*pc.ordinal)}}
                                       : nw::Function{nw::CodePoint{static_cast<uint32_t>(pc.seg.value()), static_cast<uint32_t>(pc.offset.value())}};
            for (const int64_t a : pc.args) call.args.push_back(static_cast<uint16_t>(a & 0xFFFF));
            s.prelude.push_back(std::move(call));
        }
    }
    s.near_entry = o.near_;
    if (o.data_image) s.data_image = ::cb::compat::from_py(*o.data_image);
    if (o.callees) {
        for (const auto& [at, what] : o.callees->items()) {
            nw::Callee c;
            c.name = std::get<0>(what).utf8();
            if (std::get<1>(what) >= 0) c.removes = static_cast<uint32_t>(std::get<1>(what));
            c.far = std::get<2>(what);
            s.callees[{static_cast<uint32_t>(std::get<0>(at)), static_cast<uint32_t>(std::get<1>(at))}] = std::move(c);
        }
    }
    if (o.data_end) s.data_end = nw::DataEnd{u32(std::get<0>(*o.data_end)), u32(std::get<1>(*o.data_end))};
    if (o.context) {
        held = context_of(*o.context);
        s.context = &*held;
    }
    if (o.mem_init) {
        for (const auto& item : *o.mem_init) {
            nw::MemoryInit m;
            const Where& w = std::get<0>(item);
            if (const auto* linear = std::get_if<int64_t>(&w)) m.where = u32(*linear);
            else m.where = nw::DataOffset{u32(std::get<1>(std::get<std::tuple<py::str, int64_t>>(w)))};
            m.data = ::cb::compat::from_py(std::get<1>(item));
            s.mem_init.push_back(std::move(m));
        }
    }
    if (o.branches) s.branches = nw::OffsetRange{u32(std::get<0>(*o.branches)), u32(std::get<1>(*o.branches))};
    if (o.bp) s.bp = static_cast<uint16_t>(*o.bp & 0xFFFF);
    s.ports = o.ports;
    if (o.shadow != nullptr) s.shadow = o.shadow->new_shadow();   // the recorder steps it from here on
    s.fill_pages = std::getenv("CB_GATE_ZERO") == nullptr;
}

inline Regs regs_of(std::initializer_list<std::pair<const char*, int64_t>> items) {
    Regs r;
    for (const auto& [k, v] : items) r.set(py::str(k), v);
    return r;
}

inline Trace16 trace_of(const nw::Trace16& t) {
    Trace16 rec;
    rec.exit = ::cb::compat::to_py(t.exit_text());
    if (t.ending == nw::Ending::BadOrdinal) {   // nothing ran: the record has the way it ended and the ordinal only
        rec.ordinal = t.ordinal;
        return rec;
    }
    rec.module = ::cb::compat::to_py(t.module);
    rec.seg = t.segment;
    rec.offset = t.offset;
    rec.ordinal = t.ordinal;
    for (const uint16_t a : t.args) rec.input.args.append(a);
    rec.input.arity = static_cast<int64_t>(t.args.size());
    const auto& o = t.observed;
    rec.insn_count = o.insn_count;
    for (const uint64_t a : o.path) rec.path.append(a);
    for (const auto& r : o.mem_reads) rec.mem_reads.append(native_trace::MemRead{r.pc, r.addr, r.size, r.value, r.n});
    for (const auto& w : o.mem_writes) rec.mem_writes.append(native_trace::MemWrite{w.pc, w.addr, w.size, w.value, w.n, w.old});
    for (const auto& h : t.imports_hit) rec.imports_hit.append(hit_of(h));
    rec.truncated = o.truncated;
    rec.ret = t.ax;
    rec.ret32 = t.dx_ax;
    for (size_t i = 0; i < t.layout.segment_bases.size(); ++i) rec.layout.seg_bases.set(static_cast<int64_t>(i + 1), t.layout.segment_bases[i]);
    if (t.layout.data_base) rec.layout.ds_base = *t.layout.data_base;
    rec.layout.autodata_seg = t.layout.autodata_segment;
    for (const auto& c : t.calls_out) {
        CalleeCall cc;
        cc.name = ::cb::compat::to_py(c.name);
        for (const uint16_t a : c.args) cc.args.append(a);
        cc.sp = c.sp;
        cc.n = c.n;
        cc.ret_ip = c.ret_ip;
        rec.calls_out.append(cc);
    }
    rec.unsure = t.unsure;
    if (t.interrupt) rec.interrupt = *t.interrupt;
    if (o.returned) rec.returned = true;
    if (o.limit_fault) rec.limit_fault = true;
    if (t.branches) {
        py::list<native_trace::BranchEntry> log;
        for (const auto& e : *t.branches) {
            std::optional<native_trace::Compare> cmp;
            if (e.compare) {
                cmp = native_trace::Compare(py::str(std::string(::cb::emulate::native_trace::to_text(e.compare->op))), e.compare->size, e.compare->a,
                                            e.compare->b, e.compare->at_a, e.compare->at_b);
            }
            log.append(native_trace::BranchEntry(e.address, e.taken, cmp));
        }
        rec.branches = log;
    }
    if (t.prelude_ran) rec.prelude_ran = static_cast<int64_t>(t.prelude_ran);
    rec.entry_regs = regs_of({{"AX", t.entry_regs.ax}, {"BX", t.entry_regs.bx}, {"CX", t.entry_regs.cx}, {"DX", t.entry_regs.dx}, {"SI", t.entry_regs.si},
                              {"DI", t.entry_regs.di}, {"BP", t.entry_regs.bp}, {"CS", t.entry_regs.cs}, {"DS", t.entry_regs.ds}, {"ES", t.entry_regs.es},
                              {"SS", t.entry_regs.ss}});
    rec.exit_regs = regs_of({{"SI", t.exit_regs.si}, {"DI", t.exit_regs.di}, {"DS", t.exit_regs.ds}});
    rec.sp_delta = t.sp_delta;
    rec.code_base = t.code_base;
    rec.n_imports = static_cast<int64_t>(t.n_imports);
    if (t.fault) {
        Fault f;
        f.cs = t.fault->cs;
        f.eip = t.fault->eip;
        if (t.fault->insn) f.insn = ::cb::compat::to_py(*t.fault->insn);
        f.bytes = ::cb::compat::to_py(t.fault->bytes);
        rec.fault = f;
    }
    if (t.io) {
        py::list<ports16::LogEntry> io;
        for (const auto& e : *t.io) {
            const char* what = e.kind == ::cb::emulate::ports16::Access::In ? "in" : e.kind == ::cb::emulate::ports16::Access::Out ? "out" : "...";
            io.append(ports16::LogEntry(static_cast<int64_t>(e.step), py::str(what), e.port, e.value));
        }
        rec.io = io;
    }
    for (const auto& [sel, base] : t.heap_sels) rec.heap_sels.set(sel, base);
    py::list<native_trace::LazyMap> lazy;
    for (const auto& l : o.lazy_maps)
        lazy.append(native_trace::LazyMap{l.pc, l.addr, l.page, py::str(std::string(::cb::emulate::native_trace::to_text(l.access))), l.size});
    rec.lazy_maps = lazy;
    rec.memops = o.memops;
    rec.writes_seen = o.writes_seen;
    rec._stuck = o.stuck;
    return rec;
}

// Runs `f` with the old governor's count and ceiling on the Runner's, and brings the count back (also when `f` throws).
template <class F>
decltype(auto) governed(F&& f) {
    governor().n = emulator::_GOV.n;
    governor().ceil = emulator::_GOV.ceil;
    struct Back {
        ~Back() { emulator::_GOV.n = governor().n; }
    } back;
    return unicorn_uc::shim_detail::uc_guard(std::forward<F>(f));
}

}  // namespace shim

// The module's data segment as the loader leaves it -- its fix-ups applied (the selectors of far pointers kept in it) -- the bytes a rebuilt module
// is given in place of its own, so both sides start from the same globals.  -> bytes (empty when it has no data segment).
// used by: verify.gate16.isolated16
inline py::bytes relocated_data(const py::str& path) {
    return shim::governed([&] { return ::cb::compat::to_py(::cb::ByteView(shim::runner().relocated_data(std::filesystem::path(path.utf8())))); });
}

// Model the Win16 APIs an LZ-class module calls, as the knowledge base's win16 api_model says. `uc` may be null.
inline StubResult default_stub(const Hit& hit, Uc*, HeapContext& ctx) {
    ::cb::emulate::ImportHit h;
    h.api = hit.api ? hit.api->utf8() : std::string();
    if (hit.args_stack) for (const int64_t w : *hit.args_stack) h.args_stack.push_back(w);
    struct Heap final : nw::HeapContext {
        ::codebreaker::emulate::windows16::HeapContext& old;
        explicit Heap(::codebreaker::emulate::windows16::HeapContext& o) : old(o) {}
        uint16_t alloc_heap(uint32_t size) override { return static_cast<uint16_t>(old.alloc_heap(size)); }
    } heap(ctx);
    return shim::stub_of(shim::runner().default_stub(h, heap));
}

// Trace one exported 16-bit function. Returns a TraceRecord like tracer.trace_function.  Exceptions a callback raised
// come out of here.
inline Trace16 trace_function16(const py::str& path, const Trace16Options& options = {}) {
    nw::TraceOptions t;
    std::optional<nw::RecordedCall> held;
    shim::setup_of(options, t, held);
    t.arity = static_cast<size_t>(options.arity);
    t.insn_cap = options.insn_cap;
    t.import_stub = shim::import_stub_of(options.import_stub);
    return shim::trace_of(shim::governed([&] { return shim::runner().trace(std::filesystem::path(path.utf8()), t); }));
}

// Run one NE function under the controlled Win16 environment. Returns the tracer16 TraceRecord (with layout, imports_hit,
// mem_*, ret, heaps, heap_sels).  used by: verify.gate16.isolated16, verify.lanes.win16, tests
inline Trace16 probe(const py::str& dll, const ProbeOptions& options = {}) {
    nw::ProbeOptions p;
    std::optional<nw::RecordedCall> held;
    shim::setup_of(options, p, held);
    p.arity = 0;
    if (options.insn_cap && *options.insn_cap) p.insn_cap = *options.insn_cap;
    p.cdecl_args = options.cdecl_;
    p.stub = shim::heap_stub_of(options.stub);
    if (options.stub_ret) p.stub_ret = shim::u32(*options.stub_ret);
    Trace16 rec = shim::trace_of(shim::governed([&] { return shim::runner().probe(std::filesystem::path(dll.utf8()), p); }));
    rec.heaps = std::make_tuple(HEAP_BASE, HEAP_STRIDE);
    return rec;
}

// probe's record as the one run record (emulate/run.hpp): AX and DX:AX the results, every Windows call made, the
// devices' log, SI DI DS as they went in and came out, the linear addresses run in order.  run.raw is the Trace16.
inline run::Run record(const Trace16& rec) {
    Regs entry = rec.entry_regs ? *rec.entry_regs : Regs();
    Regs out = rec.exit_regs ? *rec.exit_regs : Regs();
    py::dict<py::str, run::Register> results;
    results.set(py::str("AX"), rec.ret ? run::Register(*rec.ret) : run::Register(py::none_t{}));
    results.set(py::str("DX:AX"), rec.ret32 ? run::Register(*rec.ret32) : run::Register(py::none_t{}));
    py::list<run::Write> writes;
    for (const auto& w : rec.mem_writes)
        writes.append(run::Write{static_cast<int64_t>(w.addr), static_cast<int64_t>(w.size), static_cast<int64_t>(w.value),
                                 static_cast<int64_t>(w.old), false});
    py::list<run::Call> calls;
    for (const Hit& h : rec.imports_hit) {
        // (the whole hit is Trace16::imports_hit, through run.raw: what fits the detail is the 16-bit tracer's note)
        py::dict<py::str, run::Word> note;
        if (h.api) note.set(py::str("api"), run::Word(*h.api));
        if (h.n) note.set(py::str("n"), run::Word(*h.n));
        calls.append(run::Call{h.api, h.args_stack, run::Detail(note)});
    }
    py::list<run::IO> io;
    if (rec.io) {
        for (const auto& e : *rec.io) io.append(run::IO{std::get<0>(e), std::get<1>(e), std::get<2>(e), std::get<3>(e), std::nullopt});
    }
    run::Preserved preserved;
    for (const auto& kv : out.items())
        preserved.set(std::get<0>(kv), std::make_tuple(entry.get(std::get<0>(kv)), std::optional<int64_t>(std::get<1>(kv))));
    py::list<int64_t> path;
    for (uint64_t a : rec.path) path.append(static_cast<int64_t>(a));
    bool bad = rec.exit && *rec.exit == py::str("bad-ordinal");
    return run::Run(rec.exit, results, writes, false, calls, io, rec.sp_delta, preserved, path,
                    bad ? std::nullopt : std::optional<int64_t>(rec.insn_count), rec.truncated, py::str("little"),
                    std::make_shared<Trace16>(rec));
}

// The platform's runner (platforms/win16.py emulator.run): probe(dll, **kw) as a Run.
inline run::Run recorded(const py::str& dll, const ProbeOptions& options = {}) { return record(probe(dll, options)); }

}  // namespace codebreaker::emulate::windows16
