// app/board_automation.cpp -- see board_automation.hpp.
#include "app/board_automation.hpp"

#include <algorithm>
#include <format>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>

#include "core/fs.hpp"
#include "core/text.hpp"

namespace cb::app::board {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kDecksHeader = "#codebreaker-board-decks 1";

std::string one_field(std::string_view s) {
    std::string out(s);
    std::replace_if(out.begin(), out.end(), [](char c) { return c == '\t' || c == '\n' || c == '\r'; }, ' ');
    return out;
}

std::vector<std::string_view> tabs(std::string_view line) {
    std::vector<std::string_view> out;
    std::size_t at = 0;
    while (true) {
        const std::size_t tab = line.find('\t', at);
        out.push_back(line.substr(at, tab == std::string_view::npos ? std::string_view::npos : tab - at));
        if (tab == std::string_view::npos) return out;
        at = tab + 1;
    }
}

std::string json_text(std::string_view s) {
    std::string out = "\"";
    for (const char ch : s) {
        const auto c = static_cast<unsigned char>(ch);
        if (ch == '"') out += "\\\"";
        else if (ch == '\\') out += "\\\\";
        else if (c < 0x20 || c >= 0x7F) out += std::format("\\u{:04x}", static_cast<unsigned>(c));   // (names on these discs are bytes, not always UTF-8)
        else out += ch;
    }
    return out + "\"";
}

std::string xml_text(std::string_view s) {
    std::string out;
    for (const char ch : s) {
        const auto c = static_cast<unsigned char>(ch);
        if (ch == '&') out += "&amp;";
        else if (ch == '<') out += "&lt;";
        else if (ch == '>') out += "&gt;";
        else if (ch == '"') out += "&quot;";
        else if (c < 0x20 || c >= 0x7F) out += '?';
        else out += ch;
    }
    return out;
}

std::string csv_text(std::string_view s) {
    std::string out = "\"";
    for (const char ch : s) {
        if (ch == '"') out += "\"\"";
        else out += ch;
    }
    return out + "\"";
}

std::string link_type(Link link) {
    std::string name = text::upper(link_name(link));
    return name;
}

std::string number_text(double v) { return std::format("{:.0f}", v); }

// The cards of each module of a cluster.
std::map<std::uint32_t, std::vector<std::uint32_t>> cards_of(const Graph& graph, const Cluster& cluster) {
    std::map<std::uint32_t, std::vector<std::uint32_t>> out;
    for (const std::uint32_t m : cluster.modules) out[m];
    for (std::uint32_t card = 0; card < graph.module_of_card.size(); ++card) {
        const auto it = out.find(graph.module_of_card[card]);
        if (it != out.end()) it->second.push_back(card);
    }
    return out;
}

}  // namespace

Orphans find_orphans(const Board& board, const Index& index, const Signals* signals) {
    Orphans out;
    const std::size_t n = board.cards.size();
    std::vector<char> reached(n, 0);   // per group
    const auto counts = [](Pile pile) { return pile == Pile::Setup || pile == Pile::Programs || pile == Pile::Libraries || pile == Pile::Drivers; };
    for (const Edge& e : index.edges) {
        const std::uint32_t from = index.nodes[e.from].group, to = index.nodes[e.to].group;
        if (from != to && counts(board.cards[e.from].pile)) reached[to] = 1;
    }
    if (signals != nullptr)
        for (const NodeSignal& l : signals->links)
            if (signals->signals[l.signal].type == SignalType::Autostart && l.direction == Direction::Registers) reached[index.nodes[l.node].group] = 1;
    out.card.assign(n, 0);
    for (std::uint32_t i = 0; i < n; ++i) {
        const std::uint32_t group = index.nodes[i].group;
        out.card[i] = reached[group] ? 0 : 1;
        if (!reached[group] && group == i) ++out.distinct;
    }
    return out;
}

std::vector<Deck> builtin_decks() {
    std::vector<Deck> decks(3);
    decks[0].name = "DirectX / Media Stack";
    decks[0].names = {"ddraw", "dsound", "d3d", "dinput", "dplay", "ddhelp", "dxsetup", "directx", "msacm", "winmm", "mciavi", "mciseq", "mciwave", "avifil", "msvfw", "quartz", "amovie", "actmovie", "mmsystem", "wavemix"};
    decks[1].name = "Dial-Up Networking Stack";
    decks[1].names = {"rasapi", "rnaui", "rnaapp", "rnathunk", "rnasetup", "rnaplus", "pppmac", "tapi", "unimodem", "umdm", "modemui", "wsock", "winsock", "vtcp", "vip.386", "vdhcp", "dialer", "mdmcfg"};
    decks[2].name = "Plug and Play Boot Chain";
    decks[2].names = {"configmg", "vmm32", "ios.vxd", "ios.ini", "bios.vxd", "isapnp", "pci.vxd", "vxdldr", "ntkern", "vpowerd", "pcmcia", "esdi_506", "scsiport", "io.sys", "msdos.sys", "command.com", "setupx", "sysdm", "devcfg", "biosxlat"};
    for (Deck& d : decks) d.builtin = true;
    return decks;
}

void save_decks(const stdfs::path& root, const std::vector<Deck>& decks) {
    std::string out(kDecksHeader);
    out += '\n';
    for (const Deck& d : decks) {
        if (d.builtin) continue;
        out += "deck\t" + one_field(d.name) + '\t' + one_field(d.query) + '\t' + (d.orphans ? "1" : "0") + '\n';
        for (const std::string& name : d.names) out += "name\t" + one_field(name) + '\n';
        for (const DeckFilter& f : d.filters) out += "filter\t" + std::to_string(static_cast<int>(f.type)) + '\t' + one_field(f.value) + '\n';
    }
    fs::make_dirs(root);
    fs::write_file_atomic(root / "decks.tsv", as_bytes(out));
}

std::vector<Deck> load_decks(const stdfs::path& root) {
    std::vector<Deck> decks;
    std::error_code ec;
    if (!stdfs::is_regular_file(root / "decks.tsv", ec)) return decks;
    const std::string all = fs::read_text(root / "decks.tsv");
    if (!all.starts_with(kDecksHeader)) return decks;
    std::size_t at = 0;
    while (at < all.size()) {
        std::size_t end = all.find('\n', at);
        if (end == std::string::npos) end = all.size();
        std::string_view line(all.data() + at, end - at);
        at = end + 1;
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        const std::vector<std::string_view> p = tabs(line);
        if (p[0] == "deck" && p.size() >= 4) {
            Deck d;
            d.name = std::string(p[1]);
            d.query = std::string(p[2]);
            d.orphans = p[3] == "1";
            decks.push_back(std::move(d));
        } else if (p[0] == "name" && p.size() >= 2 && !decks.empty()) {
            decks.back().names.emplace_back(p[1]);
        } else if (p[0] == "filter" && p.size() >= 3 && !decks.empty()) {
            int type = 0;
            for (const char c : p[1]) type = c >= '0' && c <= '9' ? type * 10 + (c - '0') : type;
            if (type < static_cast<int>(kSignalTypes)) decks.back().filters.push_back({static_cast<SignalType>(type), std::string(p[2])});
        }
    }
    return decks;
}

std::vector<char> cards_named(const Board& board, const std::vector<std::string>& names) {
    std::vector<char> out(board.cards.size(), 0);
    for (std::size_t i = 0; i < board.cards.size(); ++i) {
        const std::string name = text::lower(board.cards[i].name);
        for (const std::string& wanted : names) {
            if (name.find(wanted) != std::string::npos) {
                out[i] = 1;
                break;
            }
        }
    }
    return out;
}

std::vector<std::uint32_t> batch_order(const Board& board, const Index& index, const Stacks& stacks) {
    std::vector<std::uint32_t> cards;
    for (const Stack& stack : stacks) {
        for (const std::uint32_t i : stack.todo) {
            const Card& c = board.cards[i];
            if ((c.format == "PE" || c.format == "NE" || c.format == "LE") && (c.conv == Conv::None || c.conv == Conv::Cancelled || c.conv == Conv::Failed)) cards.push_back(i);
        }
    }
    std::vector<std::uint32_t> inbound(board.cards.size(), 0);
    for (const std::uint32_t i : cards) inbound[i] = static_cast<std::uint32_t>(index.into(i).size());
    std::stable_sort(cards.begin(), cards.end(), [&](std::uint32_t a, std::uint32_t b) { return inbound[a] > inbound[b]; });
    return cards;
}

Cluster build_cluster(const Graph& graph, const Index& index, const Projection* projection, std::vector<std::uint32_t> modules) {
    Cluster cluster;
    std::sort(modules.begin(), modules.end());
    modules.erase(std::unique(modules.begin(), modules.end()), modules.end());
    cluster.modules = std::move(modules);
    std::unordered_map<std::uint32_t, std::uint32_t> position;
    position.reserve(cluster.modules.size());
    for (std::uint32_t i = 0; i < cluster.modules.size(); ++i) position.emplace(cluster.modules[i], i);
    struct Amount {
        std::uint8_t confidence = 0;
        std::uint32_t links = 0, calls = 0;
    };
    std::map<std::tuple<std::uint32_t, std::uint32_t, Link>, std::map<std::uint32_t, Amount>> by_source;   // per source's bytes: a file counts once
    for (const Edge& e : index.edges) {
        const auto from = position.find(graph.module_of_card[e.from]), to = position.find(graph.module_of_card[e.to]);
        if (from == position.end() || to == position.end() || from->second == to->second) continue;
        Amount& a = by_source[{from->second, to->second, e.link}][index.nodes[e.from].group];
        a.confidence = std::max(a.confidence, e.confidence);
        a.links = std::max(a.links, e.matched);
        a.calls = std::max(a.calls, e.calls);
    }
    cluster.edges.reserve(by_source.size());
    for (const auto& [key, sources] : by_source) {
        ClusterEdge edge;
        edge.from = std::get<0>(key);
        edge.to = std::get<1>(key);
        edge.link = std::get<2>(key);
        edge.files = static_cast<std::uint32_t>(sources.size());
        for (const auto& [group, a] : sources) {
            edge.confidence = std::max(edge.confidence, a.confidence);
            edge.links += a.links;
            edge.calls += a.calls;
        }
        if (projection != nullptr && edge.link == Link::Imports) edge.projected = projection->channel(cluster.modules[edge.from], cluster.modules[edge.to]);
        cluster.edges.push_back(edge);
    }
    return cluster;
}

std::string cluster_json(const Board& board, const Index& index, const Signals* signals, const Graph& graph, const Cluster& cluster) {
    const auto cards = cards_of(graph, cluster);
    std::string out = "{\n  \"schema\": \"codebreaker-board-graph-1\",\n  \"source\": " + json_text(board.source) +
                      ",\n  \"note\": \"Links are static. projected_calls_per_hour is a model, not a measurement.\",\n  \"nodes\": [\n";
    for (std::size_t i = 0; i < cluster.modules.size(); ++i) {
        const Module& m = graph.modules[cluster.modules[i]];
        const Card& c = board.cards[m.card];
        std::set<std::string> sources;
        for (const std::uint32_t card : cards.at(cluster.modules[i]))
            if (!board.cards[card].from.empty()) sources.insert(board.cards[card].from);
        std::string cabs;
        for (const std::string& s : sources) cabs += (cabs.empty() ? "" : ", ") + json_text(s);
        out += "    {\"id\": " + std::to_string(i) + ", \"sha256\": " + json_text(index.nodes[m.card].sha256) + ", \"filename\": " + json_text(c.name) + ", \"category\": " + json_text(pile_name(c.pile)) +
               ", \"format\": " + json_text(c.format) + ", \"status\": " + json_text(conv_words(c.conv)) + ", \"copies\": " + std::to_string(cards.at(cluster.modules[i]).size()) + ", \"path\": " + json_text(c.rel) +
               ", \"cab_sources\": [" + cabs + "]}" + (i + 1 < cluster.modules.size() ? "," : "") + "\n";
    }
    out += "  ],\n  \"edges\": [\n";
    for (std::size_t i = 0; i < cluster.edges.size(); ++i) {
        const ClusterEdge& e = cluster.edges[i];
        out += "    {\"source_id\": " + std::to_string(e.from) + ", \"target_id\": " + std::to_string(e.to) + ", \"link_type\": " + json_text(link_type(e.link)) + ", \"confidence\": " + std::to_string(e.confidence) +
               ", \"files\": " + std::to_string(e.files) + ", \"export_import_links\": " + std::to_string(e.links) + ", \"call_sites\": " + std::to_string(e.calls) + ", \"projected_calls_per_hour\": " + number_text(e.projected) + "}" +
               (i + 1 < cluster.edges.size() ? "," : "") + "\n";
    }
    out += "  ],\n  \"signals\": [\n";
    std::string node_signals;
    if (signals != nullptr) {
        std::map<std::uint32_t, std::uint32_t> ids;   // signal to its id in this file
        std::set<std::tuple<std::uint32_t, std::uint32_t, Direction>> seen;
        std::map<std::tuple<std::uint32_t, std::uint32_t, Direction>, std::uint8_t> wired;
        for (std::uint32_t i = 0; i < cluster.modules.size(); ++i) {
            for (const std::uint32_t card : cards.at(cluster.modules[i])) {
                for (const NodeSignal& l : signals->of_node(card)) {
                    const auto [id, fresh] = ids.try_emplace(l.signal, static_cast<std::uint32_t>(ids.size()));
                    (void)fresh;
                    std::uint8_t& confidence = wired[{i, id->second, l.direction}];
                    confidence = std::max(confidence, l.confidence);
                }
            }
        }
        std::vector<std::uint32_t> by_id(ids.size());
        for (const auto& [signal, id] : ids) by_id[id] = signal;
        for (std::size_t i = 0; i < by_id.size(); ++i) {
            const Signal& s = signals->signals[by_id[i]];
            out += "    {\"id\": " + std::to_string(i) + ", \"signal_type\": " + json_text(signal_type_name(s.type)) + ", \"value\": " + json_text(s.value) + "}" + (i + 1 < by_id.size() ? "," : "") + "\n";
        }
        std::size_t left = wired.size();
        for (const auto& [key, confidence] : wired) {
            node_signals += "    {\"node_id\": " + std::to_string(std::get<0>(key)) + ", \"signal_id\": " + std::to_string(std::get<1>(key)) + ", \"direction\": " + json_text(text::upper(direction_name(std::get<2>(key)))) +
                            ", \"confidence\": " + std::to_string(confidence) + "}" + (--left > 0 ? "," : "") + "\n";
        }
    }
    out += "  ],\n  \"node_signals\": [\n" + node_signals + "  ]\n}\n";
    return out;
}

std::string cluster_graphml(const Board& board, const Index& index, const Graph& graph, const Cluster& cluster) {
    std::string out =
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<graphml xmlns=\"http://graphml.graphdrawing.org/xmlns\">\n"
        "  <key id=\"filename\" for=\"node\" attr.name=\"filename\" attr.type=\"string\"/>\n  <key id=\"category\" for=\"node\" attr.name=\"category\" attr.type=\"string\"/>\n"
        "  <key id=\"format\" for=\"node\" attr.name=\"format\" attr.type=\"string\"/>\n  <key id=\"status\" for=\"node\" attr.name=\"status\" attr.type=\"string\"/>\n"
        "  <key id=\"sha256\" for=\"node\" attr.name=\"sha256\" attr.type=\"string\"/>\n  <key id=\"link_type\" for=\"edge\" attr.name=\"link_type\" attr.type=\"string\"/>\n"
        "  <key id=\"confidence\" for=\"edge\" attr.name=\"confidence\" attr.type=\"int\"/>\n  <key id=\"files\" for=\"edge\" attr.name=\"files\" attr.type=\"int\"/>\n"
        "  <key id=\"links\" for=\"edge\" attr.name=\"export_import_links\" attr.type=\"long\"/>\n  <key id=\"calls\" for=\"edge\" attr.name=\"call_sites\" attr.type=\"long\"/>\n"
        "  <key id=\"projected\" for=\"edge\" attr.name=\"projected_calls_per_hour\" attr.type=\"double\"/>\n  <graph id=\"codebreaker\" edgedefault=\"directed\">\n";
    const auto data = [](std::string_view key, std::string_view value) { return "<data key=\"" + std::string(key) + "\">" + xml_text(value) + "</data>"; };
    for (std::size_t i = 0; i < cluster.modules.size(); ++i) {
        const Module& m = graph.modules[cluster.modules[i]];
        const Card& c = board.cards[m.card];
        out += "    <node id=\"n" + std::to_string(i) + "\">" + data("filename", c.name) + data("category", pile_name(c.pile)) + data("format", c.format) + data("status", conv_words(c.conv)) + data("sha256", index.nodes[m.card].sha256) + "</node>\n";
    }
    for (std::size_t i = 0; i < cluster.edges.size(); ++i) {
        const ClusterEdge& e = cluster.edges[i];
        out += "    <edge id=\"e" + std::to_string(i) + "\" source=\"n" + std::to_string(e.from) + "\" target=\"n" + std::to_string(e.to) + "\">" + data("link_type", link_type(e.link)) + data("confidence", std::to_string(e.confidence)) +
               data("files", std::to_string(e.files)) + data("links", std::to_string(e.links)) + data("calls", std::to_string(e.calls)) + data("projected", number_text(e.projected)) + "</edge>\n";
    }
    return out + "  </graph>\n</graphml>\n";
}

std::string cluster_table(const Board& board, const Graph& graph, const Cluster& cluster) {
    std::string out = "source_entity_type,source,source_category,link_type,confidence,files,export_import_links,call_sites,projected_calls_per_hour,target_entity_type,target,target_category\r\n";
    const auto name = [&](std::uint32_t at) -> const Card& { return board.cards[graph.modules[cluster.modules[at]].card]; };
    for (const ClusterEdge& e : cluster.edges) {
        const Card& from = name(e.from);
        const Card& to = name(e.to);
        out += "maltego.Document," + csv_text(from.name) + "," + csv_text(pile_name(from.pile)) + "," + link_type(e.link) + "," + std::to_string(e.confidence) + "," + std::to_string(e.files) + "," + std::to_string(e.links) + "," +
               std::to_string(e.calls) + "," + number_text(e.projected) + ",maltego.Document," + csv_text(to.name) + "," + csv_text(pile_name(to.pile)) + "\r\n";
    }
    return out;
}

}  // namespace cb::app::board
