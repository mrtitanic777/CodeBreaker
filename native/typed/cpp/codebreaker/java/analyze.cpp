// codebreaker/java/analyze.cpp -- typed C++ port of codebreaker.java.analyze (see analyze.hpp).

#include "codebreaker/java/analyze.hpp"

#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/platforms.hpp"
#include "codebreaker/java/rules.hpp"

namespace codebreaker::java::analyze {

namespace {

using ledger::Category;
using ledger::Severity;
using py::str;

str S(const char* s) { return str(s); }

constexpr int64_t IGNORECASE = 2;

}  // namespace

// ---------------------------------------------------------------------------
// 1. PORT / STUB / TODO markers embedded in the converted source
// ---------------------------------------------------------------------------

const py::list<std::tuple<py::re::Pattern, py::str>>& _MARKER_TOKENS() {
    static const py::list<std::tuple<py::re::Pattern, py::str>> tokens = {
        {py::re::compile(S("PORT-BLOCKED")), Category::BLOCKED},
        {py::re::compile(S("PORT[- ]WARNING")), Category::WARNING},
        {py::re::compile(S("PORT\\s+stub")), Category::STUBBED},
        {py::re::compile(S("PORT\\s+NOTES?")), Category::ANNOTATED},
        {py::re::compile(S("PORT\\s+helpers?")), Category::ANNOTATED},
        {py::re::compile(S("PORT\\([^)]*\\)")), Category::ANNOTATED},   // PORT(post-vs-pre-multiply) etc.
        {py::re::compile(S("PORT")), Category::ANNOTATED},              // bare 'PORT:' -- refined below by content
        {py::re::compile(S("\\bSTUB\\b")), Category::STUBBED},
        {py::re::compile(S("\\bFIXME\\b")), Category::WARNING},
        {py::re::compile(S("\\bTODO\\b")), Category::WARNING},
        {py::re::compile(S("\\bXXX\\b")), Category::WARNING},
    };
    return tokens;
}

const py::re::Pattern& _GUESS_HINTS() {
    static const py::re::Pattern p = py::re::compile(
        S("\\b(value type|by value|value-vector|pointer|determine|infer\\w*|assume\\w*|"
          "guess\\w*|element type|was bare|reclassif\\w*|ambiguous|may be|might be)\\b"),
        IGNORECASE);
    return p;
}

const py::re::Pattern& _STUB_HINTS() {
    static const py::re::Pattern p = py::re::compile(
        S("\\b(D3D9|XInput|XAudio|occlusion|reflection|no RTTI|no stack|dumpStack|Thread|"
          "not implemented|unimplemented|removed|no such|replaced by|needs|implement\\b|"
          "LockRect|CreateTexture|gamepad)\\b"),
        IGNORECASE);
    return p;
}

const py::re::Pattern& _COMMENT_RE() {
    static const py::re::Pattern p = py::re::compile(S("(?://+|/\\*+)\\s*(.*?)\\s*(?:\\*/)?\\s*$"));
    return p;
}

std::tuple<py::str, std::optional<py::str>> _classify_marker(const py::str& comment_text, const py::str& base_category) {
    if (base_category == Category::ANNOTATED) {
        if (_GUESS_HINTS().search(comment_text).has_value()) {
            return std::make_tuple(Category::GUESSED, std::optional<py::str>(Severity::HIGH));
        }
        if (_STUB_HINTS().search(comment_text).has_value()) {
            return std::make_tuple(Category::STUBBED, std::optional<py::str>(Severity::HIGH));
        }
    }
    return std::make_tuple(base_category, std::optional<py::str>());   // None -> Event uses the category default
}

int64_t scan_markers(ledger::Ledger& ledger, const py::str& output_dir) {
    int64_t n = 0;
    for (const str& fname : py::sorted(py::os::listdir(output_dir))) {
        if (!(fname.endswith(S(".cpp")) || fname.endswith(S(".h")))) {
            continue;
        }
        str path = py::os::path::join(output_dir, fname);
        py::list<str> lines;
        try {
            py::file f = py::open(path, S("r"), S("utf-8"), S("replace"));
            lines = py::split(f.read_text(), S("\n"));
        } catch (const py::OSError&) {   // (IOError, OSError)
            continue;
        }
        int64_t i = 0;
        for (const str& raw : lines) {
            ++i;
            if (!raw.contains(S("PORT")) && !raw.contains(S("STUB")) && !raw.contains(S("TODO")) && !raw.contains(S("FIXME")) &&
                !raw.contains(S("XXX"))) {
                continue;
            }
            // isolate the comment text (after // or inside /* */); skip if no comment
            std::optional<py::re::Match> cm = _COMMENT_RE().search(raw);
            str comment = cm ? cm->group(1) : raw;
            for (const auto& [tok_re, base_cat] : _MARKER_TOKENS()) {
                std::optional<py::re::Match> m = tok_re.search(comment);
                if (!m) {
                    continue;
                }
                auto [cat, sev] = _classify_marker(comment, base_cat);
                // message = the comment text after the marker token
                str msg = comment.slice(m->end(), comment.size()).lstrip(S(" :)-")).strip();
                if (msg.empty()) {
                    msg = comment.strip();
                }
                ledger.add(fname, cat, msg, i, sev, S("marker"), m->group(0), raw.strip().slice(0, 160));
                ++n;
                break;   // one marker classification per line
            }
        }
    }
    return n;
}

// ---------------------------------------------------------------------------
// 2. Known crash/leak patterns (reuse the existing converter_lint rules)
// ---------------------------------------------------------------------------

int64_t scan_lint(ledger::Ledger& ledger, const py::str& output_dir) {
    (void)ledger;
    (void)output_dir;
    // `import converter_lint` -> ImportError: the module is not bundled, so this pass is skipped.
    return 0;
}

const py::re::Pattern& _CORRUPT_STR_RE() {
    static const py::re::Pattern p =
        py::re::compile(S("\"[^\"\\n]*->(?:png|txt|dat|zip|lvl|class|gif|ogg|wav|properties|lang|nbt|mclevel)[^\"\\n]*\""));
    return p;
}

int64_t scan_corrupted_strings(ledger::Ledger& ledger, const py::str& output_dir) {
    int64_t n = 0;
    for (const str& fname : py::sorted(py::os::listdir(output_dir))) {
        if (!(fname.endswith(S(".cpp")) || fname.endswith(S(".h")))) {
            continue;
        }
        try {
            // (no encoding given: the locale's, as for the Python)
            py::file fh = py::open(py::os::path::join(output_dir, fname), S("r"), std::nullopt, S("replace"));
            int64_t i = 0;
            for (const str& raw : fh.readlines_text()) {
                ++i;
                std::optional<py::re::Match> m = _CORRUPT_STR_RE().search(raw);
                if (m) {
                    ledger.add(fname, Category::RISK,
                               S("corrupted string literal (member-access \".\" -> \"->\" leaked into a "
                                 "string -> broken resource path): ") +
                                   m->group(0),
                               i, Severity::HIGH, S("lint"), S("high/corrupt-string-literal"), raw.strip());
                    ++n;
                }
            }
        } catch (const py::OSError&) {   // (IOError, OSError)
        }
    }
    return n;
}

// ---------------------------------------------------------------------------
// 3. Compiler errors from a build log (optional -- newest log by default)
// ---------------------------------------------------------------------------

const py::re::Pattern& _MSVC_RE() {
    static const py::re::Pattern p =
        py::re::compile(S("([A-Za-z0-9_]+\\.(?:cpp|h))\\((\\d+)\\)\\s*:\\s*(?:fatal\\s+)?error\\s+(C\\d+)\\s*:\\s*(.*)"));
    return p;
}

int64_t scan_build_log(ledger::Ledger& ledger, const std::optional<py::str>& log_path) {
    if (!log_path.has_value() || log_path->empty() || !py::os::path::isfile(*log_path)) {
        return 0;
    }
    int64_t n = 0;
    py::set<std::tuple<str, str, str, str>> seen;
    py::file f = py::open(*log_path, S("r"), S("utf-8"), S("replace"));
    for (const str& raw : f.readlines_text()) {
        std::optional<py::re::Match> m = _MSVC_RE().search(raw);
        if (!m) {
            continue;
        }
        str fname = m->group(1), line = m->group(2), code = m->group(3), msg = m->group(4);
        std::tuple<str, str, str, str> key(fname, line, code, msg.slice(0, 40));
        if (seen.contains(key)) {   // MSVC repeats errors across the /MP cl invocations
            continue;
        }
        seen.add(key);
        // (Event does int(line) on the digits the pattern matched)
        ledger.add(fname, Category::ERROR, msg.strip().slice(0, 200), py::int_parse(line), std::nullopt, S("build"), code, S(""));
        ++n;
    }
    return n;
}

std::optional<py::str> newest_build_log(const py::str& root) {
    std::optional<str> best;
    double best_time = 0;
    for (const str& f : py::os::listdir(root)) {
        if (!(f.startswith(S("build_alpha")) && f.endswith(S(".log")))) {
            continue;
        }
        str p = py::os::path::join(root, f);
        double t = py::os::path::getmtime(p);
        if (!best.has_value() || t > best_time) {   // (max() keeps the first of equals)
            best = p;
            best_time = t;
        }
    }
    return best;
}

// ---------------------------------------------------------------------------
// Orchestration
// ---------------------------------------------------------------------------

std::optional<py::str> _detect_platform(const py::str& output_dir) {
    for (const str& fname : py::sorted(py::os::listdir(output_dir))) {
        if (!fname.endswith(S(".h"))) {
            continue;
        }
        str path = py::os::path::join(output_dir, fname);
        try {
            py::file f = py::open(path, S("r"), S("utf-8"), S("replace"));
            int64_t line_no = 0;
            for (const str& line : f.readlines_text()) {
                if (line_no > 10) {
                    break;
                }
                if (line.contains(S("// Target:"))) {
                    for (const auto& [pid, plat] : platforms::PLATFORMS.items()) {
                        if (line.contains(plat.label)) {
                            return pid;
                        }
                    }
                }
                ++line_no;
            }
        } catch (const py::Exception&) {
            continue;
        }
    }
    return std::nullopt;
}

AnalyzeCounts analyze_all(ledger::Ledger& ledger, const py::str& output_dir, const std::optional<py::str>& build_log_arg,
                          const std::optional<py::str>& root_arg, const std::optional<py::str>& platform_arg) {
    str root = root_arg.has_value() && !root_arg->empty() ? *root_arg : py::os::getcwd();
    std::optional<str> build_log = build_log_arg;
    if (!build_log.has_value()) {
        build_log = newest_build_log(root);
    }
    std::optional<str> platform = platform_arg;
    if (!platform.has_value()) {
        platform = _detect_platform(output_dir);
    }
    AnalyzeCounts counts;
    counts.markers = scan_markers(ledger, output_dir);
    counts.rules = rules::scan_rules(ledger, output_dir, platform);
    counts.lint = scan_lint(ledger, output_dir);
    counts.corrupt_strings = scan_corrupted_strings(ledger, output_dir);
    counts.build_log = scan_build_log(ledger, build_log);
    counts.build_log_path = build_log;
    return counts;
}

}  // namespace codebreaker::java::analyze
