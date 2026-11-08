// codebreaker/java/textutil.hpp -- C++ port of codebreaker.java.textutil.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::textutil {

// A string-or-char literal (with backslash escapes). Used to mask literals before applying regex
// substitutions to code. Identical across the emit back-end.
// (The module-level re.compile(...), made on first use.)
const py::re::Pattern& STR_LITERAL();

py::str read_text(const py::str& path, const py::str& errors = py::str("replace"), bool missing_ok = true);
void write_text(const py::str& path, const py::str& text);

}  // namespace codebreaker::java::textutil
