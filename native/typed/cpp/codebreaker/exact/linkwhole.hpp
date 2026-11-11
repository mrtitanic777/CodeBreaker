// codebreaker/exact/linkwhole.hpp -- typed C++ port of codebreaker.exact.linkwhole.
//
// linkwhole.py -- a program linked whole from its source files by a period linker, and compared with the original.
//
//     python -m codebreaker exact PROJECT link [--keep DIR] [-v]       (a project whose "link" names a "linker")
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
//                  its end; "record": as many as fill its last 128-byte record), {"header": [FROM, TO]} (header bytes FROM..TO-1, and the checksum counting them)
//     "psp"        the segment DOS loads the linker at (some linkers leave their own addresses in the header)
//
// Typing notes:
//   * `prj` of the Python is a codebreaker.exact.project.Project (not yet ported). Here it is the abstract class Project
//     below: what linkwhole reads of it (prj.dir, prj.cfg["link"], prj.sets(), the sources, headers, include and switches of
//     each set, prj.image / prj.file / prj.exe, prj.symbols()). The set of sources `part` of the Python is a Project too
//     (prj.sets() is [prj] + the "also" sets, as in project.py).
//   * prj.cfg["link"] is the struct LinkConfig (a dict of known keys); link_config() reads it from the JSON text of that
//     dict.  An absent or empty "link" is the default LinkConfig, which `wanted` is not.
//   * A toolchain is the abstract modules::Toolchain (what compile() needs); toolchain::load() of the typed port hands one out.
//   * SystemExit(message) is py::SystemExit; print() goes to the ostream given to main().
//   * linkemu's functions are picked by name (`getattr(linkemu, lk["emulate"])`): only link24 is there, any other name is an
//     AttributeError.
#pragma once

#include <cstdint>
#include <iosfwd>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/exact/linkemu.hpp"
#include "codebreaker/exact/modules.hpp"
#include "py.hpp"

namespace codebreaker::exact::linkwhole {

using Num = std::variant<int64_t, py::str>;                  // an int, or a number written as text ("0x20")

struct Find {                                                 // "find": [{"file", "toolchain", "variant"}]
    py::str file;
    py::str toolchain;
    std::optional<py::str> variant;
};

struct ClassSpec {                                            // "class": {CLASS: {"segments": [...], "files": [...]}}
    py::list<py::str> segments;
    py::list<py::str> files;
};

struct Accept {                                               // "accept": [{"tail_zeros" | "header", "why"}]
    std::optional<Num> tail_zeros;                            // N, or "record"
    std::optional<std::tuple<Num, Num>> header;               // [FROM, TO]
    py::str why;                                              // (default "")
};

struct LinkConfig {                                           // prj.cfg["link"]
    bool has_linker = false;                                  // "linker" in the dict
    std::optional<py::str> linker;
    bool has_emulate = false;                                 // "emulate" in the dict
    py::str emulate;                                          // lk.get("emulate"): "" when none
    py::str order;                                            // lk.get("order"): "" when none
    py::str switches;                                         // lk.get("switches", "")
    py::str output = py::str("NEW");                          // lk.get("output", "NEW")
    bool map = true;                                          // lk.get("map", True) as a truth value
    std::optional<py::str> names;                             // lk.get("names")
    std::optional<py::str> placed;                            // lk.get("placed")
    py::list<py::str> last;
    py::list<Find> find;
    py::list<std::tuple<py::str, py::str>> first;             // [NAME, file]
    py::list<py::str> libraries;
    py::dict<py::str, ClassSpec> class_;                      // "class"
    py::list<Accept> accept;
    std::optional<Num> psp;                                   // an int or a number written as text
};

// the dict prj.cfg["link"] (JSON text) as a LinkConfig
LinkConfig link_config(const py::str& json_text);

class Project {
  public:
    virtual ~Project() = default;
    virtual py::str dir() const = 0;                                                  // prj.dir
    virtual py::str src() const = 0;                                                  // prj.src
    virtual py::list<std::tuple<int64_t, py::str>> sources() const = 0;               // prj.sources() -> [(address, path)]
    virtual std::shared_ptr<modules::Toolchain> toolchain() const = 0;                // prj.toolchain()
    virtual py::dict<py::str, py::bytes> headers() const = 0;                         // prj.headers()
    virtual py::list<py::str> include() const = 0;                                    // prj.include
    virtual std::optional<py::str> switches() const = 0;                              // prj.switches
    virtual py::list<std::shared_ptr<const Project>> sets() const = 0;                // prj.sets(): [prj] + the other sets
    virtual LinkConfig link() const = 0;                                              // prj.cfg.get("link") or {}
    virtual py::bytes image() const = 0;                                              // prj.image
    virtual py::bytes file() const = 0;                                               // prj.file
    virtual py::str exe() const = 0;                                                  // prj.exe
    virtual linkemu::Symbols symbols() const = 0;                                     // prj.symbols()
};

// the project says it is linked by a period linker
bool wanted(const Project& prj);

// the modules a file lists in the order they were linked: "name [odd]" a line, each a file of one of the
// project's source sets, compiled by its toolchain -> [(k, name, object bytes, odd)]
py::list<std::tuple<int64_t, py::str, py::bytes, bool>> listed(const Project& prj, const LinkConfig& lk);

// [(place, file name, object bytes)] in link order
py::list<std::tuple<int64_t, py::str, py::bytes>> modules(const Project& prj);

// the image address of the object's code: the one place its bytes (but those the linker fills in) are found
std::optional<int64_t> _found(const Project& prj, const py::bytes& raw);

// the object module up to its end record (an assembler's file may be padded to its disk sector)
py::bytes _own(const py::bytes& b);

py::str _file(const Project& prj, const py::str& spec);

py::str _name(const std::optional<py::str>& scheme, int64_t k, const py::str& f);

// an OMF index at q: (the index, where it ends)
std::tuple<int64_t, int64_t> _index(const py::bytes& b, int64_t q);

// the object with its segments of those names (a name, or one ending in _NAME) in the class cls
py::bytes reclass(const py::bytes& obj, const py::str& cls, const py::list<py::str>& segments);

// -> (the file written or None, the linker's words, its map)
std::tuple<std::optional<py::bytes>, py::str, py::str> run(const Project& prj, const py::list<std::tuple<int64_t, py::str, py::bytes>>& objs,
                                                           const std::optional<py::str>& keep = std::nullopt);

// -> (identical, [lines]): the file against the original, but for what the project accepts
std::tuple<bool, py::list<py::str>> compare(const Project& prj, const py::bytes& exe);

// `python -m codebreaker exact PROJECT link ...`: what it prints goes to `out`; the exit code
int64_t main(const Project& prj, const py::list<py::str>& argv, std::ostream& out);

}  // namespace codebreaker::exact::linkwhole
