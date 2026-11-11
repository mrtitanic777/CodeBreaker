// cb/build/period16.hpp -- a 16-bit Windows DLL built from its source with the tools of its time.
//
//     The flow of the `build16` command:
//     auto dll = period16::build(runner, tools, folder);            // the built DLL's path, if one was made
//     Comparison c = period16::compare(read(dll), read(original));  // segments identical to the original's
//     Bytes file = period16::normalize(read(dll), read(donor));     // the original's DOS stub put in front
//
// The 16-bit modules of Windows 95 were compiled with Microsoft C 6.0a and linked with the Windows SDK's LINK 5.60
// against the Windows 3.1 DLL runtime. Microsoft C 6 here compiles but has no linker or libraries, so the build
// uses both kits, in DOSBox-X:
//
//     each module   Microsoft C 6.0a   cl /c /AM /G2 /Gsw /Ox            (the code)
//     the DLL       Visual C++ 1.52c   link @<folder's .lnk> ; rc          (the NE file, its runtime)
//
// A source folder holds the modules' .c files, the headers, the module-definition (.def) file and the linker's
// response file (.lnk) that lists the objects and names the DLL. Objects with no .c (the DLL entry, LIBENTRY.OBJ)
// are taken from the Windows 3.1 DLL kit (toolchains/period/libs/win31-kit) or the DDK. That kit's lean runtime is
// the one the originals were linked with; Visual C++ 1.52c's own is a third larger.
//
// The comparison is per segment, byte for byte: LINK lays out the code exactly as the original was laid out when
// the source is the source. tests/fixtures/lzexpand is a reconstruction of LZEXPAND.DLL made this way. Its notes
// said eight of twelve segments came out identical; against the corpus's copies today it builds eleven segments,
// several the original's length, none identical (the old tool gives the same) -- a measure of what is left, not
// of the build.
//
// The `build16 <source folder> [--original MODULE] [--work DIR] [--stub DONOR]` command is declared with the app's
// commands: it calls build(), prints compare()'s lines, and writes normalize()'s file.
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/dos_run.hpp"

namespace cb::build::period16 {

// Where the tools are (settings.json: dosbox_x, msc6, msvc152) and the Windows 3.1 DLL kit.
struct Tools {
    std::filesystem::path dosbox_x;
    std::filesystem::path msc6;      // Microsoft C 6.0a: the compiler
    std::filesystem::path msvc152;   // Visual C++ 1.52c: LINK, RC, LIB, WINSTUB.EXE
    // The Windows 3.1 DLL kit: MDLLCEW.LIB, LIBW.LIB, LIBENTRY.OBJ (see find_kit).
    std::optional<std::filesystem::path> win31_kit;
    // Where LIBENTRY.OBJ is when the kit lacks it (the Windows 95 DDK's LIB folder), if known.
    std::optional<std::filesystem::path> libentry_fallback;
};

// <checkout_root>/toolchains/period/libs/win31-kit when it holds MDLLCEW.LIB.
std::optional<std::filesystem::path> find_kit(const std::filesystem::path& checkout_root);

// LIBENTRY.OBJ: the kit's, else the fallback's, else none.
std::optional<std::filesystem::path> find_libentry(const Tools& tools);

// The linker response file of a folder: the first `.lnk` by name. NotFoundError when there is none.
std::string response_file(const std::filesystem::path& folder);

// Compile and link the DLL a source folder describes. -> the path of the built DLL, or none (the work folder
// keeps CL.TXT, LINK.TXT and RC.TXT). NotFoundError when a tool is not set up. `work` is the folder to build
// in (default: a new one under the temp folder, kept).
std::optional<std::filesystem::path> build(const DosRunner& runner, const Tools& tools, const std::filesystem::path& folder,
                                           const std::optional<std::filesystem::path>& work = std::nullopt,
                                           std::string_view flags = "/AM /G2 /Gsw /Ox");

// One segment of an NE file: its length as the segment table gives it, and its bytes (a view into the file).
struct Segment {
    std::uint16_t length = 0;
    ByteView bytes;
};

// The segments of an NE file, in order. FormatError when the segment table lies.
std::vector<Segment> segments(ByteView ne_file);

// The result of comparing a build with the original, segment by segment.
struct Comparison {
    std::size_t identical = 0;
    std::size_t total = 0;             // segments of the original
    std::vector<std::string> lines;    // the report: one line per segment and a summary
};
Comparison compare(ByteView built, ByteView original);

// Put the DOS stub of `donor` (an NE or PE file, or a raw stub) in front of `built`'s NE header and fix every
// file-absolute field that moves with it (e_lfanew, the segment sectors, the non-resident names, the fast-load
// area). LINK 5.20 pads the stub to 1024 bytes; the retail modules have a shorter one. No segment byte moves
// relative to the header. FormatError when the donor's stub cannot replace the built one (not a multiple of 16
// bytes, longer than it, or a difference that is not a whole number of sectors).
Bytes normalize(ByteView built, ByteView donor);

}  // namespace cb::build::period16
