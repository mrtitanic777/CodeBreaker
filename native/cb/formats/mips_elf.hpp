// cb/formats/mips_elf.hpp -- 32-bit little-endian MIPS ELF files: what the PlayStation 2's toolchain (ps2sdk's `ee-gcc`, `.elf`) and the PSP's
// (pspsdk's `psp-gcc`: an executable `.elf`, or a relocatable module `.prx`) write.
//
// The machine is 8 (EM_MIPS); the flags say which MIPS: EF_MIPS_MACH_5900 ($00920000 in the flags) is the Emotion Engine's R5900, EF_MIPS_MACH_ALLEGREX
// ($00A20000) the PSP's. A PSP module has the type $FFA0 (ET_SCE_PRX) and its code starts at address 0: the loader relocates it (its relocation table is
// the segment of type $700000A0 and the sections of type $700000A0 / $700000A1; they are kept as the bytes they are).
//
// A file is read with its headers as they are; `data` is the file. Nothing is moved or rewritten.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::mips_elf {

inline constexpr std::uint16_t kMachineMips = 8;
inline constexpr std::uint16_t kTypeExec = 2;
inline constexpr std::uint16_t kTypePrx = 0xFFA0;
inline constexpr std::uint32_t kMachineMask = 0x00FF0000;
inline constexpr std::uint32_t kMach5900 = 0x00920000;
inline constexpr std::uint32_t kMachAllegrex = 0x00A20000;
inline constexpr std::uint32_t kFlagWrite = 1, kFlagAlloc = 2, kFlagExec = 4;
inline constexpr std::uint32_t kTypeSymtab = 2, kTypeNobits = 8;
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

struct Symbol {
    std::string name;
    std::uint32_t value = 0, size = 0;
    std::uint8_t type = 0;      // 0 none, 1 object, 2 function, 3 section, 4 file
    std::uint8_t bind = 0;      // 0 local, 1 global, 2 weak
    std::uint16_t section = 0;  // 0 undefined
};

struct Elf {
    Bytes data;
    std::uint16_t type = 0, machine = 0;
    std::uint32_t version = 0, entry = 0, phoff = 0, shoff = 0, flags = 0;
    std::uint16_t ehsize = 0, phentsize = 0, phnum = 0, shentsize = 0, shnum = 0, shstrndx = 0;
    std::vector<ProgramHeader> program_headers;
    std::vector<SectionHeader> sections;

    bool prx() const { return type == kTypePrx; }
    // The symbols of the symbol table (empty when the file was stripped).
    std::vector<Symbol> symbols() const;
    // The address the file offset is loaded at (a PT_LOAD segment holds it), or nothing.
    std::optional<std::uint32_t> address_of(std::uint32_t offset) const;
    // The file offset of a loaded address, or nothing.
    std::optional<std::uint32_t> offset_of(std::uint32_t address) const;
    // `size` bytes loaded at `address` from the file (nothing when it does not lie inside the loaded bytes).
    std::optional<ByteView> bytes_at(std::uint32_t address, std::size_t size) const;
    // A NUL-terminated string loaded at `address`.
    std::optional<std::string> string_at(std::uint32_t address) const;
    std::optional<std::uint32_t> word_at(std::uint32_t address) const;
    const SectionHeader* section(const std::string& name) const;
};

// FormatError if the bytes are not a 32-bit little-endian ELF of machine MIPS with its tables inside the file.
Elf parse(ByteView data);
// The file begins as one: ELF magic, 32-bit, little-endian, machine MIPS (the flags are the caller's to read).
bool probe_header(ByteView head);
// The flags' MIPS machine ($00920000 R5900, $00A20000 Allegrex, 0 other).
std::uint32_t machine_of(std::uint32_t flags);

}  // namespace cb::formats::mips_elf
