// app/board_c_leaders.hpp -- the whole OS's near misses: across every scored program, the functions whose recompiled bytes came closest to the original without being
// identical. Functions of the right length rank first (board_c_functions.hpp), and very small ones are left out, because a few bytes can agree by chance.
// A snapshot of the results files as they were read. No operating-system API (board_c_leaders_test.cpp).
#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "app/board_c_functions.hpp"
#include "app/board_scores.hpp"

namespace cb::app::board {

inline constexpr std::size_t kCLeadersKept = 200;
inline constexpr std::uint64_t kCLeaderMinBytes = 16;

struct CLeader {
    std::string rel, program, function, words;
    double closeness = 0;
    bool same_size = false;
    std::uint64_t size = 0;
};

struct CLeaders {
    std::vector<CLeader> top;          // best first, at most kCLeadersKept
    std::uint64_t programs = 0;        // whose results were read
    std::uint64_t candidates = 0;      // near misses of at least kCLeaderMinBytes among them
    std::uint64_t scored = 0;          // CScores::scored when this was made: a different number means it is out of date
};

// Add one program's near misses to `into`, keeping the best.
void add_c_leaders(CLeaders& into, std::string_view rel, const CNearMisses& misses);
// Every scored program's results under root/c_scores. nullopt when `cancel` was set.
std::optional<CLeaders> build_c_leaders(const std::filesystem::path& root, const CScores& scores, const std::atomic<bool>& cancel);

}  // namespace cb::app::board
