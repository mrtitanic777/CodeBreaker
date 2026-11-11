// cb/formats/zx.hpp -- ZX Spectrum programs: snapshots (`.sna`, `.z80`) and tapes (`.tap`).
//
// A snapshot is the machine stopped: the registers, and RAM from $4000 (16 KB of it the screen and the system
// variables, the program above). .sna (48K) is a 27-byte header and the 48 KB, with the program counter on the
// stack; .z80 holds the registers and the memory, compressed (ED ED n b runs) in its first version and in 16 KB
// pages from the second on (48K: page 8 at $4000, 4 at $8000, 5 at $C000; 128K: pages 3-10 are banks 0-7: bank 5
// at $4000, 2 at $8000, the one paged in at $C000). The program block taken from a snapshot is RAM from $5B00
// (after the screen and the printer buffer) to its end.
//
// A tape is a list of blocks: a 17-byte header (type: 0 a BASIC program, 3 code; name; length; the load address or
// the BASIC autostart line) then the data. The program is the code block; its entry is the address BASIC's USR
// calls (`RANDOMIZE USR 32768`), read from the tokenised BASIC.
#pragma once

#include <array>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::formats::zx {

inline constexpr std::uint32_t kProgramStart = 0x5B00;   // where a snapshot's program block begins
inline constexpr std::size_t kMemorySize = 0x10000;
inline constexpr std::array<std::string_view, 3> kExtensions = {".sna", ".z80", ".tap"};

enum class Format { Sna, Z80, Tap };

// The Z80's alternate register set (a snapshot of .sna kind stores it).
struct ShadowRegisters {
    std::uint8_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
};

struct Registers {
    std::uint8_t a = 0, f = 0, b = 0, c = 0, d = 0, e = 0, h = 0, l = 0;
    std::uint16_t ix = 0, iy = 0, sp = 0;
    std::uint8_t i = 0, r = 0;
    std::uint8_t im = 0;                         // interrupt mode
    std::optional<ShadowRegisters> shadow;       // .sna only
};

enum class Kind { Snapshot, Tape };

// A named entry point in the program block.
struct Entry {
    std::uint16_t address = 0;
    std::string name;                            // "start", "interrupt" or "usr_XXXX"
};

struct Program {
    Kind kind = Kind::Snapshot;
    std::optional<Registers> registers;          // a snapshot's; none for a tape
    Bytes memory;                                // the 64 KB address space as the program was left
    std::uint32_t origin = 0;                    // where the program block starts
    Bytes image;                                 // the program block: memory from `origin` (a tape: the code block)
    // Where the program starts: a snapshot's PC, a tape's USR calls (the first is "start"), an IM 2 interrupt handler.
    // In the order found; an address is listed once.
    std::vector<Entry> entries;
};

// The format a file name's extension says, if it is one of the three.
std::optional<Format> format_of(const std::filesystem::path& path);

// FormatError / TruncatedError if the data is not a valid file of the format (a snapshot too short, a tape with no
// code block ...).
Program parse(ByteView data, Format format);
// The same, for probing: the failure is the result, not an exception.
Result<Program> try_parse(ByteView data, Format format);

// Read a file by its extension. UnsupportedError for an extension that is none of the three.
Program read(const std::filesystem::path& path);

// A `.sna`/`.z80`/`.tap` file that parses.
bool probe(const std::filesystem::path& path);

}  // namespace cb::formats::zx
