// app/board_c_functions.cpp -- see board_c_functions.hpp.
#include "app/board_c_functions.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <nlohmann/json.hpp>
#include <system_error>

#include "core/crypto.hpp"
#include "core/fs.hpp"
#include "core/json.hpp"

namespace cb::app::board {

namespace {

// To a tenth, rounded down: a function that is not identical never reads 100.0%.
std::string c_tenths_down(double share) { return std::format("{:.1f}%", std::floor(std::clamp(share, 0.0, 1.0) * 1000.0) / 10.0); }

std::uint64_t c_json_count(const Json& entry, const char* key) {
    const auto it = entry.find(key);
    return it != entry.end() && it->is_number_unsigned() ? it->get<std::uint64_t>() : 0;
}

}  // namespace

std::string c_measure_words(CVerdict verdict, CMeasure measure, double closeness, std::uint32_t a, std::uint32_t b) {
    switch (verdict) {
        case CVerdict::Identical: return "byte-identical";
        case CVerdict::NoCompile: return "will not compile";
        case CVerdict::Library: return "run-time library";
        default: break;
    }
    switch (measure) {
        case CMeasure::BytesEqual: return c_tenths_down(closeness) + " of bytes equal";
        case CMeasure::Similar: return std::format("{} similar, {} vs {} B", c_tenths_down(closeness), count_words(a), count_words(b));
        case CMeasure::Units: return std::format("{} of {} units differ", count_words(a), count_words(b));
        case CMeasure::None: break;
    }
    return verdict == CVerdict::SameSize ? "same size" : "different";
}

std::optional<std::vector<CFunctionRow>> parse_c_function_rows(std::string_view text) {
    Json all;
    if (!json::try_parse(text, all) || !all.is_object()) return std::nullopt;
    std::vector<CFunctionRow> rows;
    rows.reserve(all.size());
    for (auto it = all.begin(); it != all.end(); ++it) {
        const Json& entry = it.value();
        if (!entry.is_object()) continue;
        const auto verdict = entry.find("verdict");
        if (verdict == entry.end() || !verdict->is_string()) continue;
        const std::string word = verdict->get<std::string>();
        CFunctionRow row;
        row.name = it.key();
        if (word == "identical") row.verdict = CVerdict::Identical;
        else if (word == "same size") row.verdict = CVerdict::SameSize;
        else if (word == "different") row.verdict = CVerdict::Different;
        else if (word == "no-compile") row.verdict = CVerdict::NoCompile;
        else if (word == "identified") row.verdict = CVerdict::Library;
        else continue;
        if (row.verdict == CVerdict::Identical) row.closeness = 1;
        if (row.verdict == CVerdict::SameSize || row.verdict == CVerdict::Different) {
            if (entry.find("total") != entry.end()) {   // 16-bit: units that differ, of how many
                const std::uint64_t of = c_json_count(entry, "total"), differing = c_json_count(entry, "differing");
                row.measure = CMeasure::Units;
                row.a = static_cast<std::uint32_t>(differing);
                row.b = static_cast<std::uint32_t>(of);
                row.closeness = of == 0 || differing >= of ? 0.0 : 1.0 - static_cast<double>(differing) / static_cast<double>(of);
            } else if (const auto share = entry.find("share"); share != entry.end() && share->is_number()) {
                row.measure = row.verdict == CVerdict::SameSize ? CMeasure::BytesEqual : CMeasure::Similar;
                row.closeness = share->get<double>();
                row.a = static_cast<std::uint32_t>(c_json_count(entry, "size"));
                row.b = static_cast<std::uint32_t>(c_json_count(entry, "original"));
            }
            row.near_miss = row.measure != CMeasure::None && row.closeness > 0;
        }
        rows.push_back(std::move(row));
    }
    return rows;
}

CNearMisses c_near_misses_of(const std::vector<CFunctionRow>& rows, std::size_t keep) {
    std::vector<CNearMiss> found;
    for (const CFunctionRow& row : rows) {
        if (!row.near_miss) continue;
        CNearMiss miss;
        miss.name = row.name;
        miss.same_size = row.verdict == CVerdict::SameSize;
        miss.closeness = row.closeness;
        miss.size = row.measure == CMeasure::Units ? row.b : row.a;
        miss.words = c_measure_words(row.verdict, row.measure, row.closeness, row.a, row.b);
        found.push_back(std::move(miss));
    }
    std::sort(found.begin(), found.end(), [](const CNearMiss& a, const CNearMiss& b) {
        if (a.same_size != b.same_size) return a.same_size;
        if (a.closeness != b.closeness) return a.closeness > b.closeness;
        if (a.size != b.size) return a.size > b.size;
        return a.name < b.name;
    });
    CNearMisses out;
    out.candidates = found.size();
    if (found.size() > keep) found.resize(keep);
    out.closest = std::move(found);
    return out;
}

std::optional<CNearMisses> parse_c_near_misses(std::string_view text, std::size_t keep) {
    const std::optional<std::vector<CFunctionRow>> rows = parse_c_function_rows(text);
    if (!rows) return std::nullopt;
    return c_near_misses_of(*rows, keep);
}

std::string c_functions_file_name(std::string_view rel) {
    const crypto::Sha256Digest digest = crypto::sha256(as_bytes(rel));
    return crypto::to_hex(digest).substr(0, 16) + ".json";
}

std::string c_scored_rel(const Board& board, const Index& index, std::uint32_t card, const CScores& scores) {
    if (card >= board.cards.size() || index.nodes.size() != board.cards.size()) return {};
    const Node& node = index.nodes[card];
    std::string best;
    bool best_scored = false;
    const auto consider = [&](std::uint32_t i) {
        const auto it = scores.by_card.find(board.cards[i].rel);
        if (it == scores.by_card.end() || (!best.empty() && (best_scored || !it->second.scored()))) return;
        best = it->first;
        best_scored = it->second.scored();
    };
    consider(card);
    if (!node.sha256.empty())
        for (std::uint32_t i = 0; i < index.nodes.size(); ++i)
            if (index.nodes[i].group == node.group) consider(i);
    return best;
}

std::optional<CNearMisses> load_c_near_misses(const std::filesystem::path& root, std::string_view rel, std::size_t keep) {
    const std::filesystem::path file = root / "c_scores" / c_functions_file_name(rel);
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec)) return std::nullopt;
    try {
        return parse_c_near_misses(fs::read_text(file), keep);
    } catch (const std::exception&) {
        return std::nullopt;   // (the scorer has it open)
    }
}

}  // namespace cb::app::board
