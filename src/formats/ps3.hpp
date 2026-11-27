// formats/ps3.hpp -- PlayStation 3 homebrew: the plain ELF PSL1GHT links (`.elf`) and the unencrypted SELF that wraps one.
//
// The PPU is a 64-bit big-endian PowerPC (machine 21, ELF64): the ELF's header says so, the program headers describe the loaded segments (the usual layout is text at
// $10000, data at $10000000), and `e_entry` is the address of a function descriptor (the PowerPC64 ELF ABI v1's `.opd` entry: the code's address, then the table of
// contents (TOC) pointer r2 is set to). Imports of the system's library modules (PRX) are named in the program header of type $60000002: the stubs of each library
// (its name, its function NIDs and the descriptors a call goes through). A NID is the first four bytes of the SHA-1 of the function's name followed by a 16-byte
// suffix, so the names PSL1GHT publishes (ppu/sprx/*/exports.h, MIT) name them (config/nids/ps3.nids); the same file names the lv2 kernel's system calls by number.
//
// A SELF (magic "SCE\0", version 2) holds the ELF header, the program headers and, in a table of 32-byte section-info entries (one per program header), each
// segment's place in the file, its length, whether it is zlib-compressed and whether it is encrypted. Segments that are encrypted are refused (FormatError:
// CodeBreaker does not decrypt); a fake-signed homebrew SELF whose segments are plain or only compressed is read as its segments inflated, laid end to end in `image`
// (each 16-byte aligned); `to_elf()` writes that image back as a plain ELF, the form a rebuilt program is compared in. SPU code inside a program stays data.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::ps3 {

inline constexpr std::uint16_t kMachinePpc64 = 21;

struct Segment {
    std::uint32_t image_offset = 0;
    std::uint32_t size = 0;           // bytes held
    std::uint64_t address = 0;        // the p_vaddr
    std::uint64_t memory_size = 0;
    std::uint32_t flags = 0;          // p_flags (1 execute, 2 write, 4 read)
    bool code() const { return (flags & 1) != 0; }
};

struct Import {
    std::string library;              // "sys_io"
    std::uint32_t nid = 0;
    std::string name;                 // from the published lists, or empty
    std::uint64_t descriptor = 0;     // the address the stub's function descriptor is at
};

struct Program {
    bool self = false;
    Bytes image;                      // the loadable segments, end to end
    std::vector<Segment> segments;
    std::uint16_t elf_type = 2;
    std::uint8_t os_abi = 0x66, abi_version = 0;
    std::uint64_t entry = 0;          // the address of the entry function's descriptor
    std::uint64_t entry_code = 0;     // ... the code it names
    std::uint64_t toc = 0;            // ... and the TOC it sets r2 to
    std::uint32_t elf_flags = 0;
    std::vector<Import> imports;
    std::uint64_t prx_param = 0, prx_param_size = 0;   // the program header of type $60000002 (kept so a rebuilt ELF names the same imports)

    std::optional<std::uint32_t> offset_of(std::uint64_t address) const;
    std::optional<std::uint64_t> address_of(std::uint32_t offset) const;
    bool in_code(std::uint32_t offset) const;
    // The big-endian word, doubleword at an address, if it is in the image.
    std::optional<std::uint64_t> read(std::uint64_t address, unsigned size) const;
    // A plain ELF holding `image` (a rebuilt one) in the layout this program has.
    Bytes to_elf(ByteView image) const;
};

// FormatError when the bytes are neither (an encrypted SELF says so).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);
// Is it an ELF64 big-endian PowerPC executable, or a SELF version 2? Reads the first kilobyte.
bool probe(const std::filesystem::path& path);

// The name PSL1GHT's lists give a function by NID and its library, and the lv2 system call of a number (empty where there is none).
std::string function_name(std::uint32_t nid);
std::string function_library(std::uint32_t nid);
std::string syscall_name(std::uint32_t number);

}  // namespace cb::formats::ps3
