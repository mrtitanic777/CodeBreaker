// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/report.hpp"

#include <algorithm>
#include <cmath>
#include <format>

#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"

namespace cb::java::report {

namespace {

using ledger::Category;
using ledger::Severity;

// Friendly one-liners describing each category, shown in the report legend.
std::string_view blurb(Category c) {
    switch (c) {
        case Category::Error: return "compiler rejected it (from the build log)";
        case Category::Guessed: return "converter made a heuristic call that may be wrong -- INSPECT";
        case Category::Stubbed: return "placeholder; needs a hand implementation";
        case Category::Blocked: return "explicitly could not be ported";
        case Category::Risk: return "matches a known console-crash / leak pattern";
        case Category::Warning: return "suspicious; verify by hand";
        case Category::Overridden: return "a hand-written file is in use for this class";
        case Category::Annotated: return "a note about a decision already taken";
        case Category::Ported: return "translated literally and cleanly";
        case Category::Rule: return "an automatic fix rule fired";
    }
    return {};
}

std::string_view severity_tag(Severity s) {
    switch (s) {
        case Severity::High: return "\xF0\x9F\x94\xB4 HIGH";   // red circle
        case Severity::Med: return "\xF0\x9F\x9F\xA1 MED";     // yellow circle
        case Severity::Low: return "\xE2\x9A\xAA LOW";         // white circle
    }
    return {};
}

std::size_t at(const auto& counts, auto key) { return counts[static_cast<std::size_t>(key)]; }

constexpr std::size_t kIndexRows = 40;   // the risk table lists the worst files only

}  // namespace

std::string bar(std::size_t n, std::size_t width, std::size_t total) {
    if (total == 0) return {};
    // round half to even, as the default rounding mode does
    const auto fill = static_cast<std::size_t>(std::nearbyint(static_cast<double>(width) * static_cast<double>(n) / static_cast<double>(total)));
    std::string out;
    for (std::size_t i = 0; i < width; ++i) out += i < fill ? "\xE2\x96\x88" : "\xC2\xB7";   // full block / middle dot
    return out;
}

std::string safe_name(std::string_view name) {
    std::string out(name);
    std::replace(out.begin(), out.end(), '/', '_');
    std::replace(out.begin(), out.end(), '\\', '_');
    return out;
}

std::string render_index(const ledger::Ledger& ledger, const Meta& meta) {
    std::vector<std::string> out;
    out.push_back("# Conversion Report");
    out.push_back("");
    const std::pair<const char*, const std::string*> shown[] = {{"Profile", &meta.profile},
                                                                {"Output Dir", &meta.output_dir},
                                                                {"Build Log", &meta.build_log},
                                                                {"Generated", &meta.generated}};
    bool any_meta = false;
    for (const auto& [label, value] : shown) {
        if (value->empty()) continue;
        out.push_back(std::format("- **{}:** {}", label, *value));
        any_meta = true;
    }
    if (any_meta) out.push_back("");

    const ledger::CategoryCounts cats = ledger.totals_by_category();
    const ledger::SeverityCounts sevs = ledger.totals_by_severity();
    out.push_back("## Totals");
    out.push_back("");
    out.push_back("| Severity | Count |");
    out.push_back("|---|---|");
    out.push_back(std::format("| {} | {} |", severity_tag(Severity::High), at(sevs, Severity::High)));
    out.push_back(std::format("| {}  | {} |", severity_tag(Severity::Med), at(sevs, Severity::Med)));
    out.push_back(std::format("| {}  | {} |", severity_tag(Severity::Low), at(sevs, Severity::Low)));
    out.push_back(std::format("| **Total events** | **{}** |", ledger.size()));
    out.push_back("");
    const std::size_t total_events = std::max<std::size_t>(1, ledger.size());
    out.push_back("| Category | Count | | What it means |");
    out.push_back("|---|---:|---|---|");
    for (Category c : ledger::all_categories()) {
        const std::size_t n = at(cats, c);
        if (n == 0) continue;
        out.push_back(std::format("| `{}` | {} | {} | {} |", ledger::name(c), n, bar(n, 18, total_events), blurb(c)));
    }
    out.push_back("");

    // Risk-ranked files -- the headline. Inspect the top of this list first.
    const std::vector<ledger::FileRisk> rows = ledger.file_risk_table();
    std::size_t risky = 0;
    std::size_t with_high = 0;
    for (const ledger::FileRisk& r : rows) {
        if (r.risk > 0) ++risky;
        if (at(r.severities, Severity::High) != 0) ++with_high;
    }
    out.push_back("## Files by risk (inspect top-down)");
    out.push_back("");
    out.push_back(std::format("{} files have findings; {} carry HIGH-severity risk.", risky, with_high));
    out.push_back("");
    out.push_back("| Risk | File | \xF0\x9F\x94\xB4 | \xF0\x9F\x9F\xA1 | guess | stub | risk | err |");
    out.push_back("|---:|---|---:|---:|---:|---:|---:|---:|");
    for (std::size_t i = 0; i < rows.size() && i < kIndexRows; ++i) {
        const ledger::FileRisk& r = rows[i];
        if (r.risk == 0) continue;
        out.push_back(std::format("| {} | [{}](files/{}.md) | {} | {} | {} | {} | {} | {} |", r.risk, r.file, safe_name(r.file),
                                  at(r.severities, Severity::High), at(r.severities, Severity::Med),
                                  at(r.categories, Category::Guessed), at(r.categories, Category::Stubbed),
                                  at(r.categories, Category::Risk), at(r.categories, Category::Error)));
    }
    out.push_back("");

    out.push_back("## Legend");
    out.push_back("");
    out.push_back("Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), "
                  "`lint` (known crash/leak pattern), `build` (compiler error).");
    out.push_back("");
    return text::join(out, "\n");
}

std::string render_file_report(std::string_view file, const std::vector<const ledger::Event*>& events) {
    std::vector<std::string> out;
    out.push_back(std::format("# {}", file));
    out.push_back("");
    out.push_back(std::format("Risk score: **{}**  \xC2\xB7  {} findings", ledger::Ledger::risk_score(events), events.size()));
    out.push_back("");
    for (Category c : ledger::all_categories()) {
        bool header_written = false;
        for (const ledger::Event* e : events) {
            if (e->category != c) continue;
            if (!header_written) {
                out.push_back(std::format("## {} \xE2\x80\x94 {}", ledger::name(c), blurb(c)));
                out.push_back("");
                header_written = true;
            }
            const std::string loc = e->line != 0 ? std::format("L{}", e->line) : "\xE2\x80\x94";
            out.push_back(std::format("- **{}** `{}` ({}, `{}`): {}", severity_tag(e->severity), loc, e->source, e->rule, e->message));
            if (!e->snippet.empty()) {
                out.push_back("  ```cpp");
                out.push_back("  " + e->snippet);
                out.push_back("  ```");
            }
        }
        if (header_written) out.push_back("");
    }
    return text::join(out, "\n");
}

std::filesystem::path write_reports(const ledger::Ledger& ledger, const std::filesystem::path& output_dir,
                                    const std::optional<std::filesystem::path>& report_dir, const Meta& meta) {
    const std::filesystem::path dir = report_dir && !report_dir->empty() ? *report_dir : output_dir / "report";
    const std::filesystem::path files_dir = dir / "files";
    fs::make_dirs(files_dir);

    fs::write_text(dir / "index.md", render_index(ledger, meta));
    for (const auto& [file, events] : ledger.by_file()) {
        fs::write_text(files_dir / (safe_name(file) + ".md"), render_file_report(file, events));
    }
    fs::write_text(dir / "report.json", ledger.to_json());
    return dir;
}

}  // namespace cb::java::report
