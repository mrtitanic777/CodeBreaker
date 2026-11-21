// app/board_inspect.cpp -- see board_inspect.hpp.
#include "app/board_inspect.hpp"

#include <algorithm>
#include <array>
#include <map>
#include <tuple>

#include "core/text.hpp"

namespace cb::app::board {

namespace {

// One linked file, across all of its links to or from this one.
struct Linked {
    std::uint32_t card = 0;
    std::array<bool, 3> kinds{};
    std::uint32_t calls = 0, functions = 0;
    Link strongest() const { return kinds[0] ? Link::Imports : kinds[1] ? Link::Installs : Link::References; }
};

void take(std::map<std::uint32_t, Linked>& into, const Index& index, std::uint32_t other, const Edge& e) {
    auto [it, fresh] = into.try_emplace(index.nodes[other].group);
    Linked& l = it->second;
    if (fresh) l.card = other;
    l.kinds[static_cast<std::size_t>(e.link)] = true;
    l.calls = std::max(l.calls, e.calls);       // (copies of one file: the larger, not the sum)
    l.functions = std::max(l.functions, e.matched);
}

std::string counted(std::uint64_t n, const char* one, const char* many) { return count_words(n) + " " + (n == 1 ? one : many); }

void add_links(Inspector& out, const Board& board, const std::map<std::uint32_t, Linked>& links, const char* title, const char* import_word) {
    std::array<std::uint32_t, 3> by_kind{};
    std::vector<const Linked*> sorted;
    sorted.reserve(links.size());
    for (const auto& [group, l] : links) {
        sorted.push_back(&l);
        for (std::size_t k = 0; k < 3; ++k) by_kind[k] += l.kinds[k] ? 1 : 0;
    }
    std::string summary;
    const auto part = [&](std::uint32_t n, const char* word) {
        if (n > 0) summary += (summary.empty() ? "" : ", ") + count_words(n) + " " + word;
    };
    part(by_kind[0], import_word);
    part(by_kind[1], "install");
    part(by_kind[2], "mention");
    out.rows.push_back({InspectorRow::Kind::Heading, std::string(title) + " (" + count_words(links.size()) + ")", summary});
    std::sort(sorted.begin(), sorted.end(), [&](const Linked* a, const Linked* b) {
        if (a->strongest() != b->strongest()) return a->strongest() < b->strongest();
        if (a->calls != b->calls) return a->calls > b->calls;
        return text::lower(board.cards[a->card].name) < text::lower(board.cards[b->card].name);
    });
    for (const Linked* l : sorted) {
        std::string detail(link_name(l->strongest()));
        if (l->calls > 0) detail += " \xC2\xB7 " + counted(l->calls, "call site", "call sites");
        InspectorRow row{InspectorRow::Kind::File, board.cards[l->card].name, std::move(detail)};
        row.card = l->card;
        out.rows.push_back(std::move(row));
    }
}

}  // namespace

Inspector build_inspector(const Board& board, const Index& index, const Signals* signals, std::uint32_t card) {
    Inspector out;
    out.card = card;
    const Card& c = board.cards[card];
    const Node& node = index.nodes[card];
    std::vector<std::uint32_t> copies;
    for (std::uint32_t i = 0; i < index.nodes.size(); ++i)
        if (i == card || (!node.sha256.empty() && index.nodes[i].group == node.group)) copies.push_back(i);
    out.copies = static_cast<std::uint32_t>(copies.size());

    const auto fact = [&](const char* name, std::string value) {
        if (!value.empty()) out.rows.push_back({InspectorRow::Kind::Fact, name, std::move(value)});
    };
    out.rows.push_back({InspectorRow::Kind::Heading, c.name, std::string(pile_name(c.pile))});
    fact("Conversion", std::string(conv_words(c.conv)));
    fact("Format", c.format);
    fact("Size", size_words(c.size));
    fact("Subsystem", node.subsystem);
    if (node.exports > 0) fact("Exports", count_words(node.exports));
    if (node.unresolved > 0) fact("Names not on the board", count_words(node.unresolved));
    fact("SHA-256", node.sha256.empty() ? std::string("unreadable") : node.sha256.substr(0, 16) + "\xE2\x80\xA6");
    fact("Where", c.rel);

    std::map<std::uint32_t, Linked> inbound, outbound;
    for (const std::uint32_t copy : copies) {
        for (const std::uint32_t at : index.into(copy)) {
            const Edge& e = index.edges[at];
            if (index.nodes[e.from].group != node.group) take(inbound, index, e.from, e);
        }
        for (const Edge& e : index.out_of(copy))
            if (index.nodes[e.to].group != node.group) take(outbound, index, e.to, e);
    }
    out.inbound = static_cast<std::uint32_t>(inbound.size());
    out.outbound = static_cast<std::uint32_t>(outbound.size());
    add_links(out, board, inbound, "Inbound", "import");
    add_links(out, board, outbound, "Outbound", "import");

    if (signals != nullptr) {
        std::map<std::pair<std::uint32_t, Direction>, std::uint8_t> wired;
        for (const std::uint32_t copy : copies)
            for (const NodeSignal& l : signals->of_node(copy)) {
                std::uint8_t& confidence = wired[{l.signal, l.direction}];
                confidence = std::max(confidence, l.confidence);
            }
        out.signals = static_cast<std::uint32_t>(wired.size());
        out.rows.push_back({InspectorRow::Kind::Heading, "Signals (" + count_words(wired.size()) + ")", {}});
        std::vector<std::pair<std::pair<std::uint32_t, Direction>, std::uint8_t>> sorted(wired.begin(), wired.end());
        std::stable_sort(sorted.begin(), sorted.end(), [&](const auto& a, const auto& b) {
            const SignalType ta = signals->signals[a.first.first].type, tb = signals->signals[b.first.first].type;
            return ta != tb ? ta < tb : a.second > b.second;
        });
        for (const auto& [key, confidence] : sorted) {
            const Signal& s = signals->signals[key.first];
            InspectorRow row{InspectorRow::Kind::Signal, s.value, std::string(signal_type_name(s.type)) + " \xC2\xB7 " + std::string(direction_name(key.second)) + " \xC2\xB7 " + std::to_string(confidence) + "%"};
            row.signal = key.first;
            out.rows.push_back(std::move(row));
        }
    }
    if (copies.size() > 1) {
        out.rows.push_back({InspectorRow::Kind::Heading, "Copies (" + count_words(copies.size()) + ")", "identical bytes"});
        for (const std::uint32_t copy : copies) {
            InspectorRow row{InspectorRow::Kind::File, board.cards[copy].rel, board.cards[copy].from.empty() ? std::string("on the disc") : std::string("in a cabinet")};
            row.card = copy;
            out.rows.push_back(std::move(row));
        }
    }
    return out;
}

HoverLinks hover_links(const Index& index, std::uint32_t card) {
    HoverLinks out;
    out.card = card;
    const std::size_t n = index.nodes.size();
    out.role.assign(n, 0);
    const Node& node = index.nodes[card];
    std::vector<std::uint8_t> by_group(n, 0);
    for (std::uint32_t i = 0; i < n; ++i) {
        if (i != card && (node.sha256.empty() || index.nodes[i].group != node.group)) continue;
        for (const Edge& e : index.out_of(i)) by_group[index.nodes[e.to].group] |= 1;
        for (const std::uint32_t at : index.into(i)) by_group[index.nodes[index.edges[at].from].group] |= 2;
    }
    by_group[node.group] = 0;   // (its own copies are not links)
    for (std::uint32_t g = 0; g < n; ++g) {
        out.uses += by_group[g] & 1;
        out.used_by += (by_group[g] >> 1) & 1;
    }
    for (std::uint32_t i = 0; i < n; ++i) out.role[i] = by_group[index.nodes[i].group];
    return out;
}

}  // namespace cb::app::board
