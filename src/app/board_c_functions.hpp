// app/board_c_functions.hpp -- a program's near misses: the functions whose recompiled bytes came closest to the original without being identical, from the
// per-function results `codebreaker match --json` left in root/c_scores/<first 16 hex of the SHA-256 of the card's rel>.json.
// The measures are not one scale. At the original's length a 32-bit function has a share of bytes equal; at another length it has only a sequence similarity; a
// 16-bit function counts differing units. So a function of the right length always ranks above one of another length, and each says which measure it has.
// No operating-system API (board_c_functions_test.cpp).
#pragma once

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"
#include "app/board_scores.hpp"

namespace cb::app::board {

inline constexpr std::size_t kCNearMissesShown = 20;

struct CNearMiss {
    std::string name, words;           // words: "99.1% of bytes equal", "84.0% similar, 120 vs 130 B", "3 of 120 units differ"
    double closeness = 0;              // within its own kind of measure, 0 to 1
    bool same_size = false;
    std::uint64_t size = 0;
};

struct CNearMisses {
    std::vector<CNearMiss> closest;    // best first, as many as were asked for
    std::uint64_t candidates = 0;      // all that compiled, differ, and have something in common with the original
};

// The text of one results file. nullopt when it is not one (it may be half written).
std::optional<CNearMisses> parse_c_near_misses(std::string_view text, std::size_t keep = kCNearMissesShown);
// "ba7816bf8f01cfea.json"
std::string c_functions_file_name(std::string_view rel);
// The rel whose score stands for this card: its own, or a copy's (a scored one first). Empty: none.
std::string c_scored_rel(const Board& board, const Index& index, std::uint32_t card, const CScores& scores);
// nullopt when the file is missing or cannot be read just now.
std::optional<CNearMisses> load_c_near_misses(const std::filesystem::path& root, std::string_view rel, std::size_t keep = kCNearMissesShown);

}  // namespace cb::app::board
