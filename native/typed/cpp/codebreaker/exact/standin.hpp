// codebreaker/exact/standin.hpp -- compat shim for cb/exact/standin.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer (exact/link, exact/recut, exact/project).
//
//     auto [raw, stand] = standin::rebuild(prj, raw, addrs, end, title, alts, decode);
//
// `Project` is the members of exact.project.Project that standin.py reads (the old layer's own interface; the real
// Project implements it); `Fixup` and `Threads` are the old shapes of an OMF fix-up and the thread table that
// exact/recut still reads its records with.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <unordered_set>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/standin.hpp"
#include "codebreaker/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::standin {

using Symbols = compare::Symbols;

// exact.project.Project, as standin.py uses it
class Project {
  public:
    virtual ~Project() = default;
    virtual py::bytes image() const = 0;                                // prj.image
    virtual Symbols symbols() const = 0;                                // prj.symbols()
    virtual int64_t loadseg() const = 0;                                // prj.loadseg
    virtual int64_t dgroup() const = 0;                                 // prj.dgroup
    virtual int64_t code_base(int64_t a) const = 0;                     // prj.code_base(a)
    virtual const py::set<int64_t>& relocs() const = 0;                 // prj.relocs()
    // prj.packed.relocs when the project has a packed original (getattr(prj, "packed", None) is truthy), else nullopt
    virtual std::optional<py::list<int64_t>> packed_relocs() const = 0;
};

// one fix-up of a FIXUPP record (a dict in the Python); `seg` and `at` are set when the fix-up is placed
struct Fixup {
    int64_t M = 0;
    int64_t loc = 0;
    int64_t off = 0;
    int64_t fm = 0;
    std::optional<int64_t> fi;
    int64_t tm = 0;
    std::optional<int64_t> ti;
    int64_t P = 0;
    int64_t disp = 0;
    std::optional<int64_t> seg;
    std::optional<int64_t> at;
};

// {(is a frame thread, thread number): (method, index or None)}
using Threads = py::dict<std::tuple<int64_t, int64_t>, std::tuple<int64_t, std::optional<int64_t>>>;

namespace detail {

namespace nw = cb::exact::standin;

inline nw::Fixup to_new(const Fixup& f) {
    using cb::compat::int_from_py;
    nw::Fixup n;
    n.segment_relative = f.M != 0;
    n.location = int_from_py<std::uint8_t>(f.loc);
    n.offset = int_from_py<std::uint16_t>(f.off);
    n.frame_method = int_from_py<std::uint8_t>(f.fm);
    if (f.fi) n.frame_datum = int_from_py<std::uint16_t>(*f.fi);
    n.target_method = int_from_py<std::uint8_t>(f.tm);
    n.target_datum = int_from_py<std::uint16_t>(f.ti.value());
    n.no_displacement = f.P != 0;
    n.displacement = int_from_py<std::uint16_t>(f.disp);
    if (f.seg) n.segment = int_from_py<std::uint32_t>(*f.seg);
    n.at = int_from_py<std::uint32_t>(f.at.value());
    return n;
}

inline Fixup to_old(const nw::Fixup& n) {
    Fixup f;
    f.M = n.segment_relative ? 1 : 0;
    f.loc = n.location;
    f.off = n.offset;
    f.fm = n.frame_method;
    if (n.frame_datum) f.fi = *n.frame_datum;
    f.tm = n.target_method;
    f.ti = n.target_datum;
    f.P = n.no_displacement ? 1 : 0;
    f.disp = n.displacement;
    return f;
}

inline nw::FixupThreads threads_to_new(const Threads& thr) {
    nw::FixupThreads t;
    for (const auto& kv : thr.items()) {
        const auto& [key, value] = kv;
        nw::FixupThreads::Thread th{cb::compat::int_from_py<std::uint8_t>(std::get<0>(value)), std::nullopt};
        if (std::get<1>(value)) th.datum = cb::compat::int_from_py<std::uint16_t>(*std::get<1>(value));
        const auto number = cb::compat::int_from_py<unsigned>(std::get<1>(key));
        if (std::get<0>(key) == 1) t.define_frame(number, th);
        else t.define_target(number, th);
    }
    return t;
}

inline void threads_to_old(const nw::FixupThreads& t, Threads& thr) {
    for (unsigned n = 0; n < 4; ++n) {
        for (int frame = 0; frame < 2; ++frame) {
            const auto& th = frame ? t.frame(n) : t.target(n);
            if (!th) continue;
            std::optional<int64_t> datum;
            if (th->datum) datum = *th->datum;
            thr.set(std::make_tuple(int64_t(frame), int64_t(n)), std::make_tuple(int64_t(th->method), datum));
        }
    }
}

inline cb::exact::SymbolTable symbols_to_new(const Symbols& syms) {
    cb::exact::SymbolTable table;
    for (const auto& kv : syms.items())
        table.insert_or_assign(std::get<0>(kv).utf8(), cb::exact::SymPlace{compare::detail::kind_of(std::get<0>(std::get<1>(kv))), std::get<1>(std::get<1>(kv))});
    return table;
}

}  // namespace detail

inline py::bytes _ix(int64_t n) {
    return cb::compat::guard([&] {
        cb::Bytes b;
        cb::exact::standin::append_index(b, cb::compat::int_from_py<std::uint16_t>(n));
        return cb::compat::to_py(cb::ByteView(b));
    });
}

inline std::tuple<int64_t, int64_t> _idx(const py::bytes& r, int64_t q) {
    return cb::compat::guard([&] {
        const cb::Bytes b = cb::compat::from_py(r);
        const auto [index, next] = cb::exact::standin::read_index(b, cb::compat::int_from_py<std::size_t>(q));
        return std::make_tuple(int64_t(index), int64_t(next));
    });
}

// a FIXUPP record's fixups, the threads resolved -> [Fixup]; thr is kept from record to record
inline py::list<Fixup> fixups(const py::bytes& r, const Threads& thr) {
    return cb::compat::guard([&] {
        auto table = detail::threads_to_new(thr);
        const cb::Bytes b = cb::compat::from_py(r);
        py::list<Fixup> out;
        for (const auto& f : cb::exact::standin::read_fixups(b, table)) out.append(detail::to_old(f));
        detail::threads_to_old(table, const_cast<Threads&>(thr));   // (the dict is a shared handle: the caller's table gains the record's threads)
        return out;
    });
}

// the fixups (their places as offsets in the segment) written for a data record that begins at base
inline py::bytes fixrec(const py::list<Fixup>& fs, int64_t base) {
    return cb::compat::guard([&] {
        std::vector<cb::exact::standin::Fixup> fixes;
        for (const Fixup& f : fs) fixes.push_back(detail::to_new(f));
        return cb::compat::to_py(cb::ByteView(cb::exact::standin::write_fixup_record(fixes, cb::compat::int_from_py<std::uint32_t>(base))));
    });
}

// -> (the object module again, the names of the functions that are stand-ins)
inline std::tuple<py::bytes, py::list<py::str>> rebuild(const Project& prj, const py::bytes& raw, const py::list<int64_t>& addrs, int64_t end,
                                                        const std::optional<py::str>& title, const py::list<py::bytes>& alts,
                                                        const compare::Decoder& decode) {
    return cb::compat::guard([&] {
        using cb::compat::int_from_py;
        const cb::Bytes image = cb::compat::from_py(prj.image());
        const cb::exact::SymbolTable symbols = detail::symbols_to_new(prj.symbols());
        std::unordered_set<std::uint32_t> relocs;
        for (int64_t r : prj.relocs()) relocs.insert(int_from_py<std::uint32_t>(r));
        std::vector<std::uint32_t> relocations;
        if (const auto packed = prj.packed_relocs()) {
            for (int64_t r : *packed) relocations.push_back(int_from_py<std::uint32_t>(r));
        } else {
            relocations.assign(relocs.begin(), relocs.end());
        }
        std::sort(relocations.begin(), relocations.end());
        cb::exact::compare::Program compare;
        compare.dgroup = int_from_py<std::uint32_t>(prj.dgroup());
        compare.loadseg = int_from_py<std::uint32_t>(prj.loadseg());
        compare.code_base = [&](std::uint32_t a) { return int_from_py<std::uint32_t>(prj.code_base(a)); };
        compare.relocs = &relocs;
        const cb::exact::standin::Program program{image, symbols, compare, relocations};
        std::vector<std::uint32_t> places;
        for (int64_t a : addrs) places.push_back(int_from_py<std::uint32_t>(a));
        std::vector<cb::Bytes> others;
        for (const py::bytes& a : alts) others.push_back(cb::compat::from_py(a));
        std::optional<std::string> name;
        if (title) name = cb::compat::from_py(*title);
        const compare::detail::OldDecoder decoder(decode);
        auto got = cb::exact::standin::rebuild(program, cb::compat::from_py(raw), places, int_from_py<std::uint32_t>(end),
                                               name ? std::optional<std::string_view>(*name) : std::nullopt, others, decoder);
        return std::make_tuple(cb::compat::to_py(cb::ByteView(got.object)), cb::compat::to_py(got.stand_ins));
    });
}

}  // namespace codebreaker::exact::standin
