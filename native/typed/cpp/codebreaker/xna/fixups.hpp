// codebreaker/xna/fixups.hpp -- compat shim for cb/xna/fixups.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer (xna/run.cpp).
//
//     (text, n) = fixups::ref_casts(text)          ((Color)(ref white)).A = 64   ->   (white).A = 64
//     (text, n, left) = fixups::constructors(text) base._002Ector(a);  ->  `: base(a)` on the constructor;
//                                                  v._002Ector(a);     ->  v = new T(a);   (T from v's declaration)
//     (text, n) = fixups::data_holders(text)       a compiler's static-data holder: its private blob structs internal
//     report = fixups::apply(folder)               all of them, over every .cs file under the folder
//
// apply()'s `left` files are '/'-separated (the python wrote the OS's separator).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/xna/fixups.hpp"
#include "py.hpp"

namespace codebreaker::xna::fixups {

// ((Color)(ref white)).A = 64   ->   (white).A = 64      -> (text, rewrites)
inline std::tuple<py::str, int64_t> ref_casts(const py::str& text) {
    const cb::xna::fixups::Rewritten r = cb::xna::fixups::ref_casts(cb::compat::from_py(text));
    return {cb::compat::to_py(r.text), cb::compat::to_py(r.count)};
}

// -> (text, rewrites, left as (line, why))
inline std::tuple<py::str, int64_t, py::list<std::tuple<int64_t, py::str>>> constructors(const py::str& text) {
    const cb::xna::fixups::ConstructorsResult r = cb::xna::fixups::constructors(cb::compat::from_py(text));
    py::list<std::tuple<int64_t, py::str>> left;
    for (const cb::xna::fixups::Unplaced& u : r.left) left.append({cb::compat::to_py(u.line), cb::compat::to_py(u.why)});
    return {cb::compat::to_py(r.text), cb::compat::to_py(r.count), left};
}

// The compiler's holder of static array data: its private blob structs become internal. -> (text, structs changed)
inline std::tuple<py::str, int64_t> data_holders(const py::str& text) {
    const cb::xna::fixups::Rewritten r = cb::xna::fixups::data_holders(cb::compat::from_py(text));
    return {cb::compat::to_py(r.text), cb::compat::to_py(r.count)};
}

// apply()'s report: the Python dict {"ref casts", "constructors", "data holders", "left"}.
struct Report {
    int64_t ref_casts = 0;
    int64_t constructors = 0;
    int64_t data_holders = 0;
    py::list<std::tuple<py::str, int64_t, py::str>> left;   // (file relative to the folder, line, why)
};

// All the rewrites, over every .cs file under the folder (changed files are written back).
inline Report apply(const py::str& folder) {
    return cb::compat::guard([&] {
        const cb::xna::fixups::Report r = cb::xna::fixups::apply(cb::fs::from_text(cb::compat::from_py(folder)));
        Report out;
        out.ref_casts = cb::compat::to_py(r.ref_casts);
        out.constructors = cb::compat::to_py(r.constructors);
        out.data_holders = cb::compat::to_py(r.data_holders);
        for (const cb::xna::fixups::LeftInFile& l : r.left)
            out.left.append({cb::compat::to_py(l.file), cb::compat::to_py(l.line), cb::compat::to_py(l.why)});
        return out;
    });
}

}  // namespace codebreaker::xna::fixups
