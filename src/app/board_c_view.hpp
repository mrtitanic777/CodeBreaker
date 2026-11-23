// app/board_c_view.hpp -- the measured C recompile scores (board_scores.hpp) as the Board page uses them: a mark on each program's card, the whole OS added up,
// and the columns narrowed and ordered by C result. Identical copies of a file are one program: scored once, counted once. A program with no score is never
// 0% and never a failure of its C: it has a state of its own. No operating-system API (board_c_view_test.cpp).
#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"
#include "app/board_scores.hpp"

namespace cb::app::board {

enum class CState : std::uint8_t {
    None,          // not a converted program and not in the table: no mark at all
    NotScored,     // converted, and the scorer has not reached it
    OutOfTime,     // the scorer gave up on it: no numbers
    ScorerFailed,  // the scorer itself stopped: no numbers, and nothing said about its C
    NoFunctions,   // scored, with nothing to compare
    Scored,
};

struct CMark {
    CState state = CState::None;
    std::uint64_t functions = 0, identical = 0, same_size = 0, no_compile = 0;
};

// What the row of C filters offers. All is none of them pressed.
enum class CView : std::uint8_t { All, MostIdentical, NearMisses, WillNotCompile, NotScored };
inline constexpr std::size_t kCViews = 4;   // the filters, without All

// The whole OS, each distinct program once.
struct CSummary {
    std::uint64_t functions = 0, identical = 0, same_size = 0, different = 0, no_compile = 0, library = 0;
    std::uint64_t scored = 0;              // programs with a measurement, those with no functions among them
    std::uint64_t not_scored = 0, out_of_time = 0, scorer_failed = 0;
    std::array<std::uint64_t, kCViews> in_view{};   // programs under each filter
    std::uint64_t unscored() const { return not_scored + out_of_time + scorer_failed; }
    std::uint64_t programs() const { return scored + unscored(); }
};

struct CMarks {
    std::vector<CMark> by_card;            // one per Board::cards
    CSummary summary;
};

// `index` (may be null) says which cards are copies of one file.
CMarks build_c_marks(const Board& board, const Index* index, const CScores& scores);

// Only the cards of a view, best first (ties keep their order). All leaves the stacks alone.
bool in_c_view(const CMark& mark, CView view);
void apply_c_view(Stacks& stacks, const CMarks& marks, CView view);

// The few words on a card: "4.2%", or what stands in for a number. Empty: no mark.
std::string c_mark_words(const CMark& mark);
// The share of a scored program's functions that came back byte-identical, 0 to 1.
float c_identical_share(const CMark& mark);

std::string_view c_view_name(CView view);
// How a view orders its cards, for the head of a column: the mark on a card is always its byte-identical share.
std::string_view c_view_order(CView view);
// A filter's words: "Near misses 412".
std::string c_view_words(const CSummary& summary, CView view);

struct CPart {
    std::string_view name;
    std::uint64_t count = 0;
};
// Every function of the scored programs falls in exactly one of these.
std::array<CPart, 5> c_summary_parts(const CSummary& summary);
// "C recompile, measured · 1,820 of 1,903 programs scored · 83 not scored yet · 412,000 functions"
std::string c_summary_words(const CSummary& summary);

}  // namespace cb::app::board
