// formats/dreamcast.hpp -- Sega Dreamcast discs: IP.BIN (the boot sectors) and 1ST_READ.BIN (the first program), from a .gdi, .cdi or .iso/.bin.
//
// A Dreamcast disc's boot track starts with a system area of 16 sectors (32 KB, before the ISO 9660 volume descriptor at sector 16): IP.BIN. The
// BIOS loads it to $8C008000 (the main RAM, cached view; $0C008000 is its physical address) and runs it; it loads the file its header names
// (1ST_READ.BIN) to $8C010000, right behind itself, and starts it:
//
//     $000  "SEGA SEGAKATANA "                     the hardware ID
//     $010  the maker ID ("SEGA ENTERPRISES"), $020 the device information (4 hex digits of CRC, " GD-ROM", the disc number "1/1")
//     $030  the area symbols (J U E), $038 the peripherals the program supports (seven hex digits of flags)
//     $040  the product number (10), $04A the version (6), $050 the release date (YYYYMMDD), $060 the boot file name ("1ST_READ.BIN")
//     $070  the software maker (16), $080 the title (128)
//     $100  the rest of the 32 KB: the table of the disc's contents, the area-protection code, the bootstrap that loads the program
//           (the BIOS enters it at $300)
//
// 1ST_READ.BIN is the first program, a SH-4 program built for $8C010000. Homebrew burned to a CD-R ("selfboot") stores it scrambled (the BIOS
// of the console undoes it when the disc is a CD-R: 32-byte slices of every 2 MB chunk in an order a pseudo-random sequence seeded with the file's
// size gives); `descramble` and `scramble` are exact inverses, and `looks_scrambled` decides which a file is (see the .cpp).
//
// Images this reads:
//   .gdi   the text file naming the tracks of a GD-ROM dump (track number, start sector, type, sector size, file name, offset); IP.BIN is in the
//          last data track (the high-density area, usually track 3), whose file holds raw 2352-byte sectors or cooked 2048-byte ones
//   .cdi   a DiscJuggler image: the sessions' tracks one after another, then the descriptor. The boot track is found by its content (IP.BIN's
//          hardware ID at a sector start, with an ISO 9660 volume descriptor 16 sectors on), in whichever sector size the track was
//          stored; the descriptor is not needed to reach it
//   .iso   2048-byte sectors;  .bin  raw 2352-byte sectors (mode 1, or mode 2 form 1)
// An ISO 9660 volume on a multi-session or GD-ROM track names its files by sector numbers from the start of the disc, not of the track; the
// reader finds the offset from where the root directory lies.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::dreamcast {

inline constexpr std::size_t kSector = 2048;
inline constexpr std::size_t kRawSector = 2352;
inline constexpr std::size_t kSystemSectors = 16;
inline constexpr std::size_t kSystemBytes = kSystemSectors * kSector;   // IP.BIN
inline constexpr std::uint32_t kRamBase = 0x0C000000;                   // the main RAM (physical): 16 MB, seen at $8C000000 cached and $AC000000 uncached
inline constexpr std::size_t kRamSize = 0x1000000;
inline constexpr std::uint32_t kIpBase = 0x0C008000;                    // IP.BIN, where the BIOS loads it (physical)
inline constexpr std::uint32_t kFirstReadBase = 0x0C010000;             // 1ST_READ.BIN, where the bootstrap loads it
inline constexpr std::uint32_t kIpEntry = 0x300;                        // the offset in IP.BIN the BIOS jumps to
inline constexpr std::uint32_t kCachedView = 0x80000000;                // what a program adds to a physical RAM address to use it cached

enum class Container : std::uint8_t { Iso, Raw, Gdi, Cdi };

struct Header {
    std::string hardware_id;       // "SEGA SEGAKATANA"
    std::string maker;
    std::string device;            // the device information
    std::string areas;             // "JUE"
    std::string peripherals;
    std::string product;
    std::string version;
    std::string release_date;
    std::string boot_file;         // "1ST_READ.BIN"
    std::string software_maker;
    std::string title;
};

struct Disc {
    Container container = Container::Iso;
    std::size_t tracks = 1;                   // a .gdi's track count (1 for the others)
    Bytes system;                             // IP.BIN: the 16 system sectors' user data (32 KB)
    Header header;
    std::string first_read_name;              // the file the header names, as the root directory spells it; empty when the disc has none
    Bytes first_read;                         // its contents as the disc holds them
    bool scrambled = false;                   // is `first_read` scrambled? (then `program` is it undone)
    Bytes program;                            // what is loaded at $8C010000: `first_read` descrambled

    std::uint32_t ip_length() const { return static_cast<std::uint32_t>(std::min(system.size(), kSystemBytes)); }
    std::uint32_t load_address() const { return kFirstReadBase; }
    // IP.BIN followed by the program: $0C008000 up (the program starts exactly where IP.BIN's 32 KB end).
    Bytes memory() const;

    // Function starts: the bootstrap's entry in IP.BIN and the program's start, then every BSR target reached by following the code from them
    // (`decode` is the SH-4 decoder); ascending, as physical addresses.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// A file's contents as a disc of this machine's: parse from the bytes of a .iso or .bin (the sector size is found from the data), or a .cdi
// (searched for its boot track); `sector_size` 0 finds it. FormatError for anything else.
Disc parse(ByteView image, std::size_t sector_size = 0);
// Any of the image kinds above, by the file's extension (a .gdi reads the track files beside it). IoError / FormatError.
Disc read(const std::filesystem::path& path);

// Is it a Dreamcast disc image? Cheap for .iso/.bin (the first sector), a read of the text for .gdi, a scan for .cdi.
bool probe(const std::filesystem::path& path);
// "SEGA SEGAKATANA " at the start of the first sector, in a cooked or raw image.
bool is_disc_start(ByteView head);

// The scramble 1ST_READ.BIN is stored with on a CD-R, and its inverse. Both leave a file of the same size; the last bytes of a chunk that do not
// fill a 32-byte slice stay where they are.
Bytes descramble(ByteView data);
Bytes scramble(ByteView data);
// Does this file read as a program only after `descramble`? (The SH-4 program's delay slots tell: a branch's slot is never another branch.)
bool looks_scrambled(ByteView data);

}  // namespace cb::formats::dreamcast
