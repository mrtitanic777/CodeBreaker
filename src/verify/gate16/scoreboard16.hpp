// verify/gate16/scoreboard16.hpp -- where each of a 16-bit module's functions differs from the original, and whose work
// that difference is.
//
//     codebreaker score16 <16-bit module> [--segment N] [--json FILE]
//     codebreaker score16 --show FUNCTION[,FUNCTION...] --json FILE      (functions from a saved board)
//
// Every reconstructed function is compiled twice from the same C, in the same source files and with the same switches: by
// Microsoft C 8.00c (the compiler that built the module) and by OpenWatcom with -zmsc (the fork that is to make Microsoft C 8's
// choices). Three comparisons follow:
//
//     C          Microsoft C 8's bytes against the original's: where they differ, the C is not what was written -- the
//                decompiler's work
//     compiler   OpenWatcom's bytes against Microsoft C 8's, from the same C: where they differ, the fork does not yet do what
//                Microsoft C 8 does -- the compiler's work
//     fits       OpenWatcom's length against the original's: a function no longer than the original goes back into its own
//                place, and nothing in the segment moves
//
// The decompiler's patterns are counted too (far pointers split into int pairs, CONCAT22, MK_FP, segment registers read through
// helpers, single-use temporaries), with the bytes the functions that show each one are off by, so the largest is the next to
// remove.
//
// The Microsoft C 8 half is match16's: the caller ran `match16::run` (without folding temporaries back: the C as the decompiler
// wrote it) and hands its `Result` in with the functions, their texts and the header; this module adds OpenWatcom's half, the
// rows and the report.
//
//     scoreboard16::Inputs in{.functions = fns, .result = &result, .texts = &texts, .header = header, .watcom = &ow,
//                             .toolchain = &chain};
//     scoreboard16::Rows rows = scoreboard16::run(in);
//     for (const std::string& line : scoreboard16::report_lines(rows)) ...
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <span>
#include <string>
#include <string_view>
#include <vector>

#include "base/watcom.hpp"
#include "core/ordered_map.hpp"
#include "verify/authored/match16.hpp"

namespace cb::verify::gate16::scoreboard16 {

using Log = std::function<void(std::string_view line)>;   // an empty function: nothing is logged
using Masked = authored::match16::Masked;

// What the decompiler writes that a programmer does not, and that a compiler turns into more code, and how often it shows in
// `text`: (name, occurrences) for each pattern that does, in the fixed order of the patterns.
std::vector<std::pair<std::string, std::size_t>> count_patterns(std::string_view text);

// OpenWatcom's switches for Microsoft's (tests/msc8 keeps the same mapping).
std::vector<std::string> watcom_flags(std::string_view microsoft_switches);

// The code alike, a fix-up's bytes (either side's) aside.
bool same_code(const Masked& a, const Masked& b);

// Without the padding a compiler puts after a function (int 3, nop, zero bytes).
Masked trim_padding(const Masked& code);

// What OpenWatcom made of each source file of the module.
struct WatcomCode {
    std::map<std::string, std::optional<Masked>> code;   // function -> its code, nullopt where OpenWatcom refused it
    std::map<std::string, std::string> refused;          // function -> why
};

// Each source file compiled by OpenWatcom -zmsc as Microsoft C compiled it (the same functions together, the same switches --
// `switches_of` is the Microsoft switches of a function). A file that does not compile is tried a function at a time, so one
// function's error does not sink its file. `files` lists function names; those with no text are left out.
WatcomCode compile_watcom(const base::watcom::OpenWatcom& watcom, const std::vector<std::vector<std::string>>& files,
                          const std::map<std::string, std::string>& texts, std::string_view header,
                          const std::function<std::string(const std::string& function)>& switches_of, const Log& log);

enum class Agreement { Same, Differs, NotApplicable };   // OpenWatcom's bytes against Microsoft C 8's: "same", "differs", "n/a"
std::string_view word(Agreement a);

struct Row {
    std::optional<std::uint32_t> segment;
    std::size_t original = 0;                                     // the original's length, padding aside
    std::optional<std::size_t> msc8;                              // Microsoft C 8's, nullopt where it did not compile
    store::ByteMatch msc8_vs_original = store::ByteMatch::NoCompile;
    std::size_t msc8_bytes_off = 0;
    std::optional<std::size_t> watcom;
    Agreement watcom_vs_msc8 = Agreement::NotApplicable;
    bool fits = false;                                            // OpenWatcom's is no longer than the original
    std::vector<std::pair<std::string, std::size_t>> patterns;   // the decompiler's patterns the C shows, and how often
    bool library = false;                                         // the compiler's run-time library, linked and not written
    std::string c;
    struct Hex {
        std::string original, msc8, watcom;
    } hex;
};

// The rows, in the module's order.
using Rows = OrderedMap<std::string, Row>;

struct Inputs {
    std::span<const authored::match16::Function> functions;   // the module's functions, in its order, with the original's bytes
    const authored::match16::Result* result = nullptr;        // what match16::run made of them (Microsoft C 8's half)
    const std::map<std::string, std::string>* texts = nullptr;   // each function's C
    std::string header;
    const base::watcom::OpenWatcom* watcom = nullptr;
    const authored::match16::Toolchain* toolchain = nullptr;  // the comparison of bytes
    std::optional<std::uint32_t> segment;                     // only the functions of this segment
    Log log;
};

Rows run(const Inputs& inputs);

// The summary of a board: the three comparisons and the largest patterns.
std::vector<std::string> report_lines(const Rows& rows);

// A function's C, and its original's, Microsoft C 8's and OpenWatcom's instructions side by side.
std::vector<std::string> show_lines(const Row& row);

// A board as JSON (the file `--json` writes) and back.
std::string rows_to_json(const Rows& rows);
Rows rows_from_json(std::string_view json);   // throws FormatError on a document that is no board

struct Arguments {
    std::string module;                               // a corpus name or a path
    std::optional<std::uint32_t> segment;
    std::string json;                                 // the file to write the board to, or to read it from for --show
    std::vector<std::string> show;                    // --show: the functions to show from a saved board
};
inline constexpr std::string_view kUsage = "codebreaker score16 <16-bit module> [--segment N] [--json FILE]\n"
                                           "codebreaker score16 --show FUNCTION --json FILE   (one function from a saved board)";
// nullopt: not a usage the command knows (print kUsage, exit 2).
std::optional<Arguments> parse_arguments(const std::vector<std::string>& args);

}  // namespace cb::verify::gate16::scoreboard16
