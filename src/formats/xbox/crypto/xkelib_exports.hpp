// formats/xbox/crypto/xkelib_exports.hpp -- a .def file for an Xbox 360 library: its exports by ordinal, read with the SDK's dumpbin.
//
//     xkelib_exports::Library lib = xkelib_exports::read(dumpbin_exe, "xboxkrnl.exe");
//     std::string def = xkelib_exports::def_text(lib);      // "LIBRARY ...", "EXPORTS", "\tname @ordinal" lines
//
// dumpbin /HEADERS names the library ("DLL name : xboxkrnl.exe") and dumpbin /EXPORTS lists "<ordinal> <name>" lines; a name that starts with
// "." is a local label of the import library and is left out. The .def is CRLF text, the exports by ordinal.
//
// The command `crypto xkelib_exports <library> <out.def> [-v]` needs the Xbox 360 SDK (its bin/win32/dumpbin.exe): the folder of the setting
// `xbox360_sdk`.
#pragma once

#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/cli.hpp"

namespace cb::formats::xbox::crypto::xkelib_exports {

struct Export {
    int ordinal = 0;
    std::string name;
};

struct Library {
    std::string name;
    std::vector<Export> exports;   // by ordinal
};

// The library's name from `dumpbin /HEADERS` text; empty when it names none.
std::string library_name(std::string_view headers);
// The exports from `dumpbin /EXPORTS` text, by ordinal (names starting with "." left out).
std::vector<Export> parse_exports(std::string_view exports);

// Run dumpbin on `library` (ToolError when it fails to run; NotFoundError for a missing file; FormatError when the library names itself nowhere).
Library read(const std::filesystem::path& dumpbin, const std::filesystem::path& library);

std::string def_text(const Library& library);

cli::Command command(const std::filesystem::path& sdk_bin_dir);

}  // namespace cb::formats::xbox::crypto::xkelib_exports
