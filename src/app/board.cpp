// app/board.cpp -- see board.hpp.
#include "app/board.hpp"

#include <algorithm>
#include <chrono>
#include <cstdio>
#include <deque>
#include <exception>
#include <format>
#include <fstream>
#include <map>
#include <set>
#include <unordered_map>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "formats/cab.hpp"
#include "formats/media.hpp"

namespace cb::app::board {

namespace stdfs = std::filesystem;

namespace {

constexpr std::string_view kBoardHeader = "#codebreaker-board 1";
constexpr std::string_view kStateHeader = "#codebreaker-board-state 1";
constexpr int kMaxCabinetDepth = 4;
constexpr std::size_t kHeadBytes = 4096;

struct ExtensionPile {
    std::string_view ext;
    Pile pile;
};
constexpr ExtensionPile kExtensions[] = {
    {"exe", Pile::Programs},  {"com", Pile::Programs},  {"scr", Pile::Programs},  {"pif", Pile::Programs},
    {"dll", Pile::Libraries}, {"ocx", Pile::Libraries}, {"cpl", Pile::Libraries}, {"acm", Pile::Libraries}, {"ax", Pile::Libraries},
    {"tlb", Pile::Libraries}, {"vbx", Pile::Libraries}, {"ime", Pile::Libraries},
    {"vxd", Pile::Drivers},   {"386", Pile::Drivers},   {"drv", Pile::Drivers},   {"sys", Pile::Drivers},   {"mpd", Pile::Drivers},
    {"pdr", Pile::Drivers},   {"vdd", Pile::Drivers},   {"dsk", Pile::Drivers},   {"ios", Pile::Drivers},
    {"inf", Pile::Setup},     {"ini", Pile::Setup},     {"reg", Pile::Setup},     {"cfg", Pile::Setup},     {"inx", Pile::Setup},
    {"lst", Pile::Setup},     {"pol", Pile::Setup},     {"adm", Pile::Setup},
    {"txt", Pile::Text},      {"doc", Pile::Text},      {"wri", Pile::Text},      {"rtf", Pile::Text},      {"bat", Pile::Text},
    {"htm", Pile::Text},      {"html", Pile::Text},     {"me", Pile::Text},       {"log", Pile::Text},      {"diz", Pile::Text},
    {"hlp", Pile::Help},      {"cnt", Pile::Help},      {"chm", Pile::Help},
    {"fon", Pile::Fonts},     {"ttf", Pile::Fonts},     {"fot", Pile::Fonts},     {"ttc", Pile::Fonts},
    {"bmp", Pile::Media},     {"ico", Pile::Media},     {"cur", Pile::Media},     {"ani", Pile::Media},     {"wav", Pile::Media},
    {"mid", Pile::Media},     {"rmi", Pile::Media},     {"avi", Pile::Media},     {"jpg", Pile::Media},     {"gif", Pile::Media},
    {"pcx", Pile::Media},     {"wmf", Pile::Media},     {"mov", Pile::Media},
    {"cab", Pile::Cabinets},
};

std::optional<Pile> pile_of_extension(std::string_view name) {
    const std::size_t dot = name.find_last_of('.');
    if (dot == std::string_view::npos) return std::nullopt;
    const std::string ext = text::lower(name.substr(dot + 1));
    for (const ExtensionPile& e : kExtensions)
        if (ext == e.ext) return e.pile;
    return std::nullopt;
}

std::uint16_t le16(ByteView b, std::size_t at) { return at + 2 <= b.size() ? static_cast<std::uint16_t>(b[at] | (b[at + 1] << 8)) : std::uint16_t{0}; }
std::uint32_t le32(ByteView b, std::size_t at) {
    return at + 4 <= b.size() ? static_cast<std::uint32_t>(le16(b, at)) | (static_cast<std::uint32_t>(le16(b, at + 2)) << 16) : 0;
}
bool starts(ByteView b, std::string_view magic) {
    return b.size() >= magic.size() && std::equal(magic.begin(), magic.end(), b.begin(), [](char c, std::uint8_t u) { return static_cast<std::uint8_t>(c) == u; });
}

bool looks_like_text(ByteView head) {
    if (head.empty()) return false;
    std::size_t plain = 0;
    for (const std::uint8_t c : head) {
        if (c == 0) return false;
        if (c >= 32 || c == '\r' || c == '\n' || c == '\t' || c == 0x1A) ++plain;
    }
    return plain * 100 >= head.size() * 95;
}

std::string escaped(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (const char c : s) {
        if (c == '\\') out += "\\\\";
        else if (c == '\t') out += "\\t";
        else if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else out += c;
    }
    return out;
}

std::string unescaped(std::string_view s) {
    std::string out;
    out.reserve(s.size());
    for (std::size_t i = 0; i < s.size(); ++i) {
        if (s[i] != '\\' || i + 1 == s.size()) {
            out += s[i];
            continue;
        }
        const char c = s[++i];
        out += c == 't' ? '\t' : c == 'n' ? '\n' : c == 'r' ? '\r' : c;
    }
    return out;
}

std::vector<std::string_view> split_tabs(std::string_view line) {
    std::vector<std::string_view> parts;
    std::size_t at = 0;
    while (true) {
        const std::size_t tab = line.find('\t', at);
        if (tab == std::string_view::npos) {
            parts.push_back(line.substr(at));
            return parts;
        }
        parts.push_back(line.substr(at, tab - at));
        at = tab + 1;
    }
}

template <class Each>
void each_line(std::string_view all, Each&& each) {
    std::size_t at = 0;
    while (at < all.size()) {
        std::size_t end = all.find('\n', at);
        if (end == std::string_view::npos) end = all.size();
        std::string_view line = all.substr(at, end - at);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        each(line);
        at = end + 1;
    }
}

std::uint64_t to_u64(std::string_view s) {
    std::uint64_t v = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') return v;
        v = v * 10 + static_cast<std::uint64_t>(c - '0');
    }
    return v;
}

std::int64_t file_time_number(const stdfs::path& p) {
    std::error_code ec;
    const auto when = fs::last_write_time(p, ec);
    return ec ? 0 : static_cast<std::int64_t>(when.time_since_epoch().count());
}

Bytes read_head(const stdfs::path& p) {
    std::ifstream in(fs::long_path(p), std::ios::binary);
    Bytes head(kHeadBytes);
    if (!in) return {};
    in.read(reinterpret_cast<char*>(head.data()), static_cast<std::streamsize>(head.size()));
    head.resize(static_cast<std::size_t>(in.gcount()));
    return head;
}

bool folder_has_a_file(const stdfs::path& folder) {
    std::error_code ec;
    if (folder.empty() || !stdfs::is_directory(folder, ec)) return false;
    for (stdfs::recursive_directory_iterator it(folder, stdfs::directory_options::skip_permission_denied, ec), end; !ec && it != end; it.increment(ec))
        if (it->is_regular_file(ec)) return true;
    return false;
}

std::string rel_text(const stdfs::path& file, const stdfs::path& files_root) {
    std::string rel = fs::to_text(file.lexically_relative(files_root));
    std::replace(rel.begin(), rel.end(), '\\', '/');
    return rel;
}

std::string name_of(std::string_view rel) {
    const std::size_t slash = rel.find_last_of('/');
    return std::string(slash == std::string_view::npos ? rel : rel.substr(slash + 1));
}

Card make_card(const stdfs::path& file, const stdfs::path& files_root, std::string from) {
    Card card;
    card.rel = rel_text(file, files_root);
    card.name = name_of(card.rel);
    card.from = std::move(from);
    std::error_code ec;
    card.size = stdfs::file_size(fs::long_path(file), ec);
    if (ec) card.size = 0;
    const Bytes head = read_head(file);
    Kind kind = classify(card.name, head);
    card.pile = kind.pile;
    card.format = std::move(kind.format);
    return card;
}

}  // namespace

std::string_view pile_name(Pile pile) {
    switch (pile) {
        case Pile::Programs: return "Programs";
        case Pile::Libraries: return "Libraries";
        case Pile::Drivers: return "Drivers";
        case Pile::Setup: return "Setup";
        case Pile::Text: return "Text";
        case Pile::Help: return "Help";
        case Pile::Fonts: return "Fonts";
        case Pile::Media: return "Media";
        case Pile::Cabinets: return "Cabinets";
        default: return "Data";
    }
}

std::string_view conv_words(Conv conv) {
    switch (conv) {
        case Conv::Queued: return "Queued";
        case Conv::Running: return "Converting";
        case Conv::Done: return "Done";
        case Conv::CannotConvert: return "Can't convert";
        case Conv::Failed: return "Failed";
        case Conv::Cancelled: return "Cancelled";
        default: return "Not converted";
    }
}

Kind classify(std::string_view name, ByteView head) {
    Kind kind;
    const std::optional<Pile> by_name = pile_of_extension(name);
    std::optional<Pile> by_bytes;
    if (starts(head, "MZ")) {
        kind.format = "MZ";
        by_bytes = Pile::Programs;
        const std::uint32_t next = le32(head, 0x3C);
        if (next >= 0x40 && next + 0x40 <= head.size()) {
            if (starts(head.subspan(next), std::string_view("PE\0\0", 4))) {
                kind.format = "PE";
                if (le16(head, next + 22) & 0x2000) by_bytes = Pile::Libraries;
            } else if (starts(head.subspan(next), "NE")) {
                kind.format = "NE";
                if (le16(head, next + 0x0C) & 0x8000) by_bytes = Pile::Libraries;
            } else if (starts(head.subspan(next), "LE") || starts(head.subspan(next), "LX")) {
                kind.format = "LE";
                by_bytes = Pile::Drivers;
            }
        }
    } else if (starts(head, "MSCF")) {
        kind.format = "CAB";
        by_bytes = Pile::Cabinets;
    } else if (starts(head, "SZDD")) {
        kind.format = "SZDD";
    } else if (looks_like_text(head)) {
        kind.format = "text";
        by_bytes = Pile::Text;
    }
    kind.pile = by_name ? *by_name : by_bytes ? *by_bytes : Pile::Data;
    return kind;
}

stdfs::path Board::file_of(const Card& card) const { return root / "files" / fs::from_text(card.rel); }

stdfs::path board_folder(const stdfs::path& boards, const stdfs::path& image) {
    std::error_code ec;
    const std::uint64_t size = stdfs::file_size(image, ec);
    const std::string identity = text::lower(fs::to_text(stdfs::absolute(image, ec))) + "|" + std::to_string(ec ? 0 : size) + "|" + std::to_string(file_time_number(image));
    std::uint64_t hash = 14695981039346656037ull;
    for (const char c : identity) hash = (hash ^ static_cast<std::uint8_t>(c)) * 1099511628211ull;
    std::string stem;
    for (const char c : fs::to_text(image.stem())) stem += (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') ? c : '_';
    return boards / std::format("{}-{:016x}", stem, hash);
}

std::span<const ReviewedEntry> reviewed_entries() {
    static constexpr ReviewedEntry kReviewed[] = {
        {"..\\Microsoft XML Parser for Java.osd", "d1eae9d83b6a26f3db7ef79cf541262daa4833e8fdfacfbcc0c7409904a1e6d6"},
    };
    return kReviewed;
}

stdfs::path place_reviewed(const stdfs::path& folder, std::string_view name, const stdfs::path& file, std::span<const ReviewedEntry> reviewed) {
    const auto entry = std::find_if(reviewed.begin(), reviewed.end(), [&](const ReviewedEntry& r) { return r.name == name; });
    if (entry == reviewed.end()) return file;
    const std::string digest = crypto::to_hex(crypto::sha256(fs::read_file(file)));
    if (digest != entry->sha256) return file;             // the same name with other bytes was not what was read: it stays contained
    const stdfs::path target = folder / file.filename();
    std::error_code ec;
    if (stdfs::exists(target, ec) && crypto::to_hex(crypto::sha256(fs::read_file(target))) != digest) return file;   // never over something else
    fs::remove_tree(target);
    stdfs::rename(file, target);
    const stdfs::path contained = file.parent_path();
    if (stdfs::is_empty(contained, ec)) stdfs::remove(contained, ec);
    return target;
}

Board scan(const stdfs::path& image, const stdfs::path& root, const ScanReport& report, const std::atomic<bool>& cancel) {
    Board board;
    board.root = root;
    board.source = fs::to_text(image);
    board.source_size = stdfs::file_size(image);
    board.source_time = file_time_number(image);
    const stdfs::path files_root = root / "files";
    fs::make_dirs(files_root);

    ScanProgress progress;
    auto last = std::chrono::steady_clock::now() - std::chrono::seconds(1);
    const auto say = [&](bool force) {
        if (!report) return;
        const auto now = std::chrono::steady_clock::now();
        if (!force && now - last < std::chrono::milliseconds(100)) return;
        last = now;
        progress.cards = board.cards.size();
        report(progress);
    };

    struct Pending {
        stdfs::path file;
        std::string rel;
        int depth = 0;
    };
    std::deque<Pending> cabinets;
    // A set can be stored one cabinet per outer cabinet, so its parts do not lie side by side. Every cabinet opened is known by (name, set);
    // one whose neighbours are not found yet waits, and when all are known they are linked together in one folder for the decoder.
    std::map<std::pair<std::string, std::uint16_t>, stdfs::path> known;
    std::vector<Pending> waiting;
    bool opened_any = false, last_try = false;
    const stdfs::path set_folders = root / "sets";
    const auto neighbour_of = [&](const stdfs::path& file, const formats::cab::SetLink& link, std::uint16_t set) -> std::optional<stdfs::path> {
        std::error_code ec;
        const stdfs::path beside = file.parent_path() / fs::from_text(link.cabinet);
        if (stdfs::exists(beside, ec)) return beside;
        const auto it = known.find({text::lower(link.cabinet), set});
        return it == known.end() ? std::nullopt : std::optional<stdfs::path>(it->second);
    };
    // The whole set this cabinet is in, each part as (its own name, where it is); nullopt when a part is missing.
    const auto whole_set = [&](const stdfs::path& file, const formats::cab::Cabinet& headers) -> std::optional<std::vector<std::pair<std::string, stdfs::path>>> {
        std::vector<std::pair<std::string, stdfs::path>> parts{{fs::to_text(file.filename()), file}};
        for (const bool forward : {false, true}) {
            formats::cab::Cabinet at = headers;
            stdfs::path here = file;
            for (int guard = 0; guard < 256; ++guard) {
                const std::optional<formats::cab::SetLink>& link = forward ? at.next : at.prev;
                if (!link) break;
                const auto next = neighbour_of(here, *link, headers.set_id);
                if (!next) return std::nullopt;
                parts.emplace_back(link->cabinet, *next);
                at = formats::cab::read(*next);
                here = *next;
            }
        }
        return parts;
    };
    const auto add_file = [&](const stdfs::path& file, const std::string& from, int depth) {
        Card card = make_card(file, files_root, from);
        if (card.format == "CAB") cabinets.push_back({file, card.rel, depth});
        board.cards.push_back(std::move(card));
    };

    // ---- the disc: every volume, each after the first in a folder of its own (as `media extract` lays them out)
    auto volumes = formats::media::volumes(image);
    std::uint64_t disc_total = 0, disc_done = 0;
    for (const auto& v : volumes)
        for (const formats::media::Entry& e : v->files()) disc_total += e.is_dir ? 0 : 1;
    progress.stage = "Extracting the disc";
    for (std::size_t vi = 0; vi < volumes.size() && !cancel; ++vi) {
        const stdfs::path base = vi == 0 ? files_root : files_root / std::format("volume{}", vi + 1);
        for (const formats::media::Entry& e : volumes[vi]->files()) {
            if (cancel) break;
            if (e.is_dir) continue;
            ++disc_done;
            try {
                const stdfs::path target = base / formats::media::safe_relative_path(e.path);
                std::error_code ec;
                if (stdfs::file_size(fs::long_path(target), ec) != e.size || ec) fs::write_file(target, volumes[vi]->read(e));
                add_file(target, {}, 0);
            } catch (const std::exception& error) {
                board.failures.push_back({e.path, error.what()});
            }
            progress.detail = count_words(disc_done) + " of " + count_words(disc_total);
            progress.percent = disc_total == 0 ? 50 : static_cast<int>(disc_done * 50 / disc_total);
            say(false);
        }
    }

    // ---- the cabinets, and the cabinets in them. A file continued from the cabinet before is written by that cabinet, whole.
    progress.stage = "Opening cabinets";
    std::uint64_t opened = 0;
    while (!cancel) {
        if (cabinets.empty()) {
            if (waiting.empty()) break;
            last_try = !opened_any;   // (a pass that opened nothing new: what still waits is told what is missing)
            opened_any = false;
            cabinets.assign(waiting.begin(), waiting.end());
            waiting.clear();
        }
        const Pending cabinet = std::move(cabinets.front());
        cabinets.pop_front();
        ++opened;
        progress.detail = count_words(opened) + " of " + count_words(opened + cabinets.size()) + " \xC2\xB7 " + name_of(cabinet.rel);
        progress.percent = 50 + static_cast<int>(opened * 50 / (opened + cabinets.size()));
        say(false);
        if (cabinet.depth >= kMaxCabinetDepth) {
            board.failures.push_back({cabinet.rel, "cabinets nested too deep"});
            continue;
        }
        try {
            const formats::cab::Cabinet headers = formats::cab::read(cabinet.file);
            known.try_emplace({text::lower(name_of(cabinet.rel)), headers.set_id}, cabinet.file);
            std::error_code ec;
            stdfs::path source = cabinet.file;
            bool has_previous = headers.prev && stdfs::exists(cabinet.file.parent_path() / fs::from_text(headers.prev->cabinet), ec);
            const bool needs_neighbours = (headers.prev && !neighbour_of(cabinet.file, *headers.prev, headers.set_id)) || (headers.next && !neighbour_of(cabinet.file, *headers.next, headers.set_id)) ||
                                          (headers.prev && !stdfs::exists(cabinet.file.parent_path() / fs::from_text(headers.prev->cabinet), ec)) ||
                                          (headers.next && !stdfs::exists(cabinet.file.parent_path() / fs::from_text(headers.next->cabinet), ec));
            if (needs_neighbours) {
                const auto parts = whole_set(cabinet.file, headers);
                if (!parts && !last_try) {
                    waiting.push_back(cabinet);
                    --opened;
                    continue;
                }
                if (parts) {
                    const stdfs::path folder = set_folders / std::format("set{:05}", headers.set_id);
                    fs::make_dirs(folder);
                    for (const auto& [part, where] : *parts) {
                        const stdfs::path link = folder / fs::from_text(part);
                        if (stdfs::exists(link, ec)) continue;
                        stdfs::create_hard_link(where, link, ec);
                        if (ec) stdfs::copy_file(where, link, stdfs::copy_options::overwrite_existing);
                        ec.clear();
                    }
                    source = folder / fs::from_text(parts->front().first);
                    has_previous = headers.prev.has_value();
                }
            }
            opened_any = true;
            std::vector<std::string> names;
            std::set<std::string> seen;
            for (const formats::cab::FileEntry& f : headers.files) {
                const bool continued = f.folder == formats::cab::kFromPrev || f.folder == formats::cab::kPrevAndNext;
                if (continued && has_previous) continue;
                if (seen.insert(text::lower(f.name)).second) names.push_back(f.name);
            }
            stdfs::path out = cabinet.file;
            out += ".d";
            const auto written = formats::cab::extract_contained(source, out, names);   // (a name that climbs out of the folder is kept inside it)
            ++board.cabinets;
            for (const formats::cab::Extracted& x : written) {
                if (cancel) break;
                try {
                    add_file(place_reviewed(out, x.name, x.path, reviewed_entries()), cabinet.rel, cabinet.depth + 1);
                } catch (const std::exception& error) {
                    board.failures.push_back({cabinet.rel + "/" + x.name, error.what()});
                }
            }
        } catch (const std::exception& error) {
            board.failures.push_back({cabinet.rel, error.what()});
        }
        say(false);
    }

    fs::remove_tree(set_folders);
    board.complete = !cancel;
    std::sort(board.cards.begin(), board.cards.end(), [](const Card& a, const Card& b) { return a.rel < b.rel; });
    progress.stage = board.complete ? "Done" : "Cancelled";
    progress.detail = count_words(board.cards.size()) + " files";
    progress.percent = board.complete ? 100 : progress.percent;
    say(true);
    return board;
}

void save_states(const Board& board) {
    std::string out(kStateHeader);
    out += '\n';
    for (const Card& c : board.cards) {
        if (c.conv == Conv::None || c.conv == Conv::Queued || c.conv == Conv::Running) continue;
        out += escaped(c.rel) + '\t' + std::to_string(static_cast<int>(c.conv)) + '\t' + escaped(c.output) + '\t' + escaped(c.note) + '\n';
    }
    fs::write_file_atomic(board.root / "state.tsv", as_bytes(out));
}

void save(const Board& board) {
    std::string out(kBoardHeader);
    out += '\n';
    out += "source\t" + escaped(board.source) + '\t' + std::to_string(board.source_size) + '\t' + std::to_string(board.source_time) + '\t' +
           std::to_string(board.cabinets) + '\t' + (board.complete ? "1" : "0") + '\n';
    for (const Card& c : board.cards)
        out += "card\t" + escaped(c.rel) + '\t' + escaped(c.from) + '\t' + std::to_string(c.size) + '\t' + std::to_string(static_cast<int>(c.pile)) + '\t' + escaped(c.format) + '\n';
    for (const Failure& f : board.failures) out += "fail\t" + escaped(f.path) + '\t' + escaped(f.why) + '\n';
    fs::make_dirs(board.root);
    fs::write_file_atomic(board.root / "board.tsv", as_bytes(out));
    save_states(board);
}

std::optional<Board> load(const stdfs::path& root) {
    std::error_code ec;
    if (!stdfs::is_regular_file(root / "board.tsv", ec)) return std::nullopt;
    Board board;
    board.root = root;
    const std::string all = fs::read_text(root / "board.tsv");
    if (!all.starts_with(kBoardHeader)) return std::nullopt;
    each_line(all, [&](std::string_view line) {
        const std::vector<std::string_view> p = split_tabs(line);
        if (p[0] == "source" && p.size() >= 6) {
            board.source = unescaped(p[1]);
            board.source_size = to_u64(p[2]);
            board.source_time = static_cast<std::int64_t>(to_u64(p[3]));
            board.cabinets = to_u64(p[4]);
            board.complete = p[5] == "1";
        } else if (p[0] == "card" && p.size() >= 6) {
            Card c;
            c.rel = unescaped(p[1]);
            c.name = name_of(c.rel);
            c.from = unescaped(p[2]);
            c.size = to_u64(p[3]);
            c.pile = static_cast<Pile>(std::min<std::uint64_t>(to_u64(p[4]), kPileCount - 1));
            c.format = unescaped(p[5]);
            board.cards.push_back(std::move(c));
        } else if (p[0] == "fail" && p.size() >= 3) {
            board.failures.push_back({unescaped(p[1]), unescaped(p[2])});
        }
    });
    if (!board.complete) return std::nullopt;
    if (stdfs::is_regular_file(root / "state.tsv", ec)) {
        const std::string states = fs::read_text(root / "state.tsv");
        if (states.starts_with(kStateHeader)) {
            std::unordered_map<std::string_view, std::size_t> by_rel;
            by_rel.reserve(board.cards.size());
            for (std::size_t i = 0; i < board.cards.size(); ++i) by_rel.emplace(board.cards[i].rel, i);
            each_line(states, [&](std::string_view line) {
                if (line.starts_with('#')) return;
                const std::vector<std::string_view> p = split_tabs(line);
                if (p.size() < 4) return;
                const std::string rel = unescaped(p[0]);
                const auto it = by_rel.find(rel);
                if (it == by_rel.end()) return;
                Card& c = board.cards[it->second];
                const std::uint64_t number = to_u64(p[1]);
                if (number > static_cast<std::uint64_t>(Conv::Cancelled)) return;
                const Conv conv = static_cast<Conv>(number);
                if (conv == Conv::Queued || conv == Conv::Running) return;
                c.output = unescaped(p[2]);
                c.note = unescaped(p[3]);
                c.conv = conv;
                if (conv == Conv::Done && !folder_has_a_file(fs::from_text(c.output))) {   // the result has gone: the card goes back to work
                    c.conv = Conv::None;
                    c.output.clear();
                }
            });
        }
    }
    return board;
}

void record(Card& card, Conv conv, const stdfs::path& output, std::string note) {
    card.note = std::move(note);
    card.output.clear();
    if (conv == Conv::Done && !folder_has_a_file(output)) {
        card.conv = Conv::Failed;
        if (card.note.empty()) card.note = "The conversion reported success but left no files.";
        return;
    }
    card.conv = conv;
    if (conv == Conv::Done) card.output = fs::to_text(output);
}

Stacks build_stacks(const Board& board, std::string_view query, const StackOptions& options) {
    Stacks stacks;
    const std::string wanted = text::lower(query);
    constexpr std::uint32_t kNone = 0xFFFFFFFFu;
    std::vector<std::uint32_t> master(options.group != nullptr ? board.cards.size() : 0, kNone);   // per group: the card that stands for it
    std::vector<std::uint32_t> shown;
    shown.reserve(board.cards.size());
    for (std::size_t i = 0; i < board.cards.size(); ++i) {
        const Card& c = board.cards[i];
        if (options.allowed != nullptr && !(*options.allowed)[i]) continue;
        if (!wanted.empty() && text::lower(c.rel).find(wanted) == std::string::npos) continue;
        if (options.group == nullptr) {
            shown.push_back(static_cast<std::uint32_t>(i));
            continue;
        }
        std::uint32_t& m = master[(*options.group)[i]];
        if (m == kNone || (c.conv == Conv::Done && board.cards[m].conv != Conv::Done)) m = static_cast<std::uint32_t>(i);
    }
    for (const std::uint32_t m : master)
        if (m != kNone) shown.push_back(m);
    for (const std::uint32_t i : shown) {
        const Card& c = board.cards[i];
        Stack& s = stacks[static_cast<std::size_t>(c.pile)];
        (c.conv == Conv::Done ? s.done : s.todo).push_back(i);
    }
    std::vector<std::string> keys(board.cards.size());
    for (Stack& s : stacks) {
        for (const std::vector<std::uint32_t>* list : {&s.todo, &s.done})
            for (const std::uint32_t i : *list) keys[i] = text::lower(board.cards[i].name);
        const auto by_name = [&](std::uint32_t a, std::uint32_t b) { return keys[a] != keys[b] ? keys[a] < keys[b] : a < b; };
        std::sort(s.todo.begin(), s.todo.end(), by_name);
        std::sort(s.done.begin(), s.done.end(), by_name);
    }
    return stacks;
}

std::string size_words(std::uint64_t bytes) {
    if (bytes < 1024) return std::format("{} B", bytes);
    if (bytes < 1024 * 1024) return std::format("{:.1f} KB", static_cast<double>(bytes) / 1024);
    if (bytes < 1024ull * 1024 * 1024) return std::format("{:.1f} MB", static_cast<double>(bytes) / (1024.0 * 1024));
    return std::format("{:.2f} GB", static_cast<double>(bytes) / (1024.0 * 1024 * 1024));
}

std::string count_words(std::uint64_t count) {
    std::string digits = std::to_string(count), out;
    for (std::size_t i = 0; i < digits.size(); ++i) {
        if (i > 0 && (digits.size() - i) % 3 == 0) out += ',';
        out += digits[i];
    }
    return out;
}

}  // namespace cb::app::board
