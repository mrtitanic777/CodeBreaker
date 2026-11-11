// cb/exact/aim.hpp -- where a candidate's code differs from the program's, and how: what the beam rewrites first.
//
// The compiler's listing of a candidate gives each instruction the line of the source it came from. The candidate's
// instructions are lined up with the program's (both read by the same disassembler, the numbers a link would change
// masked), and each stretch that differs is told by its kind:
//
//     Frame    the same instructions, only the places in the frame ([bp - N]) other
//     Width    a byte read where a word is, or the other way (byte ptr, cbw, sub ah,ah)
//     Sign     a jump of the other signedness (jl / jb ..), sar / shr, imul / mul
//     Order    the same instructions in another order or layout (the jumps, the blocks)
//     Shape    something else: the code of a statement itself
//
// and by the lines of the candidate that stretch comes from. A diagnosis is one compile more (the caller keeps it).
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"
#include "cb/exact/x86.hpp"

namespace cb::exact::aim {

enum class Kind { Frame, Width, Sign, Order, Shape };

// The kind's name as the refiners' tables spell it ("frame", "width", "sign", "order", "shape").
std::string_view kind_name(Kind k);

// One instruction of a compiler listing: where it is in the function, its bytes, the source line before it.
struct ListingEntry {
    std::uint32_t offset = 0;
    Bytes bytes;
    std::optional<std::uint32_t> line;
};

// What differs between the program's function and the candidate's.
struct Diagnosis {
    // How many instructions differ, by kind, in the order the kinds were first met (callers rank them by count and
    // let that order break a tie).
    std::vector<std::pair<Kind, std::size_t>> kinds;
    std::vector<std::uint32_t> lines;   // source lines of the differing stretches, the first one first
    std::optional<std::uint32_t> first() const {
        return lines.empty() ? std::nullopt : std::optional<std::uint32_t>(lines.front());
    }
};

// The instructions of the listing's function (the first, or `name`'s; a leading underscore is not compared), each
// with the source line before it ("; Line N"). The listing is the MASM-style text the Microsoft compilers write
// with their code bytes: `*** 00001a  55 8b ec ...`.
std::vector<ListingEntry> listing_code(std::string_view listing, std::optional<std::string_view> name = std::nullopt);

// The candidate's function bytes, from its listing (the instructions laid at their offsets from the first's).
Bytes code_of(const std::vector<ListingEntry>& listing);

// An instruction as compared: mnemonic and operands, the numbers a link changes (places of code and data, targets)
// masked, a frame's place and a small constant kept.
std::string normalize(const x86::Insn& insn);

// The kind of a differing stretch: `a` the program's, `b` the candidate's (normalized instructions).
Kind classify(const std::vector<std::string>& a, const std::vector<std::string>& b);

// `orig` the program's bytes of the function, `code` the candidate's (code_of its listing), `listing` its
// listing_code.
Diagnosis diagnose(ByteView orig, ByteView code, const std::vector<ListingEntry>& listing,
                   const x86::Decoder16& decoder = x86::capstone_decoder());

// ---- the line-up (difflib's longest-matching-block algorithm, no junk) -----------------------------------------

struct Opcode {
    enum class Tag { Replace, Delete, Insert, Equal };
    Tag tag = Tag::Equal;
    std::size_t a1 = 0, a2 = 0, b1 = 0, b2 = 0;   // a[a1, a2) turns into b[b1, b2)
    friend bool operator==(const Opcode&, const Opcode&) = default;
};

// How to turn `a` into `b`: the edit script of difflib.SequenceMatcher(None, a, b, autojunk=False), the earliest
// longest match first. The same alignment tools (the source-difference views, the beam's "does this change touch a
// line") use.
std::vector<Opcode> opcodes(const std::vector<std::string>& a, const std::vector<std::string>& b);

}  // namespace cb::exact::aim
