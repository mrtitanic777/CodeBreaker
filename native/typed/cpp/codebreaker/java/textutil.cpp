// codebreaker/java/textutil.cpp -- C++ port of codebreaker.java.textutil.
//
// Small shared text/IO helpers used across the transpiler.
//
// NOTE ON "DUPLICATION": several modules define their own `_split_top_commas`, `_balanced_paren`,
// `sub_outside_literals`, etc. Those look like duplicates but are deliberately *context-tuned* and
// are NOT interchangeable -- e.g. `parse._split_top_commas` tracks `<>` while
// the old `emit.statements._split_top_commas` intentionally did not (it would mis-balance `->`) and instead
// tracks `{}` and backslash escapes. They are left in place on purpose. Only the genuinely-identical
// utilities below are centralized here.

#include "codebreaker/java/textutil.hpp"

namespace codebreaker::java::textutil {

const py::re::Pattern& STR_LITERAL() {
    static const py::re::Pattern pattern =
        py::re::compile(py::str(R"PAT("(?:\\.|[^"\\])*"|'(?:\\.|[^'\\])*')PAT"));
    return pattern;
}

// Read a UTF-8 text file.
//
// errors    -- passed to open() ('replace' tolerates undecodable bytes; 'strict' raises).
// missing_ok -- when True, an IOError/OSError (e.g. missing file) yields '' instead of raising.
py::str read_text(const py::str& path, const py::str& errors, bool missing_ok) {
    try {
        py::file f = py::open(path, py::str("r"), py::str("utf-8"), errors);
        py::str text = f.read_text();
        f.close();
        return text;
    } catch (const py::OSError&) {   // (IOError is OSError)
        if (missing_ok) {
            return py::str("");
        }
        throw;
    }
}

// Write a UTF-8 text file, overwriting.
void write_text(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"));
    f.write(text);
    f.close();
}

}  // namespace codebreaker::java::textutil
