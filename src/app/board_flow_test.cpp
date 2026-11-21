// Tests of board_flow.hpp: modules by name, identical copies counted once, flow bands by kind, and the trace's columns and heaviest path.
#include "app/board_flow.hpp"

#include <algorithm>

#include "core/testing.hpp"

namespace cb::app::board {
namespace {

// app -> user -> kernel; tool -> user; tool -> kernel; two identical copies of tool; a setup script installs app.
struct Sample {
    Board board;
    Index index;
    Sample() {
        const struct { const char* rel; Pile pile; std::uint32_t group; } files[] = {
            {"a/APP.EXE", Pile::Programs, 0},    {"a/USER.DLL", Pile::Libraries, 1}, {"a/KERNEL.DLL", Pile::Libraries, 2},
            {"b/TOOL.EXE", Pile::Programs, 3},   {"c/TOOL.EXE", Pile::Programs, 3},  {"d/SETUP.INF", Pile::Setup, 5},
        };
        for (const auto& f : files) {
            Card c;
            c.rel = f.rel;
            c.name = c.rel.substr(c.rel.find('/') + 1);
            c.pile = f.pile;
            board.cards.push_back(c);
            Node n;
            n.group = f.group;
            index.nodes.push_back(n);
        }
        index.edges = {{0, 1, Link::Imports, 100}, {1, 2, Link::Imports, 100}, {3, 1, Link::Imports, 100}, {4, 1, Link::Imports, 100},
                       {3, 2, Link::Imports, 100}, {5, 0, Link::Installs, 100}};
        index.finish();
    }
};

std::uint32_t by_name(const Graph& g, const char* name) {
    for (std::uint32_t m = 0; m < g.modules.size(); ++m)
        if (g.modules[m].name == name) return m;
    CB_CHECK(false);
    return 0;
}

CB_TEST("app.board_flow.copies_of_a_file_are_one_module_and_count_once", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    CB_CHECK_EQ(g.modules.size(), std::size_t{5});
    const Module& user = g.modules[by_name(g, "user.dll")];
    CB_CHECK_EQ(user.fan_in, 2u);   // app, and tool once though two copies import it
    CB_CHECK_EQ(g.modules[by_name(g, "kernel.dll")].fan_in, 2u);
    CB_CHECK(g.module_of_card[3] == g.module_of_card[4]);
});

CB_TEST("app.board_flow.only_the_chosen_kinds_of_link_count", [] {
    const Sample s;
    const Graph imports = build_graph(s.board, s.index, kLinkImports);
    CB_CHECK_EQ(imports.modules[by_name(imports, "app.exe")].fan_in, 0u);
    const Graph both = build_graph(s.board, s.index, kLinkImports | kLinkInstalls);
    CB_CHECK_EQ(both.modules[by_name(both, "app.exe")].fan_in, 1u);
    const Graph none = build_graph(s.board, s.index, kLinkReferences);
    CB_CHECK(!none.most_linked().has_value());
});

CB_TEST("app.board_flow.a_channel_can_be_as_wide_as_its_links_or_its_call_sites", [] {
    Sample s;
    for (Edge& e : s.index.edges) {
        if (e.link != Link::Imports) continue;
        e.symbols = 10;
        e.matched = e.from == 3 ? 7 : 4;
        e.calls = e.from == 3 ? 100 : 20;
    }
    s.index.edges[3].calls = 90;   // the other copy of tool: identical bytes count once, at the larger amount
    s.index.edges[3].matched = 6;
    const Graph links = build_graph(s.board, s.index, kLinkImports, Width::Links);
    CB_CHECK_EQ(links.modules[by_name(links, "user.dll")].fan_in, 4u + 7u);
    const Graph calls = build_graph(s.board, s.index, kLinkImports | kLinkInstalls, Width::Calls);
    CB_CHECK_EQ(calls.modules[by_name(calls, "user.dll")].fan_in, 20u + 100u);
    CB_CHECK_EQ(calls.modules[by_name(calls, "app.exe")].fan_in, 0u);   // an install has no call sites
    s.index.edges[1].calls = 0;
    const Graph without = build_graph(s.board, s.index, kLinkImports, Width::Calls);
    CB_CHECK(without.modules[by_name(without, "user.dll")].out.empty());   // no call site, no channel of that width
});

CB_TEST("app.board_flow.a_library_shows_its_imports_left_and_each_export_with_its_users_right", [] {
    Sample s;
    s.index.edges[1].calls = 5;   // user imports kernel
    for (Edge& e : s.index.edges)
        if (e.link == Link::Imports && e.calls == 0) e.calls = 3;
    s.index.exports = {{1, 1, "Create"}, {1, 2, "Destroy"}, {1, 3, ""}};
    s.index.finish();
    s.index.export_uses = {{0, 0, 10}, {3, 0, 30}, {4, 0, 20}, {3, 1, 1}};   // copies 3 and 4 of tool: one file, its larger count
    s.index.finish();
    const Graph g = build_graph(s.board, s.index, kLinkImports, Width::Calls);
    const std::uint32_t user = by_name(g, "user.dll");
    const LibraryView v = compute_library(g, s.index, user, Width::Calls);
    CB_CHECK_EQ(v.left.size(), std::size_t{1});
    CB_CHECK_EQ(v.left[0].label, std::string("kernel.dll"));
    CB_CHECK_EQ(v.left[0].weight, std::uint64_t{5});
    CB_CHECK_EQ(v.right.size(), std::size_t{3});
    CB_CHECK_EQ(v.right[0].label, std::string("Create"));
    CB_CHECK_EQ(v.right[0].calls, std::uint64_t{40});
    CB_CHECK_EQ(v.right[0].users.size(), std::size_t{2});
    CB_CHECK_EQ(v.right[0].users[0].module, by_name(g, "tool.exe"));
    CB_CHECK_EQ(v.right[2].label, std::string("#3"));
    CB_CHECK(v.right[2].users.empty());   // exported, used by nothing on the board: still a channel
    const LibraryView one = compute_library(g, s.index, user, Width::Calls, "create");
    CB_CHECK_EQ(one.left.size(), std::size_t{2});
    CB_CHECK_EQ(one.left[0].module, std::int64_t{by_name(g, "tool.exe")});
    CB_CHECK_EQ(one.right.size(), std::size_t{1});
    CB_CHECK_EQ(one.right[0].module, std::int64_t{user});
});

CB_TEST("app.board_flow.a_flow_groups_what_comes_in_and_goes_out_by_kind", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const Flow f = compute_flow(g, by_name(g, "user.dll"));
    CB_CHECK_EQ(f.in.size(), std::size_t{1});
    CB_CHECK(f.in[0].pile == Pile::Programs);
    CB_CHECK_EQ(f.in[0].modules, 2u);
    CB_CHECK_EQ(f.in_total, std::uint64_t{2});
    CB_CHECK_EQ(f.out.size(), std::size_t{1});
    CB_CHECK_EQ(f.out[0].top, by_name(g, "kernel.dll"));
});

CB_TEST("app.board_flow.a_trace_puts_callers_left_dependencies_right_and_marks_the_heaviest_path", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports | kLinkInstalls);
    const std::uint32_t user = by_name(g, "user.dll"), kernel = by_name(g, "kernel.dll"), app = by_name(g, "app.exe"), setup = by_name(g, "setup.inf");
    const Trace t = compute_trace(g, user, 10);
    CB_CHECK_EQ(t.columns[kTraceCenter][0], user);
    CB_CHECK_EQ(t.columns[kTraceCenter + 1][0], kernel);
    CB_CHECK_EQ(t.columns[kTraceCenter - 1].size(), std::size_t{2});
    CB_CHECK_EQ(t.columns[kTraceCenter - 2][0], setup);
    CB_CHECK_EQ(t.columns[kTraceCenter - 2].size(), std::size_t{1});
    const std::vector<std::uint32_t> expected{app, user, kernel};
    CB_CHECK(t.heaviest == expected);   // app has an installer linking to it, tool has nothing; the installer itself has no weight to be chosen by
    std::size_t marked = 0;
    for (const TraceLink& l : t.links) marked += l.heaviest ? 1 : 0;
    CB_CHECK_EQ(marked, std::size_t{2});
});

CB_TEST("app.board_flow.a_full_column_keeps_the_heaviest_path_and_counts_the_rest", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports | kLinkInstalls);
    const std::uint32_t user = by_name(g, "user.dll");
    const Trace t = compute_trace(g, user, 1);
    CB_CHECK_EQ(t.columns[kTraceCenter - 1].size(), std::size_t{1});
    CB_CHECK_EQ(t.hidden[kTraceCenter - 1], 1u);
    CB_CHECK(std::find(t.heaviest.begin(), t.heaviest.end(), t.columns[kTraceCenter - 1][0]) != t.heaviest.end());
});

}  // namespace
}  // namespace cb::app::board
