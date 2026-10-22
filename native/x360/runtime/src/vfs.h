#pragma once

#include <filesystem>
#include <optional>
#include <string>

// Guest path resolution: maps Xbox device paths onto host folders.
//
//   \Device\Flash\...   -> the console's flash file system (SystemUpdate/nand), read-only; \SystemRoot links to it
//
// Accepted forms: "GAME:\res\x", "\??\GAME:\res\x", NT object names that were mounted or linked ("\SystemRoot\x"),
// and symbolic links created with ObCreateSymbolicLink. Names are case-insensitive; "." and ".." are resolved and may
// not leave a device's root.
namespace vfs
{
    struct ResolvedPath
    {
        std::filesystem::path host;
        bool readOnly = true;
        std::string device; // lower-case device name
    };

    // device: "game" / "game:" (a DOS device) or an NT object name ("\Device\Flash").
    void MountDevice(const std::string& device, const std::filesystem::path& root, bool readOnly);
    bool UnmountDevice(const std::string& device);
    std::optional<ResolvedPath> Resolve(const std::string& guestPath);

    // ObCreateSymbolicLink / ObDeleteSymbolicLink: link names like "\??\savedrive:" pointing at another guest path.
    void CreateLink(const std::string& link, const std::string& target);
    // A link's target, or nullopt (NtOpenSymbolicLinkObject / NtQuerySymbolicLinkObject).
    std::optional<std::string> LinkTarget(const std::string& link);
    // The path with its symbolic links followed to a mounted device (ObTranslateSymbolicLink), or nullopt.
    std::optional<std::string> TranslatePath(const std::string& guestPath);
    bool DeleteLink(const std::string& link);
}
