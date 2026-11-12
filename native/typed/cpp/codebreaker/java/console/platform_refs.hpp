// codebreaker/java/console/platform_refs.hpp -- SHIM over cb/java/console/platform_refs.hpp (docs/CPP_DESIGN.md 6.3);
// deleted with its last py:: consumer (transpile).
//
// The new module works on the new IR's JClass; the shim hands it each old class reduced to what the detection reads
// (names, flags, interfaces, method signatures -- no bodies). `emit_references_last_skipped` is the old module's
// "last call" state, kept here for the one caller that reads it.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/console/platform_refs.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::platform_refs {

namespace shim_ {
namespace cbr = cb::java::console::platform_refs;

// An old class as the detection sees it: everything but the field list and the method bodies.
inline cb::java::ir::JClass lite(const java::ir::JClass& c) {
    cb::java::ir::JClass out;
    out.name = cb::compat::from_py(c.name);
    if (c.parent) out.parent = cb::compat::from_py(*c.parent);
    out.interfaces = cb::compat::from_py(c.interfaces);
    out.is_interface = c.is_interface;
    out.is_abstract = c.is_abstract;
    for (const py::ref<java::ir::Method>& m : c.methods) {
        cb::java::ir::Method nm;
        nm.name = cb::compat::from_py(m->name);
        nm.is_static = m->is_static;
        nm.is_ctor = m->is_ctor;
        for (const py::ref<java::ir::Param>& p : m->params) {
            cb::java::ir::Param np;
            np.type = java::ir::to_new(*p->jtype);
            nm.params.push_back(std::move(np));
        }
        out.methods.push_back(std::move(nm));
    }
    return out;
}

inline std::vector<cb::java::ir::JClass> lite(const py::list<py::ref<java::ir::JClass>>& classes) {
    std::vector<cb::java::ir::JClass> out;
    for (const auto& c : classes) out.push_back(lite(*c));
    return out;
}
}  // namespace shim_

inline const py::dict<py::str, py::list<py::str>> REFERENCE_REPLACES = [] {
    py::dict<py::str, py::list<py::str>> d;
    for (const auto& [base, repl] : shim_::cbr::reference_replaces()) d.set(cb::compat::to_py(base), cb::compat::to_py(repl));
    return d;
}();

inline const py::set<py::str> REFERENCE_SKIP = [] {
    py::set<py::str> s;
    for (const auto& name : shim_::cbr::reference_skip()) s.add(cb::compat::to_py(name));
    return s;
}();

inline py::set<py::str> available_references(const py::str& reference_dir) {
    return cb::compat::to_py(shim_::cbr::available_references(console::shim::path_of(reference_dir)));
}

inline std::optional<py::str> detect_main_class(const py::list<py::ref<java::ir::JClass>>& classes) {
    const auto lite = shim_::lite(classes);
    return cb::compat::to_py(shim_::cbr::detect_main_class(lite));
}

inline py::str generate_entry_point(const py::str& main_class) {
    return cb::compat::to_py(shim_::cbr::generate_entry_point(cb::compat::from_py(main_class)));
}

inline py::str generate_pc_entry_point(const py::str& main_class) {
    return cb::compat::to_py(shim_::cbr::generate_pc_entry_point(cb::compat::from_py(main_class)));
}

// The old class, or nullptr when no class (or more than one) qualifies.
inline py::ref<java::ir::JClass> detect_runnable_game_class(const py::list<py::ref<java::ir::JClass>>& classes) {
    const auto lite = shim_::lite(classes);
    const cb::java::ir::JClass* found = shim_::cbr::detect_runnable_game_class(lite);
    if (found == nullptr) return nullptr;
    return classes[static_cast<int64_t>(found - lite.data())];
}

inline py::str _runnable_ctor_args(const py::ref<java::ir::JClass>& cls) {
    return cb::compat::to_py(shim_::cbr::runnable_ctor_args(shim_::lite(*cls)));
}

inline py::str generate_runnable_entry_point(const py::ref<java::ir::JClass>& cls) {
    return cb::compat::to_py(shim_::cbr::generate_runnable_entry_point(shim_::lite(*cls)));
}

struct AuditResult {
    py::set<py::str> complete;   // bases with a .reference.h
    py::list<py::str> orphans;   // sorted .cpp-only bases, excluding Main and *.ext
};

inline AuditResult audit_references(const py::str& reference_dir) {
    const auto a = shim_::cbr::audit_references(console::shim::path_of(reference_dir));
    return AuditResult{cb::compat::to_py(a.complete), cb::compat::to_py(a.orphans)};
}

inline py::list<py::str> verify_integrated(const py::str& out_dir, const py::set<py::str>& bases) {
    return cb::compat::to_py(shim_::cbr::verify_integrated(console::shim::path_of(out_dir), cb::compat::from_py(bases)));
}

// What the last emit_references call skipped (the game defines the name itself).
inline py::list<py::str> emit_references_last_skipped;

inline py::set<py::str> emit_references(const py::str& out_dir, const py::str& reference_dir,
                                        const py::set<py::str>& game_classes = py::set<py::str>(),
                                        const std::optional<py::str>& main_class = std::nullopt,
                                        const py::ref<java::ir::JClass>& runnable_game = nullptr,
                                        const py::str& platform = py::str("xbox360")) {
    return cb::compat::guard([&] {
        shim_::cbr::EmitOptions options;
        options.game_classes = cb::compat::from_py(game_classes);
        if (main_class) options.main_class = cb::compat::from_py(*main_class);
        const cb::java::ir::JClass runnable = runnable_game ? shim_::lite(*runnable_game) : cb::java::ir::JClass{};
        if (runnable_game) options.runnable_game = &runnable;
        options.platform = shim_::cbr::platform_of(cb::compat::from_py(platform));
        const auto r = shim_::cbr::emit_references(console::shim::path_of(out_dir), console::shim::path_of(reference_dir), options);
        emit_references_last_skipped = cb::compat::to_py(r.skipped_collisions);
        return cb::compat::to_py(r.bases);
    });
}

}  // namespace codebreaker::java::console::platform_refs
