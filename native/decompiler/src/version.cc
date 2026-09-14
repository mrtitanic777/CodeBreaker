// XEXMagic - build identity. See version.h.

#include "version.h"

#include <cstdio>

#ifdef _WIN32
#include <windows.h>
#endif

namespace xexmagic {

std::string BuildId() {
  std::string id = kVersion;
#ifdef _WIN32
  // The executable's own last-write time. Every relink moves it, which is
  // exactly the granularity a cache needs: a rebuilt decompiler produces
  // different output from the same input.
  char path[MAX_PATH];
  DWORD n = GetModuleFileNameA(nullptr, path, MAX_PATH);
  if (n && n < MAX_PATH) {
    WIN32_FILE_ATTRIBUTE_DATA fad{};
    if (GetFileAttributesExA(path, GetFileExInfoStandard, &fad)) {
      char stamp[64];
      snprintf(stamp, sizeof(stamp), "+%08lX%08lX",
               (unsigned long)fad.ftLastWriteTime.dwHighDateTime,
               (unsigned long)fad.ftLastWriteTime.dwLowDateTime);
      id += stamp;
    }
  }
#endif
  return id;
}

}  // namespace xexmagic
