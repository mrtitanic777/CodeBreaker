// codebreaker/formats/omf.hpp -- typed C++ of codebreaker.formats.omf
//
// omf.py -- Intel/Microsoft OMF-86 object modules: read one whole, write the few kinds exact makes.
// Answered by the C++ (native/formats/omf: formats.dll); its Python reader is gone (docs/NATIVE.md,
// step 4), but for patched and rename_symbols (a spec read by a regular expression; a Python function
// applied to every name).
// (Also the 32-bit records -- SEGDEF, PUBDEF, LEDATA, LIDATA, FIXUPP with 4-byte offsets -- that a
// 32-bit compiler of the period writes, e.g. MSVC 2.0's: build/masm.py, invert/msvc2.)
//
//     o = omf::read(data)
//         o.seg      {segment name: bytes}            (LEDATA and LIDATA laid out)
//         o.segs     [(name, class, length, align)]   in the order defined
//         o.groups   {group: [segment names]}
//         o.pubs     [(name, segment or None, offset)]
//         o.ext      [names]                          (EXTDEF and COMDEF)
//         o.comm     {name: (size, far)}              the communal variables (COMDEF)
//         o.fix      [(segment, offset, kind, target, displacement)]
//         o.fixloc   [the fixup's 4-bit location field], one per o.fix (9, 11, 13: 32-bit kinds)
//                    kind: 'off' 'base' 'ptr' 'lo' 'rel' (self-relative); target: ('ext', name) /
//                    ('seg', name) / ('grp', name)
//         o.raw      the bytes read
//     omf::functions(o, seg="_TEXT")  -> [(name, offset, length)] the segment's publics, in order
//     omf::code_segments(o)           -> the object's code segments
//     omf::holes(o, seg)              -> offsets of the segment the linker fills in
//     omf::library(data)              -> [(module name, bytes)] of a .LIB
//     omf::records(data)              -> [(type, payload)]
//     omf::record(type, payload)      -> one record's bytes, checksum included
//     omf::name(s)                    -> a length-prefixed name
//
// Typing notes (how the Python's dynamic parts are typed here):
//   * The readers are the C++ of native/formats/omf (omf.cc, linked in: no ctypes, no pickle); what the
//     Python got back as pickled Python values these functions return as typed values.  An exception the
//     reader raised comes back as the py:: exception of the same Python type (struct.error is
//     py::StructError).  A question the C++ leaves to its caller (`Fallback`: an a.out object given to
//     _code / code_segment / function_bytes / names_of) is thrown as the `Fallback` below.
//   * The Python's `Obj` (an attribute bag; omf.read gives one, so does exact.aout.read) is the struct
//     `Obj`.  An a.out object has no fixloc / combine / start (reading them was an AttributeError) and has
//     `syms`; `from_aout` says which kind it is.  A target is (kind, name | index | None): the std::variant.
//   * walk() and optional-header style generators are the class `Walk`: the records up to a failure, then
//     the failure raised by the call to next() that reaches it.
//   * Names (title, seg, text, the keys of a dict of publics) are str; the Python's native layer also took
//     bytes there, which no caller gives.
//   * A bytearray argument (the Python took it for `data`) is not a separate type here: pass bytes.
//   * main(argv): Python's main takes argv (sys.argv[1:]); sys.exit is the returned status.
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::formats::omf {

// native.Fallback: the C++ leaves this question to its caller
struct Fallback : py::Exception {
    Fallback() : py::Exception() {}
    explicit Fallback(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "Fallback"; }
};

// ('seg' | 'grp' | 'ext', name) -- for a 'grp' whose name is not known, its index; ('?', None)
using Target = std::tuple<py::str, std::variant<std::monostate, py::str, int64_t>>;
using Fixup = std::tuple<py::str, int64_t, py::str, Target, int64_t>;         // (segment, offset, kind, target, displacement)
using Public = std::tuple<py::str, std::optional<py::str>, int64_t>;          // (name, segment or None, offset)
using SegDef = std::tuple<py::str, py::str, int64_t, int64_t>;                // (name, class, length, align)
using Sym = std::tuple<py::str, int64_t, int64_t>;                            // (a.out only) (name, type, value)

struct Obj {
    py::dict<py::str, py::bytes> seg;
    py::list<SegDef> segs;
    py::dict<py::str, py::list<py::str>> groups;
    py::list<Public> pubs;
    py::list<py::str> ext;
    py::dict<py::str, std::tuple<int64_t, bool>> comm;
    py::list<Fixup> fix;
    py::list<int64_t> fixloc;                                   // (OMF only)
    py::dict<py::str, int64_t> combine;                         // (OMF only) segment name -> combination
    std::optional<std::tuple<py::str, int64_t>> start;          // (OMF only) the start address, if MODEND gives one
    py::dict<py::str, int64_t> filled;
    py::list<Sym> syms;                                         // (a.out only)
    py::bytes raw;
    bool from_aout = false;
};

// The records of an OMF object: after the records (offset, type, payload), the error the walk met.
class Walk {
  public:
    // (offset, type, payload), or nullopt at the end (StopIteration); the failure the walk met is thrown
    // when the records before it have been given
    std::optional<std::tuple<int64_t, int64_t, py::bytes>> next();

    py::list<std::tuple<int64_t, int64_t, py::bytes>> items_;
    std::optional<std::tuple<py::str, py::str>> error_;        // (exception name, message) -- not yet raised
    int64_t pos_ = 0;
};

extern const py::dict<int64_t, int64_t> _WIDTH;                 // what the linker fills in, by a fix-up's location field: the bytes it covers
extern const py::dict<int64_t, py::str> _KIND;                  // record type -> its name (the listing)

// ---- the C++ reader (native/formats/omf)
py::list<std::tuple<int64_t, py::bytes>> records(const py::bytes& data);
py::bytes record(int64_t t, const py::bytes& payload);        // one record's bytes, checksum included
py::bytes name(const py::str& s);                              // a length-prefixed name (a str is encoded latin1)
py::bytes name(const py::bytes& s);                            // (the same, for bytes)

// the module with bytes of a segment's data replaced: spec SEG+OFF=HEXBYTES (in its LEDATA records)
py::bytes patched(const py::bytes& raw, const py::str& spec);

// The object module `data` read whole (an omf.Obj: see the module docstring).
Obj read(const py::bytes& data);

// [(name, module bytes)] of an OMF library (.LIB), in its order; a name met again is NAME#2, #3 ...
py::list<std::tuple<py::str, py::bytes>> library(const py::bytes& data);

// the offsets of a segment's bytes the linker fills in
py::set<int64_t> holes(const Obj& o, const py::str& seg);

// the object's code segments, the first named first (a compiler's NAME_TEXT, or more by #pragma alloc_text)
py::list<py::str> code_segments(const Obj& o);

// [(name, offset, length)] of a code segment, in order. The last ends where the segment's data end (a SEGDEF
// may count a byte more: C 5.0 at times counts one past a closing NOP); code before the first name is
// "(module start)" (an assembly module's)
py::list<std::tuple<py::str, int64_t, int64_t>> functions(const Obj& o, const py::str& seg = py::str("_TEXT"));

// the object with external names changed (a compiler that calls its helpers by other names than the
// library the program was linked with)
py::bytes rename_externs(const py::bytes& data, const py::dict<py::str, py::str>& mapping);

// the object with every external and public name nm made fn(nm) (EXTDEF, COMDEF, PUBDEF): the names a batch
// of candidates was compiled under given back.  The names are given to fn in the order the records hold them.
py::bytes rename_symbols(const py::bytes& data, const std::function<py::str(const py::str&)>& fn);

// an object module of the program's own bytes: a run of its code (and of its data), with the public names
// others call it by - a stand-in for a part no source and no library module gives. Its words for the linker
// are the program's already, so it stands only where it stood.
py::bytes raw_object(const py::str& title, const py::bytes& code, const py::dict<py::str, int64_t>& pubs,
                     const py::bytes& data = py::bytes(), const py::dict<py::str, int64_t>& dpubs = py::dict<py::str, int64_t>(),
                     const py::str& text = py::str("_TEXT"), int64_t dalign = 1);

// an object module of variables without a value, each a public name at its place in _BSS (start..end): the
// linker places communal names by its own table's order (a hash of the names), and ours are not the original
// names - so they are given their places outright and nothing communal is left to place
py::bytes bss_object(const py::dict<py::str, int64_t>& names, int64_t start, int64_t end,
                     const py::str& title = py::str("bssvars"), const py::str& seg = py::str("_BSS"), bool para = false);

// (the public names an object module defines, the external names it needs)
std::tuple<py::set<py::str>, py::set<py::str>> names_of(const py::bytes& data);

// Exact public symbol names from an OMF object's PUBDEF records (0x90/0x91). Length-prefixed
// names, so no greedy-match errors -- the authoritative `_name@N` decorations wcc386 emitted.
// It reads only those records, so it reads objects read() cannot (names_of: the full reader).
py::set<py::str> publics(const py::bytes& data);

// External symbol names an OMF object refers to (EXTDEF records, 0x8C).
py::list<py::str> externs(const py::bytes& data);

// ---- record by record, for the readers that walk the records themselves (invert's quad_align)
// (offset, type, payload) of each record from `start`, the payload without its checksum: zero bytes
// between records (a library's padding) passed over, the walk ended by a LIBEND
Walk walk(const py::bytes& data, int64_t start = 0);

// an OMF index at q: (value, the offset past it)
std::tuple<int64_t, int64_t> _idx(const py::bytes& r, int64_t q);
std::tuple<int64_t, int64_t> index(const py::bytes& r, int64_t q);

// ---- one object's code, cut at its publics (build/masm, build/msc6, the ne_asm comparers, invert's probes)
// (code segment's name, its bytes, {offsets the linker fills in}, {public: (segment, offset)}): the code
// segment is the first whose class is CODE or whose name ends TEXT; its bytes run to the end of its data;
// a fix-up covers the bytes of its kind.
std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>, py::dict<py::str, std::tuple<py::str, int64_t>>> _code(const py::bytes& data);

// (name, bytes, {offsets the linker fills in}) of an object file's code segment.
std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>> code_segment(const py::bytes& data);

// {public name: (bytes, {offsets the linker fills in})} of an object file's code segment, cut between
// consecutive public names. A name keeps the compiler's leading underscore.
py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> function_bytes(const py::bytes& data);

// ---- a listing: python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]
// python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]
// each module's segments, publics and externals; --raw every record (type, length)
int64_t main(const py::list<py::str>& argv);

}  // namespace codebreaker::formats::omf
