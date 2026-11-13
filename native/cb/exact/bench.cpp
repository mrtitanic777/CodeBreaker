#include "cb/exact/bench.hpp"

#include <algorithm>
#include <format>
#include <map>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/log.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/carve.hpp"
#include "cb/exact/draft/package.hpp"

namespace cb::exact::bench {

namespace {

// at most `n` bytes of an error's message
std::string bench_first(std::string_view s, std::size_t n) { return std::string(s.substr(0, n)); }

bool bench_hex(std::string_view s) {
    return !s.empty() && std::all_of(s.begin(), s.end(), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); });
}

}  // namespace

std::vector<Row> run(project::Project& project, const std::vector<std::uint32_t>& want, bool show) {
    const std::shared_ptr<draft::Project> view = project.as_draft();
    const std::optional<std::string> inverse = view->inverse_model_name();
    if (!inverse) throw UsageError(std::format("the toolchain {} names no inverse model (its profile's \"inverse\")", view->toolchain_name()));
    const std::shared_ptr<draft::InverseModel> model = view->load_inverse_model(*inverse);
    const std::map<std::uint32_t, std::string> names = draft::code_names(*view);
    const ByteView img = project.image();
    std::map<std::uint32_t, std::uint32_t> lengths;   // the functions carve finds, by address
    for (const carve::Function& f : carve::functions(project)) lengths[f.address] = f.length;
    std::map<std::uint32_t, std::filesystem::path> have;   // those that have a one-function source
    for (const project::Source& s : project.sources())
        if (s.at >= 0 && lengths.contains(static_cast<std::uint32_t>(s.at))) have.emplace(static_cast<std::uint32_t>(s.at), s.path);
    std::vector<std::uint32_t> todo = want;
    if (todo.empty())
        for (const auto& [a, p] : have) todo.push_back(a);
    std::sort(todo.begin(), todo.end());
    std::vector<Row> rows;
    for (const std::uint32_t a : todo) {
        const auto len = lengths.find(a);
        if (len == lengths.end()) continue;
        if (static_cast<std::size_t>(a) + len->second > img.size()) continue;
        draft::ModelTexts texts;
        try {   // (the model gives up on a function: noted, the next one is tried)
            texts = draft::model_texts(*model, img.subspan(a, len->second), std::format("f{:05x}", a), a);
        } catch (const Error& e) {
            rows.push_back({a, "declined: " + bench_first(e.what(), 80), ""});
            continue;
        }
        Row row{a, "", ""};
        try {   // (a pass that fails on one function: noted, not fatal)
            const draft::ProveOutcome proven = view->prove_texts(*model, texts, a, len->second, names);
            row.source = proven.source;
            row.verdict = proven.verdict;
        } catch (const Error& e) {
            row.source = std::format("{}: {}\n", e.kind(), e.what());
            row.verdict = std::format("failed: {}: {}", e.kind(), bench_first(e.what(), 80));
        }
        if (show) {
            out("== {:05x} {} bytes: {}\n{}\n", a, len->second, row.verdict, row.source);
            if (const auto source = have.find(a); source != have.end())
                out("-- the source:\n{}\n", text::from_latin1(fs::read_file(source->second)));
        }
        rows.push_back(std::move(row));
    }
    return rows;
}

int main(project::Project& project, const std::vector<std::string>& args) {
    std::vector<std::uint32_t> want;
    for (std::size_t k = 0; k < args.size(); ++k) {   // (the file --out names is not an address, whatever its name is made of)
        if (k > 0 && args[k - 1] == "--out") continue;
        if (!args[k].starts_with('-') && bench_hex(args[k])) want.push_back(static_cast<std::uint32_t>(std::stoul(args[k], nullptr, 16)));
    }
    const std::vector<Row> rows = run(project, want, std::find(args.begin(), args.end(), "--show") != args.end());
    // the verdicts counted by their first word (up to a colon), the most common first, ties in the order first met
    std::vector<std::pair<std::string, std::size_t>> tally;
    for (const Row& r : rows) {
        const std::string key = r.verdict.substr(0, r.verdict.find(':'));
        const auto it = std::find_if(tally.begin(), tally.end(), [&](const auto& t) { return t.first == key; });
        if (it == tally.end()) tally.emplace_back(key, 1);
        else ++it->second;
    }
    std::stable_sort(tally.begin(), tally.end(), [](const auto& a, const auto& b) { return a.second > b.second; });
    if (const auto o = std::find(args.begin(), args.end(), "--out"); o != args.end() && o + 1 != args.end()) {
        std::string lines;
        for (const Row& r : rows) lines += std::format("{:05x} {}\n", r.address, r.verdict);
        fs::write_text(fs::from_text(*(o + 1)), lines);
    }
    std::vector<std::string> parts;
    for (const auto& [k, n] : tally) parts.push_back(std::format("{} {}", n, k));
    out("{} functions: {}\n", rows.size(), text::join(parts, ", "));
    return 0;
}

}  // namespace cb::exact::bench
