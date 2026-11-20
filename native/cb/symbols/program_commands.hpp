// cb/symbols/program_commands.hpp -- the two commands that read a PowerPC (Xbox 360) program for its names, complete.
//
//     classes <.xex or image> [-o classes.json]            its C++ classes from run-time type information (cpp_classes)
//     xdk-match <.xex or image> [<lib folder>] [-o F]      the XDK library functions it was linked with (xdk_libraries)
//
//     table.set(program_commands::classes_command({.config = config}));
//     table.set("xdk-match", program_commands::xdk_match_runner({.config = config}));
//
// Everything they run on comes from the configuration: the folder unpacked images are kept in
// (<state>/unpacked/xex), and the XDK's library folder (setting xdk_libs, else the XDK's usual install). An .xex is read through its
// basefile; any other file is read as a PowerPC image.
#pragma once

#include <filesystem>
#include <functional>
#include <span>
#include <string>
#include <vector>

#include "cb/base/config.hpp"
#include "cb/core/cli.hpp"
#include "cb/symbols/cpp_classes.hpp"
#include "cb/symbols/xdk_libraries.hpp"

namespace cb::symbols::program_commands {

struct Environment {
    config::Config config;
};

// The classes of a program file: an .xex or a PowerPC image. NotFoundError / IoError / FormatError for a file that is not one.
cpp_classes::Classes classes_of(const std::filesystem::path& program, const Environment& environment);

// The text of a program file as the XDK matcher reads it (its .text section). FormatError when it has none.
xdk_libraries::ProgramText program_text(const std::filesystem::path& program, const Environment& environment);

// The signatures of the .lib files of a folder (kept under <state>/xdk_signatures, named by the libraries' stamp, so a changed
// XDK is read again).
std::vector<xdk_libraries::Signature> signatures_of(const std::filesystem::path& folder, const Environment& environment);

// `codebreaker classes <program.xex or image> [-o classes.json]`.
cli::Command classes_command(const Environment& environment);

// `codebreaker xdk-match <program.xex or image> [<XDK lib folder>] [-o names.json]`, as the command table's raw runner takes it.
int xdk_match(std::span<const std::string> args, const Environment& environment);
std::function<int(std::span<const std::string>)> xdk_match_runner(const Environment& environment);

}  // namespace cb::symbols::program_commands
