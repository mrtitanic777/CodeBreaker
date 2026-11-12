// codebreaker/emulate/emulator.hpp -- SHIM over cb/emulate/emulator.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a trace as py:: records, trace
// options as a bag of optionals, the process-wide governor and stubbing default, the module-level functions) for the ports that still use it
// (analyze/observe, build/module, emulate/driver, emulate/windows16, verify/lanes/*). Conversion and forwarding only; deleted with its last py::
// consumer.
//
// One process-wide cb::emulate::emulator::Tracer does the work. It resolves modules through the old corpus, reads the headers through the old
// prototypes, and makes its engines from the Unicorn the recorder was pointed at (the host's copy: engines and recorder must agree), as the old
// port did. What the old module kept in module state and nobody outside used (_load, _LOADED, _RESOLVED, _MACHINES, _outs) is not here.
//
// Where the old code's quirks were bugs they are fixed in the new code, not mimicked here (an overlay's imports are now merged: see
// native/cb/emulate/emulator_test.cpp).
#pragma once

#include <array>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/emulator.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/run.hpp"
#include "codebreaker/emulate/unicorn_uc.hpp"
#include "codebreaker/knowledge/package.hpp"
#include "codebreaker/symbols/prototypes.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::emulate::emulator {

using core16::Uc;
namespace ne = ::cb::emulate::emulator;

// Entry-state constants, identical to difftest._emulate so traces are comparable to the oracle difftest trusts.
inline constexpr int64_t _SCRATCH_BASE = ne::kScratchBase, _SCRATCH_SIZE = ne::kScratchSize;
inline constexpr int64_t _STACK_BASE = ne::kStackBase, _STACK_SIZE = ne::kStackSize;
// EBX, ESI, EDI, EBP at entry
inline constexpr std::array<int64_t, 4> SAVED_SENTINELS = {ne::kSavedSentinels[0], ne::kSavedSentinels[1], ne::kSavedSentinels[2], ne::kSavedSentinels[3]};
inline constexpr int64_t _STACK_TOP = ne::kStackTop, _RET = ne::kReturnAddress;
inline constexpr int64_t _PAGE = ne::kPage;

// Recording caps -- a trace is evidence, not a full replay log, so it is bounded and records that it was.
inline constexpr int64_t _MAX_INSNS = ne::kMaxInsns;
inline constexpr int64_t _MAX_MEM = ne::kMaxMem;

// Per-FUNCTION instruction governor ({n, ceil}); recon.reconstruct_one resets it; the hook stops emulation once it is exceeded. (Copied to the
// Tracer's before each run and back after it.)
inline native_trace::Gov _GOV;

// IAT sentinels: a unique, recognizable landing address per import slot.
inline constexpr int64_t _SENT_BASE = ne::kSentinelBase, _SENT_STRIDE = ne::kSentinelStride;

// Process-wide default for external-call stubbing, set once so EVERY trace inherits it; an explicit argument to trace_function always overrides it.
struct DefaultStub {
    bool enabled = false;
    native_trace::SigResolver resolver;   // (empty: none)
};
inline DefaultStub _DEFAULT_STUB;

// A func selector: None (first function), a section offset or VA, or a string ("0x...", decimal).
using FuncSelector = std::variant<py::none_t, int64_t, py::str>;
// _resolve_func(module, func): (off, va0, code, end, secname)
struct ResolvedFunc {
    int64_t off = 0, va0 = 0;
    py::bytes code;
    int64_t end = 0;
    py::str secname;
};

// --- the branch log (trace_function(branches=True)) ----------------------------------------------------------------------------
inline constexpr int64_t _BRANCH_CAP = 4096;   // branch events kept per run
inline const py::list<py::str> _COMPARES = py::list<py::str>{py::str("cmp"), py::str("test"), py::str("sub"), py::str("and"), py::str("or"),
                                                             py::str("dec"), py::str("inc"), py::str("add"), py::str("xor")};
inline const py::list<py::str> _FLAGLESS = py::list<py::str>{py::str("mov"), py::str("movzx"), py::str("movsx"), py::str("lea"),   py::str("push"),
                                                             py::str("pop"), py::str("nop"),   py::str("xchg"),  py::str("not"),   py::str("cdq"),
                                                             py::str("cwde"), py::str("cbw"),  py::str("bswap")};
// capstone register name -> the 32-bit register it is part of
inline const py::dict<py::str, py::str> _PARENT = [] {
    py::dict<py::str, py::str> parent;
    for (char r : std::string("abcd")) {
        const std::string full = std::string("e") + r + "x";
        for (const std::string& n : {std::string(1, r) + "l", std::string(1, r) + "h", std::string(1, r) + "x", full}) parent.set(py::str(n), py::str(full));
    }
    for (const char* r : {"si", "di", "bp", "sp"}) {
        parent.set(py::str(r), py::str(std::string("e") + r));
        parent.set(py::str(std::string("e") + r), py::str(std::string("e") + r));
    }
    return parent;
}();

// Unicorn's id of an x86 register by (any-case) name, None when there is none.
inline std::optional<int64_t> _uc_reg(const py::str& name) { return unicorn_uc::x86_reg(name.upper()); }

// --- the record ---------------------------------------------------------------------------------------------------------------------
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

// trace_function's keyword arguments (see cb/emulate/emulator.hpp's TraceOptions for what each is).
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

namespace shim {

// The one Tracer of the process, over the old corpus and prototypes, making engines from the recorder's own Unicorn.
inline ne::Tracer& tracer() {
    static ne::Tracer instance([] {
        ne::Environment env;
        env.resolve = [](std::string_view name) -> std::filesystem::path {
            return std::filesystem::path(base::corpus::resolve(::cb::compat::to_py(name)).utf8());   // (ValueError for a name the corpus does not have)
        };
        env.prototype = [](std::string_view name) -> std::optional<::cb::symbols::prototypes::Prototype> {
            try {
                const std::optional<symbols::prototypes::Prototype> old = symbols::prototypes::of(::cb::compat::to_py(name), 32);
                if (!old) return std::nullopt;
                ::cb::symbols::prototypes::Prototype p;
                for (const auto& param : old->parameters) p.parameters.push_back({std::get<0>(param).utf8(), std::get<1>(param).utf8()});
                return p;
            } catch (const py::Exception&) {   // (no headers here: nothing is given back)
                return std::nullopt;
            }
        };
        env.open_engine = []() -> std::unique_ptr<::cb::core16::Uc> {
            native_trace::require();   // points the recorder at the host's libraries, or raises saying why not
            return ::cb::compat::guard([] { return ::cb::emulate::unicorn_uc::open_x86_from(unicorn_uc::shim_detail::dll(), 32); });
        };
        return env;
    }());
    return instance;
}

inline ne::FuncSelector selector_of(const FuncSelector& f) {
    if (std::holds_alternative<py::none_t>(f)) return ne::FuncSelector{};
    if (const auto* number = std::get_if<int64_t>(&f)) return ne::FuncSelector{static_cast<uint64_t>(*number)};
    return ne::FuncSelector{std::get<py::str>(f).utf8()};
}

inline ne::TraceOptions options_of(const TraceOptions& o) {
    ne::TraceOptions n;
    if (o.args) {
        std::vector<uint32_t> args;
        for (const int64_t a : *o.args) args.push_back(static_cast<uint32_t>(a & 0xFFFFFFFF));
        n.args = std::move(args);
    }
    if (o.arity) n.arity = static_cast<int>(*o.arity);
    n.insn_cap = o.insn_cap;
    if (o.mem_init) {
        for (const auto& item : *o.mem_init) n.mem_init.push_back({static_cast<uint32_t>(std::get<0>(item)), ::cb::compat::from_py(std::get<1>(item))});
    }
    n.stub_externals = o.stub_externals ? *o.stub_externals : _DEFAULT_STUB.enabled;   // inherit the process default
    n.stub_ret = o.stub_ret;
    n.stub_cleanup = o.stub_cleanup;
    const native_trace::SigResolver& sig = o.sig_resolver ? *o.sig_resolver : _DEFAULT_STUB.resolver;
    if (sig) n.argc = [sig](const ::cb::emulate::native_trace::ImportName& name) { return sig(py::str(name.dll), py::str(name.name)); };
    n.stub_ret_unique = o.stub_ret_unique;
    if (o.overlay) n.overlay = o.overlay->utf8();
    if (o.sentinels) {
        std::array<uint32_t, 4> s{};
        for (size_t k = 0; k < 4; ++k) s[k] = static_cast<uint32_t>((*o.sentinels)[k] & 0xFFFFFFFF);
        n.sentinels = s;
    }
    n.reads = o.reads;
    n.stop_wild = o.stop_wild;
    n.quiet_stack = o.quiet_stack;
    n.reuse = o.reuse;
    n.fast = o.fast;
    n.lean = o.lean;
    if (o.ecx) n.ecx = static_cast<uint32_t>(*o.ecx & 0xFFFFFFFF);
    if (o.edx) n.edx = static_cast<uint32_t>(*o.edx & 0xFFFFFFFF);
    if (o.pads) n.pads = ::cb::emulate::native_trace::AddressRange{static_cast<uint64_t>(std::get<0>(*o.pads)), static_cast<uint64_t>(std::get<1>(*o.pads))};
    n.branches = o.branches;
    if (o.stub_at) {
        for (const auto& [address, import] : o.stub_at->items()) n.stub_at.push_back({static_cast<uint64_t>(address), {std::get<0>(import).utf8(), std::get<1>(import).utf8()}});
    }
    return n;
}

// The old record from the new one: what trace_function's Python left in its dict.
inline Trace trace_of(const ne::Trace& t, bool lean, bool branches) {
    Trace rec;
    rec.module = ::cb::compat::to_py(t.module);
    rec.module_path = ::cb::compat::to_py(t.module_path.string());
    rec.target_sha256 = ::cb::compat::to_py(t.target_sha256);
    rec.section = ::cb::compat::to_py(t.section);
    rec.func_off = t.func_off;
    rec.func_va = t.func_va;
    rec.func_end_va = t.func_end_va;
    for (const uint32_t a : t.args) rec.input.args.append(a);
    rec.input.arity = static_cast<int64_t>(t.args.size());
    rec.entry_esp = t.entry_esp;
    native_trace::TraceRecord tr;
    native_trace::Run::convert(t.record, tr, lean, branches);
    if (tr.insn_count) rec.insn_count = *tr.insn_count;
    if (tr.truncated) rec.truncated = *tr.truncated;
    if (tr.path) rec.path = *tr.path;
    if (tr.mnemonics) {   // mnemonics keyed by int va -> stringified for JSON friendliness
        for (const auto& kv : tr.mnemonics->items()) rec.mnemonics.set(py::percent(py::str("0x%x"), static_cast<int64_t>(std::get<0>(kv))), std::get<1>(kv));
    }
    if (tr.calls) rec.calls = *tr.calls;
    if (tr.mem_reads) rec.mem_reads = *tr.mem_reads;
    if (tr.mem_writes) rec.mem_writes = *tr.mem_writes;
    if (tr.lazy_maps) rec.lazy_maps = *tr.lazy_maps;
    if (tr.pages0) rec.pages0 = *tr.pages0;
    rec.governed = tr.governed;
    rec.memops = tr.memops;
    rec.writes_seen = tr.writes_seen;
    rec._dirty = tr._dirty;
    rec._stuck = tr._stuck;
    rec.bogus_stubs = tr.bogus_stubs;
    rec.ran_into_zeros = tr.ran_into_zeros;
    rec.api_deps = tr.api_deps;
    rec.api_calls = tr.api_calls;
    rec.stubbed_externals = tr.stubbed_externals;
    rec.vxd_services = tr.vxd_services;
    rec.branches = tr.branches;
    rec.exit = ::cb::compat::to_py(t.exit_text());
    if (t.ret) rec.ret = *t.ret;
    if (t.saved_regs) rec.saved_regs = std::array<int64_t, 4>{(*t.saved_regs)[0], (*t.saved_regs)[1], (*t.saved_regs)[2], (*t.saved_regs)[3]};
    if (t.esp_delta) rec.esp_delta = *t.esp_delta;
    if (t.st0) rec.st0 = *t.st0;
    return rec;
}

// The old one-run record from the new one (the Trace stays the new code's own: `raw` is the old Trace).
inline run::Run run_of(const ::cb::emulate::Run& r, std::shared_ptr<void> raw) {
    py::dict<py::str, run::Register> results;
    for (const auto& [name, v] : r.results) {
        if (const auto* i = std::get_if<uint64_t>(&v)) results.set(py::str(name), run::Register(static_cast<int64_t>(*i)));
        else if (const auto* d = std::get_if<double>(&v)) results.set(py::str(name), run::Register(*d));
        else results.set(py::str(name), run::Register(py::none_t{}));
    }
    py::list<run::Write> writes;
    for (const auto& w : r.writes) {
        run::Write o;
        o.address = static_cast<int64_t>(w.address);
        o.size = w.size;
        o.value = static_cast<int64_t>(w.value);
        if (w.old) o.old = static_cast<int64_t>(*w.old);
        o.harness = w.harness;
        writes.append(o);
    }
    py::list<run::Call> calls;
    for (const auto& c : r.calls) {
        run::Call o;
        if (c.name) o.name = py::str(*c.name);
        if (c.args) {
            py::list<int64_t> args;
            for (const int64_t a : *c.args) args.append(a);
            o.args = args;
        }
        o.detail = run::Detail(py::none_t{});
        calls.append(o);
    }
    std::optional<run::Preserved> preserved;
    if (r.preserved) {
        run::Preserved p;
        auto signed_of = [](const std::optional<uint64_t>& v) { return v ? std::optional<int64_t>(static_cast<int64_t>(*v)) : std::nullopt; };
        for (const auto& [name, saved] : *r.preserved) p.set(py::str(name), std::make_tuple(signed_of(saved.on_entry), signed_of(saved.at_end)));
        preserved = p;
    }
    py::list<int64_t> path;
    for (const uint64_t a : r.path) path.append(static_cast<int64_t>(a));
    return run::Run(r.exit ? std::optional<py::str>(py::str(*r.exit)) : std::nullopt, results, writes, r.capped, calls, py::list<run::IO>(),
                    r.stack, preserved, path, r.steps ? std::optional<int64_t>(static_cast<int64_t>(*r.steps)) : std::nullopt, r.truncated,
                    py::str("little"), std::move(raw));
}

}  // namespace shim

// Map a user-supplied func selector to the function's offset, base, code, end and section. Raises ValueError for a module with no functions or no
// function at the address.  used by: verify.lanes.win32
inline ResolvedFunc _resolve_func(const py::str& module, const FuncSelector& func) {
    return ::cb::compat::guard([&] {
        const ne::ResolvedFunction f = shim::tracer().resolve(module.utf8(), shim::selector_of(func));
        ResolvedFunc out;
        out.off = f.off;
        out.va0 = f.module->va0;
        out.code = ::cb::compat::to_py(::cb::ByteView(f.module->code));
        out.end = f.end;
        out.secname = ::cb::compat::to_py(f.module->section);
        return out;
    });
}

// Run one function under Unicorn with full hooks; return a TraceRecord.  Raises ValueError when the module has no function there; an exception a
// native hook raised comes out of here.  used by: analyze.observe, tests
inline Trace trace_function(const py::str& module, const FuncSelector& func = py::none_t{}, const TraceOptions& options = {}) {
    ne::Tracer& tracer = shim::tracer();
    tracer.governor().n = _GOV.n;
    tracer.governor().ceil = _GOV.ceil;
    ne::TraceOptions n = shim::options_of(options);
    // a Shadow32 kept up beside the run is the recorder's: the Tracer is handed the new Shadow the old one wraps
    if (options.shadow != nullptr) n.shadow = options.shadow->new_shadow();
    ne::Trace t;
    try {
        t = ::cb::compat::guard([&] { return tracer.trace(module.utf8(), shim::selector_of(func), n); });
    } catch (...) {
        _GOV.n = tracer.governor().n;
        throw;
    }
    _GOV.n = tracer.governor().n;
    return shim::trace_of(t, options.lean, options.branches);
}

// The platform's runner (platforms/win32.py emulator.run): trace_function(module, **kw) as a Run -- EAX (and x87 ST0) the results, the imports and
// callbacks entered with their arguments, EBX ESI EDI EBP against the sentinels they were given (`sentinels`, else SAVED_SENTINELS), every
// instruction run (first seen first). run.raw holds the old Trace (std::shared_ptr<Trace>).  used by: verify.lanes.win32, build.module
inline run::Run recorded(const py::str& module, const FuncSelector& func = py::none_t{}, const TraceOptions& options = {}) {
    ne::Tracer& tracer = shim::tracer();
    tracer.governor().n = _GOV.n;
    tracer.governor().ceil = _GOV.ceil;
    ne::TraceOptions n = shim::options_of(options);
    if (options.shadow != nullptr) n.shadow = options.shadow->new_shadow();
    std::shared_ptr<const ne::Trace> t;
    try {
        t = std::make_shared<const ne::Trace>(::cb::compat::guard([&] { return tracer.trace(module.utf8(), shim::selector_of(func), n); }));
    } catch (...) {
        _GOV.n = tracer.governor().n;
        throw;
    }
    _GOV.n = tracer.governor().n;
    std::optional<std::array<uint32_t, 4>> given = n.sentinels;
    return shim::run_of(ne::record(t, given), std::make_shared<Trace>(shim::trace_of(*t, options.lean, options.branches)));
}

// Argument count from the knowledge base's win32 api_model (documented counts for common APIs, A/W variants sharing one), or None. (The old world
// has no Knowledge object to hand over: the lookup stays over its process-wide model until verify/lanes/win32 is refactored.)
// used by: verify.lanes.win32
inline std::optional<int64_t> _builtin_argc(const std::optional<py::str>& name) {
    py::str n = name ? *name : py::str("");
    auto model = knowledge::api_model(py::str("win32"), n);
    if (!model && !n.empty() && (n.slice_from(-1) == py::str("A") || n.slice_from(-1) == py::str("W"))) model = knowledge::api_model(py::str("win32"), n.slice(0, n.size() - 1));
    if (!model) return std::nullopt;
    auto argc = model->get(py::str("argc"));
    if (!argc || argc->kind != knowledge::sources::Json::Int) return std::nullopt;
    return argc->i;
}

}  // namespace codebreaker::emulate::emulator
