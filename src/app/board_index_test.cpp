// Tests of board_index.hpp: setup scripts, names in bytes, which copy a name means, grouping of identical files, and the kept index.
#include "app/board_index.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

namespace stdfs = std::filesystem;

stdfs::path bi_folder(const char* name) {
    const stdfs::path p = stdfs::temp_directory_path() / "cb_board_index_test" / name;
    fs::remove_tree(p);
    fs::make_dirs(p);
    return p;
}

Card bi_card(const char* rel, Pile pile, const char* format = "") {
    Card c;
    c.rel = rel;
    c.name = stdfs::path(rel).filename().string();
    c.pile = pile;
    c.format = format;
    return c;
}

bool bi_has(const std::vector<std::string>& v, const char* s) { return std::find(v.begin(), v.end(), s) != v.end(); }

CB_TEST("app.board_index.a_setup_script_installs_what_its_copyfiles_sections_list", [] {
    const std::vector<std::string> files = inf_copied_files(
        "; a comment\r\n[Install]\r\nCopyFiles=Sys.Copy, Help.Copy,@direct.dll\r\nAddReg=Reg\r\n"
        "[Sys.Copy]\r\nnew.dll,old.dl_,,32\r\nplain.exe ; why\r\n[Help.Copy]\r\nguide.hlp\r\n"
        "[Unrelated]\r\nnot_copied.dll\r\n[Reg]\r\nHKLM,Software\\X,,,1\r\n");
    CB_CHECK(bi_has(files, "new.dll"));
    CB_CHECK(bi_has(files, "old.dl_"));
    CB_CHECK(bi_has(files, "plain.exe"));
    CB_CHECK(bi_has(files, "guide.hlp"));
    CB_CHECK(bi_has(files, "direct.dll"));
    CB_CHECK(!bi_has(files, "not_copied.dll"));
    CB_CHECK_EQ(files.size(), std::size_t{5});
});

CB_TEST("app.board_index.names_are_found_in_ascii_and_utf16_and_only_whole", [] {
    const std::unordered_set<std::string> known{"shell32.dll", "help.hlp", "a.exe"};
    Bytes data = to_bytes(std::string("xx\0LoadLibrary\0SHELL32.DLL\0notshell32.dll\0", 42));
    for (const char c : std::string_view("\\help.hlp")) {
        data.push_back(static_cast<std::uint8_t>(c));
        data.push_back(0);
    }
    const std::vector<std::string> found = names_in_bytes(data, known);
    CB_CHECK(bi_has(found, "shell32.dll"));
    CB_CHECK(bi_has(found, "help.hlp"));
    CB_CHECK(!bi_has(found, "a.exe"));
});

CB_TEST("app.board_index.a_name_means_the_nearest_copy_and_says_how_sure", [] {
    Board b;
    b.cards = {bi_card("a/app.exe", Pile::Programs), bi_card("a/lib.dll", Pile::Libraries), bi_card("z/lib.dll", Pile::Libraries), bi_card("z/only.dll", Pile::Libraries)};
    const NameTable table(b);
    const auto never = [](std::uint32_t, std::uint32_t) { return false; };
    const auto always = [](std::uint32_t, std::uint32_t) { return true; };
    const auto near = table.resolve("LIB.DLL", 0, never);
    CB_CHECK(near.has_value());
    CB_CHECK_EQ(near->card, 1u);
    CB_CHECK_EQ(int{near->confidence}, 90);
    CB_CHECK_EQ(int{table.resolve("lib.dll", 0, always)->confidence}, 100);
    CB_CHECK_EQ(int{table.resolve("only", 0, never)->confidence}, 100);   // no extension: tried as a module
    CB_CHECK(!table.resolve("missing.dll", 0, never).has_value());
    CB_CHECK(!table.resolve("app.exe", 0, never).has_value());            // only itself
});

CB_TEST("app.board_index.a_board_groups_identical_files_links_them_and_comes_back", [] {
    const stdfs::path root = bi_folder("whole");
    Board b;
    b.root = root;
    b.complete = true;
    b.cards = {bi_card("a/one.txt", Pile::Text), bi_card("b/setup.inf", Pile::Setup), bi_card("c/one.txt", Pile::Text), bi_card("c/tool.exe", Pile::Data)};
    fs::write_text(b.file_of(b.cards[0]), "same bytes");
    fs::write_text(b.file_of(b.cards[1]), "[I]\nCopyFiles=F\n[F]\ntool.exe\nabsent.dll\n");
    fs::write_text(b.file_of(b.cards[2]), "same bytes");
    fs::write_text(b.file_of(b.cards[3]), "reads one.txt");
    const std::atomic<bool> go{false};
    const std::optional<Index> index = build_index(b, {}, go);
    CB_CHECK(index.has_value());
    CB_CHECK_EQ(index->nodes[2].group, 0u);
    CB_CHECK_EQ(index->nodes[0].copies, 2u);
    CB_CHECK_EQ(index->nodes[1].copies, 1u);
    CB_CHECK_EQ(index->nodes[1].unresolved, 1u);
    bool installs = false;
    for (const Edge& e : index->out_of(1)) installs = installs || (e.to == 3 && e.link == Link::Installs && e.confidence == 100);
    CB_CHECK(installs);
    CB_CHECK_EQ(index->into(3).size(), std::size_t{1});
    save_index(b, *index);
    const std::optional<Index> again = load_index(b);
    CB_CHECK(again.has_value());
    CB_CHECK(again->edges == index->edges);
    CB_CHECK_EQ(again->nodes[2].sha256, index->nodes[2].sha256);
    b.cards.pop_back();
    CB_CHECK(!load_index(b).has_value());   // for other cards
});

CB_TEST("app.board_index.a_missing_file_is_counted_and_cancel_stops", [] {
    const stdfs::path root = bi_folder("missing");
    Board b;
    b.root = root;
    b.cards = {bi_card("gone.bin", Pile::Data)};
    const std::atomic<bool> go{false}, stop{true};
    const std::optional<Index> index = build_index(b, {}, go);
    CB_CHECK(index.has_value());
    CB_CHECK_EQ(index->unreadable, std::uint64_t{1});
    CB_CHECK(index->nodes[0].sha256.empty());
    CB_CHECK(!build_index(b, {}, stop).has_value());
});

CB_TEST("app.board_index.call_sites_count_direct_calls_loads_and_calls_of_a_thunk", [] {
    const Bytes code = {
        0xFF, 0x15, 0x00, 0x20, 0x40, 0x00,         // 0x401000  call [0x402000]         module 0
        0x8B, 0x35, 0x04, 0x20, 0x40, 0x00,         // 0x401006  mov esi, [0x402004]     module 1
        0xA1, 0x04, 0x20, 0x40, 0x00,               // 0x40100C  mov eax, [0x402004]     module 1
        0xE8, 0x05, 0x00, 0x00, 0x00,               // 0x401011  call 0x40101B           the thunk, module 0
        0x90, 0x90, 0x90, 0x90, 0x90,               // 0x401016
        0xFF, 0x25, 0x00, 0x20, 0x40, 0x00,         // 0x40101B  jmp [0x402000]
        0xFF, 0x15, 0x08, 0x20, 0x40, 0x00,         // 0x401021  call [0x402008]         not an import
    };
    const std::unordered_map<std::uint64_t, std::uint32_t> slots{{0x402000, 0}, {0x402004, 1}};
    const CodeSpan span{code, 0x401000};
    const std::vector<std::uint32_t> sites = pe_call_sites(std::span<const CodeSpan>(&span, 1), slots, 2);
    CB_CHECK_EQ(sites[0], 2u);
    CB_CHECK_EQ(sites[1], 2u);
});

CB_TEST("app.board_index.an_ne_relocation_chain_counts_every_place_and_cannot_loop", [] {
    const Bytes chained = {0x04, 0x00, 0, 0, 0x08, 0x00, 0, 0, 0xFF, 0xFF};
    CB_CHECK_EQ(ne_chain_sites(chained, 0), 3u);
    const Bytes loop = {0x02, 0x00, 0x00, 0x00};
    CB_CHECK(ne_chain_sites(loop, 0) <= 3u);
    CB_CHECK_EQ(ne_chain_sites(loop, 0xFFFF), 1u);
});

CB_TEST("app.board_index.exports_and_their_users_are_kept_in_order", [] {
    const stdfs::path root = bi_folder("exports");
    Board b;
    b.root = root;
    b.complete = true;
    b.cards = {bi_card("a.dll", Pile::Libraries), bi_card("b.exe", Pile::Programs)};
    fs::write_text(b.file_of(b.cards[0]), "a");
    fs::write_text(b.file_of(b.cards[1]), "b");
    Index index;
    index.nodes.resize(2);
    index.exports = {{0, 7, "Second"}, {0, 2, "First"}};
    index.finish();
    CB_CHECK_EQ(index.exports_of(0)[0].name, std::string("First"));
    index.export_uses = {{1, 1, 9}};
    index.finish();
    CB_CHECK_EQ(index.uses_of(1).size(), std::size_t{1});
    CB_CHECK_EQ(index.uses_of(0).size(), std::size_t{0});
    save_index(b, index);
    const std::optional<Index> again = load_index(b);
    CB_CHECK(again.has_value());
    CB_CHECK_EQ(again->exports[1].name, std::string("Second"));
    CB_CHECK_EQ(again->uses_of(1)[0].calls, 9u);
});

CB_TEST("app.board_index.a_damaged_binary_is_reported_not_thrown", [] {
    const Bytes junk(16, 0x4D);
    CB_CHECK(read_binary(junk, "PE").damaged);
    CB_CHECK(read_binary(junk, "NE").damaged);
    CB_CHECK(read_binary(junk, "LE").damaged);
    CB_CHECK(!read_binary(junk, "text").damaged);
});

}  // namespace
}  // namespace cb::app::board
