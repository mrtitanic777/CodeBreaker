// cb/source/dos_writer.hpp -- a real-mode program as assembly source that builds back into it, and the file laid out as the original is.
//
//     dos_writer::Result r = dos_writer::reconstruct("find.exe", "out", options);
//     r.same, r.file, r.file_same, r.notes, r.remarks, r.instructions, r.raw, r.rounds
//
// The one program is dos_program's (source written, assembled, linked and compared round after round, until it builds back as the
// original: its load module, relocation table, entry and stack). What this adds is the file around it and the files made of several
// programs:
//
//   * the program as a FILE laid out as the original's: its header's own data (<stem>.hdr), the load module -- packed as LINK /EXEPACK
//     stored it when the original is (build/exepack: the fills in <stem>.pack, the unpacker built from its own source in exepack/), or as
//     PKLITE did (build/pklite: the entry code and decompressor in pklite/) -- and the bytes behind the load module (<stem>.ovl). What the
//     layout takes from the original is read from it once and kept beside the source (<stem>.build.json, with the side files), so
//     `rebuild` makes the file from the folder alone;
//   * IO.SYS (a kernel image): its loader, its BIOS + DOS body, SYSINIT and the EXEPACK unpacker are four programs, written and built as
//     any other; SYSINIT is then packed as LINK /EXEPACK stored it and the three joined into the file;
//   * a DOS program with a Windows module behind it (REGEDIT.EXE, XCOPY32.EXE): the DOS program written and built here, the module built
//     whole by the module builder (`Options::build_windows`), and the module put behind the program (build/stubjoin);
//   * a driver with a VxD in one file (DRVSPACE.BIN): the driver written and built here, the VxD by the driver lane
//     (`Options::write_vxd`), and the file put together as the original is (build/vxdfile).
//
// The other writers do not depend on this one: a Windows file's DOS stub reaches it through `stub_finder`.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/analyze/dos_functions.hpp"
#include "cb/build/watcom.hpp"
#include "cb/source/assembly.hpp"
#include "cb/source/dos_program.hpp"
#include "cb/source/stub.hpp"

namespace cb::source::dos_writer {

using Log = std::function<void(const std::string&)>;

// What the driver lane made of the VxD of a driver+vxd file.
struct VxdPart {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;
    bool linked = false;
    bool same = false;
    std::size_t rounds = 0, instructions = 0, raw = 0, data = 0;
    std::vector<std::string> notes;
};
using WriteVxd = std::function<VxdPart(const std::filesystem::path& program, const std::filesystem::path& outdir, const Log& log)>;

// What the module builder made of the Windows module of a DOS+Windows file: built whole in `outdir`, as <outdir>/<file name in lower case>.
struct WindowsBuild {
    bool linked = false;
    std::size_t functions = 0;
    std::vector<std::string> stand_ins;     // the functions built as stand-ins (rejected or absent), sorted
    std::vector<std::string> undefined;
    std::vector<std::string> extra_imports; // imports the build has that the original has not
    std::size_t ran = 0, ran_same = 0;      // exports run beside the original's, and how many came out the same
    std::string error;                      // why it did not build, when it did not
};
using BuildWindows = std::function<WindowsBuild(const std::filesystem::path& program, const std::filesystem::path& outdir)>;

struct Options {
    assembly::Tools tools;                        // OpenWatcom's assembler and linker
    std::filesystem::path probes;                 // the file what has been probed is kept in (state/assembly_probes.json)
    Log log;                                      // a round's account, when given
    WriteVxd write_vxd;                           // a driver+vxd file needs it (UsageError without)
    BuildWindows build_windows;                   // a DOS+Windows file needs it (UsageError without)
};

// The tools of an OpenWatcom tree, as the DOS programs are assembled and linked.
Options options_for(const build::watcom::OpenWatcom& open_watcom, const std::filesystem::path& probes);

// What laying the file out takes from the original (kept in <stem>.build.json).
struct Facts {
    std::string file;                              // the file's name (upper case)
    std::optional<std::string> packer;             // "EXEPACK", "PKLITE", "moves itself"
    bool overlay = false;                          // bytes behind the load module (<stem>.ovl)
    std::optional<std::uint32_t> min_alloc;
    std::optional<bool> full_last;                 // a last page that is exactly full is written as 512
    std::optional<unsigned> words;                 // EXEPACK: the header words of its unpacker
    // The relocation table's order: EXEPACK stored the linear offsets; any other file the (offset, segment) pairs of its table; PKLITE
    // the (segment, offset) pairs of its runs.
    std::vector<std::uint32_t> offsets;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> pairs;
};

// What one program of a combined file came to.
struct Part {
    std::string name;
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;
    bool linked = false;
    bool same = false;
    std::size_t rounds = 0, instructions = 0, raw = 0, data = 0;
    std::vector<std::string> notes;
};

struct Result {
    std::vector<std::filesystem::path> sources;   // the program's source; each part's for a combined file
    std::optional<std::filesystem::path> image;   // the program as linked (the file made, for a combined file)
    bool linked = false;
    bool same = false;                            // the program (all its programs) came back as the original's
    std::size_t rounds = 0;
    std::vector<std::string> notes;
    std::vector<std::string> remarks;
    std::string kind;                             // "mz", "com", "driver", "kernel", "driver+vxd", "dos+windows"
    std::size_t instructions = 0, raw = 0, data = 0;
    std::map<std::uint32_t, analyze::dos_functions::Function> functions;   // by offset in the image
    std::vector<Part> parts;
    std::optional<Facts> facts;
    bool equivalent = false;                      // the same programs, laid out another way (see `notes`)
    std::optional<WindowsBuild> windows;
    std::optional<std::filesystem::path> file;    // the file laid out as the original's
    std::optional<bool> file_same;
};

// Write a program's source, build it, and make it come back as the original (see the top). `program` may be a corpus path of any kind of
// real-mode file. FormatError when it is not one; UsageError for a combined file whose other writer was not given.
Result reconstruct(const std::filesystem::path& program, const std::filesystem::path& outdir, const Options& options);

// Make the program from its folder alone (what `reconstruct` wrote: <stem>.asm, .lnk, .build.json and the side files): assemble, link, set
// the header's entry and stack where the source cannot say them, lay the file out. -> the file made. `description`: the .build.json to read
// (default: the first the folder has). NotFoundError when the folder has none; ToolError when the assembler or linker fails.
std::filesystem::path rebuild(const std::filesystem::path& folder, const Options& options, const std::optional<std::string>& description = std::nullopt);

// The DOS stub of a Windows file, from its own source when that builds back (what pe_writer, ne_writer and le_writer take as
// Options::find_stub): the stub is carved from the file (when it is a program, not the 128-byte "cannot be run in DOS mode" kind), written
// as source here, and its built file is given when it is the original's bytes.
FindStub stub_finder(const Options& options);

// What the command `dos-source <program> [--out DIR]` needs of its surroundings.
struct Environment {
    Options options;
    std::filesystem::path state;                  // the default out folder is <state>/dos_source/<program name>
    std::function<std::optional<std::filesystem::path>(std::string_view name)> retail;   // a corpus name to its file
};
// The command line without the command's name; `say` takes every line printed. -> the exit code (2: no program named, 1: it did not come
// back).
int command(std::span<const std::string> args, const Environment& env, const std::function<void(std::string_view)>& say);

// The command's usage text.
std::string_view usage();

}  // namespace cb::source::dos_writer
