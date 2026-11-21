// app/board_flow.cpp -- see board_flow.hpp.
#include "app/board_flow.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "core/text.hpp"

namespace cb::app::board {

std::optional<std::uint32_t> Graph::most_linked() const {
    std::optional<std::uint32_t> best;
    for (std::uint32_t i = 0; i < modules.size(); ++i)
        if (modules[i].fan_in > 0 && (!best || modules[i].fan_in > modules[*best].fan_in)) best = i;
    return best;
}

std::string_view width_unit(Width width) {
    switch (width) {
        case Width::Links: return "export-import links";
        case Width::Calls: return "call sites";
        default: return "files";
    }
}

Graph build_graph(const Board& board, const Index& index, unsigned links, Width width) {
    Graph graph;
    const std::size_t n = board.cards.size();
    graph.module_of_card.resize(n);
    std::vector<std::string> names(n);
    for (std::size_t i = 0; i < n; ++i) names[i] = text::lower(board.cards[i].name);
    std::vector<std::string> sorted = names;
    std::sort(sorted.begin(), sorted.end());
    sorted.erase(std::unique(sorted.begin(), sorted.end()), sorted.end());
    graph.modules.resize(sorted.size());
    std::unordered_map<std::string, std::uint32_t> id;
    id.reserve(sorted.size());
    for (std::uint32_t m = 0; m < sorted.size(); ++m) {
        graph.modules[m].name = sorted[m];
        id.emplace(sorted[m], m);
    }
    std::vector<std::uint32_t> best_inbound(sorted.size(), 0);
    std::vector<bool> placed(sorted.size(), false);
    for (std::uint32_t i = 0; i < n; ++i) {
        const std::uint32_t m = id.at(names[i]);
        graph.module_of_card[i] = m;
        const auto inbound = static_cast<std::uint32_t>(index.into(i).size());
        if (!placed[m] || inbound > best_inbound[m]) {
            placed[m] = true;
            best_inbound[m] = inbound;
            graph.modules[m].card = i;
            graph.modules[m].pile = board.cards[i].pile;
        }
    }
    // (source name, target name, source's bytes, amount): a file counts once however many copies of it there are
    struct Seen {
        std::uint32_t from, to, group, amount;
    };
    std::vector<Seen> seen;
    seen.reserve(index.edges.size());
    if (width != Width::Files) links &= kLinkImports;   // (only an import has functions and call sites)
    for (const Edge& e : index.edges) {
        if ((links & (1u << static_cast<unsigned>(e.link))) == 0) continue;
        const std::uint32_t from = graph.module_of_card[e.from], to = graph.module_of_card[e.to];
        const std::uint32_t amount = width == Width::Links ? e.matched : width == Width::Calls ? e.calls : 1;
        if (from != to && amount > 0) seen.push_back({from, to, index.nodes[e.from].group, amount});
    }
    std::sort(seen.begin(), seen.end(), [](const Seen& a, const Seen& b) {
        return std::tie(a.from, a.to, a.group, b.amount) < std::tie(b.from, b.to, b.group, a.amount);   // (the largest amount of a group first)
    });
    seen.erase(std::unique(seen.begin(), seen.end(), [](const Seen& a, const Seen& b) { return a.from == b.from && a.to == b.to && a.group == b.group; }), seen.end());
    std::set<std::pair<std::uint32_t, std::uint32_t>> fans;
    for (std::size_t i = 0; i < seen.size();) {
        const std::uint32_t from = seen[i].from, to = seen[i].to;
        std::uint64_t weight = 0;
        std::size_t j = i;
        for (; j < seen.size() && seen[j].from == from && seen[j].to == to; ++j) {
            fans.emplace(to, seen[j].group);
            weight += seen[j].amount;
        }
        const auto capped = static_cast<std::uint32_t>(std::min<std::uint64_t>(weight, 0xFFFFFFFFu));
        graph.modules[from].out.push_back({to, capped});
        graph.modules[to].in.push_back({from, capped});
        if (width != Width::Files) graph.modules[to].fan_in = static_cast<std::uint32_t>(std::min<std::uint64_t>(std::uint64_t{graph.modules[to].fan_in} + weight, 0xFFFFFFFFu));
        i = j;
    }
    if (width == Width::Files)
        for (const auto& [module, group] : fans) ++graph.modules[module].fan_in;
    return graph;
}

namespace {

std::vector<FlowBand> bands_of(const Graph& graph, const std::vector<ModuleEdge>& edges, std::uint64_t& total) {
    std::map<Pile, FlowBand> by_pile;
    for (const ModuleEdge& e : edges) {
        const Module& other = graph.modules[e.module];
        auto [it, fresh] = by_pile.try_emplace(other.pile);
        FlowBand& band = it->second;
        band.pile = other.pile;
        band.weight += e.weight;
        ++band.modules;
        if (fresh || other.fan_in > graph.modules[band.top].fan_in) band.top = e.module;
        total += e.weight;
    }
    std::vector<FlowBand> out;
    out.reserve(by_pile.size());
    for (const auto& [pile, band] : by_pile) out.push_back(band);
    std::stable_sort(out.begin(), out.end(), [](const FlowBand& a, const FlowBand& b) { return a.weight > b.weight; });
    return out;
}

// The modules at each distance 1..3 from `focus` along in-edges or out-edges.
std::array<std::vector<std::uint32_t>, 3> rings(const Graph& graph, std::uint32_t focus, bool callers) {
    std::array<std::vector<std::uint32_t>, 3> out;
    std::unordered_set<std::uint32_t> seen{focus};
    std::vector<std::uint32_t> frontier{focus};
    for (std::size_t hop = 0; hop < 3; ++hop) {
        for (const std::uint32_t m : frontier)
            for (const ModuleEdge& e : callers ? graph.modules[m].in : graph.modules[m].out)
                if (seen.insert(e.module).second) out[hop].push_back(e.module);
        frontier = out[hop];
    }
    return out;
}

// From the focus, always on to the neighbour most files link to, as long as it is one hop further. A neighbour nothing links to has no weight to choose it by:
// the path ends there rather than pick one of many equals.
std::vector<std::uint32_t> chain(const Graph& graph, std::uint32_t focus, const std::array<std::vector<std::uint32_t>, 3>& ring, bool callers) {
    std::vector<std::uint32_t> path;
    std::uint32_t here = focus;
    for (std::size_t hop = 0; hop < 3; ++hop) {
        const std::unordered_set<std::uint32_t> next(ring[hop].begin(), ring[hop].end());
        std::optional<std::uint32_t> best;
        for (const ModuleEdge& e : callers ? graph.modules[here].in : graph.modules[here].out) {
            if (!next.contains(e.module)) continue;
            if (!best || graph.modules[e.module].fan_in > graph.modules[*best].fan_in ||
                (graph.modules[e.module].fan_in == graph.modules[*best].fan_in && e.module < *best))
                best = e.module;
        }
        if (!best || graph.modules[*best].fan_in == 0) break;
        path.push_back(*best);
        here = *best;
    }
    return path;
}

}  // namespace

Flow compute_flow(const Graph& graph, std::uint32_t focus) {
    Flow flow;
    flow.focus = focus;
    flow.in = bands_of(graph, graph.modules[focus].in, flow.in_total);
    flow.out = bands_of(graph, graph.modules[focus].out, flow.out_total);
    return flow;
}

LibraryView compute_library(const Graph& graph, const Index& index, std::uint32_t focus, Width width, std::string_view function) {
    LibraryView view;
    view.focus = focus;
    view.function = std::string(function);
    // ---- every function of every copy of the module, and who uses it. A file counts once however many copies of it there are.
    struct Gathered {
        std::string label;
        std::map<std::pair<std::uint32_t, std::uint32_t>, std::uint64_t> by_user;   // (user module, user's bytes): call sites
    };
    std::map<std::string, Gathered> functions;
    for (std::uint32_t card = 0; card < graph.module_of_card.size(); ++card) {
        if (graph.module_of_card[card] != focus) continue;
        const std::span<const ExportedSymbol> own = index.exports_of(card);
        for (std::size_t i = 0; i < own.size(); ++i) {
            std::string key = own[i].name.empty() ? "#" + std::to_string(own[i].ordinal) : text::lower(own[i].name);
            Gathered& g = functions[std::move(key)];
            if (g.label.empty() || g.label.front() == '#') g.label = own[i].name.empty() ? "#" + std::to_string(own[i].ordinal) : own[i].name;
            for (const ExportUse& use : index.uses_of(index.first_export(card) + static_cast<std::uint32_t>(i))) {
                const std::uint32_t user = graph.module_of_card[use.from];
                if (user == focus) continue;
                std::uint64_t& calls = g.by_user[{user, index.nodes[use.from].group}];
                calls = std::max<std::uint64_t>(calls, use.calls);
            }
        }
    }
    const auto users_of = [](const Gathered& g) {
        std::map<std::uint32_t, std::uint64_t> sum;
        for (const auto& [who, calls] : g.by_user) sum[who.first] += calls;
        std::vector<LibraryUser> users;
        users.reserve(sum.size());
        for (const auto& [module, calls] : sum) users.push_back({module, calls});
        std::stable_sort(users.begin(), users.end(), [](const LibraryUser& a, const LibraryUser& b) { return a.calls > b.calls; });
        return users;
    };
    const auto heaviest = [](std::vector<LibraryChannel>& side, std::uint64_t& total) {
        std::stable_sort(side.begin(), side.end(), [](const LibraryChannel& a, const LibraryChannel& b) { return a.weight != b.weight ? a.weight > b.weight : a.label < b.label; });
        for (const LibraryChannel& c : side) total += c.weight;
    };
    if (!function.empty()) {
        const auto it = functions.find(std::string(function));
        if (it != functions.end()) {
            for (const LibraryUser& u : users_of(it->second)) {
                LibraryChannel c;
                c.label = graph.modules[u.module].name;
                c.module = u.module;
                c.calls = u.calls;
                c.weight = width == Width::Calls ? u.calls : 1;
                view.left.push_back(std::move(c));
            }
        }
        LibraryChannel owner;
        owner.label = graph.modules[focus].name;
        owner.module = focus;
        for (const LibraryChannel& c : view.left) {
            owner.weight += c.weight;
            owner.calls += c.calls;
        }
        view.right.push_back(std::move(owner));
    } else {
        for (const ModuleEdge& e : graph.modules[focus].out) {
            LibraryChannel c;
            c.label = graph.modules[e.module].name;
            c.module = e.module;
            c.weight = e.weight;
            view.left.push_back(std::move(c));
        }
        for (const auto& [key, g] : functions) {
            LibraryChannel c;
            c.label = g.label;
            c.key = key;
            c.users = users_of(g);
            for (const LibraryUser& u : c.users) c.calls += u.calls;
            c.weight = width == Width::Calls ? c.calls : c.users.size();
            view.right.push_back(std::move(c));
        }
    }
    heaviest(view.left, view.left_total);
    heaviest(view.right, view.right_total);
    return view;
}

Trace compute_trace(const Graph& graph, std::uint32_t focus, std::size_t per_column) {
    Trace trace;
    trace.focus = focus;
    trace.columns[kTraceCenter] = {focus};
    const auto callers = rings(graph, focus, true), callees = rings(graph, focus, false);
    const std::vector<std::uint32_t> up = chain(graph, focus, callers, true), down = chain(graph, focus, callees, false);
    trace.heaviest.assign(up.rbegin(), up.rend());
    trace.heaviest.push_back(focus);
    trace.heaviest.insert(trace.heaviest.end(), down.begin(), down.end());
    std::unordered_set<std::uint32_t> on_path(trace.heaviest.begin(), trace.heaviest.end());
    const auto fill = [&](std::size_t column, std::vector<std::uint32_t> modules) {
        std::sort(modules.begin(), modules.end(), [&](std::uint32_t a, std::uint32_t b) {
            const bool pa = on_path.contains(a), pb = on_path.contains(b);
            if (pa != pb) return pa;
            if (graph.modules[a].fan_in != graph.modules[b].fan_in) return graph.modules[a].fan_in > graph.modules[b].fan_in;
            return a < b;
        });
        if (modules.size() > per_column) {
            trace.hidden[column] = static_cast<std::uint32_t>(modules.size() - per_column);
            modules.resize(per_column);
        }
        trace.columns[column] = std::move(modules);
    };
    for (std::size_t hop = 0; hop < 3; ++hop) {
        fill(kTraceCenter - hop - 1, callers[hop]);
        fill(kTraceCenter + hop + 1, callees[hop]);
    }
    std::set<std::pair<std::uint32_t, std::uint32_t>> path_steps;
    for (std::size_t i = 0; i + 1 < trace.heaviest.size(); ++i) path_steps.emplace(trace.heaviest[i], trace.heaviest[i + 1]);
    for (std::size_t column = 0; column + 1 < kTraceColumns; ++column) {
        std::unordered_map<std::uint32_t, std::uint32_t> right;
        for (std::uint32_t p = 0; p < trace.columns[column + 1].size(); ++p) right.emplace(trace.columns[column + 1][p], p);
        for (std::uint32_t p = 0; p < trace.columns[column].size(); ++p) {
            const std::uint32_t from = trace.columns[column][p];
            for (const ModuleEdge& e : graph.modules[from].out) {
                const auto it = right.find(e.module);
                if (it != right.end()) trace.links.push_back({static_cast<std::uint8_t>(column), p, it->second, e.weight, path_steps.contains({from, e.module})});
            }
        }
    }
    return trace;
}

}  // namespace cb::app::board
