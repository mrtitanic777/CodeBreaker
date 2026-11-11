// cb/build/driver.hpp -- build a virtual device driver from assembly source, and check it against the original.
//
//     codebreaker driver-build <source folder or .asm> [--original FILE] [--out DIR]
//
//     Tools tools = find_tools(open_watcom, kit_include_dir, state_folder);
//     BuildResult r = build(tools, {.source = "..\\Windows95\\vmm32\\vkd", .work = work, .original = "vkd.vxd"});
//     r.assembled, r.linked, r.path, r.checks, r.notes
//
// The source is one .asm written with the driver kit's macros (Declare_Virtual_Device, Begin_Service_Table,
// VMMcall ...). It is assembled with OpenWatcom's wasm against the kit's include files and linked with wlink into an LE
// image whose one export is the device descriptor block. Segments get the attributes the kit's own module-definition
// file gives each class of segment (locked code is preloaded, initialisation code is discardable, ...); whether the
// driver is loaded at start-up or on demand is taken from the original when there is one.
//
// The check is structural -- does the rebuilt file have what the original has:
//
//     objects      the same number of objects, each of the same size and attributes
//     descriptor   the same device descriptor block (name, numbers, versions, flags, order, what it points at)
//     services     the same service table (as many services, the same ones left empty, each starting with the same
//     code) exports      the same exported names and ordinals, pointing at the descriptor block module       the same
//     kind of module (start-up, on demand, old style)
//
// and, where objects pair up, how many of their bytes are equal outside the places the loader patches. Whether the two
// BEHAVE alike is verify/lanes/driver's question (`driver-judge`). Everything is written under the work folder; the
// source folder is only read.
//
// Nothing here is global: the tools, the kit and the folders are passed in. What the command line needs of the rest of
// the program (the corpus, the state folder) is an `Environment`.
#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "cb/build/watcom.hpp"
#include "cb/core/cli.hpp"
#include "cb/formats/le.hpp"

namespace cb::build::driver {

// What the kit's module-definition file says of each class of segment, as wlink spells it.
struct SegmentClass {
    std::string_view name;
    std::string_view attributes;
};
inline constexpr std::array<SegmentClass, 8> kSegmentClasses{{{"LCODE", "preload"},
                                                              {"MCODE", "preload iopl"},
                                                              {"ICODE", "discardable"},
                                                              {"PCODE", "loadoncall"},
                                                              {"PDATA", "loadoncall shared"},
                                                              {"SCODE", "resident"},
                                                              {"DBOCODE", "preload conforming"},
                                                              {"16ICODE", "preload discardable"}}};

// One tool run takes at most this long.
inline constexpr std::chrono::seconds kToolTimeout{300};

// ---- the tools -----------------------------------------------------------------------------------------------------

// The assembler or linker to use. Drivers need the builds of wasm and wlink that understand the kit's macros and write
// the driver flavour of LE; in the OpenWatcom tree those are the ones in binnt. Absolute path, or empty when there is
// none.
std::filesystem::path tool(const watcom::OpenWatcom& open_watcom, std::string_view name);

// The kit's include files in a folder whose path has no spaces (the assembler cannot be given one that has):
// `kit_source` itself if it has none, else a copy of its .INC files in `copy_to` (made once). Empty when there is no
// kit (`kit_source` empty) or no folder without spaces to put a copy in.
std::filesystem::path kit_folder(const std::filesystem::path& kit_source, const std::filesystem::path& copy_to);

// Where the assembler, the linker and the kit are, and what they run under.
struct Tools {
    std::filesystem::path wasm, wlink;      // empty: not found
    std::filesystem::path kit;              // the include folder (no spaces); empty: not found
    std::map<std::string, std::string> env; // added to the environment of both
};
// The tools of `open_watcom`, run for Win32 targets. `kit_source` is the Windows 95 DDK's include folder; its copy (if
// it has spaces in its path) goes to `<state>/driver_kit`.
Tools find_tools(const watcom::OpenWatcom& open_watcom, const std::filesystem::path& kit_source,
                 const std::filesystem::path& state);

// The .asm to build: `path` itself, or in a folder the file named after the folder. UsageError when the folder has
// none.
std::filesystem::path source_of(const std::filesystem::path& path);

// The driver of `candidates` a source file was made from: the one with the same name (case aside), the first in the
// order given; nullopt when there is none.
std::optional<std::filesystem::path> original_of(const std::filesystem::path& source,
                                                 std::span<const std::filesystem::path> candidates);

// ---- assembling and linking ------------------------------------------------------------------------------------

// What one tool run made: the file, or nullopt when the tool failed; and its messages (stdout then stderr, trimmed).
struct ToolResult {
    std::optional<std::filesystem::path> file;
    std::string log;
};

// `source` assembled into `work`. Fails with a message when there is no assembler or kit.
ToolResult assemble(const Tools& tools, const std::filesystem::path& source, const std::filesystem::path& work);

struct LinkOptions {
    bool dynamic = false; // loaded on demand (a `virtdevice`), not at start-up
    // [(segment class, attributes)] replacing the kit's classes, for source that names its own (one per object of the
    // original); empty: the kit's.
    std::vector<std::pair<std::string, std::string>> classes;
    // false exports the DDB by ordinal only, with no name -- which is how a member of a container (VMM in VMM32.VXD)
    // exports it, the name living in the container's own directory.
    bool named = true;
    // The text of the driver's .def: its DESCRIPTION and EXPORTS (name @ordinal) are linked as it says -- the
    // description and the exports go in the non-resident name table, as Microsoft's LINK put them.
    std::string definitions;
};
// Link one object file into a driver exporting <device>_DDB, written to `out` (with its .lnk and .map beside it).
ToolResult link(const Tools& tools, const std::filesystem::path& obj, std::string_view device,
                const std::filesystem::path& out, const LinkOptions& options = {});

// ---- the structural check ----------------------------------------------------------------------------------------

// What `compare` looks at, for one image.
struct Structure {
    struct Object {
        std::uint32_t size = 0, flags = 0;
        friend bool operator==(const Object&, const Object&) = default;
    };
    struct Export {
        std::optional<std::string> name;
        std::uint32_t ordinal = 0;
        friend bool operator==(const Export&, const Export&) = default;
    };
    std::vector<Object> objects;
    std::optional<formats::le::Descriptor> descriptor;
    std::vector<std::optional<formats::le::Place>> services;
    std::vector<Export> exports;
    bool export_is_descriptor = false;
    std::uint32_t module_flags = 0;
    std::size_t references = 0; // places the loader patches
};
Structure structure(const formats::le::Module& image);

struct Checks {
    bool objects = true, descriptor = true, services = true, exports = true, module = true;
    bool all() const { return objects && descriptor && services && exports && module; }
};
struct Comparison {
    Checks checks;
    std::vector<std::string> notes; // one line for each check that failed
    // The share of bytes equal outside patched places, over the objects that pair up by number and size; nullopt when
    // none do.
    std::optional<double> same;
};
// Check a rebuilt image against the original.
Comparison compare(const formats::le::Module& built, const formats::le::Module& original);

// ---- the whole build -------------------------------------------------------------------------------------------

struct BuildRequest {
    std::filesystem::path source;                  // a folder or an .asm
    std::filesystem::path work;                    // objects, linker script, map and image go here
    std::optional<std::filesystem::path> original; // the driver to check against (see original_of)
};
struct BuildResult {
    std::string name;             // the source's name, lower case
    std::filesystem::path source; // the .asm
    bool assembled = false, linked = false;
    std::optional<std::filesystem::path> path; // the rebuilt driver
    std::string log;
    std::optional<std::filesystem::path> original;
    std::optional<Checks> checks; // when there is an original and the rebuilt file could be read
    std::vector<std::string> notes;
    std::optional<double> same;
};
// Assemble and link the driver whose source is `request.source`; with an original, check the result against it.
// UsageError for a folder without its .asm; everything else that goes wrong is in the result.
BuildResult build(const Tools& tools, const BuildRequest& request);

// One line for a build result.
std::string report(const BuildResult& result);

// ---- the command -------------------------------------------------------------------------------------------------

// What the command needs of the rest of the program.
struct Environment {
    Tools tools;
    // A corpus name or a file -> the driver (corpus.retail); nullopt when there is none.
    std::function<std::optional<std::filesystem::path>(std::string_view)> retail;
    std::vector<std::filesystem::path> corpus_drivers; // every driver of the corpus (original_of looks here)
    std::filesystem::path state;                       // builds go in <state>/driver_build/<name>
};
// `driver-build <source folder or .asm> ... [--original FILE] [--out DIR]`: prints a line for each source and the notes
// of its failed checks. Exit code 1 when a source did not build or did not check out.
cli::Command command(const Environment& environment);

} // namespace cb::build::driver
