// codebreaker/java/console/vcxproj.hpp -- SHIM over cb/java/console/vcxproj.hpp (docs/CPP_DESIGN.md 6.3); deleted with
// its last py:: consumer (run, transpile, project_layout).
//
// The old module held the active profile's folder rules in a module variable that profiles.on_change reseeded. The
// new module takes them as a value, so the shim keeps that one variable, for the old callers: _reseed() replaces it,
// run() hands it down.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "codebreaker/java/console/_shim.hpp"
#include "cb/java/console/vcxproj.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::console::vcxproj {

// The rules of the active profile (compiled), as the Python's _FILTER_RULES.
inline cb::java::console::vcxproj::FilterRules& active_rules_() {
    static cb::java::console::vcxproj::FilterRules rules;
    return rules;
}

// Re-derive the .filters classification rules when the active target profile changes (profiles.on_change).
// `vcxproj_filters` is the new profile's.
inline void _reseed(const py::list<std::tuple<py::str, py::str>>& vcxproj_filters) {
    cb::compat::guard([&] {
        std::vector<cb::java::console::vcxproj::FilterRule> rules;
        for (const auto& [pattern, filter] : vcxproj_filters) rules.push_back({pattern.utf8(), filter.utf8()});
        active_rules_() = cb::java::console::vcxproj::FilterRules(rules);
    });
}

// Regenerate the .vcxproj ClCompile/ClInclude file list from the .cpp/.h in proj_dir, and (by default) also
// (re)write the subsystem <name>.vcxproj.filters. `proj_dir` defaults to the current directory. Prints the status.
inline void run(const std::optional<py::str>& proj_dir = std::nullopt, const std::optional<py::str>& name = std::nullopt,
                bool write_filters_too = true) {
    cb::compat::guard([&] {
        namespace nw = cb::java::console::vcxproj;
        nw::Options options;
        if (name && name->size() > 0) options.name = name->utf8();
        options.write_filters = write_filters_too;
        const std::filesystem::path dir = proj_dir && proj_dir->size() > 0 ? shim::path_of(*proj_dir) : std::filesystem::current_path();
        py::print(cb::compat::to_py(nw::run(dir, active_rules_(), options).summary()));
    });
}

}  // namespace codebreaker::java::console::vcxproj
