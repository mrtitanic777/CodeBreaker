// Tests of board.hpp: what a file is, the kept board, and that a card is Done only when a conversion really left files.
#include "app/board.hpp"

#include <atomic>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

namespace stdfs = std::filesystem;

stdfs::path bt_folder(const char* name) {
    const stdfs::path p = stdfs::temp_directory_path() / "cb_board_test" / name;
    fs::remove_tree(p);
    fs::make_dirs(p);
    return p;
}

Bytes bt_exe(const char* signature, std::uint16_t flags_at, std::uint16_t flags) {
    Bytes b(0x200, 0);
    b[0] = 'M';
    b[1] = 'Z';
    b[0x3C] = 0x80;
    b[0x80] = static_cast<std::uint8_t>(signature[0]);
    b[0x81] = static_cast<std::uint8_t>(signature[1]);
    b[0x80u + flags_at] = static_cast<std::uint8_t>(flags);
    b[0x80u + flags_at + 1u] = static_cast<std::uint8_t>(flags >> 8);
    return b;
}

Board bt_board(const stdfs::path& root) {
    Board b;
    b.root = root;
    b.source = "disc.iso";
    b.source_size = 10;
    b.complete = true;
    for (const char* rel : {"a/KERNEL32.DLL", "a/NOTEPAD.EXE", "b/SETUP.INF"}) {
        Card c;
        c.rel = rel;
        c.name = stdfs::path(rel).filename().string();
        c.size = 100;
        c.pile = c.name.ends_with("DLL") ? Pile::Libraries : c.name.ends_with("EXE") ? Pile::Programs : Pile::Setup;
        b.cards.push_back(c);
    }
    return b;
}

CB_TEST("app.board.a_known_extension_chooses_the_pile_and_the_bytes_name_the_format", [] {
    const Bytes ne = bt_exe("NE", 0x0C, 0x8000);
    const Kind driver = classify("VGA.DRV", ne);
    CB_CHECK(driver.pile == Pile::Drivers);
    CB_CHECK_EQ(driver.format, std::string("NE"));
    CB_CHECK(classify("SETUP.INF", as_bytes("[Version]\r\n")).pile == Pile::Setup);
    CB_CHECK(classify("ARIAL.TTF", {}).pile == Pile::Fonts);
    CB_CHECK(classify("WIN95_02.CAB", as_bytes("MSCF")).pile == Pile::Cabinets);
});

CB_TEST("app.board.with_no_known_extension_the_bytes_choose_the_pile", [] {
    CB_CHECK(classify("MYSTERY", bt_exe("NE", 0x0C, 0x8000)).pile == Pile::Libraries);
    CB_CHECK(classify("MYSTERY", bt_exe("NE", 0x0C, 0)).pile == Pile::Programs);
    CB_CHECK(classify("MYSTERY", bt_exe("LE", 0x10, 0)).pile == Pile::Drivers);
    CB_CHECK(classify("README", as_bytes("plain words\r\n")).pile == Pile::Text);
    const Bytes binary(64, 0);
    CB_CHECK(classify("BLOB", binary).pile == Pile::Data);
    CB_CHECK_EQ(classify("X.BI_", as_bytes("SZDD\x88\xF0")).format, std::string("SZDD"));
});

CB_TEST("app.board.a_done_without_files_is_refused", [] {
    const stdfs::path root = bt_folder("refused");
    Card card;
    record(card, Conv::Done, root / "nothing-here", {});
    CB_CHECK(card.conv == Conv::Failed);
    CB_CHECK(card.output.empty());
    fs::make_dirs(root / "empty");
    record(card, Conv::Done, root / "empty", {});
    CB_CHECK(card.conv == Conv::Failed);
    fs::write_text(root / "real" / "src" / "main.c", "int main;");
    record(card, Conv::Done, root / "real", {});
    CB_CHECK(card.conv == Conv::Done);
    CB_CHECK(!card.output.empty());
});

CB_TEST("app.board.a_kept_board_comes_back_and_a_done_whose_output_went_is_undone", [] {
    const stdfs::path root = bt_folder("kept");
    Board b = bt_board(root);
    b.failures.push_back({"X.CAB", "bad\ttab"});
    fs::write_text(root / "out1" / "a.c", "x");
    fs::write_text(root / "out2" / "b.c", "y");
    record(b.cards[0], Conv::Done, root / "out1", {});
    record(b.cards[1], Conv::Done, root / "out2", {});
    record(b.cards[2], Conv::CannotConvert, {}, "no route");
    save(b);
    fs::remove_tree(root / "out2");
    const std::optional<Board> again = load(root);
    CB_CHECK(again.has_value());
    CB_CHECK_EQ(again->cards.size(), std::size_t{3});
    CB_CHECK_EQ(again->failures[0].why, std::string("bad\ttab"));
    CB_CHECK(again->cards[0].conv == Conv::Done);
    CB_CHECK(again->cards[1].conv == Conv::None);
    CB_CHECK(again->cards[2].conv == Conv::CannotConvert);
    CB_CHECK_EQ(again->cards[2].note, std::string("no route"));
});

CB_TEST("app.board.a_conversion_in_flight_is_not_kept_and_an_unfinished_board_is_not_loaded", [] {
    const stdfs::path root = bt_folder("flight");
    Board b = bt_board(root);
    b.cards[0].conv = Conv::Running;
    b.cards[1].conv = Conv::Queued;
    save(b);
    const std::optional<Board> again = load(root);
    CB_CHECK(again.has_value());
    CB_CHECK(again->cards[0].conv == Conv::None);
    CB_CHECK(again->cards[1].conv == Conv::None);
    b.complete = false;
    save(b);
    CB_CHECK(!load(root).has_value());
});

CB_TEST("app.board.stacks_split_done_from_to_do_sort_by_name_and_filter", [] {
    const stdfs::path root = bt_folder("stacks");
    Board b = bt_board(root);
    Card extra = b.cards[0];
    extra.rel = "z/ADVAPI32.DLL";
    extra.name = "ADVAPI32.DLL";
    b.cards.push_back(extra);
    b.cards[0].conv = Conv::Done;
    Stacks s = build_stacks(b);
    const Stack& libraries = s[static_cast<std::size_t>(Pile::Libraries)];
    CB_CHECK_EQ(libraries.done.size(), std::size_t{1});
    CB_CHECK_EQ(libraries.todo.size(), std::size_t{1});
    CB_CHECK_EQ(b.cards[libraries.todo[0]].name, std::string("ADVAPI32.DLL"));
    s = build_stacks(b, "notepad");
    CB_CHECK_EQ(s[static_cast<std::size_t>(Pile::Programs)].total(), std::size_t{1});
    CB_CHECK_EQ(s[static_cast<std::size_t>(Pile::Libraries)].total(), std::size_t{0});
});

CB_TEST("app.board.identical_files_stack_as_one_master_and_a_converted_copy_stands_for_them", [] {
    Board b = bt_board(bt_folder("masters"));
    Card copy = b.cards[0];
    copy.rel = "z/KERNEL32.DLL";
    b.cards.push_back(copy);
    b.cards[3].conv = Conv::Done;
    const std::vector<std::uint32_t> group{0, 1, 2, 0};
    StackOptions options;
    options.group = &group;
    Stacks s = build_stacks(b, {}, options);
    const Stack& libraries = s[static_cast<std::size_t>(Pile::Libraries)];
    CB_CHECK_EQ(libraries.total(), std::size_t{1});
    CB_CHECK_EQ(libraries.done.size(), std::size_t{1});
    CB_CHECK_EQ(libraries.done[0], 3u);
    const std::vector<char> allowed{1, 0, 0, 0};
    options.allowed = &allowed;
    s = build_stacks(b, {}, options);
    CB_CHECK_EQ(s[static_cast<std::size_t>(Pile::Libraries)].todo.size(), std::size_t{1});   // only the copy the filter lets through
    CB_CHECK_EQ(s[static_cast<std::size_t>(Pile::Programs)].total(), std::size_t{0});
});

CB_TEST("app.board.another_image_or_a_changed_one_has_another_folder", [] {
    const stdfs::path root = bt_folder("identity");
    fs::write_text(root / "a.iso", "one");
    fs::write_text(root / "b.iso", "one");
    const stdfs::path a = board_folder(root / "boards", root / "a.iso");
    CB_CHECK(a != board_folder(root / "boards", root / "b.iso"));
    CB_CHECK(a == board_folder(root / "boards", root / "a.iso"));
    fs::write_text(root / "a.iso", "longer");
    CB_CHECK(a != board_folder(root / "boards", root / "a.iso"));
});

CB_TEST("app.board.words", [] {
    CB_CHECK_EQ(count_words(1234567), std::string("1,234,567"));
    CB_CHECK_EQ(count_words(12), std::string("12"));
    CB_CHECK_EQ(size_words(512), std::string("512 B"));
    CB_CHECK_EQ(size_words(1536), std::string("1.5 KB"));
});

}  // namespace
}  // namespace cb::app::board
