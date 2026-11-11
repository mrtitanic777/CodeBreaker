// codebreaker/exact/emu/isis.hpp -- compat shim for cb/exact/emu/isis.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumer (exact/plm86). Only what a consumer uses is kept: the old
// Machine class, `main`, `Exit` and `PAR` were the Python module's internals and its command line.
#pragma once

#include <iostream>
#include <ostream>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/exact/emu/isis.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::emu::isis {

// class Result: console, status, steps
class Result {
  public:
    py::str console;
    int64_t status;   // 0: the program ended; -1: the instruction limit stopped it
    int64_t steps;
    Result(const py::str& console_, int64_t status_, int64_t steps_) : console(console_), status(status_), steps(steps_) {}
};

// program: host path of the ISIS object file; tail: the command line after the program name; drives: {n: the host
// directory of :Fn:}
inline Result run(const py::str& program, const py::str& tail, const py::dict<int64_t, py::str>& drives, bool echo = false,
                  int64_t limit = 2000000000, std::ostream& out = std::cout) {
    using cb::compat::from_py;
    using cb::compat::int_from_py;
    namespace isis = cb::exact::emu::isis;
    return cb::compat::guard([&] {
        isis::Drives host_drives;
        for (const auto& kv : drives.items()) host_drives[int_from_py<int>(std::get<0>(kv))] = cb::fs::from_text(from_py(std::get<1>(kv)));
        isis::Options options;
        options.limit = int_from_py<std::uint64_t>(limit);
        if (echo) options.echo = [&out](std::string_view text) { out << text; };
        const isis::Result r = isis::run(cb::fs::from_text(from_py(program)), from_py(tail), host_drives, options);
        return Result(cb::compat::to_py(r.console), r.finished ? 0 : -1, cb::compat::to_py(r.steps));
    });
}

}  // namespace codebreaker::exact::emu::isis
