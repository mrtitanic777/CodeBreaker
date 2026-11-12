// codebreaker/java/analyze.hpp -- SHIM over cb/java/analyze.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// Only analyze_all is here (the `analyze` command and its counts record). The new analyzers add their events to a
// new ledger, which the shim appends to the old one; `converter_lint` is an optional external module that is not
// bundled, so the `lint` count is always 0, as the Python's ImportError path gave.
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/analyze.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/rules.hpp"
#include "py.hpp"

namespace codebreaker::java::analyze {

// What analyze_all() gives: the Python's dict of counts, with the log used as '_build_log_path'.
struct AnalyzeCounts {
    int64_t markers = 0;
    int64_t rules = 0;
    int64_t lint = 0;
    int64_t corrupt_strings = 0;
    int64_t build_log = 0;
    std::optional<py::str> build_log_path;   // '_build_log_path'
};
template <class F> void for_each_field(const AnalyzeCounts& r, F&& f) {
    f("markers", r.markers);
    f("rules", r.rules);
    f("lint", r.lint);
    f("corrupt_strings", r.corrupt_strings);
    f("build_log", r.build_log);
    f("_build_log_path", r.build_log_path);
}

// Run every available analyzer against the artifacts and return per-source counts.
inline AnalyzeCounts analyze_all(ledger::Ledger& led, const py::str& output_dir, const std::optional<py::str>& build_log = std::nullopt,
                                 const std::optional<py::str>& root = std::nullopt, const std::optional<py::str>& platform = std::nullopt) {
    return cb::compat::guard([&] {
        namespace cba = cb::java::analyze;
        auto path = [](const std::optional<py::str>& s) -> std::optional<std::filesystem::path> {
            if (!s || s->empty()) return std::nullopt;   // (the Python's `x or default`: an empty string is not given)
            return cb::fs::from_text(cb::compat::from_py(*s));
        };
        const cb::java::profiles::Profile profile = rules::current_profile_();
        cba::Options options;
        options.build_log = path(build_log);
        options.root = path(root);
        if (platform && !platform->empty()) options.platform = cb::compat::from_py(*platform);
        options.profile = &profile;
        cb::java::ledger::Ledger found;
        const cba::Counts c = cba::analyze_all(found, cb::fs::from_text(cb::compat::from_py(output_dir)), options);
        for (const cb::java::ledger::Event& e : found.events()) led.events.append(ledger::from_new(e));
        AnalyzeCounts out;
        out.markers = static_cast<int64_t>(c.markers);
        out.rules = static_cast<int64_t>(c.rules);
        out.corrupt_strings = static_cast<int64_t>(c.corrupt_strings);
        out.build_log = static_cast<int64_t>(c.build_log);
        if (c.build_log_path) out.build_log_path = cb::compat::to_py(cb::fs::to_text(*c.build_log_path));
        return out;
    });
}

}  // namespace codebreaker::java::analyze
