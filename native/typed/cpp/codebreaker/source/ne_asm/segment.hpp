// codebreaker/source/ne_asm/segment.hpp -- SHIM over cb/source/ne_asm/segment.hpp (docs/CPP_DESIGN.md 6.3): the call-graph-tracing disassembler
// of the code segments of an NE module. The old types in (NEFile, py::dict, py::list), the new class and functions called; the old public
// data members of `Seg` are filled from the new Seg when it is made. Deleted with its last py:: consumer.
//
// Differences from the old port that the shim cannot hide:
//   * The reports (`print_function`, `cmd_*`) go to CodeBreaker's stdout (cb::out), not to the old layer's.
//   * `_ord2name`, `_export_names`, `_find_starts` and `_carve` (the steps of the constructor) are not offered: nothing used them.
//   * `fixups`, `export_names`, `funcs` are in offset order; `ordinals` and `rows` are as the new code reads them.
//   * The prologue scan the Python described and never ran stays off (see cb/source/ne_asm/segment.cpp, kScanPushBpPrologue).
//   * `Seg` keeps its NEFile by reference, as the old class did: the NEFile must outlive it.
#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/source/ne_asm/segment.hpp"
#include "codebreaker/formats/ne.hpp"
#include "py.hpp"

namespace codebreaker::source::ne_asm::segment {

namespace cbs = cb::source::ne_asm::segment;

namespace shim_ {

// {number: name} of a name table the new code answers by lookup, over the numbers 0..`last`.
template <class Lookup>
py::dict<int64_t, py::str> table_of(Lookup lookup, uint32_t last) {
    py::dict<int64_t, py::str> out;
    for (uint32_t n = 0; n <= last; ++n) {
        if (const char* name = lookup(n)) out.set(n, py::str(name));
    }
    return out;
}

inline cbs::SharedOrdinals ordinals_of(const py::dict<py::str, py::str>& d) {
    auto out = std::make_shared<cbs::Ordinals>();
    for (const auto& kv : d.items()) (*out)[cb::compat::from_py(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
    return out;
}

}  // namespace shim_

// Window message names (mirrors wndproc.py's table).
inline const py::dict<int64_t, py::str> WM = shim_::table_of(cbs::window_message_name, 0x400);
// DOS INT 21h function names, for annotating the KERNEL wrappers' actual syscalls.
inline const py::dict<int64_t, py::str> DOS21 = shim_::table_of(cbs::dos_function_name, 0xFF);

// Parse ORDINALS.md tables into {'USER.39': 'BeginPaint', ...}.
inline py::dict<py::str, py::str> load_ordinals(const py::str& path) {
    return cb::compat::guard([&] {
        py::dict<py::str, py::str> out;
        for (const auto& [name, value] : cbs::load_ordinals(cb::fs::from_text(cb::compat::from_py(path)))) out.set(cb::compat::to_py(name), cb::compat::to_py(value));
        return out;
    });
}

// objdump the whole blob as 16-bit code; return [(offset, asm_text), ...].
inline py::list<std::tuple<int64_t, py::str>> disasm_whole(const py::bytes& code, int64_t base = 0) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, py::str>> rows;
        for (const cbs::Row& r : cbs::disassemble(cb::compat::from_py(code), cb::compat::int_from_py<uint32_t>(base))) rows.append(std::make_tuple(int64_t{r.offset}, cb::compat::to_py(r.text)));
        return rows;
    });
}

// a function the carve found: [start, end) and its name
struct Func {
    int64_t start = 0, end = 0;
    py::str name;
};

// what a call/jmp resolves to: Sys (label, name or None: an imported routine), Int (target: a function of this segment),
// Far (label: another segment of the module), NotACall (not a call to anything known)
enum class CallKind { NotACall, Sys, Int, Far };
struct Resolved {
    CallKind kind = CallKind::NotACall;
    py::str label;                    // Sys, Far
    std::optional<py::str> name;      // Sys: the ordinal's name when known
    int64_t target = 0;               // Int
};

namespace shim_ {
inline Func func_of(const cbs::Func& f) { return Func{f.start, f.end, cb::compat::to_py(f.name)}; }
inline CallKind kind_of(cbs::CallKind k) {
    switch (k) {
        case cbs::CallKind::System: return CallKind::Sys;
        case cbs::CallKind::Internal: return CallKind::Int;
        case cbs::CallKind::Far: return CallKind::Far;
    }
    return CallKind::NotACall;
}
}  // namespace shim_

// One CODE segment: disassembled, with functions discovered and calls resolved.
class Seg {
  public:
    const formats::ne::NEFile& ne;
    int64_t si;
    formats::ne::Segment seg;
    py::dict<py::str, py::str> ordinals;
    py::dict<int64_t, py::str> name_override;
    py::list<py::str> mods;
    py::dict<int64_t, py::str> fixups;   // site_off -> "MODULE.ord"/"segN:0xNNNN"
    py::bytes code;
    py::list<std::tuple<int64_t, py::str>> rows;
    py::dict<int64_t, int64_t> off2idx;
    py::set<int64_t> valid;
    py::dict<int64_t, py::str> export_names;
    py::set<int64_t> starts;
    py::dict<int64_t, Func> funcs;       // start_off -> Func

    Seg(const formats::ne::NEFile& ne_, int64_t seg_index, const py::dict<py::str, py::str>& ordinals_,
        const std::optional<py::dict<int64_t, py::str>>& name_override_ = std::nullopt)
        : ne(ne_), si(seg_index), ordinals(ordinals_), name_override(name_override_ ? name_override_->copy() : py::dict<int64_t, py::str>()) {
        cb::compat::guard([&] {
            cbs::NameOverrides overrides;
            for (const auto& kv : name_override.items()) overrides[cb::compat::int_from_py<uint32_t>(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
            inner_ = std::make_shared<const cbs::Seg>(ne.module(), cb::compat::int_from_py<std::size_t>(seg_index), shim_::ordinals_of(ordinals), std::move(overrides));
            const cbs::Seg& s = *inner_;
            const auto& info = s.info();
            seg = formats::ne::Segment{info.index, static_cast<int64_t>(info.file_offset), info.length, info.flags, info.min_alloc, py::str(info.is_data() ? "DATA" : "CODE")};
            mods = cb::compat::to_py(s.modules());
            for (const auto& [offset, target] : s.fixups()) fixups.set(offset, cb::compat::to_py(target.label));
            code = cb::compat::to_py(s.code());
            for (std::size_t i = 0; i < s.rows().size(); ++i) {
                rows.append(std::make_tuple(int64_t{s.rows()[i].offset}, cb::compat::to_py(s.rows()[i].text)));
                off2idx.set(s.rows()[i].offset, static_cast<int64_t>(i));   // an offset met twice keeps the later row
            }
            for (const auto& kv : off2idx.items()) valid.add(std::get<0>(kv));
            export_names = cb::compat::to_py(s.export_names());
            starts = cb::compat::to_py(s.starts());
            for (const auto& [start, f] : s.functions()) funcs.set(start, shim_::func_of(f));
        });
    }

    py::str name_at(int64_t off) const { return cb::compat::to_py(inner_->name_at(cb::compat::int_from_py<uint32_t>(off))); }

    // Return what the call/jmp at `off` resolves to.
    Resolved resolve_call(int64_t off, const py::str& asm_) const {
        Resolved out;
        const auto call = inner_->resolve_call(cb::compat::int_from_py<uint32_t>(off), cb::compat::from_py(asm_));
        if (!call) return out;
        out.kind = shim_::kind_of(call->kind);
        out.label = cb::compat::to_py(call->label);
        if (call->name) out.name = cb::compat::to_py(*call->name);
        out.target = call->target;
        return out;
    }
    std::tuple<Func, py::list<std::tuple<int64_t, py::str>>> func_rows(int64_t start) const {
        const cbs::FunctionRows f = inner_->function_rows(cb::compat::int_from_py<uint32_t>(start));
        py::list<std::tuple<int64_t, py::str>> out;
        for (const cbs::Row& r : f.rows) out.append(std::make_tuple(int64_t{r.offset}, cb::compat::to_py(r.text)));
        return std::make_tuple(shim_::func_of(f.func), out);
    }
    // Distinct resolved calls made by a function, in order.
    py::list<std::tuple<CallKind, py::str>> calls_in(int64_t start) const {
        py::list<std::tuple<CallKind, py::str>> out;
        for (const cbs::Call& c : inner_->calls_in(cb::compat::int_from_py<uint32_t>(start))) out.append(std::make_tuple(shim_::kind_of(c.kind), cb::compat::to_py(cbs::describe_call(*inner_, c))));
        return out;
    }
    // Positive [bp+N] frame slots a function reads = its incoming PASCAL args.
    py::list<int64_t> args_used(int64_t start) const { return cb::compat::to_py(inner_->args_used(cb::compat::int_from_py<uint32_t>(start))); }
    // Read a little-endian WORD from the segment image, or None if OOB.
    std::optional<int64_t> word_at(int64_t off) const {
        if (off < 0 || off > UINT32_MAX) return std::nullopt;
        const auto w = inner_->word_at(static_cast<uint32_t>(off));
        return w ? std::optional<int64_t>(*w) : std::nullopt;
    }
    // If `start` is a WndProc, return [(msg, handler_off), ...]; else [].
    py::list<std::tuple<int64_t, int64_t>> dispatch(int64_t start) const {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const cbs::Dispatch& d : inner_->dispatch(cb::compat::int_from_py<uint32_t>(start))) out.append(std::make_tuple(int64_t{d.message}, int64_t{d.handler}));
        return out;
    }
    // Resolved calls a single dispatch handler makes.
    py::list<py::str> handler_calls(int64_t start, int64_t handler_off, const py::list<int64_t>& handler_starts) const {
        std::vector<uint32_t> starts_in;
        for (const int64_t s : handler_starts) starts_in.push_back(cb::compat::int_from_py<uint32_t>(s));
        return cb::compat::to_py(inner_->handler_calls(cb::compat::int_from_py<uint32_t>(start), cb::compat::int_from_py<uint32_t>(handler_off), starts_in));
    }
    // Detect the `mov bx,IMM; push bx; jmp TARGET` MOVEABLE-entry thunk; return TARGET (the shared dispatcher) or None.
    std::optional<int64_t> is_thunk(int64_t start) const {
        const auto t = inner_->is_thunk(cb::compat::int_from_py<uint32_t>(start));
        return t ? std::optional<int64_t>(*t) : std::nullopt;
    }

    const cbs::Seg& inner() const { return *inner_; }

  private:
    std::shared_ptr<const cbs::Seg> inner_;
};

// (segment, offset) of the entry with this ordinal, or None
inline std::optional<std::tuple<int64_t, int64_t>> find_seg_of_entry(const formats::ne::NEFile& ne, int64_t ordinal) {
    return cb::compat::guard([&]() -> std::optional<std::tuple<int64_t, int64_t>> {
        const auto found = cbs::find_entry(ne.module(), cb::compat::int_from_py<uint32_t>(ordinal));
        return found ? std::optional<std::tuple<int64_t, int64_t>>(std::make_tuple(int64_t{found->first}, int64_t{found->second})) : std::nullopt;
    });
}

// ---- annotated printing
inline py::str annotate_row(const Seg& sg, int64_t off, const py::str& asm_) {
    return cb::compat::to_py(cbs::annotate_row(sg.inner(), cbs::Row{cb::compat::int_from_py<uint32_t>(off), cb::compat::from_py(asm_)}));
}
inline void print_function(const Seg& sg, int64_t start, bool body = true) {
    cb::out_text(cbs::function_report(sg.inner(), cb::compat::int_from_py<uint32_t>(start), body));
}
// BFS over internal calls from `start`; return reachable func starts in discovery order.
inline py::list<int64_t> trace_callgraph(const Seg& sg, int64_t start) {
    return cb::compat::to_py(cbs::call_graph(sg.inner(), cb::compat::int_from_py<uint32_t>(start)));
}

// ---- commands
inline void cmd_map(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals) {
    cb::compat::guard([&] { cb::out_text(cbs::map_report(ne.module(), shim_::ordinals_of(ordinals))); });
}
inline void cmd_list(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index) {
    cb::compat::guard([&] { cb::out_text(cbs::list_report(ne.module(), shim_::ordinals_of(ordinals), cb::compat::int_from_py<std::size_t>(seg_index))); });
}
inline void cmd_fn(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t addr) {
    cb::compat::guard([&] {
        cb::out_text(cbs::function_listing(ne.module(), shim_::ordinals_of(ordinals), cb::compat::int_from_py<std::size_t>(seg_index), cb::compat::int_from_py<uint32_t>(addr)));
    });
}
// Emit a ready-to-refine C skeleton for a window/dialog procedure.
inline void cmd_cskel(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start) {
    cb::compat::guard([&] {
        cb::out_text(cbs::skeleton(ne.module(), shim_::ordinals_of(ordinals), cb::compat::int_from_py<std::size_t>(seg_index), cb::compat::int_from_py<uint32_t>(start)));
    });
}
// Scan code segments for real window procedures.
inline void cmd_wndprocs(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, std::optional<int64_t> only_seg = std::nullopt) {
    cb::compat::guard([&] {
        std::optional<std::size_t> only;
        if (only_seg) only = cb::compat::int_from_py<std::size_t>(*only_seg);
        cb::out_text(cbs::wndprocs_report(ne.module(), shim_::ordinals_of(ordinals), only));
    });
}
inline void cmd_trace(const formats::ne::NEFile& ne, const py::dict<py::str, py::str>& ordinals, int64_t seg_index, int64_t start) {
    cb::compat::guard([&] {
        cb::out_text(cbs::trace_report(ne.module(), shim_::ordinals_of(ordinals), cb::compat::int_from_py<std::size_t>(seg_index), cb::compat::int_from_py<uint32_t>(start)));
    });
}

// the script's command line: argv is sys.argv[1:]
inline int64_t main(const py::list<py::str>& argv) {
    return cb::compat::guard([&]() -> int64_t { return cbs::main(cb::compat::from_py(argv)); });
}

}  // namespace codebreaker::source::ne_asm::segment
