// codebreaker/java/projects/minecraft/platform_coverage.hpp -- SHIM over
// cb/java/projects/minecraft/platform_coverage.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer
// (transpile and the Minecraft project package call write_report).
//
// The Python module's private helpers (_read, _provided_symbols, the regexes ...) are not carried over: nothing outside
// the module used them. The settings key xbox360_sdk is read here (through the sdk shim) for the SDK scan.
#pragma once

#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/projects/minecraft/platform_coverage.hpp"
#include "codebreaker/java/console/_shim.hpp"
#include "codebreaker/java/console/sdk.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::projects::minecraft::platform_coverage {

struct Subsystem {
    py::str key;
    py::str name;
    py::list<py::str> holders;
    py::list<py::str> types;
    py::list<py::str> reference;
    py::list<py::str> options;
    py::list<py::str> notes;
};

using SymbolUse = std::tuple<py::str, int64_t, py::str>;   // (symbol, uses, "REUSABLE" | "STUB" | "MISSING")
using TypeUse = std::tuple<py::str, int64_t, bool>;

struct Coverage {
    Subsystem sub;
    py::list<SymbolUse> symbols;
    py::list<TypeUse> types;
    bool ref_exists = false;
    py::list<SymbolUse> missing;
    py::list<SymbolUse> stub;
    py::list<SymbolUse> reusable;
};

namespace shim_ {
namespace cbp = cb::java::projects::minecraft::platform_coverage;

inline const char* status_text(cbp::Status s) {
    return s == cbp::Status::Reusable ? "REUSABLE" : s == cbp::Status::Stub ? "STUB" : "MISSING";
}

inline py::list<py::str> names(const std::vector<std::string_view>& v) {
    py::list<py::str> out;
    for (std::string_view s : v) out.append(cb::compat::to_py(s));
    return out;
}

inline Coverage to_old(const cbp::Coverage& c) {
    Coverage out;
    const cbp::Subsystem& s = *c.sub;
    out.sub = Subsystem{cb::compat::to_py(s.key), cb::compat::to_py(s.name), names(s.holders), names(s.types),
                        names(s.reference), names(s.options), names(s.notes)};
    for (const cbp::SymbolUse& u : c.symbols) {
        SymbolUse t{cb::compat::to_py(u.name), static_cast<int64_t>(u.count), py::str(status_text(u.status))};
        out.symbols.append(t);
        (u.status == cbp::Status::Missing ? out.missing : u.status == cbp::Status::Stub ? out.stub : out.reusable).append(t);
    }
    for (const cbp::TypeUse& t : c.types) out.types.append(TypeUse{cb::compat::to_py(t.name), static_cast<int64_t>(t.count), t.in_reference});
    out.ref_exists = c.ref_exists;
    return out;
}
}  // namespace shim_

inline py::list<Coverage> analyze(const py::str& output_dir, const std::optional<py::str>& reference_dir = std::nullopt,
                                  const std::optional<py::set<py::str>>& runtime_files = std::nullopt) {
    return cb::compat::guard([&] {
        std::set<std::string> runtime;
        if (runtime_files) runtime = cb::compat::from_py(*runtime_files);
        py::list<Coverage> out;
        for (const auto& c : shim_::cbp::analyze(console::shim::path_of(output_dir), console::shim::given(reference_dir), runtime)) {
            out.append(shim_::to_old(c));
        }
        return out;
    });
}

inline std::tuple<py::str, py::list<Coverage>> write_report(const py::str& output_dir,
                                                            const std::optional<py::str>& reference_dir = std::nullopt,
                                                            const std::optional<py::set<py::str>>& runtime_files = std::nullopt,
                                                            const std::optional<py::str>& sdk_root = std::nullopt) {
    return cb::compat::guard([&] {
        shim_::cbp::WriteOptions options;
        options.reference_dir = console::shim::given(reference_dir);
        if (runtime_files) options.runtime_files = cb::compat::from_py(*runtime_files);
        options.sdk_root = console::shim::given(sdk_root);
        options.sdk_hints = console::sdk::detail_::hints_for(py::str("xbox360"), std::nullopt);
        const auto written = shim_::cbp::write_report(console::shim::path_of(output_dir), options);
        py::list<Coverage> results;
        for (const auto& c : written.results) results.append(shim_::to_old(c));
        return std::make_tuple(console::shim::native(written.path), results);
    });
}

}  // namespace codebreaker::java::projects::minecraft::platform_coverage
