// XEXMagic - MSVC C++ name demangler
// Decodes Microsoft Visual C++ mangled names into readable type signatures

#include "msvc_demangle.h"

#include <cstring>
#include <mutex>
#include <string>
#include <vector>

// On Windows, use UnDecorateSymbolName from DbgHelp
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <dbghelp.h>
// MSVC: #pragma comment(lib, "dbghelp.lib")
// MinGW/Clang: link with -ldbghelp in CMakeLists.txt

std::string MsvcDemangle(const std::string& mangled) {
  if (mangled.empty() || mangled[0] != '?') return mangled;

  char buf[2048];
  DWORD result;
  {
    // Every DbgHelp entry point is documented as single-threaded: the library
    // keeps global state and the caller must serialise. The SDK index parses
    // libraries across all cores and demangles as it goes, which crashed the
    // process outright until this lock existed.
    static std::mutex dbghelp_mu;
    std::lock_guard<std::mutex> lk(dbghelp_mu);
    result = UnDecorateSymbolName(mangled.c_str(), buf, sizeof(buf),
                                  UNDNAME_COMPLETE |
                                      UNDNAME_NO_ACCESS_SPECIFIERS);
  }

  if (result > 0) return std::string(buf);
  return mangled;
}

#else
// Non-Windows: simple passthrough (MSVC mangling is Windows-only)
std::string MsvcDemangle(const std::string& mangled) {
  return mangled;
}
#endif
