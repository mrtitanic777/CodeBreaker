// codebreaker/translate/dotnet/cs2cpp.hpp -- COMPAT SHIM over cb/translate/dotnet/cs2cpp.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer.
//
// The module is native/cb/translate/dotnet/cs2cpp*.{hpp,cpp}: the pass that rebinds the C# kit's L2 to Java's library and writes the Java
// skeleton the C++03 back end reads (`cs2cpp::ToJava`, `skeleton`, `convert`, `host_print`), over the typed IR tree in an arena. It has no
// caller of the old types in C++ (the only readers were the Python judge's C# lane, which reaches it through the command layer), and its
// old shape -- a pass that rewrote a shared-pointer tree, a `Transpiler` that handed the old transpiler's summary back -- does not cross to
// the typed tree and the converter's new back-end interface, so nothing is kept here: this header stays so the module's old include path
// resolves, and new code includes the new one.
#pragma once

#include "cb/translate/dotnet/cs2cpp.hpp"
