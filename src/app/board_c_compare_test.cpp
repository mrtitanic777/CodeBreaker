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

CB_TEST("app.board_c_compare.a_signature_over_several_lines_is_one_definition", [] {
    constexpr std::string_view kLong =
        "int2 __stdcall16far\r\n"
        "LongOne(int2 a1,int2 a2,\r\n"
        "        int2 a3)\r\n"
        "\r\n"
        "{\r\n"
        "  return a1;\r\n"
        "}\r\n"
        "\r\n"
        "void Other(int a1);\r\n"
        "\r\n"
        "sub_1_0040(int2 a1,\r\n"
        "          int2 a2)\r\n"
        "{\r\n"
        "  return;\r\n"
        "}\r\n";
    const CCodeSlice s = c_function_source(kLong, "LongOne");
    CB_CHECK_EQ(s.lines.size(), std::size_t{7});
    CB_CHECK_EQ(s.lines[0], "int2 __stdcall16far");             // its return type comes with it
    CB_CHECK(c_function_source(kLong, "Other").lines.empty());   // a prototype is no definition
    CB_CHECK_EQ(c_function_labels(kLong, 0).count("s1_L40"), std::size_t{1});
});

CB_TEST("app.board_c_compare.the_original_is_read_from_its_own_bytes", [] {
    const std::uint8_t code32[] = {0x8B, 0x44, 0x24, 0x04, 0xC3};
    const CCodeSlice a = c_disassemble(code32, 0x401000, 32);
    CB_CHECK_EQ(a.lines.size(), std::size_t{2});
    CB_CHECK_EQ(a.lines[0], "00401000  mov eax, dword ptr [esp + 4]");
    CB_CHECK_EQ(a.lines[1], "00401004  ret");
    const std::uint8_t code16[] = {0x55, 0x8B, 0xEC};
    const CCodeSlice b = c_disassemble(code16, 0x10, 16);
    CB_CHECK_EQ(b.lines[0], "0010  push bp");
    CB_CHECK_EQ(b.lines[1], "0011  mov bp, sp");
    CB_CHECK_EQ(c_function_end({0x10, 0x40, 0x20}, 0x10, 0x100), std::uint64_t{0x20});
    CB_CHECK_EQ(c_function_end({0x10, 0x40}, 0x40, 0x100), std::uint64_t{0x100});
    CB_CHECK(!c_original_code({}, "PE", "absent", "{}").has_value());
    CB_CHECK(!c_original_code({}, "PE", "half", "{\"half\":").has_value());
    CB_CHECK(!c_original_code({}, "PE", "f", R"({"f": {"verdict": "different", "share": 0.5, "size": 4, "original": 4}})").has_value());   // not a PE
    CB_CHECK(!c_original_code({}, "LE", "f", R"({"f": {"verdict": "different"}})").has_value());
});

CB_TEST("app.board_c_compare.a_byte_diff_marks_what_differs_and_not_what_the_linker_fills", [] {
    const std::uint8_t built[] = {0x8B, 0xC3, 0xE8, 1, 2, 3, 4, 0xC3}, original[] = {0x8B, 0xC1, 0xE8, 9, 9, 9, 9, 0xC3};
    const std::uint8_t mask[] = {0, 0, 0, 1, 1, 1, 1, 0};
    const CByteDiff same = c_byte_diff(built, mask, original, mask);
    CB_CHECK(same.same_length);
    CB_CHECK_EQ(same.compared, std::uint64_t{4});
    CB_CHECK_EQ(same.differing, std::uint64_t{1});
    CB_CHECK_EQ(same.built.lines[0], "0000  8B C3 E8 01 02 03 04 C3");
    CB_CHECK(same.built.differ[0] == (std::vector<std::uint8_t>{1}));
    CB_CHECK(same.original.differ[0] == (std::vector<std::uint8_t>{1}));
    CB_CHECK_EQ(same.built.spans[0].size(), std::size_t{4});   // the linker's bytes are dim
    CB_CHECK_EQ(same.built.spans[0][0].start, std::uint32_t{6 + 3 * 3});

    const std::uint8_t longer[] = {0x55, 0x90, 0x90, 0xC3}, shorter[] = {0x55, 0xC3};
    const std::uint8_t none4[4] = {}, none2[2] = {};
    const CByteDiff lengths = c_byte_diff(longer, none4, shorter, none2);
    CB_CHECK(!lengths.same_length);
    CB_CHECK(lengths.built.differ[0] == (std::vector<std::uint8_t>{1, 2}));   // the start and the end line up, the middle is extra
    CB_CHECK(lengths.original.differ[0].empty());
    CB_CHECK_EQ(lengths.differing, std::uint64_t{2});
    CB_CHECK_EQ(lengths.compared, std::uint64_t{4});

    const std::uint8_t many[33] = {};
    const std::uint8_t many_mask[33] = {};
    CB_CHECK_EQ(c_byte_diff(many, many_mask, many, many_mask).built.lines.size(), std::size_t{3});
});

CB_TEST("app.board_c_compare.a_bytes_file_is_read_and_bad_entries_are_no_diff", [] {
    constexpr std::string_view kBytes = R"({"f": {"built": "8bc3", "built_mask": "0000", "original": "8bc1", "original_mask": "0000"},
        "short": {"built": "8bc3", "built_mask": "00", "original": "8bc1", "original_mask": "0000"},
        "odd": {"built": "8bc", "built_mask": "0000", "original": "8bc1", "original_mask": "0000"}})";
    const std::optional<CByteDiff> f = parse_c_byte_diff(kBytes, "f");
    CB_CHECK(f.has_value());
    CB_CHECK_EQ(f->differing, std::uint64_t{1});
    CB_CHECK(!parse_c_byte_diff(kBytes, "short").has_value());
    CB_CHECK(!parse_c_byte_diff(kBytes, "odd").has_value());
    CB_CHECK(!parse_c_byte_diff(kBytes, "absent").has_value());
    CB_CHECK(!parse_c_byte_diff("{\"half", "f").has_value());
    CB_CHECK_EQ(c_bytes_file_name("abc"), "ba7816bf8f01cfea.bytes.json");
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
