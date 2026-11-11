// cb/symbols/libraries/coff.hpp -- the functions of a COFF object or `!<arch>` archive: each one's name, bytes and mask.
//
//     for (const coff::Member& m : coff::members(data)) { ... m.label, m.body ... }   // the objects of an archive
//     const coff::Object obj = coff::Object::parse(body);                             // one object's tables
//     const std::vector<Function> fs = coff::functions(body);                         // its functions (x86 or PowerPC)
//     const auto imp = coff::import_by_ordinal(body);                                 // an import library's member
//
// The 32-bit tools' libraries -- Visual C++'s C runtime and MFC, the Xbox 360 XDK -- are archives of COFF objects. What
// the linker fills in is listed by each code section's relocation records, so nothing is guessed: on x86 the bytes of
// the field (two or four), on PowerPC the bits of the instruction word the relocation's type fills (the 24-bit
// distance of a `bl`, a 16-bit half of an address), the opcode and registers still compared.
//
// The one archive and object reader of CodeBreaker: the library matcher (libraries/package), the runtime library's
// digests, the XDK signatures, the import-library readers of symbols/ordinals and the authored-source matcher all read
// through it. The structures are little-endian whatever the machine (the Xbox 360's code is big-endian, its objects'
// headers are not).
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/symbols/libraries/package.hpp"

namespace cb::symbols::libraries::coff {

inline constexpr std::uint16_t kI386 = 0x014C;
inline constexpr std::uint16_t kPowerPc = 0x01F2;

// IMAGE_REL_PPC_* (the low byte of a PowerPC relocation's type)
inline constexpr std::uint16_t kRelAddr32 = 0x2, kRelAddr24 = 0x3, kRelAddr16 = 0x4, kRelAddr14 = 0x5;
inline constexpr std::uint16_t kRelRel24 = 0x6, kRelRel14 = 0x7, kRelAddr32Nb = 0xA;
inline constexpr std::uint16_t kRelSecRel = 0xB, kRelRefHi = 0x10, kRelRefLo = 0x11, kRelPair = 0x12;
inline constexpr std::uint16_t kRelSecRelLo = 0x13, kRelSecRelHi = 0x14, kRelGpRel = 0x15;

// IMAGE_SCN_CNT_CODE
inline constexpr std::uint32_t kSectionCode = 0x20;

// One object of an archive: its label (a long name resolved through the archive's name table, the trailing '/' taken
// off) and its bytes, a view into the archive's data.
struct Member {
    std::string label;
    ByteView body;
};

// The objects of a COFF archive (`!<arch>`); the linker's own members (the symbol tables, the long-name table) left
// out. A member the end of the file cuts short is taken as far as it goes; a header that gives no size ends the
// archive (what is before it is kept). Data that is not an archive has no members.
std::vector<Member> members(ByteView data);

// A section table entry.
struct Section {
    std::string name;              // as the 8-byte field holds it ("/n" long names are not resolved)
    std::uint32_t size = 0;
    std::uint32_t ptr = 0;         // the raw data's file offset
    std::uint32_t relptr = 0;      // the relocations' file offset
    std::uint16_t nrel = 0;
    std::uint32_t chars = 0;
};

// A symbol table entry.
struct Symbol {
    std::string name;
    std::uint32_t value = 0;
    std::int16_t sec = 0;          // the section number: 1-based; 0 undefined, -1 absolute, -2 debug
    std::uint16_t type = 0;
    std::uint8_t cls = 0;          // storage class: 2 external, 3 static, 6 label
};

struct Relocation {
    std::uint32_t offset = 0;      // in the section
    std::uint32_t symbol = 0;      // index into the symbol table (auxiliary records counted)
    std::uint16_t type = 0;
};

// A COFF object: its headers, section table, symbols and relocations, read once. The data must outlive the object.
class Object {
public:
    // TruncatedError when the headers or section table run past the data. A symbol table that is cut short ends
    // where the data does.
    static Object parse(ByteView data);

    std::uint16_t machine() const { return machine_; }
    std::uint32_t symbol_table_offset() const { return symptr_; }
    const std::vector<Section>& sections() const { return sections_; }
    // The symbol table as the file lays it out: an auxiliary record is a slot of its own, nullopt, so that a
    // relocation's symbol index addresses it directly.
    const std::vector<std::optional<Symbol>>& symbols() const { return symbols_; }
    ByteView data() const { return data_; }

    // The relocations of a section (TruncatedError when the table runs past the data).
    std::vector<Relocation> relocations(const Section& section) const;
    // The raw data of a section (TruncatedError when it runs past the data; a section without any is empty).
    ByteView raw_data(const Section& section) const;

private:
    ByteView data_;
    std::uint16_t machine_ = 0;
    std::uint32_t symptr_ = 0;
    std::vector<Section> sections_;
    std::vector<std::optional<Symbol>> symbols_;
};

// The functions of one object: each external function of its code sections (x86), or each function a symbol starts
// (PowerPC); none for an object of another machine. TruncatedError / FormatError for a damaged one.
std::vector<Function> functions(ByteView data);

// The bits of an instruction word a PowerPC relocation of this type fills; nullopt: the whole word.
std::optional<std::uint32_t> field_bits(std::uint16_t relocation_type);

// A member of an import library: the function it makes importable by number.
struct ImportByOrdinal {
    std::string dll;            // as the member spells it: the short form holds the file name ("KERNEL32.dll"), the long form
                                // the name of its import descriptor symbol ("KERNEL32")
    std::string symbol;         // the linker's name for the function
    std::uint32_t ordinal = 0;
};
// Which function a member of a 32-bit import library imports by number, in either of the forms the tools wrote: the
// short import header, or a little object file. nullopt for a member that imports by name, is not an import at all or
// is damaged.
std::optional<ImportByOrdinal> import_by_ordinal(ByteView member);

}  // namespace cb::symbols::libraries::coff
