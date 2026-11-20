// formats/jaguar.hpp -- Atari Jaguar programs: cartridge images (`.j64`, `.jag`, `.rom`) and executables (COFF, `.cof` / `.abs`).
//
// The Jaguar's main processor is a 68000 (the GPU and DSP are RISC cores whose code is data here); its 24-bit address space holds 2 MB of
// DRAM at $000000, the cartridge at $800000 (up to 6 MB), Tom's registers at $F00000 (the GPU's RAM at $F03000) and Jerry's at $F10000 (the DSP's
// RAM at $F1B000).
//
// A cartridge image is the ROM as it sits at $800000. The first $400 bytes hold the boot signature, then the header: a long of flags ($04040404
// for a 64-bit ROM bus) and, at $400 + 4, the address the boot code starts the program at (usually $802000). The program is the 68000's:
// what flow reaches from that address.
//
// A COFF executable (the output of Atari's `aln` or the Removers' `jlinker`, big-endian, magic $0150) has an a.out header holding the entry
// address and sections that load at their own addresses (typically $4000 in DRAM). Sections with no bytes in the file (BSS) are not loaded.
// A `.jag` file that is a COFF is read as one; a Jaguar Server `JAGR` file is not read.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::jaguar {

inline constexpr std::uint32_t kRomBase = 0x800000;
inline constexpr std::uint32_t kRomMax = 0x600000;
inline constexpr std::size_t kHeaderAt = 0x400;     // the cartridge header: flags, then the start address
inline constexpr std::size_t kMemorySize = 0x1000000;

enum class Kind { Cartridge, Coff };

// A stretch of the file that loads at an address.
struct Segment {
    std::uint32_t address = 0;
    std::uint32_t offset = 0;   // in the file
    std::uint32_t size = 0;
    std::string name;           // "ROM", or the COFF section's name
};

struct Program {
    Kind kind = Kind::Cartridge;
    Bytes file;                     // the file as it was
    std::vector<Segment> segments;
    std::uint32_t entry = 0;
    std::uint32_t header_flags = 0; // a cartridge's long at $400

    // The 16 MB the 68000 sees: the segments at their addresses, the rest zero.
    Bytes memory() const;
    // The file offset of `address`, or nullopt where no segment holds it.
    std::optional<std::uint32_t> offset(std::uint32_t address) const;
    // The segment that holds `address`.
    const Segment* segment_at(std::uint32_t address) const;
    // Function starts: the entry, then every call target reached from it.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError when the bytes are neither a cartridge nor a COFF this reads.
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// A cartridge: a `.j64` / `.jag` / `.rom` whose start address lies in the ROM; or a COFF executable of the 68000 (`.cof`, `.abs`, `.jag`).
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::jaguar
