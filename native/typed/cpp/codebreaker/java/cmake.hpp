// codebreaker/java/cmake.hpp -- C++ from codebreaker.java.cmake.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::cmake {

// the CMakeLists.txt template; `{name}` is the executable's name, `{{` and `}}` are literal braces
extern const py::str _TEMPLATE;

// Write out_dir/CMakeLists.txt for an executable called `name`. -> its path
py::str write_cmakelists(const py::str& out_dir, const py::str& name);

}  // namespace codebreaker::java::cmake
