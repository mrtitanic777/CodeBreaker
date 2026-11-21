// app/board_index.cpp -- see board_index.hpp.
#include "app/board_index.hpp"

#include <algorithm>
#include <chrono>
#include <exception>
#include <map>
#include <set>
#include <tuple>
#include <variant>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/le.hpp"
#include "formats/ne.hpp"
#include "formats/pe.hpp"

namespace cb::app::board {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kIndexHeader = "#codebreaker-board-index 5";
constexpr std::string_view kModuleExtensions[] = {".dll", ".exe", ".drv", ".386", ".vxd", ".sys"};

bool name_char(std::uint8_t c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '-' || c == '$' || c == '~' || c == '!' || c == '.';
}

void take_token(std::string& token, const std::unordered_set<std::string>& known, std::set<std::string>& found) {
    while (!token.empty() && token.back() == '.') token.pop_back();
    if (token.size() >= 4 && token.find('.') != std::string::npos) {
        std::string lowered = text::lower(token);
        if (known.contains(lowered)) found.insert(std::move(lowered));
    }
    token.clear();
}

std::string parent_of(std::string_view rel) {
    const std::size_t slash = rel.find_last_of('/');
    return std::string(slash == std::string_view::npos ? std::string_view{} : rel.substr(0, slash));
}

std::size_t common_prefix(std::string_view a, std::string_view b) {
    std::size_t n = 0;
    while (n < a.size() && n < b.size() && a[n] == b[n]) ++n;
    return n;
}

std::vector<std::string_view> fields(std::string_view line, char separator) {
    std::vector<std::string_view> out;
    std::size_t at = 0;
    while (true) {
        const std::size_t end = line.find(separator, at);
        out.push_back(text::trim(line.substr(at, end == std::string_view::npos ? std::string_view::npos : end - at)));
        if (end == std::string_view::npos) return out;
        at = end + 1;
    }
}

std::uint64_t number(std::string_view s) {
    std::uint64_t v = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') break;
        v = v * 10 + static_cast<std::uint64_t>(c - '0');
    }
    return v;
}

// Whether the bytes of a file of this pile are worth searching for file names (pictures and sounds are not).
bool searched(Pile pile) { return pile != Pile::Media && pile != Pile::Fonts && pile != Pile::Cabinets; }

}  // namespace

std::string_view link_name(Link link) {
    switch (link) {
        case Link::Imports: return "imports";
        case Link::Installs: return "installs";
        default: return "references";
    }
}

void Index::finish() {
    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return std::tie(a.from, a.to, a.link) < std::tie(b.from, b.to, b.link);
    });
    const std::size_t n = nodes.size();
    out_start.assign(n + 1, 0);
    in_start.assign(n + 1, 0);
    for (const Edge& e : edges) {
        ++out_start[e.from + 1];
        ++in_start[e.to + 1];
    }
    for (std::size_t i = 0; i < n; ++i) {
        out_start[i + 1] += out_start[i];
        in_start[i + 1] += in_start[i];
    }
    in_edges.assign(edges.size(), 0);
    std::vector<std::uint32_t> next(in_start.begin(), in_start.end() - 1);
    for (std::size_t i = 0; i < edges.size(); ++i) in_edges[next[edges[i].to]++] = static_cast<std::uint32_t>(i);
    std::sort(exports.begin(), exports.end(), [](const ExportedSymbol& a, const ExportedSymbol& b) { return std::tie(a.card, a.ordinal) < std::tie(b.card, b.ordinal); });
    std::sort(export_uses.begin(), export_uses.end(), [](const ExportUse& a, const ExportUse& b) { return std::tie(a.symbol, a.from) < std::tie(b.symbol, b.from); });
    export_start.assign(n + 1, 0);
    for (const ExportedSymbol& e : exports) ++export_start[e.card + 1];
    for (std::size_t i = 0; i < n; ++i) export_start[i + 1] += export_start[i];
    use_start.assign(exports.size() + 1, 0);
    for (const ExportUse& u : export_uses) ++use_start[u.symbol + 1];
    for (std::size_t i = 0; i < exports.size(); ++i) use_start[i + 1] += use_start[i];
}

std::span<const ExportedSymbol> Index::exports_of(std::uint32_t card) const {
    return std::span<const ExportedSymbol>(exports).subspan(export_start[card], export_start[card + 1] - export_start[card]);
}

std::span<const ExportUse> Index::uses_of(std::uint32_t symbol) const {
    return std::span<const ExportUse>(export_uses).subspan(use_start[symbol], use_start[symbol + 1] - use_start[symbol]);
}

std::span<const Edge> Index::out_of(std::uint32_t card) const {
    return std::span<const Edge>(edges).subspan(out_start[card], out_start[card + 1] - out_start[card]);
}

std::span<const std::uint32_t> Index::into(std::uint32_t card) const {
    return std::span<const std::uint32_t>(in_edges).subspan(in_start[card], in_start[card + 1] - in_start[card]);
}

std::vector<std::uint32_t> pe_call_sites(std::span<const CodeSpan> code, const std::unordered_map<std::uint64_t, std::uint32_t>& slot_module, std::size_t modules) {
    std::vector<std::uint32_t> sites(modules, 0);
    const auto u32_at = [](ByteView b, std::size_t at) {
        return static_cast<std::uint32_t>(b[at]) | (static_cast<std::uint32_t>(b[at + 1]) << 8) | (static_cast<std::uint32_t>(b[at + 2]) << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24);
    };
    std::unordered_map<std::uint64_t, std::uint32_t> thunks;
    for (const CodeSpan& span : code)
        for (std::size_t i = 0; i + 6 <= span.bytes.size(); ++i)
            if (span.bytes[i] == 0xFF && span.bytes[i + 1] == 0x25)
                if (const auto hit = slot_module.find(u32_at(span.bytes, i + 2)); hit != slot_module.end()) thunks.emplace(span.va + i, hit->second);
    for (const CodeSpan& span : code) {
        const ByteView b = span.bytes;
        for (std::size_t i = 0; i + 5 <= b.size(); ++i) {
            const std::uint8_t op = b[i];
            if (op == 0xE8) {
                const auto rel = static_cast<std::int32_t>(u32_at(b, i + 1));
                const auto target = static_cast<std::uint64_t>(static_cast<std::int64_t>(span.va + i + 5) + rel);
                if (const auto hit = thunks.find(target); hit != thunks.end()) {
                    ++sites[hit->second];
                    i += 4;
                }
            } else if (op == 0xA1) {
                if (const auto hit = slot_module.find(u32_at(b, i + 1)); hit != slot_module.end()) {
                    ++sites[hit->second];
                    i += 4;
                }
            } else if (i + 6 <= b.size() && ((op == 0xFF && b[i + 1] == 0x15) || (op == 0x8B && (b[i + 1] & 0xC7) == 0x05))) {
                if (const auto hit = slot_module.find(u32_at(b, i + 2)); hit != slot_module.end()) {
                    ++sites[hit->second];
                    i += 5;
                }
            }
        }
    }
    return sites;
}

std::uint32_t ne_chain_sites(ByteView segment, std::uint16_t first) {
    std::uint32_t count = 0;
    std::uint32_t at = first;
    const std::uint32_t limit = static_cast<std::uint32_t>(segment.size() / 2 + 1);
    while (at != 0xFFFF && at + 2 <= segment.size() && count < limit) {
        ++count;
        at = static_cast<std::uint32_t>(segment[at]) | (static_cast<std::uint32_t>(segment[at + 1]) << 8);
    }
    return std::max<std::uint32_t>(count, 1);
}

BinaryFacts read_binary(ByteView data, std::string_view format) {
    BinaryFacts facts;
    std::map<std::string, std::uint32_t> seen;
    const auto add = [&](const std::string& module) -> std::optional<std::uint32_t> {
        if (module.empty() || module == "?") return std::nullopt;
        const auto [it, fresh] = seen.try_emplace(text::lower(module), static_cast<std::uint32_t>(facts.modules.size()));
        if (fresh) {
            facts.modules.push_back(module);
            facts.uses.emplace_back();
        }
        return it->second;
    };
    const auto exported = [&](const std::optional<std::string>& name, std::uint64_t ordinal) {
        const auto number = static_cast<std::uint32_t>(ordinal);
        std::string& kept = facts.export_list[number];
        if (name && !name->empty()) {
            facts.export_names[text::lower(*name)] = number;
            if (kept.empty()) kept = *name;
        }
        facts.export_names["#" + std::to_string(ordinal)] = number;
    };
    try {
        if (format == "PE") {
            const formats::pe::Image image = formats::pe::Image::parse(data);
            facts.subsystem = formats::pe::subsystem_label(image.optional_header().subsystem);
            const std::uint64_t base = image.optional_header().image_base;
            std::unordered_map<std::uint64_t, std::uint32_t> slots;     // an import slot, to its function below
            std::vector<std::pair<std::uint32_t, std::string>> functions;
            std::map<std::pair<std::uint32_t, std::string>, std::uint32_t> function_ids;
            // (the descriptors, not the address table: imports() merges slots, and loses a module whose slots another module's share)
            for (const bool delayed : {false, true}) {
                for (const formats::pe::ImportModule& m : image.import_table(delayed)) {
                    const auto at = add(m.module);
                    if (!at) continue;
                    for (const formats::pe::ImportSymbol& s : m.symbols) {
                        std::string symbol = s.name ? text::lower(*s.name) : "#" + std::to_string(s.ordinal.value_or(0));
                        const auto [id, fresh] = function_ids.try_emplace({*at, symbol}, static_cast<std::uint32_t>(functions.size()));
                        if (fresh) functions.emplace_back(*at, std::move(symbol));
                        slots.emplace(base + s.slot_rva, id->second);
                    }
                }
            }
            std::vector<CodeSpan> code;
            for (const auto& section : image.code_sections()) code.push_back({image.section_data(*section.section), section.va});
            const std::vector<std::uint32_t> sites = pe_call_sites(code, slots, functions.size());
            for (std::size_t i = 0; i < sites.size(); ++i) {
                facts.uses[functions[i].first].symbols[functions[i].second] += sites[i];
                facts.uses[functions[i].first].sites += sites[i];
            }
            const std::vector<formats::pe::Export> exports = image.exports();
            for (const formats::pe::Export& e : exports) exported(e.name, e.ordinal);
            facts.exports = static_cast<std::uint32_t>(exports.size());
        } else if (format == "NE") {
            const formats::ne::Module module = formats::ne::Module::open(Bytes(data.begin(), data.end()));
            const std::vector<std::string> refs = module.module_refs();
            for (const std::string& m : refs) add(m);
            const ByteView all = module.data();
            for (const formats::ne::Segment& segment : module.segments()) {
                if (segment.file_offset == 0 || segment.file_offset >= all.size()) continue;
                const ByteView bytes = all.subspan(segment.file_offset, std::min<std::uint64_t>(segment.length, all.size() - segment.file_offset));
                for (const formats::ne::Relocation& r : module.relocations(segment)) {
                    std::uint16_t ref = 0;
                    std::string symbol;
                    if (const auto* by_ordinal = std::get_if<formats::ne::ImportOrdinalRef>(&r.target)) {
                        ref = by_ordinal->module;
                        symbol = "#" + std::to_string(by_ordinal->ordinal);
                    } else if (const auto* by_name = std::get_if<formats::ne::ImportNameRef>(&r.target)) {
                        ref = by_name->module;
                        symbol = text::lower(module.imported_name(by_name->name_offset));
                    }
                    if (ref == 0 || ref > refs.size()) continue;
                    const auto at = add(refs[ref - 1u]);
                    if (!at) continue;
                    const std::uint32_t places = r.additive() ? 1 : ne_chain_sites(bytes, r.offset);
                    facts.uses[*at].symbols[symbol] += places;
                    facts.uses[*at].sites += places;
                }
            }
            for (const std::uint32_t ordinal : module.export_ordinals()) exported(std::nullopt, ordinal);
            for (const auto& table : {module.resident_names(), module.nonresident_names()})
                for (const formats::ne::Name& n : table)
                    if (n.ordinal != 0) exported(n.name, n.ordinal);
            facts.exports = static_cast<std::uint32_t>(module.export_ordinals().size());
        } else if (format == "LE") {
            const formats::le::Module module = formats::le::Module::from_buffer(Bytes(data.begin(), data.end()));
            const std::vector<std::string> refs = module.import_modules();
            for (const std::string& m : refs) add(m);
            if (!refs.empty()) {
                for (const auto& [place, fixup] : module.fixups()) {
                    const auto* import = std::get_if<formats::le::ImportFixup>(&fixup);
                    if (import == nullptr || import->module == 0 || import->module > refs.size()) continue;
                    const auto at = add(refs[import->module - 1u]);
                    if (!at) continue;
                    ++facts.uses[*at].symbols["#" + std::to_string(import->ordinal)];
                    ++facts.uses[*at].sites;
                }
            }
            const std::vector<formats::le::Export> exports = module.exports();
            for (const formats::le::Export& e : exports) exported(e.name, e.ordinal);
            facts.exports = static_cast<std::uint32_t>(exports.size());
        }
    } catch (const std::exception&) {
        facts.damaged = true;
    }
    return facts;
}

std::vector<std::string> inf_copied_files(std::string_view script) {
    std::map<std::string, std::vector<std::string>> sections;
    std::vector<std::string> wanted, out;
    std::vector<std::string>* current = nullptr;
    std::size_t at = 0;
    while (at < script.size()) {
        std::size_t end = script.find('\n', at);
        if (end == std::string_view::npos) end = script.size();
        std::string_view line = script.substr(at, end - at);
        at = end + 1;
        if (const std::size_t semi = line.find(';'); semi != std::string_view::npos) line = line.substr(0, semi);
        line = text::trim(line);
        if (line.empty()) continue;
        if (line.front() == '[') {
            const std::size_t close = line.find(']');
            current = close == std::string_view::npos ? nullptr : &sections[text::lower(text::trim(line.substr(1, close - 1)))];
            continue;
        }
        const std::size_t equals = line.find('=');
        if (equals != std::string_view::npos && text::lower(text::trim(line.substr(0, equals))) == "copyfiles") {
            for (const std::string_view item : fields(line.substr(equals + 1), ',')) {
                if (item.empty()) continue;
                if (item.front() == '@') out.emplace_back(text::trim(item.substr(1)));
                else wanted.push_back(text::lower(item));
            }
            continue;
        }
        if (current != nullptr) current->emplace_back(line);
    }
    for (const std::string& name : wanted) {
        const auto section = sections.find(name);
        if (section == sections.end()) continue;
        for (const std::string& entry : section->second) {
            const std::vector<std::string_view> parts = fields(entry, ',');
            for (std::size_t i = 0; i < std::min<std::size_t>(parts.size(), 2); ++i)
                if (!parts[i].empty() && parts[i].find('=') == std::string_view::npos) out.emplace_back(parts[i]);
        }
    }
    std::set<std::string> seen;
    std::vector<std::string> unique;
    for (std::string& name : out)
        if (seen.insert(text::lower(name)).second) unique.push_back(std::move(name));
    return unique;
}

std::vector<std::string> names_in_bytes(ByteView data, const std::unordered_set<std::string>& known) {
    std::set<std::string> found;
    std::string token;
    for (const std::uint8_t c : data) {
        if (name_char(c)) token += static_cast<char>(c);
        else if (!token.empty()) take_token(token, known, found);
    }
    if (!token.empty()) take_token(token, known, found);
    for (std::size_t phase = 0; phase < 2; ++phase) {   // UTF-16LE: a name's characters at every other byte, a zero between
        token.clear();
        for (std::size_t i = phase; i + 1 < data.size(); i += 2) {
            if (data[i + 1] == 0 && name_char(data[i])) token += static_cast<char>(data[i]);
            else if (!token.empty()) take_token(token, known, found);
        }
        if (!token.empty()) take_token(token, known, found);
    }
    return {found.begin(), found.end()};
}

NameTable::NameTable(const Board& board) : board_(board) {
    by_name_.reserve(board.cards.size());
    for (std::size_t i = 0; i < board.cards.size(); ++i) by_name_[text::lower(board.cards[i].name)].push_back(static_cast<std::uint32_t>(i));
    names_.reserve(by_name_.size());
    for (const auto& [name, cards] : by_name_) names_.insert(name);
}

std::optional<Resolved> NameTable::resolve(std::string_view name, std::uint32_t from, const std::function<bool(std::uint32_t, std::uint32_t)>& identical) const {
    std::string wanted = text::lower(name);
    if (const std::size_t slash = wanted.find_last_of("/\\"); slash != std::string::npos) wanted.erase(0, slash + 1);
    if (wanted == "kernel") wanted = "krnl386.exe";   // (Windows loads the module named KERNEL from this file)
    auto it = by_name_.find(wanted);
    if (it == by_name_.end() && wanted.find('.') == std::string::npos) {
        for (const std::string_view extension : kModuleExtensions) {
            it = by_name_.find(wanted + std::string(extension));
            if (it != by_name_.end()) break;
        }
    }
    if (it == by_name_.end()) return std::nullopt;
    std::vector<std::uint32_t> candidates;
    for (const std::uint32_t c : it->second)
        if (c != from) candidates.push_back(c);
    if (candidates.empty()) return std::nullopt;   // only itself: not a link
    const std::string& from_rel = board_.cards[from].rel;
    const std::string from_folder = parent_of(from_rel);
    std::uint32_t best = candidates.front();
    std::size_t best_shared = 0;
    bool same_folder = false, first = true;
    for (const std::uint32_t c : candidates) {   // the copy nearest in the tree; the first of equals (cards are sorted, so this is stable)
        const std::string& rel = board_.cards[c].rel;
        const bool here = parent_of(rel) == from_folder;
        const std::size_t shared = common_prefix(rel, from_rel);
        if (first || (here && !same_folder) || (here == same_folder && shared > best_shared)) {
            best = c;
            best_shared = shared;
            same_folder = here;
            first = false;
        }
    }
    bool all_same = true;
    for (const std::uint32_t c : candidates) all_same = all_same && (c == best || identical(c, best));
    return Resolved{best, static_cast<std::uint8_t>(candidates.size() == 1 || all_same ? 100 : same_folder ? 90 : 60)};
}

std::optional<Index> build_index(const Board& board, const IndexReport& report, const std::atomic<bool>& cancel) {
    Index index;
    const std::size_t n = board.cards.size();
    index.nodes.resize(n);
    const NameTable table(board);
    std::vector<std::vector<std::string>> imported(n), installed(n), mentioned(n);
    std::vector<std::vector<ModuleUse>> uses(n);
    std::vector<std::unordered_map<std::string, std::uint32_t>> export_names(n);
    std::vector<std::map<std::uint32_t, std::string>> export_lists(n);
    auto last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    const auto say = [&](const char* stage, std::uint64_t done, std::uint64_t total, bool force) {
        if (!report) return;
        const auto now = std::chrono::steady_clock::now();
        if (!force && now - last < std::chrono::milliseconds(100)) return;
        last = now;
        report({stage, done, total});
    };

    for (std::size_t i = 0; i < n; ++i) {
        if (cancel) return std::nullopt;
        say("Reading files", i, n, false);
        const Card& card = board.cards[i];
        Node& node = index.nodes[i];
        node.group = static_cast<std::uint32_t>(i);
        Bytes data;
        try {
            data = fs::read_file(board.file_of(card));
        } catch (const std::exception&) {
            ++index.unreadable;
            continue;
        }
        const crypto::Sha256Digest digest = crypto::sha256(data);
        node.sha256 = crypto::to_hex(digest);
        if (card.format == "PE" || card.format == "NE" || card.format == "LE") {
            BinaryFacts facts = read_binary(data, card.format);
            index.damaged += facts.damaged ? 1 : 0;
            node.exports = facts.exports;
            node.subsystem = std::move(facts.subsystem);
            imported[i] = std::move(facts.modules);
            uses[i] = std::move(facts.uses);
            export_names[i] = std::move(facts.export_names);
            export_lists[i] = std::move(facts.export_list);
        }
        if (card.pile == Pile::Setup && text::lower(card.name).ends_with(".inf")) installed[i] = inf_copied_files(as_text(data));
        if (searched(card.pile)) mentioned[i] = names_in_bytes(data, table.names());
    }

    say("Grouping copies", 0, n, true);
    std::unordered_map<std::string, std::uint32_t> first_of;
    first_of.reserve(n);
    for (std::size_t i = 0; i < n; ++i) {
        Node& node = index.nodes[i];
        if (node.sha256.empty()) continue;
        const auto [it, fresh] = first_of.try_emplace(node.sha256, static_cast<std::uint32_t>(i));
        node.group = it->second;
        if (!fresh) ++index.nodes[it->second].copies;
    }
    for (Node& node : index.nodes) node.copies = index.nodes[node.group].copies;

    say("Linking", 0, n, true);
    const auto identical = [&](std::uint32_t a, std::uint32_t b) { return !index.nodes[a].sha256.empty() && index.nodes[a].group == index.nodes[b].group; };
    std::map<std::tuple<std::uint32_t, std::uint32_t, Link>, Edge> edges;
    std::map<std::tuple<std::uint32_t, std::uint32_t, std::uint32_t>, std::uint32_t> symbol_uses;   // (exporting card, ordinal, importing card): call sites
    const auto link = [&](std::uint32_t from, const Resolved& to, Link kind, const ModuleUse* use = nullptr) {
        Edge& e = edges[{from, to.card, kind}];
        e.confidence = std::max(e.confidence, to.confidence);
        if (use == nullptr) return;
        e.symbols += static_cast<std::uint32_t>(use->symbols.size());
        e.calls += use->sites;
        for (const auto& [symbol, sites] : use->symbols) {
            const auto hit = export_names[to.card].find(symbol);
            if (hit == export_names[to.card].end()) continue;
            ++e.matched;
            symbol_uses[{to.card, hit->second, from}] += sites;
        }
    };
    for (std::size_t i = 0; i < n; ++i) {
        if (cancel) return std::nullopt;
        say("Linking", i, n, false);
        const auto from = static_cast<std::uint32_t>(i);
        std::set<std::uint32_t> strong;
        for (std::size_t m = 0; m < imported[i].size(); ++m) {
            const std::string& name = imported[i][m];
            if (const auto to = table.resolve(name, from, identical)) {
                link(from, *to, Link::Imports, &uses[i][m]);
                strong.insert(to->card);
            } else {
                ++index.nodes[i].unresolved;
                ++index.missing[text::lower(name)];
            }
        }
        for (const std::string& name : installed[i]) {
            if (const auto to = table.resolve(name, from, identical)) {
                link(from, *to, Link::Installs);
                strong.insert(to->card);
            } else {
                ++index.nodes[i].unresolved;
                ++index.missing[text::lower(name)];
            }
        }
        for (const std::string& name : mentioned[i]) {
            const auto to = table.resolve(name, from, identical);
            if (to && !strong.contains(to->card)) link(from, *to, Link::References);   // (a file it already imports or installs needs no weaker link too)
        }
    }
    index.edges.reserve(edges.size());
    for (const auto& [key, e] : edges) index.edges.push_back({std::get<0>(key), std::get<1>(key), std::get<2>(key), e.confidence, e.symbols, e.matched, e.calls});
    for (std::uint32_t card = 0; card < n; ++card)
        for (const auto& [ordinal, name] : export_lists[card]) index.exports.push_back({card, ordinal, name});
    index.finish();   // (the exports in order: a use finds its symbol by position)
    index.export_uses.reserve(symbol_uses.size());
    for (const auto& [key, sites] : symbol_uses) {
        const auto [card, ordinal, from] = key;
        const std::span<const ExportedSymbol> own = index.exports_of(card);
        const auto at = std::lower_bound(own.begin(), own.end(), ordinal, [](const ExportedSymbol& e, std::uint32_t o) { return e.ordinal < o; });
        if (at != own.end() && at->ordinal == ordinal) index.export_uses.push_back({from, index.first_export(card) + static_cast<std::uint32_t>(at - own.begin()), sites});
    }
    index.finish();
    say("Done", n, n, true);
    return index;
}

void save_index(const Board& board, const Index& index) {
    std::string out(kIndexHeader);
    out += '\n';
    out += "size\t" + std::to_string(index.nodes.size()) + '\t' + std::to_string(index.unreadable) + '\t' + std::to_string(index.damaged) + '\n';
    for (std::size_t i = 0; i < index.nodes.size(); ++i) {
        const Node& n = index.nodes[i];
        out += "n\t" + board.cards[i].rel + '\t' + n.sha256 + '\t' + std::to_string(n.group) + '\t' + std::to_string(n.copies) + '\t' + std::to_string(n.exports) + '\t' +
               std::to_string(n.unresolved) + '\t' + n.subsystem + '\n';
    }
    for (const Edge& e : index.edges)
        out += "e\t" + std::to_string(e.from) + '\t' + std::to_string(e.to) + '\t' + std::to_string(static_cast<int>(e.link)) + '\t' + std::to_string(e.confidence) + '\t' + std::to_string(e.symbols) + '\t' +
               std::to_string(e.matched) + '\t' + std::to_string(e.calls) + '\n';
    for (const auto& [name, count] : index.missing) out += "m\t" + name + '\t' + std::to_string(count) + '\n';
    for (const ExportedSymbol& e : index.exports) {
        std::string name = e.name;
        std::replace_if(name.begin(), name.end(), [](char c) { return c == '\t' || c == '\n' || c == '\r'; }, ' ');
        out += "x\t" + std::to_string(e.card) + '\t' + std::to_string(e.ordinal) + '\t' + name + '\n';
    }
    for (const ExportUse& u : index.export_uses) out += "u\t" + std::to_string(u.from) + '\t' + std::to_string(u.symbol) + '\t' + std::to_string(u.calls) + '\n';
    fs::write_file_atomic(board.root / "index.tsv", as_bytes(out));
}

std::optional<Index> load_index(const Board& board) {
    std::error_code ec;
    const stdfs::path file = board.root / "index.tsv";
    if (!stdfs::is_regular_file(file, ec)) return std::nullopt;
    const std::string all = fs::read_text(file);
    if (!all.starts_with(kIndexHeader)) return std::nullopt;
    Index index;
    index.nodes.reserve(board.cards.size());
    bool good = true;
    std::size_t at = 0;
    while (at < all.size() && good) {
        std::size_t end = all.find('\n', at);
        if (end == std::string::npos) end = all.size();
        const std::string_view line(all.data() + at, end - at);
        at = end + 1;
        const std::vector<std::string_view> p = fields(line, '\t');
        if (p[0] == "size" && p.size() >= 4) {
            good = number(p[1]) == board.cards.size();
            index.unreadable = number(p[2]);
            index.damaged = number(p[3]);
        } else if (p[0] == "n" && p.size() >= 8) {
            const std::size_t i = index.nodes.size();
            if (i >= board.cards.size() || board.cards[i].rel != p[1]) {
                good = false;
                break;
            }
            Node n;
            n.sha256 = std::string(p[2]);
            n.group = static_cast<std::uint32_t>(number(p[3]));
            n.copies = static_cast<std::uint32_t>(number(p[4]));
            n.exports = static_cast<std::uint32_t>(number(p[5]));
            n.unresolved = static_cast<std::uint32_t>(number(p[6]));
            n.subsystem = std::string(p[7]);
            good = n.group < board.cards.size();
            index.nodes.push_back(std::move(n));
        } else if (p[0] == "e" && p.size() >= 8) {
            Edge e;
            e.from = static_cast<std::uint32_t>(number(p[1]));
            e.to = static_cast<std::uint32_t>(number(p[2]));
            e.link = static_cast<Link>(std::min<std::uint64_t>(number(p[3]), 2));
            e.confidence = static_cast<std::uint8_t>(std::min<std::uint64_t>(number(p[4]), 100));
            e.symbols = static_cast<std::uint32_t>(number(p[5]));
            e.matched = static_cast<std::uint32_t>(number(p[6]));
            e.calls = static_cast<std::uint32_t>(number(p[7]));
            good = e.from < board.cards.size() && e.to < board.cards.size();
            index.edges.push_back(e);
        } else if (p[0] == "m" && p.size() >= 3) {
            index.missing[std::string(p[1])] = static_cast<std::uint32_t>(number(p[2]));
        } else if (p[0] == "x" && p.size() >= 4) {
            ExportedSymbol e;
            e.card = static_cast<std::uint32_t>(number(p[1]));
            e.ordinal = static_cast<std::uint32_t>(number(p[2]));
            e.name = std::string(p[3]);
            good = e.card < board.cards.size();
            index.exports.push_back(std::move(e));
        } else if (p[0] == "u" && p.size() >= 4) {
            ExportUse u;
            u.from = static_cast<std::uint32_t>(number(p[1]));
            u.symbol = static_cast<std::uint32_t>(number(p[2]));
            u.calls = static_cast<std::uint32_t>(number(p[3]));
            good = u.from < board.cards.size() && u.symbol < index.exports.size();
            index.export_uses.push_back(u);
        }
    }
    if (!good || index.nodes.size() != board.cards.size()) return std::nullopt;
    index.finish();
    return index;
}

}  // namespace cb::app::board
