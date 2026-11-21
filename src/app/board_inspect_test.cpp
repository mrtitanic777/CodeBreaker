// Tests of board_inspect.hpp: links across copies, identical sources counted once, signals in the inspector, and what a hover lights up.
#include "app/board_inspect.hpp"

#include <algorithm>

#include "core/testing.hpp"

namespace cb::app::board {
namespace {

// app (two identical copies) imports lib/1 and lib/2 (identical); setup installs lib/2; lib imports core; other is unrelated.
struct Sample {
    Board board;
    Index index;
    Signals signals;
    Sample() {
        const struct { const char* rel; Pile pile; std::uint32_t group; } files[] = {
            {"a/app.exe", Pile::Programs, 0}, {"b/app.exe", Pile::Programs, 0}, {"c/lib.dll", Pile::Libraries, 2}, {"d/lib.dll", Pile::Libraries, 2},
            {"e/setup.inf", Pile::Setup, 4},  {"f/core.dll", Pile::Libraries, 5}, {"g/other.txt", Pile::Text, 6},
        };
        for (const auto& f : files) {
            Card c;
            c.rel = f.rel;
            c.name = c.rel.substr(2);
            c.pile = f.pile;
            board.cards.push_back(c);
            Node n;
            n.group = f.group;
            n.sha256 = "hash" + std::to_string(f.group);
            index.nodes.push_back(n);
        }
        Edge a{0, 2, Link::Imports, 100, 3, 3, 40}, b{1, 3, Link::Imports, 100, 3, 3, 50}, c{4, 3, Link::Installs, 100}, d{2, 5, Link::Imports, 100, 1, 1, 7};
        index.edges = {a, b, c, d};
        index.finish();
        signals.signals = {{SignalType::Mutex, "shared"}, {SignalType::IoPort, "0x3f8"}};
        signals.links = {{2, 0, Direction::Writes, 70}, {3, 0, Direction::Writes, 60}, {3, 1, Direction::Reads, 50}};
        signals.finish(board.cards.size());
    }
};

const InspectorRow* find_row(const Inspector& i, InspectorRow::Kind kind, std::string_view text) {
    for (const InspectorRow& r : i.rows)
        if (r.kind == kind && r.text == text) return &r;
    return nullptr;
}

CB_TEST("app.board_inspect.links_are_counted_across_copies_and_identical_sources_once", [] {
    const Sample s;
    const Inspector i = build_inspector(s.board, s.index, &s.signals, 2);
    CB_CHECK_EQ(i.copies, 2u);
    CB_CHECK_EQ(i.inbound, 2u);     // app (both copies are one file) and setup
    CB_CHECK_EQ(i.outbound, 1u);
    const InspectorRow* app = find_row(i, InspectorRow::Kind::File, "app.exe");
    CB_CHECK(app != nullptr);
    CB_CHECK(app->detail.find("50 call sites") != std::string::npos);   // the larger of its copies, not 90
    CB_CHECK(find_row(i, InspectorRow::Kind::File, "setup.inf") != nullptr);
    CB_CHECK(find_row(i, InspectorRow::Kind::File, "core.dll") != nullptr);
    CB_CHECK(find_row(i, InspectorRow::Kind::File, "other.txt") == nullptr);
});

CB_TEST("app.board_inspect.signals_keep_their_best_confidence_and_a_click_target", [] {
    const Sample s;
    const Inspector i = build_inspector(s.board, s.index, &s.signals, 2);
    CB_CHECK_EQ(i.signals, 2u);
    const InspectorRow* mutex = find_row(i, InspectorRow::Kind::Signal, "shared");
    CB_CHECK(mutex != nullptr);
    CB_CHECK_EQ(mutex->signal, std::int64_t{0});
    CB_CHECK(mutex->detail.find("70%") != std::string::npos);
});

CB_TEST("app.board_inspect.copies_are_listed_only_when_there_are_some", [] {
    const Sample s;
    CB_CHECK(find_row(build_inspector(s.board, s.index, &s.signals, 2), InspectorRow::Kind::File, "d/lib.dll") != nullptr);
    const Inspector alone = build_inspector(s.board, s.index, nullptr, 6);
    CB_CHECK_EQ(alone.copies, 1u);
    CB_CHECK_EQ(alone.inbound + alone.outbound, 0u);
    for (const InspectorRow& r : alone.rows) CB_CHECK(!r.text.starts_with("Copies"));
});

CB_TEST("app.board_inspect.a_hover_lights_what_it_uses_what_uses_it_and_their_copies", [] {
    const Sample s;
    const HoverLinks h = hover_links(s.index, 2);
    CB_CHECK_EQ(h.uses, 1u);
    CB_CHECK_EQ(h.used_by, 2u);
    CB_CHECK_EQ(int{h.role[0]}, 2);
    CB_CHECK_EQ(int{h.role[1]}, 2);   // the other copy of app
    CB_CHECK_EQ(int{h.role[4]}, 2);
    CB_CHECK_EQ(int{h.role[5]}, 1);
    CB_CHECK_EQ(int{h.role[3]}, 0);   // its own copy is not a link
    CB_CHECK_EQ(int{h.role[6]}, 0);
});

}  // namespace
}  // namespace cb::app::board
