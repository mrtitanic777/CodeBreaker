// cb/formats/vita.hpp -- PlayStation Vita homebrew: the unencrypted SELF (`eboot.bin` of a `.vpk`, what VitaSDK's vita-make-fself writes) and the plain ELF it holds
// (`.velf`).
//
// A Vita ELF is 32-bit little-endian ARM (machine 40) of a Sony type (0xFE00 executable, 0xFE04 relocatable executable); its code is Thumb-2 (VitaSDK's
// compiler default) with ARM state and the VFP / NEON beside it. Its loadable segments are linked at $81000000 (a fixed-address executable) or at 0 (a relocatable
// one, whose relocation table is a segment of type $60000000). `e_entry` is not an address: its top two bits are a segment number and the rest an offset in that
// segment, where the module information is (`SceModuleInfo`: the module's name, the tables of exports and imports, and the start and stop functions).
//
// A SELF (magic "SCE\0", version 3) holds the ELF header and program headers (at the offsets it gives) and, in a table of `segment_info` entries, each segment's
// place in the file, its length, whether it is compressed (zlib) and whether it is encrypted. A fake-signed homebrew SELF is compressed and not encrypted; one whose
// segments are encrypted is refused (FormatError: CodeBreaker does not decrypt). The program is read as its loadable segments inflated and laid end to end (`image`),
// each segment 16-byte aligned; `to_elf()` writes that image back as a plain ELF (the form a rebuilt program is compared in).
//
// Imports are a table per library of function NIDs and the stubs the loader fills in; the function NIDs are named from VitaSDK's published database
// (vitasdk/vita-headers, MIT: formats/vita_nids.cpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::vita {

inline constexpr std::uint16_t kMachineArm = 40;

struct Segment {
    std::uint32_t image_offset = 0;   // in `Program::image`
    std::uint32_t size = 0;           // bytes held (the file size)
    std::uint32_t address = 0;        // the p_vaddr
    std::uint32_t memory_size = 0;
    std::uint32_t flags = 0;          // p_flags (1 execute, 2 write, 4 read)
    bool code() const { return (flags & 1) != 0; }
};

struct Import {
    std::string library;              // "SceCtrl"
    std::uint32_t library_nid = 0;
    std::uint32_t nid = 0;
    std::string name;                 // from the database, or empty
    std::uint32_t stub = 0;           // the stub's address (Thumb bit clear)
};

struct Program {
    bool self = false;                // the file was a SELF
    Bytes image;                      // the loadable segments inflated, end to end
    std::vector<Segment> segments;
    std::uint16_t elf_type = 0xFE04;
    std::uint32_t elf_entry = 0;      // as the header says (segment number in the top two bits)
    std::uint32_t elf_flags = 0;
    std::string module_name;
    std::vector<Import> imports;
    std::vector<std::uint32_t> entries;   // addresses of the module's start and stop functions (Thumb bit as stored)

    std::optional<std::uint32_t> offset_of(std::uint32_t address) const;   // image offset of a loaded address
    std::optional<std::uint32_t> address_of(std::uint32_t offset) const;
    bool in_code(std::uint32_t offset) const;
    // A plain ELF holding `image` (a rebuilt one) in the layout this program has.
    Bytes to_elf(ByteView image) const;
};

// FormatError when the bytes are neither (an encrypted SELF says so).
Program parse(ByteView data);
Program read(const std::filesystem::path& path);
// Is it a SELF version 3 or an ARM ELF of a Sony type? Reads the first kilobyte.
bool probe(const std::filesystem::path& path);

// The names VitaSDK's database gives a function and a library by NID (empty when it has none).
std::string function_name(std::uint32_t nid);
std::string library_name(std::uint32_t nid);

}  // namespace cb::formats::vita
