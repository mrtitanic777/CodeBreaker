// formats/neogeo.hpp -- Neo Geo (AES / MVS) cartridge programs: the P-ROM, the 68000's program.
//
// A Neo Geo cartridge is several ROM chips: P (the 68000 program), S (the fix layer), C (sprites), M (the Z80's sound program) and V
// (ADPCM samples). The 68000 sees the first megabyte of the P-ROM at $000000 and, in a window at $200000, one megabyte of the rest (a write to
// $2FFFF0 picks which). The system BIOS ($C00000, 128 KB) starts the game: the vectors in the P-ROM mostly point into it, it reads the header
// at $100 ("NEO-GEO", the game's number, the eye-catcher settings) and calls the game through four jump instructions at $122 (USER),
// $128 (PLAYER_START), $12E (DEMO_END) and $134 (COIN_SOUND). Level 1 is the vertical blank, level 2 the raster timer.
//
// Layouts this reads, all giving the P-ROM as the 68000 sees it (big-endian words, from address 0):
//   - the P-ROM as is (`.p1`, `.bin`, `-p1.*`);
//   - the P-ROM with each pair of bytes swapped (how some dumpers' files are stored), told by the header reading "EN-OEG" at $100;
//   - a `.neo` file (NeoSD / MAME-less container): a 4 KB header, then the P, S, M, V1, V2 and C ROMs one after another. The M-ROM is kept.
//   - two files holding the even and the odd bytes (`join_halves`).
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::neogeo {

inline constexpr std::size_t kHeaderAt = 0x100;
inline constexpr std::size_t kMinSize = 0x200;            // vectors and header
inline constexpr std::size_t kRomWindow = 0x100000;       // the first megabyte of the P-ROM, at $000000
inline constexpr std::uint32_t kBankAt = 0x200000;        // the window the other megabytes are switched into
inline constexpr std::uint32_t kBankSelect = 0x2FFFF0;
inline constexpr std::size_t kNeoHeader = 4096;
inline constexpr std::uint32_t kFirstCode = 0x122;        // the jump table of the header is the first code
inline constexpr std::array<std::string_view, 4> kCallbackNames = {"user", "player_start", "demo_end", "coin_sound"};

enum class Layout { Plain, WordSwapped, Neo, Halves };
std::string_view layout_text(Layout layout);

struct Header {
    std::uint8_t system_version = 0;      // $107
    std::uint16_t ngh = 0;                // $108: the game's number
    std::uint32_t program_size = 0;       // $10A
    std::uint32_t backup_ram = 0;         // $10E
    std::uint16_t save_size = 0;          // $112
    std::uint8_t eye_catcher = 0;         // $114: 0 the BIOS shows it, 1 the game does, 2 none
    std::uint8_t eye_catcher_bank = 0;    // $115
    std::array<std::uint32_t, 3> settings = {};   // $116: the Japan, US and Europe soft-DIP tables
    std::uint32_t security = 0;           // $182: the security code the BIOS calls (a pointer to a routine; 0 where there is none)
    // The target of each of the four jumps at $122, $128, $12E and $134 (kCallbackNames), 0 where the bytes there are not a JMP.
    std::array<std::uint32_t, 4> callbacks = {};
};

// What the `.neo` container says about the other ROMs.
struct Neo {
    std::uint32_t p_size = 0, s_size = 0, m_size = 0, v1_size = 0, v2_size = 0, c_size = 0;
    std::uint32_t year = 0, genre = 0, ngh = 0;
    std::string name, manufacturer;
};

// An exception or interrupt vector that points into the P-ROM.
struct Vector {
    std::string name;
    std::uint32_t address = 0;
};

// The name of vector number `n`: "bus_error" ... "vblank" (25), "timer" (26), "reset_button" (27), "trap0"; empty for one that is reserved.
std::string vector_name(unsigned n);

struct Rom {
    Layout layout = Layout::Plain;
    Bytes data;                           // the P-ROM as the 68000 sees it from address 0
    Header header;
    std::uint32_t initial_ssp = 0;
    std::uint32_t reset = 0;
    std::vector<Vector> vectors;          // each vector that is even, in the P-ROM and not before the jump table
    std::optional<Neo> neo;               // a `.neo` file's table of the other ROMs
    Bytes sound;                          // the M-ROM (the Z80's program) of a `.neo` file

    // The P-ROM in a 16 MB address space: the first megabyte at 0, the next at $200000.
    Bytes memory() const;
    // The address the byte at P-ROM offset `offset` is at (offsets from the second megabyte are in the $200000 window).
    static std::uint32_t address_of(std::size_t offset);
    // The P-ROM offset of an address, or `data.size()` when no byte of the P-ROM is there.
    std::size_t offset_of(std::uint32_t address) const;

    // Function starts: where the four callbacks jump to, the reset and the vectors the P-ROM holds, then every JSR / BSR target reached from them
    // (the JMP instructions of the header themselves are not functions: the flow starts from them as well, see `callback_jumps`).
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
    // The addresses of the header's JMP instructions ($122, $128, $12E, $134) whose callback is set.
    std::vector<std::uint32_t> callback_jumps() const;
};

// `extension` (".neo") is a hint. FormatError for an image that is too short, or has no "NEO-GEO" header.
Rom parse(ByteView data, std::string_view extension = {});
Rom read(const std::filesystem::path& path);

// A P-ROM whose bytes are stored in pairs reversed, turned into the 68000's order.
Bytes swap_words(ByteView data);
// Two files that hold the even and the odd bytes of a P-ROM, joined.
Bytes join_halves(ByteView even, ByteView odd);

// "NEO-GEO" (or its word-swapped form) at $100, or a `.neo` file.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::neogeo
