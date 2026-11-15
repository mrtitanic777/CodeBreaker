// cb/formats/gba.hpp -- Game Boy Advance cartridges (`.gba`, `.agb`, `.bin`): the ROM image the console maps at $08000000.
//
// A cartridge begins with a 192-byte header that the BIOS checks before it jumps to the first instruction:
//
//     $00  the entry point: one ARM instruction, a branch (`b start`), conventionally; the BIOS runs it in ARM state
//     $04  the Nintendo logo, 156 bytes (the BIOS compares it with its own copy and locks up when it differs)
//     $A0  the title, 12 bytes of upper-case text, zero padded      $AC  the game code, 4 characters (AGB-XXXX)
//     $B0  the maker code, 2 characters                             $B2  the fixed value $96
//     $B3  the main unit code (0 for the GBA)                       $B4  the device type (bit 7: debugging)
//     $B5  7 reserved bytes (zero)                                  $BC  the software version
//     $BD  the header complement: -(sum of $A0..$BC) - 25, the check byte the BIOS computes over the header
//     $BE  2 reserved bytes (zero)
//
// A program that is started from another console over the link cable (multiboot) has a boot header after these: at $C0 the boot mode,
// at $C4 the slave id; its code begins at $C0 or after. The cartridge ROM is up to 32 MB, mapped at $08000000 (and again at $0A000000
// and $0C000000 with different wait states); the processor is the ARM7TDMI (decompile/arm), which runs ARM and Thumb code.
//
// Addresses carry the instruction state in their lowest bit (decompile/arm.hpp): a function that starts in Thumb state has an odd address.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <set>
#include <string>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::gba {

inline constexpr std::uint32_t kRomBase = 0x08000000;
inline constexpr std::size_t kHeaderSize = 0xC0;
inline constexpr std::size_t kMaxRom = 32u * 1024 * 1024;
inline constexpr std::uint8_t kFixedValue = 0x96;

// The Nintendo logo, as the BIOS checks it ($04..$9F).
extern const std::array<std::uint8_t, 156> kLogo;

struct Header {
    std::uint32_t entry_word = 0;                 // the instruction at $00
    std::optional<std::uint32_t> entry;           // where its branch goes (an address in the ROM)
    bool logo_ok = false;                         // the logo is the console's
    std::string title;                            // $A0, 12 characters, trailing zeros removed
    std::string game_code;                        // $AC
    std::string maker_code;                       // $B0
    std::uint8_t unit_code = 0, device_type = 0, version = 0;
    std::uint8_t complement = 0;                  // $BD as stored
    std::uint8_t computed_complement = 0;         // what the header's bytes make it
    bool complement_ok() const { return complement == computed_complement; }
};

struct Rom {
    Bytes data;                                   // the whole file: the cartridge's ROM
    Header header;

    // The ROM as the processor sees it from $08000000.
    ByteView memory() const { return data; }
    std::uint32_t end() const { return kRomBase + static_cast<std::uint32_t>(data.size()); }

    // Function starts, as addresses ($08000000 and up, odd for Thumb): the entry point and every call target reached by following the
    // code from it. `decode` is the ARM decoder (source/isa: it reads the state from the address's lowest bit); it is given addresses
    // as offsets into the ROM.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the file is shorter than the header, larger than 32 MB, or has no $96 at $B2 (the mark every cartridge carries).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// $96 at $B2 and a branch instruction at $00 (what the BIOS starts), in a file of at least 192 bytes.
bool probe(const std::filesystem::path& path);

// The check byte over header bytes $A0..$BC.
std::uint8_t header_complement(ByteView header);

}  // namespace cb::formats::gba
