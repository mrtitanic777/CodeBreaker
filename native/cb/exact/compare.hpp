// cb/exact/compare.hpp -- a compiled function against the program's bytes at its place.
//
// Every byte must be the program's, except the words a linker fills in; those must come to the program's word when
// the place of what they name is put in. Names whose place is known are in the project's SYMBOLS; a name met for the
// first time is learnt from the program (the place its word implies), and the next function that names it is held to
// that. A word naming the file's own data must point, in the program's data segment, at what the file's data has
// there.
//
// The program's code may be in several segments (the project's code_segments: their image offsets); an offset within
// one wraps at 64K. Its segment words are those of a load at the project's loadseg.
#pragma once

#include <cstdint>
#include <functional>
#include <string>
#include <unordered_set>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/object.hpp"
#include "cb/exact/symbols.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::compare {

// What compare reads of the project (exact/project, refactored later; it fills this in).
struct Program {
    std::int64_t dgroup = 0;      // the image offset of the data segment (negative: the data lies below the image, as a Pascal program's may)
    std::uint32_t loadseg = 0;    // the segment the program is loaded at (its segment words are relative to it)
    std::function<std::uint32_t(std::uint32_t)> code_base;   // the image offset of the code segment an address is in
    // The places in the load image the loader adds the load segment to (the program's relocation table); null or
    // empty when the image has none (an EXEPACK-packed one), in which case a word naming a segment is taken on trust.
    const std::unordered_set<std::uint32_t>* relocs = nullptr;
};

// The function under test: object bytes [offset, offset+length) of `segment`, held to the image at `at`.
struct Subject {
    const ObjectFile& object;
    std::string name;
    std::uint32_t offset = 0;
    std::uint32_t length = 0;
    std::uint32_t at = 0;
    std::string segment = "_TEXT";   // the object's code segment the function is in
};

// Where the image address of the segment's offset x is, for a file whose functions each lie at a place of their
// own; negative means "not known". Without it the file's code lies together from `Subject::at`.
using Where = std::function<std::int64_t(std::uint32_t)>;

enum class Severity { Error, Warning };

struct Note {
    Severity severity = Severity::Error;
    std::string text;   // starts "+<offset in the function>: " when it is about one word
};

struct Report {
    std::vector<std::uint32_t> bad;   // offsets (in the function) of bytes that differ
    std::vector<Note> notes;          // what is wrong with the linker's words
    // The function is the program's: no byte differs and no note is more than a warning.
    bool identical() const;
};

// Compare. `symbols` is the project's SYMBOLS (read, and extended with the names this function meets for the first
// time); `learnt` collects just those new names, in the order they were learnt. Throws TruncatedError when a word the
// comparison needs lies beyond the image, FormatError when the object lacks the segment.
Report function(const Subject& subject, ByteView image, const Program& program, SymbolTable& symbols,
                SymbolTable& learnt, const Where& where = nullptr,
                const x86::Decoder16& decoder = x86::capstone_decoder());

}  // namespace cb::exact::compare
