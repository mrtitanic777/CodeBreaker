// formats/segacd.hpp -- Sega CD / Mega-CD discs: the two 68000 programs on the boot sectors.
//
// A Sega CD disc starts with a system area of 16 sectors (32 KB, before the ISO 9660 volume descriptor at $8000). Its first sector is the
// boot header, and what follows is code:
//
//     $000  "SEGADISCSYSTEM  "                           the disc's ID
//     $010  volume name, $020 system name                 11 characters each, then a version and a type
//     $030  IP: start, size, entry, work RAM size         four longs; start and size are offsets in the system area
//     $040  SP: start, size, entry, work RAM size
//     $100  a Mega Drive header ("SEGA MEGA DRIVE", the names, the serial, the regions)
//     $200  the security code for the disc's region, running up to the IP
//
// The main CPU's program is the security code and the IP: bytes $200 up to the IP's end, which the BIOS loads at $FF0000 of the main CPU's
// work RAM and runs (the first function is the security code at $FF0000; the IP's entry is its start plus its entry offset). The sub CPU's
// program, the SP, is loaded to $6000 of its PRG-RAM, after the CD BIOS's $6000 bytes. It begins with a module header -- the name
// ("MAIN-SUBCPU"), a version, the offset of its jump table at $18 -- and the table is four words, each an offset from the table's start to the
// SP's initialisation, its main loop, the level 2 interrupt handler and the user call (0 for none). The sub CPU calls the CD BIOS through
// `jsr $5F22` (_CDBIOS) with a function number in D0.
//
// Images this reads: 2048 bytes a sector (`.iso`) or raw 2352 (`.bin`, with the 12-byte sync and a 4-byte header, mode 1); a sector's user
// data is what the boot area is made of. Only the system area is read: the files after it are the program's data and the programs the IP
// loads from them.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::segacd {

inline constexpr std::size_t kSector = 2048;
inline constexpr std::size_t kRawSector = 2352;
inline constexpr std::size_t kRawHeader = 16;
inline constexpr std::size_t kBootBytes = 0x8000;            // the system area: 16 sectors
inline constexpr std::uint32_t kSecurityAt = 0x200;          // the first byte of the main CPU's program
inline constexpr std::uint32_t kMainBase = 0xFF0000;         // where the main CPU's program is loaded
inline constexpr std::uint32_t kSubBase = 0x6000;            // where the sub CPU's program is loaded
inline constexpr std::uint32_t kBiosCall = 0x5F22;           // _CDBIOS
inline constexpr std::array<std::string_view, 4> kSubEntryNames = {"sp_init", "sp_main", "sp_int2", "sp_user"};

// IP and SP, as the boot header says them.
struct Program {
    std::uint32_t start = 0, size = 0, entry = 0, work_ram = 0;
};

struct Header {
    std::string disc_id;           // "SEGADISCSYSTEM"
    std::string volume, system;
    std::string system_type;       // $100: "SEGA MEGA DRIVE"
    std::string copyright;
    std::string domestic_name, overseas_name;
    std::string serial;
    std::string region;            // $1F0
    Program ip, sp;
};

struct Disc {
    std::size_t sector_size = kSector;    // 2048 or 2352
    Bytes boot;                            // the system area's user data (up to 32 KB)
    Header header;
    // The boot offsets the programs come from: the main program is boot[main_begin, main_end), the sub program boot[sub_begin, sub_end).
    std::uint32_t main_begin = kSecurityAt, main_end = 0;
    std::uint32_t sub_begin = 0, sub_end = 0;    // equal when the disc has no SP

    bool has_sub() const { return sub_end > sub_begin; }
    // The address the byte at boot offset `offset` is at on its CPU; `offset` must be in a program.
    std::uint32_t main_address(std::uint32_t offset) const { return kMainBase + (offset - main_begin); }
    std::uint32_t sub_address(std::uint32_t offset) const { return kSubBase + (offset - sub_begin); }
    // Does the address belong to the sub CPU's program (below the main CPU's work RAM)?
    static bool is_sub(std::uint32_t address) { return address < kMainBase; }

    // The main CPU's address space (16 MB, the program at $FF0000) and the sub CPU's (its PRG-RAM from 0, the SP at $6000).
    Bytes main_memory() const;
    Bytes sub_memory() const;

    // The SP's four entries (init, main, int2, user) as addresses; 0 where the table says none or the offset is outside the SP.
    std::array<std::uint32_t, 4> sub_entries() const;

    // Function starts of both programs (main: the security code, the IP's entry; sub: the SP's table entries), then every JSR / BSR target
    // reached from them; ascending.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the image is not a Sega CD disc, or too short for the boot header.
Disc parse(ByteView data);
Disc read(const std::filesystem::path& path);

// "SEGADISCSYSTEM" at the start of the disc's first sector, in a 2048-byte or a raw 2352-byte image.
bool probe(const std::filesystem::path& path);
// The same for the bytes of a file's start (kRawHeader + 14 of them or more), so another format can tell a Sega CD disc from a cartridge.
bool is_disc_start(ByteView head);

}  // namespace cb::formats::segacd
