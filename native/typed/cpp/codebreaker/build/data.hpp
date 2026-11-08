// codebreaker/build/data.hpp -- typed C++ from codebreaker.build.data.
//
// data.py -- a module's initialised data as C, so a rebuilt module carries its own.
//
// The per-function builds borrow the original's data: a 32-bit function's globals are bound to
// the original's addresses, a 16-bit one's to offsets in the original's data segment. A whole
// module has to bring that data along. This writes it as C the compiler lays out exactly as the
// original has it, with every pointer in it written as a pointer.
//
//     d = data32(path, function_at)        // 32-bit: each data section as one object
//     d = data16(path, function_at)        // 16-bit: the automatic data segment
//     d.source          the C file defining the data
//     d.declarations    what the functions' header needs to refer to it
//     d.pointers        pointers written as pointers;  d.unresolved: [(where, why)] left as numbers
//
// 32-bit. Each data section becomes an object `codebreaker_data_N`: runs of bytes and, at every
// place the module's relocation table marks, a pointer member -- to a reconstructed function by
// name, or to a place in a data object. Exported data is an object of its own under its
// exported name, cut out of its section (`layout_holds` checks the pieces stayed together). The functions' references are
// moved the same way: `relocate_header` rebinds the header's globals, `relocate_text` rewrites
// the address constants in a function. Which constants ARE addresses is not guessed from their
// value: only a constant that appears among the relocated words inside that function's own code
// is rewritten (`address_constants`). A module without a relocation table (most .EXEs) gives no
// such evidence and cannot be moved; `data32` says so.
//
// 16-bit. Near data is addressed by plain offsets from DS, which no table marks, so the data
// segment is not moved: it is written as one object that must be linked FIRST so that it starts
// at offset 0 of the automatic data segment, and every offset in the code stays right. Far
// pointers stored in it (the segment's relocation records) are written as far pointers to the
// function, import or data they name. A selector stored on its own, or a bare offset of a
// function, has no C form here: it is left zero and listed in `unresolved`.
//
// Typing and runtime notes:
//   * The callbacks the Python takes (function_at, segment_symbol) are std::function: function_at(va) for a
//     32-bit module, function_at((segment, offset)) for a 16-bit one, each answering the name of the
//     reconstructed function there or None (an empty name counts as None, as an empty str is falsy in Python).
//   * A piece of an object (the Python's tuples ("bytes", off, blob) | ("zero", off, count) |
//     ("pointer", off, C type, C expression, note)) is the record Piece below; the field a kind does not use is empty.
//   * Data.imports maps a symbol to (module, ordinal) or (module, name): a std::variant<int64_t, py::str> second item.
//   * Virtual addresses are int64_t (the 32-bit modules this is for are far below it); a PE32+ image base is
//     taken modulo 2^64 like the rest of the typed pe port.
//   * far_segments: the Python takes any collection whose membership says which segments the build carries (a
//     tuple, a set, or the dict of segment images); here a py::set<int64_t> of their numbers.
//   * relocate_header: a constant of more than 63 bits in the header is OverflowError (no real header has one);
//     relocate_text reads a constant of any length exactly as far as it matters (see parse_hex_).
#pragma once

#include <cstdint>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "codebreaker/formats/ne.hpp"
#include "codebreaker/formats/pe.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::build::data {

// what a section may hold that the linker or the resource compiler makes anew: exports, imports,
// resources, relocations, debug information
extern const py::list<int64_t> _REMADE;
inline constexpr int64_t _CODE = 0x20000020;

// a code segment's selector, as each compiler can say it in a constant
extern const py::list<py::str> _SELECTOR_OF;

// the name of the reconstructed function at an address, or None
using FunctionAt32 = std::function<std::optional<py::str>(int64_t)>;
// ... at (segment, offset), or None
using FunctionAt16 = std::function<std::optional<py::str>(const std::tuple<int64_t, int64_t>&)>;
// the C symbol of a code segment, or None
using SegmentSymbol = std::function<std::optional<py::str>(int64_t)>;

// A piece of one object: kind is "bytes" (blob), "zero" / "tail" (count) or "pointer" (ctype, expr, note).
struct Piece {
    py::str kind;
    int64_t offset = 0;
    py::bytes blob;
    int64_t count = 0;
    py::str ctype, expr, note;
};

// {offset: (width, C type, expression, note)}
using Pointers = py::dict<int64_t, std::tuple<int64_t, py::str, py::str, py::str>>;

class Data {
  public:
    py::str source, declarations;
    py::list<std::tuple<int64_t, py::str, int64_t, int64_t>> sections;   // (number, name, virtual address, size) -- 32-bit
    py::list<std::tuple<py::str, int64_t, int64_t, int64_t>> parts;      // (C name, virtual address, size, section number): the objects written
    int64_t pointers = 0;
    py::list<std::tuple<py::str, py::str>> unresolved;                   // (where, why)
    py::dict<py::str, std::tuple<py::str, std::variant<int64_t, py::str>>> imports;   // 16-bit: {symbol: (module, ordinal or name)} named only by data
    bool movable = true;
    int64_t size = 0;
    py::dict<std::tuple<int64_t, int64_t>, py::str> exported;            // 16-bit: {(segment, offset): C name} of exported data
    py::list<Piece> pieces;                                              // 16-bit: the segment's pieces, as data16 cut it (see _pieces)
    py::dict<int64_t, py::str> cuts;                                     // 16-bit: {offset: name} where an exported variable starts

    // The C expression for an address inside a carried section, or None.
    std::optional<py::str> place(int64_t va) const;
};

// ---------------------------------------------------------------------------------------------
// writing an object: bytes, runs of zeros, pointers
// ---------------------------------------------------------------------------------------------
py::list<py::str> _rows(const py::bytes& blob);

// C for one object. pieces: ("bytes", offset, blob) | ("zero", offset, count) |
// ("pointer", offset, C type, C expression, note).
py::str _object(const py::str& struct_name, const py::str& name, const py::list<Piece>& pieces, bool far = false,
                const std::optional<py::str>& based = std::nullopt);

// Cut `blob` (padded with zeros to `size`) at the pointer places, and long runs of zeros
// out of the bytes between. pointers: {offset: (width, C type, expression, note)}.
py::list<Piece> _pieces(const py::bytes& blob, int64_t size, const Pointers& pointers);

// ---------------------------------------------------------------------------------------------
// 32-bit
// ---------------------------------------------------------------------------------------------
// {virtual address of a relocated word: the address it holds} from the relocation table.
py::dict<int64_t, int64_t> relocated_words(const formats::pe::PEFile& pe);

// {function address: the set of addresses its code refers to}. `starts` is every function
// address of the module; a function runs to the next one.
py::dict<int64_t, py::set<int64_t>> address_constants(const py::str& path, const py::list<int64_t>& starts);

// The data sections of a 32-bit module as C. `function_at(va)` -> the name of the
// reconstructed function at an address, or None. `named` is {address: C name} for places that
// must be objects of their own (exported data): a section is then written as several objects
// one after another, which only works if the compiler keeps them together -- `layout_holds`
// checks that on the compiled object.
Data data32(const py::str& path, const FunctionAt32& function_at,
            const std::optional<py::dict<int64_t, py::str>>& named = std::nullopt);

// {symbol: (segment index, offset)} from an object file's public-name records.
py::dict<py::str, std::tuple<int64_t, int64_t>> _public_offsets(const py::bytes& obj);

// Whether, in the compiled data object, the objects a section was written as lie one after
// another as they do in the original.
bool layout_holds(const py::bytes& obj, const Data& d);

// Rebind the header's globals from the original's addresses to the carried data.
// -> (header, [addresses that are in no carried section])
std::tuple<py::str, py::list<int64_t>> relocate_header(const py::str& header, const Data& d);

// Rewrite the address constants of one function. `addresses` is what its code refers to
// (see address_constants). -> (text, [addresses that could not be placed])
std::tuple<py::str, py::list<int64_t>> relocate_text(const py::str& text, const py::set<int64_t>& addresses, const Data& d,
                                                     const FunctionAt32& function_at);

// ---------------------------------------------------------------------------------------------
// 16-bit
// ---------------------------------------------------------------------------------------------
// Every relocated place of a segment: [(offset, source kind, addend, record)], following
// the chains the file keeps (each place holds the offset of the next; 0xFFFF ends one).
py::list<std::tuple<int64_t, int64_t, int64_t, formats::ne::Relocation>> _places(const formats::ne::NEFile& nef,
                                                                                  const formats::ne::Segment& seg,
                                                                                  const py::bytes& blob);

// The fix-ups of segment `s` (its file bytes in `blob`, cleared where the loader writes): the
// places that are pointers, {offset: (width, C type, expression, note)}, and the imports named.
// The same for the automatic data segment and for each other data segment the build carries.
// `own` is the number of the segment itself when it is not the automatic one.
std::tuple<Pointers, py::list<py::str>> _relocated(const formats::ne::NEFile& nef, const formats::ne::Segment& s,
                                                   const py::bytearray& blob, int64_t auto_, const py::set<int64_t>& far_segments,
                                                   const FunctionAt16& function_at, const SegmentSymbol& segment_symbol, Data& d,
                                                   const py::str& label = py::str("data"),
                                                   const std::optional<int64_t>& own = std::nullopt);

// The automatic data segment of a 16-bit module as C. `function_at((segment, offset))` ->
// the reconstructed function there, or None; `far_segments` are the other segments the build
// carries as `codebreaker_segment_N`.
Data data16(const py::str& path, const FunctionAt16& function_at, const py::set<int64_t>& far_segments = py::set<int64_t>(),
            const SegmentSymbol& segment_symbol = SegmentSymbol());

// Pieces (see _pieces) cut so that each offset starts one.
py::list<Piece> _cut(const py::list<Piece>& pieces, const py::list<int64_t>& offsets);

// The declaration, in data.c, of another data segment the build carries: an object of its
// own layout (far_segments), named as the code knows it.
py::str _segment_declaration(int64_t n);

// C for the other segments a 16-bit module's code reads from: {number: bytes}. Each is a
// segment of its own, starting with it: code (and assembly above all) reaches into it by the
// original's offsets. With the module (`path`), the places the loader fixes up are what they
// point at -- a string in the automatic segment, a function, a selector -- not the bytes the
// description gave them: SYSDETMG's segment 14, a table of far pointers to its strings and
// functions, kept the made-up selector 1080h, and its hardware detection faulted on it.
// (The code elsewhere declares the segment as bytes; data.c, compiled with CODEBREAKER_DATA,
// defines it with its pointers -- the linker joins them by name.)
// An empty function_at answers None everywhere (the Python's default).
py::str far_segments(const py::dict<int64_t, py::bytes>& segments, const std::optional<py::str>& path = std::nullopt,
                     const FunctionAt16& function_at = FunctionAt16(), const SegmentSymbol& segment_symbol = SegmentSymbol());

}  // namespace codebreaker::build::data
