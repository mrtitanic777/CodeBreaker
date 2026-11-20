// formats/nro.hpp -- Nintendo Switch programs: homebrew `.nro` files, `.nso` executables, and the AArch64 ELF devkitA64 links them from.
//
// A NRO is the homebrew loader's format and is position independent. The file is its own image: a header at 0 (a `b` to the entry, the offset of the MOD0 block, the
// magic "HOMEBREW" and "NRO0", the file's size, then offset and size of the text, read-only data and data segments, the bss size, the build id, and the offsets of the
// dynamic string and symbol tables), the three segments at their offsets (each a multiple of the 4 KB page), and after the last byte the file's size says an optional asset
// section ("ASET": the icon, the NACP control data and a RomFS, kept as data). Addresses are offsets from the load base (0 here). The MOD0 block, found by the header, gives the
// `.dynamic` section, and its DT_RELA / DT_JMPREL tables the relocations the loader applies: R_AARCH64_RELATIVE (`*(u64*)(base + offset) = base + addend`), GLOB_DAT, JUMP_SLOT.
//
// A NSO is the same kind of image as the system's modules keep it: a 0x100-byte header (magic "NSO0", flags, then for each of text, read-only data and data its place in the
// file, its offset in memory and its size, the bss size, the build id and the compressed sizes), the segments LZ4-block compressed when the flags say so. `parse` inflates them:
// the program here is the decompressed one, and the container is only kept by `file`. Encrypted containers (NSP, XCI, NCA) are recognised by `encrypted_container` and not read.
//
// An ELF is read for its PT_LOAD segments (laid out by virtual address), entry point, symbols and, when it has a PT_DYNAMIC, the same relocations.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

#include "core/bytes.hpp"

namespace cb::formats::nro {

enum class Kind { Nro, Nso, Elf };

inline constexpr std::uint32_t kPage = 0x1000;
inline constexpr std::uint16_t kMachineAarch64 = 183;
inline constexpr std::uint32_t kRelativeReloc = 1027;   // R_AARCH64_RELATIVE
inline constexpr std::uint32_t kGlobDat = 1025, kJumpSlot = 1026, kAbs64 = 257;

struct Segment {
    std::uint64_t address = 0;       // in the image
    std::uint64_t size = 0;          // in the file (and the image)
    std::uint64_t memory_size = 0;   // with the bss
    std::uint64_t file_offset = 0;   // in `file`
    bool code = false, writable = false;
    std::string name;                // "text", "rodata", "data"
};

struct Relocation {
    std::uint64_t offset = 0;        // where the loader writes
    std::uint32_t type = 0;
    std::uint32_t symbol = 0;
    std::int64_t addend = 0;
};

struct Symbol {
    std::string name;
    std::uint64_t value = 0, size = 0;
    unsigned info = 0;               // st_info
    unsigned section = 0;            // st_shndx (0: undefined)
};

struct Program {
    Kind kind = Kind::Nro;
    Bytes file;                      // as read
    Bytes flat;                      // what the writer rebuilds: a NRO's file; a NSO's or ELF's loaded image (address a is flat[a])
    bool flat_is_file = true;
    std::vector<Segment> segments;
    std::uint64_t entry = 0;
    std::uint64_t image_size = 0;    // the segments and the bss
    std::vector<Relocation> relocations;
    std::vector<std::pair<std::uint64_t, std::uint64_t>> relocation_tables;   // (address, size) of each DT_RELA / DT_JMPREL table
    std::map<std::uint64_t, std::string> names;   // address -> symbol (an ELF's, or the dynamic symbols a NRO defines)
    std::vector<Symbol> imports;     // undefined dynamic symbols
    std::uint64_t mod_offset = 0;    // the MOD0 block, when there is one
    std::uint64_t dynamic = 0;       // the .dynamic section
    std::uint64_t asset_offset = 0;  // a NRO: where the ASET block starts (the file's size when there is none), and its parts
    struct AssetPart {
        std::uint64_t offset = 0, size = 0;
    };
    AssetPart icon, nacp, romfs;
    std::string build_id;            // hex
    std::string note;                // what was not read ("compressed segments inflated", ...)

    bool in_code(std::uint64_t address) const;
    bool in_image(std::uint64_t address) const { return address < flat.size(); }
};

// FormatError when the bytes are none of the three (an encrypted container says so).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);
// Is it a NRO, a NSO, or an AArch64 ELF? Reads the first kilobyte.
bool probe(const std::filesystem::path& path);
// The kind of container an encrypted one is ("NSP (PFS0)", "XCI", "NCA") when the bytes are one, for `info`.
std::optional<std::string> encrypted_container(ByteView head);

// LZ4 block decompression (the format NSO segments use). FormatError on a malformed block or one that does not inflate to `expected` bytes.
Bytes lz4_decompress(ByteView block, std::size_t expected);
// A valid (literal-only or run-copying) LZ4 block of `data`, for tests that need a compressed segment.
Bytes lz4_compress_simple(ByteView data);

}  // namespace cb::formats::nro
