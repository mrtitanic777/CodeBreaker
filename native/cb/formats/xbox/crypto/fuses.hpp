// cb/formats/xbox/crypto/fuses.hpp -- the Xbox 360's eFuse dump: twelve lines of eight bytes, one of them (lines 3 and 4, 0x18..0x28)
// the console's CPU key.
//
//     fuses::Dump dump = fuses::Dump::parse(file_bytes);        // FormatError when shorter than the twelve lines
//     dump.cpu_key();                                            // 16 bytes
//     dump.lockdown_value();                                     // the lock-down value: how many updates the console has taken
//     fuses::set_line(image, 3, key_first_half);                 // build a dump
//
// The lock-down value (LDV) is the number of bits set in fuse lines 6 and 7 (0x30..0x40). The command `crypto fuses <dump>` prints each
// line, the CPU key and the LDV.
#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <string>

#include "cb/core/bytes.hpp"
#include "cb/core/cli.hpp"

namespace cb::formats::xbox::crypto::fuses {

inline constexpr std::size_t kLineSize = 8;
inline constexpr std::size_t kLineCount = 12;
inline constexpr std::size_t kDumpSize = kLineSize * kLineCount;
inline constexpr std::size_t kCpuKeyOffset = 0x18;

using Line = std::array<std::uint8_t, kLineSize>;

class Dump {
public:
    // The first twelve lines of `data` (more bytes are ignored). FormatError when there are fewer.
    static Dump parse(ByteView data);

    const Line& line(std::size_t index) const { return lines_.at(index); }
    // Lines 3 and 4, which hold the CPU key.
    Bytes cpu_key() const;
    unsigned lockdown_value() const;

private:
    std::array<Line, kLineCount> lines_{};
};

// Write `value` (eight bytes) into fuse line `line` of a dump image. UsageError for a line past the twelfth or a value that is not eight bytes.
void set_line(Bytes& image, std::size_t line, ByteView value);

// The report: "  fuseline_NN" per line (lines 3 and 4 as one "cpu_key" row), then the lock-down value, as the tool prints it.
std::string report(const Dump& dump);

cli::Command command();

}  // namespace cb::formats::xbox::crypto::fuses
