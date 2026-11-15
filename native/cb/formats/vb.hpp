// cb/formats/vb.hpp -- Nintendo Virtual Boy cartridge images (`.vb`, `.vboy`): the ROM as the CPU sees it at the top of its address space.
//
// The Virtual Boy's CPU is an NEC V810 with 27 address lines. The cartridge ROM is a power of two in size (16 KB to 16 MB) and appears at $07000000-$07FFFFFF, repeated to fill
// the 16 MB; the top of the 32-bit space ($FFFFFxxx) is the same ROM again, so the last bytes of the file are what the CPU sees at $FFFFFFFF. Everything the console needs fixed is
// in the file's last 0x220 bytes:
//
//     size - $220   the header: title (20 bytes, Shift-JIS), 5 reserved bytes, maker (2), game code (4), version (1)       ($FFFFFDE0)
//     size - $200   the interrupt and exception vectors, 16 bytes apiece: game pad, timer zero, game pak, comm, VIP ($FFFFFE00..$FFFFFE40), then
//                   floating point ($FFFFFF60), divide by zero ($70), invalid opcode ($80), trap 0-15 ($A0), trap 16-31 ($B0), address trap ($C0), duplexed exception ($D0)
//     size - $10    the reset vector ($FFFFFFF0): where the CPU starts
//
// Each slot holds code (typically a `jr` to the handler). A game's code is linked at $07000000 + its offset in the file, so that is the address this reader gives every byte.
#pragma once

#include <cstdint>
#include <filesystem>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::formats::vb {

inline constexpr std::uint32_t kBase = 0x07000000;       // where the ROM is linked: base + offset is a byte's address
inline constexpr std::size_t kMinSize = 0x4000;
inline constexpr std::size_t kMaxSize = 0x1000000;

// One interrupt, exception or reset slot: `from_end` bytes before the end of the ROM.
struct VectorSlot {
    std::uint32_t from_end;
    std::string_view name;      // "reset", "gamepad", "timer", "game_pak", "comm", "vip", ...
};
const std::vector<VectorSlot>& vector_slots();

struct Header {
    std::string title;          // trailing spaces and zeros dropped
    std::string maker;
    std::string game_code;
    std::uint8_t version = 0;
};

struct Rom {
    Bytes data;                 // the file
    Header header;

    std::size_t header_offset() const { return data.size() - 0x220; }
    std::uint32_t address(std::size_t offset) const { return kBase + static_cast<std::uint32_t>(offset); }
    // The address the CPU starts at: the code at the reset slot.
    std::uint32_t reset_address() const { return address(data.size() - 0x10); }
    // The slots (offset in the file, name) that hold code: the reset slot always; a vector slot when its first halfword is neither $0000 nor $FFFF (padding).
    struct Entry {
        std::size_t offset;
        std::string_view name;
    };
    std::vector<Entry> entries() const;
};

// FormatError when the file is not a Virtual Boy cartridge (a power of two from 16 KB to 16 MB, a reset slot that holds code).
Rom parse(ByteView data);
Rom read(const std::filesystem::path& path);

// A `.vb` / `.vboy` file of a power-of-two size whose reset slot holds code. Reads the file's tail only.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::vb
