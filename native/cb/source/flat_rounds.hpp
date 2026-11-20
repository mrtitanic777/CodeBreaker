// cb/source/flat_rounds.hpp -- an x86 file written whole as one NASM source, found out in rounds.
//
// The x86 files written whole -- a 32-bit Windows program, a 16-bit one, a driver (source/pe_writer, ne_writer, le_writer) -- are
// one NASM source each, assembled flat (-f bin). What the assembler does with each instruction is found out, not assumed: every
// round assembles the whole file, reads where each instruction went from the assembler's map of labels, and writes as its bytes,
// next round, each instruction the assembler rejected or gave back different; until nothing differs.
//
// A writer implements FlatImage<Key> -- how its file is written with a set of instructions held as bytes, and what "different"
// means for it -- and flat() runs the rounds. `Key` names an instruction in the writer's own terms (an address, a segment and an
// offset) and is ordered.
//
//     class PeImage : public FlatImage<std::uint32_t> { ... };
//     FlatResult<std::uint32_t> r = flat(original, image, work, "prog", {});
//     if (r.same) ...
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "cb/compare/image.hpp"
#include "cb/core/bytes.hpp"
#include "cb/core/error.hpp"

namespace cb::source::flat_rounds {

// Where NASM put a label: its address in its section, and its place in the file.
struct Label {
    std::uint64_t address = 0;
    std::uint64_t place = 0;
};
// The map of labels NASM writes with `[map symbols ...]`: by label name, looked up only.
using LabelMap = std::unordered_map<std::string, Label>;

// A file NASM assembled flat.
struct Assembled {
    std::optional<std::filesystem::path> file;   // the image; nothing when NASM failed
    std::string said;                            // what the assembler said (stderr, then stdout)
    std::filesystem::path map;                   // its map of labels
};

// Assemble `source` flat (-f bin) as <work>/<name>, with its map of labels beside it (<name>.map). The source is text in Latin-1.
Assembled assemble(std::string_view source, const std::filesystem::path& work, std::string_view name);

// The labels of a map file ("<place> <address> <name>" lines).
LabelMap read_map(const std::filesystem::path& path);

// The line numbers (1-based) NASM's messages name as errors ("file.asm:123: error: ...").
std::vector<std::size_t> error_lines(std::string_view said);

// ---- the rounds -------------------------------------------------------------------------------------------------

template <class Key>
struct Source {
    std::string text;
    std::map<std::size_t, Key> instruction_at_line;   // by the 1-based line of `text` the instruction is written on
};

template <class Key>
class FlatImage {
public:
    virtual ~FlatImage() = default;
    // The file with the instructions in `raw` as bytes. `final`: the last writing, without the round's marks.
    virtual Source<Key> write(const std::set<Key>& raw, bool final) = 0;
    // The instructions the assembler did not give back, given what it built and its labels.
    virtual std::set<Key> differing(ByteView built, const LabelMap& labels, const std::set<Key>& raw) = 0;
};

struct FlatOptions {
    std::size_t max_rounds = 30;
    bool new_only = false;                                   // the assembler rejecting only what is already bytes ends it
    std::function<void(std::string_view)> say = nullptr;     // progress, one line a round
};

template <class Key>
struct FlatResult {
    std::filesystem::path source;
    std::optional<std::filesystem::path> image;   // the file built
    bool same = false;                            // it is the original
    std::size_t rounds = 0;
    std::set<Key> raw;                            // the instructions written as bytes
    std::string log;                              // the end of the assembler's last message
};

// The end of a message, at most `n` bytes.
std::string tail(std::string_view said, std::size_t n);

// The whole file at `path`, for the rounds (IoError when it cannot be read).
Bytes read_all(const std::filesystem::path& path);

// The rounds that find which instructions are written as their bytes, then the file built. ToolError when the assembler rejects
// nothing it can be blamed for (anything, or with `new_only`, anything not already bytes).
template <class Key>
FlatResult<Key> flat(ByteView original, FlatImage<Key>& image, const std::filesystem::path& work, std::string_view name,
                     const FlatOptions& options) {
    const auto say = [&](const std::string& line) {
        if (options.say) options.say(line);
    };
    std::set<Key> raw;
    std::size_t rounds = 0;
    std::string said;
    while (rounds < options.max_rounds) {
        ++rounds;
        const Source<Key> src = image.write(raw, false);
        const Assembled built = assemble(src.text, work, name);
        said = built.said;
        if (!built.file) {
            std::set<Key> bad;
            for (const std::size_t n : error_lines(said)) {
                if (n < 2) continue;
                if (const auto hit = src.instruction_at_line.find(n - 1); hit != src.instruction_at_line.end()) bad.insert(hit->second);
            }
            say("round " + std::to_string(rounds) + ": " + std::to_string(bad.size()) + " instructions the assembler rejects");
            std::set<Key> fresh;
            for (const Key& k : bad) {
                if (!raw.contains(k)) fresh.insert(k);
            }
            if ((options.new_only ? fresh : bad).empty()) throw ToolError("nasm", 1, tail(said, 2000), "rejects the source");
            raw.insert(bad.begin(), bad.end());
            continue;
        }
        const Bytes data = read_all(*built.file);
        const std::set<Key> bad = image.differing(data, read_map(built.map), raw);
        say("round " + std::to_string(rounds) + ": " + std::to_string(bad.size()) + " instructions come back different");
        if (bad.empty()) break;
        raw.insert(bad.begin(), bad.end());
    }
    const Source<Key> src = image.write(raw, true);
    const Assembled built = assemble(src.text, work, name);
    FlatResult<Key> r;
    r.source = work / (std::string(name) + ".asm");
    r.raw = std::move(raw);
    r.rounds = rounds;
    r.log = tail(built.said, 2000);
    if (built.file) {
        const Bytes data = read_all(*built.file);
        r.image = built.file;
        r.same = data.size() == original.size() && std::equal(data.begin(), data.end(), original.begin());
        if (!r.same) {
            const std::size_t at = compare::image::first_difference(data, original).value_or(original.size());
            say("first difference at file offset " + std::to_string(at) + " (sizes " + std::to_string(data.size()) + ", " +
                std::to_string(original.size()) + ")");
        }
    }
    return r;
}

// What a whole-file writer reports of its result, and the exit code: 0 when the rebuilt file is the original.
struct FlatReport {
    std::size_t instructions = 0;
    std::size_t raw = 0;
    std::size_t rounds = 0;
    bool same = false;
    bool built = false;
    std::filesystem::path source;
};
struct ReportText {
    std::string text;
    int exit_code = 0;
};
ReportText report(const std::filesystem::path& program, const FlatReport& r);

}  // namespace cb::source::flat_rounds
