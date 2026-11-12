// codebreaker/translate/backends/java_libc.hpp -- COMPAT SHIM over cb/translate/backends/java_libc.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/java_libc.{hpp,cpp}: a C library call as Java, over the API-mapping registry. It is
// used by the Java back end's writers (cb/translate/backends/java and java_objects), which take it as a java::LibraryCalls; the old
// writers that called `java_libc::call` with their own function-writer type are gone with the old java.hpp, which is a shim now. What
// is left here is the registry accessor the old module had, over the old registry shim's cache.
#pragma once

#include <memory>

#include "codebreaker/java/apimap.hpp"
#include "codebreaker/translate/backends/java.hpp"

namespace codebreaker::translate::backends::java_libc {

// The API-mapping registry the C library rows are read from (loaded once).
inline std::shared_ptr<codebreaker::java::apimap::Registry> registry() { return codebreaker::java::apimap::load(); }

}  // namespace codebreaker::translate::backends::java_libc
