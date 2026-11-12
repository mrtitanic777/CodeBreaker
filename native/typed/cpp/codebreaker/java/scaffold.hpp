// codebreaker/java/scaffold.hpp -- SHIM over cb/java/scaffold.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// Only what the `scaffold` command calls is here: run() and the language labels. The per-language emitters are the
// new module's own now.
#pragma once

#include <cstdint>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/java/scaffold.hpp"
#include "py.hpp"

namespace codebreaker::java::scaffold {

inline const py::dict<py::str, py::str> LANG_LABEL = [] {
    py::dict<py::str, py::str> d;
    d.set(py::str("c"), py::str("C"));
    d.set(py::str("csharp"), py::str("C#"));
    d.set(py::str("rust"), py::str("Rust"));
    return d;
}();

struct Result {
    py::list<py::str> written;
    int64_t classes = 0;
    py::str language;
};

// Parse the Java tree and emit one skeleton file per class in `language` (c|csharp|rust). ValueError for a language
// with no emitter.
inline Result run(const py::str& java_dir, const py::str& out_dir, const py::str& language) {
    return cb::compat::guard([&] {
        namespace cbs = cb::java::scaffold;
        const auto lang = cbs::parse_language(cb::compat::from_py(language));
        if (!lang) throw py::ValueError("no scaffold emitter for language '" + cb::compat::from_py(language) + "'");
        const cbs::Scaffold made = cbs::write(cb::fs::from_text(cb::compat::from_py(java_dir)),
                                              cb::fs::from_text(cb::compat::from_py(out_dir)), *lang);
        Result r;
        r.written = cb::compat::to_py(made.written);
        r.classes = static_cast<int64_t>(made.written.size());
        r.language = language;
        return r;
    });
}

}  // namespace codebreaker::java::scaffold
