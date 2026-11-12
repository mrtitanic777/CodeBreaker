// codebreaker/java/report.hpp -- SHIM over cb/java/report.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// Only what the converter and the `analyze` command call is here: write_reports. The page-building helpers of the
// Python (_bar, _safe, _write_index, _write_file_report) are the new module's own now.
#pragma once

#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/report.hpp"
#include "codebreaker/java/ledger.hpp"
#include "py.hpp"

namespace codebreaker::java::report {

// Render a Ledger into the report bundle; -> the report folder. `meta` holds the four keys the index reads
// (profile, output_dir, build_log, generated); a missing or empty value is not shown.
inline py::str write_reports(const ledger::Ledger& led, const py::str& output_dir,
                             const std::optional<py::str>& report_dir = std::nullopt,
                             const py::dict<py::str, py::str>& meta = py::dict<py::str, py::str>()) {
    return cb::compat::guard([&] {
        cb::java::report::Meta m;
        auto pick = [&](const char* key) {
            const std::optional<py::str> v = meta.get(py::str(key));
            return v ? cb::compat::from_py(*v) : std::string();
        };
        m.profile = pick("profile");
        m.output_dir = pick("output_dir");
        m.build_log = pick("build_log");
        m.generated = pick("generated");
        std::optional<std::filesystem::path> dir;
        if (report_dir && !report_dir->empty()) dir = cb::fs::from_text(cb::compat::from_py(*report_dir));
        const std::filesystem::path out = cb::java::report::write_reports(led.to_new(), cb::fs::from_text(cb::compat::from_py(output_dir)), dir, m);
        return cb::compat::to_py(cb::fs::to_text(out));
    });
}

}  // namespace codebreaker::java::report
