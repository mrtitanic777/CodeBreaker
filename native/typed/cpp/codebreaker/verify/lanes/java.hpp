// codebreaker/verify/lanes/java.hpp -- COMPAT SHIM over cb/verify/lanes/java.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/verify/lanes/java.{hpp,cpp}: the Java kind (JavaLane), given the tools and the transpiler it works with.
// This header keeps what not-yet-refactored ports still ask of the old module by its old names: the kind's constants, the
// module's questions (is_module, source_dir, sha) and where the host's tools are (jdk, compiler -- read from the settings, which is
// the old layer's). The old prepare/build/judge are not kept: their one caller, translate/javajudge, is JavaLane's now.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/verify/lanes/java.hpp"
#include "codebreaker/decompile/jar.hpp"
#include "py.hpp"
#include "py_os.hpp"
#include "py_settings.hpp"
#include "py_subprocess.hpp"

namespace codebreaker::verify::lanes::java {

namespace newlane = cb::verify::lanes;

inline const py::str KIND = py::str("java");
inline constexpr int64_t REV = newlane::JavaLane::kRev;      // bump when what a verdict means changes
inline const py::str SOURCE_REV = py::str("java-3");         // who made the candidates: the transpiler, not the native decompiler
inline constexpr bool LIBRARY = false;
inline constexpr bool PER_FUNCTION = true;
inline const py::str STD = py::str("c++03");                 // the dialect the lane judges in when none is asked for
inline const py::str CPP_STD = py::str("C++03");
inline const std::optional<py::str> PLATFORM = std::nullopt;   // no platform: the generic stdafx.h, nothing a host compiler lacks
inline const py::list<py::str> FLAGS = {py::str("-std=c++03"), py::str("-fsyntax-only"), py::str("-w")};

// A folder holding .java files (at any depth), or a single .java file.
inline bool is_module(const py::str& path) { return newlane::JavaLane::is_module(cb::fs::from_text(path.utf8())); }
// The folder the source is in.
inline py::str source_dir(const py::str& path) { return cb::compat::to_py(cb::fs::to_text(newlane::JavaLane::source_dir(cb::fs::from_text(path.utf8())))); }
// What results are filed under: the .java files' names and contents, in a fixed order.
inline py::str sha(const py::str& path) { return cb::compat::to_py(newlane::JavaLane::module_sha(cb::fs::from_text(path.utf8()))); }

// (javac, java) -- the JDK the behavioural judge runs the original with -- or ("", "").
inline std::tuple<py::str, py::str> jdk() {
    std::optional<py::str> found = codebreaker::decompile::jar::find_java();
    py::str java = found ? *found : py::str();
    py::str javac;
    if (!java.empty()) {
        py::str sibling = py::os::path::join(py::os::path::dirname(java), py::str("javac") + py::str(".exe"));
        if (py::os::path::isfile(sibling)) {
            javac = sibling;
        } else {
            std::optional<py::str> w = py::shutil::which(py::str("javac"));
            javac = w ? *w : py::str();
        }
    }
    if (!javac.empty()) return {javac, java};
    return {py::str(), py::str()};
}

// The host C++ compiler (settings.json: host_cxx, else g++ on PATH), or "" when there is none.
inline py::str compiler() {
    py::str cxx = py::settings::get_str(py::str("host_cxx"));
    if (!cxx.empty() && py::os::path::isfile(cxx)) return cxx;
    std::optional<py::str> w = py::shutil::which(py::str("g++"));
    return w ? *w : py::str();
}

}  // namespace codebreaker::verify::lanes::java
