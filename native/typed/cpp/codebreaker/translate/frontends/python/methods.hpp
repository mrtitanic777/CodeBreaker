// codebreaker/translate/frontends/python/methods.hpp -- SHIM over cb/translate/frontends/python/methods.hpp (docs/CPP_DESIGN.md 6.3): the types of Python's
// builtins, the methods of its builtin types and the standard library calls the front end maps. The module is
// cb::translate::frontends::python::methods now: `library()` is the infer::Library the interpreter asks.
//
// Nothing included the old port (its callers were the old inference and the old package translation, both refactored with it and removed), so this shim
// carries no old types: it only gives the old namespace the new module's name.
#pragma once

#include "cb/translate/frontends/python/methods.hpp"

namespace codebreaker::translate::frontends::python::methods {

namespace cb_methods = ::cb::translate::frontends::python::methods;

}  // namespace codebreaker::translate::frontends::python::methods
