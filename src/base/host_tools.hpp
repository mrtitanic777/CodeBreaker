// base/host_tools.hpp -- the development kits and tools installed on this computer, found without being told where they are, so a fresh
// CodeBreaker works with what the machine already has.
//
//     for (const host_tools::Found& f : host_tools::scan()) ...   // {"java", "C:/Program Files/Eclipse Adoptium/jdk-21.0.2.13-hotspot/bin/java.exe", "..."}
//
// Each tool is a setting (settings.json's key) and is looked for where its installers put it:
//
//     java           a JDK 11 or newer (one with javac): JAVA_HOME, then the JDK folders of Oracle, Temurin, Microsoft, Zulu, Liberica,
//                    Corretto, Semeru and IntelliJ (~/.jdks), then PATH; the newest wins. The value is its bin/java.exe.
//     visual_studio  the newest Visual Studio with the C++ x64 tools (vswhere).
//     msbuild        that Visual Studio's MSBuild.exe.
//     cmake          cmake.exe on PATH, else Visual Studio's own, else C:/Program Files/CMake.
//     ilspycmd       the .NET global tool (~/.dotnet/tools/ilspycmd.exe), or on PATH.
//     xbox360_sdk    XEDK, else C:/Program Files (x86)/Microsoft Xbox 360 SDK.
//     openwatcom     WATCOM, else C:/WATCOM or C:/OpenWatcom.
//
// What is found is used only where settings.json, the work folder and CB_<KEY> say nothing (config::current()); the app writes it into
// settings.json the first time it starts, where Settings shows it and a person can change it.
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
    std::string how;      // where it was found ("JAVA_HOME", "vswhere", "PATH", the folder searched)
};

// Every tool found on this computer (at most one per key). Quick: folders, environment variables and one run of vswhere.
std::vector<Found> scan();

// The major version of the JDK in `home` from its `release` file (JAVA_VERSION="17.0.2", "1.8.0_381" is 8); nullopt when it is not a JDK with
// javac (bin/javac.exe).
std::optional<int> jdk_version(const std::filesystem::path& home);
// The newest JDK of at least `least` among `homes`.
std::optional<std::filesystem::path> newest_jdk(const std::vector<std::filesystem::path>& homes, int least = 11);
// The major version in a JAVA_VERSION text ("17.0.2" -> 17, "1.8.0_381" -> 8, "21" -> 21).
std::optional<int> java_major(std::string_view version);

}  // namespace cb::config::host_tools
