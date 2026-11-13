// codebreaker/exact/bss.hpp -- COMPAT SHIM over cb/exact/bss.hpp (docs/CPP_DESIGN.md 6.3), for the old layer's callers (exact/link). Deleted with its last py::
// consumer. Conversion and forwarding only: the proposal is native/cb/exact/bss.hpp, over the cb project the old Project holds (`impl_`).
//
// bss.py -- the program's variables that begin without a value, each at its place (src/BSS).
//
//     python -m codebreaker exact PROJECT bss [--write]
//
// They lie from the end of the file's data to the stack's segment: the program's own (the data places SYMBOLS learnt, and the sources' names for data it has no
// place for: g_XXXX after its place) and the library's (its modules' own _BSS segments, a "hole" the linker fills with them; its communal names).
//
//     src/BSS:  range LO HI / hole ADDR LEN / name ADDR      (hex)
//
// What differs from the port it replaces: main() prints to stdout (its `out` is not used).
#pragma once

#include <cstdint>
#include <iostream>
#include <tuple>

#include "cb/exact/bss.hpp"
#include "codebreaker/exact/project.hpp"
#include "py.hpp"

namespace codebreaker::exact::bss {

namespace nb = ::cb::exact::bss;

using Hole = std::tuple<int64_t, int64_t, py::list<py::str>>;

struct Proposal {
    int64_t edata = 0;                       // the range's start (DS offset)
    int64_t stack = 0;                       // and end
    py::dict<py::str, int64_t> names;        // {name: DS offset}
    py::list<Hole> holes;                    // a library module's own _BSS between them
};

namespace shim_detail {
inline nb::Proposal to_new(int64_t edata, int64_t stack, const py::dict<py::str, int64_t>& names, const py::list<Hole>& holes) {
    nb::Proposal p;
    p.edata = edata;
    p.stack = stack;
    for (const auto& [n, a] : names.items()) p.names[n.utf8()] = a;
    for (const Hole& h : holes) {
        nb::Hole out{std::get<0>(h), std::get<1>(h), {}};
        for (const py::str& m : std::get<2>(h)) out.modules.push_back(m.utf8());
        p.holes.push_back(std::move(out));
    }
    return p;
}
}  // namespace shim_detail

inline Proposal propose(project::Project& prj) {
    const nb::Proposal p = nb::propose(*prj.impl_);
    Proposal out;
    out.edata = p.edata;
    out.stack = p.stack;
    for (const auto& [n, a] : p.names) out.names.set(py::str(n), a);
    for (const nb::Hole& h : p.holes) {
        py::list<py::str> modules;
        for (const std::string& m : h.modules) modules.append(py::str(m));
        out.holes.append(std::make_tuple(h.address, h.length, modules));
    }
    return out;
}

inline py::list<py::str> lines(project::Project& prj, int64_t edata, int64_t stack, const py::dict<py::str, int64_t>& names, const py::list<Hole>& holes) {
    py::list<py::str> out;
    for (const std::string& l : nb::lines(prj.impl_->profile_parts()->communal, shim_detail::to_new(edata, stack, names, holes))) out.append(py::str(l));
    return out;
}

inline int64_t main(project::Project& prj, const py::list<py::str>& argv, std::ostream& = std::cout) {
    std::vector<std::string> args;
    for (const py::str& a : argv) args.push_back(a.utf8());
    return nb::run(*prj.impl_, args);
}

}  // namespace codebreaker::exact::bss
