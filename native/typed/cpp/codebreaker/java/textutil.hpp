// codebreaker/java/textutil.hpp -- SHIM over cb/java/textutil.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
#pragma once

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/textutil.hpp"
#include "py_os.hpp"

namespace codebreaker::java::textutil {

// A string-or-char literal (with backslash escapes): the pattern the old emit passes still scan with.
inline const py::re::Pattern& STR_LITERAL() {
    static const py::re::Pattern pattern = py::re::compile(py::str(R"PAT("(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')PAT"));
    return pattern;
}

// errors == "strict" is the one mode the new reader has besides replacing; anything else replaces.
inline py::str read_text(const py::str& path, const py::str& errors = py::str("replace"), bool missing_ok = true) {
    return cb::compat::guard([&] {
        namespace tu = cb::java::textutil;
        const auto mode = errors == py::str("strict") ? tu::Decode::Strict : tu::Decode::Replace;
        const auto p = cb::fs::from_text(cb::compat::from_py(path));
        if (!missing_ok) return cb::compat::to_py(tu::read_text(p, mode));
        return cb::compat::to_py(tu::try_read_text(p, mode).value_or(std::string()));
    });
}

inline void write_text(const py::str& path, const py::str& text) {
    cb::compat::guard([&] {
        cb::java::textutil::write_text(cb::fs::from_text(cb::compat::from_py(path)), cb::compat::from_py(text));
    });
}

}  // namespace codebreaker::java::textutil
