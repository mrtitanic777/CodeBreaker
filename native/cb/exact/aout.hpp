// cb/exact/aout.hpp -- a Version 7 a.out object file for the 8086 (XENIX's C compiler), read into the same record
// an OMF module gives (cb/exact/object.hpp).
//
// The file: 16 bytes of header (0407h/0107h magic, text, data, bss and symbol sizes, the entry, a word, and whether
// there is relocation), the text, the data, then - unless the last header word says none - one relocation word for
// every word of the text and of the data (0: none), then the symbols (8 characters, a type word, a value word).
//
// A relocation word: bit 0 set, the word is relative to the place after it (a call or jump); bits 1-3 what it names
// (1 the text, 2 the data, 3 the bss, 4 an external symbol, bits 4-14 its number); bit 15 set, the operand begins
// one byte on (the 8086 has operands at odd places too).
//
// Fix-up kinds: Off is a word that holds a place, Rel a place relative to the word's end; targets are a segment
// (_TEXT, _DATA, _BSS) or an external. A word that names the data or the bss, and a symbol's value, count from that
// segment's own start (the linker adds the piece's place; LINK 2.4x relinked from these objects shows it). A relative
// word's own value counts from the start of the text, so its displacement is set to make it count as an OMF
// self-relative fix-up's would.
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/object.hpp"

namespace cb::exact::aout {

// One entry of the symbol table as the file has it.
struct Symbol {
    std::string name;
    std::uint16_t type = 0;
    std::uint16_t value = 0;
};

struct Image {
    ObjectFile object;               // kind Aout; segments _TEXT, _DATA, _BSS
    std::vector<Symbol> symbols;     // the file's symbol table, in order (fix-ups refer to it by index)
};

// Does the file begin with an a.out magic (0407, 0410, 0411 octal)?
bool is_aout(ByteView data);

// Read an a.out object. Throws TruncatedError when a section or table the header announces is not in the file, and
// FormatError when a relocation word names a symbol the table does not have.
Image read(ByteView data);

}  // namespace cb::exact::aout
