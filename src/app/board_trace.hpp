// app/board_trace.hpp -- real calls, measured while the operating system ran in an emulator (Win95VM's calltrace), named with the files installed on that machine.
//
// The trace holds addresses. A target counts as an import call only if it is a real export of an installed image; a source only if that image holds, at that address,
// the indirect call or jump the emulator saw, so two files with one preferred base are not confused. What cannot be named that way is counted, never guessed.
// Not seen by the trace: 16-bit code, VxDs, and direct calls. No operating-system API (board_trace_test.cpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <functional>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace cb::app::board {

struct MeasuredChannel {
    std::string from, to;              // file names, lower case
    std::uint64_t count = 0;
};
struct MeasuredFunction {
    std::string module, function, user;
    std::uint64_t count = 0;
};
struct Measured {
    double seconds = 0;                // of guest time
    std::uint64_t transfers = 0;       // every indirect call or jump counted
    std::uint64_t ring0 = 0, not_export = 0, no_source = 0, ambiguous = 0, inside = 0, named = 0;
    std::vector<MeasuredChannel> channels;     // busiest first
    std::vector<MeasuredFunction> functions;   // busiest first
    double per_hour(std::uint64_t count) const { return seconds > 0 ? static_cast<double>(count) * 3600 / seconds : 0; }
};

// A PE image installed on the traced machine.
struct InstalledImage {
    std::string name;                  // lower case
    std::uint32_t base = 0, size = 0;
    std::unordered_map<std::uint32_t, std::string> exports;   // RVA to its name, "#ordinal" when it has none; forwarders are not code here
    std::function<bool(std::uint32_t rva)> has_transfer;      // an FF /2 or FF /4 instruction is stored at this RVA
};
// Every 32-bit PE image under a folder.
std::vector<InstalledImage> read_installed(const std::filesystem::path& folder);

// The text calltrace wrote, against the images installed. nullopt when it is not a trace.
std::optional<Measured> read_trace(std::string_view text, std::span<const InstalledImage> images);

// root/measured.tsv, replaced whole.
void save_measured(const std::filesystem::path& root, const Measured& measured);
std::optional<Measured> load_measured(const std::filesystem::path& root);

}  // namespace cb::app::board
