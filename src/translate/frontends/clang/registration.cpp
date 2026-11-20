// translate/frontends/clang/registration.cpp -- the clang front end (C and C++ units from clang's AST JSON) registers itself with the hub.
#include "translate/registry.hpp"

namespace cb::translate::registry {

void register_clang_front_end(Registry& r) { r.add(FrontEnd{"c", "clang's AST", {".c", ".h"}}); }

}  // namespace cb::translate::registry
