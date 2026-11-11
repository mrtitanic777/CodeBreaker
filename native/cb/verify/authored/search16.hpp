// cb/verify/authored/search16.hpp -- the last bytes of a near miss, found by trying small rewrites of its source.
//
//     codebreaker match <16-bit module> --search ROUNDS
//
// A function a few bytes from the original under Microsoft C is often one the compiler would make exactly from a
// source written a little differently: its locals declared in another order (the compiler lays the frame out by
// declaration, so every [bp-N] moves), or one of them unsigned where the decompiler guessed signed (a compare, a shift,
// a widening). Each round takes every near miss's best text so far, rewrites it one small way, compiles all of them (one
// emulator session for each compiler and switches), and keeps a rewrite that comes nearer. A rewrite that comes out
// identical is source: the compiler says so, byte for byte.
//
// The rewrites are drawn from a seeded generator, so a search is repeatable: the same near misses, seed and compiler give
// the same rounds.
#pragma once

#include <compare>
#include <cstddef>
#include <cstdint>
#include <functional>
#include <map>
#include <optional>
#include <random>
#include <string>
#include <string_view>
#include <vector>

#include "cb/verify/verdicts.hpp"

namespace cb::verify::authored::search16 {

using Rng = std::mt19937_64;

// How near a rewritten function's bytes are to the original's.
struct ByteVerdict {
    ByteMatch match = ByteMatch::NoCompile;
    std::size_t differing = 0;   // bytes that differ
    std::size_t total = 0;       // bytes compared
};

// The compiler and the switches a function is judged under (and which of them won, as the caller records it).
struct Group {
    std::string compiler, switches, winner;
    auto operator<=>(const Group&) const = default;
};

// A function's best text so far, its verdict, and the group it is judged under.
struct NearMiss {
    std::string function;
    std::string text;
    ByteVerdict verdict;
    Group group;
};

// What the search found for a function: its best text and that text's verdict.
struct Best {
    std::string function;
    std::string text;
    ByteVerdict verdict;
};

// Compile and judge `texts` ({function: text}) under one group -> {function: verdict}.
using CompileGroup = std::function<std::map<std::string, ByteVerdict>(const Group&, const std::map<std::string, std::string>&)>;
// A verdict's score: higher is nearer.
using Score = std::function<double(const ByteVerdict&)>;
using Log = std::function<void(std::string_view line)>;   // an empty function: nothing is logged

struct Options {
    int rounds = 1;
    std::uint64_t seed = 1995;
    Log log;
};

// Search for nearer texts for the near misses; the best found per function, in the order given. A function whose
// verdict is already identical is left as it is.
std::vector<Best> search(const std::vector<NearMiss>& near, const CompileGroup& compile_group, const Score& score,
                         const Options& options);

// One small rewrite of `text`, or nullopt when there is nothing of the kind to rewrite. Kinds: two locals declared in
// the other order; a local's signedness; a comparison with a constant written the other way (`x <= 0x19` as
// `x < 0x1a`); an if/else with its branches swapped under the negated test; two adjacent statements exchanged;
// `x = x + y` as `x += y` (and `x += 1` as `x++`) or back; `(a == b)` as `(b == a)`.
std::optional<std::string> rewrite(std::string_view text, Rng& rng);

// ---- the pieces of rewrite, for tests ---------------------------------------------------------------------
// The indices of the body's leading declarations (not the K&R parameter declarations).
std::vector<std::size_t> leading_declarations(const std::vector<std::string>& lines);
// `cond` said the other way: a comparison turned around, anything else under a !.
std::string negate_condition(std::string_view cond);

// An if/else written a block to a line: the line of the if, of its else, and of the closing brace of the else.
struct IfElse {
    std::size_t if_line = 0, else_line = 0, end_line = 0;
    bool operator==(const IfElse&) const = default;
};
std::vector<IfElse> if_else_blocks(const std::vector<std::string>& lines);

// The first line k (from `from`) of each pair of adjacent simple statements of one block (neither a declaration
// nor a control line, one of them an assignment).
std::vector<std::size_t> swappable_statements(const std::vector<std::string>& lines, std::size_t from);

}  // namespace cb::verify::authored::search16
