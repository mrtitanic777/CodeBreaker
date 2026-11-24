// app/board_c_history.hpp -- what changed in the measured C scores since a snapshot. A snapshot is a copy of c_scores.tsv kept under root/c_history/, named for
// when it was taken (20261124-034012.tsv); the Board compares the scores now with the newest one: functions newly byte-identical, programs that did better or worse,
// programs scored since. No operating-system API beyond reading and writing the snapshot files (board_c_history_test.cpp).
#pragma once

#include <chrono>
#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "app/board_scores.hpp"

namespace cb::app::board {

struct CHistoryRow {
    std::string rel, program;
    bool scored_before = false;          // (false: scored since the snapshot)
    std::uint64_t identical_before = 0, identical_now = 0, functions_now = 0;
    std::int64_t delta() const { return static_cast<std::int64_t>(identical_now) - static_cast<std::int64_t>(identical_before); }
};

struct CHistory {
    std::string snapshot;                // its file name; empty: none was taken
    std::uint64_t identical_before = 0, identical_now = 0, scored_before = 0, scored_now = 0;
    std::uint64_t better = 0, worse = 0, newly_scored = 0;
    std::vector<CHistoryRow> changes;    // the biggest changes first, then what was scored since (by name)
};

// The scores now against those of a snapshot named `snapshot`.
CHistory diff_c_scores(const CScores& before, const CScores& now, std::string snapshot);
// "+120 byte-identical functions · 14 programs better · 2 worse · 30 newly scored", or "No change".
std::string c_history_words(const CHistory& history);

// "20261124-034012.tsv" for that local time.
std::string c_snapshot_name(const std::chrono::local_seconds& when);
// "Nov 24, 2026 at 3:40 AM" from such a name; the name itself when it is not one.
std::string c_snapshot_words(std::string_view name);

// The newest snapshot's name and table. nullopt: none.
std::optional<std::pair<std::string, std::string>> load_latest_c_snapshot(const std::filesystem::path& root);
// Keep `table` (the text of c_scores.tsv) as root/c_history/<name>. The file written.
std::filesystem::path save_c_snapshot(const std::filesystem::path& root, std::string_view name, std::string_view table);

}  // namespace cb::app::board
