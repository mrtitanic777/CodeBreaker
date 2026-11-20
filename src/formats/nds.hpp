// formats/nds.hpp -- Nintendo DS cartridges (`.nds`): the ROM image with its header, the two programs and the file system.
//
// The DS has two processors, and a cartridge holds a program for each: the ARM9 (an ARM946E-S, ARMv5TE: the main processor, 4 MB of main RAM at
// $02000000) and the ARM7 (an ARM7TDMI, the Game Boy Advance's processor: sound, touch screen, wireless, the link to the ARM9). The 512-byte header
// (padded with zeros to $4000, the "ROM header size" it names) says where each program lies in the file, where it is loaded in memory and where it
// starts:
//
//     $000 title (12)    $00C game code (4)    $010 maker code (2)    $012 unit code    $013 encryption seed    $014 device capacity
//     $01D region        $01E version          $01F autostart
//     $020 ARM9: ROM offset, entry address, RAM address, size    $030 ARM7: the same four
//     $040 file name table (FNT): offset, size    $048 file allocation table (FAT): offset, size
//     $050 ARM9 overlay table: offset, size       $058 ARM7 overlay table: offset, size
//     $060 card control (normal), $064 (KEY1)     $068 icon and title (the banner): offset     $06C secure area CRC-16    $06E secure area delay
//     $070 ARM9 / $074 ARM7 auto-load hooks       $078 secure area disable (8)    $080 total used ROM size    $084 header size ($4000)
//     $0C0 the Nintendo logo (156 bytes, the Game Boy Advance's)    $15C the logo's CRC-16 ($CF56)    $15E the header's CRC-16 (of $000-$15D)
//     $160 debug ROM offset, size, RAM address
//
// The file system is the FNT (directories and file names), the FAT (a pair {start, end} of file offsets for each file id) and the files themselves;
// the overlay tables list the program parts that are loaded on demand (each a file). The banner holds the icon (32 x 32, 4 bits a pixel, with a
// 16-colour palette) and the game's title in up to six languages, 256 bytes of UTF-16 each.
//
// Function addresses are real addresses in the program's own memory (the ARM9's and ARM7's code are each loaded at their RAM address), odd for a
// function in Thumb state (decompile/arm.hpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::nds {

inline constexpr std::size_t kHeaderSize = 0x200;      // the header proper; a ROM holds at least this
inline constexpr std::size_t kMaxRom = 512u * 1024 * 1024;
inline constexpr std::uint16_t kLogoCrc = 0xCF56;

enum class Cpu : std::uint8_t { Arm9, Arm7 };

// One of the two programs.
struct Binary {
    std::uint32_t rom_offset = 0, entry = 0, ram_address = 0, size = 0;
    std::uint32_t end() const { return ram_address + size; }
    bool holds(std::uint32_t address) const { return address >= ram_address && address - ram_address < size; }
};

struct Header {
    std::string title, game_code, maker_code;
    std::uint8_t unit_code = 0, seed = 0, capacity = 0, region = 0, version = 0, autostart = 0;
    Binary arm9, arm7;
    std::uint32_t fnt_offset = 0, fnt_size = 0, fat_offset = 0, fat_size = 0;
    std::uint32_t ovl9_offset = 0, ovl9_size = 0, ovl7_offset = 0, ovl7_size = 0;
    std::uint32_t banner_offset = 0, used_size = 0, header_size = 0;
    bool logo_ok = false;                    // the logo bytes are the console's
    std::uint16_t logo_crc = 0;              // $15C as stored
    std::uint16_t header_crc = 0;            // $15E as stored
    std::uint16_t computed_header_crc = 0;   // what $000-$15D make
    bool header_crc_ok() const { return header_crc == computed_header_crc; }
};

// A stretch of the file with a meaning of its own, in file order.
struct Region {
    enum class Kind : std::uint8_t { Header, Arm9, Arm7, Fnt, Fat, Overlays9, Overlays7, Banner, File };
    Kind kind = Kind::File;
    std::uint32_t offset = 0, size = 0;
    std::uint32_t id = 0;                    // File: its file id
    std::string name;                        // File: its path from the name table, if it has one
    std::uint32_t end() const { return offset + size; }
};

struct Rom {
    Bytes data;
    Header header;

    const Binary& binary(Cpu cpu) const { return cpu == Cpu::Arm9 ? header.arm9 : header.arm7; }
    // The program's bytes as they lie in memory (empty when the header's range is not inside the file).
    ByteView memory(Cpu cpu) const;
    // Which program an address belongs to (the ARM9's when both hold it); nullopt for neither.
    std::optional<Cpu> cpu_of(std::uint32_t address) const;

    // Function starts of one program, as real addresses (odd for Thumb): its entry point and every call target reached by following the code from it.
    // `decode` is the processor's decoder (source/isa: arm9_decoder for the ARM9, arm_decoder for the ARM7).
    std::vector<std::uint32_t> functions(Cpu cpu, const Decoder& decode) const;

    // The file's layout, in file order and not overlapping: the header, the two programs, the tables, the banner and each file the FAT lists. A
    // range that lies outside the file or inside an earlier one is left out.
    std::vector<Region> regions() const;
};

// FormatError if the file is shorter than the header, larger than 512 MB, or has no DS logo checksum ($CF56 at $15C).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// The DS logo checksum at $15C and the Game Boy Advance's logo at $0C0, in a file of at least 512 bytes.
bool probe(const std::filesystem::path& path);

// CRC-16 as the console computes it (the MODBUS polynomial $A001, starting at $FFFF).
std::uint16_t crc16(ByteView data);

}  // namespace cb::formats::nds
