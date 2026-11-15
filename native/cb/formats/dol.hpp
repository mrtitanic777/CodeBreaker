// cb/formats/dol.hpp -- Nintendo GameCube and Wii executables (`.dol`): the program the console's boot code or the Homebrew Channel loads.
//
// A DOL is a 256-byte header followed by up to 18 sections: 7 of code ("text") and 11 of data. The header, all words big-endian:
//
//     $00  the file offset of each section (18 words: text 0-6, then data 0-10)
//     $48  the load address of each section (18 words)
//     $90  the size of each section in bytes (18 words)
//     $D8  the address of the zero-filled (bss) area       $DC  its size
//     $E0  the entry point                                  $E4  28 bytes of padding
//
// A section that is not used has offset, address and size 0. Sections are loaded at their address (cached, virtual: 0x80000000 and up; the
// Wii's second memory, MEM2, is at 0x90000000) and the program starts at the entry point, running on the GameCube's Gekko or the Wii's
// Broadway (decompile/ppc: the same instructions). The format does not say which console the program is for; `console()` guesses from what
// the program touches: MEM2 or the Wii's Hollywood registers mean the Wii, everything else the GameCube.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/ppc_segment.hpp"

namespace cb::formats::dol {

inline constexpr std::size_t kHeaderSize = 0x100;
inline constexpr std::size_t kTextSections = 7;
inline constexpr std::size_t kDataSections = 11;
inline constexpr std::size_t kSections = kTextSections + kDataSections;

enum class Console { GameCube, Wii };
std::string_view console_text(Console console);   // "GameCube" / "Wii"

struct Dol {
    Bytes data;                                       // the whole file
    std::array<std::uint32_t, kSections> offsets{};
    std::array<std::uint32_t, kSections> addresses{};
    std::array<std::uint32_t, kSections> sizes{};
    std::uint32_t bss_address = 0, bss_size = 0, entry = 0;

    // The sections that are used, in section order (code first).
    std::vector<ppc_segment::Segment> segments() const;
    // Which console it is for (see above).
    Console console() const;
    // The section that holds `address`, or nullptr.
    const ppc_segment::Segment* segment_at(const std::vector<ppc_segment::Segment>& segments, std::uint32_t address) const;
};

// FormatError if the file is shorter than the header, a section lies outside the file or is not word-aligned in memory, no section is used,
// a section's address is not a console address, or the entry point is not in a code section.
Dol parse(ByteView data);
Dol read(const std::filesystem::path& path);

// A header that parse() accepts (reads the header only).
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::dol
