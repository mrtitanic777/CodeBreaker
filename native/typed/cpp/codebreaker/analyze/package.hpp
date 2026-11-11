// codebreaker/analyze/package.hpp -- COMPAT SHIM over cb/analyze/package.hpp (docs/CPP_DESIGN.md 6.3), for the old
// codebreaker/analyze/__init__.py port. Deleted with its last py:: consumer.
//
// What can be worked out ABOUT a module, beside decompiling it: the overview text lives in native/cb/analyze/package.hpp.
// DOC is what `codebreaker.analyze.__doc__` said, here the overview as a py::str.
#pragma once

#include "cb/analyze/package.hpp"
#include "cb/compat/py_bridge.hpp"
#include "py.hpp"

namespace codebreaker::analyze {

inline const py::str DOC = ::cb::compat::to_py(::cb::analyze::overview());

}  // namespace codebreaker::analyze
