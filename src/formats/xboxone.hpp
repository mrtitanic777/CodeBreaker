// formats/xboxone.hpp -- Xbox One / Xbox Series homebrew: the x64 PE32+ executable of a UWP app, in the package dev mode installs or loose.
//
// A dev-mode app is a Universal Windows Platform app: a 64-bit PE image (PE32+, machine 0x8664, the AppContainer flag set in its DLL characteristics) linked against
// the Windows API set the console's Windows-derived system provides, in a package. The package is a zip of the app's files: `.appx` (the older name) or `.msix`; its
// `AppxManifest.xml` names the identity, the architecture and the executable the app starts, and `AppxBlockMap.xml` and `AppxSignature.p7x` sign it. An
// `.appxbundle` / `.msixbundle` is a zip of such packages (one per architecture and resolution) with `AppxMetadata/AppxBundleManifest.xml`; the x64 package is the one that
// holds the code. CodeBreaker reads the primary executable the manifest names (or the largest PE32+ the package holds), and any other PE32+ member by name.
//
// What it does not read: a retail package is an `.xvc` / `.msixvc` (an XVD image: "msft-xvd" at 0x200) or an encrypted `.eappx` / `.eappxbundle`, whose files are
// encrypted with the console's keys; they are recognised and refused (FormatError), and CodeBreaker does not decrypt. Only the PE image is used; the package's
// resources (`resources.pri`, the assets, the WinRT metadata) are listed and left.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/pe.hpp"

namespace cb::formats::xboxone {

struct Member {
    std::string name;
    std::uint64_t size = 0;
};

struct Program {
    std::string container;            // "PE32+ executable", "APPX package", "MSIX package", "APPX bundle", "MSIX bundle"
    std::string identity, publisher, version, architecture;   // the manifest's Identity
    std::string member;               // the executable's name in the package ("" for a loose one)
    std::vector<Member> members;      // the executables and DLLs of the package (by their names), with the count of the other files in `other_members`
    std::size_t other_members = 0;
    bool app_container = false;       // the DLL characteristics say it runs in the app sandbox (UWP)
    Bytes file;                       // the PE32+ file
};

// FormatError when it is none of these; an encrypted package says what it is.
Program read(const std::filesystem::path& path, const std::string& member = "");
Program parse(ByteView data, const std::string& name = "package", const std::string& member = "");

// The PE image of the program.
pe::Image image_of(const Program& program);

// Is it a loose x64 PE32+ with the AppContainer flag, a package with an x64 PE32+ in it, or an encrypted package (recognised)? Reads what it needs, never throws.
bool probe(const std::filesystem::path& path);

// Is it an encrypted package (an XVD / .xvc / .msixvc, an .eappx)?
bool is_encrypted_package(const std::filesystem::path& path);

}  // namespace cb::formats::xboxone
