// cb/java/analyze.hpp -- build a Ledger from artifacts the converter ALREADY produces.
//
// The analyzers are deliberately non-invasive: they do not touch the generator or its output. They mine the signal
// that is already sitting in the output tree (the 500+ `// PORT:` markers), the structural rules, the build log --
// and turn it into structured Events.
//
// Each analyzer takes a Ledger and the relevant path(s) and appends Events; `analyze_all` runs them all.
#pragma once

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string>

#include "cb/java/ledger.hpp"
#include "cb/java/profiles/package.hpp"

namespace cb::java::analyze {

// -- 1. PORT / STUB / TODO markers embedded in the converted source --------------------------------------------------
//
// Walk the converted .cpp/.h files of `output_dir` and turn every PORT/STUB/TODO/FIXME/XXX marker in a comment into
// an Event. A bare `// PORT:` note is sharpened by what it says: a guess (a future bug) or a stub (unfinished work)
// becomes a HIGH GUESSED/STUBBED event, anything else an ANNOTATED one. Returns how many events were added.
std::size_t scan_markers(ledger::Ledger& ledger, const std::filesystem::path& output_dir);

// -- 2. Known crash/leak patterns -------------------------------------------------------------------------------------
//
// Flag string literals corrupted by the converter's member-access (`.` -> `->`) translation leaking into them -- a
// rare edge case that silently breaks a resource path, e.g. "/gui/furnace.png" emitted as "/gui/furnace->png": the
// texture load fails, returns 0, and the engine samples the last-bound atlas (the font sheet) full-screen. Returns
// how many were found.
std::size_t scan_corrupted_strings(ledger::Ledger& ledger, const std::filesystem::path& output_dir);

// -- 3. Compiler errors from a build log ------------------------------------------------------------------------------
//
// Parse MSVC `File.cpp(line): error CNNNN: msg` lines into ERROR events (MSVC repeats errors across its parallel
// compiler invocations; a repeat is counted once). A log path that is not a file gives 0.
std::size_t scan_build_log(ledger::Ledger& ledger, const std::filesystem::path& log_path);

// The most recently modified build_alpha*.log in `root`, if there is one (nullopt as well when `root` is not a folder).
std::optional<std::filesystem::path> newest_build_log(const std::filesystem::path& root);

// -- Orchestration ----------------------------------------------------------------------------------------------------

// The target platform named by the "// Target: <label>" comment in the first lines of a generated header: the key of
// the platform whose label the comment contains.
std::optional<std::string> detect_platform(const std::filesystem::path& output_dir);

struct Options {
    // the build log to read; nullopt: the newest in `root`
    std::optional<std::filesystem::path> build_log;
    // where to look for build logs; nullopt: the current folder
    std::optional<std::filesystem::path> root;
    // the platform key the rules adapt to; nullopt: detected from the output
    std::optional<std::string> platform;
    // the target's heavy types and nullable getters/fields; nullptr: the generic profile
    const profiles::Profile* profile = nullptr;
};

// What analyze_all found, per analyzer.
struct Counts {
    std::size_t markers = 0;
    std::size_t rules = 0;
    std::size_t corrupt_strings = 0;
    std::size_t build_log = 0;
    std::optional<std::filesystem::path> build_log_path;   // the log that was read, if any
};

// Run every analyzer against the artifacts of `output_dir`.
Counts analyze_all(ledger::Ledger& ledger, const std::filesystem::path& output_dir, const Options& options = {});

}  // namespace cb::java::analyze
