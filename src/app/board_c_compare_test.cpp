// Tests of board_c_compare.hpp: a function found in the C and in the listing, where each stops, what is said when one is missing, and the page's panel.
#include "app/board_c_compare.hpp"

#include "app/window_board.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

constexpr std::string_view kCompareC =
    "int4 __cdecl sub_001000();\r\n"
    "\r\n"
    "int4 __cdecl sub_001000(void)\r\n"
    "\r\n"
    "{\r\n"
    "\tcaller_sub_001000(1);\r\n"
    "  return sub_001020();\r\n"
    "}\r\n"
    "\r\n"
    "int4 sub_001020(void)\r\n"
    "{\r\n"
    "  return 2;\r\n"
    "}\r\n"
    "\r\n"
    "void Named(int a1)\r\n"
    "\r\n"
    "{\r\n"
    "  return;\r\n"
    "}\r\n";

constexpr std::string_view kCompareAsm =
    "bits 32\r\n"
    "IMAGE equ 0x400000\r\n"
    "section .text\r\n"
    "L401000:\r\n"
    "    push ebp\r\n"
    "L401010:\r\n"
    "    call L401020\r\n"
    "\r\n"
    "L401020:\r\n"
    "    mov eax, 2\r\n"
    "    ret\r\n"
    "section .data\r\n"
    "    db 0\r\n";

CB_TEST("app.board_c_compare.a_function_is_its_signature_to_its_closing_brace", [] {
    const CCodeSlice s = c_function_source(kCompareC, "sub_001000");
    CB_CHECK_EQ(s.lines.size(), std::size_t{6});                       // (not the prototype, nor a longer name that ends the same)
    CB_CHECK_EQ(s.lines[0], "int4 __cdecl sub_001000(void)");
    CB_CHECK_EQ(s.lines[3], "    caller_sub_001000(1);");
    CB_CHECK_EQ(s.lines[5], "}");
    CB_CHECK_EQ(c_function_source(kCompareC, "Named").lines.size(), std::size_t{5});
    CB_CHECK(c_function_source(kCompareC, "sub_0010").lines.empty());
});

CB_TEST("app.board_c_compare.a_numbered_function_has_its_address_label", [] {
    CB_CHECK_EQ(c_listing_base(kCompareAsm), std::uint64_t{0x400000});
    CB_CHECK_EQ(c_listing_base("bits 16\n"), std::uint64_t{0});
    CB_CHECK_EQ(*c_function_label("sub_00BA30", 0x400000), "L40ba30");
    CB_CHECK_EQ(*c_function_label("sub_1_0024", 0), "s1_L24");
    CB_CHECK(!c_function_label("WEP", 0x400000).has_value());
    CB_CHECK(!c_function_label("sub_00BA30", 0).has_value());
    CB_CHECK(!c_function_label("sub_nothex", 0x400000).has_value());
});

CB_TEST("app.board_c_compare.the_assembly_stops_at_the_next_function_or_section", [] {
    const auto ends = c_function_labels(kCompareC, 0x400000);
    CB_CHECK_EQ(ends.size(), std::size_t{2});
    const CCodeSlice first = c_function_assembly(kCompareAsm, "L401000", ends);
    CB_CHECK_EQ(first.lines.size(), std::size_t{4});                   // an inner label does not end it; the blank line before the next is dropped
    CB_CHECK_EQ(first.lines.back(), "    call L401020");
    const CCodeSlice last = c_function_assembly(kCompareAsm, "L401020", ends);
    CB_CHECK_EQ(last.lines.size(), std::size_t{3});
    CB_CHECK_EQ(last.lines.back(), "    ret");
    CB_CHECK(c_function_assembly(kCompareAsm, "L40", ends).lines.empty());
});

struct CCompareFixture {
    Board board;
    Index index;
    std::filesystem::path folder;
    CCompareFixture() {
        folder = std::filesystem::temp_directory_path() / "cb_board_c_compare_test" / "app";
        fs::remove_tree(folder);
        fs::make_dirs(folder);
        fs::write_text(folder / "app.c", kCompareC);
        fs::write_text(folder / "app.exe.asm", kCompareAsm);
        Card c;
        c.rel = "a/APP.EXE";
        c.name = "APP.EXE";
        c.format = "PE";
        c.conv = Conv::Done;
        c.output = fs::to_text(folder);
        board.cards.push_back(c);
        Node n;
        n.sha256 = "bytes";
        index.nodes.push_back(n);
        index.finish();
    }
    ~CCompareFixture() { fs::remove_tree(folder); }
};

CB_TEST("app.board_c_compare.each_side_says_why_it_is_empty", [] {
    CCompareFixture f;
    const CFunctionCompare whole = load_c_compare(f.board, f.index, 0, nullptr, "sub_001020", "90.0% of bytes equal");
    CB_CHECK_EQ(whole.program, "APP.EXE");
    CB_CHECK_EQ(whole.source.size(), std::size_t{4});
    CB_CHECK_EQ(whole.assembly.size(), std::size_t{3});
    CB_CHECK(whole.source_note.empty() && whole.assembly_note.empty());
    CB_CHECK_EQ(whole.source_spans.size(), whole.source.size());
    const CFunctionCompare named = load_c_compare(f.board, f.index, 0, nullptr, "Named", "");
    CB_CHECK(!named.source.empty() && named.assembly.empty());
    CB_CHECK_EQ(named.assembly_note, "The listing labels code by address, and this function is known only by name.");
    CB_CHECK_EQ(load_c_compare(f.board, f.index, 0, nullptr, "sub_009999", "").assembly_note, "The listing has no label at this function's address.");
    f.board.cards[0].conv = Conv::None;
    CB_CHECK_EQ(load_c_compare(f.board, f.index, 0, nullptr, "sub_001020", "").source_note, "This program's conversion is not on disk.");
});

CB_TEST("app.board_c_compare.the_panel_opens_from_a_row_scrolls_and_closes", [] {
    using namespace window;
    CCompareFixture f;
    Model m;
    m.board = std::make_shared<Board>(f.board);
    m.board_index = std::make_shared<const Index>(f.index);
    m.board_scores = std::make_shared<const CScores>(*parse_c_scores(
        "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds\n"
        "a/APP.EXE\tPE\tscored\t2\t0\t1\t1\t0\t0\t9\n"));
    fs::make_dirs(f.folder / "root" / "c_scores");
    m.board->root = f.folder / "root";
    fs::write_text(m.board->root / "c_scores" / c_functions_file_name("a/APP.EXE"), R"({"sub_001020": {"verdict": "same size", "share": 0.9, "size": 5, "original": 5}})");
    board_rebuild_stacks(m);
    board_inspect_card(m, 0);
    int row = -1;
    for (std::size_t i = 0; i < m.board_inspector->rows.size(); ++i)
        if (m.board_inspector->rows[i].kind == InspectorRow::Kind::Function) row = static_cast<int>(i);
    CB_CHECK(row >= 0);

    Layout shell;
    shell.left_card = {0, 0, 1200, 700};
    const BoardLayout before = compute_board_layout(shell, m);
    const InspectorLayout drawer = layout_inspector(before, m);
    const Rect& shown = drawer.rows[static_cast<std::size_t>(row - drawer.first)];
    CB_CHECK(hit_test_board(before, m, shown.x + 4, shown.y + 4).control == Control::BoardInspectRow);

    board_open_compare(m, row);
    CB_CHECK(m.board_compare != nullptr);
    CB_CHECK_EQ(m.board_compare->function, "sub_001020");
    const BoardCompareLayout panel = layout_board_compare(compute_board_layout(shell, m));
    CB_CHECK(panel.source.w > 0 && panel.source.right() <= panel.assembly.x);
    CB_CHECK(hit_test_board(compute_board_layout(shell, m), m, panel.close.x + 2, panel.close.y + 2).control == Control::BoardCompareClose);
    CB_CHECK(hit_test_board(compute_board_layout(shell, m), m, panel.source.x + 2, panel.source.y + 2).control == Control::None);   // nothing under it is clicked
    board_close_compare(m);
    CB_CHECK(m.board_compare == nullptr);
    CB_CHECK(m.board_inspecting);                                      // the drawer it came from is still there
});

}  // namespace
}  // namespace cb::app::board
