// java/frontend_registration.cpp -- the Java front end (javac's tree through java/frontend) registers itself with the translator's hub.
#include "translate/registry.hpp"

namespace cb::translate::registry {

void register_java_front_end(Registry& r) { r.add(FrontEnd{"java", "javac's tree", {".java"}}); }

}  // namespace cb::translate::registry
