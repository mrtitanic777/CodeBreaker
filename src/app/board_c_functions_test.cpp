// Tests of board_c_functions.hpp: what counts as a near miss, the order across measures, the words, the file's name, and the inspector's section.
#include "app/board_c_functions.hpp"

#include "app/board_inspect.hpp"
#include "app/window_board.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kCFunctions = R"({
  "same":  {"verdict": "same size", "share": 0.9, "size": 100, "original": 100, "flags": "/O2"},
  "almost": {"verdict": "same size", "share": 0.9996, "size": 2500, "original": 2500, "flags": "/O2"},
  "longer": {"verdict": "different", "share": 0.95, "size": 90, "original": 100, "flags": "/O2"},
  "seg16": {"verdict": "same size", "differing": 1, "total": 50, "segment": 1, "flags": "msvc152"},
  "whole": {"verdict": "identical", "share": 1.0, "size": 33, "original": 33, "flags": "/O2"},
  "crt":   {"verdict": "identified", "flags": "/O2"},
  "bad":   {"verdict": "no-compile", "share": 0.0, "size": 0, "original": 9, "flags": "/O2"},
  "none":  {"verdict": "different", "share": 0.0, "size": 5, "original": 9, "flags": "/O2"},
  "all16": {"verdict": "different", "differing": 40, "total": 40, "segment": 2, "flags": "msvc152"}
})";

CB_TEST("app.board_c_functions.the_right_length_ranks_first_and_each_names_its_measure", [] {
    const std::optional<CNearMisses> n = parse_c_near_misses(kCFunctions);
    CB_CHECK(n.has_value());
    CB_CHECK_EQ(n->candidates, std::uint64_t{4});
    CB_CHECK_EQ(n->closest[0].name, "almost");
    CB_CHECK_EQ(n->closest[0].words, "99.9% of bytes equal");   // (rounded down: it is not identical)
    CB_CHECK_EQ(n->closest[1].name, "seg16");
    CB_CHECK_EQ(n->closest[1].words, "1 of 50 units differ");
    CB_CHECK_EQ(n->closest[2].name, "same");
    CB_CHECK_EQ(n->closest[3].name, "longer");                  // 95% similar is still below 90% equal: another length
    CB_CHECK_EQ(n->closest[3].words, "95.0% similar, 90 vs 100 B");
});

CB_TEST("app.board_c_functions.a_file_being_written_is_no_result", [] {
    CB_CHECK(!parse_c_near_misses("{\n  \"half\": {\"verdict\": \"diff").has_value());
    CB_CHECK(!parse_c_near_misses("[1, 2]").has_value());
    CB_CHECK(parse_c_near_misses("{}")->closest.empty());
    CB_CHECK_EQ(c_functions_file_name("abc"), "ba7816bf8f01cfea.json");
});

CB_TEST("app.board_c_functions.only_the_closest_are_kept_and_all_are_counted", [] {
    std::string text = "{";
    for (int i = 0; i < 30; ++i) text += std::string(i ? "," : "") + "\"f" + std::to_string(i) + "\":{\"verdict\":\"same size\",\"share\":0." + std::to_string(10 + i) + ",\"size\":10,\"original\":10}";
    const CNearMisses n = *parse_c_near_misses(text + "}");
    CB_CHECK_EQ(n.candidates, std::uint64_t{30});
    CB_CHECK_EQ(n.closest.size(), kCNearMissesShown);
    CB_CHECK_EQ(n.closest[0].name, "f29");
});

CB_TEST("app.board_c_functions.the_inspector_lists_them_under_the_score_and_a_copy_answers", [] {
    Board board;
    Index index;
    for (const char* rel : {"a/app.exe", "b/app.exe"}) {
        Card c;
        c.rel = rel;
        c.name = "app.exe";
        c.format = "PE";
        board.cards.push_back(c);
        Node n;
        n.group = 0;
        n.sha256 = "same bytes";
        index.nodes.push_back(n);
    }
    index.finish();
    const CScores scores = *parse_c_scores(
        "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
        "a/app.exe\tPE\tscored\t9\t1\t3\t3\t1\t1\t9\n");
    CB_CHECK_EQ(c_scored_rel(board, index, 1, scores), "a/app.exe");
    const CNearMisses near = *parse_c_near_misses(kCFunctions);
    const Inspector shown = build_inspector(board, index, nullptr, 1, &scores, &near);
    std::size_t heading = shown.rows.size(), inbound = shown.rows.size();
    for (std::size_t i = 0; i < shown.rows.size(); ++i) {
        if (shown.rows[i].text == "Closest to identical (4)") heading = i;
        if (shown.rows[i].text.starts_with("Inbound")) inbound = i;
    }
    CB_CHECK(heading < inbound);
    CB_CHECK_EQ(shown.rows[heading].detail, "measured");
    CB_CHECK_EQ(shown.rows[heading + 1].text, "almost");
    CB_CHECK_EQ(shown.rows[heading + 1].detail, "99.9% of bytes equal");
    for (const InspectorRow& r : build_inspector(board, index, nullptr, 1, &scores).rows) CB_CHECK(!r.text.starts_with("Closest"));
});

CB_TEST("app.board_c_functions.a_c_filter_lays_out_only_the_columns_it_found", [] {
    using namespace window;
    Board board;
    Index index;
    const auto add = [&](const char* rel, Pile pile) {
        Card c;
        c.rel = rel;
        c.name = c.rel.substr(2);
        c.format = "PE";
        c.conv = Conv::Done;
        c.pile = pile;
        board.cards.push_back(c);
        Node n;
        n.group = static_cast<std::uint32_t>(index.nodes.size());
        n.sha256 = "hash" + std::to_string(n.group);
        index.nodes.push_back(n);
    };
    add("a/app.exe", Pile::Programs);
    add("a/drv.sys", Pile::Drivers);
    add("a/lib.dll", Pile::Libraries);
    index.finish();
    Model m;
    m.board = std::make_shared<Board>(board);
    m.board_index = std::make_shared<const Index>(index);
    m.board_scores = std::make_shared<const CScores>(*parse_c_scores(
        "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
        "a/app.exe\tPE\tscored\t10\t1\t2\t7\t0\t0\t9\n"
        "a/drv.sys\tPE\tscored\t10\t1\t2\t7\t0\t0\t9\n"
        "a/lib.dll\tPE\tscored\t10\t1\t0\t9\t0\t0\t9\n"));
    board_rebuild_stacks(m);
    Layout shell;
    shell.left_card = {0, 0, 1200, 700};
    CB_CHECK_EQ(board_columns_shown(m), kPileCount);
    board_toggle_c_view(m, CView::NearMisses);                 // the libraries have none
    CB_CHECK_EQ(board_columns_shown(m), std::size_t{2});
    const BoardLayout b = compute_board_layout(shell, m);
    CB_CHECK(b.column_width == kBoardColumnWide);
    const auto drivers = static_cast<std::size_t>(Pile::Drivers), libraries = static_cast<std::size_t>(Pile::Libraries);
    const BoardColumn second = compute_board_column(b, m, drivers);
    CB_CHECK(second.foundation.x == b.tableau.x + kBoardColumnWide + kBoardColumnGap);   // directly after Programs
    CB_CHECK(compute_board_column(b, m, libraries).foundation.w == 0);
    const PageHit hit = hit_test_board(b, m, second.foundation.x + 5, second.foundation.y + 5);
    CB_CHECK(hit.control == Control::BoardFoundation);
    CB_CHECK_EQ(hit.index, static_cast<int>(drivers));
    const std::vector<PageHit> order = focus_order_board(m);
    CB_CHECK(std::find(order.begin(), order.end(), PageHit{Control::BoardFoundation, static_cast<int>(libraries)}) == order.end());
    board_toggle_c_view(m, CView::NearMisses);
    CB_CHECK_EQ(board_columns_shown(m), kPileCount);
});

}  // namespace
}  // namespace cb::app::board
