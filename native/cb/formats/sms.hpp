// cb/formats/sms.hpp -- Sega Master System, Game Gear and SG-1000 cartridge images (`.sms`, `.gg`, `.sg`).
//
// A cartridge image is the ROM as it is, nothing in front (a 512-byte copier header is recognised by the size and
// dropped). The Master System BIOS checks a header at $7FF0 (or $3FF0 / $1FF0 for small ROMs): "TMR SEGA", two
// reserved bytes, the checksum, the product code and version, the region and the ROM size. The Game Gear's is the
// same with regions 5-7.
//
// The Z80 sees the ROM through Sega's mapper: three 16 KB slots at $0000, $4000 and $8000, each showing the page
// written to $FFFD / $FFFE / $FFFF (the first 1 KB always page 0, where the vectors are); RAM is 8 KB at $C000,
// repeated at $E000. At power-on the slots hold pages 0, 1 and 2, which is what memory() maps; other pages are
// listed by number, their addresses known only by running the program (it writes the mapper registers itself).
// SG-1000 cartridges have no mapper.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::sms {

inline constexpr std::size_t kPage = 0x4000;
inline constexpr std::array<std::string_view, 3> kExtensions = {".sms", ".gg", ".sg"};

enum class System { MasterSystem, GameGear, Sg1000 };
std::string_view to_string(System s);   // "Master System", "Game Gear", "SG-1000"

// The "TMR SEGA" header.
struct Header {
    std::uint32_t at = 0;                  // where it sits ($7FF0, $3FF0 or $1FF0)
    std::uint16_t checksum = 0;
    std::string product;                   // the BCD product code, leading zeros dropped ("0" if all zero)
    std::uint8_t version = 0;
    std::string region;                    // "Japan", "export", "international", or "?"
    std::optional<std::uint32_t> size;     // the ROM size the header declares, if its code is a listed one
};

// The addresses the Z80 starts at: reset, the maskable interrupt (RST $38) and the NMI.
inline constexpr std::uint32_t kResetVector = 0x0000;
inline constexpr std::uint32_t kIrqVector = 0x0038;
inline constexpr std::uint32_t kNmiVector = 0x0066;

struct Rom {
    Bytes data;                            // the ROM (a copier's header, when there was one, left out)
    std::optional<Header> header;          // SG-1000 and some homebrew have none
    System system = System::MasterSystem;
    bool has_mapper = false;               // more than the 48 KB the three slots show

    // The 16 KB pages (views into `data`; the last may be shorter).
    std::size_t page_count() const { return (data.size() + kPage - 1) / kPage; }
    ByteView page(std::size_t index) const;

    // The 64 KB the Z80 sees at power-on: pages 0-2 in slots 0-2, RAM zero.
    Bytes memory() const;

    // Where in the ROM the byte the Z80 reads at `address` at power-on is: an offset, or nullopt.
    std::optional<std::uint32_t> mapped(std::uint32_t address) const;

    // Function starts in the power-on address space ($0000-$BFFF): the reset, interrupt and NMI vectors, then every
    // CALL and RST target reached by following the code from them. `decode` is the Z80 decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// `extension` (".gg", ".sg"; anything else means Master System) names the system when the ROM has no header.
Rom parse(ByteView data, std::string_view extension = {});
Rom read(const std::filesystem::path& path);

// A file with one of the cartridge extensions that is at least 8 KB.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::sms
