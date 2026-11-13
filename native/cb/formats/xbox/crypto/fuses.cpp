#include "cb/formats/xbox/crypto/fuses.hpp"

#include <algorithm>
#include <bit>
#include <format>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/formats/xbox/crypto/tool_support.hpp"

namespace cb::formats::xbox::crypto::fuses {

namespace {

constexpr std::size_t kLockdownFirstLine = 6;   // lines 6 and 7 hold the lock-down bits
constexpr std::size_t kLockdownLines = 2;

}  // namespace

Dump Dump::parse(ByteView data) {
    if (data.size() < kDumpSize) throw FormatError("fuses", data.size(), "fuse data too small (twelve lines of eight bytes are needed)");
    Dump dump;
    for (std::size_t i = 0; i < kLineCount; ++i) std::copy_n(data.begin() + static_cast<std::ptrdiff_t>(i * kLineSize), kLineSize, dump.lines_[i].begin());
    return dump;
}

Bytes Dump::cpu_key() const {
    Bytes key;
    for (const std::size_t i : {std::size_t{3}, std::size_t{4}}) key.insert(key.end(), lines_[i].begin(), lines_[i].end());
    return key;
}

unsigned Dump::lockdown_value() const {
    unsigned weight = 0;
    for (std::size_t i = kLockdownFirstLine; i < kLockdownFirstLine + kLockdownLines; ++i)
        for (const std::uint8_t b : lines_[i]) weight += static_cast<unsigned>(std::popcount(b));
    return weight;
}

void set_line(Bytes& image, std::size_t line, ByteView value) {
    if (line >= kLineCount) throw UsageError("fuse line " + std::to_string(line) + " is past the twelfth");
    if (value.size() != kLineSize) throw UsageError("a fuse line is eight bytes");
    if (image.size() < kDumpSize) image.resize(kDumpSize, 0);
    std::copy(value.begin(), value.end(), image.begin() + static_cast<std::ptrdiff_t>(line * kLineSize));
}

std::string report(const Dump& dump) {
    std::string out;
    for (std::size_t i = 0; i < kLineCount; ++i) {
        if (i == 3) {
            out += std::format("  {:<20} {}\n", "cpu_key", support::hex_upper(dump.cpu_key()));
        } else if (i != 4) {
            out += std::format("  {:<20} {}\n", std::format("fuseline_{:02}", i), support::hex_upper(dump.line(i)));
        }
    }
    out += std::format("  {:<20} {}\n", "ldv", dump.lockdown_value());
    return out;
}

cli::Command command() {
    return cli::Command{"fuses",
                        "parse an Xbox 360 eFuse dump (fuse lines, CPU key, lock-down value)",
                        {cli::Positional{"dump", "the fuse dump (96 bytes)"}},
                        [](const cli::Args& args) {
                            cb::out_text(report(Dump::parse(fs::read_file(fs::from_text(args.required("dump"))))));
                            return 0;
                        },
                        {},
                        "",
                        ""};
}

}  // namespace cb::formats::xbox::crypto::fuses
