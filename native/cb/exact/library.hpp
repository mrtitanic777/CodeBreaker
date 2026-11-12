// cb/exact/library.hpp -- the library modules a program was linked with, found in it; the rest of its library part
// as stand-ins.
//
//     codebreaker exact PROJECT library [--write] [-v]
//
// A linked program holds each library module it took byte for byte, but for the words the linker filled in. Every
// module of the candidate libraries (the toolchain profile's for the project's model, then the project's own
// "libraries") is looked for in the program: its code (each code segment, the filled words left out) among the
// program's code, its data in the data segment.
//
//   - code found at one place places the module; where several modules have the same bytes there (one routine in
//     several libraries, or entry points that share a body) the one whose public name the program's callers use
//     (SYMBOLS) is taken, else the earliest library's; the others are noted ("also")
//   - a module of data only is placed by its data, else by its names' places (SYMBOLS, or the words of the placed
//     modules' code that name them)
//   - the library part is the run of placed modules at the end of the code: it begins after the last gap longer than
//     `gap` bytes (project "library_gap", default 4096) before them - a lone module among the program's own functions
//     is not its start (project "library_start" says it outright)
//   - inside the library part, code no module accounts for becomes a stand-in (RAW: the program's own bytes), with
//     the public names the placed modules' calls into it show
//
// The linker takes modules in the order it comes to need them, which hangs on the order of names in the objects that
// need them, not ours - so a rebuild links them as objects in the program's order: src/CRTLIB, one line each:
//
//     [TAG:]NAME        # code 03bf8 (334 bytes)  data 1520  (also ...)
//     RAW name code=LO-HI pub=NAME@OFF ...        # (no library module has these bytes)
//
// The Map `locate` returns owns the modules it names (a Placed holds a pointer to one): it is moved, not copied.
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <memory>
#include <optional>
#include <set>
#include <string>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/ordered_map.hpp"
#include "cb/exact/modules.hpp"
#include "cb/exact/x86.hpp"
#include "cb/formats/omf.hpp"

namespace cb::exact::library {

// What the command needs of a project beyond exact/modules (exact/project implements it). The libraries and objects
// are named by the files they are, the kit's paths already worked out.
class Project : public modules::Project {
public:
    struct LibraryFile {
        std::string tag;               // "" or the tag that prefixes the library's modules in src/CRTLIB
        std::filesystem::path file;
    };
    struct DataPad {
        std::uint32_t address = 0, length = 0;
    };
    // The libraries to look in, in the order of preference: the toolchain profile's for the project's model, then
    // the project's own.
    virtual std::vector<LibraryFile> libraries() const = 0;
    // The kit's loose objects for the model (SVARSTCK.OBJ, SSETARGV.OBJ ...); one that is not there is passed over.
    virtual std::vector<std::filesystem::path> kit_objects() const = 0;
    // The objects the program links before the library (project "first_objects").
    virtual std::vector<std::filesystem::path> first_objects() const = 0;
    virtual std::uint32_t library_gap() const { return 4096; }
    virtual std::optional<std::uint32_t> library_start() const = 0;
    // project "data_sizes": the length of a name's data where the kit's module does not say.
    virtual std::map<std::string, std::uint32_t> data_sizes() const = 0;
    // project "data_pads": zeros of the data no module accounts for, which the link's fit found.
    virtual std::vector<DataPad> data_pads() const = 0;
};

// ---- the library's modules -----------------------------------------------------------------------------------------

// The filled bytes of one segment of a module, and the words in it the linker fills in.
struct Piece {
    std::string segment;
    Bytes bytes;
    std::vector<bool> hole;   // hole[k]: byte k is a word the linker fills in (as long as bytes)
    std::size_t hole_total = 0;   // every hole the object names in the segment, those past the filled bytes too
    std::int64_t solid() const { return static_cast<std::int64_t>(bytes.size()) - static_cast<std::int64_t>(hole_total); }   // what can tell
};

struct PublicPlace {
    std::optional<std::string> segment;   // none: an absolute name
    std::uint32_t offset = 0;
};

// One module of a library (an object module in a .LIB).
class Member {
public:
    // FormatError / UnsupportedError for an object this reader does not read.
    Member(std::string tag, std::string name, Bytes raw, int rank);

    std::string tag, name;
    Bytes raw;
    int rank = 0;                       // the library's place among the candidates: -1 for a loose object
    formats::omf::Object object;
    std::vector<Piece> code;            // each code segment that holds bytes
    std::optional<Piece> data;          // _DATA, if it holds bytes
    std::vector<std::string> segments;  // the segments in the order the object defines them (then those only a public names)
    OrderedMap<std::string, PublicPlace> pubs;   // the public names in the order the object gives them

    std::string label() const { return tag.empty() ? name : tag + ":" + name; }
    // The length and alignment (in bytes) of a segment; (0, 1) for none.
    struct SegmentSize {
        std::uint32_t length = 0, align = 1;
    };
    SegmentSize seglen(std::string_view segment) const;
    // The filled bytes of a segment and its holes; none if it has no bytes.
    std::optional<Piece> piece_of(std::string_view segment) const;
};

// The modules of every candidate library, in order (a module this reader cannot read is not a candidate).
std::vector<std::unique_ptr<Member>> read_modules(const Project& project);

// Every place in img[lo, hi) where `piece` stands, the holes left out.
std::vector<std::size_t> find_piece(ByteView img, const Piece& piece, std::size_t lo, std::size_t hi);

// ---- where they stand ----------------------------------------------------------------------------------------------

// A module placed: its code at an address (and its data at an offset of the data segment) of the program.
struct Placed {
    const Member* module = nullptr;
    std::optional<std::int64_t> code;
    std::optional<std::int64_t> data;
    std::string why;                      // how it was placed when its bytes did not say
    std::vector<const Member*> also;      // modules of the same bytes there, not taken
    // Where its code ends: its first code segment's length after `code`; none without code.
    std::optional<std::int64_t> end() const;
};

// A data-only module of another version than the kit's: its data as the program has it (a stand-in).
struct DataRaw {
    std::string name;                     // the kit module's, or the one public it keeps
    const Member* member = nullptr;       // the kit module this is another version of (none: a stand-in under another name, or a pad)
    const Member* source = nullptr;       // the kit module whose bytes it follows (none: a pad)
    std::int64_t data = 0, end = 0;       // where its data stands in the data segment
    OrderedMap<std::string, std::int64_t> pubs;   // name -> offset
    std::string why;
    bool by_next = false;                 // its end is where the next known data begins
};

// A stand-in for code no library module accounts for: the program's own bytes.
struct Raw {
    std::int64_t lo = 0, hi = 0;
    std::vector<std::pair<std::int64_t, std::string>> pubs;   // (offset, name) the placed modules' calls show, by offset
};

// A stand-in's data: where it stands, its public names, and how it was found.
struct RawData {
    std::int64_t from = 0, to = 0;
    OrderedMap<std::string, std::int64_t> pubs;
    std::string note;
};

enum class RefKind { Code, Data };
struct Ref {
    RefKind kind = RefKind::Code;
    std::int64_t address = 0;
};
// What the words of the placed modules that name externals show: each name's place.
using Refs = std::map<std::string, Ref>;

enum class Item { First, Module, Raw, Data };
// The library's part in the order the linker took it.
struct OrderEntry {
    Item item = Item::Module;
    std::variant<const Placed*, const DataRaw*, const Raw*> what;
};

struct Missing {
    std::string named_by;                // the module that names it
    std::vector<std::string> defined_by; // the library modules that define it (another version is in the program)
};

struct Map {
    Map() = default;
    Map(Map&&) noexcept = default;
    Map& operator=(Map&&) noexcept = default;
    Map(const Map&) = delete;
    Map& operator=(const Map&) = delete;

    std::vector<std::unique_ptr<Member>> members;         // everything the rest points into
    std::optional<std::int64_t> start;                    // where the library's part begins
    std::vector<Placed> coded, donly, first, lone;        // with code / data only / bytes tell nothing / among the program's own code
    std::vector<Raw> raws;
    Refs refs, all_refs;
    std::map<std::string, Missing> missing;               // names the placed modules need that nothing placed defines
    struct KitObject {
        std::int64_t start = 0, end = 0;
        std::string label;
    };
    std::vector<KitObject> objects;                       // the kit's loose objects found in the program
    std::vector<DataRaw> data_raws;
    std::map<std::int64_t, RawData> raw_data;             // by the code stand-in's start
    std::vector<OrderEntry> order;
};

// Find the library's modules in the program. `decoder` reads the code stand-ins for the data they address.
Map locate(const Project& project, const x86::Decoder16& decoder = x86::capstone_decoder());

// The modules in the linker's order, as src/CRTLIB writes them.
std::vector<std::string> lines(const Map& map, std::string_view title = "");

// `exact PROJECT library [--write] [-v]`: the CRTLIB text and a summary go to stdout; --write puts the text in
// src/CRTLIB. Returns the command's exit code.
int run(const Project& project, const std::vector<std::string>& args, const x86::Decoder16& decoder = x86::capstone_decoder());

}  // namespace cb::exact::library
