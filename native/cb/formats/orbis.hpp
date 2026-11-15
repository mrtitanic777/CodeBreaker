// cb/formats/orbis.hpp -- PlayStation 4 and PlayStation 5 homebrew: the x86-64 ELF a homebrew toolchain links, and the unencrypted ("fake") SELF that wraps one.
//
// Both consoles run an x86-64 FreeBSD-derived system (Orbis on the PS4's AMD Jaguar, Prospero on the PS5's Zen 2). What homebrew is made of:
//
//   * a plain ELF64 (type EXEC or DYN, machine 62) -- what the PS5 payload SDK links (and the PS4 payload SDK, and OpenOrbis before its own post-link step). Its
//     imports are ordinary undefined dynamic symbols named by their C names ("sceKernelUsleep", "printf"), called through a GOT slot a relocation of type
//     GLOB_DAT or JUMP_SLOT fills in;
//   * the Orbis ELF (OELF, type 0xFE10 / 0xFE18) OpenOrbis's `create-fself` makes of that and wraps in a fake SELF as `eboot.bin`. The system's dynamic linker reads
//     its dynamic data from a segment of type 0x61000000 (PT_SCE_DYNLIBDATA) whose tags (0x6100xxxx) are the usual ones moved: the symbol, string and relocation
//     tables are offsets inside that segment, imports are symbols named `<NID>#<library>#<module>`. A NID is the first eight bytes of SHA-1(name + a fixed 16-byte
//     salt), reversed, as base 64 without padding (`+` and `-`); CodeBreaker names the ones its list of well-known functions (formats/orbis_nids.cpp) hashes to;
//   * the fake SELF (magic 1D 3D 15 4F, key type 0x101, program type "fake"): a 32-byte header, a table of 32-byte segment entries, a copy of the ELF header and
//     program headers, an extended-info block (program authority, type, versions and a SHA-256 of the ELF), the NPDRM control block, the meta blocks and a zero
//     signature, then each segment's bytes as they are. The loadable segments are the plain bytes (the blocks are hashed, not compressed or encrypted), so the
//     ELF is read back by putting each segment at its program header's file offset. A SELF whose program type is not "fake", or whose entries are compressed,
//     is a retail or encrypted one: refused (FormatError; CodeBreaker does not decrypt).
//
// A PS5 SELF (magic 54 14 F5 EE) and a PS4 package (".pkg", magic 7F 43 4E 54) are recognised and refused the same way. Which console a program is for: a plain
// payload ELF does not say (the PS5 payload SDK links the same instruction set, the same libraries and the same ELF for both, and the PS5 runs PS4 programs), so it is
// told by its file name ("-ps5.elf", "prospero") and taken for a PS4 one otherwise; an OELF or a SELF is OpenOrbis's, so the PS4's.
//
// `file` is always the plain ELF (for a SELF, the one rebuilt from it): the form a rebuilt program is compared in.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::orbis {

enum class Console { Ps4, Ps5 };

inline constexpr std::uint16_t kMachineX86_64 = 62;
inline constexpr std::uint32_t kPtLoad = 1, kPtDynamic = 2, kPtSceDynlibData = 0x61000000, kPtSceProcParam = 0x61000001, kPtSceRelro = 0x61000010;

struct Segment {
    std::uint32_t type = 0, flags = 0;
    std::uint64_t offset = 0, address = 0, file_size = 0, memory_size = 0, align = 0;
    bool loaded() const { return type == kPtLoad || type == kPtSceRelro; }   // (the RELRO segment is mapped like a LOAD one)
    bool code() const { return type == kPtLoad && (flags & 1) != 0; }
};

struct Symbol {
    std::string name;                 // the C name, or `<NID>#<library>#<module>` in an Orbis ELF
    std::uint64_t value = 0;
    std::uint64_t size = 0;
    std::uint8_t info = 0;
    std::uint16_t section = 0;
    bool function() const { return (info & 15) == 2; }
    bool defined() const { return section != 0; }
};

struct Relocation {
    std::uint64_t offset = 0;         // the place (a virtual address)
    std::uint32_t type = 0;           // R_X86_64_*: 1 64, 6 GLOB_DAT, 7 JUMP_SLOT, 8 RELATIVE
    std::uint32_t symbol = 0;
    std::int64_t addend = 0;
};

// A function the program calls that is not in it: the GOT slot the system's linker fills with its address.
struct Import {
    std::string name;                 // the function's name (from the symbol, or from the NID when the list has it); empty when unknown
    std::string nid;                  // an Orbis ELF's NID
    std::string library;              // "libkernel", or a PS5 payload's needed library when the symbol says none
    std::uint64_t slot = 0;
};

struct Program {
    Console console = Console::Ps4;
    bool self = false;                // read from a SELF
    bool orbis = false;               // an Orbis ELF (SCE dynamic data), not a plain one
    std::string container;            // "ELF", "OELF", "fake SELF"
    Bytes file;                       // the plain ELF
    std::uint16_t type = 3;
    std::uint64_t entry = 0;
    std::vector<Segment> segments;    // every program header
    std::vector<std::string> needed;
    std::vector<Symbol> symbols;      // the dynamic symbols (index = the relocations' symbol number)
    std::vector<Symbol> functions;    // defined functions by address: the dynamic table's and the static one's
    std::vector<Relocation> relocations;
    std::vector<Import> imports;      // by slot
    std::vector<std::uint64_t> initializers;   // the DT_INIT_ARRAY entries and DT_INIT
    std::uint32_t program_type = 0;   // a SELF's extended info: 1 fake
    std::uint64_t paid = 0;

    // The file offset of a virtual address inside a loaded segment's file bytes, and the reverse.
    std::optional<std::uint64_t> offset_of(std::uint64_t address) const;
    std::optional<std::uint64_t> address_of(std::uint64_t offset) const;
    bool in_code(std::uint64_t address) const;
    // The name of the import whose slot is `address`, or empty.
    std::string import_at(std::uint64_t address) const;
    // The highest address a loaded segment reaches (the image's size in memory).
    std::uint64_t image_end() const;
};

// FormatError when the bytes are neither (an encrypted SELF, a PS5 SELF, a package: each says what it is).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// What a file is, without parsing the whole of it: the first kilobytes.
enum class Kind { None, Elf, Fself, RetailSelf, Ps5Self, Pkg };
Kind kind_of(ByteView head);
// An x86-64 ELF a PS4 or PS5 program can be (see the console rule above); `console` says which. Reads the first 8 KB.
bool probe(const std::filesystem::path& path, Console console);
// Is a parsed x86-64 ELF a PlayStation program: an Orbis ELF, or one that needs the system's libraries (libkernel, libSce*, *.sprx)? A Linux program is not.
bool is_playstation(const Program& program);
// The console the file at `file` holding `program` is for (the rule above).
Console console_of(const std::filesystem::path& file, const Program& program);

// The name of a function whose NID is `nid` (from the list of well-known functions), and a NID of a name.
std::string function_name(const std::string& nid);
std::string nid_of(const std::string& name);
// A FreeBSD / Orbis system call by number ("write"), empty when it has no name here.
std::string syscall_name(std::uint32_t number);

}  // namespace cb::formats::orbis
