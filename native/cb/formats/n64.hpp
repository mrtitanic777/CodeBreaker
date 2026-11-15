// cb/formats/n64.hpp -- Nintendo 64 cartridges: `.z64` (big-endian, the console's own byte order), `.v64` (every pair of bytes swapped) and
// `.n64` (every word's bytes reversed, so little-endian). A ROM is read and held in the big-endian form; `in_order` writes it back in the
// order it was read in.
//
// The first 64 bytes are the header (all fields big-endian):
//
//     $00 PI bus config   $80371240 (the domain 1 latency, pulse width, page size and release the PI is set up with)
//     $04 clock rate      (0: the default, 62.5 MHz)    $08 entry point   $0C release
//     $10 CRC1  $14 CRC2  (checksums of the first megabyte of the program: see `Rom::checksums`)    $18 8 reserved bytes
//     $20 name (20 bytes, space padded)    $34 7 reserved bytes    $3B category ('N' cartridge)   $3C game code (2 letters)
//     $3E country code ('E' USA, 'P' Europe, 'J' Japan ...)    $3F version
//
// $40-$FFF is the boot code, IPL3: 4032 bytes of R4300 code the console's PIF ROM runs from SP memory (it is held as data; it is a program in
// its own right and a copy of one of five Nintendo ones (the CIC chips' CIC-NUS-6101 ... 6106), or a homebrew replacement). It checks the
// CRCs, sets up RDRAM, copies the first 1 MB of the program from ROM $1000 to the entry point and jumps there. `Rom::load_address()` is
// that address: the header's entry point as IPL3 uses it (the 6103 and 6106 boot codes move it, by $100000 and $200000 below the header's
// value; the others use it as written). The program, as the lanes see it, is those bytes in RDRAM; whatever is in the ROM past 1 MB
// (and past what the program loads) is data the program reads through the PI.
//
// Function addresses are KSEG0 addresses ($80001000 is the usual first), as the program is linked.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::n64 {

inline constexpr std::size_t kHeaderSize = 0x40;
inline constexpr std::size_t kBootSize = 0x1000;                  // header and IPL3: the program starts here in the ROM
inline constexpr std::size_t kLoadSize = 0x100000;                // what IPL3 copies
inline constexpr std::size_t kRdramSize = 0x800000;
inline constexpr std::uint32_t kPiConfig = 0x80371240;
inline constexpr std::array<std::string_view, 3> kExtensions = {".z64", ".v64", ".n64"};

// How the file lays out the ROM's bytes.
enum class ByteOrder : std::uint8_t {
    Z64,   // as the console reads them: 80 37 12 40
    V64,   // pairs swapped: 37 80 40 12
    N64,   // words reversed: 40 12 37 80
};
std::string_view order_text(ByteOrder order);   // ".z64" ".v64" ".n64"

// The boot code's chip, from the boot code's CRC-32 (the five Nintendo ones; Unknown for any other: a homebrew IPL3).
enum class Cic : std::uint8_t { Unknown, C6101, C6102, C6103, C6105, C6106 };
std::string_view cic_text(Cic cic);   // "CIC-NUS-6102"

struct Header {
    std::uint32_t pi_config = 0;
    std::uint32_t clock_rate = 0;
    std::uint32_t entry = 0;
    std::uint32_t release = 0;
    std::uint32_t crc1 = 0, crc2 = 0;
    std::string name;                   // the 20 bytes, right-trimmed of spaces and zeros
    char category = 0;
    std::string game_code;              // 2 letters
    char country = 0;
    std::uint8_t version = 0;
};

struct Rom {
    Bytes data;                         // the whole ROM, big-endian (.z64 order)
    ByteOrder order = ByteOrder::Z64;   // the order the file had
    Header header;
    Cic cic = Cic::Unknown;
    std::uint32_t boot_crc32 = 0;       // CRC-32 of $40-$FFF

    // The boot code ($40-$FFF).
    ByteView boot_code() const { return ByteView(data).subspan(kHeaderSize, kBootSize - kHeaderSize); }
    // The program: what IPL3 copies (up to 1 MB from $1000).
    ByteView program() const;
    // Where IPL3 copies the program to and starts it (a KSEG0 address).
    std::uint32_t load_address() const;
    // The ROM in the byte order it was read in (the file as it was).
    Bytes in_order() const { return convert(data, ByteOrder::Z64, order); }

    // The RDRAM as the program finds it after IPL3: 8 MB, the program at its load address.
    Bytes memory() const;

    // The checksums of the program as the boot code computes them (nullopt for a ROM shorter than $101000 or a boot code that is not one of
    // the five whose algorithm is known), and whether the header's are those.
    std::optional<std::pair<std::uint32_t, std::uint32_t>> checksums() const;
    bool checksums_match() const;

    // Function starts: the entry point and every JAL target reached by following the code from it (the addresses come back as the program
    // writes them). `decode` is the R4300 decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;

    // `bytes` laid out as `from` says, laid out as `to` says.
    static Bytes convert(ByteView bytes, ByteOrder from, ByteOrder to);
};

// FormatError if the file is not a ROM (no PI configuration word in any of the three orders) or is shorter than the boot code.
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// The file begins with the PI configuration word in one of the three byte orders, and is at least as long as the boot code.
bool probe(const std::filesystem::path& path);
// The same from the first four bytes (nullopt: not a ROM).
std::optional<ByteOrder> order_of(ByteView head);

}  // namespace cb::formats::n64
