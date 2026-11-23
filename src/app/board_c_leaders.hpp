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

// Every function of the scored programs, compactly: half a million of them stay searchable.
struct CCatalogEntry {
    std::uint32_t name_at = 0, program = 0, a = 0, b = 0;
    double closeness = 0;
    std::uint16_t name_size = 0;
    CVerdict verdict = CVerdict::Different;
    CMeasure measure = CMeasure::None;
};
struct CCatalog {
    std::string names;                 // the functions' names, end to end
    std::vector<CCatalogEntry> entries;
    std::vector<std::string> rels;     // the programs
};
void add_c_catalog(CCatalog& into, std::string_view rel, const std::vector<CFunctionRow>& rows);

struct CSearch {
    std::vector<CLeader> rows;         // the exact names first, then those that start with the first word, then the rest; closest first within each
    std::uint64_t matches = 0;         // all of them, shown or not
};
inline constexpr std::size_t kCSearchShown = 200;
// Every word of `query` (case ignored) must be in the function's name or its program's file name.
CSearch search_c_catalog(const CCatalog& catalog, std::string_view query, std::size_t keep = kCSearchShown);

// The rows as a table: rank, program, path, function, measured, closeness. Tabs and line breaks in a field become spaces.
std::string c_leaders_tsv(const std::vector<CLeader>& rows);
// A file name for an export of this search ("closest-functions.tsv" with none).
std::string c_leaders_file_name(std::string_view query);

struct CLeaders {
    CCatalog catalog;
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
