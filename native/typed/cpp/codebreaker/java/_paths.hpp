// codebreaker/java/_paths.hpp -- C++ port of codebreaker.java._paths.
#pragma once

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::_paths {

// _HERE = os.path.dirname(os.path.abspath(__file__)): the folder of the Java package, whose data
// (platform/, api_maps/) sits beside the code. C++ has no __file__, so the module holds it as a
// variable: it starts as the folder of this source file's own location and the host sets it to the
// package folder with set_here() (the Python module's value is fixed by where _paths.py lives).
py::str _HERE();
void set_here(const py::str& here);

// Absolute path to a data file or folder shipped with the package.
// (data_path(*parts): the parts are passed as a list.)
py::str data_path(const py::list<py::str>& parts);

}  // namespace codebreaker::java::_paths
