// Tests of board_scores.hpp: the table, what counts toward the totals, and the inspector's section.
#include "app/board_scores.hpp"

#include "app/board_inspect.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kScoreTable =
    "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
    "a/app.exe\tPE\tscored\t10\t1\t2\t3\t4\t0\t9\n"
    "b/lib.dll\tNE\tscored\t10\t3\t1\t2\t1\t3\t8\n"
    "c/big.drv\tNE\ttimeout\t0\t0\t0\t0\t0\t0\t3600\n";

CB_TEST("app.board_scores.totals_leave_out_programs_without_a_score", [] {
    const std::optional<CScores> s = parse_c_scores(kScoreTable);
    CB_CHECK(s.has_value());
    CB_CHECK_EQ(s->scored, std::uint64_t{2});
    CB_CHECK_EQ(s->unscored, std::uint64_t{1});
    CB_CHECK_EQ(s->total.functions, std::uint64_t{20});
    CB_CHECK_EQ(s->total.identical, std::uint64_t{4});
    CB_CHECK_EQ(s->total.library, std::uint64_t{3});
    CB_CHECK(!s->by_card.at("c/big.drv").scored());
    CB_CHECK_EQ(score_percent(4, 20), "20.0%");
    CB_CHECK_EQ(score_percent(1, 0), "0.0%");
    CB_CHECK(!parse_c_scores("not the table\n").has_value());
});

CB_TEST("app.board_scores.the_inspector_shows_a_cards_score_or_says_it_has_none", [] {
    Board board;
    Index index;
    for (const char* rel : {"a/app.exe", "c/big.drv", "d/new.exe", "e/note.txt"}) {
        Card c;
        c.rel = rel;
        c.name = c.rel.substr(2);
        c.format = c.rel.ends_with(".txt") ? "text" : c.rel.ends_with(".drv") ? "NE" : "PE";
        board.cards.push_back(c);
        Node n;
        n.group = static_cast<std::uint32_t>(index.nodes.size());
        n.sha256 = "hash" + std::to_string(n.group);
        index.nodes.push_back(n);
    }
    index.finish();
    const CScores scores = *parse_c_scores(kScoreTable);
    const auto value = [&](std::uint32_t card, std::string_view name) -> std::string {
        for (const InspectorRow& r : build_inspector(board, index, nullptr, card, &scores).rows)
            if (r.kind == InspectorRow::Kind::Fact && r.text == name) return r.detail;
        return "";
    };
    CB_CHECK_EQ(value(0, "Byte-identical"), "1 of 10 (10.0%)");
    CB_CHECK_EQ(value(1, "C result"), "ran out of time: no score");
    CB_CHECK_EQ(value(2, "C result"), "not scored yet");
    CB_CHECK_EQ(value(3, "C result"), "");   // (not a program)
});

}  // namespace
}  // namespace cb::app::board
