// cb-unity: alone (its file-local helpers are named for what they do here, not for a unity neighbour)
#include "cb/java/analyze.hpp"

#include <algorithm>
#include <set>
#include <tuple>
#include <vector>

#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/java/platforms.hpp"
#include "cb/java/rules.hpp"
#include "cb/java/strview.hpp"
#include "cb/java/textutil.hpp"

namespace cb::java::analyze {

namespace {

namespace stdfs = std::filesystem;
using ledger::Category;
using ledger::EventDetail;
using ledger::Severity;

// A marker token and the category a comment holding it gets by default.
struct MarkerToken {
    Regex pattern;
    Category category;
};

// Marker tokens, most specific FIRST so 'PORT-BLOCKED' wins over 'PORT'.
const std::vector<MarkerToken>& marker_tokens() {
    static const std::vector<MarkerToken> tokens = [] {
        std::vector<MarkerToken> t;
        t.push_back({Regex(R"(PORT-BLOCKED)"), Category::Blocked});
        t.push_back({Regex(R"(PORT[- ]WARNING)"), Category::Warning});
        t.push_back({Regex(R"(PORT\s+stub)"), Category::Stubbed});
        t.push_back({Regex(R"(PORT\s+NOTES?)"), Category::Annotated});
        t.push_back({Regex(R"(PORT\s+helpers?)"), Category::Annotated});
        t.push_back({Regex(R"(PORT\([^)]*\))"), Category::Annotated});   // PORT(post-vs-pre-multiply) etc.
        t.push_back({Regex(R"(PORT)"), Category::Annotated});            // a bare 'PORT:' -- sharpened by its content
        t.push_back({Regex(R"(\bSTUB\b)"), Category::Stubbed});
        t.push_back({Regex(R"(\bFIXME\b)"), Category::Warning});
        t.push_back({Regex(R"(\bTODO\b)"), Category::Warning});
        t.push_back({Regex(R"(\bXXX\b)"), Category::Warning});
        return t;
    }();
    return tokens;
}

// Content keywords that RE-CLASSIFY a bare `// PORT:` comment into a sharper bucket. These are the highest-value
// signals: a "guess" is a future bug, a "stub" is unfinished work.
const Regex& guess_hints() {
    static const Regex re(R"(\b(value type|by value|value-vector|pointer|determine|infer\w*|assume\w*|)"
                          R"(guess\w*|element type|was bare|reclassif\w*|ambiguous|may be|might be)\b)",
                          RegexFlags::IgnoreCase);
    return re;
}

const Regex& stub_hints() {
    static const Regex re(R"(\b(D3D9|XInput|XAudio|occlusion|reflection|no RTTI|no stack|dumpStack|Thread|)"
                          R"(not implemented|unimplemented|removed|no such|replaced by|needs|implement\b|)"
                          R"(LockRect|CreateTexture|gamepad)\b)",
                          RegexFlags::IgnoreCase);
    return re;
}

// The text of a line's comment (after `//` or inside `/* */`), trimmed; the whole line if it has no comment.
const Regex& comment_pattern() {
    static const Regex re(R"((?://+|/\*+)\s*(.*?)\s*(?:\*/)?\s*$)");
    return re;
}

// The converted sources of a folder, by name: its .cpp and .h files.
std::vector<std::string> converted_file_names(const stdfs::path& dir, bool headers_only = false) {
    std::vector<std::string> names;
    std::error_code ec;
    for (stdfs::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        const bool header = name.ends_with(".h");
        if (header || (!headers_only && name.ends_with(".cpp"))) names.push_back(name);
    }
    std::sort(names.begin(), names.end());
    return names;
}

bool mentions_a_marker(std::string_view line) {
    for (std::string_view word : {"PORT", "STUB", "TODO", "FIXME", "XXX"}) {
        if (contains(line, word)) return true;
    }
    return false;
}

// `: )-` stripped from the left, then whitespace from both ends.
std::string_view marker_message(std::string_view after_token) {
    const std::size_t first = after_token.find_first_not_of(" :)-");
    return text::trim(first == std::string_view::npos ? std::string_view() : after_token.substr(first));
}

}  // namespace

std::size_t scan_markers(ledger::Ledger& ledger, const stdfs::path& output_dir) {
    std::size_t added = 0;
    for (const std::string& fname : converted_file_names(output_dir)) {
        const std::optional<std::string> content = textutil::try_read_text(output_dir / fs::from_text(fname));
        if (!content) continue;
        std::uint32_t number = 0;
        for (std::string_view raw : text::split(*content, '\n')) {
            ++number;
            if (!mentions_a_marker(raw)) continue;
            const std::optional<RegexMatch> cm = comment_pattern().search(raw);
            const std::string_view comment = cm ? cm->group(1) : raw;
            for (const MarkerToken& token : marker_tokens()) {
                const std::optional<RegexMatch> m = token.pattern.search(comment);
                if (!m) continue;
                Category category = token.category;
                std::optional<Severity> severity;   // nullopt: the category's default
                if (category == Category::Annotated) {
                    if (guess_hints().test(comment)) {
                        category = Category::Guessed;
                        severity = Severity::High;
                    } else if (stub_hints().test(comment)) {
                        category = Category::Stubbed;
                        severity = Severity::High;
                    }
                }
                // the message is the comment text after the marker token
                std::string_view message = marker_message(comment.substr(m->end()));
                if (message.empty()) message = text::trim(comment);
                ledger.add(fname, category, std::string(message),
                           {.line = number,
                            .severity = severity,
                            .source = "marker",
                            .rule = std::string(m->whole()),
                            .snippet = std::string(text::utf8_prefix(text::trim(raw), 160))});
                ++added;
                break;   // one marker classification per line
            }
        }
    }
    return added;
}

std::size_t scan_corrupted_strings(ledger::Ledger& ledger, const stdfs::path& output_dir) {
    static const Regex corrupt(
        R"("[^"\n]*->(?:png|txt|dat|zip|lvl|class|gif|ogg|wav|properties|lang|nbt|mclevel)[^"\n]*")");
    std::size_t found = 0;
    for (const std::string& fname : converted_file_names(output_dir)) {
        const std::optional<std::string> content = textutil::try_read_text(output_dir / fs::from_text(fname));
        if (!content) continue;
        std::uint32_t number = 0;
        for (std::string_view raw : text::split(*content, '\n')) {
            ++number;
            const std::optional<RegexMatch> m = corrupt.search(raw);
            if (!m) continue;
            ledger.add(fname, Category::Risk,
                       "corrupted string literal (member-access \".\" -> \"->\" leaked into a string -> broken resource path): " +
                           std::string(m->whole()),
                       {.line = number,
                        .severity = Severity::High,
                        .source = "lint",
                        .rule = "high/corrupt-string-literal",
                        .snippet = std::string(text::trim(raw))});
            ++found;
        }
    }
    return found;
}

std::size_t scan_build_log(ledger::Ledger& ledger, const stdfs::path& log_path) {
    static const Regex msvc(R"(([A-Za-z0-9_]+\.(?:cpp|h))\((\d+)\)\s*:\s*(?:fatal\s+)?error\s+(C\d+)\s*:\s*(.*))");
    const std::optional<std::string> content = textutil::try_read_text(log_path);
    if (!content) return 0;
    std::size_t found = 0;
    std::set<std::tuple<std::string, std::string, std::string, std::string>> seen;
    for (std::string_view raw : text::split(*content, '\n')) {
        const std::optional<RegexMatch> m = msvc.search(raw);
        if (!m) continue;
        const std::string_view message = m->group(4);
        // MSVC repeats errors across its parallel compiler invocations
        const auto key = std::make_tuple(std::string(m->group(1)), std::string(m->group(2)), std::string(m->group(3)),
                                         std::string(text::utf8_prefix(message, 40)));
        if (!seen.insert(key).second) continue;
        const std::uint32_t line = text::parse_int<std::uint32_t>(m->group(2)).value_or(0);
        ledger.add(std::string(m->group(1)), Category::Error, std::string(text::utf8_prefix(text::trim(message), 200)),
                   {.line = line, .source = "build", .rule = std::string(m->group(3))});
        ++found;
    }
    return found;
}

std::optional<stdfs::path> newest_build_log(const stdfs::path& root) {
    std::optional<stdfs::path> newest;
    stdfs::file_time_type newest_time;
    std::error_code ec;
    for (stdfs::directory_iterator it(root, ec), end; !ec && it != end; it.increment(ec)) {
        const std::string name = fs::to_text(it->path().filename());
        if (!name.starts_with("build_alpha") || !name.ends_with(".log")) continue;
        std::error_code time_ec;
        const stdfs::file_time_type t = stdfs::last_write_time(it->path(), time_ec);
        if (time_ec) continue;
        if (!newest || t > newest_time) {
            newest = it->path();
            newest_time = t;
        }
    }
    return newest;
}

std::optional<std::string> detect_platform(const stdfs::path& output_dir) {
    constexpr std::size_t kHeaderLines = 11;   // the target is named in the first lines of a header
    for (const std::string& fname : converted_file_names(output_dir, /*headers_only=*/true)) {
        const std::optional<std::string> content = textutil::try_read_text(output_dir / fs::from_text(fname));
        if (!content) continue;
        std::size_t seen = 0;
        for (std::string_view line : text::split(*content, '\n')) {
            if (seen++ >= kHeaderLines) break;
            if (!contains(line, "// Target:")) continue;
            for (const platforms::Platform& p : platforms::platforms()) {
                if (contains(line, p.label)) return std::string(p.key);
            }
        }
    }
    return std::nullopt;
}

Counts analyze_all(ledger::Ledger& ledger, const stdfs::path& output_dir, const Options& options) {
    Counts counts;
    const stdfs::path root = options.root ? *options.root : stdfs::current_path();
    counts.build_log_path = options.build_log ? options.build_log : newest_build_log(root);
    const std::optional<std::string> platform = options.platform ? options.platform : detect_platform(output_dir);

    counts.markers = scan_markers(ledger, output_dir);
    counts.rules = rules::scan_rules(ledger, output_dir, {.platform = platform.value_or(""), .profile = options.profile});
    counts.corrupt_strings = scan_corrupted_strings(ledger, output_dir);
    if (counts.build_log_path) counts.build_log = scan_build_log(ledger, *counts.build_log_path);
    return counts;
}

}  // namespace cb::java::analyze
