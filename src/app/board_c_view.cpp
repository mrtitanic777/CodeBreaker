// app/board_c_view.cpp -- see board_c_view.hpp.
#include "app/board_c_view.hpp"

#include <algorithm>

namespace cb::app::board {

namespace {

bool c_view_is_binary(std::string_view format) { return format == "PE" || format == "NE" || format == "LE" || format == "MZ"; }

CMark c_mark_of_row(const CScore& row) {
    CMark mark;
    if (!row.scored()) {
        mark.state = row.result == "timeout" ? CState::OutOfTime : CState::ScorerFailed;
        return mark;
    }
    mark.state = row.functions == 0 ? CState::NoFunctions : CState::Scored;
    mark.functions = row.functions;
    mark.identical = row.identical;
    mark.same_size = row.same_size;
    mark.no_compile = row.no_compile;
    return mark;
}

// a's n of its functions is a larger share than b's (whole numbers: no rounding decides an order).
bool c_larger_share(std::uint64_t a_n, std::uint64_t a_of, std::uint64_t b_n, std::uint64_t b_of) { return a_n * b_of > b_n * a_of; }

bool c_ranks_before(const CMark& a, const CMark& b, CView view) {
    switch (view) {
        case CView::MostIdentical:
            if (a.identical * b.functions != b.identical * a.functions) return c_larger_share(a.identical, a.functions, b.identical, b.functions);
            return a.identical > b.identical;
        case CView::NearMisses:
            if (a.same_size * b.functions != b.same_size * a.functions) return c_larger_share(a.same_size, a.functions, b.same_size, b.functions);
            return a.same_size > b.same_size;
        case CView::WillNotCompile: return a.no_compile > b.no_compile;
        default: return false;
    }
}

}  // namespace

CMarks build_c_marks(const Board& board, const Index* index, const CScores& scores) {
    struct Group {
        const CScore* row = nullptr;
        bool converted = false, program = false;
    };
    const std::size_t n = board.cards.size();
    const bool linked = index != nullptr && index->nodes.size() == n;
    const auto group_of = [&](std::size_t i) -> std::size_t { return linked && !index->nodes[i].sha256.empty() ? index->nodes[i].group : i; };
    std::vector<Group> groups(n);
    for (std::size_t i = 0; i < n; ++i) {
        const Card& c = board.cards[i];
        Group& g = groups[group_of(i)];
        g.converted = g.converted || c.conv == Conv::Done;
        g.program = g.program || c_view_is_binary(c.format);
        const auto it = scores.by_card.find(c.rel);
        if (it != scores.by_card.end() && (g.row == nullptr || (it->second.scored() && !g.row->scored()))) g.row = &it->second;   // (one copy scored is all of them scored)
    }
    CMarks out;
    out.by_card.resize(n);
    std::vector<CMark> marks(n);
    for (std::size_t at = 0; at < n; ++at) {
        const Group& g = groups[at];
        CMark& mark = marks[at];
        if (g.row != nullptr) mark = c_mark_of_row(*g.row);
        else if (g.converted && g.program) mark.state = CState::NotScored;
        if (mark.state == CState::None) continue;
        CSummary& s = out.summary;
        CPileScore& pile = out.by_pile[static_cast<std::size_t>(board.cards[at].pile)];
        switch (mark.state) {
            case CState::NotScored:
                ++s.not_scored;
                ++pile.unscored;
                break;
            case CState::OutOfTime:
                ++s.out_of_time;
                ++pile.unscored;
                break;
            case CState::ScorerFailed:
                ++s.scorer_failed;
                ++pile.unscored;
                break;
            default:
                ++s.scored;
                ++pile.scored;
                pile.functions += g.row->functions;
                pile.identical += g.row->identical;
                s.functions += g.row->functions;
                s.identical += g.row->identical;
                s.same_size += g.row->same_size;
                s.different += g.row->different;
                s.no_compile += g.row->no_compile;
                s.library += g.row->library;
                break;
        }
        for (std::size_t v = 0; v < kCViews; ++v) s.in_view[v] += in_c_view(mark, static_cast<CView>(v + 1)) ? 1 : 0;
    }
    for (std::size_t i = 0; i < n; ++i) out.by_card[i] = marks[group_of(i)];
    return out;
}

bool in_c_view(const CMark& mark, CView view) {
    switch (view) {
        case CView::All: return true;
        case CView::MostIdentical: return mark.state == CState::Scored && mark.identical > 0;
        case CView::NearMisses: return mark.state == CState::Scored && mark.same_size > 0;
        case CView::WillNotCompile: return mark.state == CState::Scored && mark.no_compile > 0;
        case CView::NotScored: return mark.state == CState::NotScored || mark.state == CState::OutOfTime || mark.state == CState::ScorerFailed;
    }
    return false;
}

void apply_c_view(Stacks& stacks, const CMarks& marks, CView view) {
    if (view == CView::All) return;
    const auto mark_of = [&](std::uint32_t card) -> const CMark& { return marks.by_card[card]; };
    for (Stack& stack : stacks) {
        for (std::vector<std::uint32_t>* list : {&stack.todo, &stack.done}) {
            std::erase_if(*list, [&](std::uint32_t card) { return !in_c_view(mark_of(card), view); });
            std::stable_sort(list->begin(), list->end(), [&](std::uint32_t a, std::uint32_t b) { return c_ranks_before(mark_of(a), mark_of(b), view); });
        }
    }
}

std::string c_mark_words(const CMark& mark) {
    switch (mark.state) {
        case CState::None: return {};
        case CState::NotScored: return "not yet";
        case CState::OutOfTime: return "timed out";
        case CState::ScorerFailed: return "no score";
        case CState::NoFunctions: return "no code";
        case CState::Scored: return score_percent(mark.identical, mark.functions);
    }
    return {};
}

float c_identical_share(const CMark& mark) {
    return mark.state != CState::Scored || mark.functions == 0 ? 0.0f : static_cast<float>(static_cast<double>(mark.identical) / static_cast<double>(mark.functions));
}

std::string_view c_view_name(CView view) {
    switch (view) {
        case CView::All: return "All";
        case CView::MostIdentical: return "Most identical";
        case CView::NearMisses: return "Near misses";
        case CView::WillNotCompile: return "Will not compile";
        case CView::NotScored: return "Not scored";
    }
    return {};
}

std::string_view c_view_key(CView view) {
    switch (view) {
        case CView::MostIdentical: return "identical";
        case CView::NearMisses: return "near";
        case CView::WillNotCompile: return "nocompile";
        case CView::NotScored: return "unscored";
        case CView::All: break;
    }
    return "all";
}

CView c_view_from_key(std::string_view key) {
    for (const CView view : {CView::MostIdentical, CView::NearMisses, CView::WillNotCompile, CView::NotScored})
        if (key == c_view_key(view)) return view;
    return CView::All;
}

std::string c_pile_words(const CPileScore& pile) {
    if (pile.functions > 0) return score_percent(pile.identical, pile.functions);
    if (pile.scored > 0) return "no code";
    return pile.unscored > 0 ? "not yet" : "";
}

std::string c_detail_words(const CMark& mark) {
    switch (mark.state) {
        case CState::None: return {};
        case CState::NotScored: return "C not scored yet";
        case CState::OutOfTime: return "C ran out of time: no score";
        case CState::ScorerFailed: return "C scorer failed: no score";
        case CState::NoFunctions: return "C no functions to compare";
        case CState::Scored: return "C " + count_words(mark.identical) + " of " + count_words(mark.functions) + " byte-identical (" + score_percent(mark.identical, mark.functions) + ")";
    }
    return {};
}

std::string_view c_view_order(CView view) {
    switch (view) {
        case CView::MostIdentical: return "by identical";
        case CView::NearMisses: return "by same size";
        case CView::WillNotCompile: return "by no-compile";
        case CView::NotScored: return "by name";
        case CView::All: break;
    }
    return {};
}

std::string c_view_words(const CSummary& summary, CView view) {
    std::string words(c_view_name(view));
    if (view != CView::All) words += " " + count_words(summary.in_view[static_cast<std::size_t>(view) - 1]);
    return words;
}

std::array<CPart, 5> c_summary_parts(const CSummary& s) {
    return {CPart{"Byte-identical", s.identical}, CPart{"Same size", s.same_size}, CPart{"Different", s.different}, CPart{"Will not compile", s.no_compile}, CPart{"Run-time library", s.library}};
}

std::string c_summary_words(const CSummary& s) {
    std::string words = "C recompile, measured \xC2\xB7 " + count_words(s.scored) + " of " + count_words(s.programs()) + " programs scored";
    const auto part = [&](std::uint64_t n, const char* what) {
        if (n > 0) words += " \xC2\xB7 " + count_words(n) + " " + what;
    };
    part(s.not_scored, "not scored yet");
    part(s.out_of_time, "out of time");
    part(s.scorer_failed, "scorer failed");
    return words + " \xC2\xB7 " + count_words(s.functions) + " functions";
}

}  // namespace cb::app::board
