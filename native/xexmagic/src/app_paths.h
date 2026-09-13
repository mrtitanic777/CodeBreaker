// XEXMagic - where the tool keeps its own files.
//
// Settings, uploaded images and databases all live under %APPDATA%\XEXMagic,
// not beside the executable and not beside the image. An SDK directory under
// Program Files is not writable, a build directory is not permanent, and the
// working directory is whatever the tool happened to be started from.

#pragma once

#include <string>

namespace xexmagic {

// %APPDATA%\XEXMagic, created if missing, with a trailing separator.
// Falls back to the working directory only if APPDATA is unset.
std::string AppDataDir();

// Creates `path` and every missing parent. True if it exists afterwards.
bool EnsureDir(const std::string& path);

// AppDataDir() + sub + separator, created.
std::string AppSubDir(const std::string& sub);

}  // namespace xexmagic
