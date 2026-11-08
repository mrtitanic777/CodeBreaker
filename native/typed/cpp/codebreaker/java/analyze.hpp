// codebreaker/java/analyze.hpp -- typed C++ port of codebreaker.java.analyze.
//
// analyze.py -- build a Ledger from artifacts the converter ALREADY produces.
//
// Phase 1 is deliberately non-invasive: we do not touch java2xbox360.py or its
// output. We mine the signal that's already sitting in the output tree (the 500+
// `// PORT:` markers), the lint rules, the build log, and the handwritten-override
// set -- and turn it into structured Events.
//
// Later phases will move event production *into* the generator/rules so we capture
// PORTED/GUESSED decisions at the moment they're made instead of reconstructing
// them. Until then, these analyzers give you the per-file ledger today.
//
// Each analyzer takes a Ledger and the relevant path(s) and appends Events.
//
// Notes on the port:
//   * The module-level tables that hold a Category (_MARKER_TOKENS) are functions returning the table, made on
//     first use: Category's strings are constants of another translation unit, and the order two units'
//     constants are made in is not defined. The regular expressions are made the same way, for the same reason.
//   * `converter_lint` is an optional external module that is not bundled; scan_lint() takes the ImportError
//     path (returns 0) -- as the Python does wherever it is not importable.
//   * analyze_all()'s dict of counts is a record (AnalyzeCounts): five counts and the path of the log used (or None).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "py.hpp"
#include "py_os.hpp"

#include "codebreaker/java/ledger.hpp"

namespace codebreaker::java::analyze {

// ---------------------------------------------------------------------------
// 1. PORT / STUB / TODO markers embedded in the converted source
// ---------------------------------------------------------------------------

// Marker tokens, most-specific FIRST so 'PORT-BLOCKED' wins over 'PORT'.
// Each entry: (compiled regex matching the token, default Category).
const py::list<std::tuple<py::re::Pattern, py::str>>& _MARKER_TOKENS();

// Content keywords that RE-CLASSIFY a bare `// PORT:` comment into a sharper bucket.
// These are the highest-value signals: a "guess" is a future bug, a "stub" is unfinished work.
const py::re::Pattern& _GUESS_HINTS();
const py::re::Pattern& _STUB_HINTS();

const py::re::Pattern& _COMMENT_RE();

// Refine a bare PORT marker by its content; return (category, severity). The severity is None when the Event
// is to use the category's default.
std::tuple<py::str, std::optional<py::str>> _classify_marker(const py::str& comment_text, const py::str& base_category);

// Walk converted .cpp/.h and turn every PORT/STUB/TODO marker into an Event.
int64_t scan_markers(ledger::Ledger& ledger, const py::str& output_dir);

// ---------------------------------------------------------------------------
// 2. Known crash/leak patterns (reuse the existing converter_lint rules)
// ---------------------------------------------------------------------------

// Fold an optional external linter's findings into the ledger.
//
// `converter_lint` is an optional, external rule module (the legacy monolithic linter). It is
// not bundled with CodeBreaker; when it is not importable this pass is simply skipped. Provide your own
// module exposing `lint_tree(dir) -> [Finding(file, line, message, severity, rule)]` to enable it.
int64_t scan_lint(ledger::Ledger& ledger, const py::str& output_dir);

// Catches the converter's member-access (`.` -> `->`) translation leaking into a STRING LITERAL --
// a rare AST edge case that silently breaks a resource path, e.g. "/gui/furnace.png" emitted as
// "/gui/furnace->png": the texture load fails, returns 0, and the engine samples the last-bound
// atlas (the font sheet) full-screen. Real occurrences: GuiFurnace (furnace.png), CanvasMajongLogo
// (logo.png) -- fixed via overrides. This flags any recurrence on regen so it can't ship silently.
const py::re::Pattern& _CORRUPT_STR_RE();

// Flag string literals corrupted by member-access translation (broken resource paths).
int64_t scan_corrupted_strings(ledger::Ledger& ledger, const py::str& output_dir);

// ---------------------------------------------------------------------------
// 3. Compiler errors from a build log (optional -- newest log by default)
// ---------------------------------------------------------------------------

const py::re::Pattern& _MSVC_RE();

// Parse MSVC `File.cpp(line): error CNNNN: msg` lines into ERROR events.
// (log_path: the Python's `not log_path` -- None or empty -- gives 0.)
int64_t scan_build_log(ledger::Ledger& ledger, const std::optional<py::str>& log_path);

// Return the path of the most-recently-modified build_alpha*.log, or None.
std::optional<py::str> newest_build_log(const py::str& root);

// ---------------------------------------------------------------------------
// Orchestration
// ---------------------------------------------------------------------------

// Try to detect the target platform from a generated header's comment line.
std::optional<py::str> _detect_platform(const py::str& output_dir);

// What analyze_all() gives: the Python's dict of counts, with the log used as '_build_log_path'.
struct AnalyzeCounts {
    int64_t markers = 0;
    int64_t rules = 0;
    int64_t lint = 0;
    int64_t corrupt_strings = 0;
    int64_t build_log = 0;
    std::optional<py::str> build_log_path;   // '_build_log_path'
};
template <class F> void for_each_field(const AnalyzeCounts& r, F&& f) {
    f("markers", r.markers);
    f("rules", r.rules);
    f("lint", r.lint);
    f("corrupt_strings", r.corrupt_strings);
    f("build_log", r.build_log);
    f("_build_log_path", r.build_log_path);
}

// Run every available analyzer against the artifacts and return per-source counts.
AnalyzeCounts analyze_all(ledger::Ledger& ledger, const py::str& output_dir, const std::optional<py::str>& build_log = std::nullopt,
                          const std::optional<py::str>& root = std::nullopt, const std::optional<py::str>& platform = std::nullopt);

}  // namespace codebreaker::java::analyze
