// codebreaker/build/asm16.hpp -- COMPAT shim for cb/build/asm16.hpp (docs/CPP_DESIGN.md 6.3): the old Function record and free
// functions in, the new Function called, the old records out; header-only, no logic. Deleted with its last old-layer consumer
// (build/module, port/asmdata, verify/gate16).
//
//     asm16::Function f(nef, seg, start, end, public, symbol_for, cs_ok, extra_entries, whole, holes);
//     f.text, f.imports, f.why          (f.text is None when it cannot be written so; f.why says why)
//     asm16::check(obj_bytes, f)        -> "" or what differs
//
//   * The old Function's public data (text, imports, why, externs, missing, tables, far_missing, reaches, holes, raw_hole_calls,
//     raw_far_entries, publics, cs_labels, bits, start, end, blob, code_segments, sites, ins, symbolic, calls, labels, module, seg,
//     public_) are copies made when the Function is made and again after the two calls that go on working (write and
//     raw_calls_into_holes); `assembled` is the slot build.module sets and is the shim's own.
//   * set_capstone_dll is accepted and ignored: Capstone is loaded by cb::disasm::capstone, beside the executable.
//   * The decoder engines are the calling thread's (a thread_local DecoderSet).
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "cb/build/asm16.hpp"
#include "cb/compat/py_bridge.hpp"
#include "cb/platforms/decoder.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::asm16 {

namespace na = ::cb::build::asm16;

// the path of build/asm16.py (the Python's __file__; build/module hashes its text into its cache stamp) -- used by: build.module
inline py::str module_file() {
    auto root = py::os::getenv(py::str("CB_CHECKOUT_ROOT"));
    return py::os::path::join(root ? *root : py::str("."), py::str("codebreaker"), py::str("build"), py::str("asm16.py"));
}

inline void set_capstone_dll(const py::str&) {}

// an import is named by its ordinal (int) or its name (str)
using Which = std::variant<int64_t, py::str>;

// One call of symbol_for: kind is "function" (seg, off), "segment" (seg), "data" (seg: a data segment's first byte) or "import"
// (module, which). Unused fields stay at their defaults.
struct Reference {
    py::str kind;
    int64_t seg = 0;
    int64_t off = 0;
    py::str module;
    Which which = int64_t(0);
};
using SymbolFor = std::function<std::optional<py::str>(const Reference&)>;

// `public`: the symbol to define, or [(offset, symbol)] for each function of a run
using Publics = std::variant<py::str, py::list<std::tuple<int64_t, py::str>>>;

namespace shim {

inline Which old_which(const na::Which& w) {
    if (const auto* n = std::get_if<std::uint16_t>(&w)) return Which(static_cast<int64_t>(*n));
    return Which(cb::compat::to_py(std::get<std::string>(w)));
}
inline na::Which new_which(const Which& w) {
    if (const auto* n = std::get_if<int64_t>(&w)) return na::Which(static_cast<std::uint16_t>(*n));
    return na::Which(std::get<py::str>(w).utf8());
}
inline Reference old_reference(const na::Reference& r) {
    Reference out;
    if (const auto* f = std::get_if<na::FunctionRef>(&r)) {
        out.kind = py::str("function"), out.seg = f->seg, out.off = f->off;
    } else if (const auto* s = std::get_if<na::SegmentRef>(&r)) {
        out.kind = py::str("segment"), out.seg = s->seg;
    } else if (const auto* d = std::get_if<na::DataRef>(&r)) {
        out.kind = py::str("data"), out.seg = d->seg;
    } else {
        const auto& i = std::get<na::ImportRef>(r);
        out.kind = py::str("import");
        if (i.module) out.module = cb::compat::to_py(*i.module);
        out.which = old_which(i.which);
    }
    return out;
}
inline py::str linkage_text(na::Linkage k) {
    switch (k) {
        case na::Linkage::Near: return py::str("near");
        case na::Linkage::Far: return py::str("far");
        case na::Linkage::Byte: return py::str("byte");
    }
    return py::str("near");
}
inline cb::platforms::DecoderSet& decoders() {
    static thread_local cb::platforms::DecoderSet set;
    return set;
}

}  // namespace shim

class Function {
  public:
    // `seg` the segment number (1-based), [start, end) the function in it -- or the run of functions, written together as their
    // source file had them. cs_ok: offsets of the segment below it may be read through CS (the lead-in). extra_entries: other
    // places of the run called into from outside. whole: the run is a whole segment (any place may be read through CS by its
    // number). holes: [(start, end, symbol)] of functions inside it rebuilt from C, which the build places after it.
    Function(const formats::ne::NEFile& nef, int64_t seg, int64_t start, int64_t end, const Publics& public_, const SymbolFor& symbol_for,
             int64_t cs_ok = 0, const py::list<int64_t>& extra_entries = py::list<int64_t>(), bool whole = false,
             const py::list<std::tuple<int64_t, int64_t, py::str>>& holes = py::list<std::tuple<int64_t, int64_t, py::str>>()) {
        na::Request request;
        request.seg = static_cast<std::uint16_t>(seg);
        request.start = static_cast<std::uint32_t>(start);
        request.end = static_cast<std::uint32_t>(end);
        if (const auto* one = std::get_if<py::str>(&public_)) {
            request.publics.push_back({request.start, one->utf8()});
        } else {
            for (const auto& [off, symbol] : std::get<py::list<std::tuple<int64_t, py::str>>>(public_)) {
                request.publics.push_back({static_cast<std::uint32_t>(off), symbol.utf8()});
            }
        }
        request.cs_ok = static_cast<std::uint32_t>(cs_ok);
        for (const int64_t e : extra_entries) request.extra_entries.push_back(static_cast<std::uint32_t>(e));
        request.whole = whole;
        for (const auto& [lo, hi, symbol] : holes) request.holes.push_back({static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(hi), symbol.utf8()});
        const na::SymbolFor wrapped = [&](const na::Reference& r) -> std::optional<std::string> {
            const std::optional<py::str> answer = symbol_for(shim::old_reference(r));
            if (!answer) return std::nullopt;
            return answer->utf8();
        };
        cb::compat::guard([&] { impl_ = std::make_shared<na::Function>(nef.module(), request, wrapped, shim::decoders()); });
        refresh();
    }

    // --- the result
    std::optional<py::str> text;                             // the assembly, None when it cannot be written so
    py::dict<py::str, std::tuple<py::str, Which>> imports;   // symbol -> (module, ordinal or name)
    py::str why;                                             // why text is None
    py::dict<py::str, py::str> externs;                      // symbol -> "far" | "near" | "byte"
    py::set<int64_t> missing;                                // near-call targets without a symbol
    py::set<int64_t> tables;                                 // jump-table places
    py::set<std::tuple<int64_t, int64_t>> far_missing;       // (segment, offset) a far call or jump reaches mid-run
    std::optional<int64_t> reaches;                          // refused for jumping into the middle of this offset's function
    py::list<std::tuple<int64_t, int64_t, py::str>> holes;   // sorted
    py::dict<int64_t, std::tuple<py::str, int64_t>> raw_hole_calls;   // offset -> (symbol, opcode)
    py::set<std::tuple<int64_t, int64_t>> raw_far_entries;   // (segment, offset) a raw far call or pushed pointer goes to
    py::list<std::tuple<int64_t, py::str>> publics;          // (offset, symbol), sorted
    py::dict<int64_t, int64_t> cs_labels;                    // offset of an instruction -> the place in the run it reads through CS
    // set by build.module after it assembled the function: the text it says (why) was assembled with, or None
    std::optional<py::str> assembled;

    // --- what the build and check read (set once the code is followed)
    int64_t bits = 16;
    int64_t start = 0, end = 0;
    py::bytes blob;                                          // the segment's bytes as the loader has them
    py::dict<int64_t, int64_t> code_segments;                // segment -> bytes of code
    py::dict<int64_t, std::tuple<int64_t, py::str>> sites;   // offset -> (width, directive text)
    py::dict<int64_t, std::tuple<py::bytes, py::str, py::str, std::optional<int64_t>>> ins;   // offset -> (bytes, mnemonic, operands, label target)
    py::dict<int64_t, std::tuple<int64_t, py::str>> symbolic;   // offset -> (size, text): an instruction whose fixup has a name
    py::dict<int64_t, std::tuple<int64_t, py::str>> calls;      // offset -> (size, text): near calls/jumps leaving the run
    py::set<int64_t> labels;
    py::str module;
    int64_t seg = 0;
    py::str public_;                                         // the first public symbol

    // the mnemonic text of the instruction at `at`
    py::str mnemonic(int64_t at) const {
        return cb::compat::guard([&] { return cb::compat::to_py(impl_->mnemonic(static_cast<std::uint32_t>(at))); });
    }
    // {offset: (text, bytes, mask)} of the instructions the assembler's spelling is probed for
    py::dict<int64_t, std::tuple<py::str, py::bytes, py::bytes>> candidates() const {
        py::dict<int64_t, std::tuple<py::str, py::bytes, py::bytes>> out;
        for (const auto& [at, c] : impl_->candidates()) {
            out.set(at, std::make_tuple(cb::compat::to_py(c.text), cb::compat::to_py(cb::ByteView(c.original)), cb::compat::to_py(cb::ByteView(c.mask))));
        }
        return out;
    }
    // {name: offset} of each function rebuilt from C (a hole) that a near call or jump in the part of the frame NOT followed as
    // code goes to (also fills raw_hole_calls)
    py::dict<py::str, int64_t> raw_calls_into_holes() {
        py::dict<py::str, int64_t> out;
        for (const auto& [name, at] : cb::compat::guard([&] { return impl_->raw_calls_into_holes(); })) out.set(cb::compat::to_py(name), at);
        refresh();
        return out;
    }
    // The source, with the instructions in `good` (offsets) as mnemonics and the rest as bytes (None: every instruction). With lo,
    // hi: only that piece of it.
    py::str write(const std::optional<py::set<int64_t>>& good, const std::optional<int64_t>& lo = std::nullopt,
                  const std::optional<int64_t>& hi = std::nullopt) {
        na::Function::Good wanted;
        if (good) {
            std::set<std::uint32_t> offsets;
            for (const int64_t a : *good) offsets.insert(static_cast<std::uint32_t>(a));
            wanted = std::move(offsets);
        }
        std::optional<std::pair<std::uint32_t, std::uint32_t>> piece;
        if (lo || hi) piece = {static_cast<std::uint32_t>(lo.value_or(impl_->start())), static_cast<std::uint32_t>(hi.value_or(impl_->end()))};
        py::str out = cb::compat::guard([&] { return cb::compat::to_py(impl_->write(wanted, piece)); });
        refresh();
        return out;
    }

    const na::Function& impl() const { return *impl_; }
    na::Function& impl() { return *impl_; }

  private:
    // the old record's data, from the new Function's
    void refresh() {
        using cb::compat::to_py;
        const na::Function& f = *impl_;
        text = f.text() ? std::optional<py::str>(to_py(*f.text())) : std::nullopt;
        why = to_py(f.why());
        imports = {};
        for (const auto& [sym, i] : f.imports()) imports.set(to_py(sym), std::make_tuple(to_py(i.module), shim::old_which(i.which)));
        externs = {};
        for (const auto& [sym, k] : f.externs()) externs.set(to_py(sym), shim::linkage_text(k));
        missing = {};
        for (const auto a : f.missing()) missing.add(static_cast<int64_t>(a));
        tables = {};
        for (const auto a : f.tables()) tables.add(static_cast<int64_t>(a));
        far_missing = {};
        for (const auto& [s, o] : f.far_missing()) far_missing.add(std::make_tuple(static_cast<int64_t>(s), static_cast<int64_t>(o)));
        reaches = f.reaches() ? std::optional<int64_t>(*f.reaches()) : std::nullopt;
        holes = {};
        for (const auto& h : f.holes()) holes.append(std::make_tuple(static_cast<int64_t>(h.start), static_cast<int64_t>(h.end), to_py(h.symbol)));
        raw_hole_calls = {};
        for (const auto& [a, c] : f.raw_hole_calls()) raw_hole_calls.set(static_cast<int64_t>(a), std::make_tuple(to_py(c.symbol), static_cast<int64_t>(c.opcode)));
        raw_far_entries = {};
        for (const auto& [s, o] : f.raw_far_entries()) raw_far_entries.add(std::make_tuple(static_cast<int64_t>(s), static_cast<int64_t>(o)));
        publics = {};
        for (const auto& p : f.publics()) publics.append(std::make_tuple(static_cast<int64_t>(p.offset), to_py(p.symbol)));
        cs_labels = {};
        for (const auto& [a, d] : f.cs_labels()) cs_labels.set(static_cast<int64_t>(a), static_cast<int64_t>(d));
        bits = f.bits();
        start = f.start();
        end = f.end();
        blob = to_py(f.blob());
        code_segments = {};
        for (const auto& [s, n] : f.code_segments()) code_segments.set(static_cast<int64_t>(s), static_cast<int64_t>(n));
        sites = {};
        for (const auto& [a, d] : f.sites()) sites.set(static_cast<int64_t>(a), std::make_tuple(static_cast<int64_t>(d.size), to_py(d.text)));
        ins = {};
        for (const auto& [a, i] : f.insns()) {
            ins.set(static_cast<int64_t>(a), std::make_tuple(to_py(cb::ByteView(i.bytes)), to_py(i.mnemonic), to_py(i.operands),
                                                            i.target ? std::optional<int64_t>(*i.target) : std::nullopt));
        }
        symbolic = {};
        for (const auto& [a, d] : f.symbolic()) symbolic.set(static_cast<int64_t>(a), std::make_tuple(static_cast<int64_t>(d.size), to_py(d.text)));
        calls = {};
        for (const auto& [a, d] : f.calls()) calls.set(static_cast<int64_t>(a), std::make_tuple(static_cast<int64_t>(d.size), to_py(d.text)));
        labels = {};
        for (const auto a : f.labels()) labels.add(static_cast<int64_t>(a));
        module = to_py(f.module_name());
        seg = f.seg();
        public_ = to_py(f.first_public());
    }

    std::shared_ptr<na::Function> impl_;
};

// A piece of a frame (see Function::write) against the original. -> "" or what differs.
inline py::str check_piece(const py::bytes& obj, Function& f, int64_t lo, int64_t hi) {
    return cb::compat::guard([&] {
        const std::optional<std::string> bad = na::check_piece(cb::compat::from_py(obj), f.impl(), static_cast<std::uint32_t>(lo), static_cast<std::uint32_t>(hi));
        return cb::compat::to_py(bad.value_or(""));
    });
}

// The assembled object against the original: every byte the same but those the loader or the linker sets. -> "" or where.
inline py::str check(const py::bytes& obj, const Function& f) {
    return cb::compat::guard([&] { return cb::compat::to_py(na::check(cb::compat::from_py(obj), f.impl()).value_or("")); });
}

// The offsets (in the segment) where the assembled bytes are not the original's, the places the loader or the linker sets aside.
inline py::list<int64_t> differences(const py::bytes& code, const py::bytes& mask, const Function& f) {
    py::list<int64_t> out;
    for (const std::uint32_t at : na::differences(cb::compat::from_py(code), cb::compat::from_py(mask), f.impl())) out.append(static_cast<int64_t>(at));
    return out;
}

}  // namespace codebreaker::build::asm16
