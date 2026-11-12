// cb/exact/linkwhole.hpp -- a program linked whole from its source files by a period linker, and compared with the
// original.
//
//     codebreaker exact PROJECT link [--keep DIR] [-v]       (a project whose "link" names a "linker")
//
// For a program made of whole source files (each a module: PL/M, assembler, C compiled whole), each file of each
// source set is compiled by its set's toolchain, the objects are put in the order of their places in the program
// (the address in each file's name), and the linker the project names is run on them under the DOS emulator. The
// file it writes is compared with the original byte for byte.
//
// project.json "link":
//     "linker"     the LINK.EXE to run (a path from the project's folder, like "image")
//     "switches"   its switches, e.g. "/STACK:3072"
//     "output"     the run file's name (default NEW);  "map": false - no map (NUL), else one of that name
//                  (the names change nothing the linker makes of the modules, but some linkers leave a trace of
//                  their names' places in the header)
//     "names"      the objects' names: "sequence:M" (default: MA, MB, .. - short, so the command line holds
//                  them), "pairs:D" (DA .. DZ, EA ..), or "files"
//     "placed"     the pattern of files of no code that have a place in the order all the same (group 1 the
//                  address), compiled by the first set's toolchain - e.g. a module of shared declarations
//     "last"       files of no code and no place: after all the others, in this order
//     "find"       [{"file", "toolchain", "variant"}]: modules with no place in their name, each placed where
//                  its own code is found in the program (the bytes the linker fills in passed over)
//     "first"      objects before all - [NAME, file]: NAME.OBJ on the command line
//     "libraries"  libraries after the objects
//                  (a file is from the project's folder, or "KIT:path" in a kit)
//     "class"      {CLASS: {"segments": [names], "files": [files]}}: those files' segments of those names are given
//                  the class CLASS before the link (a class the compiler cannot give: the linker orders by class)
//     "accept"     what may differ, each with "why": {"tail_zeros": N} (the original has N zero bytes more at
//                  its end; "record": as many as fill its last 128-byte record), {"header": [FROM, TO]} (header
//                  bytes FROM..TO-1, and the checksum counting them)
//     "psp"        the segment DOS loads the linker at (some linkers leave their own addresses in the header)
//     "emulate"    a linker done here instead of run (cb/exact/linkemu: "link24"); "order" then names a file that
//                  lists the modules in the order they were linked
//
// What the module asks of its surroundings -- the project, the toolchains, the DOS emulator that runs the linker --
// it asks through the small interfaces below; exact/project, exact/toolchain and exact/emu/dos fill them.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/exact/linkemu.hpp"
#include "cb/exact/modules.hpp"
#include "cb/exact/symbols.hpp"

namespace cb::exact::linkwhole {

// The program cannot be linked as the project says: a source file no set has, a file that does not compile, code
// that is not found in the program, a linker the module does not know.
class LinkError : public Error {
public:
    explicit LinkError(const std::string& what) : Error(what) {}
    CB_ERROR_BOILERPLATE(LinkError)
};

// ---- the "link" entry of project.json ------------------------------------------------------------------------------

// How the objects are named on the linker's command line: "sequence:M" (MA .. MZ, M0 .. M9, M36 ..), "pairs:D" (DA ..
// DZ, EA ..) or "files" (the file's name cut to eight letters and digits).
struct NameScheme {
    enum class Kind { Sequence, Pairs, Files };
    Kind kind = Kind::Sequence;
    char prefix = 'M';   // the first letter of a sequence's names; the first letter of the first pair's
};

struct Find {
    std::string file;
    std::string toolchain;
    std::optional<std::string> variant;
};

struct FirstObject {   // "first": [NAME, file] -- NAME.OBJ on the command line, taken from the file
    std::string name;
    std::string file;
};

// The segments of these names (a name, or one ending in _NAME) of these files are given the class.
struct ClassSpec {
    std::vector<std::string> segments;
    std::vector<std::string> files;
};

// What may differ from the original. One entry says one thing.
struct Accept {
    struct TailZeros {
        bool fill_record = false;   // "record": as many as fill the last 128-byte record of the file
        std::uint32_t count = 0;    // otherwise this many (0: nothing is accepted)
    };
    struct HeaderRange {
        std::uint32_t from = 0, to = 0;   // bytes from..to-1
    };
    std::optional<TailZeros> tail_zeros;
    std::optional<HeaderRange> header;
    std::string why;
};

struct LinkConfig {
    std::optional<std::string> linker;
    std::optional<std::string> emulate;   // the name of a linker done here
    std::optional<std::string> order;
    std::string switches;
    std::string output = "NEW";
    bool map = true;
    std::optional<NameScheme> names;
    std::optional<std::string> placed;    // a pattern of file names; group 1 is the place in hex
    std::vector<std::string> last;
    std::vector<Find> find;
    std::vector<FirstObject> first;
    std::vector<std::string> libraries;
    OrderedMap<std::string, ClassSpec> classes;   // by class, in the order the project gives them
    std::vector<Accept> accept;
    std::optional<std::uint32_t> psp;
};

// The "link" entry as project.json writes it (numbers may be written as text: "0x20"). ParseError for an entry that
// is not one. The empty text, or `null`, is the default LinkConfig (which `wanted` rejects).
LinkConfig parse_link_config(std::string_view json_text);

// ---- what the module asks of its surroundings ----------------------------------------------------------------------

class Project {
public:
    virtual ~Project() = default;
    virtual std::filesystem::path dir() const = 0;                         // the project's folder
    virtual std::filesystem::path src() const = 0;                         // this set's source folder
    virtual std::vector<std::pair<std::uint32_t, std::filesystem::path>> sources() const = 0;   // (address, file) of this set
    virtual std::shared_ptr<const modules::Toolchain> toolchain() const = 0;
    virtual std::map<std::string, Bytes> headers() const = 0;
    virtual std::vector<std::string> include() const = 0;
    virtual std::optional<std::string> switches() const = 0;
    // The project's own set first, then the sets after it ("also"). Each has its own folder, toolchain and headers.
    virtual std::vector<std::shared_ptr<const Project>> sets() const = 0;
    virtual const LinkConfig& link() const = 0;
    virtual ByteView image() const = 0;                                    // the original program's image
    virtual ByteView file() const = 0;                                     // the original program's whole file
    virtual std::string exe() const = 0;                                   // its name, as the project gives it
    virtual SymbolTable symbols() const = 0;
    // A file of the link entry ("image", or "KIT:path" in a toolchain's kit), as a path.
    virtual std::filesystem::path resolve(std::string_view spec) const = 0;
    // The toolchain a "find" names.
    virtual std::shared_ptr<const modules::Toolchain> load_toolchain(std::string_view name, const std::optional<std::string>& variant) const = 0;
};

// The DOS emulator, as far as running the linker goes: run `exe` in `workdir` with the command line, give what it
// wrote to the console (Latin-1 text as bytes). `psp` is the segment it is loaded at.
class DosHost {
public:
    virtual ~DosHost() = default;
    virtual std::string run(const std::filesystem::path& exe, const std::filesystem::path& workdir, std::string_view command_line,
                            std::optional<std::uint32_t> psp) const = 0;
};

// ---- linking ---------------------------------------------------------------------------------------------------

// The project says it is linked by a period linker (or one done here).
bool wanted(const LinkConfig& config);

// A module of the file "order" lists: "name [odd]" a line, each a file of one of the project's source sets.
struct ListedModule {
    std::size_t index = 0;
    std::string name;
    Bytes object;
    bool odd = false;   // the code ends on an odd byte: the module after it is byte aligned
};
// Each compiled by its set's toolchain. LinkError when no set has a file or one does not compile.
std::vector<ListedModule> listed(const Project& project, const LinkConfig& config);

constexpr std::uint32_t kLastPlace = 0x100000;   // the places of "last" files: after every other, in the order given

// One object of the program, in link order.
struct PlacedObject {
    std::uint32_t place = 0;   // an address in the program; kLastPlace + k for the k'th of "last"
    std::string name;          // the file's name
    Bytes object;
};
// Every file of every set, put in the order of its place. LinkError when a file does not compile or a "find"
// is not found in the program.
std::vector<PlacedObject> placed_objects(const Project& project);

// The image address of the object's code: the one place its bytes (but those the linker fills in) are found; none
// when there are several places, none, or too few bytes (under eight) to tell by.
std::optional<std::uint32_t> place_of(const Project& project, ByteView object);

// The object module up to its end record (an assembler's file may be padded to its disk sector).
ByteView own_records(ByteView object);

// The name of the k'th object on the linker's command line.
std::string object_name(const std::optional<NameScheme>& scheme, std::size_t k, std::string_view file);

// The object with its segments of those names (a name, or one ending in _NAME, as the object spells it in capitals)
// in the class `cls`. FormatError for a record that runs past the end; UnsupportedError for a name index that takes
// two bytes.
Bytes reclass(ByteView object, std::string_view cls, const std::vector<std::string>& segments);

struct LinkOutput {
    std::optional<Bytes> exe;   // the file the linker wrote, if it wrote one
    std::string console;        // what it said
    std::string map;            // the map it wrote (empty: none)
};
// Link the objects with the project's linker under the DOS emulator, in a folder of its own (deleted after, unless
// `keep` names one).
LinkOutput link(const Project& project, const std::vector<PlacedObject>& objects, const DosHost& dos,
                const std::optional<std::filesystem::path>& keep = std::nullopt);

struct Comparison {
    bool identical = false;   // the same but for what the project accepts
    std::vector<std::string> notes;
};
// The file against the original, but for what the project accepts.
Comparison compare(const LinkConfig& config, ByteView original, ByteView exe);

// `exact PROJECT link [--keep DIR] [-v]`: what it prints goes to stdout; the exit code (0: the file is the original).
// ToolError of tool "link24" and the like are not caught here.
int run(const Project& project, const std::vector<std::string>& args, const DosHost& dos);

}  // namespace cb::exact::linkwhole
