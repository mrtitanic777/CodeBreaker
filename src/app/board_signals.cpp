// app/board_signals.cpp -- see board_signals.hpp.
#include "app/board_signals.hpp"

#include <algorithm>
#include <chrono>
#include <exception>
#include <format>
#include <map>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>

#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/le.hpp"
#include "formats/pe.hpp"

namespace cb::app::board {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kSignalsHeader = "#codebreaker-board-signals 2";
constexpr std::size_t kLongestValue = 200;

bool printable(std::uint8_t c) { return c >= 0x20 && c < 0x7F; }

// A value safe to keep in one field and to draw.
std::string clean(std::string_view s) {
    std::string out;
    for (const char c : s.substr(0, kLongestValue)) out += printable(static_cast<std::uint8_t>(c)) ? c : '?';
    return out;
}

std::string_view without_comment(std::string_view line) {
    bool quoted = false;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] == '"') quoted = !quoted;
        else if (line[i] == ';' && !quoted) return line.substr(0, i);
    }
    return line;
}

template <class Each>
void each_line(std::string_view all, Each&& each) {
    std::size_t at = 0;
    while (at < all.size()) {
        std::size_t end = all.find('\n', at);
        if (end == std::string_view::npos) end = all.size();
        each(text::trim(all.substr(at, end - at)));
        at = end + 1;
    }
}

std::string unquoted(std::string_view s) {
    s = text::trim(s);
    if (s.size() >= 2 && s.front() == '"' && s.back() == '"') s = s.substr(1, s.size() - 2);
    return std::string(s);
}

std::string substituted(std::string_view line, const std::map<std::string, std::string>& strings) {
    std::string out;
    for (std::size_t i = 0; i < line.size(); ++i) {
        if (line[i] != '%') {
            out += line[i];
            continue;
        }
        const std::size_t close = line.find('%', i + 1);
        if (close == std::string_view::npos) {
            out += line.substr(i);
            break;
        }
        if (close == i + 1) {
            out += '%';
        } else {
            const auto it = strings.find(text::lower(line.substr(i + 1, close - i - 1)));
            if (it != strings.end()) out += it->second;
            else out += line.substr(i, close - i + 1);
        }
        i = close;
    }
    return out;
}

std::string root_name(std::string_view root) {
    const std::string r = text::lower(text::trim(root));
    if (r == "hklm" || r == "hkey_local_machine") return "hklm";
    if (r == "hkcu" || r == "hkey_current_user") return "hkcu";
    if (r == "hkcr" || r == "hkey_classes_root") return "hkcr";
    if (r == "hku" || r == "hkey_users") return "hku";
    if (r == "hkr") return "hkr";
    return {};
}

std::string joined_key(const std::string& root, std::string_view subkey) {
    std::string key = text::lower(text::trim(subkey));
    while (!key.empty() && (key.back() == '\\' || key.back() == '/')) key.pop_back();
    return key.empty() ? root : root + "\\" + key;
}

std::optional<std::uint32_t> hex_number(std::string_view s) {
    s = text::trim(s);
    if (s.empty() || s.size() > 4) return std::nullopt;
    std::uint32_t v = 0;
    for (const char c : s) {
        const int d = c >= '0' && c <= '9' ? c - '0' : c >= 'a' && c <= 'f' ? c - 'a' + 10 : c >= 'A' && c <= 'F' ? c - 'A' + 10 : -1;
        if (d < 0) return std::nullopt;
        v = v * 16 + static_cast<std::uint32_t>(d);
    }
    return v;
}

// "5", "S:5", "L:11": a decimal channel after any attribute letters.
std::optional<std::uint32_t> channel_number(std::string_view s) {
    if (const std::size_t colon = s.find_last_of(':'); colon != std::string_view::npos) s = s.substr(colon + 1);
    s = text::trim(s);
    if (s.empty() || s.size() > 2) return std::nullopt;
    std::uint32_t v = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') return std::nullopt;
        v = v * 10 + static_cast<std::uint32_t>(c - '0');
    }
    return v <= 15 ? std::optional<std::uint32_t>(v) : std::nullopt;
}

void sorted_unique(std::vector<std::uint32_t>& v) {
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());
}

std::uint64_t cards_hash(const Board& board) {
    std::uint64_t hash = 14695981039346656037ull;
    for (const Card& c : board.cards) {
        for (const char ch : c.rel) hash = (hash ^ static_cast<std::uint8_t>(ch)) * 1099511628211ull;
        hash = (hash ^ 0xFFu) * 1099511628211ull;
    }
    return hash;
}

// A registry path without HKLM or HKCU: a program's bytes say "Software\..." and not which hive, and must meet the script that writes the same key.
std::string shared_key(std::string_view key) {
    if (key.starts_with("hklm\\") || key.starts_with("hkcu\\")) key.remove_prefix(5);
    return clean(key);
}

std::optional<std::string> string_at(const formats::pe::Image& image, std::uint64_t base, std::uint32_t va, bool wide) {
    if (va < base) return std::nullopt;
    const auto offset = image.rva_to_offset(va - base);
    if (!offset) return std::nullopt;
    const ByteView d = image.data();
    std::string out;
    const std::size_t step = wide ? 2 : 1;
    for (std::size_t at = *offset; at + step <= d.size() && out.size() <= 128; at += step) {
        if (d[at] == 0 && (!wide || d[at + 1] == 0)) return out.size() >= 3 ? std::optional<std::string>(out) : std::nullopt;
        if (!printable(d[at]) || (wide && d[at + 1] != 0)) return std::nullopt;
        out += static_cast<char>(d[at]);
    }
    return std::nullopt;
}

struct ObjectApi {
    std::string_view name;
    SignalType type;
    Direction direction;
};
constexpr ObjectApi kObjectApis[] = {
    {"createmutex", SignalType::Mutex, Direction::Writes},          {"openmutex", SignalType::Mutex, Direction::Reads},
    {"createevent", SignalType::Event, Direction::Writes},          {"openevent", SignalType::Event, Direction::Reads},
    {"createsemaphore", SignalType::Semaphore, Direction::Writes},  {"opensemaphore", SignalType::Semaphore, Direction::Reads},
    {"createfilemapping", SignalType::Mapping, Direction::Writes},  {"openfilemapping", SignalType::Mapping, Direction::Reads},
    {"registerwindowmessage", SignalType::WindowMessage, Direction::Listens},
};

std::uint32_t u32_le(ByteView b, std::size_t at) {
    return static_cast<std::uint32_t>(b[at]) | (static_cast<std::uint32_t>(b[at + 1]) << 8) | (static_cast<std::uint32_t>(b[at + 2]) << 16) | (static_cast<std::uint32_t>(b[at + 3]) << 24);
}

// Collects signals with one id per (type, value), the highest confidence of each link.
class Builder {
public:
    void add(std::uint32_t node, SignalType type, const std::string& value, Direction direction, std::uint8_t confidence) {
        if (value.empty()) return;
        const auto [it, fresh] = ids_.try_emplace({type, value}, static_cast<std::uint32_t>(ids_.size()));
        (void)fresh;
        std::uint8_t& kept = links_[{node, it->second, direction}];
        kept = std::max(kept, confidence);
    }
    Signals finish(std::size_t cards) {
        Signals out;
        std::vector<std::uint32_t> remap(ids_.size());
        out.signals.reserve(ids_.size());
        for (const auto& [key, id] : ids_) {   // (a map: already by type, then value)
            remap[id] = static_cast<std::uint32_t>(out.signals.size());
            out.signals.push_back({key.first, key.second});
        }
        out.links.reserve(links_.size());
        for (const auto& [key, confidence] : links_) out.links.push_back({std::get<0>(key), remap[std::get<1>(key)], std::get<2>(key), confidence});
        out.finish(cards);
        return out;
    }

private:
    std::map<std::pair<SignalType, std::string>, std::uint32_t> ids_;
    std::map<std::tuple<std::uint32_t, std::uint32_t, Direction>, std::uint8_t> links_;
};

}  // namespace

std::string_view signal_type_name(SignalType type) {
    switch (type) {
        case SignalType::Clsid: return "COM class";
        case SignalType::Mutex: return "Mutex";
        case SignalType::Event: return "Event";
        case SignalType::Semaphore: return "Semaphore";
        case SignalType::Mapping: return "Shared memory";
        case SignalType::WindowMessage: return "Window message";
        case SignalType::IoPort: return "I/O port";
        case SignalType::Irq: return "IRQ";
        case SignalType::Dma: return "DMA";
        case SignalType::RegKey: return "Registry key";
        case SignalType::Autostart: return "Autostart";
        default: return "Hardware class";
    }
}

std::string_view direction_name(Direction direction) {
    switch (direction) {
        case Direction::Reads: return "reads";
        case Direction::Writes: return "writes";
        case Direction::Registers: return "registers";
        case Direction::Listens: return "listens";
        default: return "mentions";
    }
}

void Signals::finish(std::size_t cards) {
    std::sort(links.begin(), links.end(), [](const NodeSignal& a, const NodeSignal& b) { return std::tie(a.node, a.signal, a.direction) < std::tie(b.node, b.signal, b.direction); });
    node_start.assign(cards + 1, 0);
    signal_start.assign(signals.size() + 1, 0);
    for (const NodeSignal& l : links) {
        ++node_start[l.node + 1];
        ++signal_start[l.signal + 1];
    }
    for (std::size_t i = 0; i < cards; ++i) node_start[i + 1] += node_start[i];
    for (std::size_t i = 0; i < signals.size(); ++i) signal_start[i + 1] += signal_start[i];
    signal_links.assign(links.size(), 0);
    std::vector<std::uint32_t> next(signal_start.begin(), signal_start.end() - 1);
    for (std::size_t i = 0; i < links.size(); ++i) signal_links[next[links[i].signal]++] = static_cast<std::uint32_t>(i);
}

std::span<const NodeSignal> Signals::of_node(std::uint32_t card) const {
    return std::span<const NodeSignal>(links).subspan(node_start[card], node_start[card + 1] - node_start[card]);
}

std::span<const std::uint32_t> Signals::of_signal(std::uint32_t signal) const {
    return std::span<const std::uint32_t>(signal_links).subspan(signal_start[signal], signal_start[signal + 1] - signal_start[signal]);
}

std::vector<std::string> inf_fields(std::string_view line) {
    std::vector<std::string> out(1);
    bool quoted = false;
    for (const char c : line) {
        if (c == '"') quoted = !quoted;
        else if (c == ',' && !quoted) out.emplace_back();
        else out.back() += c;
    }
    for (std::string& f : out) f = std::string(text::trim(f));
    return out;
}

SetupFacts parse_inf(std::string_view script) {
    SetupFacts facts;
    std::map<std::string, std::vector<std::string>> sections;
    std::vector<std::string>* current = nullptr;
    each_line(script, [&](std::string_view raw) {
        const std::string_view line = text::trim(without_comment(raw));
        if (line.empty()) return;
        if (line.front() == '[') {
            const std::size_t close = line.find(']');
            current = close == std::string_view::npos ? nullptr : &sections[text::lower(text::trim(line.substr(1, close - 1)))];
        } else if (current != nullptr) {
            current->emplace_back(line);
        }
    });
    std::map<std::string, std::string> strings;
    if (const auto it = sections.find("strings"); it != sections.end()) {
        for (const std::string& line : it->second) {
            const std::size_t equals = line.find('=');
            if (equals != std::string::npos) strings[text::lower(text::trim(std::string_view(line).substr(0, equals)))] = unquoted(std::string_view(line).substr(equals + 1));
        }
    }
    std::set<std::string> add_reg, update_inis;
    for (const auto& [name, lines] : sections) {
        if (name == "strings") continue;
        for (const std::string& line : lines) {
            const std::size_t equals = line.find('=');
            if (equals == std::string::npos) continue;
            const std::string key = text::lower(text::trim(std::string_view(line).substr(0, equals)));
            const std::string_view value = std::string_view(line).substr(equals + 1);
            if (key == "addreg" || key == "updateinis") {
                for (const std::string& item : inf_fields(value))
                    if (!item.empty()) (key == "addreg" ? add_reg : update_inis).insert(text::lower(item));
            } else if (key == "ioconfig") {
                for (const std::string& item : inf_fields(value)) {
                    std::string_view range = item;
                    if (const std::size_t paren = range.find('('); paren != std::string_view::npos) range = range.substr(0, paren);
                    if (range.find('@') != std::string_view::npos) continue;   // "size@min-max": anywhere in a range, no fixed port
                    if (const std::size_t colon = range.find_last_of(':'); colon != std::string_view::npos) range = range.substr(colon + 1);
                    if (const auto port = hex_number(range.substr(0, range.find('-')))) facts.io_ports.push_back(*port);
                }
            } else if (key == "irqconfig" || key == "dmaconfig") {
                for (const std::string& item : inf_fields(value))
                    if (const auto channel = channel_number(item)) (key == "irqconfig" ? facts.irqs : facts.dmas).push_back(*channel);
            } else if (key == "class" && name == "version") {
                facts.device_class = text::lower(substituted(unquoted(value), strings));
            }
        }
    }
    for (const std::string& name : add_reg) {
        const auto section = sections.find(name);
        if (section == sections.end()) continue;
        for (const std::string& line : section->second) {
            const std::vector<std::string> f = inf_fields(substituted(line, strings));
            if (f.size() < 2) continue;
            const std::string root = root_name(f[0]);
            if (root.empty()) continue;
            RegWrite write;
            write.key = joined_key(root, f[1]);
            for (std::size_t i = f.size(); i > 4; --i) {   // root, key, value name, flags, then the data
                if (!f[i - 1].empty()) {
                    write.data = f[i - 1];
                    break;
                }
            }
            facts.registry.push_back(std::move(write));
        }
    }
    for (const std::string& name : update_inis) {
        const auto section = sections.find(name);
        if (section == sections.end()) continue;
        for (const std::string& line : section->second) {
            const std::vector<std::string> f = inf_fields(substituted(line, strings));   // file, section, the old entry (removed), the new entry
            if (f.size() >= 4 && !f[3].empty()) facts.inis.push_back({text::lower(f[0]), text::lower(f[1]), f[3]});
        }
    }
    sorted_unique(facts.io_ports);
    sorted_unique(facts.irqs);
    sorted_unique(facts.dmas);
    return facts;
}

std::vector<RegWrite> parse_reg(std::string_view content) {
    std::vector<RegWrite> out;
    std::string key;
    each_line(content, [&](std::string_view line) {
        if (line.empty() || line.front() == ';') return;
        if (line.front() == '[') {
            key.clear();
            const std::size_t close = line.find(']');
            if (close == std::string_view::npos || line[1] == '-') return;   // ([-KEY] deletes it)
            const std::string_view path = line.substr(1, close - 1);
            const std::size_t slash = path.find('\\');
            const std::string root = root_name(path.substr(0, slash));
            if (root.empty()) return;
            key = joined_key(root, slash == std::string_view::npos ? std::string_view{} : path.substr(slash + 1));
            out.push_back({key, {}});
            return;
        }
        if (key.empty()) return;
        const std::size_t equals = line.find('=');
        if (equals == std::string_view::npos) return;
        const std::string_view value = text::trim(line.substr(equals + 1));
        if (value == "-") return;   // (deletes the value)
        std::string data;
        const std::string plain = unquoted(value);
        for (std::size_t i = 0; i < plain.size(); ++i) {
            if (plain[i] == '\\' && i + 1 < plain.size() && plain[i + 1] == '\\') ++i;
            data += plain[i];
        }
        out.push_back({key, std::move(data)});
    });
    return out;
}

void for_each_string(ByteView data, std::size_t min_length, const std::function<void(std::string_view)>& each) {
    std::string run;
    const auto flush = [&] {
        if (run.size() >= min_length) each(run);
        run.clear();
    };
    for (const std::uint8_t c : data) {
        if (printable(c)) run += static_cast<char>(c);
        else flush();
    }
    flush();
    for (std::size_t phase = 0; phase < 2; ++phase) {
        for (std::size_t i = phase; i + 1 < data.size(); i += 2) {
            if (data[i + 1] == 0 && printable(data[i])) run += static_cast<char>(data[i]);
            else flush();
        }
        flush();
    }
}

std::vector<std::string> guids_in_text(std::string_view s) {
    std::vector<std::string> out;
    for (std::size_t at = s.find('{'); at != std::string_view::npos && at + 38 <= s.size(); at = s.find('{', at + 1)) {
        const std::string_view candidate = s.substr(at, 38);
        if (candidate[37] != '}') continue;
        bool good = true;
        for (std::size_t i = 1; i < 37 && good; ++i) {
            const char c = candidate[i];
            if (i == 9 || i == 14 || i == 19 || i == 24) good = c == '-';
            else good = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
        }
        if (good && candidate != "{00000000-0000-0000-0000-000000000000}") out.push_back(text::lower(candidate));   // (the null GUID: a blank, not a class)
    }
    return out;
}

std::optional<std::array<std::uint8_t, 16>> guid_bytes(std::string_view guid) {
    if (guids_in_text(guid).size() != 1 || guid.size() != 38) return std::nullopt;
    const auto byte = [&](std::size_t at) { return static_cast<std::uint8_t>(*hex_number(guid.substr(at, 2))); };
    return std::array<std::uint8_t, 16>{byte(7),  byte(5),  byte(3),  byte(1),  byte(12), byte(10), byte(17), byte(15),
                                        byte(20), byte(22), byte(25), byte(27), byte(29), byte(31), byte(33), byte(35)};
}

std::string registry_path(std::string_view s) {
    std::string path = text::lower(s);
    for (const auto& [full, short_name] : {std::pair<std::string_view, std::string_view>{"hkey_local_machine\\", ""}, {"hkey_current_user\\", ""}, {"hklm\\", ""}, {"hkcu\\", ""}})
        if (path.starts_with(full)) path.erase(0, full.size());
    if (!path.starts_with("software\\") && !path.starts_with("system\\currentcontrolset\\")) return {};
    if (path.find('\\', 9) == std::string::npos) return {};
    if (path.find_first_of("\"%<>|*?") != std::string::npos) return {};
    while (!path.empty() && path.back() == '\\') path.pop_back();
    return path;
}

std::vector<NamedObject> named_objects(ByteView data) {
    std::vector<NamedObject> out;
    const std::optional<formats::pe::Image> parsed = formats::pe::Image::try_parse(data);
    if (!parsed) return out;
    const formats::pe::Image& image = *parsed;
    const std::uint64_t base = image.optional_header().image_base;
    if (base > 0xFFFFFFFFu) return out;
    struct Api {
        const ObjectApi* api;
        bool wide;
    };
    std::unordered_map<std::uint32_t, Api> slots;
    try {
        for (const formats::pe::ImportModule& module : image.import_table(false)) {
            for (const formats::pe::ImportSymbol& symbol : module.symbols) {
                if (!symbol.name || symbol.name->size() < 2) continue;
                const std::string name = text::lower(*symbol.name);
                const char last = name.back();
                if (last != 'a' && last != 'w') continue;
                const std::string_view stem = std::string_view(name).substr(0, name.size() - 1);
                for (const ObjectApi& api : kObjectApis)
                    if (api.name == stem) slots.emplace(static_cast<std::uint32_t>(base + symbol.slot_rva), Api{&api, last == 'w'});
            }
        }
    } catch (const std::exception&) {
        return out;
    }
    if (slots.empty()) return out;
    std::unordered_map<std::uint64_t, Api> thunks;
    const auto sections = image.code_sections();
    for (const auto& section : sections) {
        const ByteView code = image.section_data(*section.section);
        for (std::size_t i = 0; i + 6 <= code.size(); ++i)
            if (code[i] == 0xFF && code[i + 1] == 0x25)
                if (const auto hit = slots.find(u32_le(code, i + 2)); hit != slots.end()) thunks.emplace(section.va + i, hit->second);
    }
    std::set<std::tuple<SignalType, Direction, std::string>> seen;
    const auto argument = [&](ByteView code, std::size_t call, const Api& api) {
        for (std::size_t back = 5; back <= 32 && back <= call; ++back) {
            const std::size_t at = call - back;
            if (code[at] != 0x68) continue;
            if (const auto name = string_at(image, base, u32_le(code, at + 1), api.wide)) {
                if (seen.insert({api.api->type, api.api->direction, *name}).second) out.push_back({api.api->type, api.api->direction, *name});
                return;
            }
        }
    };
    for (const auto& section : sections) {
        const ByteView code = image.section_data(*section.section);
        for (std::size_t i = 0; i + 5 <= code.size(); ++i) {
            if (code[i] == 0xE8) {
                const auto target = static_cast<std::uint64_t>(static_cast<std::int64_t>(section.va + i + 5) + static_cast<std::int32_t>(u32_le(code, i + 1)));
                if (const auto hit = thunks.find(target); hit != thunks.end()) argument(code, i, hit->second);
            } else if (i + 6 <= code.size() && code[i] == 0xFF && code[i + 1] == 0x15) {
                if (const auto hit = slots.find(u32_le(code, i + 2)); hit != slots.end()) argument(code, i, hit->second);
            }
        }
    }
    return out;
}

std::vector<PortUse> ports_in_code(ByteView code) {
    std::set<std::pair<std::uint16_t, bool>> found;
    const auto look = [&](std::size_t after, std::uint32_t port) {
        if (port < 0x20 || port > 0xFFFF) return;
        for (std::size_t i = after; i < after + 6 && i < code.size(); ++i) {
            const std::uint8_t c = code[i];
            if (c >= 0xEC && c <= 0xEF) {
                found.emplace(static_cast<std::uint16_t>(port), c >= 0xEE);
                return;
            }
        }
    };
    for (std::size_t i = 0; i + 3 < code.size(); ++i) {
        if (code[i] == 0x66 && code[i + 1] == 0xBA) {
            look(i + 4, static_cast<std::uint32_t>(code[i + 2]) | (static_cast<std::uint32_t>(code[i + 3]) << 8));
        } else if (code[i] == 0xBA && i + 5 <= code.size() && (i == 0 || code[i - 1] != 0x66)) {
            look(i + 5, u32_le(code, i + 1));
        }
    }
    std::vector<PortUse> out;
    out.reserve(found.size());
    for (const auto& [port, writes] : found) out.push_back({port, writes});
    return out;
}

std::optional<Signals> build_signals(const Board& board, const Index& index, const SignalReport& report, const std::atomic<bool>& cancel) {
    const std::size_t n = board.cards.size();
    Builder builder;
    const NameTable table(board);
    const auto identical = [&](std::uint32_t a, std::uint32_t b) { return !index.nodes[a].sha256.empty() && index.nodes[a].group == index.nodes[b].group; };
    auto last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    const auto say = [&](const char* stage, std::uint64_t done) {
        if (!report) return;
        const auto now = std::chrono::steady_clock::now();
        if (now - last < std::chrono::milliseconds(100)) return;
        last = now;
        report({stage, done, n});
    };
    // Every file a piece of text names, seen from `node`.
    const auto targets = [&](std::uint32_t node, std::string_view words) {
        std::vector<std::uint32_t> out;
        for (const std::string& name : names_in_bytes(as_bytes(words), table.names()))
            if (const auto to = table.resolve(name, node, identical)) out.push_back(to->card);
        return out;
    };
    const auto registry = [&](std::uint32_t node, const std::vector<RegWrite>& writes) {
        for (const RegWrite& w : writes) {
            // HKR is relative to each device's own key: the same words under it in two scripts are two different keys, so it is no shared channel
            const bool relative = w.key == "hkr" || w.key.starts_with("hkr\\");
            if (!relative) builder.add(node, SignalType::RegKey, shared_key(w.key), Direction::Writes, 100);
            const bool com = w.key.find("clsid\\") != std::string::npos;
            for (const std::string& guid : guids_in_text(w.key)) {
                if (com) builder.add(node, SignalType::Clsid, guid, Direction::Registers, 100);
                if (com && !w.data.empty())
                    for (const std::uint32_t to : targets(node, w.data)) builder.add(to, SignalType::Clsid, guid, Direction::Registers, 100);
            }
            const std::size_t tail = w.key.rfind("\\currentversion\\");
            if (tail == std::string::npos) continue;
            const std::string last_part = w.key.substr(tail + 16);
            if (last_part != "run" && last_part != "runonce" && last_part != "runservices" && last_part != "runservicesonce") continue;
            const std::string value = w.key.substr(0, w.key.find('\\')) + " " + last_part;
            builder.add(node, SignalType::Autostart, value, Direction::Writes, 100);
            for (const std::uint32_t to : targets(node, w.data)) builder.add(to, SignalType::Autostart, value, Direction::Registers, 100);
        }
    };
    const auto ini_entry = [&](std::uint32_t node, std::string_view file, std::string_view section, std::string_view entry, Direction own) {
        const std::size_t equals = entry.find('=');
        if (equals == std::string_view::npos) return;
        const std::string key = text::lower(text::trim(entry.substr(0, equals)));
        const std::string_view value = text::trim(entry.substr(equals + 1));
        const bool starts = (file == "system.ini" && section == "386enh" && key == "device") || (file == "system.ini" && section == "boot" && (key == "shell" || key == "drivers")) ||
                            (file == "win.ini" && section == "windows" && (key == "load" || key == "run"));
        if (!starts || value.empty() || value.front() == '*') return;   // (*name is a device built into VMM32, not a file)
        const std::string signal = std::string(file) + " [" + std::string(section) + "] " + key;
        builder.add(node, SignalType::Autostart, signal, own, 100);
        for (const std::uint32_t to : targets(node, value)) builder.add(to, SignalType::Autostart, signal, Direction::Registers, 100);
    };

    std::unordered_map<std::string, std::array<std::uint8_t, 16>> known;   // GUIDs seen as text; their stored form is looked for in the binaries
    const auto note_guid = [&](const std::string& guid) {
        if (known.contains(guid)) return;
        if (const auto bytes = guid_bytes(guid)) known.emplace(guid, *bytes);
    };
    for (std::uint32_t i = 0; i < n; ++i) {
        if (cancel) return std::nullopt;
        say("Reading signals", i);
        const Card& card = board.cards[i];
        const std::string name = text::lower(card.name);
        const bool binary = card.format == "PE" || card.format == "NE" || card.format == "LE";
        const bool inf = name.ends_with(".inf"), reg = name.ends_with(".reg"), ini = name == "system.ini" || name == "win.ini";
        if (!binary && !inf && !reg && !ini) continue;
        Bytes data;
        try {
            data = fs::read_file(board.file_of(card));
        } catch (const std::exception&) {
            continue;
        }
        const std::string_view words = as_text(data);
        if (inf) {
            const SetupFacts facts = parse_inf(words);
            registry(i, facts.registry);
            for (const IniWrite& w : facts.inis) ini_entry(i, w.file, w.section, w.entry, Direction::Writes);
            for (const std::string& guid : guids_in_text(words)) note_guid(guid);
            std::vector<std::uint32_t> drivers;
            for (const Edge& e : index.out_of(i))
                if (e.link == Link::Installs && board.cards[e.to].pile == Pile::Drivers) drivers.push_back(e.to);
            const auto bind = [&](SignalType type, const std::string& value) {
                builder.add(i, type, value, Direction::Registers, 100);
                for (const std::uint32_t d : drivers) builder.add(d, type, value, Direction::Registers, 50);
            };
            for (const std::uint32_t port : facts.io_ports) bind(SignalType::IoPort, std::format("0x{:03x}", port));
            for (const std::uint32_t irq : facts.irqs) bind(SignalType::Irq, std::format("irq {}", irq));
            for (const std::uint32_t dma : facts.dmas) bind(SignalType::Dma, std::format("dma {}", dma));
            if (!facts.device_class.empty()) bind(SignalType::HwClass, clean(facts.device_class));
        } else if (reg) {
            const std::vector<RegWrite> writes = parse_reg(words);
            registry(i, writes);
            for (const RegWrite& w : writes)
                for (const std::string& guid : guids_in_text(w.key)) note_guid(guid);
        } else if (ini) {
            std::string section;
            each_line(words, [&](std::string_view raw) {
                const std::string_view line = text::trim(without_comment(raw));
                if (line.empty()) return;
                if (line.front() == '[') {
                    const std::size_t close = line.find(']');
                    section = close == std::string_view::npos ? std::string() : text::lower(line.substr(1, close - 1));
                } else {
                    ini_entry(i, name, section, line, Direction::Mentions);
                }
            });
        }
        if (!binary) continue;
        for_each_string(data, 8, [&](std::string_view s) {
            for (const std::string& guid : guids_in_text(s)) {
                note_guid(guid);
                builder.add(i, SignalType::Clsid, guid, Direction::Mentions, 60);
            }
            if (const std::string key = registry_path(s); !key.empty()) builder.add(i, SignalType::RegKey, clean(key), Direction::Mentions, 60);
        });
        if (card.format == "PE")
            for (const NamedObject& object : named_objects(data)) builder.add(i, object.type, clean(object.name), object.direction, 70);
        if (card.pile == Pile::Drivers) {
            const auto ports = [&](ByteView code) {
                for (const PortUse& use : ports_in_code(code)) builder.add(i, SignalType::IoPort, std::format("0x{:03x}", use.port), use.writes ? Direction::Writes : Direction::Reads, 50);
            };
            try {
                if (card.format == "LE") {
                    const formats::le::Module module = formats::le::Module::from_buffer(data);
                    for (const formats::le::Object& object : module.objects())
                        if (object.executable()) ports(module.object_data(object));
                } else if (card.format == "PE") {
                    const formats::pe::Image image = formats::pe::Image::parse(ByteView(data));
                    for (const auto& section : image.code_sections()) ports(image.section_data(*section.section));
                }
            } catch (const std::exception&) {}   // (a driver whose code cannot be laid out shows no ports)
        }
    }

    // ---- the GUIDs in the form a program stores them: 16 bytes, found only for GUIDs some text on the board names
    std::unordered_map<std::string, std::string> by_bytes;
    std::vector<bool> first_two(65536, false);
    by_bytes.reserve(known.size());
    for (const auto& [guid, bytes] : known) {
        by_bytes.emplace(std::string(reinterpret_cast<const char*>(bytes.data()), bytes.size()), guid);
        first_two[bytes[0] | (bytes[1] << 8)] = true;
    }
    if (!by_bytes.empty()) {
        for (std::uint32_t i = 0; i < n; ++i) {
            if (cancel) return std::nullopt;
            say("Matching COM classes", i);
            const Card& card = board.cards[i];
            if (card.format != "PE" && card.format != "NE" && card.format != "LE") continue;
            Bytes data;
            try {
                data = fs::read_file(board.file_of(card));
            } catch (const std::exception&) {
                continue;
            }
            for (std::size_t at = 0; at + 16 <= data.size(); ++at) {
                if (!first_two[data[at] | (data[at + 1] << 8)]) continue;
                const auto hit = by_bytes.find(std::string(reinterpret_cast<const char*>(data.data() + at), 16));
                if (hit != by_bytes.end()) builder.add(i, SignalType::Clsid, hit->second, Direction::Mentions, 60);
            }
        }
    }
    return builder.finish(n);
}

void save_signals(const Board& board, const Signals& signals) {
    std::string out(kSignalsHeader);
    out += '\n';
    out += "size\t" + std::to_string(board.cards.size()) + '\t' + std::to_string(cards_hash(board)) + '\n';
    for (const Signal& s : signals.signals) out += "s\t" + std::to_string(static_cast<int>(s.type)) + '\t' + s.value + '\n';
    for (const NodeSignal& l : signals.links)
        out += "l\t" + std::to_string(l.node) + '\t' + std::to_string(l.signal) + '\t' + std::to_string(static_cast<int>(l.direction)) + '\t' + std::to_string(l.confidence) + '\n';
    fs::write_file_atomic(board.root / "signals.tsv", as_bytes(out));
}

std::optional<Signals> load_signals(const Board& board) {
    std::error_code ec;
    const stdfs::path file = board.root / "signals.tsv";
    if (!stdfs::is_regular_file(file, ec)) return std::nullopt;
    const std::string all = fs::read_text(file);
    if (!all.starts_with(kSignalsHeader)) return std::nullopt;
    const auto number = [](std::string_view s) {
        std::uint64_t v = 0;
        for (const char c : s) {
            if (c < '0' || c > '9') break;
            v = v * 10 + static_cast<std::uint64_t>(c - '0');
        }
        return v;
    };
    Signals signals;
    bool good = false;
    std::size_t at = 0;
    while (at < all.size()) {
        std::size_t end = all.find('\n', at);
        if (end == std::string::npos) end = all.size();
        const std::string_view line(all.data() + at, end - at);
        at = end + 1;
        std::array<std::string_view, 5> p{};
        std::size_t count = 0, from = 0;
        while (count < p.size()) {
            const std::size_t tab = line.find('\t', from);
            p[count++] = line.substr(from, tab == std::string_view::npos ? std::string_view::npos : tab - from);
            if (tab == std::string_view::npos) break;
            from = tab + 1;
        }
        if (p[0] == "size" && count >= 3) {
            good = number(p[1]) == board.cards.size() && number(p[2]) == cards_hash(board);
            if (!good) return std::nullopt;
        } else if (p[0] == "s" && count >= 3) {
            signals.signals.push_back({static_cast<SignalType>(std::min<std::uint64_t>(number(p[1]), kSignalTypes - 1)), std::string(p[2])});
        } else if (p[0] == "l" && count >= 5) {
            NodeSignal l;
            l.node = static_cast<std::uint32_t>(number(p[1]));
            l.signal = static_cast<std::uint32_t>(number(p[2]));
            l.direction = static_cast<Direction>(std::min<std::uint64_t>(number(p[3]), 4));
            l.confidence = static_cast<std::uint8_t>(std::min<std::uint64_t>(number(p[4]), 100));
            if (l.node >= board.cards.size() || l.signal >= signals.signals.size()) return std::nullopt;
            signals.links.push_back(l);
        }
    }
    if (!good) return std::nullopt;
    signals.finish(board.cards.size());
    return signals;
}

}  // namespace cb::app::board
