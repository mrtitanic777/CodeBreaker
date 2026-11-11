// cb/platforms/java.hpp -- Java: a folder of source (or a .jar), translated to C++ and judged by running both under the
// JVM. A .jar is Java by its name; a folder or a .java file is Java when the Java lane says it is a module
// (`Probes::is_java_module`).
#pragma once

#include "cb/platforms/package.hpp"

namespace cb::platforms {

Platform java();

}  // namespace cb::platforms
