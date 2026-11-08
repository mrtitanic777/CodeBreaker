// codebreaker/java/report.hpp -- typed C++ port of codebreaker.java.report.
//
// report.py -- render a Ledger into the report bundle.
//
// Outputs (under <output_dir>/report/ by default):
//   index.md        -- run summary: totals, severity, the RISK-RANKED file list,
//                      and the override / backport-debt section. Read this first.
//   <File>.md       -- per-file ledger: every event for that file, grouped by
//                      category, with line numbers + snippets.
//   report.json     -- the whole ledger, machine-readable (for the GUI / diffs).
//
// The risk ranking is the point: it tells you WHICH few files to inspect before
// you build, instead of discovering it by crashing on hardware.
//
// Notes on the port:
//   * the files are written in Python's text mode (encoding utf-8, newline '\n' becoming '\r\n' on
//     Windows), as the Python's open(..., 'w', encoding='utf-8') does;
//   * `meta` is a {str: str} dict (the four keys the index reads are profile, output_dir, build_log and
//     generated); a value that is missing, None or '' is skipped alike, so None needs no type here;
//   * `report_dir=None` is std::nullopt (an empty string counts as None too, as `report_dir or ...` does).
#pragma once

#include <cstdint>
#include <optional>

#include "py.hpp"
#include "codebreaker/java/ledger.hpp"

namespace codebreaker::java::report {

// Friendly one-liners describing each category, shown in the report legend.
extern const py::dict<py::str, py::str> _CAT_BLURB;

extern const py::dict<py::str, py::str> _SEV_TAG;

py::str _safe(const py::str& name);

// -> the report folder
py::str write_reports(const ledger::Ledger& ledger, const py::str& output_dir,
                      const std::optional<py::str>& report_dir = std::nullopt,
                      const py::dict<py::str, py::str>& meta = py::dict<py::str, py::str>());

// a bar of `n` out of `total`, `width` cells wide ('' when there is no total)
py::str _bar(int64_t n, int64_t width = 24, const std::optional<int64_t>& total = std::nullopt);

void _write_index(const ledger::Ledger& ledger, const py::str& report_dir, const py::dict<py::str, py::str>& meta);

void _write_file_report(const py::str& fname, const py::list<py::ref<ledger::Event>>& evs, const ledger::Ledger& ledger,
                        const py::str& files_dir);

}  // namespace codebreaker::java::report
