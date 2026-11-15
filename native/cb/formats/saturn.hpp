// cb/formats/saturn.hpp -- Sega Saturn discs: the boot sectors (IP.BIN) and the first program the disc loads.
//
// A Saturn disc starts with a system area of 16 sectors (32 KB, before the ISO 9660 volume descriptor at sector 16). Its first sectors are
// the IP.BIN the BIOS loads to $06002000 (the Saturn's high work RAM) and runs:
//
//     $000  "SEGA SEGASATURN "                     the hardware ID
//     $010  the maker ID, $020 the product number (10) and version (6), $030 the release date (8) and the device information (8),
//     $040  the area symbols (J T U B K A E L), $050 the compatible peripherals, $060 the game title (112 characters)
//     $0E0  the size of the IP in bytes            $0E8  the master SH-2's stack pointer, $0EC the slave's
//     $0F0  the first program's load address       $0F4  its size (0: the whole file)
//     $100  the security code (up to $E00), then the four area-code entries (32 bytes each, from $E00: JAPAN, TAIWAN and PHILIPPINES,
//           USA and CANADA, EUROPE: each begins with a `bra` to the next) and the initial program (from $E80) that loads the first
//           program from the disc and starts it
//
// The first program is the file the disc names first in its root directory (ABS.TXT, BIB.TXT and CPY.TXT are the licence texts), by
// convention 0.BIN, and is loaded at the address the header gives, usually $06004000. Both SH-2s share one program: the master runs it from
// its start, and starts the slave with the SMPC.
//
// Images this reads: `.iso` (2048 bytes a sector), `.bin` (raw 2352-byte sectors, mode 1 or mode 2 form 1) and the `.cue` sheet naming
// one. Only the data track's system area and its first program are read: the rest of the disc is the program's data.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::saturn {

inline constexpr std::size_t kSector = 2048;
inline constexpr std::size_t kRawSector = 2352;
inline constexpr std::size_t kSystemSectors = 16;
inline constexpr std::size_t kSystemBytes = kSystemSectors * kSector;   // the system area
inline constexpr std::uint32_t kIpBase = 0x06002000;                    // where the BIOS loads IP.BIN
inline constexpr std::uint32_t kHighRam = 0x06000000;                   // the high work RAM: 1 MB
inline constexpr std::size_t kHighRamSize = 0x100000;
inline constexpr std::uint32_t kAreaEntries = 0xE00;                    // the first of the four area-code entries, in IP.BIN
inline constexpr std::uint32_t kInitAt = 0xE80;                         // the initial program

struct Header {
    std::string hardware_id;       // "SEGA SEGASATURN"
    std::string maker;
    std::string product;
    std::string version;
    std::string release_date;
    std::string device;
    std::string areas;             // "JTUE": the regions the disc is for
    std::string peripherals;
    std::string title;
    std::uint32_t ip_size = 0;
    std::uint32_t master_stack = 0, slave_stack = 0;
    std::uint32_t first_read_address = 0, first_read_size = 0;
};

struct Disc {
    std::size_t sector_size = kSector;       // the user data of a sector as the image holds it: 2048
    Bytes system;                             // the 16 system sectors' user data (32 KB)
    Header header;
    std::string first_read_name;              // the root directory's first program ("0.BIN"); empty when the disc has none
    Bytes first_read;                         // its contents

    // The bytes of IP.BIN: `ip_size` of the system area, clamped to the system area and to an even length (at least the header).
    std::uint32_t ip_length() const;
    // The first program's load address (the header's; $06004000 when it says none).
    std::uint32_t load_address() const;
    // The 1 MB of high work RAM with IP.BIN at $06002000 and the first program where it loads.
    Bytes memory() const;

    // Function starts: the four area-code entries of IP.BIN and the first program's start, then every BSR target reached by following the
    // code from them (`decode` is the SH-2 decoder); ascending, as addresses.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError if the image is not a Saturn disc, or has no system area.
Disc parse(ByteView image, std::size_t sector_size = 0);
Disc read(const std::filesystem::path& path);

// "SEGA SEGASATURN " at the start of the disc's first sector, in a cooked or raw image, or in the image a `.cue` sheet names.
bool probe(const std::filesystem::path& path);
// The same for the bytes of a file's start (kRawHeader + 16 of them or more).
bool is_disc_start(ByteView head);

}  // namespace cb::formats::saturn
