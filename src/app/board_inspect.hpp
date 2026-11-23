// app/board_inspect.hpp -- what one card is connected to: the rows of the X-Ray inspector (facts, copies, inbound and outbound links, signals) and the cards a hover
// lights up on the board. Links are counted across every copy of the file, because a name resolves to the nearest copy, and identical source files count once.
// No operating-system API (board_inspect_test.cpp).
#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "app/board.hpp"
#include "app/board_index.hpp"
#include "app/board_scores.hpp"
#include "app/board_signals.hpp"

namespace cb::app::board {

struct InspectorRow {
    enum class Kind : std::uint8_t { Heading, Fact, File, Signal };
    Kind kind = Kind::Fact;
    std::string text, detail;
    std::int64_t card = -1;            // File: the card a click goes to
    std::int64_t signal = -1;          // Signal: the signal a click filters by
};

struct Inspector {
    std::uint32_t card = 0;
    std::uint32_t copies = 1, inbound = 0, outbound = 0, signals = 0;   // distinct files, and distinct (signal, direction)
    std::vector<InspectorRow> rows;
};
Inspector build_inspector(const Board& board, const Index& index, const Signals* signals, std::uint32_t card, const CScores* scores = nullptr);

// What a hovered card is linked to. role per card: bit 0 it uses that card, bit 1 that card uses it. Copies of a linked file are all lit.
struct HoverLinks {
    std::uint32_t card = 0;
    std::uint32_t uses = 0, used_by = 0;   // distinct files
    std::vector<std::uint8_t> role;
};
HoverLinks hover_links(const Index& index, std::uint32_t card);

}  // namespace cb::app::board
