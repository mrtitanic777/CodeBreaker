// translate/frontends/python/registration.cpp -- the Python front end (CPython's ast, the translator's inference) registers itself with the hub.
#include "translate/registry.hpp"

namespace cb::translate::registry {

void register_python_front_end(Registry& r) { r.add(FrontEnd{"python", "CPython's ast and the translator's inference", {".py"}}); }

}  // namespace cb::translate::registry
