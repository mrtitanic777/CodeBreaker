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

std::optional<CNearMisses> parse_c_near_misses(std::string_view text) {
    Json all;
    if (!json::try_parse(text, all) || !all.is_object()) return std::nullopt;
    std::vector<CNearMiss> found;
    for (auto it = all.begin(); it != all.end(); ++it) {
        const Json& entry = it.value();
        if (!entry.is_object()) continue;
        const auto verdict = entry.find("verdict");
        if (verdict == entry.end() || !verdict->is_string()) continue;
        const std::string word = verdict->get<std::string>();
        if (word != "same size" && word != "different") continue;   // (identical, the run-time library and what did not compile are no near misses)
        CNearMiss miss;
        miss.name = it.key();
        miss.same_size = word == "same size";
        if (const auto total = entry.find("total"); total != entry.end()) {   // 16-bit: units that differ, of how many
            const std::uint64_t of = c_json_count(entry, "total"), differing = c_json_count(entry, "differing");
            if (of == 0 || differing >= of) continue;
            miss.closeness = 1.0 - static_cast<double>(differing) / static_cast<double>(of);
            miss.size = of;
            miss.words = std::format("{} of {} units differ", count_words(differing), count_words(of));
        } else {
            const auto share = entry.find("share");
            if (share == entry.end() || !share->is_number()) continue;
            miss.closeness = share->get<double>();
            if (miss.closeness <= 0) continue;
            miss.size = c_json_count(entry, "size");
            miss.words = miss.same_size ? c_tenths_down(miss.closeness) + " of bytes equal"
                                        : std::format("{} similar, {} vs {} B", c_tenths_down(miss.closeness), count_words(miss.size), count_words(c_json_count(entry, "original")));
        }
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
    if (found.size() > kCNearMissesShown) found.resize(kCNearMissesShown);
    out.closest = std::move(found);
    return out;
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

std::optional<CNearMisses> load_c_near_misses(const std::filesystem::path& root, std::string_view rel) {
    const std::filesystem::path file = root / "c_scores" / c_functions_file_name(rel);
    std::error_code ec;
    if (!std::filesystem::is_regular_file(file, ec)) return std::nullopt;
    try {
        return parse_c_near_misses(fs::read_text(file));
    } catch (const std::exception&) {
        return std::nullopt;   // (the scorer has it open)
    }
}

}  // namespace cb::app::board
