// formats/sega32x.hpp -- Sega 32X cartridge images (`.32x`): a Genesis ROM with the SH-2 programs of the add-on in it.
//
// The ROM is a Mega Drive cartridge (formats/genesis: the 68000 starts from the vectors at $000, the header at $100 reads "SEGA 32X", or
// "SEGA SSF" for a ROM that uses the SSF mapper) with the 32X's header after it. At $3C0:
//
//     $3C0  the module name, 16 characters
//     $3D0  (zero in the ROMs seen)                $3D4  the ROM offset of the SH-2 program
//     $3D8  (zero in the ROMs seen)                $3DC  its length in bytes
//     $3E0  the master SH-2 start address          $3E4  the slave SH-2 start address
//     $3E8  the master's vector base (VBR)         $3EC  the slave's vector base
//     $3F0  the 68000 initial program
//
// The 32X boot code copies the SH-2 program to the start of SDRAM at $06000000 (256 KB; the 68000 side sees the cartridge through
// the 32X windows and the SH-2s see it at $02000000), and each SH-2 starts at its address with VBR where the header says: the program
// begins with the vector tables (a vector is a longword: power-on PC and SP, manual reset PC and SP, illegal instruction ... the traps and
// the interrupt levels the 32X raises: V-blank, H-blank, the command interrupt, PWM), then code and data. The two SH-2s run the one program;
// the master's start is usually the C runtime's and the slave's waits on a flag the master sets.
//
// The 68000 side is a Genesis program (formats/genesis reads it, source/console_writer writes it); this reads what is the SH-2s'.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::sega32x {

inline constexpr std::size_t kHeaderAt = 0x3C0;
inline constexpr std::size_t kMinSize = 0x400;
inline constexpr std::uint32_t kSdram = 0x06000000;        // where the SH-2 program is copied to
inline constexpr std::size_t kSdramSize = 0x40000;
inline constexpr std::uint32_t kCartWindow = 0x02000000;   // the cartridge as the SH-2s see it
inline constexpr std::size_t kCartWindowSize = 0x400000;
inline constexpr std::uint32_t kFrameBuffer = 0x04000000;  // the 32X frame buffer (DRAM): 128 KB, and the overwrite image at +$20000
inline constexpr std::size_t kFrameBufferSize = 0x40000;
inline constexpr std::array<std::string_view, 1> kExtensions = {".32x"};

struct Header {
    std::string system;                   // $100: "SEGA 32X", "SEGA SSF" ...
    std::string domestic_name;            // $120
    std::string overseas_name;            // $150
    std::string serial;                   // $180
    std::string region;                   // $1F0
    std::string module_name;              // $3C0
    std::uint32_t reserved_3d0 = 0, sh2_offset = 0, reserved_3d8 = 0, sh2_size = 0;
    std::uint32_t master_start = 0, slave_start = 0, master_vbr = 0, slave_vbr = 0;
};

struct Rom {
    Bytes data;                           // the cartridge as the 68000 sees it from address 0
    Header header;

    // The SH-2 program: the ROM bytes at [sh2_offset, sh2_offset + sh2_size), cut at the end of the ROM and of SDRAM.
    ByteView sh2_program() const;
    // Does the SH-2 program lie in the ROM and in SDRAM?
    bool has_sh2_program() const;
    // The address the program's byte at `offset` has, and does an address lie in the program (in any cache view)?
    static std::uint32_t address_of(std::uint32_t offset) { return kSdram + offset; }
    bool in_program(std::uint32_t address) const;

    // SDRAM (256 KB) with the program at its start.
    Bytes sdram() const;

    // The addresses the two vector tables point at that lie in the program and hold an instruction (the stack pointers and the zeros are
    // left out), ascending and without repeats.
    std::vector<std::uint32_t> vector_targets(const Decoder& decode) const;

    // Function starts: the two start addresses, the vectors' targets, and every BSR target reached by following the code from them
    // (`decode` is the SH-2 decoder), ascending.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the image is shorter than the header.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// Is this the start of a 32X cartridge (`head` holds its first $400 bytes)? "SEGA" at $100, and start addresses in SDRAM or the cartridge
// window, with "32X" in the system field or the file named .32x (`dot_32x`).
bool is_rom_start(ByteView head, bool dot_32x);
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::sega32x
