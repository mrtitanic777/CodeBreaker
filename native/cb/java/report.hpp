// cb/java/report.hpp -- render a Ledger into the report bundle.
//
// Outputs (under <output_dir>/report/ by default):
//   index.md        -- run summary: totals, severity, the RISK-RANKED file list. Read this first.
//   files/<File>.md -- per-file ledger: every event for that file, grouped by category, with line numbers + snippets.
//   report.json     -- the whole ledger, machine-readable (for the GUI / diffs).
//
// The risk ranking is the point: it tells you WHICH few files to inspect before you build, instead of discovering
// it by crashing on hardware.
#pragma once

#include <filesystem>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/java/ledger.hpp"

namespace cb::java::report {

// What the index says about the run, above the totals. An empty value is not shown.
struct Meta {
    std::string profile;
    std::string output_dir;
    std::string build_log;
    std::string generated;
};

// A bar of `n` out of `total`, `width` cells wide ("" when there is no total).
std::string bar(std::size_t n, std::size_t width, std::size_t total);

// A file name made safe to use as a report file name: '/' and '\' become '_'.
std::string safe_name(std::string_view name);

// The text of index.md.
std::string render_index(const ledger::Ledger& ledger, const Meta& meta = {});

// The text of one file's report: its events grouped by category.
std::string render_file_report(std::string_view file, const std::vector<const ledger::Event*>& events);

// Write index.md, files/<file>.md for each file with events, and report.json into `report_dir` (default:
// <output_dir>/report); returns the report folder. IoError when a file cannot be written.
std::filesystem::path write_reports(const ledger::Ledger& ledger, const std::filesystem::path& output_dir,
                                    const std::optional<std::filesystem::path>& report_dir = std::nullopt,
                                    const Meta& meta = {});

}  // namespace cb::java::report
