// codebreaker/java/reflection_factory.hpp -- SHIM over cb/java/reflection_factory.hpp (docs/CPP_DESIGN.md 6.3); deleted
// with its last py:: consumer. Only what the callers use is carried: the result records and run(); the pass's private
// helpers (_split_top, _find_factory, _reuse ...) and its regex constants were internal.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/reflection_factory.hpp"

namespace codebreaker::java::reflection_factory {

namespace cbr = cb::java::reflection_factory;

struct Factory {
    py::str registry;
    py::str ret;
    py::list<py::str> args;
    py::list<py::str> classes;
};

struct RegistryResult {
    py::str file;
    int64_t sites;
    py::str registry;
    py::str ret;
    py::list<py::str> args;
    py::list<py::str> classes;
};

struct Reused {
    py::str file;
    int64_t sites;
};

struct RunResult {
    py::list<RegistryResult> registries;
    py::list<Reused> reused;
    int64_t total_sites;
    int64_t total_classes;
};

namespace shim_ {
// The old module kept the project's registrars in a module global that run() set and process() read.
inline cbr::Registrars& registrars() {
    static cbr::Registrars current;
    return current;
}
inline cbr::Registrars to_new(const std::optional<py::list<py::str>>& names) {
    cbr::Registrars r;
    if (names && names->size() > 0) r.names = cb::compat::from_py(*names);
    return r;
}
}  // namespace shim_

// Rewrite reflection sites in `text` (one .cpp, `registry` = its class name) to a generated factory.
inline std::tuple<py::str, int64_t, std::optional<Factory>> process(const py::str& text, const py::str& registry) {
    return cb::compat::guard([&] {
        cbr::ProcessResult r = cbr::process(cb::compat::from_py(text), cb::compat::from_py(registry), shim_::registrars());
        std::optional<Factory> f;
        if (r.factory) {
            f = Factory{cb::compat::to_py(r.factory->registry), cb::compat::to_py(r.factory->ret),
                        cb::compat::to_py(r.factory->args), cb::compat::to_py(r.factory->classes)};
        }
        return std::make_tuple(cb::compat::to_py(r.text), static_cast<int64_t>(r.sites), f);
    });
}

// Two passes: (1) generate a factory for each registry class from its registration table; (2) rewrite reflection
// sites in OTHER files to reuse those factories via ReflectionFactory.h.
inline RunResult run(const py::str& out_dir, const std::optional<py::list<py::str>>& registrars = std::nullopt) {
    return cb::compat::guard([&] {
        if (registrars && registrars->size() > 0) shim_::registrars() = shim_::to_new(registrars);   // (the global stuck once set)
        cbr::RunResult r = cbr::run(cb::fs::from_text(cb::compat::from_py(out_dir)), shim_::registrars());
        RunResult out;
        for (const cbr::RegistryResult& g : r.registries) {
            out.registries.append(RegistryResult{cb::compat::to_py(g.file), static_cast<int64_t>(g.sites),
                                                 cb::compat::to_py(g.factory.registry), cb::compat::to_py(g.factory.ret),
                                                 cb::compat::to_py(g.factory.args), cb::compat::to_py(g.factory.classes)});
        }
        for (const cbr::ReusedResult& u : r.reused) out.reused.append(Reused{cb::compat::to_py(u.file), static_cast<int64_t>(u.sites)});
        out.total_sites = static_cast<int64_t>(r.total_sites);
        out.total_classes = static_cast<int64_t>(r.total_classes);
        return out;
    });
}

}  // namespace codebreaker::java::reflection_factory
