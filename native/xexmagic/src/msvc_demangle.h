// XEXMagic - MSVC C++ name demangler
// Decodes Microsoft Visual C++ mangled names into readable signatures
#pragma once

#include <string>

// Demangle an MSVC decorated name (e.g., "?foo@Class@@QAEHXZ" -> "int Class::foo(void)")
// Returns the original string if demangling fails.
std::string MsvcDemangle(const std::string& mangled);
