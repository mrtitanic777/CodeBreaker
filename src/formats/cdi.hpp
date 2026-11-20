// formats/cdi.hpp -- Philips CD-i programs: the CD-RTOS (OS-9/68000) modules on a disc image, or a module file.
//
// The CD-i player's processor is a Philips 68070 (a 68000 with a 24-bit address bus and an on-chip UART, timer and DMA), and its operating system is
// CD-RTOS, a variant of Microware's OS-9. A program is a *module*: a header, then the code, position-independent (everything is reached through
// the data pointer in A6 and PC-relative addressing), with the system called through `TRAP #0` followed by a word naming the call. The header
// (big-endian, $30 bytes, then the executable part):
//
//     $00 sync $4AFC   $02 system revision   $04 module size (long)   $08 owner   $0C name offset (long)   $10 access
//     $12 type (1 program, 2 subroutine, 3 multi, 4 data, $B ..., $C system, $D file manager, $E physical driver, $F descriptor)   $13 language (1: 68000 object)
//     $14 attributes   $15 revision   $16 edition   $18 usage   $1C symbol table   $2E header parity (the one's complement of the XOR of the
//     header's other words: the XOR of all 24 words is $FFFF)   $30 execution offset (long)   $34 exception offset   $38 memory size   $3C stack size ...
//
// A disc image (`.iso` / `.bin` / `.cdi`, 2048-byte sectors, the Disc Label "CD-I " in an early sector) is read for the modules in it: a file starts at
// a sector, and a module that passes the header parity starts there, the next one where the first ends. A file that is a module (`.mod`, `.rtf`, or
// one that starts with the sync) is read as one. The filesystem (path table, directories) is not read.
//
// What the model runs: the executable modules of type 1, 2, 3, $C, $D and $E in the 68000's language, each placed at an address of its own in the
// first megabyte (from $10000, each rounded up to $100), its entry its base plus the execution offset.
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <vector>

#include "core/bytes.hpp"
#include "formats/code_scan.hpp"

namespace cb::formats::cdi {

inline constexpr std::uint16_t kSync = 0x4AFC;
inline constexpr std::size_t kHeader = 0x30;
inline constexpr std::size_t kSector = 2048;
inline constexpr std::uint32_t kFirstBase = 0x10000;
inline constexpr std::uint32_t kLimit = 0x100000;       // modules are placed below this
inline constexpr std::size_t kMemorySize = 0x1000000;

struct Module {
    std::uint32_t file_offset = 0;
    std::uint32_t size = 0;
    std::string name;
    std::uint8_t type = 0, language = 0, attributes = 0, revision = 0;
    std::uint16_t edition = 0;
    std::uint32_t exec_offset = 0, memory = 0, stack = 0;
    std::uint32_t address = 0;   // where the model places it (0: not placed)
    bool runs() const { return address != 0; }
    std::uint32_t entry() const { return address + exec_offset; }
};

struct Program {
    Bytes file;
    bool disc = false;                  // a disc image (the Disc Label was seen) rather than a module file
    std::vector<Module> modules;        // every module found, in file order
    std::vector<Module> placed() const;   // the ones the model runs

    // The 16 MB the 68000 sees: the placed modules at their addresses, the rest zero.
    Bytes memory() const;
    // The file offset of an address inside a placed module, or nullopt.
    std::optional<std::uint32_t> offset(std::uint32_t address) const;
    const Module* module_at(std::uint32_t address) const;
    std::vector<std::uint32_t> entries() const;
    std::vector<std::uint32_t> functions(const Decoder& decode) const;
};

// FormatError when the bytes hold no module.
Program parse(ByteView data);
Program read(const std::filesystem::path& path);

// A `.iso` / `.bin` / `.cdi` / `.img` that has the Disc Label or a module at a sector start, or a `.mod` / `.rtf` / file starting with a module. Reads the first sectors only.
bool probe(const std::filesystem::path& path);

// The header at `at` of `d` as a module, nullopt where it is not one (the sync, the size inside the data, the parity).
std::optional<Module> module_at(ByteView d, std::size_t at);

}  // namespace cb::formats::cdi
