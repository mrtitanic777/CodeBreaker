// codebreaker/exact/library.hpp -- typed C++ port of codebreaker.exact.library.
//
// library.py -- the library modules a program was linked with, found in it; the rest of its library part as stand-ins.
//
//     python -m codebreaker exact PROJECT library [--write] [-v]
//
// A linked program holds each library module it took byte for byte, but for the words the linker
// filled in. Every module of the candidate libraries (the toolchain profile's for the project's
// model, then the project's own "libraries") is looked for in the program: its code (each code
// segment, the filled words left out) among the program's code, its data in the data segment.
//
//   - code found at one place places the module; where several modules have the same bytes there
//     (one routine in several libraries, or entry points that share a body) the one whose public
//     name the program's callers use (SYMBOLS) is taken, else the earliest library's; the others are
//     noted ("also")
//   - a module of data only is placed by its data, else by its names' places (SYMBOLS, or the words
//     of the placed modules' code that name them)
//   - the library part is the run of placed modules at the end of the code: it begins after the last
//     gap longer than `gap` bytes (project "library_gap", default 4096) before them - a lone module
//     among the program's own functions is not its start (project "library_start" says it outright)
//   - inside the library part, code no module accounts for becomes a stand-in (RAW: the program's own
//     bytes), with the public names the placed modules' calls into it show
//
// The linker takes modules in the order it comes to need them, which hangs on the order of names in
// the objects that need them, not ours - so a rebuild links them as objects in the program's order:
// src/CRTLIB, one line each:
//
//     [TAG:]NAME        # code 03bf8 (334 bytes)  data 1520  (also ...)
//     RAW name code=LO-HI pub=NAME@OFF ...        # (no library module has these bytes)
//
//     library::find(project) -> Map (.modules [Placed], .start, .raws, .lines())
//
// Typing notes (how the Python's duck-typed values are typed here):
//   * `prj` is the interface `Project` below: the members of exact.project.Project (and of the toolchain profile
//     behind prj.toolchain().p) that library.py reads. It is a modules::Project (exact/modules.hpp: image, dgroup,
//     symbols(), code_end, code_base(), path(), ...) because the Python calls exact.modules.starts(prj) with it, and
//     adds what library.py reads of prj.cfg and of the profile: library_gap, library_start, model, the project's own
//     "libraries", "first_objects", "data_sizes", "data_pads", the profile's "libraries" and "objects" of a model, its
//     "kit", and prj.dir. `prj._library` (locate()'s cache on the project) is the member `_library`.
//   * cfg's "data_sizes" and "data_pads" hold numbers (an int, or text for int()); the values are ints here (a
//     number written as text is for the project's implementation to read), the pads' keys are hexadecimal text.
//   * The Python's tuples are std::tuple: Member.code is [(segment, filled bytes, holes)], Member.data is
//     (bytes, holes) or None, Member.pubs is {public name: (segment or None, offset)}; a raw stand-in is
//     (start, end, [(offset, public name)]) and a raw data run (data from, to, {public: offset}, note).
//   * The rows of Map.order are (address or None, kind, item): the item is a Placed, a DataRaw or a raw stand-in
//     (the Python's tuple), a std::variant of those.
//   * DataRaw.m is a kit Member, or a `_Named` (a kit module's stand-in under another name): the member (null for
//     a _Named) and `name` (the Member's, or the _Named's) are kept; `raw` is _Named.raw (b"" for none).
//   * Member.l2 is the module's L2 document (translate::l2::Dict); a public's place is read back from it, as the
//     Python reads it.
//   * `max(set(segs), key=segs.count)` (the main code segment) breaks a tie by the Python's set order, which is
//     arbitrary; here the first of the tied segments in the order the modules are placed.
//   * main() prints with `print`: the text goes to the function given (std::cout where none is).
//   * Python ints are int64_t.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/exact/modules.hpp"
#include "codebreaker/exact/omf.hpp"
#include "codebreaker/translate/l2.hpp"
#include "py.hpp"

namespace codebreaker::exact::library {

namespace omf = codebreaker::exact::omf;
namespace l2 = codebreaker::translate::l2;

// W = lambda b, k: struct.unpack_from("<H", b, k)[0]
int64_t W(const py::bytes& b, int64_t k);

// (segment, the bytes the object fills, the offsets the linker fills in)
using CodeSeg = std::tuple<py::str, py::bytes, py::set<int64_t>>;
// (the bytes of _DATA, the offsets the linker fills in)
using DataSeg = std::tuple<py::bytes, py::set<int64_t>>;
using Pub = std::tuple<std::optional<py::str>, int64_t>;   // (segment or None, offset)

// One module of a library (an object module in a .LIB). What it holds is its L2 Module (the one
// module model, l2.module): each public name a MachineFragment at its place (origin: the member,
// its segment's number among the member's segments, the offset) -- code or data the program links
// byte for byte. The bytes themselves, the places the linker fills in (holes) and the segments'
// lengths are the object's facts, read from it (omf.read): .code, .data, .seglen.
class Member {
  public:
    py::str tag;
    py::str name;
    py::bytes raw;
    int64_t rank = 0;
    omf::Obj o;
    py::list<CodeSeg> code;
    std::optional<DataSeg> data;
    py::list<std::optional<py::str>> segments;
    l2::Dict l2;
    py::dict<py::str, Pub> pubs;   // {public name: (its segment, its offset)}, read from the member's L2 Module

    Member(const py::str& tag, const py::str& name, const py::bytes& raw, int64_t rank);

    // (tag + ":" if tag else "") + name
    py::str label() const;
    // (length, alignment) of a segment: (0, 1) for none
    std::tuple<int64_t, int64_t> seglen(const py::str& seg) const;
};
using MemberP = py::ref<Member>;

class Placed {
  public:
    MemberP m;
    std::optional<int64_t> code;
    std::optional<int64_t> data;
    py::str why;
    py::list<MemberP> also;

    Placed(const MemberP& m, const std::optional<int64_t>& code = std::nullopt, const std::optional<int64_t>& data = std::nullopt,
           const py::str& why = py::str(""));

    // where its code ends: code + the length of the module's first code segment; None without code
    std::optional<int64_t> end() const;
};
using PlacedP = py::ref<Placed>;

// a data-only module of another version than the kit's: its data as the program has it (a stand-in)
class DataRaw {
  public:
    MemberP m;               // the kit's module (null for a _Named: a stand-in under another name)
    py::str name;            // m.name, or the _Named's name (= its label)
    py::bytes raw;           // _Named.raw (b"" for none); the Member's raw otherwise
    int64_t data = 0;
    int64_t end = 0;
    py::dict<py::str, int64_t> pubs;
    py::str why;
    py::list<MemberP> also;
    bool by_next = false;

    DataRaw(const MemberP& m, int64_t lo, int64_t hi, const py::dict<py::str, int64_t>& pubs, const py::str& why);

    // (length, alignment) of a segment: (end - data, 2) for _DATA, else (0, 1)
    std::tuple<int64_t, int64_t> seglen(const py::str& seg) const;
};
using DataRawP = py::ref<DataRaw>;

// (start, end, [(offset, public name)])
using Raw = std::tuple<int64_t, int64_t, py::list<std::tuple<int64_t, py::str>>>;
// (data from, data to, {public: offset}, note)
using RawData = std::tuple<int64_t, int64_t, py::dict<py::str, int64_t>, py::str>;
using Block = std::variant<PlacedP, DataRawP>;
using Item = std::variant<PlacedP, DataRawP, Raw>;
using Row = std::tuple<std::optional<int64_t>, py::str, Item>;
using Refs = py::dict<py::str, std::tuple<py::str, int64_t>>;   // {external name: (C or D, address)}

class Map {
  public:
    std::optional<int64_t> start;
    py::list<PlacedP> coded, donly, first, lone;
    py::list<Raw> raws;
    Refs refs, allrefs;
    py::dict<py::str, std::tuple<py::str, py::list<py::str>>> missing;
    py::list<std::tuple<int64_t, int64_t, py::str>> objects;
    py::list<DataRawP> dataraws;
    py::dict<int64_t, RawData> rawdata;
    py::list<Row> order;
};
using MapP = py::ref<Map>;

// a library of the profile or of the project: its "tag" ("" when it has none) and "path"
struct LibRef {
    py::str tag;
    py::str path;
};

// exact.project.Project (and its toolchain profile), as library.py uses it
class Project : public modules::Project {
  public:
    // prj.cfg.get("model", "S")
    virtual py::str model() const = 0;
    // prj.toolchain().p.get("libraries", {}).get(model, [])
    virtual py::list<LibRef> profile_libraries(const py::str& model) const = 0;
    // prj.toolchain().p.get("objects", {}).get(model, [])
    virtual py::list<py::str> profile_objects(const py::str& model) const = 0;
    // prj.toolchain().p["kit"] (KeyError where the profile has none)
    virtual std::optional<py::str> profile_kit() const = 0;
    // prj.cfg.get("libraries", [])
    virtual py::list<LibRef> project_libraries() const = 0;
    // prj.cfg.get("first_objects", [])
    virtual py::list<py::str> first_objects() const = 0;
    // prj.cfg.get("library_gap", 4096)
    virtual int64_t library_gap() const = 0;
    // prj.cfg.get("library_start"): None, a number, or one written as text (int(x, 0))
    virtual std::optional<std::variant<int64_t, py::str>> library_start() const = 0;
    // prj.cfg.get("data_sizes", {})
    virtual py::dict<py::str, int64_t> data_sizes() const = 0;
    // prj.cfg.get("data_pads", {}): {address in hex text: length}
    virtual py::dict<py::str, int64_t> data_pads() const = 0;
    // prj.dir
    virtual py::str dir() const = 0;

    // prj._library: locate()'s answer, kept
    mutable MapP _library;
};

// [(tag, path)] the libraries to look in, in the order of preference
py::list<std::tuple<py::str, py::str>> candidates(const Project& prj);

py::list<MemberP> modules(const Project& prj);

// every place in img[lo:hi] where d stands, the holes left out
py::list<int64_t> find(const py::bytes& img, const py::bytes& d, const py::set<int64_t>& holes, int64_t lo, int64_t hi);

MapP locate(const Project& prj, bool verbose = false);
MapP _locate(const Project& prj);

py::list<DataRawP> _data_standins(const Project& prj, const py::list<MemberP>& mods, py::set<const Member*>& taken,
                                  py::dict<py::str, std::tuple<py::str, py::list<py::str>>>& missing, const Refs& refs,
                                  const py::list<PlacedP>& coded, const py::list<PlacedP>& donly);

// [(code start, end, object)] the kit's loose objects (the profile's "objects" for the model: SVARSTCK.OBJ,
// SSETARGV.OBJ ...) whose code stands in the program at one place
py::list<std::tuple<int64_t, int64_t, py::str>> _objects(const Project& prj, const py::bytes& img, int64_t dg);

// the publics of the kit's objects the program links before the library (project "first_objects")
py::set<py::str> _first_object_names(const Project& prj);

// {external name: (C or D, address)}: what the words of the placed modules that name externals show - in
// their code, and in their data where its place is known (stdio's _iob names the buffers)
Refs _refs(const Project& prj, const py::list<PlacedP>& placed);

// where the code ends: before the data segment, less the zeros that pad to its paragraph
int64_t _code_end(const py::bytes& img, int64_t dg);

// {code stand-in's start: (data from, to, {public: offset}, note)} - a code stand-in is another version of the
// kit's module that defines its publics; that module's data, as far as the program has it (placed by its first
// run of bytes that are not zeros, as long as the bytes agree, its zeros at the end left out), is the stand-in's
py::dict<int64_t, RawData> _raw_data(const Project& prj, const py::list<MemberP>& mods, const py::list<Raw>& raws, const py::bytes& ds);

// the modules in the linker's order. By code; a module of data only where its data stands among the coded
// modules' data: the library's data is laid out module after module (the lengths and alignments of their
// SEGDEFs), a data-only module going in where the layout reaches its place; of the starts for the library's
// data, the one that puts the most modules whose data's place is known at that place
py::list<Row> _order(const py::list<PlacedP>& coded, const py::list<Block>& donly, const py::list<Raw>& raws,
                     const py::dict<int64_t, RawData>& rawdata = py::dict<int64_t, RawData>());

py::list<py::str> lines(const Map& mp, const py::str& title = py::str(""));

// `print`ed text goes to `out` (std::cout when null)
int64_t main(const Project& prj, const py::list<py::str>& argv, const std::function<void(const py::str&)>& out = nullptr);

}  // namespace codebreaker::exact::library
