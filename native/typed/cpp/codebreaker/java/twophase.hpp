// codebreaker/java/twophase.hpp -- SHIM over cb/java/twophase.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The classes the pipeline holds are the old IR; the shim hands the new code a copy of each (the analysis only
// reads them).
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/twophase.hpp"
#include "codebreaker/java/ir.hpp"
#include "py.hpp"

namespace codebreaker::java::twophase {

namespace cbt = cb::java::twophase;

// A parsed class (codebreaker.java.parse's result).
using ClassInfo = py::ref<ir::JClass>;

// The record run() returns ({'sites': int, 'files': [str]}).
struct RunResult {
    int64_t sites = 0;
    py::list<py::str> files;
};

namespace shim_ {
inline cbt::NameSet names(const py::set<py::str>& s) {
    cbt::NameSet out;
    for (const py::str& n : s) out.insert(cb::compat::from_py(n));
    return out;
}
}  // namespace shim_

// The classes to build in two phases (see cb/java/twophase.hpp). `values` is the set of value-type names.
inline py::set<py::str> compute(const py::list<ClassInfo>& classes, const py::set<py::str>& values = py::set<py::str>()) {
    return cb::compat::guard([&] {
        std::vector<cb::java::ir::JClass> converted;
        converted.reserve(static_cast<std::size_t>(classes.size()));
        for (const ClassInfo& c : classes) converted.push_back(ir::to_new(*c));
        py::set<py::str> out;
        for (const std::string& n : cbt::compute(converted, shim_::names(values))) out.add(cb::compat::to_py(n));
        return out;
    });
}

// `new C(args)` -> `(new C())->j2xInit(args)` for C in names. -> (text, how many).
inline std::tuple<py::str, int64_t> rewrite_news(const py::str& text, const py::set<py::str>& names) {
    return cb::compat::guard([&] {
        const cbt::Rewritten r = cbt::rewrite_news(cb::compat::from_py(text), shim_::names(names));
        return std::make_tuple(cb::compat::to_py(r.text), static_cast<int64_t>(r.count));
    });
}

// A stack object `C tmp(args);` / `C tmp;` -> `C tmp; tmp.j2xInit(args);`. -> (text, how many)
inline std::tuple<py::str, int64_t> rewrite_stack(const py::str& text, const py::set<py::str>& names) {
    return cb::compat::guard([&] {
        const cbt::Rewritten r = cbt::rewrite_stack(cb::compat::from_py(text), shim_::names(names));
        return std::make_tuple(cb::compat::to_py(r.text), static_cast<int64_t>(r.count));
    });
}

// The post-pass over the emitted tree: every `new C(...)` of a two-phase class.
inline RunResult run(const py::str& out_dir, const py::set<py::str>& names) {
    return cb::compat::guard([&] {
        const cbt::RunResult r = cbt::run(cb::fs::from_text(cb::compat::from_py(out_dir)), shim_::names(names));
        RunResult out;
        out.sites = static_cast<int64_t>(r.sites);
        out.files = cb::compat::to_py(r.files);
        return out;
    });
}

}  // namespace codebreaker::java::twophase
