// codebreaker/translate/frontends/python/lower.hpp -- SHIM over cb/translate/frontends/python/lower.hpp (docs/CPP_DESIGN.md 6.3): a typed Python
// module into L2, or a refusal with its reason. The lowering is cb::translate::frontends::python::lower now (lower.hpp, with lower_records,
// lower_types, lower_locals, lower_stmt, lower_comp, lower_expr and lower_call behind it).
//
// Nothing included the old port (its one consumer, the Python front end's package translation, was written against the abstract `Lowering` of
// package.hpp and is refactored with it), so this shim carries no old types: it only gives the old namespace the new module's name. The
// old names map as: Refused -> lower::Refused (its `refusal()`), Context -> lower::Context, Records -> lower::Records, FnLower -> lower::FnLower,
// lower_functions / close_refusals -> the same names over lower::Lowered, ModuleResult -> lower::ModuleResult.
#pragma once

#include "cb/translate/frontends/python/lower.hpp"

namespace codebreaker::translate::frontends::python::lower {

namespace cb_lower = ::cb::translate::frontends::python::lower;

}  // namespace codebreaker::translate::frontends::python::lower
