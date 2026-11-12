// codebreaker/translate/backends/csharp_java.hpp -- COMPAT SHIM over cb/translate/backends/csharp_java.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/csharp_java.{hpp,cpp}: the C# back end's knowledge of Java's library and semantics, a
// csharp::Library the new back end asks. The old JavaLib wrote through the old csharp.hpp's Writer, which is the new back end's now;
// the old layer reaches Java units through csharp::write (the shim in csharp.hpp), which registers the library. Nothing here is left
// to forward: this header stays so the module's old include path (and the glue that names it) still resolves.
#pragma once

#include "codebreaker/translate/backends/csharp.hpp"
