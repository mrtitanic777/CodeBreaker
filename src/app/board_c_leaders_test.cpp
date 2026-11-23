// Tests of board_c_leaders.hpp: the order across programs, what is left out, a snapshot's count, and the page's list and stepping.
#include "app/board_c_leaders.hpp"

#include "app/window_board.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kLeaderTable =
    "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
    "a/one.exe\tPE\tscored\t3\t0\t2\t1\t0\t0\t9\n"
    "b/two.dll\tPE\tscored\t2\t0\t1\t1\t0\t0\t9\n"
    "c/slow.drv\tNE\ttimeout\t0\t0\t0\t0\t0\t0\t3600\n";

constexpr std::string_view kLeaderOne = R"({
  "close":  {"verdict": "same size", "share": 0.95, "size": 40, "original": 40},
  "tiny":   {"verdict": "same size", "share": 0.99, "size": 8, "original": 8},
  "longer": {"verdict": "different", "share": 0.99, "size": 90, "original": 100}
})";
constexpr std::string_view kLeaderTwo = R"({
  "best":  {"verdict": "same size", "share": 0.98, "size": 64, "original": 64},
  "other": {"verdict": "different", "share": 0.40, "size": 20, "original": 30}
})";

struct CLeaderFixture {
    std::filesystem::path root;
    CScores scores;
    CLeaderFixture() {
        root = std::filesystem::temp_directory_path() / "cb_board_c_leaders_test" / "board";
        fs::remove_tree(root);
        fs::make_dirs(root / "c_scores");
        fs::write_text(root / "c_scores" / c_functions_file_name("a/one.exe"), kLeaderOne);
        fs::write_text(root / "c_scores" / c_functions_file_name("b/two.dll"), kLeaderTwo);
        scores = *parse_c_scores(kLeaderTable);
    }
    ~CLeaderFixture() { fs::remove_tree(root); }
};

CB_TEST("app.board_c_leaders.the_closest_across_programs_come_first", [] {
    const CLeaderFixture f;
    const std::atomic<bool> go{false};
    const CLeaders l = *build_c_leaders(f.root, f.scores, go);
    CB_CHECK_EQ(l.programs, std::uint64_t{2});
    CB_CHECK_EQ(l.scored, std::uint64_t{2});
    CB_CHECK_EQ(l.candidates, std::uint64_t{4});               // not the 8-byte one
    CB_CHECK_EQ(l.top.size(), std::size_t{4});
    CB_CHECK_EQ(l.top[0].function, "best");
    CB_CHECK_EQ(l.top[0].program, "two.dll");
    CB_CHECK_EQ(l.top[1].function, "close");
    CB_CHECK_EQ(l.top[2].function, "longer");                  // 99% similar is below 95% equal: another length
    const std::atomic<bool> stop{true};
    CB_CHECK(!build_c_leaders(f.root, f.scores, stop).has_value());
});

CB_TEST("app.board_c_leaders.only_the_best_are_held", [] {
    CLeaders l;
    CNearMisses misses;
    for (int i = 0; i < 1000; ++i) misses.closest.push_back({"f" + std::to_string(i), "w", static_cast<double>(i) / 1000.0, true, 32});
    add_c_leaders(l, "a/many.exe", misses);
    CB_CHECK_EQ(l.candidates, std::uint64_t{1000});
    CB_CHECK(l.top.size() <= kCLeadersKept);
    CB_CHECK_EQ(l.top[0].function, "f999");
});

CB_TEST("app.board_c_leaders.the_list_opens_a_function_and_the_panel_steps_through", [] {
    using namespace window;
    const CLeaderFixture f;
    Board board;
    board.root = f.root;
    Index index;
    for (const char* rel : {"a/one.exe", "b/two.dll"}) {
        Card c;
        c.rel = rel;
        c.name = c.rel.substr(2);
        c.format = "PE";
        board.cards.push_back(c);
        Node n;
        n.group = static_cast<std::uint32_t>(index.nodes.size());
        n.sha256 = "hash" + std::to_string(n.group);
        index.nodes.push_back(n);
    }
    index.finish();
    Model m;
    m.board = std::make_shared<Board>(board);
    m.board_index = std::make_shared<const Index>(index);
    m.board_scores = std::make_shared<const CScores>(f.scores);
    board_rebuild_stacks(m);
    CB_CHECK(board_toggle_leaders(m));                         // nothing read yet: it asks for a scan
    const std::atomic<bool> go{false};
    m.board_leaders = std::make_shared<const CLeaders>(*build_c_leaders(f.root, f.scores, go));
    board_close_leaders(m);
    CB_CHECK(!board_toggle_leaders(m));                        // up to date: no second scan
    CB_CHECK(m.board_leaders_open);

    Layout shell;
    shell.left_card = {0, 0, 1200, 700};
    const BoardLayout b = compute_board_layout(shell, m);
    CB_CHECK(b.c_leaders.w > 0);
    const BoardLeadersLayout list = layout_board_leaders(b, m);
    CB_CHECK_EQ(list.rows.size(), std::size_t{4});
    const PageHit row = hit_test_board(b, m, list.rows[1].x + 4, list.rows[1].y + 4);
    CB_CHECK(row.control == Control::BoardLeaderRow);
    CB_CHECK_EQ(row.index, 1);

    board_open_leader(m, 1);
    CB_CHECK_EQ(m.board_compare->function, "close");
    CB_CHECK_EQ(m.board_compare->program, "one.exe");
    CB_CHECK_EQ(m.board_compare_list.size(), std::size_t{4});
    board_compare_step(m, 1);
    CB_CHECK_EQ(m.board_compare->function, "longer");
    board_compare_step(m, -2);
    CB_CHECK_EQ(m.board_compare->function, "best");
    board_compare_step(m, -1);                                 // the first stays the first
    CB_CHECK_EQ(m.board_compare_at, 0);
    const BoardCompareLayout panel = layout_board_compare(b);
    CB_CHECK(hit_test_board(b, m, panel.next.x + 2, panel.next.y + 2).control == Control::BoardCompareNext);
    CB_CHECK(hit_test_board(b, m, panel.prev.x + 2, panel.prev.y + 2).control == Control::BoardComparePrev);
    board_close_compare(m);
    CB_CHECK(m.board_compare_list.empty());
    CB_CHECK(m.board_leaders_open);                            // back to the list it came from
    board_close_leaders(m);
    CB_CHECK(!m.board_leaders_open);
});

}  // namespace
}  // namespace cb::app::board
