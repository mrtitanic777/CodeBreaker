// cb/exact/link.hpp -- the program linked from its sources by the period linker, and compared with the original file.
//
//     codebreaker exact PROJECT link [-v] [--keep DIR] [--map] [--fit]
//
//     link::Built b = link::build(*project, dos);            // the linked file or none, LINK's words, its map, the stand-ins
//     std::vector<std::string> lines = link::diagnose(*project, original_file, *b.exe);
//
// What goes in, in this order:
//     src/FIRSTOBJ   the kit's objects linked before the program's own (one a line)
//     src/MODULES    the program's modules (exact/modules.hpp): compiled whole, a function not yet the program's made a stand-in
//                    of its bytes (exact/standin.hpp); NAME.OBJ: the kit's
//     src/BSS        the variables without a value at their places (exact/bss.hpp): "range LO HI", "hole ADDR LEN" (a library
//                    module's own _BSS between), "name ADDR"; an object before the library part, one after each hole
//     src/CRTLIB     the library's modules in the program's order (exact/library.hpp): those the rest needs are linked as objects;
//                    RAW lines are the program's bytes
//     src/LINKOPT    the linker's switches;  src/EXEMOD  the kit's EXEMOD run after, one line each
//
// The linker and the files it is run with are the toolchain profile's "link" part (project::LinkSpec):
//     {"kit": kit, "run": "disk3/LINK.EXE", "dir": kit folder of libraries and objects, "libs": {model: [library files]},
//      "exemod": "disk3/EXEMOD.EXE", "psp": paragraph}
//
// The result is compared with the original: header, relocation table, load image; where they part, the first difference is put to
// the stage that makes that part (a function's code, the data, the library's part, the variables without a value, the header).
//
// The linker is run under the DOS emulator the caller gives (linkwhole::DosHost). A program whose project says it is linked whole
// (project.json "link") is exact/linkwhole's: `run` hands it over.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/linkwhole.hpp"
#include "cb/exact/project.hpp"

namespace cb::exact::link {

// An object module and the name the linker is told it by.
struct NamedObject {
    std::string name;
    Bytes object;
};

// The program's own modules, compiled; the names of the functions that are stand-ins of the program's bytes. LinkError for a module
// that does not compile; UsageError for a profile with no linker. `say` (with `verbose`) is told what a module did.
struct Objects {
    std::vector<NamedObject> objects;
    std::vector<std::string> stand_ins;
    std::vector<std::string> notes;         // "   MODULE: stand-ins f01234 ..." (with `verbose`)
};
Objects objects(project::Project& project, bool verbose = false);

// Where the program's own code ends: the first code place src/CRTLIB names (a pad before the library is the last module's when CRTLIB
// has no RAW piece for it), else the end carve finds.
std::uint32_t library_start(project::Project& project);

// src/FIRSTOBJ: the kit's objects linked before the program's own.
std::vector<NamedObject> first_objects(project::Project& project);

struct BssObjects {
    std::vector<NamedObject> objects;
    bool after = false;     // the communal names are in a segment of their own, after the library
};
BssObjects bss_objects(project::Project& project);

struct CrtResult {
    std::vector<NamedObject> modules;       // the library's modules the rest needs, in the program's order
    std::vector<std::string> missing;       // names left unresolved, sorted
    std::vector<std::string> conflicts;     // where the program has one routine and ours names two
};
// The library's modules the rest of `objs` needs, as objects in the program's order (src/CRTLIB). LinkError for a CRTLIB that names a
// module no library of the profile has, or a hand-written one that does not assemble.
CrtResult crt(project::Project& project, const std::vector<NamedObject>& objs);

// The DOS name of an object: its capitals and digits, eight at most ("M" for none).
std::string dos_name(std::string_view name);

// Run the linker over the objects (in a folder of its own, deleted after unless `keep`), then EXEMOD. The file it wrote, if any, what
// it said, and its map.
linkwhole::LinkOutput run_link(project::Project& project, const std::vector<NamedObject>& objs, const linkwhole::DosHost& dos,
                               const std::optional<std::filesystem::path>& keep = std::nullopt, std::string_view options = "");

// An EXE as the stages compare it.
struct Parts {
    std::array<std::uint16_t, 14> header{};
    Bytes image;                            // the load image
    std::vector<std::uint64_t> relocations; // (offset << 16) | segment, in the table's order
};
// FormatError / TruncatedError for a file that is not an EXE.
Parts parts(ByteView exe);

// The names of the header's fields, in order.
const std::vector<std::string>& field_names();

// Where `exe` parts from the original, and which stage makes that part. `reference`: the original file; none for an original that is
// EXEPACKed (ours is then held to the unpacked image).
std::vector<std::string> diagnose(project::Project& project, const std::optional<ByteView>& reference, ByteView exe);

// Where b runs ahead of or behind a: at each change, the place in `a` and how far b is ahead of it.
struct Shift {
    std::size_t at = 0;
    std::int64_t by = 0;
};
std::vector<Shift> shifts(ByteView a, ByteView b, std::size_t step = 2, std::size_t span = 12, std::size_t reach = 160);

// The same as lines: "DS:XXXX on, ours is +N" at each change (runs of few distinct bytes -- zeros, a table of one value --
// say nothing and are skipped), sixteen at most (the lines are for reading: `shifts` is for the program).
std::vector<std::string> drift(ByteView a, ByteView b, std::size_t step = 2, std::size_t span = 12, std::size_t reach = 160);

// The linker's switches (src/LINKOPT).
std::string options_of(project::Project& project);

struct Built {
    std::optional<Bytes> exe;
    std::string console, map;
    std::vector<std::string> stand_ins;
    std::vector<std::string> notes;         // "not resolved: ..." and the routines the program has twice (what `quiet` leaves out)
};
// `options` override src/LINKOPT; `quiet`: no notes.
Built build(project::Project& project, const linkwhole::DosHost& dos, const std::optional<std::filesystem::path>& keep = std::nullopt,
            bool verbose = false, bool quiet = false, const std::optional<std::string>& options = std::nullopt);

// The data stand-ins' lengths fitted by the link: each round the first place where ours runs behind or ahead of the program in the data
// segment is put to the stand-in of a guessed length just before it, which is made that much longer or shorter (project.json
// "data_sizes", kept; or a pad of zeros no module accounts for: "data_pads"). -> the lines of what was done.
std::vector<std::string> fit(project::Project& project, const linkwhole::DosHost& dos, int rounds = 8);

// `exact PROJECT link [-v] [--keep DIR] [--map] [--fit]`: 0 when the image, header and relocations are the original's.
int run(project::Project& project, const std::vector<std::string>& args, const linkwhole::DosHost& dos);

}  // namespace cb::exact::link
