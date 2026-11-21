// Tests of board_automation.hpp: orphans, decks kept and read back, the order of a batch, and the three export formats.
#include "app/board_automation.hpp"

#include <algorithm>

#include "core/fs.hpp"
#include "core/testing.hpp"

namespace cb::app::board {
namespace {

namespace stdfs = std::filesystem;

// app imports lib; setup installs app; readme mentions lib; lonely is linked to by nothing; started is linked to by nothing but runs at boot; noted is named only by readme.
struct Sample {
    Board board;
    Index index;
    Signals signals;
    Sample() {
        board.source = "disc \"1\".iso";
        const struct { const char* rel; Pile pile; const char* format; } files[] = {
            {"a/app.exe", Pile::Programs, "PE"}, {"a/lib.dll", Pile::Libraries, "PE"}, {"a/setup.inf", Pile::Setup, "text"}, {"a/readme.txt", Pile::Text, "text"},
            {"a/lonely.dll", Pile::Libraries, "NE"}, {"a/started.exe", Pile::Programs, "PE"}, {"a/noted.hlp", Pile::Help, ""},
        };
        std::uint32_t i = 0;
        for (const auto& f : files) {
            Card c;
            c.rel = f.rel;
            c.name = c.rel.substr(2);
            c.pile = f.pile;
            c.format = f.format;
            board.cards.push_back(c);
            Node n;
            n.group = i++;
            n.sha256 = "hash" + std::to_string(n.group);
            index.nodes.push_back(n);
        }
        index.edges = {{0, 1, Link::Imports, 100, 5, 4, 30}, {2, 0, Link::Installs, 100}, {3, 1, Link::References, 100}, {3, 6, Link::References, 100}};
        index.finish();
        signals.signals = {{SignalType::Autostart, "hklm run"}, {SignalType::Mutex, "a<b>&\"c"}};
        signals.links = {{5, 0, Direction::Registers, 100}, {0, 1, Direction::Writes, 70}};
        signals.finish(board.cards.size());
    }
};

CB_TEST("app.board_automation.an_orphan_has_no_link_from_code_or_setup_and_does_not_start_itself", [] {
    const Sample s;
    const Orphans o = find_orphans(s.board, s.index, &s.signals);
    CB_CHECK(!o.card[0] && !o.card[1]);
    CB_CHECK(o.card[2] && o.card[3] && o.card[4]);   // nothing links to the script, the text or lonely
    CB_CHECK(!o.card[5]);                            // it starts at boot
    CB_CHECK(o.card[6]);                             // a text file naming it is no link from code or setup
    CB_CHECK_EQ(o.distinct, 4u);
});

CB_TEST("app.board_automation.copies_of_a_linked_file_are_not_orphans", [] {
    Sample s;
    s.index.nodes[4].group = 1;   // lonely now holds lib's bytes
    s.index.finish();
    CB_CHECK(!find_orphans(s.board, s.index, &s.signals).card[4]);
});

CB_TEST("app.board_automation.a_deck_is_kept_and_read_back_and_builtin_ones_are_not_written", [] {
    const stdfs::path root = stdfs::temp_directory_path() / "cb_board_automation_test" / "decks";
    fs::remove_tree(root);
    std::vector<Deck> decks = builtin_decks();
    Deck mine;
    mine.name = "Mine\twith a tab";
    mine.query = "kernel";
    mine.orphans = true;
    mine.names = {"ddraw", "dsound"};
    mine.filters = {{SignalType::Mutex, "__DDHelpMutex__"}};
    decks.push_back(mine);
    save_decks(root, decks);
    const std::vector<Deck> again = load_decks(root);
    CB_CHECK_EQ(again.size(), std::size_t{1});
    CB_CHECK_EQ(again[0].name, std::string("Mine with a tab"));
    CB_CHECK(again[0].orphans);
    CB_CHECK_EQ(again[0].names.size(), std::size_t{2});
    CB_CHECK(again[0].filters[0].type == SignalType::Mutex);
    CB_CHECK_EQ(again[0].filters[0].value, std::string("__DDHelpMutex__"));
    CB_CHECK(load_decks(root / "none").empty());
});

CB_TEST("app.board_automation.a_deck_of_names_matches_any_of_them", [] {
    const Sample s;
    const std::vector<char> named = cards_named(s.board, {"lib", "noted"});
    CB_CHECK(named[1] && named[6]);
    CB_CHECK(!named[0]);
    CB_CHECK_EQ(builtin_decks().size(), std::size_t{3});
});

CB_TEST("app.board_automation.a_batch_takes_unconverted_binaries_most_linked_first", [] {
    Sample s;
    s.board.cards[5].conv = Conv::Done;
    s.board.cards[4].conv = Conv::CannotConvert;
    const std::vector<std::uint32_t> order = batch_order(s.board, s.index, build_stacks(s.board));
    const std::vector<std::uint32_t> expected{1, 0};   // lib has two links in, app one; text, help, done and unconvertible files are left out
    CB_CHECK(order == expected);
});

CB_TEST("app.board_automation.a_cluster_keeps_only_links_among_its_modules", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports | kLinkInstalls | kLinkReferences);
    const Cluster c = build_cluster(g, s.index, nullptr, {g.module_of_card[0], g.module_of_card[1], g.module_of_card[1]});
    CB_CHECK_EQ(c.modules.size(), std::size_t{2});
    CB_CHECK_EQ(c.edges.size(), std::size_t{1});
    CB_CHECK(c.edges[0].link == Link::Imports);
    CB_CHECK_EQ(c.edges[0].calls, std::uint64_t{30});
    CB_CHECK_EQ(c.edges[0].links, std::uint64_t{4});
});

CB_TEST("app.board_automation.the_exports_escape_what_their_formats_need", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports | kLinkInstalls | kLinkReferences);
    std::vector<std::uint32_t> all;
    for (std::uint32_t m = 0; m < g.modules.size(); ++m) all.push_back(m);
    const Cluster c = build_cluster(g, s.index, nullptr, all);
    const std::string json = cluster_json(s.board, s.index, &s.signals, g, c);
    CB_CHECK(json.find("disc \\\"1\\\".iso") != std::string::npos);
    CB_CHECK(json.find("\"link_type\": \"IMPORTS\"") != std::string::npos);
    CB_CHECK(json.find("a<b>&\\\"c") != std::string::npos);
    CB_CHECK(json.find("\"direction\": \"WRITES\"") != std::string::npos);
    const std::string xml = cluster_graphml(s.board, s.index, g, c);
    CB_CHECK(xml.find("edgedefault=\"directed\"") != std::string::npos);
    CB_CHECK_EQ(static_cast<std::size_t>(std::count(xml.begin(), xml.end(), '\n')) > c.edges.size(), true);
    const std::string table = cluster_table(s.board, g, c);
    CB_CHECK(table.starts_with("source_entity_type,"));
    CB_CHECK(table.find("maltego.Document,\"app.exe\"") != std::string::npos);
});

}  // namespace
}  // namespace cb::app::board
