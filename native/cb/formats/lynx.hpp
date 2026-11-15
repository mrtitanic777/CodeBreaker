// cb/formats/lynx.hpp -- Atari Lynx programs: cartridge images (`.lnx`) and BLL / BS93 objects (`.o`, the homebrew loader format).
//
// A `.lnx` file is a 64-byte header ("LYNX", the two banks' block sizes, a version, the cart's name and maker, the screen rotation, the
// AUDIN and EEPROM flags) and the cartridge's bytes. The Lynx has no program ROM of that kind: its boot ROM reads the first byte of the
// cartridge (256 minus a count of encrypted blocks, 1 to 5), reads 51 bytes per block, decrypts each with RSA (cube modulo a 512-bit
// public key: the keys Handy and Felix publish), and runs the result at $0200. That first stage then reads the rest of the cart through
// Suzy's RCART0 register, a directory entry and a program at a time, into RAM, and runs it.
//
// What CodeBreaker takes "the program" to be, then, is the image the boot sequence leaves in RAM:
//
//   * the first stage is decrypted here (`stage1`, the plain code at $0200); the encrypted bytes stay the source's data;
//   * what the first stage and the loaders it starts copy from the cartridge into RAM is found by running them (emulate/lynx.hpp plays
//     the boot ROM: the decrypt, and the shift routine at $FE00 that selects a cartridge block). Each stretch of the cart copied to a
//     run of RAM is a `Segment`; the places the loaders jump into are the `entries`. The cartridge's other bytes (the directory, padding,
//     an unloaded block) are data.
//
// A BS93 object is the format homebrew build tools produce for a development loader (a Comlynx cable, a flash cart, Handy): ten bytes
// ($80 $08, the load address and the length of header and body, both big-endian, "BS93") and the body, which loads at its address and
// starts there. It is one segment and one entry, found without running anything.
//
// The 64 KB the CPU sees (`memory()`): RAM $0000-$FBFF, with the first stage at $0200 and each segment over it, in cart order.
#pragma once

#include <cstdint>
#include <filesystem>
#include <map>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/formats/code_scan.hpp"

namespace cb::formats::lynx {

inline constexpr std::size_t kHeaderSize = 64;
inline constexpr std::size_t kBs93Header = 10;
inline constexpr std::size_t kEncryptedBlock = 51;       // bytes of one encrypted block
inline constexpr std::size_t kMaxEncryptedBlocks = 5;
inline constexpr std::uint32_t kRamEnd = 0xFC00;         // Suzy begins here
inline constexpr std::uint32_t kStage1Address = 0x0200;  // where the boot ROM puts the decrypted first stage

enum class Kind { Cartridge, Bs93 };

// A stretch of the file loaded to RAM.
struct Segment {
    std::uint32_t offset = 0;    // in the file (the header counted)
    std::uint32_t address = 0;   // in RAM
    std::uint32_t size = 0;
    friend bool operator==(const Segment&, const Segment&) = default;
};

struct Program {
    Kind kind = Kind::Cartridge;
    Bytes file;                      // the file as it was
    // cartridge header
    std::string name;
    std::string manufacturer;
    std::uint16_t bank0_block = 0;   // the block size of bank 0 in bytes (1024 for a 256 KB cart)
    std::uint16_t bank1_block = 0;
    std::uint16_t version = 0;
    std::uint8_t rotation = 0;       // 0 none, 1 left, 2 right
    std::uint8_t audin = 0;
    std::uint8_t eeprom = 0;
    // the first stage
    std::uint32_t encrypted_blocks = 0;   // 0 when the first byte is not a plausible count
    Bytes stage1;                         // decrypted (50 bytes per block); empty when it did not decrypt
    // BS93
    std::uint16_t load_address = 0;
    std::uint16_t block_length = 0;       // header and body
    // what the loaders do (a BS93 object: its one segment; a cartridge: filled by `set_loaded`)
    std::vector<Segment> segments;
    std::vector<std::uint32_t> entries;

    std::size_t data_offset() const { return kind == Kind::Cartridge ? kHeaderSize : 0; }
    // The cartridge's bytes (the file after the header).
    ByteView cart() const { return ByteView(file).subspan(data_offset()); }

    // What a boot run found: the segments (sorted by file offset, none overlapping in the file) and the addresses the loaders entered.
    void set_loaded(std::vector<Segment> loaded, std::vector<std::uint32_t> entered);

    // The 64 KB the CPU sees at the start of a run (see above).
    Bytes memory() const;

    // The RAM address the file offset `offset` is loaded to, if some segment holds it and no later segment covers that address.
    std::optional<std::uint32_t> address_of(std::uint32_t offset) const;

    // Function starts: the entries and every call target followed from them. `decode` is the 65SC02 decoder.
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// Decrypt `count` encrypted blocks (51 bytes each) as the boot ROM does; empty when a block's check byte or the running sum is wrong.
Bytes decrypt_blocks(ByteView encrypted, std::size_t count);

// Parse a `.lnx` image or a BS93 object. FormatError if it is neither or is cut short.
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// Is the file a `.lnx` ("LYNX" at 0) or a BS93 object ("BS93" at 6, a length the file holds)? Reads a few bytes.
bool probe(const std::filesystem::path& path);
bool is_cartridge(ByteView data);
bool is_bs93(ByteView data);

// The hardware registers by their documented names: Suzy at $FC00, Mikey at $FD00 and the memory map control at $FFF9.
const std::map<std::uint32_t, std::string>& registers();

}  // namespace cb::formats::lynx
