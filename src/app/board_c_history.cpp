// app/board_c_history.cpp -- see board_c_history.hpp.
#include "app/board_c_history.hpp"

#include <algorithm>
#include <array>
#include <format>
#include <system_error>

#include "app/board.hpp"
#include "core/fs.hpp"

namespace cb::app::board {

namespace {

bool c_history_digits(std::string_view s) { return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return c >= '0' && c <= '9'; }); }

int c_history_number(std::string_view s) {
    int n = 0;
    for (const char c : s) n = n * 10 + (c - '0');
    return n;
}

}  // namespace

CHistory diff_c_scores(const CScores& before, const CScores& now, std::string snapshot) {
    CHistory h;
    h.snapshot = std::move(snapshot);
    h.identical_before = before.total.identical;
    h.identical_now = now.total.identical;
    h.scored_before = before.scored;
    h.scored_now = now.scored;
    std::vector<CHistoryRow> moved, added;
    for (const auto& [rel, score] : now.by_card) {
        if (!score.scored()) continue;
        const std::size_t slash = rel.rfind('/');
        CHistoryRow row{rel, slash == std::string::npos ? rel : rel.substr(slash + 1)};
        row.identical_now = score.identical;
        row.functions_now = score.functions;
        const auto was = before.by_card.find(rel);
        if (was == before.by_card.end() || !was->second.scored()) {
            ++h.newly_scored;
            added.push_back(std::move(row));
            continue;
        }
        row.scored_before = true;
        row.identical_before = was->second.identical;
        if (row.delta() == 0) continue;
        ++(row.delta() > 0 ? h.better : h.worse);
        moved.push_back(std::move(row));
    }
    std::sort(moved.begin(), moved.end(), [](const CHistoryRow& a, const CHistoryRow& b) {
        const auto size = [](std::int64_t d) { return d < 0 ? -d : d; };
        if (size(a.delta()) != size(b.delta())) return size(a.delta()) > size(b.delta());
        return a.rel < b.rel;
    });
    std::sort(added.begin(), added.end(), [](const CHistoryRow& a, const CHistoryRow& b) { return a.program != b.program ? a.program < b.program : a.rel < b.rel; });
    h.changes = std::move(moved);
    h.changes.insert(h.changes.end(), std::make_move_iterator(added.begin()), std::make_move_iterator(added.end()));
    return h;
}

std::string c_history_words(const CHistory& h) {
    const std::int64_t delta = static_cast<std::int64_t>(h.identical_now) - static_cast<std::int64_t>(h.identical_before);
    std::string words;
    const auto part = [&](const std::string& text) { words += (words.empty() ? "" : " \xC2\xB7 ") + text; };
    if (delta != 0) part(std::format("{}{} byte-identical functions", delta > 0 ? "+" : "-", count_words(static_cast<std::uint64_t>(delta > 0 ? delta : -delta))));
    if (h.better > 0) part(count_words(h.better) + (h.better == 1 ? " program better" : " programs better"));
    if (h.worse > 0) part(count_words(h.worse) + (h.worse == 1 ? " program worse" : " programs worse"));
    if (h.newly_scored > 0) part(count_words(h.newly_scored) + " newly scored");
    return words.empty() ? std::string("No change") : words;
}

std::string c_snapshot_name(const std::chrono::local_seconds& when) { return std::format("{:%Y%m%d-%H%M%S}.tsv", when); }

std::string c_snapshot_words(std::string_view name) {
    if (name.size() < 15 || name[8] != '-' || !c_history_digits(name.substr(0, 8)) || !c_history_digits(name.substr(9, 6))) return std::string(name);
    static constexpr std::array<std::string_view, 12> kMonths = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    const int month = c_history_number(name.substr(4, 2)), day = c_history_number(name.substr(6, 2)), hour = c_history_number(name.substr(9, 2)), minute = c_history_number(name.substr(11, 2));
    if (month < 1 || month > 12 || hour > 23 || minute > 59) return std::string(name);
    return std::format("{} {}, {} at {}:{:02} {}", kMonths[static_cast<std::size_t>(month - 1)], day, name.substr(0, 4), hour % 12 == 0 ? 12 : hour % 12, minute, hour < 12 ? "AM" : "PM");
}

std::optional<std::pair<std::string, std::string>> load_latest_c_snapshot(const std::filesystem::path& root) {
    const std::filesystem::path folder = root / "c_history";
    std::error_code ec;
    std::string newest;
    for (std::filesystem::directory_iterator it(folder, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (it->is_regular_file(ec) && name.ends_with(".tsv") && name > newest) newest = name;   // (the names sort by time)
    }
    if (newest.empty()) return std::nullopt;
    try {
        return std::pair{newest, fs::read_text(folder / newest)};
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

std::filesystem::path save_c_snapshot(const std::filesystem::path& root, std::string_view name, std::string_view table) {
    const std::filesystem::path file = root / "c_history" / std::string(name);
    fs::make_dirs(file.parent_path());
    fs::write_file_atomic(file, as_bytes(table));
    return file;
}

}  // namespace cb::app::board
