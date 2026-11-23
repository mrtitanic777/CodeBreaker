// app/board_c_leaders.cpp -- see board_c_leaders.hpp.
#include "app/board_c_leaders.hpp"

#include <algorithm>
#include <limits>

namespace cb::app::board {

namespace {

bool c_leader_before(const CLeader& a, const CLeader& b) {
    if (a.same_size != b.same_size) return a.same_size;
    if (a.closeness != b.closeness) return a.closeness > b.closeness;
    if (a.size != b.size) return a.size > b.size;
    if (a.rel != b.rel) return a.rel < b.rel;
    return a.function < b.function;
}

void c_leaders_trim(CLeaders& leaders) {
    std::sort(leaders.top.begin(), leaders.top.end(), c_leader_before);
    if (leaders.top.size() > kCLeadersKept) leaders.top.resize(kCLeadersKept);
}

}  // namespace

void add_c_leaders(CLeaders& into, std::string_view rel, const CNearMisses& misses) {
    const std::size_t slash = rel.rfind('/');
    const std::string program(slash == std::string_view::npos ? rel : rel.substr(slash + 1));
    for (const CNearMiss& miss : misses.closest) {
        if (miss.size < kCLeaderMinBytes) continue;
        ++into.candidates;
        into.top.push_back({std::string(rel), program, miss.name, miss.words, miss.closeness, miss.same_size, miss.size});
    }
    if (into.top.size() >= kCLeadersKept * 4) c_leaders_trim(into);   // (the whole OS is half a million functions: only the best are held)
}

std::optional<CLeaders> build_c_leaders(const std::filesystem::path& root, const CScores& scores, const std::atomic<bool>& cancel) {
    CLeaders out;
    out.scored = scores.scored;
    for (const auto& [rel, row] : scores.by_card) {
        if (cancel) return std::nullopt;
        if (!row.scored() || row.functions == 0) continue;
        const std::optional<CNearMisses> misses = load_c_near_misses(root, rel, std::numeric_limits<std::size_t>::max());
        if (!misses) continue;
        ++out.programs;
        add_c_leaders(out, rel, *misses);
    }
    c_leaders_trim(out);
    return out;
}

}  // namespace cb::app::board
