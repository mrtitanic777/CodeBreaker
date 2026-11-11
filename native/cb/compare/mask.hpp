// cb/compare/mask.hpp -- the parts of a function left out when it is compared with another build of it.
//
// Two builds of the same code differ where a loader or linker filled something in: a relocated address, the
// distance of a call, a far call the linker rewrote. Every comparison of one function with another -- a library's
// function against a program, a compiled reconstruction against the original, one version of a module against the
// next -- leaves those places out, and does it here.
//
// A mask comes in one of two forms. For code read a byte at a time (x86) a `ByteMask` says, byte by byte, whether
// the byte is left out: any non-zero flag means left out (`of` makes one from a set of offsets). A mask shorter than
// the code covers only its prefix: the bytes past its end are compared. For code made of fixed-size words
// (PowerPC) a `WordMask` says, word by word, which bits of the word are left out -- a relocation there fills a
// field of the instruction, and its opcode and registers are still compared.
//
//     Bytes m = mask::of(std::vector{3, 4, 5}, code.size());
//     bool same = mask::same(built, original, {m});
//     DistanceResult d = mask::distance(a, b, ma, mb);
#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <string_view>
#include <utility>
#include <vector>

#include "cb/core/bytes.hpp"

namespace cb::compare::mask {

using ByteMask = ByteView;                          // flag per byte, non-zero = left out
using WordMask = std::span<const std::uint64_t>;    // per word, the bits left out

// How two byte runs compare, best first; the words the verdict tables print are `to_text`.
enum class Match { Identical, SameSize, Different };
std::string_view to_text(Match m);                  // "identical", "same size", "different"

// A byte mask `size` long that leaves out every offset in `offsets` (those outside it are ignored).
template <class Offsets>
Bytes of(const Offsets& offsets, std::size_t size) {
    Bytes out(size, 0);
    for (auto o : offsets) {
        if (!std::cmp_less(o, 0) && std::cmp_less(o, size)) out[static_cast<std::size_t>(o)] = 1;
    }
    return out;
}

// `code` with the bytes at `offsets` set to zero: what is left to compare, or to digest. Offsets outside the code
// are ignored.
template <class Offsets>
Bytes blank(ByteView code, const Offsets& offsets) {
    Bytes out(code.begin(), code.end());
    for (auto o : offsets) {
        if (!std::cmp_less(o, 0) && std::cmp_less(o, out.size())) out[static_cast<std::size_t>(o)] = 0;
    }
    return out;
}

// Whether `a` and `b` are as long as each other and the same at every byte no mask leaves out.
bool same(ByteView a, ByteView b, std::span<const ByteMask> masks = {});

// The offsets (up to the shorter one's length) where `a` and `b` differ and no mask leaves the byte out.
std::vector<std::size_t> differing(ByteView a, ByteView b, std::span<const ByteMask> masks = {});

// Whether two runs of words are the same outside the bits `ignore` leaves out of each word. Compares as many words
// as the shortest of the three runs holds.
bool same_words(std::span<const std::uint64_t> a, std::span<const std::uint64_t> b, WordMask ignore);

enum class ByteOrder { Little, Big };

struct DistanceResult {
    Match verdict = Match::Identical;
    std::size_t differing = 0;                      // units that differ (a longer or shorter run counts its tail)
    std::size_t compared = 0;                       // units in the longer run
    std::optional<std::size_t> first;               // the first unit that differs
};

// How far `a` is from `b`, byte by byte, outside what the two masks leave out. When the lengths differ the whole
// tail counts as differing, so a longer or shorter piece is always farther than one of the right length.
DistanceResult distance(ByteView a, ByteView b, ByteMask ma = {}, ByteMask mb = {});

// The same unit by unit, a unit being a word of `unit` bytes (1..8) read in `order`; `ma[i]` / `mb[i]` are the bits
// of word i left out (a missing entry leaves none out). Throws UsageError for a unit outside 1..8.
DistanceResult distance_words(ByteView a, ByteView b, std::size_t unit, ByteOrder order, WordMask ma = {},
                              WordMask mb = {});

}  // namespace cb::compare::mask
