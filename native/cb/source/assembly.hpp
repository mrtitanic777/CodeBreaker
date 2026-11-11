// cb/source/assembly.hpp -- what the assembly-source writers share (drivers, real-mode programs).
//
//     operands("mov ax, 0x1f")              a decoder's operand text in the assembler's spelling: "mov ax, 01Fh"
//     data_lines(bytes)                     bytes as `db` lines, runs of text as text
//     Prober prober(tools, known_file);
//     prober.probe(candidates, labels, near, outdir, Mode::Flat32)
//                                           which instruction texts the assembler turns back into the original's bytes
//
// An instruction is written as its mnemonic only when the assembler turns that text back into the bytes the original
// has -- assemblers choose between equivalent encodings, and a different choice moves everything behind it. That is
// checked rather than assumed: every distinct instruction text is assembled once, at a place of its own in a probe
// file, and its bytes are read back from the object file. What the probe has established is remembered in a file
// (state/assembly_probes.json), so a text is assembled once however many programs use it.
//
// The assembler and linker are OpenWatcom's, given as `Tools` (where they are, the environment they run in).
#pragma once

#include <algorithm>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <map>
#include <mutex>
#include <optional>
#include <set>
#include <span>
#include <string>
#include <string_view>
#include <tuple>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::source::assembly {

// OpenWatcom's assembler and linker, and the environment they run in (WATCOM, PATH, INCLUDE ...). A tool that is not
// there is an empty path: using it fails with a message, not an exception.
struct Tools {
    std::filesystem::path wasm;
    std::filesystem::path wlink;
    std::map<std::string, std::string> env;
    std::chrono::milliseconds timeout = std::chrono::minutes(5);
};

// What a tool run made: the file (absent when it failed) and the tool's own messages (stdout and stderr, trimmed).
struct Made {
    std::optional<std::filesystem::path> file;
    std::string log;
};

// Assemble one file with `wasm -zq`; the object file is `<work>/<source stem>.obj`.
Made assemble(const Tools& tools, const std::filesystem::path& source, const std::filesystem::path& work);

// Link with `wlink` from the directives in `lines` (written to <out stem>.lnk, with a `name <out>` line added).
Made link(const Tools& tools, std::span<const std::string> lines, const std::filesystem::path& out, const std::filesystem::path& work);

// ---- spelling ---------------------------------------------------------------------------------------------

// The condition codes of the jump and set families.
inline constexpr std::string_view kConditions[] = {"o", "no", "b", "ae", "e", "ne", "be", "a", "s", "ns", "p", "np", "l", "ge", "le", "g"};

// Hexadecimal numbers the way the assembler reads them (0x1f -> 01Fh). Only a lower-case 0x number standing alone
// (not part of a longer word) is one.
std::string operands(std::string_view text);

// The assembler's name for a decoder's mnemonic ("pushal" -> "pushad", "int3" -> "int 3"); a name that is the same
// in both is returned as it is.
std::string_view mnemonic(std::string_view name);

// A string instruction as a decoder writes it: "rep movsb", "stosd".
struct StringOp {
    std::string_view prefix;   // "rep ", "repe ", "repne " or empty
    std::string_view op;       // movs cmps stos lods scas ins outs
    char width = 'b';          // b, w or d
};
// nullopt when `text` does not begin with one (the rest of the text may carry operands).
std::optional<StringOp> string_op(std::string_view text);

// Bytes as `db` lines of `per_line` values; runs of five or more printable characters (other than the quote) as quoted
// text, at most 60 to a line.
std::vector<std::string> data_lines(ByteView data, std::size_t per_line = 16);

// ---- probing ----------------------------------------------------------------------------------------------

// One instruction text to try: what it assembles to must equal `original` at the bytes outside patched fields.
struct Candidate {
    std::string text;
    Bytes original;   // the original bytes outside patched fields, in order
    Bytes mask;       // one byte per byte of the instruction: non-zero where a loader patches a field
    friend auto operator<=>(const Candidate&, const Candidate&) = default;
};

enum class Mode {
    Flat32,   // 32-bit code, flat model
    Real16,   // 16-bit real-mode code
};

// Remembers what probes have established, in a file the instances of CodeBreaker share. One Prober per file: its
// probes run one at a time.
class Prober {
public:
    Prober(Tools tools, std::filesystem::path known_file) : tools_(std::move(tools)), known_file_(std::move(known_file)) {}

    // Which candidates assemble to their original bytes. `labels` are the names the texts may mention; `near` the ones
    // used as call or jump targets. Candidates the file already has an answer for are not assembled again.
    std::set<Candidate> probe(std::span<const Candidate> candidates, const std::set<std::string>& labels, const std::set<std::string>& near,
                              const std::filesystem::path& outdir, Mode mode = Mode::Flat32);

    // {key: did the text assemble back} as the file has it (empty when there is none or it is damaged).
    std::map<std::string, bool> known() const;

private:
    Tools tools_;
    std::filesystem::path known_file_;
    std::mutex mutex_;
};

// The data records of the last segment an object file defines, {offset: bytes}, joined where one record continues the
// one before. FormatError when a data record is too short to say where it goes.
std::map<std::uint32_t, Bytes> last_segment_data(ByteView object);

// ---- finding the instructions that did not come back ---------------------------------------------------------

// One piece of the original, in order: where it is, how long, which of its bytes a loader patches (non-zero = patched;
// absent: none), the key of the instruction it is when it was written as a mnemonic, and whether it is a jump inside the
// segment.
template <class Key>
struct Piece {
    std::size_t offset = 0;
    std::size_t length = 0;
    std::optional<Bytes> mask;
    std::optional<Key> key;
    bool inner_jump = false;
};

// How much longer or shorter than the original an instruction may have come back, -8 to 16 bytes, the nearer to the
// original first.
inline constexpr int kSizeDeltas[] = {0, -1, 1, -2, 2, -3, 3, -4, 4, -5, 5, -6, 6, -7, 7, -8, 8, 9, 10, 11, 12, 13, 14, 15, 16};

// Which instructions did not come back as they were. `pieces` covers the original in order; the built bytes are walked
// piece by piece, so that an instruction that came back longer or shorter is found itself, and not the first place
// behind it where the bytes no longer line up. A jump that only changed because what it jumps over changed size is
// blamed only when nothing else is. -> the set of keys to write as bytes instead.
template <class Key>
std::set<Key> culprits(ByteView original, ByteView built, const std::vector<Piece<Key>>& pieces) {
    const auto same = [&](std::int64_t pos, const Piece<Key>& piece) {
        const std::int64_t n = static_cast<std::int64_t>(piece.length);
        if (pos < 0 || pos + n > static_cast<std::int64_t>(built.size())) return false;
        const ByteView got = built.subspan(static_cast<std::size_t>(pos), piece.length);
        const std::size_t from = std::min(piece.offset, original.size());
        const ByteView want = original.subspan(from, std::min(piece.length, original.size() - from));   // a piece past the end is short
        if (!piece.mask) return got.size() == want.size() && std::equal(got.begin(), got.end(), want.begin());
        const std::size_t m = std::min({got.size(), want.size(), piece.mask->size()});
        for (std::size_t i = 0; i < m; ++i) {
            if ((*piece.mask)[i] == 0 && got[i] != want[i]) return false;
        }
        return true;
    };

    std::int64_t pos = 0;
    std::set<Key> bad, jumps;
    const std::size_t count = pieces.size();
    std::size_t k = 0;
    while (k < count) {
        const Piece<Key>& piece = pieces[k];
        if (same(pos, piece)) {
            pos += static_cast<std::int64_t>(piece.length);
            ++k;
            continue;
        }
        if (piece.key) (piece.inner_jump ? jumps : bad).insert(*piece.key);
        // where does the built file continue? try the sizes this piece may have come back with, checking the next few
        // pieces that are long enough to mean something (or just the next one)
        std::size_t ahead = 0;
        for (std::size_t j = k + 1; j < std::min(k + 5, count); ++j) {
            if (pieces[j].length >= 2) ++ahead;
        }
        if (ahead == 0) ahead = std::min(k + 2, count) - std::min(k + 1, count);
        const std::size_t upto = std::min(k + 1 + std::max<std::size_t>(1, ahead), count);
        bool found = false;
        for (const int d : kSizeDeltas) {
            const std::int64_t p = pos + static_cast<std::int64_t>(piece.length) + d;
            if (p < pos) continue;
            std::int64_t q = p;
            bool all = true;
            for (std::size_t j = k + 1; j < upto; ++j) {
                if (!same(q, pieces[j])) {
                    all = false;
                    break;
                }
                q += static_cast<std::int64_t>(pieces[j].length);
            }
            if (all) {
                pos = p;
                found = true;
                break;
            }
        }
        if (!found) break;   // lost: what was found so far is the lesson
        ++k;
    }
    return bad.empty() ? jumps : bad;
}

}  // namespace cb::source::assembly
