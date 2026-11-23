// app/board_scores.hpp -- how the generated C of each program recompiled: root/c_scores.tsv, one row a program, measured by `codebreaker match` with the period compiler.
// "Identical" is byte for byte with relocations masked; a program that was not scored, or whose run ran out of time, has no numbers and is never counted as a failure of its C.
// No operating-system API (board_scores_test.cpp).
#pragma once

#include <cstdint>
#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>

namespace cb::app::board {

struct CScore {
    std::string format, result;        // result: "scored", "timeout", "failed"
    std::uint64_t functions = 0, identical = 0, same_size = 0, different = 0, no_compile = 0;
    std::uint64_t library = 0;         // recognized as the compiler's run-time library: linked, not written
    bool scored() const { return result == "scored"; }
};

struct CScores {
    std::unordered_map<std::string, CScore> by_card;   // by the card's rel
    CScore total;                      // over the scored programs
    std::uint64_t scored = 0, unscored = 0;
};

// The text of c_scores.tsv. nullopt when it is not that table.
std::optional<CScores> parse_c_scores(std::string_view text);
std::optional<CScores> load_c_scores(const std::filesystem::path& root);

// "4.2%": n of total, to a tenth of a percent.
std::string score_percent(std::uint64_t n, std::uint64_t total);
// For the status line: how many functions came back byte-identical, over how many programs.
std::string c_score_words(const CScores& scores);

}  // namespace cb::app::board
