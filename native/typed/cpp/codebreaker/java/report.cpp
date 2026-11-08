// codebreaker/java/report.cpp -- typed C++ port of codebreaker.java.report (see the header).

#include "codebreaker/java/report.hpp"

#include <cmath>

namespace codebreaker::java::report {

using ledger::Category;
using ledger::Event;
using ledger::Ledger;
using ledger::Severity;

// (keys spelled as the strings Category.X / Severity.X hold: this file's constants must not depend on the
// order the ledger's own are made in)
const py::dict<py::str, py::str> _CAT_BLURB = {
    {py::str("ERROR"), py::str("compiler rejected it (from the build log)")},
    {py::str("GUESSED"), py::str("converter made a heuristic call that may be wrong -- INSPECT")},
    {py::str("STUBBED"), py::str("placeholder; needs a hand implementation")},
    {py::str("BLOCKED"), py::str("explicitly could not be ported")},
    {py::str("RISK"), py::str("matches a known console-crash / leak pattern")},
    {py::str("WARNING"), py::str("suspicious; verify by hand")},
    {py::str("OVERRIDDEN"), py::str("a hand-written file is in use for this class")},
    {py::str("ANNOTATED"), py::str("a note about a decision already taken")},
    {py::str("PORTED"), py::str("translated literally and cleanly")},
    {py::str("RULE"), py::str("an automatic fix rule fired")},
};

const py::dict<py::str, py::str> _SEV_TAG = {
    {py::str("HIGH"), py::str("\xF0\x9F\x94\xB4 HIGH")},   // red circle
    {py::str("MED"), py::str("\xF0\x9F\x9F\xA1 MED")},     // yellow circle
    {py::str("LOW"), py::str("\xE2\x9A\xAA LOW")},         // white circle
};

py::str _safe(const py::str& name) {
    return name.replace(py::str("/"), py::str("_")).replace(py::str("\\"), py::str("_"));
}

// open(path, 'w', encoding='utf-8').write(text)
static void write_text_(const py::str& path, const py::str& text) {
    py::file f = py::open(path, py::str("w"), py::str("utf-8"));
    f.write(text);
    f.close();
}

py::str write_reports(const Ledger& ledger, const py::str& output_dir, const std::optional<py::str>& report_dir_arg,
                      const py::dict<py::str, py::str>& meta) {
    py::str report_dir = (report_dir_arg && py::truthy(*report_dir_arg)) ? *report_dir_arg
                                                                          : py::os::path::join(output_dir, py::str("report"));
    py::os::makedirs(report_dir, 0777, true);
    py::str files_dir = py::os::path::join(report_dir, py::str("files"));
    py::os::makedirs(files_dir, 0777, true);

    _write_index(ledger, report_dir, meta);
    for (const auto& [fname, evs] : ledger.by_file().items()) {
        _write_file_report(fname, evs, ledger, files_dir);
    }
    write_text_(py::os::path::join(report_dir, py::str("report.json")), ledger.to_json());
    return report_dir;
}

py::str _bar(int64_t n, int64_t width, const std::optional<int64_t>& total) {
    if (!total || !*total) {
        return py::str("");
    }
    // int(round(width * n / float(total))): round() is half-to-even, as nearbyint is in the default mode
    int64_t fill = static_cast<int64_t>(std::nearbyint(static_cast<double>(width * n) / static_cast<double>(*total)));
    return py::repeat(py::str("\xE2\x96\x88"), fill) + py::repeat(py::str("\xC2\xB7"), width - fill);   // full block, middle dot
}

void _write_index(const Ledger& ledger, const py::str& report_dir, const py::dict<py::str, py::str>& meta) {
    py::list<py::str> L;
    L.append(py::str("# Conversion Report"));
    L.append(py::str(""));
    if (py::truthy(meta)) {
        for (const char* k : {"profile", "output_dir", "build_log", "generated"}) {
            py::str key(k);
            if (py::truthy(meta.get(key, py::str("")))) {
                L.append(py::percent(py::str("- **%s:** %s"), key.replace(py::str("_"), py::str(" ")).title(), meta.at(key)));
            }
        }
        L.append(py::str(""));
    }

    py::dict<py::str, int64_t> cats = ledger.totals_by_category();
    py::dict<py::str, int64_t> sevs = ledger.totals_by_severity();
    L.append(py::str("## Totals"));
    L.append(py::str(""));
    L.append(py::str("| Severity | Count |"));
    L.append(py::str("|---|---|"));
    L.append(py::percent(py::str("| \xF0\x9F\x94\xB4 HIGH | %d |"), sevs.at(Severity::HIGH)));
    L.append(py::percent(py::str("| \xF0\x9F\x9F\xA1 MED  | %d |"), sevs.at(Severity::MED)));
    L.append(py::percent(py::str("| \xE2\x9A\xAA LOW  | %d |"), sevs.at(Severity::LOW)));
    L.append(py::percent(py::str("| **Total events** | **%d** |"), ledger.size()));
    L.append(py::str(""));
    int64_t total_ev = ledger.size() > 1 ? ledger.size() : 1;   // max(1, len(ledger))
    L.append(py::str("| Category | Count | | What it means |"));
    L.append(py::str("|---|---:|---|---|"));
    for (const py::str& c : Category::ALL) {
        int64_t n = cats.get(c, 0);
        if (n) {
            L.append(py::percent(py::str("| `%s` | %d | %s | %s |"), c, n, _bar(n, 18, total_ev), _CAT_BLURB.get(c, py::str(""))));
        }
    }
    L.append(py::str(""));

    // Risk-ranked files -- the headline. Inspect the top of this list first.
    py::list<ledger::FileRisk> rows = ledger.file_risk_table();
    py::list<ledger::FileRisk> risky;
    for (const ledger::FileRisk& r : rows) {
        if (std::get<1>(r) > 0) risky.append(r);
    }
    int64_t high_files = 0;
    for (const ledger::FileRisk& r : rows) {
        if (std::get<3>(r).get(Severity::HIGH, 0)) high_files += 1;
    }
    L.append(py::str("## Files by risk (inspect top-down)"));
    L.append(py::str(""));
    L.append(py::percent(py::str("%d files have findings; %d carry HIGH-severity risk."), py::len(risky), high_files));
    L.append(py::str(""));
    L.append(py::str("| Risk | File | \xF0\x9F\x94\xB4 | \xF0\x9F\x9F\xA1 | guess | stub | risk | err |"));
    L.append(py::str("|---:|---|---:|---:|---:|---:|---:|---:|"));
    for (const ledger::FileRisk& row : rows.slice(0, 40)) {
        const py::str& fname = std::get<0>(row);
        int64_t score = std::get<1>(row);
        const py::dict<py::str, int64_t>& c = std::get<2>(row);
        const py::dict<py::str, int64_t>& s = std::get<3>(row);
        if (score == 0) {
            continue;
        }
        py::str link = py::percent(py::str("files/%s.md"), _safe(fname));
        L.append(py::percent(py::str("| %d | [%s](%s) | %d | %d | %d | %d | %d | %d |"), score, fname, link,
                             s.get(Severity::HIGH, 0), s.get(Severity::MED, 0),
                             c.get(Category::GUESSED, 0), c.get(Category::STUBBED, 0),
                             c.get(Category::RISK, 0), c.get(Category::ERROR, 0)));
    }
    L.append(py::str(""));

    L.append(py::str("## Legend"));
    L.append(py::str(""));
    L.append(py::str("Sources: `marker` (PORT/STUB/TODO comment), `rule` (structural crash-class check), "
                     "`lint` (known crash/leak pattern), `build` (compiler error)."));
    L.append(py::str(""));

    write_text_(py::os::path::join(report_dir, py::str("index.md")), py::join(py::str("\n"), L));
}

void _write_file_report(const py::str& fname, const py::list<py::ref<Event>>& evs, const Ledger& ledger,
                        const py::str& files_dir) {
    py::list<py::str> L;
    L.append(py::percent(py::str("# %s"), fname));
    L.append(py::str(""));
    L.append(py::percent(py::str("Risk score: **%d**  \xC2\xB7  %d findings"), ledger.risk_score(evs), py::len(evs)));
    L.append(py::str(""));
    py::dict<py::str, py::list<py::ref<Event>>> by_cat;
    for (const py::ref<Event>& e : evs) {
        by_cat.setdefault(e->category, py::list<py::ref<Event>>()).append(e);
    }
    for (const py::str& c : Category::ALL) {
        if (!by_cat.contains(c) || by_cat.at(c).empty()) {
            continue;
        }
        const py::list<py::ref<Event>>& group = by_cat.at(c);
        L.append(py::percent(py::str("## %s \xE2\x80\x94 %s"), c, _CAT_BLURB.get(c, py::str(""))));
        L.append(py::str(""));
        for (const py::ref<Event>& e : group) {
            py::str loc = e->line ? py::percent(py::str("L%d"), e->line) : py::str("\xE2\x80\x94");
            py::str tag = _SEV_TAG.get(e->severity, e->severity);
            L.append(py::percent(py::str("- **%s** `%s` (%s, `%s`): %s"), tag, loc, e->source, e->rule, e->message));
            if (py::truthy(e->snippet)) {
                L.append(py::str("  ```cpp"));
                L.append(py::percent(py::str("  %s"), e->snippet));
                L.append(py::str("  ```"));
            }
        }
        L.append(py::str(""));
    }
    write_text_(py::os::path::join(files_dir, _safe(fname) + py::str(".md")), py::join(py::str("\n"), L));
}

}  // namespace codebreaker::java::report
