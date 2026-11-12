// codebreaker/exact/data.hpp -- compat shim for cb/exact/data.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion and
// forwarding. Deleted with its last old-layer consumer (exact/run, exact/project).
//
//     python -m codebreaker exact PROJECT data MODULE [--draft] [--write]
//
//     cstr(bytes) -> text       draft(prj, row) -> {file name: text}       main(prj, argv) -> exit code
//
// `Project` is the old layer's own interface (modules::Project's members and the toolchain profile's data rule); the real
// one implements it. Source text crosses as Python text whose characters are the file's bytes (Latin-1), as in
// exact/modules. What main prints goes to stdout (cb::out). The functions the Python kept for its own use (walk, _align,
// externs, parse_structs, the globals CODE, FUNCS and HOME ...) are not shimmed: nothing outside the module called them.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/data.hpp"
#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/project_forward.hpp"
#include "py.hpp"

namespace codebreaker::exact::data {

class Project : public modules::Project {
  public:
    // prj.toolchain().p.get("data", {}).get("order", "after_next_function")
    virtual py::str data_order() const = 0;
};

namespace detail {

using OldProject = codebreaker::exact::data::Project;   // (inside NewView, `Project` is the new layer's base class)

// The old project as the new layer's interface.
class NewView final : public modules::detail::ForwardedProject<cb::exact::data::Project> {
  public:
    explicit NewView(const OldProject& old) : ForwardedProject(old), order_(cb::compat::from_py(old.data_order())) {}
    std::string data_order() const override { return order_; }

  private:
    std::string order_;
};

}  // namespace detail

inline py::str cstr(const py::bytes& b) {
    return cb::compat::guard([&] { return modules::detail::latin1_text(cb::exact::data::c_string(cb::compat::from_py(b))); });
}

// {file name: text} - the module's data files proposed (the profile's rule says where each run goes)
inline py::dict<py::str, py::str> draft(const Project& prj, const modules::Row& row) {
    return cb::compat::guard([&] {
        py::dict<py::str, py::str> out;
        for (const auto& f : cb::exact::data::draft(detail::NewView(prj), modules::detail::row_to_new(row)))
            out.set(cb::compat::to_py(f.name), modules::detail::latin1_text(f.text));
        return out;
    });
}

inline int64_t main(const Project& prj, const py::list<py::str>& argv) {
    return cb::compat::guard([&] {
        std::vector<std::string> args;
        for (const py::str& a : argv) args.push_back(cb::compat::from_py(a));
        return int64_t(cb::exact::data::run(detail::NewView(prj), args));
    });
}

}  // namespace codebreaker::exact::data
