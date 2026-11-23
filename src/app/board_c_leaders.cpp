// app/board_c_leaders.cpp -- see board_c_leaders.hpp.
#include "app/board_c_leaders.hpp"

#include <algorithm>
#include <format>
#include <limits>

#include "core/fs.hpp"
#include "core/text.hpp"

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

std::string_view c_catalog_file(std::string_view rel) {
    const std::size_t slash = rel.rfind('/');
    return slash == std::string_view::npos ? rel : rel.substr(slash + 1);
}

char c_catalog_lower(char c) { return c >= 'A' && c <= 'Z' ? static_cast<char>(c - 'A' + 'a') : c; }

// `word` (lower case) in `in`, case ignored.
bool c_catalog_holds(std::string_view in, std::string_view word) {
    if (word.size() > in.size()) return false;
    for (std::size_t at = 0; at + word.size() <= in.size(); ++at) {
        std::size_t k = 0;
        while (k < word.size() && c_catalog_lower(in[at + k]) == word[k]) ++k;
        if (k == word.size()) return true;
    }
    return false;
}

std::string c_leader_field(std::string_view s) {
    std::string out(s);
    std::replace_if(out.begin(), out.end(), [](char c) { return c == '\t' || c == '\n' || c == '\r'; }, ' ');
    return out;
}

}  // namespace

void add_c_catalog(CCatalog& into, std::string_view rel, const std::vector<CFunctionRow>& rows) {
    const auto program = static_cast<std::uint32_t>(into.rels.size());
    into.rels.emplace_back(rel);
    for (const CFunctionRow& row : rows) {
        CCatalogEntry e;
        e.name_at = static_cast<std::uint32_t>(into.names.size());
        e.name_size = static_cast<std::uint16_t>(std::min<std::size_t>(row.name.size(), 0xFFFF));
        into.names.append(row.name, 0, e.name_size);
        e.program = program;
        e.a = row.a;
        e.b = row.b;
        e.closeness = row.closeness;
        e.verdict = row.verdict;
        e.measure = row.measure;
        into.entries.push_back(e);
    }
}

CSearch search_c_catalog(const CCatalog& catalog, std::string_view query, std::size_t keep) {
    CSearch out;
    std::vector<std::string> words;
    for (std::size_t at = 0; at < query.size();) {
        const std::size_t start = query.find_first_not_of(' ', at);
        if (start == std::string_view::npos) break;
        const std::size_t end = std::min(query.find(' ', start), query.size());
        words.push_back(text::lower(query.substr(start, end - start)));
        at = end;
    }
    if (words.empty()) return out;
    std::vector<std::vector<char>> in_program(words.size(), std::vector<char>(catalog.rels.size()));
    for (std::size_t w = 0; w < words.size(); ++w)
        for (std::size_t p = 0; p < catalog.rels.size(); ++p) in_program[w][p] = c_catalog_holds(c_catalog_file(catalog.rels[p]), words[w]) ? 1 : 0;
    struct Hit {
        std::uint32_t entry = 0;
        std::uint8_t rank = 2;
    };
    std::vector<Hit> hits;
    for (std::uint32_t i = 0; i < catalog.entries.size(); ++i) {
        const CCatalogEntry& e = catalog.entries[i];
        const std::string_view name = std::string_view(catalog.names).substr(e.name_at, e.name_size);
        bool all = true;
        for (std::size_t w = 0; w < words.size() && all; ++w) all = in_program[w][e.program] != 0 || c_catalog_holds(name, words[w]);
        if (!all) continue;
        const std::string_view first = words[0];
        const bool prefix = name.size() >= first.size() && std::equal(first.begin(), first.end(), name.begin(), [](char a, char b) { return a == c_catalog_lower(b); });
        hits.push_back({i, static_cast<std::uint8_t>(prefix ? (name.size() == first.size() ? 0 : 1) : 2)});
    }
    out.matches = hits.size();
    const auto before = [&](const Hit& a, const Hit& b) {
        if (a.rank != b.rank) return a.rank < b.rank;
        const double ca = catalog.entries[a.entry].closeness, cb = catalog.entries[b.entry].closeness;
        return ca != cb ? ca > cb : a.entry < b.entry;
    };
    const std::size_t shown = std::min(keep, hits.size());
    std::partial_sort(hits.begin(), hits.begin() + static_cast<std::ptrdiff_t>(shown), hits.end(), before);
    out.rows.reserve(shown);
    for (std::size_t i = 0; i < shown; ++i) {
        const CCatalogEntry& e = catalog.entries[hits[i].entry];
        const std::string& rel = catalog.rels[e.program];
        out.rows.push_back({rel, std::string(c_catalog_file(rel)), catalog.names.substr(e.name_at, e.name_size), c_measure_words(e.verdict, e.measure, e.closeness, e.a, e.b), e.closeness, e.verdict == CVerdict::SameSize,
                            e.measure == CMeasure::Units ? e.b : e.a});
    }
    return out;
}

std::string c_leaders_tsv(const std::vector<CLeader>& rows) {
    std::string out = "rank\tprogram\tpath\tfunction\tmeasured\tcloseness\n";
    for (std::size_t i = 0; i < rows.size(); ++i) {
        const CLeader& r = rows[i];
        out += std::format("{}\t{}\t{}\t{}\t{}\t{:.4f}\n", i + 1, c_leader_field(r.program), c_leader_field(r.rel), c_leader_field(r.function), c_leader_field(r.words), r.closeness);
    }
    return out;
}

std::string c_leaders_file_name(std::string_view query) {
    std::string slug;
    for (const char c : query) {
        const bool plain = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '-';
        if (plain) slug += c;
        else if (!slug.empty() && slug.back() != '_') slug += '_';
        if (slug.size() == 40) break;
    }
    while (!slug.empty() && slug.back() == '_') slug.pop_back();
    return slug.empty() ? "closest-functions.tsv" : "functions-" + slug + ".tsv";
}

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
        std::optional<std::vector<CFunctionRow>> rows;
        try {
            const std::filesystem::path file = root / "c_scores" / c_functions_file_name(rel);
            std::error_code ec;
            if (std::filesystem::is_regular_file(file, ec)) rows = parse_c_function_rows(fs::read_text(file));
        } catch (const std::exception&) {}   // (the scorer has it open: left out of this snapshot)
        if (!rows) continue;
        ++out.programs;
        add_c_catalog(out.catalog, rel, *rows);
        add_c_leaders(out, rel, c_near_misses_of(*rows, std::numeric_limits<std::size_t>::max()));
    }
    c_leaders_trim(out);
    return out;
}

}  // namespace cb::app::board
