// codebreaker/exact/aout.hpp -- compat shim for cb/exact/aout.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/aout.hpp"
#include "py.hpp"

namespace codebreaker::exact::aout {

inline const py::list<int64_t> MAGICS = {0407, 0410, 0411, 0x0107, 0x0109};

inline bool is_aout(const py::bytes& data) { return cb::exact::aout::is_aout(cb::compat::from_py(data)); }

using Target = std::tuple<py::str, py::str>;                                     // ("seg", "_TEXT") or ("ext", name)
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;           // (segment, offset, kind, target, disp)
using SegInfo = std::tuple<py::str, py::str, int64_t, int64_t>;                 // (name, class, size, align)
using Sym = std::tuple<py::str, int64_t, int64_t>;                              // (name, type, value)
using Pub = std::tuple<py::str, py::str, int64_t>;                              // (name, segment, value)

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::dict<py::str, int64_t> filled;
    py::list<SegInfo> segs;
    py::dict<py::str, py::list<py::str>> groups;
    py::list<Sym> syms;
    py::list<Pub> pubs;
    py::list<py::str> ext;
    py::dict<py::str, std::tuple<int64_t, bool>> comm;
    py::list<Fixup> fix;
    py::bytes raw;
};

// The old record of an object file (also what exact/linkemu and exact/compare take, as the Python's omf.read gives).
inline Obj to_old(const cb::exact::ObjectFile& o, const std::vector<cb::exact::aout::Symbol>& symbols) {
    using cb::compat::to_py;
    Obj out;
    for (const auto& [name, bytes] : o.seg) out.seg.set(to_py(name), to_py(cb::ByteView(bytes)));
    for (const auto& [name, n] : o.filled) out.filled.set(to_py(name), to_py(n));
    for (const auto& s : o.segs) out.segs.append(SegInfo(to_py(s.name), to_py(s.cls), to_py(s.length), to_py(s.align)));
    for (const auto& [name, members] : o.groups) out.groups.set(to_py(name), to_py(members));
    for (const auto& s : symbols) out.syms.append(Sym(to_py(s.name), s.type, s.value));
    for (const auto& p : o.pubs) out.pubs.append(Pub(to_py(p.name), to_py(*p.segment), to_py(p.offset)));
    for (const auto& e : o.ext) out.ext.append(to_py(e));
    for (const auto& [name, v] : o.comm) out.comm.set(to_py(name), std::make_tuple(to_py(v.size), v.far));
    for (const auto& f : o.fix)
        out.fix.append(Fixup(to_py(f.segment), to_py(f.offset), py::str(f.kind == cb::exact::FixKind::Rel ? "rel" : "off"),
                             Target(py::str(f.target.kind == cb::exact::TargetKind::External ? "ext" : "seg"), to_py(f.target.name)),
                             to_py(f.displacement)));
    out.raw = to_py(cb::ByteView(o.raw));
    return out;
}

inline Obj read(const py::bytes& data) {
    return cb::compat::guard([&] {
        const cb::exact::aout::Image image = cb::exact::aout::read(cb::compat::from_py(data));
        return to_old(image.object, image.symbols);
    });
}

}  // namespace codebreaker::exact::aout
