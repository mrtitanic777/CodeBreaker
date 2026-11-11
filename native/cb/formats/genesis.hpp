// cb/formats/genesis.hpp -- Sega Genesis / Mega Drive cartridge images (`.md`, `.bin`, `.gen`, `.smd`).
//
// The 68000 starts by reading two longs at address 0: the stack pointer and the program's start. Then come the
// exception vectors (bus error, address error, illegal instruction, ...) and the interrupt autovectors -- on the
// Genesis level 4 is the horizontal blank ($70) and level 6 the vertical blank ($78). The header at $100 starts
// "SEGA" (the TMSS hardware of later consoles refuses to start a cartridge without it). There is no bank switching
// below 4 MB: the ROM is simply at $000000-$3FFFFF. An .smd dump (a copier's 512-byte header, then 16 KB blocks
// stored odd bytes first) is turned into that plain image.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::genesis {

inline constexpr std::size_t kHeaderAt = 0x100;
inline constexpr std::array<std::string_view, 4> kExtensions = {".md", ".bin", ".gen", ".smd"};
inline constexpr std::size_t kMinSize = 0x200;          // vectors and header
inline constexpr std::size_t kRomWindow = 0x400000;     // the ROM's address range

// The header at $100, text fields decoded from latin-1 and trimmed.
struct Header {
    std::string system;                   // "SEGA MEGA DRIVE", "SEGA GENESIS" ...
    std::string copyright;
    std::string domestic_name;            // $120
    std::string overseas_name;            // $150
    std::string serial;
    std::uint16_t checksum = 0;           // what the header holds (see Rom::computed_checksum)
    std::string devices;
    std::uint32_t rom_start = 0, rom_end = 0;
    std::uint32_t ram_start = 0, ram_end = 0;
    std::string region;
};

// The name of vector number `n` (the long at 4 * n): "bus_error", "hblank", "vblank", "trap0" ...; empty for a vector
// that is reserved or unused.
std::string vector_name(unsigned n);

// An exception or interrupt vector that points into the ROM.
struct Vector {
    std::string name;          // "bus_error", "hblank", "vblank", "trap0" ...
    std::uint32_t address = 0;
};

struct Rom {
    bool smd = false;          // the file was an .smd dump (data is the de-interleaved image)
    Bytes data;                // the ROM as the 68000 sees it from address 0
    Header header;
    std::uint32_t initial_ssp = 0;
    std::uint32_t reset = 0;
    std::vector<Vector> vectors;   // each exception vector (in vector-number order) that is even, at or above $200 and inside the ROM

    // The ROM placed in a 16 MB address space.
    Bytes memory() const;

    // The checksum the header should hold: the sum of the words from $200, modulo 2^16.
    std::uint16_t computed_checksum() const;

    // Function starts: the reset vector and the handlers the vectors name, then every JSR and BSR target reached by
    // following the code from them. `decode` is the 68000 decoder (a TRAP is a Flow::Branch; its handler is not a
    // function start).
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// `extension` (".smd") is a second hint that the image is an .smd dump. FormatError if the image is too short.
Rom parse(ByteView data, std::string_view extension = {});
Rom read(const std::filesystem::path& path);

// An .smd image turned into the plain one: a 512-byte header, then 16 KB blocks holding each block's odd bytes
// first.
Bytes deinterleave_smd(ByteView data);

// "SEGA" at $100 (or $101 for a dump with a stray byte first), or an .md/.gen/.smd file with a header's worth of
// bytes.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::genesis
