// Tests of board_projection.hpp: the kinds of function, residency, copies counted once, the busiest channel and hot path, and the dots.
#include "app/board_projection.hpp"

#include <algorithm>
#include <cmath>

#include "core/testing.hpp"

namespace cb::app::board {
namespace {

// app.exe (runs 2% of the hour) and explorer.exe (core) both call user.dll!GetMessageA; user.dll calls core.dll!LoadLibraryA. Two copies of app.exe.
struct Sample {
    Board board;
    Index index;
    Sample() {
        const struct { const char* rel; Pile pile; std::uint32_t group; } files[] = {
            {"a/app.exe", Pile::Programs, 0}, {"b/app.exe", Pile::Programs, 0}, {"c/explorer.exe", Pile::Programs, 2}, {"d/user.dll", Pile::Libraries, 3}, {"e/core.dll", Pile::Libraries, 4},
        };
        for (const auto& f : files) {
            Card c;
            c.rel = f.rel;
            c.name = c.rel.substr(2);
            c.pile = f.pile;
            board.cards.push_back(c);
            Node n;
            n.group = f.group;
            index.nodes.push_back(n);
        }
        index.edges = {{0, 3, Link::Imports, 100}, {1, 3, Link::Imports, 100}, {2, 3, Link::Imports, 100}, {3, 4, Link::Imports, 100}};
        index.exports = {{3, 1, "GetMessageA"}, {4, 1, "LoadLibraryA"}};
        index.finish();
        index.export_uses = {{0, 0, 10}, {1, 0, 4}, {2, 0, 5}, {3, 1, 7}};
        index.finish();
    }
};

std::uint32_t module_named(const Graph& g, const char* name) {
    for (std::uint32_t m = 0; m < g.modules.size(); ++m)
        if (g.modules[m].name == name) return m;
    CB_CHECK(false);
    return 0;
}

CB_TEST("app.board_projection.a_function_is_known_by_its_name_in_either_form", [] {
    CB_CHECK(api_kind("GetMessageA") == ApiKind::MessagePump);
    CB_CHECK(api_kind("sendmessagew") == ApiKind::MessagePump);
    CB_CHECK(api_kind("EnterCriticalSection") == ApiKind::Sync);
    CB_CHECK(api_kind("LoadLibraryA") == ApiKind::Lifecycle);
    CB_CHECK(api_kind("#12") == ApiKind::Other);
    CB_CHECK(api_rate(ApiKind::MessagePump) > api_rate(ApiKind::Paint));
    CB_CHECK(api_rate(ApiKind::Paint) > api_rate(ApiKind::Lifecycle));
});

CB_TEST("app.board_projection.calls_are_sites_times_kind_times_residency_and_copies_count_once", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const Projection p = build_projection(s.board, s.index, nullptr);
    const std::uint32_t app = module_named(g, "app.exe"), explorer = module_named(g, "explorer.exe"), user = module_named(g, "user.dll"), core = module_named(g, "core.dll");
    CB_CHECK(std::abs(p.residency[explorer] - 1.0) < 1e-9);
    CB_CHECK(std::abs(p.residency[app] - kIdleResidency) < 1e-9);
    CB_CHECK(std::abs(p.residency[user] - kResidencyDecay) < 1e-9);           // imported by a resident file
    CB_CHECK(std::abs(p.residency[core] - kResidencyDecay * kResidencyDecay) < 1e-9);
    CB_CHECK(std::abs(p.channel(app, user) - 10 * 3600 * kIdleResidency) < 1e-6);   // the larger of its two copies, once
    CB_CHECK(std::abs(p.channel(explorer, user) - 5 * 3600.0) < 1e-6);
    CB_CHECK(std::abs(p.function(user, "getmessagea") - (5 * 3600.0 + 10 * 3600 * kIdleResidency)) < 1e-6);
    CB_CHECK(std::abs(p.function_user(user, "getmessagea", explorer) - 5 * 3600.0) < 1e-6);
    CB_CHECK_EQ(p.channel(user, app), 0.0);
});

CB_TEST("app.board_projection.the_hot_path_runs_through_the_busiest_channel", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const Projection p = build_projection(s.board, s.index, nullptr);
    const std::uint32_t explorer = module_named(g, "explorer.exe"), user = module_named(g, "user.dll"), core = module_named(g, "core.dll");
    CB_CHECK(std::abs(p.max_channel - 5 * 3600.0) < 1e-6);
    const std::vector<std::uint32_t> expected{explorer, user, core};
    CB_CHECK(p.hot_path == expected);
});

CB_TEST("app.board_projection.an_autostart_file_is_resident", [] {
    const Sample s;
    Signals signals;
    signals.signals = {{SignalType::Autostart, "hklm run"}};
    signals.links = {{0, 0, Direction::Registers, 100}};
    signals.finish(s.board.cards.size());
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const Projection p = build_projection(s.board, s.index, &signals);
    CB_CHECK(std::abs(p.residency[module_named(g, "app.exe")] - 1.0) < 1e-9);
});

CB_TEST("app.board_projection.a_trace_replaces_the_model_where_it_saw_and_calibrates_the_rest", [] {
    Sample s;
    // a second function nobody traced: its model is scaled by what the trace said of its kind
    s.index.exports = {{3, 1, "GetMessageA"}, {3, 2, "PeekMessageA"}, {4, 1, "LoadLibraryA"}};
    s.index.finish();
    s.index.export_uses = {{0, 0, 10}, {1, 0, 4}, {2, 0, 5}, {2, 1, 2}, {3, 2, 7}};
    s.index.finish();
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const std::uint32_t explorer = module_named(g, "explorer.exe"), user = module_named(g, "user.dll"), app = module_named(g, "app.exe");
    Measured m;
    m.seconds = 3600;
    m.functions = {{"user.dll", "GetMessageA", "explorer.exe", 900}};
    const Projection p = build_projection(s.board, s.index, nullptr, &m);
    CB_CHECK(p.has_measured);
    CB_CHECK(p.channel_measured(explorer, user));
    CB_CHECK(!p.channel_measured(app, user));
    CB_CHECK(p.function_user_measured(user, "getmessagea", explorer));
    CB_CHECK(!p.function_user_measured(user, "peekmessagea", explorer));
    CB_CHECK(std::abs(p.function_user(user, "getmessagea", explorer) - 900) < 1e-6);   // as measured
    CB_CHECK(std::abs(p.channel(explorer, user) - 900) < 1e-6);                         // the channel is what the trace saw, no model added
    const double scale = 900.0 / (5 * 3600);
    CB_CHECK(std::abs(p.kind_scale[static_cast<std::size_t>(ApiKind::MessagePump)] - scale) < 1e-9);
    CB_CHECK(std::abs(p.function_user(user, "peekmessagea", explorer) - 2 * 3600 * scale) < 1e-6);
    CB_CHECK(std::abs(p.channel(app, user) - 10 * 3600 * kIdleResidency * scale) < 1e-6);   // a caller the trace did not see stays a scaled model
    const std::vector<std::uint32_t> expected{explorer, user};
    CB_CHECK(p.hot_path == expected);                                                   // the hot path is only what really ran
});

CB_TEST("app.board_projection.a_measured_hot_path_stops_at_a_trickle_and_activity_counts_what_ran", [] {
    const Sample s;
    const Graph g = build_graph(s.board, s.index, kLinkImports);
    const std::uint32_t explorer = module_named(g, "explorer.exe"), user = module_named(g, "user.dll"), core = module_named(g, "core.dll"), app = module_named(g, "app.exe");
    Measured m;
    m.seconds = 3600;
    m.functions = {{"user.dll", "GetMessageA", "explorer.exe", 1000}, {"core.dll", "LoadLibraryA", "user.dll", 50}};
    const Projection p = build_projection(s.board, s.index, nullptr, &m);
    const std::vector<std::uint32_t> expected{explorer, user};   // 50 of 1000 is under the share
    CB_CHECK(p.hot_path == expected);
    CB_CHECK(std::abs(p.activity[user] - 1050) < 1e-6);
    CB_CHECK(std::abs(p.activity[core] - 50) < 1e-6);
    CB_CHECK_EQ(p.activity[app], 0.0);                           // a modeled channel is no activity once there is a trace
});

CB_TEST("app.board_projection.more_calls_means_more_and_faster_dots_relative_to_the_busiest", [] {
    CB_CHECK(flow_dots(0, 100, 1, 0).empty());
    CB_CHECK_EQ(flow_dots(100, 100, 0, 3).size(), static_cast<std::size_t>(kMaxFlowDots));
    CB_CHECK_EQ(flow_dots(0.001, 100, 0, 3).size(), std::size_t{1});          // the quietest live channel still shows
    CB_CHECK(flow_dots(25, 100, 0, 3).size() < flow_dots(81, 100, 0, 3).size());
    CB_CHECK_EQ(flow_intensity(25, 100), 0.5);
    CB_CHECK_EQ(flow_intensity(500, 100), 1.0);
    const auto moved = [](double rate) {
        const float a = flow_dots(rate, 100, 0, 0)[0], b = flow_dots(rate, 100, 0.1, 0)[0];
        return b - a;
    };
    CB_CHECK(moved(100) > moved(4) * 2);
    for (const float f : flow_dots(60, 100, 12345.6, 77)) CB_CHECK(f >= 0 && f < 1);
});

CB_TEST("app.board_projection.rates_read_short", [] {
    CB_CHECK_EQ(rate_words(870), std::string("870"));
    CB_CHECK_EQ(rate_words(34000), std::string("34K"));
    CB_CHECK_EQ(rate_words(1200000), std::string("1.2M"));
});

}  // namespace
}  // namespace cb::app::board
