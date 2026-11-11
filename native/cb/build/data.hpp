// cb/build/data.hpp -- a module's initialised data as C, so a rebuilt module carries its own.
//
// The per-function builds borrow the original's data: a 32-bit function's globals are bound to the original's
// addresses, a 16-bit one's to offsets in the original's data segment. A whole module has to bring that data along.
// This writes it as C the compiler lays out exactly as the original has it, with every pointer in it written as a
// pointer.
//
//     Data d = data32(image, "lz32.dll", function_at);   // 32-bit: each data section as one object
//     Data d = data16(module, names);                    // 16-bit: the automatic data segment
//     d.source          the C file defining the data
//     d.declarations    what the functions' header needs to refer to it
//     d.pointers        pointers written as pointers;  d.unresolved: where the rest was left as numbers
//
// 32-bit. Each data section becomes an object `codebreaker_data_N`: runs of bytes and, at every place the module's
// relocation table marks, a pointer member -- to a reconstructed function by name, or to a place in a data object.
// Exported data is an object of its own under its exported name, cut out of its section (`layout_holds` checks the
// pieces stayed together). The functions' references are moved the same way: `relocate_header` rebinds the header's
// globals, `relocate_text` rewrites the address constants in a function. Which constants ARE addresses is not guessed
// from their value: only a constant that appears among the relocated words inside that function's own code is rewritten
// (`address_constants`). A module without a relocation table (most .EXEs) gives no such evidence and cannot be moved:
// `Data::movable` says so.
//
// 16-bit. Near data is addressed by plain offsets from DS, which no table marks, so the data segment is not moved: it
// is written as one object that must be linked FIRST so that it starts at offset 0 of the automatic data segment, and
// every offset in the code stays right. Far pointers stored in it (the segment's relocation records) are written as far
// pointers to the function, import or data they name. A selector stored on its own, or a bare offset of a function, has
// no C form here: it is left zero and listed in `unresolved`.
//
// The reconstructed functions are known to the caller, not to this module: `function_at` answers the C name of the
// function at an address (an empty answer is "none"). Nothing here is global. The images are the caller's: this
// module never opens a file.
#pragma once

#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ne.hpp"
#include "cb/formats/pe.hpp"

namespace cb::build::data {

// ---- the callbacks ------------------------------------------------------------------------------------------------

// The C name of the reconstructed function at a virtual address (32-bit modules); nullopt or "" when there is none.
using FunctionAt32 = std::function<std::optional<std::string>(std::uint64_t va)>;

// A place in a 16-bit module: the number of the segment (1-based, as the module's tables number them) and the offset in
// it.
struct Address16 {
    std::uint16_t segment = 0;
    std::uint16_t offset = 0;
    friend bool operator==(const Address16&, const Address16&) = default;
};

// What the caller names in a 16-bit module. An absent function answers "none" everywhere.
struct Names16 {
    // The reconstructed function at (segment, offset).
    std::function<std::optional<std::string>(Address16)> function_at;
    // The C symbol of a code segment (the selector of a symbol in that segment is the segment's selector).
    std::function<std::optional<std::string>(std::uint16_t segment)> segment_symbol;
};

// ---- an object of the data file ----------------------------------------------------------------------------------

// A run of bytes of an object, from `offset`.
struct ByteRun {
    std::uint32_t offset = 0;
    Bytes bytes;
};
// A long run of zeros (the object holds them without writing each).
struct ZeroRun {
    std::uint32_t offset = 0;
    std::uint32_t count = 0;
};
// A pointer: `ctype` is its C type, `expression` the C constant it holds, `note` a comment ("" for none).
struct PointerCell {
    std::uint32_t offset = 0;
    std::string ctype, expression, note;
};
using Piece = std::variant<ByteRun, ZeroRun, PointerCell>;

// What a relocated place holds, by the place's offset. `width` is the bytes the pointer replaces.
struct PointerSlot {
    std::uint32_t width = 0;
    std::string ctype, expression, note;
};
// Sorted by offset: the pieces are cut in that order. The offsets of a segment are 16-bit, a section's 32-bit.
using Pointers = std::map<std::uint32_t, PointerSlot>;

// ---- the result --------------------------------------------------------------------------------------------------

// A data section a 32-bit module carries.
struct Section {
    std::uint32_t number = 0; // 1-based
    std::string name;
    std::uint64_t va = 0;
    std::uint32_t size = 0;
};
// One C object written: a section, or a piece of one cut out for an exported name.
struct Part {
    std::string symbol;
    std::uint64_t va = 0;
    std::uint32_t size = 0;
    std::uint32_t section = 0; // the section's number
};
// A pointer left as a number: where it is and why.
struct Unresolved {
    std::string where, why;
};
// An import a data segment names that no function does: by ordinal or by name.
struct Import {
    std::string module;
    std::variant<std::uint16_t, std::string> by;
};

struct Data {
    std::string source, declarations;
    std::vector<Section> sections; // 32-bit
    std::vector<Part> parts;       // the objects written, in address order
    std::size_t pointers = 0;
    std::vector<Unresolved> unresolved;
    std::map<std::string, Import> imports; // 16-bit: by C symbol (sorted: written into scripts)
    bool movable = true;
    std::uint32_t size = 0;
    std::map<std::pair<std::uint16_t, std::uint16_t>, std::string>
        exported;                              // 16-bit: (segment, offset) -> C name of exported data
    std::vector<Piece> pieces;                 // 16-bit: the segment's pieces, as data16 cut them
    std::map<std::uint32_t, std::string> cuts; // 16-bit: where an exported variable starts -> its name

    // The C expression for an address inside a carried section, or nullopt.
    std::optional<std::string> place(std::uint64_t va) const;
};

// ---- writing an object -------------------------------------------------------------------------------------------

// C for one object: a packed struct (each member exactly at its place: the compiler would otherwise put a word on an
// even offset and move everything after it) and its initialised definition. `far` puts it in a far segment; `based`
// names the segment it is based in.
std::string object(std::string_view struct_name, std::string_view name, std::span<const Piece> pieces, bool far = false,
                   std::optional<std::string_view> based = std::nullopt);

// `blob` (padded with zeros to `size`) cut at the pointer places, with every run of 64 zeros or more between them
// made a ZeroRun. A pointer that overlaps the one before is left out.
std::vector<Piece> cut_pieces(ByteView blob, std::uint32_t size, const Pointers& pointers);

// ---- 32-bit ------------------------------------------------------------------------------------------------------

// {virtual address of a relocated word -> the address it holds}, from the relocation table. Sorted by address.
using RelocatedWords = std::map<std::uint64_t, std::uint64_t>;
RelocatedWords relocated_words(const formats::pe::Image& pe);

// {function address -> the addresses its code refers to}. `starts` is every function address of the module; a function
// runs to the next one (the last one 1 MiB on).
std::map<std::uint64_t, std::set<std::uint64_t>> address_constants(const formats::pe::Image& pe,
                                                                   std::span<const std::uint64_t> starts);

// The data sections of a 32-bit module as C. `file_name` is the module's name, for the comment at the top of the file.
// `named` is {address -> C name} for places that must be objects of their own
// (exported data): a section is then written as several objects one after another, which only works if the compiler
// keeps them together -- `layout_holds` checks that on the compiled object.
Data data32(const formats::pe::Image& pe, std::string_view file_name, const FunctionAt32& function_at,
            const std::map<std::uint64_t, std::string>& named = {});

// Whether, in the compiled data object (an OMF .obj), the objects a section was written as lie one after another as
// they do in the original. False too for an object that cannot be read.
bool layout_holds(ByteView omf_object, const Data& d);

// The header's globals (`#define NAME (*(TYPE *)0xADDRESS)`) rebound from the original's addresses to the carried data.
struct Relocated {
    std::string text;
    std::vector<std::uint64_t> lost; // addresses that are in no carried section, in order met
};
// A constant of more than 64 bits is left alone and not listed (no real header has one).
Relocated relocate_header(std::string_view header, const Data& d);

// The address constants of one function (its text after the first `{`). `addresses` is what its code refers to (see
// address_constants); only those constants are rewritten.
Relocated relocate_text(std::string_view text, const std::set<std::uint64_t>& addresses, const Data& d,
                        const FunctionAt32& function_at);

// ---- 16-bit ------------------------------------------------------------------------------------------------------

// One relocated place of a segment: where it is, the kind of its source (0 byte, 2 selector, 3 far pointer, 5 offset),
// the addend it carries, and which of the segment's relocation records put it there.
struct RelocationPlace {
    std::uint16_t offset = 0;
    std::uint8_t source = 0;
    std::uint16_t addend = 0;
    std::size_t record = 0; // an index into the relocations given
};
// Every relocated place of a segment, in the order of its relocation records, following the chains the file keeps (each
// place holds the offset of the next; 0xFFFF ends one). `blob` is the segment's bytes.
std::vector<RelocationPlace> places(std::span<const formats::ne::Relocation> relocations, ByteView blob);

// The automatic data segment of a 16-bit module as C. `far_segments` are the numbers of the other segments the build
// carries as `codebreaker_segment_N`.
Data data16(const formats::ne::Module& module, const Names16& names, const std::set<std::uint16_t>& far_segments = {});

// C for the other segments a 16-bit module's code reads from: {number -> bytes}. Each is a segment of its own, starting
// with it: code (and assembly above all) reaches into it by the original's offsets. With the module (`module`), the
// places the loader fixes up are what they point at -- a string in the automatic segment, a function, a selector -- not
// the bytes the description gave them: SYSDETMG's segment 14, a table of far pointers to its strings and functions,
// kept the made-up selector 1080h, and its hardware detection faulted on it. (The code elsewhere declares the segment
// as bytes; data.c, compiled with CODEBREAKER_DATA, defines it with its pointers -- the linker joins them by name.)
std::string far_segments(const std::map<std::uint16_t, Bytes>& segments, const formats::ne::Module* module = nullptr,
                         const Names16& names = {});

} // namespace cb::build::data
