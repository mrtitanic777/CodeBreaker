// codebreaker/exact/compare.hpp -- compat shim for cb/exact/compare.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer.
//
//     bad, notes = compare::function(obj, image, name, offset, length, at, project, symbols, learnt, decode,
//                                    text="_TEXT", where=None)
//
// Stand-ins (the Python takes these as duck-typed objects of modules with no C++ port yet):
//   Obj      an OMF/a.out object as omf.read / aout.read return it (the members compare reads: seg, fix, pubs)
//   Project  the members of exact.project.Project compare reads (dgroup, loadseg, code_base(), relocs())
//   Decoder  platforms.decoder(16).disasm(code, address): the 16-bit x86 instructions of a buffer
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::compare {

// What a fixup names: ("ext", name) an external, ("seg", segment) a segment's start, or (anything, segment).
using Target = std::tuple<py::str, py::str>;
// One fixup: (segment, offset of the word in it, kind "rel"|"ptr"|"base"|"off", target, displacement).
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;
// One public: (name, segment, offset).
using Public = std::tuple<py::str, py::str, int64_t>;
// {name: (kind "C" | "D" | "S" | "?", address)}
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::list<Fixup> fix;
    py::list<Public> pubs;
};

struct Project {
    int64_t dgroup = 0;
    int64_t loadseg = 0;
    std::function<int64_t(int64_t)> code_base;                // the image offset of the code segment `a` is in
    std::function<const py::set<int64_t>&()> relocs;          // the places in the load image the loader adds the load segment to
};

// One decoded instruction (capstone's address, size, op_str and bytes).
struct Insn {
    int64_t address = 0;
    int64_t size = 0;
    py::str op_str;
    py::bytes bytes;
};
using Decoder = std::function<py::list<Insn>(const py::bytes&, int64_t)>;

using Where = std::function<int64_t(int64_t)>;

namespace detail {

inline cb::exact::SymKind kind_of(const py::str& s) {
    const std::string k = s.utf8();
    return k == "C" ? cb::exact::SymKind::Code : k == "D" ? cb::exact::SymKind::Data : k == "S" ? cb::exact::SymKind::Segment : cb::exact::SymKind::Unknown;
}

// A decoder given as a function of the old layer (bytes in, instructions out).
class OldDecoder final : public cb::exact::x86::Decoder16 {
public:
    explicit OldDecoder(const Decoder& d) : decode_(d) {}
    std::vector<cb::exact::x86::Insn> all(cb::ByteView code, std::uint32_t address) const override {
        std::vector<cb::exact::x86::Insn> out;
        for (const Insn& i : decode_(cb::compat::to_py(code), address))
            out.push_back({cb::compat::int_from_py<std::uint32_t>(i.address), cb::compat::from_py(i.bytes), "", cb::compat::from_py(i.op_str)});
        return out;
    }
    std::optional<cb::exact::x86::Insn> first(cb::ByteView code, std::uint32_t address) const override {
        std::vector<cb::exact::x86::Insn> v = all(code, address);
        if (v.empty()) return std::nullopt;
        return std::move(v.front());
    }

private:
    const Decoder& decode_;
};

inline cb::exact::ObjectFile object_of(const Obj& o) {
    using namespace cb::exact;
    ObjectFile out;
    for (const auto& kv : o.seg.items()) out.seg.insert_or_assign(std::get<0>(kv).utf8(), cb::compat::from_py(std::get<1>(kv)));
    for (const Public& p : o.pubs) out.pubs.push_back({std::get<0>(p).utf8(), std::get<1>(p).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<2>(p))});
    for (const Fixup& f : o.fix) {
        const std::string kind = std::get<2>(f).utf8(), tk = std::get<0>(std::get<3>(f)).utf8();
        out.fix.push_back({std::get<0>(f).utf8(), cb::compat::int_from_py<std::uint32_t>(std::get<1>(f)),
                           kind == "rel" ? FixKind::Rel : kind == "ptr" ? FixKind::Ptr : kind == "base" ? FixKind::Base : kind == "off" ? FixKind::Off : FixKind::Lo,
                           {tk == "ext" ? TargetKind::External : tk == "grp" ? TargetKind::Group : TargetKind::Segment, std::get<1>(std::get<3>(f)).utf8()},
                           cb::compat::int_from_py<std::int32_t>(std::get<4>(f))});
    }
    return out;
}

inline py::str warning_text(const std::string& t) { return py::str(t); }

}  // namespace detail

// -> (offsets of differing bytes, notes; a warning's note starts with "? ")
inline std::tuple<py::list<int64_t>, py::list<py::str>> function(
    const Obj& o, const py::bytes& img, const py::str& name, int64_t off, int64_t ln, int64_t at, const Project& prj,
    const Symbols& syms, const Symbols& learn, const Decoder& decode, const py::str& text = py::str("_TEXT"),
    const std::optional<Where>& where = std::nullopt) {
    return cb::compat::guard([&] {
        using namespace cb::exact;
        using cb::compat::int_from_py;
        const ObjectFile object = detail::object_of(o);
        const cb::Bytes image = cb::compat::from_py(img);
        SymbolTable table, learnt;
        for (const auto& kv : syms.items())
            table.insert_or_assign(std::get<0>(kv).utf8(), SymPlace{detail::kind_of(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))});
        std::unordered_set<std::uint32_t> relocs;
        for (int64_t r : prj.relocs()) relocs.insert(int_from_py<std::uint32_t>(r));
        cb::exact::compare::Program program;
        program.dgroup = int_from_py<std::uint32_t>(prj.dgroup);
        program.loadseg = int_from_py<std::uint32_t>(prj.loadseg);
        program.code_base = [&](std::uint32_t a) { return int_from_py<std::uint32_t>(prj.code_base(a)); };
        program.relocs = &relocs;
        cb::exact::compare::Where placed;
        if (where) placed = [&](std::uint32_t x) { return (*where)(x); };
        detail::OldDecoder decoder(decode);
        const cb::exact::compare::Subject subject{object, name.utf8(), int_from_py<std::uint32_t>(off), int_from_py<std::uint32_t>(ln),
                                       int_from_py<std::uint32_t>(at), text.utf8()};
        const cb::exact::compare::Report report = cb::exact::compare::function(subject, image, program, table, learnt, placed, decoder);
        for (const auto& [n, place] : learnt) {   // (the names learnt join both of the caller's tables)
            const std::string letter(1, sym_kind_letter(place.kind));
            syms.set(py::str(n), std::make_tuple(py::str(letter), place.address));
            learn.set(py::str(n), std::make_tuple(py::str(letter), place.address));
        }
        py::list<int64_t> bad = cb::compat::to_py(report.bad);
        py::list<py::str> notes;
        for (const cb::exact::compare::Note& n : report.notes) notes.append(py::str(n.severity == cb::exact::compare::Severity::Warning ? "? " + n.text : n.text));
        return std::make_tuple(bad, notes);
    });
}

inline bool identical(const py::list<int64_t>& bad, const py::list<py::str>& notes) {
    if (!bad.empty()) return false;
    for (const py::str& n : notes)
        if (!n.startswith(py::str("?"))) return false;
    return true;
}

}  // namespace codebreaker::exact::compare
