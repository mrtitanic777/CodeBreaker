// Tests of board_c_history.hpp: what counts as a change, the order, the words, snapshot names, and the page's panel.
#include "app/board_c_history.hpp"

#include "app/window_board.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kHistoryHeader = "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n";

CScores c_history_table(std::string_view rows) { return *parse_c_scores(std::string(kHistoryHeader) + std::string(rows)); }

CB_TEST("app.board_c_history.better_worse_and_newly_scored", [] {
    const CScores before = c_history_table(
        "a/up.exe\tPE\tscored\t10\t2\t0\t8\t0\t0\t1\n"
        "a/down.dll\tPE\tscored\t10\t5\t0\t5\t0\t0\t1\n"
        "a/same.dll\tPE\tscored\t10\t3\t0\t7\t0\t0\t1\n"
        "a/big.exe\tPE\tscored\t100\t10\t0\t90\t0\t0\t1\n"
        "a/slow.drv\tNE\ttimeout\t0\t0\t0\t0\t0\t0\t3600\n");
    const CScores now = c_history_table(
        "a/up.exe\tPE\tscored\t10\t4\t0\t6\t0\t0\t1\n"
        "a/down.dll\tPE\tscored\t10\t4\t0\t6\t0\t0\t1\n"
        "a/same.dll\tPE\tscored\t10\t3\t0\t7\t0\t0\t1\n"
        "a/big.exe\tPE\tscored\t100\t40\t0\t60\t0\t0\t1\n"
        "a/slow.drv\tNE\tscored\t20\t1\t0\t19\t0\t0\t300\n"
        "b/new.exe\tPE\tscored\t5\t5\t0\t0\t0\t0\t1\n");
    const CHistory h = diff_c_scores(before, now, "20261124-034012.tsv");
    CB_CHECK_EQ(h.better, std::uint64_t{2});
    CB_CHECK_EQ(h.worse, std::uint64_t{1});
    CB_CHECK_EQ(h.newly_scored, std::uint64_t{2});               // the timed-out one counts as newly scored
    CB_CHECK_EQ(h.changes.size(), std::size_t{5});               // (same.dll did not change)
    CB_CHECK_EQ(h.changes[0].program, "big.exe");                // the biggest change first
    CB_CHECK_EQ(h.changes[0].delta(), std::int64_t{30});
    CB_CHECK_EQ(h.changes[1].program, "up.exe");                 // +2 before -1
    CB_CHECK_EQ(h.changes[2].program, "down.dll");
    CB_CHECK(!h.changes[3].scored_before);
    CB_CHECK_EQ(h.changes[3].program, "new.exe");                // then the newly scored, by name
    CB_CHECK_EQ(c_history_words(h), "+37 byte-identical functions \xC2\xB7 2 programs better \xC2\xB7 1 program worse \xC2\xB7 2 newly scored");
    CB_CHECK_EQ(c_history_words(diff_c_scores(before, before, "x")), "No change");
});

CB_TEST("app.board_c_history.snapshot_names_say_when", [] {
    using namespace std::chrono;
    const local_seconds when = local_days{year{2026} / November / 24} + hours{15} + minutes{4} + seconds{12};
    CB_CHECK_EQ(c_snapshot_name(when), "20261124-150412.tsv");
    CB_CHECK_EQ(c_snapshot_words("20261124-150412.tsv"), "Nov 24, 2026 at 3:04 PM");
    CB_CHECK_EQ(c_snapshot_words("20261124-000500.tsv"), "Nov 24, 2026 at 12:05 AM");
    CB_CHECK_EQ(c_snapshot_words("notes.tsv"), "notes.tsv");
});

CB_TEST("app.board_c_history.the_newest_snapshot_is_compared_and_the_panel_shows_it", [] {
    using namespace window;
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "cb_board_c_history_test" / "board";
    fs::remove_tree(root);
    fs::make_dirs(root);
    CB_CHECK(!load_latest_c_snapshot(root).has_value());
    save_c_snapshot(root, "20261101-090000.tsv", std::string(kHistoryHeader) + "a/up.exe\tPE\tscored\t10\t1\t0\t9\t0\t0\t1\n");
    save_c_snapshot(root, "20261120-090000.tsv", std::string(kHistoryHeader) + "a/up.exe\tPE\tscored\t10\t2\t0\t8\t0\t0\t1\n");
    const auto latest = load_latest_c_snapshot(root);
    CB_CHECK(latest.has_value());
    CB_CHECK_EQ(latest->first, "20261120-090000.tsv");

    Board board;
    board.root = root;
    Model m;
    m.board = std::make_shared<Board>(board);
    m.board_scores = std::make_shared<const CScores>(c_history_table("a/up.exe\tPE\tscored\t10\t6\t0\t4\t0\t0\t1\n"));
    board_toggle_history(m);
    CB_CHECK(m.board_history_open);
    CB_CHECK(m.board_history != nullptr);
    CB_CHECK_EQ(m.board_history->snapshot, "20261120-090000.tsv");
    CB_CHECK_EQ(m.board_history->changes[0].delta(), std::int64_t{4});
    Layout shell;
    shell.left_card = {0, 0, 1200, 700};
    const BoardLayout b = compute_board_layout(shell, m);
    CB_CHECK(b.c_history.w > 0 && b.c_history.right() <= b.c_leaders.x);
    const BoardHistoryLayout panel = layout_board_history(b, m);
    CB_CHECK_EQ(panel.rows.size(), std::size_t{1});
    // the chart: each snapshot's byte-identical functions, oldest first, then now's; the list starts under it
    CB_CHECK_EQ(m.board_trend.size(), std::size_t{3});
    CB_CHECK(m.board_trend[0].identical == 1 && m.board_trend[1].identical == 2 && m.board_trend[2].identical == 6 && m.board_trend[2].name == "now");
    CB_CHECK(panel.chart.h > 0 && panel.chart.bottom() <= panel.head.y && panel.head.bottom() <= panel.list.y);
    CB_CHECK(hit_test_board(b, m, panel.snapshot.x + 2, panel.snapshot.y + 2).control == Control::BoardHistorySnapshot);
    CB_CHECK(hit_test_board(b, m, panel.close.x + 2, panel.close.y + 2).control == Control::BoardHistoryClose);
    board_toggle_leaders(m);                                    // one list at a time
    CB_CHECK(!m.board_history_open);
    fs::remove_tree(root);
});

}  // namespace
}  // namespace cb::app::board
