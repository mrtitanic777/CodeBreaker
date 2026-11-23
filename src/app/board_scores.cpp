// app/board_scores.cpp -- see board_scores.hpp.
#include "app/board_scores.hpp"

#include <format>
#include <system_error>
#include <vector>

#include "app/board.hpp"
#include "core/fs.hpp"

namespace cb::app::board {

namespace {

constexpr std::string_view kScoresHeader = "card\tformat\tresult\tfunctions\tidentical\tsame_size\tdifferent\tno_compile\tother\tseconds";

std::vector<std::string_view> score_fields(std::string_view line) {
    std::vector<std::string_view> out;
    for (;;) {
        const std::size_t tab = line.find('\t');
        out.push_back(line.substr(0, tab));
        if (tab == std::string_view::npos) return out;
        line.remove_prefix(tab + 1);
    }
}

std::uint64_t score_number(std::string_view s) {
    std::uint64_t n = 0;
    for (const char c : s) {
        if (c < '0' || c > '9') return 0;
        n = n * 10 + static_cast<std::uint64_t>(c - '0');
    }
    return n;
}

}  // namespace

std::optional<CScores> parse_c_scores(std::string_view text) {
    if (!text.starts_with(kScoresHeader)) return std::nullopt;
    CScores out;
    bool first = true;
    while (!text.empty()) {
        const std::size_t end = text.find('\n');
        std::string_view line = text.substr(0, end);
        text.remove_prefix(end == std::string_view::npos ? text.size() : end + 1);
        if (!line.empty() && line.back() == '\r') line.remove_suffix(1);
        if (first || line.empty()) {
            first = false;
            continue;
        }
        const std::vector<std::string_view> p = score_fields(line);
        if (p.size() < 10) continue;      // (a row still being written)
        CScore s;
        s.format = p[1];
        s.result = p[2];
        s.functions = score_number(p[3]);
        s.identical = score_number(p[4]);
        s.same_size = score_number(p[5]);
        s.different = score_number(p[6]);
        s.no_compile = score_number(p[7]);
        s.library = score_number(p[8]);
        if (s.scored()) {
            ++out.scored;
            out.total.functions += s.functions;
            out.total.identical += s.identical;
            out.total.same_size += s.same_size;
            out.total.different += s.different;
            out.total.no_compile += s.no_compile;
            out.total.library += s.library;
        } else {
            ++out.unscored;
        }
        out.by_card.insert_or_assign(std::string(p[0]), std::move(s));
    }
    return out;
}

std::optional<CScores> load_c_scores(const std::filesystem::path& root) {
    std::error_code ec;
    if (!std::filesystem::is_regular_file(root / "c_scores.tsv", ec)) return std::nullopt;
    return parse_c_scores(fs::read_text(root / "c_scores.tsv"));
}

std::string score_percent(std::uint64_t n, std::uint64_t total) {
    return total == 0 ? std::string("0.0%") : std::format("{:.1f}%", 100.0 * static_cast<double>(n) / static_cast<double>(total));
}

std::string c_score_words(const CScores& scores) {
    return "C byte-identical: " + count_words(scores.total.identical) + " of " + count_words(scores.total.functions) + " functions (" +
           score_percent(scores.total.identical, scores.total.functions) + ") in " + count_words(scores.scored) + " programs";
}

}  // namespace cb::app::board
