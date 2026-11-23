// Tests of board_c_view.hpp: marks, copies counted once, the filters and their order, the whole-OS sums, and the page's row of filters.
#include "app/board_c_view.hpp"

#include "app/window_board.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kCViewTable =
    "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
    "a/best.exe\tPE\tscored\t10\t5\t1\t2\t1\t1\t9\n"
    "a/mid.exe\tPE\tscored\t100\t10\t40\t30\t20\t0\t9\n"
    "a/zero.exe\tPE\tscored\t4\t0\t0\t4\t0\t0\t9\n"
    "a/empty.exe\tPE\tscored\t0\t0\t0\t0\t0\t0\t9\n"
    "a/slow.drv\tNE\ttimeout\t0\t0\t0\t0\t0\t0\t3600\n"
    "a/broke.dll\tNE\tfailed\t0\t0\t0\t0\t0\t0\t7\n"
    "a/twin1.dll\tPE\tscored\t10\t1\t0\t9\t0\t0\t9\n"
    "a/partial.dll\tPE\tsco";   // (a row still being written)

struct CViewFixture {
    Board board;
    Index index;
    CScores scores;
    CViewFixture() {
        const auto add = [&](const char* name, const char* format, Conv conv, int group = -1) {
            Card c;
            c.rel = std::string("a/") + name;
            c.name = name;
            c.format = format;
            c.conv = conv;
            c.pile = Pile::Programs;
            board.cards.push_back(c);
            Node n;
            n.group = group >= 0 ? static_cast<std::uint32_t>(group) : static_cast<std::uint32_t>(index.nodes.size());
            n.sha256 = "hash" + std::to_string(n.group);
            index.nodes.push_back(n);
        };
        add("best.exe", "PE", Conv::Done);        // 0
        add("mid.exe", "PE", Conv::Done);         // 1
        add("zero.exe", "PE", Conv::Done);        // 2
        add("empty.exe", "PE", Conv::Done);       // 3
        add("slow.drv", "NE", Conv::Done);        // 4
        add("broke.dll", "NE", Conv::Done);       // 5
        add("twin1.dll", "PE", Conv::Done);       // 6
        add("twin2.dll", "PE", Conv::Done, 6);    // 7: the same bytes as 6
        add("new.exe", "PE", Conv::Done);         // 8: converted, no row
        add("todo.exe", "PE", Conv::None);        // 9: not converted, no row
        add("note.txt", "text", Conv::Done);      // 10: not a program
        index.finish();
        scores = *parse_c_scores(kCViewTable);
    }
};

CB_TEST("app.board_c_view.an_unscored_program_is_never_a_number", [] {
    const CViewFixture f;
    const CMarks marks = build_c_marks(f.board, &f.index, f.scores);
    CB_CHECK_EQ(c_mark_words(marks.by_card[0]), "50.0%");
    CB_CHECK_EQ(c_mark_words(marks.by_card[2]), "0.0%");       // measured, and really none
    CB_CHECK_EQ(c_mark_words(marks.by_card[3]), "no code");
    CB_CHECK_EQ(c_mark_words(marks.by_card[4]), "timed out");
    CB_CHECK_EQ(c_mark_words(marks.by_card[5]), "no score");
    CB_CHECK_EQ(c_mark_words(marks.by_card[8]), "not yet");
    CB_CHECK_EQ(c_mark_words(marks.by_card[9]), "");           // not converted: nothing to score
    CB_CHECK_EQ(c_mark_words(marks.by_card[10]), "");          // not a program
    CB_CHECK(c_identical_share(marks.by_card[4]) == 0.0f);
    CB_CHECK(!in_c_view(marks.by_card[4], CView::WillNotCompile));
    CB_CHECK(!in_c_view(marks.by_card[8], CView::MostIdentical));
});

CB_TEST("app.board_c_view.copies_share_a_score_and_count_once", [] {
    const CViewFixture f;
    const CMarks marks = build_c_marks(f.board, &f.index, f.scores);
    CB_CHECK_EQ(c_mark_words(marks.by_card[7]), "10.0%");      // the copy with no row of its own
    const CSummary& s = marks.summary;
    CB_CHECK_EQ(s.scored, std::uint64_t{5});                   // best, mid, zero, empty, the twins as one
    CB_CHECK_EQ(s.not_scored, std::uint64_t{1});
    CB_CHECK_EQ(s.out_of_time, std::uint64_t{1});
    CB_CHECK_EQ(s.scorer_failed, std::uint64_t{1});
    CB_CHECK_EQ(s.programs(), std::uint64_t{8});
    CB_CHECK_EQ(s.functions, std::uint64_t{124});
    CB_CHECK_EQ(s.identical, std::uint64_t{16});
    std::uint64_t parts = 0;
    for (const CPart& p : c_summary_parts(s)) parts += p.count;
    CB_CHECK_EQ(parts, s.functions);
    CB_CHECK_EQ(c_summary_words(s), "C recompile, measured \xC2\xB7 5 of 8 programs scored \xC2\xB7 1 not scored yet \xC2\xB7 1 out of time \xC2\xB7 1 scorer failed \xC2\xB7 124 functions");
    CB_CHECK_EQ(c_view_words(s, CView::NotScored), "Not scored 3");
});

CB_TEST("app.board_c_view.each_view_narrows_and_orders_the_columns", [] {
    const CViewFixture f;
    const CMarks marks = build_c_marks(f.board, &f.index, f.scores);
    std::vector<std::uint32_t> group(f.board.cards.size());
    for (std::size_t i = 0; i < group.size(); ++i) group[i] = f.index.nodes[i].group;
    StackOptions options;
    options.group = &group;
    const auto done_in = [&](CView view) {
        Stacks stacks = build_stacks(f.board, {}, options);
        apply_c_view(stacks, marks, view);
        return stacks[static_cast<std::size_t>(Pile::Programs)].done;
    };
    CB_CHECK(done_in(CView::MostIdentical) == (std::vector<std::uint32_t>{0, 1, 6}));   // 50%, then 10% with more of them, then 10%
    CB_CHECK(done_in(CView::NearMisses) == (std::vector<std::uint32_t>{1, 0}));         // 40%, 10%
    CB_CHECK(done_in(CView::WillNotCompile) == (std::vector<std::uint32_t>{1, 0}));     // 20, 1
    CB_CHECK(done_in(CView::NotScored) == (std::vector<std::uint32_t>{5, 8, 4}));       // broke, new, slow: by name
    CB_CHECK_EQ(done_in(CView::All).size(), std::size_t{9});
    CB_CHECK_EQ(marks.summary.in_view[0], std::uint64_t{3});
    CB_CHECK_EQ(c_view_order(CView::NearMisses), "by same size");
});

CB_TEST("app.board_c_view.the_page_filters_and_gives_the_column_back", [] {
    using namespace window;
    CViewFixture f;
    Model m;
    m.board = std::make_shared<Board>(f.board);
    m.board_index = std::make_shared<const Index>(f.index);
    m.board_scores = std::make_shared<const CScores>(f.scores);
    board_rebuild_stacks(m);
    const auto programs = static_cast<std::size_t>(Pile::Programs);
    CB_CHECK(!m.board_show_done[programs]);
    board_toggle_c_view(m, CView::NearMisses);
    CB_CHECK(m.board_c_view == CView::NearMisses);
    CB_CHECK(m.board_show_done[programs]);                     // what it found is on the foundation
    CB_CHECK_EQ(m.board_stacks[programs].done.size(), std::size_t{2});
    board_toggle_c_view(m, CView::WillNotCompile);             // from one view to another keeps what was there first
    board_toggle_c_view(m, CView::WillNotCompile);
    CB_CHECK(m.board_c_view == CView::All);
    CB_CHECK(!m.board_show_done[programs]);
    CB_CHECK_EQ(m.board_stacks[programs].done.size(), std::size_t{9});

    Layout shell;
    shell.left_card = {0, 0, 1100, 640};
    const BoardLayout b = compute_board_layout(shell, m);
    CB_CHECK(b.c_summary.h > 0);
    const Rect& pill = b.c_pills[2];
    const PageHit hit = hit_test_board(b, m, pill.x + 2, pill.y + 2);
    CB_CHECK(hit.control == Control::BoardCView);
    CB_CHECK_EQ(hit.index, 3);
    CB_CHECK(b.c_pills.back().right() <= b.orphans_button.x);
    m.board_scores.reset();
    CB_CHECK(compute_board_layout(shell, m).c_summary.h == 0);  // no table: no row claiming anything
});

CB_TEST("app.board_c_view.a_column_and_a_strip_say_what_was_measured", [] {
    const CViewFixture f;
    const CMarks marks = build_c_marks(f.board, &f.index, f.scores);
    const CPileScore& programs = marks.by_pile[static_cast<std::size_t>(Pile::Programs)];
    CB_CHECK_EQ(programs.functions, std::uint64_t{124});
    CB_CHECK_EQ(programs.scored, std::uint64_t{5});
    CB_CHECK_EQ(programs.unscored, std::uint64_t{3});
    CB_CHECK_EQ(c_pile_words(programs), "12.9%");
    CB_CHECK_EQ(c_pile_words(CPileScore{0, 0, 0, 4}), "not yet");     // never 0%
    CB_CHECK_EQ(c_pile_words(CPileScore{0, 0, 2, 0}), "no code");
    CB_CHECK_EQ(c_pile_words(CPileScore{}), "");
    CB_CHECK_EQ(c_detail_words(marks.by_card[1]), "C 10 of 100 byte-identical (10.0%)");
    CB_CHECK_EQ(c_detail_words(marks.by_card[4]), "C ran out of time: no score");
    CB_CHECK_EQ(c_detail_words(marks.by_card[8]), "C not scored yet");
    CB_CHECK_EQ(c_detail_words(marks.by_card[9]), "");
    for (const CView view : {CView::All, CView::MostIdentical, CView::NearMisses, CView::WillNotCompile, CView::NotScored}) CB_CHECK(c_view_from_key(c_view_key(view)) == view);
    CB_CHECK(c_view_from_key("from a newer version") == CView::All);
});

CB_TEST("app.board_c_view.new_rows_reach_the_page_and_a_bad_read_changes_nothing", [] {
    using namespace window;
    const std::filesystem::path root = std::filesystem::temp_directory_path() / "cb_board_c_view_test" / "live";
    fs::remove_tree(root);
    fs::make_dirs(root);
    const std::string header = "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n";
    fs::write_text(root / "c_scores.tsv", header + "a/best.exe\tPE\tscored\t10\t5\t1\t2\t1\t1\t9\n");
    CViewFixture f;
    f.board.root = root;
    Model m;
    m.board = std::make_shared<Board>(f.board);
    m.board_index = std::make_shared<const Index>(f.index);
    board_load_decks(m);
    board_rebuild_stacks(m);
    m.status_text = "Files \xC2\xB7 " + c_score_words(*m.board_scores);
    CB_CHECK(!board_refresh_scores(m));                                // nothing new
    CB_CHECK_EQ(m.board_c_marks->summary.scored, std::uint64_t{1});

    fs::write_text(root / "c_scores.tsv", header + "a/best.exe\tPE\tscored\t10\t5\t1\t2\t1\t1\t9\na/mid.exe\tPE\tscored\t100\t10\t40\t30\t20\t0\t9\na/new.exe\tPE\tsc");
    m.board_batch_armed = 3;
    CB_CHECK(!board_refresh_scores(m));                                // a batch waiting for its second press is left alone
    m.board_batch_armed = 0;
    CB_CHECK(board_refresh_scores(m));
    CB_CHECK_EQ(m.board_c_marks->summary.scored, std::uint64_t{2});    // (the row still being written is not counted)
    CB_CHECK_EQ(c_mark_words(m.board_c_marks->by_card[8]), "not yet");
    CB_CHECK_EQ(m.status_text, "Files \xC2\xB7 " + c_score_words(*m.board_scores));

    fs::write_text(root / "c_scores.tsv", "half a heade");
    CB_CHECK(!board_refresh_scores(m));
    CB_CHECK_EQ(m.board_c_marks->summary.scored, std::uint64_t{2});
    fs::remove_tree(root);
});

}  // namespace
}  // namespace cb::app::board
