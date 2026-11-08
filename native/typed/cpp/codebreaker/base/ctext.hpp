// codebreaker/base/ctext.hpp -- C++ from codebreaker.base.ctext.
#pragma once

#include <functional>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::base::ctext {

// the three patterns of comments, strings and character constants (module constants of the Python)
extern const py::re::Pattern _OPEN;
extern const py::re::Pattern _CLOSED;
extern const py::re::Pattern _ACROSS;

py::str _inside(const py::re::Match& m);

py::str blank(const py::str& text, bool whole = false, bool across_lines = false);

py::str sub_code(const py::str& text, const py::re::Pattern& rx, const std::function<py::str(const py::re::Match&)>& fn);

}  // namespace codebreaker::base::ctext
