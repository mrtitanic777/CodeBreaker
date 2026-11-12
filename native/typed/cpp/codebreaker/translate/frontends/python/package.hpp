// codebreaker/translate/frontends/python/package.hpp -- SHIM over cb/translate/frontends/python/package.hpp (docs/CPP_DESIGN.md 6.3): the Python front end,
// CPython's syntax tree into L2 typed by inference. The module is cb::translate::frontends::python::package now (`program_of_tree`,
// `program_of_sources`, `translate` and the `Translation` it returns).
//
// Nothing included the old port, so this shim carries no old types: it only gives the old namespace the new module's name. The old names map as:
// pyfe::translate / build_unit / used_types -> package::translate (one function: the units and what they need), pyfe::ModuleResult ->
// lower::ModuleResult, pyfe::Issue -> lower::Refusal, pyfe::Lowering and its Context, Records and FnLower -> the classes of lower.hpp.
#pragma once

#include "cb/translate/frontends/python/package.hpp"

namespace codebreaker::translate::frontends::python::pyfe {

namespace cb_package = ::cb::translate::frontends::python::package;

}  // namespace codebreaker::translate::frontends::python::pyfe
