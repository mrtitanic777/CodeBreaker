// codebreaker/java/console/worklist.hpp -- SHIM over cb/java/console/worklist.hpp (docs/CPP_DESIGN.md 6.3); deleted with
// its last py:: consumer (run).
#pragma once

#include <optional>

#include "codebreaker/java/console/_shim.hpp"
#include "cb/java/console/worklist.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"

namespace codebreaker::java::console::worklist {

// Compile every <out_dir>/*.cpp with `cc` (default: settings host_cxx, else g++), write the worklist to
// `worklist_path` (default WORKLIST.md) and print a one-line summary. `std_` is the Python's `std` (renamed: `std` is
// C++'s namespace). -> 0
inline int64_t run(const py::str& out_dir, const std::optional<py::str>& cc = std::nullopt,
                   const py::str& std_ = py::str("c++03"), const std::optional<py::str>& worklist_path = std::nullopt) {
    return cb::compat::guard([&]() -> int64_t {
        cb::java::console::worklist::Options options;
        if (cc && cc->size() > 0) options.compiler = cc->utf8();
        else if (const py::str host = py::settings::get_str(py::str("host_cxx")); host.size() > 0) options.compiler = host.utf8();
        options.standard = std_.utf8();
        const std::string line = cb::java::console::worklist::run(
            shim::path_of(out_dir), worklist_path && worklist_path->size() > 0 ? shim::path_of(*worklist_path) : std::filesystem::path("WORKLIST.md"),
            options);
        py::print(cb::compat::to_py(line));
        return 0;
    });
}

}  // namespace codebreaker::java::console::worklist
