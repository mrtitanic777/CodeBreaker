// base/host_tools.hpp -- the development kits and tools installed on this computer, found without being told where they are, so a fresh
// CodeBreaker works with what the machine already has. What is looked for, and where, is the tool catalog (config/tools/*.json, read by
// tool_catalog.hpp): a JDK 11 or newer, Visual Studio with its MSBuild and CMake, ILSpy, the Xbox 360 SDK, OpenWatcom ...
//
//     for (const host_tools::Found& f : host_tools::scan(root)) ...   // {"java", "C:/Program Files/Eclipse Adoptium/jdk-21.0.2.13-hotspot/bin/java.exe", "..."}
//
// What is found is used only where the environment, settings.json, the work folder and config/attached_tools.json say nothing
// (config::current()); the app attaches it the first time it starts (config/attached_tools.json), where Settings, Advanced shows it.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace cb::config::host_tools {

struct Found {
    std::string key;      // the setting
    std::string value;    // as settings.json would hold it
    std::string how;      // where it was found ("JAVA_HOME", "Visual Studio Installer", "PATH", the folder searched)
};

// Every tool of the checkout's catalog found on this computer (at most one per setting). Quick: folders, environment variables and one run of
// vswhere.
std::vector<Found> scan(const std::filesystem::path& checkout_root);

// The major version of the JDK in `home` from its `release` file (JAVA_VERSION="17.0.2", "1.8.0_381" is 8); nullopt when it is not a JDK with
// javac (bin/javac.exe).
std::optional<int> jdk_version(const std::filesystem::path& home);
// The newest JDK of at least `least` among `homes`.
std::optional<std::filesystem::path> newest_jdk(const std::vector<std::filesystem::path>& homes, int least = 11);
// The major version in a JAVA_VERSION text ("17.0.2" -> 17, "1.8.0_381" -> 8, "21" -> 21).
std::optional<int> java_major(std::string_view version);

}  // namespace cb::config::host_tools
