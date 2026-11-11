// codebreaker/xna/mgfx.hpp -- compat shim for cb/xna/mgfx.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer (xna/port.cpp).
//
//     mgfx::add_parameters(data, parameters)   the compiled effect with every parameter in `parameters` (name,
//                                              semantic, type, cls, rows, columns, elements, members, value --
//                                              formats/xnb.py writes them) it lacks appended, with its default
//
// A key that is None in the python (semantic, rows, columns, elements, value) is nullopt here; "members" None is the
// empty list. `value` holds numbers (ints are held as the floats the module makes of them).
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/xna/mgfx.hpp"
#include "py.hpp"

namespace codebreaker::xna::mgfx {

struct Parameter {
    py::str name;
    std::optional<py::str> semantic;
    py::str type;
    py::str cls;
    std::optional<int64_t> rows;
    std::optional<int64_t> columns;
    std::optional<int64_t> elements;
    std::optional<py::list<double>> value;
    py::list<Parameter> members;
};

namespace detail {

// A count the python could not have written (negative) reaches the new module as a value it refuses when written.
inline std::uint32_t count_to_new(const std::optional<int64_t>& v) {
    if (!v) return 0;
    return *v < 0 || *v > UINT32_MAX ? UINT32_MAX : static_cast<std::uint32_t>(*v);
}

inline cb::xna::mgfx::Parameter to_new(const Parameter& p) {
    cb::xna::mgfx::Parameter out;
    out.name = cb::compat::from_py(p.name);
    if (p.semantic) out.semantic = cb::compat::from_py(*p.semantic);
    out.type = cb::xna::mgfx::parse_type(cb::compat::from_py(p.type));
    out.cls = cb::xna::mgfx::parse_class(cb::compat::from_py(p.cls));
    out.rows = count_to_new(p.rows);
    out.columns = count_to_new(p.columns);
    out.elements = count_to_new(p.elements);
    if (p.value) out.value = cb::compat::from_py(*p.value);
    for (const Parameter& m : p.members) out.members.push_back(to_new(m));
    return out;
}

}  // namespace detail

// `data` (an .mgfxo) with each of `parameters` it lacks (by name) appended to its parameter list.
inline py::bytes add_parameters(const py::bytes& data, const py::list<Parameter>& parameters) {
    return cb::compat::guard([&] {
        std::vector<cb::xna::mgfx::Parameter> params;
        for (const Parameter& p : parameters) params.push_back(detail::to_new(p));
        const cb::Bytes bytes = cb::compat::from_py(data);
        return cb::compat::to_py(cb::ByteView(cb::xna::mgfx::add_parameters(bytes, params)));
    });
}

}  // namespace codebreaker::xna::mgfx
