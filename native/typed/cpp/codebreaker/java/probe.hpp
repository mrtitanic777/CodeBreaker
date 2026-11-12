// codebreaker/java/probe.hpp -- SHIM over cb/java/probe.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumers
// (verify/lanes/java, java/transpile, translate/javajudge, translate/dotnet/judge).
//
// What the old callers reach is kept, forwarding to cb::java::probe:
//   * the settings the Python read at module level are built per call: the libraries (`java_libs`), the stand-ins' folder
//     (codebreaker/java/probe_shims in the checkout) and CXX_STD, the dialect the C++17 back end's caller reassigns;
//   * the Python's parsed-class cache is a cache of source trees by folder (COMPAT: the new tree is an object the caller holds),
//     emptied for a folder when stage_sources rewrites it;
//   * plan()'s answer is a Todo that owns the tree its classes point into;
//   * a class handed to run() is looked up by name in the staged tree (the old and new IR are separate types).
// What nothing old called (the harness builders, probeable, fields, the tuples' tagged forms) is not carried over.
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <tuple>
#include <vector>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/probe.hpp"
#include "codebreaker/java/ir.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::probe {

namespace cbp = cb::java::probe;

using ClassRef = py::ref<ir::JClass>;

inline constexpr int64_t REV = cbp::kRevision;
inline constexpr int64_t TIMEOUT = 60;
inline py::str CXX_STD = py::str("c++03");   // the dialect the program and harnesses compile as (c++17 for the C++17 back end)

namespace shim_detail {

inline std::filesystem::path path_of(const py::str& s) { return cb::fs::from_text(cb::compat::from_py(s)); }

inline std::vector<std::filesystem::path> library_jars() {
    const py::str folder = py::settings::get_str(py::str("java_libs"));
    return folder.empty() ? std::vector<std::filesystem::path>{} : cbp::list_libraries(path_of(folder));
}

inline cbp::Toolchain toolchain(const py::str& cxx, const py::str& javac, const py::str& java, const std::optional<py::str>& prefix) {
    cbp::Toolchain t;
    t.cxx = path_of(cxx);
    t.javac = path_of(javac);
    t.java = path_of(java);
    t.cxx_std = cb::compat::from_py(CXX_STD);
    if (prefix && !prefix->empty()) t.prefix = path_of(*prefix);
    t.libraries = library_jars();
    t.shim_sources = path_of(py::settings::checkout_root()) / "codebreaker" / "java" / "probe_shims";
    return t;
}

// The parsed-class cache of the Python module, by source folder.
inline std::mutex trees_lock;
inline std::map<std::string, std::shared_ptr<cbp::SourceTree>>& trees() {
    static std::map<std::string, std::shared_ptr<cbp::SourceTree>> cache;
    return cache;
}
inline std::shared_ptr<cbp::SourceTree> tree_of(const py::str& dir) {
    std::lock_guard lock(trees_lock);
    auto& slot = trees()[cb::compat::from_py(dir)];
    if (!slot) slot = std::make_shared<cbp::SourceTree>(path_of(dir));
    return slot;
}

}  // namespace shim_detail

// The jars the game is compiled and run against, sorted; [] when none is set.
inline py::list<py::str> libraries() {
    py::list<py::str> out;
    for (const auto& jar : shim_detail::library_jars()) out.append(cb::compat::to_py(cb::fs::to_text(jar)));
    return out;
}

// The stand-in classes' folder, or '' when there are none.
inline py::str shims(const py::str& work, const py::str& javac) {
    return cb::compat::guard([&] {
        const auto folder = cbp::compile_shims(shim_detail::path_of(work), shim_detail::toolchain(py::str(), javac, py::str(), std::nullopt));
        return folder ? cb::compat::to_py(cb::fs::to_text(*folder)) : py::str();
    });
}

inline py::list<py::list<py::str>> cases(const py::list<py::str>& types) {
    return cb::compat::guard([&] { return cb::compat::to_py(cbp::cases(cb::compat::from_py(types))); });
}
inline py::str _java_literal(const py::str& t, const py::str& v) { return cb::compat::to_py(cbp::java_literal(cb::compat::from_py(t), cb::compat::from_py(v))); }
inline py::dict<py::str, py::str> _results(const py::str& text) { return cb::compat::to_py(cbp::parse_results(cb::compat::from_py(text))); }
inline bool same(const py::str& rt, const py::str& a, const py::str& b) {
    return cbp::same(cb::compat::from_py(rt), cb::compat::from_py(a), cb::compat::from_py(b));
}

// The module's .java files laid out by package. -> dest
inline py::str stage_sources(const py::str& java_dir, const py::str& dest) {
    return cb::compat::guard([&] {
        cbp::stage_sources(shim_detail::path_of(java_dir), shim_detail::path_of(dest));
        std::lock_guard lock(shim_detail::trees_lock);
        shim_detail::trees().erase(cb::compat::from_py(dest));
        return dest;
    });
}

// The whole emitted program as one archive ('' if it cannot be made).
inline py::str library(const py::str& cxx, const py::str& cpp_dir, const py::str& work, const std::optional<py::str>& prefix = std::nullopt,
                       const std::optional<int64_t>& jobs = std::nullopt) {
    return cb::compat::guard([&] {
        const auto lib = cbp::library(shim_detail::toolchain(cxx, py::str(), py::str(), prefix), shim_detail::path_of(cpp_dir), shim_detail::path_of(work),
                                      jobs ? static_cast<std::size_t>(*jobs) : 0);
        return lib ? cb::compat::to_py(cb::fs::to_text(*lib)) : py::str();
    });
}

// Compile the harness and link it against the program's archive. -> (ok, why, [classes linked])
inline std::tuple<bool, py::str, py::list<py::str>> build_native(const py::str& cxx, const py::str& cpp_dir, const py::str& harness, const py::str& exe,
                                                                 const py::str& lib, const std::optional<py::str>& prefix = std::nullopt) {
    return cb::compat::guard([&] {
        const cbp::NativeBuild built = cbp::build_native(shim_detail::toolchain(cxx, py::str(), py::str(), prefix), shim_detail::path_of(cpp_dir),
                                                         shim_detail::path_of(harness), shim_detail::path_of(exe), shim_detail::path_of(lib));
        return std::make_tuple(built.ok, cb::compat::to_py(built.why), cb::compat::to_py(built.linked));
    });
}

// {class name: (classes folder, harness class to run) or ('', why)}
using Compiled = py::dict<py::str, std::tuple<py::str, py::str>>;

// The classes that have something to run; it owns the source tree they were found in.
struct Todo {
    std::shared_ptr<cbp::SourceTree> tree;
    std::vector<cbp::ClassPlan> plans;
};

inline Todo plan(const py::str& java_dir, const py::str& cpp_dir, const py::list<py::str>& names) {
    return cb::compat::guard([&] {
        Todo todo;
        todo.tree = shim_detail::tree_of(java_dir);
        todo.plans = cbp::plan(*todo.tree, shim_detail::path_of(cpp_dir), cb::compat::from_py(names));
        return todo;
    });
}

// The Java harnesses of `todo`, compiled in batches.
inline Compiled compile_java(const py::str& java_dir, const Todo& todo, const py::str& work, const py::str& javac, int64_t size = 40,
                             const std::optional<int64_t>& jobs = std::nullopt, const py::str& tag = py::str("batch")) {
    return cb::compat::guard([&] {
        (void)java_dir;   // (the Todo's tree is the staged folder)
        Compiled out;
        const cbp::Compiled done = cbp::compile_java(*todo.tree, todo.plans, shim_detail::path_of(work), shim_detail::toolchain(py::str(), javac, py::str(), std::nullopt),
                                                     static_cast<std::size_t>(size), jobs ? static_cast<std::size_t>(*jobs) : 0, cb::compat::from_py(tag));
        for (const auto& [name, h] : done) {
            out.set(cb::compat::to_py(name), h.ok() ? std::make_tuple(cb::compat::to_py(cb::fs::to_text(h.classes_dir)), cb::compat::to_py(h.main_class))
                                                    : std::make_tuple(py::str(), cb::compat::to_py(h.why)));
        }
        return out;
    });
}

struct RunResult {
    py::list<py::str> methods;
    int64_t judged = 0;
    int64_t total = 0;
    int64_t cases = 0;
    py::list<std::tuple<py::str, py::list<py::str>, py::str, py::str>> differs;   // (method, input, java, cpp)
    py::str why;
    int64_t unsteady = 0;
};

// Judge one class (found by name in the staged tree).
inline RunResult run(const py::str& java_dir, const py::str& cpp_dir, const ClassRef& cls, const py::str& work, const py::str& cxx, const py::str& javac,
                     const py::str& java, const std::optional<py::str>& prefix = std::nullopt, const std::optional<Compiled>& compiled = std::nullopt) {
    return cb::compat::guard([&] {
        const auto tree = shim_detail::tree_of(java_dir);
        const cb::java::ir::JClass* found = tree->lookup(cb::compat::from_py(cls->name));
        RunResult out;
        if (found == nullptr) {
            out.why = py::str("the class is not in the staged sources");
            return out;
        }
        cbp::Compiled batch;
        if (compiled) {
            for (const auto& [name, folder_and_main] : compiled->items()) {
                const py::str folder = std::get<0>(folder_and_main), main = std::get<1>(folder_and_main);
                cbp::CompiledHarness h;
                if (folder.empty()) h.why = cb::compat::from_py(main);
                else {
                    h.classes_dir = shim_detail::path_of(folder);
                    h.main_class = cb::compat::from_py(main);
                }
                batch[cb::compat::from_py(name)] = std::move(h);
            }
        }
        const cbp::RunResult r = cbp::run(*tree, shim_detail::path_of(cpp_dir), *found, shim_detail::path_of(work), shim_detail::toolchain(cxx, javac, java, prefix),
                                          compiled ? &batch : nullptr);
        out.methods = cb::compat::to_py(r.methods);
        out.judged = static_cast<int64_t>(r.judged);
        out.total = static_cast<int64_t>(r.total);
        out.cases = static_cast<int64_t>(r.cases);
        for (const cbp::Difference& d : r.differs) {
            out.differs.append(std::make_tuple(cb::compat::to_py(d.method), cb::compat::to_py(d.inputs), cb::compat::to_py(d.java), cb::compat::to_py(d.native)));
        }
        out.why = cb::compat::to_py(r.why);
        out.unsteady = static_cast<int64_t>(r.unsteady);
        return out;
    });
}

}  // namespace codebreaker::java::probe
