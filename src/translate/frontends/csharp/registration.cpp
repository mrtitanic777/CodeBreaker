// translate/frontends/csharp/registration.cpp -- the C# front end (Roslyn through the .NET SDK) registers itself with the hub.
#include "translate/registry.hpp"

namespace cb::translate::registry {

void register_csharp_front_end(Registry& r) { r.add(FrontEnd{"csharp", "Roslyn through the .NET SDK", {".cs"}}); }

}  // namespace cb::translate::registry
