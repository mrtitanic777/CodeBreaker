// cb/formats/ppc_elf.hpp -- 32-bit big-endian PowerPC ELF programs of the Nintendo consoles: GameCube and Wii homebrew (`.elf`, what
// devkitPPC's linker writes before a DOL is made from it) and Wii U executables and libraries (`.rpx`, `.rpl`, and the plugins and
// modules the homebrew loaders take, which are RPLs).
//
// A Wii U RPX is an ELF with the OS/ABI byte 0xCA (CafeOS), type 0xFE01, whose section headers come first (at $40) and whose big sections
// are compressed: a section with the flag 0x08000000 holds a 4-byte big-endian size and a zlib stream. `parse` inflates them, laying the
// file out again (every offset after a section moves by what the section grew; the flag is cleared and the size says what is there),
// so everything after reads the same for every ELF: `data` is that file, the decompressed RPX the other tools (wiiurpxtool -d) write. The
// CRC section, the file information section and the relocations are kept as they were; the program is not relocated.
//
// A plain ELF is kept byte for byte.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/dol.hpp"
#include "cb/formats/ppc_segment.hpp"

namespace cb::formats::ppc_elf {

inline constexpr std::uint16_t kMachinePpc = 20;
inline constexpr std::uint16_t kTypeExec = 2;
inline constexpr std::uint16_t kTypeRpl = 0xFE01;       // the Wii U's executables and libraries
inline constexpr std::uint8_t kOsAbiCafe = 0xCA;
inline constexpr std::uint32_t kFlagAlloc = 2, kFlagExec = 4;
inline constexpr std::uint32_t kFlagDeflated = 0x08000000;   // an RPL section stored as size + zlib stream
inline constexpr std::uint32_t kTypeNobits = 8;
inline constexpr std::size_t kHeaderSize = 52;
inline constexpr std::size_t kSectionHeaderSize = 40;
inline constexpr std::size_t kProgramHeaderSize = 32;

struct SectionHeader {
    std::uint32_t name_offset = 0, type = 0, flags = 0, address = 0, offset = 0, size = 0, link = 0, info = 0, align = 0, entsize = 0;
    std::string name;
};

struct ProgramHeader {
    std::uint32_t type = 0, offset = 0, vaddr = 0, paddr = 0, filesz = 0, memsz = 0, flags = 0, align = 0;
};

struct Elf {
    Bytes data;                                        // the file (an RPX with its sections inflated)
    bool wiiu = false;                                 // OS/ABI 0xCA or type 0xFE01
    std::uint8_t os_abi = 0, abi_version = 0;
    std::uint16_t type = 0, machine = 0;
    std::uint32_t version = 0, entry = 0, phoff = 0, shoff = 0, flags = 0;
    std::uint16_t ehsize = 0, phentsize = 0, phnum = 0, shentsize = 0, shnum = 0, shstrndx = 0;
    std::vector<ProgramHeader> program_headers;
    std::vector<SectionHeader> sections;
    std::size_t inflated_sections = 0;                 // how many sections were stored compressed in the file

    // What is loaded: the allocated sections that hold bytes (code: the executable ones); a program with no section headers, its loadable
    // segments.
    std::vector<ppc_segment::Segment> segments() const;
    // For a GameCube or Wii ELF: which of the two, by what the code touches (see formats/dol). A Wii U program has `wiiu` set.
    dol::Console console() const;
};

// FormatError if the file is not a 32-bit big-endian PowerPC ELF of type EXEC or the Wii U's RPL, has headers outside the file, or holds a
// compressed section that does not inflate.
Elf parse(ByteView data);
Elf read(const std::filesystem::path& path);

// The ELF identification and header of an executable (or RPX / RPL) for PowerPC; reads 52 bytes.
bool probe(const std::filesystem::path& path);
// The same on bytes already read.
bool is_ppc_elf(ByteView head);

}  // namespace cb::formats::ppc_elf
