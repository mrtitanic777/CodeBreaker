// XEXMagic - application data locations. See app_paths.h.

#include "app_paths.h"

#include <cstdlib>
#include <filesystem>
#include <system_error>

namespace fs = std::filesystem;

namespace xexmagic {

bool EnsureDir(const std::string& path) {
  std::error_code ec;
  if (fs::is_directory(path, ec)) return true;
  fs::create_directories(path, ec);
  return fs::is_directory(path, ec);
}

std::string AppDataDir() {
  std::string root;
#ifdef _WIN32
  if (const char* appdata = getenv("APPDATA")) root = appdata;
#else
  if (const char* home = getenv("HOME")) root = std::string(home) + "/.config";
#endif
  if (root.empty()) {
    std::error_code ec;
    root = fs::current_path(ec).string();
  }
  std::string dir = (fs::path(root) / "XEXMagic").string();
  EnsureDir(dir);
  return dir + (char)fs::path::preferred_separator;
}

std::string AppSubDir(const std::string& sub) {
  std::string dir = AppDataDir() + sub;
  EnsureDir(dir);
  return dir + (char)fs::path::preferred_separator;
}

}  // namespace xexmagic
