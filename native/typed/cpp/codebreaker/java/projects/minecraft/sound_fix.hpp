// codebreaker/java/projects/minecraft/sound_fix.hpp -- SHIM over cb/java/projects/minecraft/sound_fix.hpp
// (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// The text form (process, run, stranded) forwards to the new module. patch() works on the OLD translate IR (py::ref
// nodes), which the new module cannot see; it stays in sound_fix.cpp as a legacy form until translate/ir and
// translate/rewrite are refactored (COMPAT(py): remove then).
#pragma once

#include <cstdint>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/sound_fix.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/translate/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::sound_fix {

struct Touched {
    py::str file;
    int64_t sfx = 0;
    int64_t music = 0;
};

struct Result {
    py::list<Touched> files;
    int64_t sites = 0;
    py::list<py::str> stranded;
};

namespace shim_ {
namespace cbs = cb::java::projects::minecraft::sound_fix;
inline Result to_old(const cbs::Report& r) {
    Result out;
    for (const auto& f : r.files)
        out.files.append(Touched{cb::compat::to_py(f.file), static_cast<int64_t>(f.sfx), static_cast<int64_t>(f.music)});
    out.sites = static_cast<int64_t>(r.sites);
    out.stranded = cb::compat::to_py(r.stranded);
    return out;
}
}  // namespace shim_

inline std::tuple<py::str, int64_t, int64_t> process(const py::str& text) {
    const auto r = shim_::cbs::rewrite(cb::compat::from_py(text));
    return {cb::compat::to_py(r.text), static_cast<int64_t>(r.sfx), static_cast<int64_t>(r.music)};
}

inline Result run(const py::str& out_dir) {
    return cb::compat::guard([&] { return shim_::to_old(shim_::cbs::run(console::shim::path_of(out_dir))); });
}

inline py::list<py::str> stranded(const py::str& out_dir) {
    return cb::compat::guard([&] { return cb::compat::to_py(shim_::cbs::stranded(console::shim::path_of(out_dir))); });
}

// The tree-shape pass over the old IR (legacy: see the file comment).
Result patch(const py::list<codebreaker::translate::ir::NodeP>& units);

}  // namespace codebreaker::java::projects::minecraft::sound_fix
