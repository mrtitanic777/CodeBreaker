// cb/core/regex.hpp -- cb::Regex, a small backtracking regular-expression engine for the few structural
// patterns CodeBreaker has left once idioms became code (docs/CPP_DESIGN.md 3.7).
//
// Syntax (ECMAScript-like): literals, `.`, classes `[a-z_]` / `[^...]` (a leading `]` is a literal), `\d \w \s`
// and their negations -- ASCII only --, `\b \B`, anchors `^ $` (line anchors with Multiline, else text anchors)
// and `\A \z` (always text anchors), groups `( )`, `(?: )`, `(?<name> )` (also `(?P<name> )`), lookahead
// `(?= )` `(?! )`, FIXED-width lookbehind `(?<= )` `(?<! )`, alternation `|`, quantifiers `* + ? {n} {n,} {n,m}`
// each with a lazy `?` form, one backreference form `\1`..`\9`, escapes `\n \r \t \f \v \0 \xHH`, and a leading
// `(?i)`/`(?m)`/`(?s)` group of inline flags. Not supported (a ParseError, not silent): possessive quantifiers,
// atomic groups, `\Z`, `\p{..}`, Unicode classes, non-ASCII bytes inside a `[...]`.
//
// Matching is over BYTES: offsets are byte offsets, `.` and negated classes consume one byte, `\w \d \s` are
// ASCII. UTF-8 text passes through intact (a multi-byte character is several bytes, each matched by `.`).
//
// A pattern is compiled ONCE: `static const cb::Regex re(R"(...)");`. A Regex constructed in a loop is a review
// failure. A Regex is immutable and may be used from many threads at once.
//
// A match runs under a STEP BUDGET (default 10 million VM steps per search call). A pattern/input pair that
// exceeds it does not hang: `try_search` etc. return a Result error, and the throwing forms throw
// cb::UnsupportedError("regex", ...). A Match holds string_views into the subject: the subject must outlive it.
#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"

namespace cb {

enum class RegexFlags : unsigned {
    None = 0,
    IgnoreCase = 1,   // ASCII case folding
    Multiline = 2,    // ^ and $ match at line boundaries
    DotAll = 4,       // . also matches '\n'
};
constexpr RegexFlags operator|(RegexFlags a, RegexFlags b) {
    return static_cast<RegexFlags>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}
constexpr bool has_flag(RegexFlags set, RegexFlags f) { return (static_cast<unsigned>(set) & static_cast<unsigned>(f)) != 0; }

inline constexpr std::uint64_t kDefaultRegexSteps = 10'000'000;

// One successful match: group 0 is the whole match.
class RegexMatch {
public:
    std::size_t group_count() const { return caps_.size() / 2; }       // including group 0
    bool has(std::size_t group) const { return group < group_count() && caps_[2 * group] != kNone; }
    // The text of the group; empty if it did not take part (check has() to tell that from an empty match).
    std::string_view group(std::size_t group) const;
    std::string_view operator[](std::size_t g) const { return group(g); }
    std::string_view whole() const { return group(0); }
    std::size_t start(std::size_t group = 0) const { return has(group) ? static_cast<std::size_t>(caps_[2 * group]) : npos; }
    std::size_t end(std::size_t group = 0) const { return has(group) ? static_cast<std::size_t>(caps_[2 * group + 1]) : npos; }
    // A named group's text; nullopt if the name is unknown or the group did not take part.
    std::optional<std::string_view> named(std::string_view name) const;
    std::string_view subject() const { return subject_; }
    static constexpr std::size_t npos = static_cast<std::size_t>(-1);

private:
    friend struct RegexMatchAccess;
    static constexpr std::ptrdiff_t kNone = -1;
    std::string_view subject_;
    std::vector<std::ptrdiff_t> caps_;
    std::shared_ptr<const std::vector<std::pair<std::string, std::size_t>>> names_;   // shared with the Regex
};

class Regex {
public:
    // Compile; throws cb::ParseError (file "<regex>", column = offset in the pattern + 1) on a bad pattern.
    explicit Regex(std::string_view pattern, RegexFlags flags = RegexFlags::None,
                   std::uint64_t step_budget = kDefaultRegexSteps);
    // Compile without throwing.
    static Result<Regex> compile(std::string_view pattern, RegexFlags flags = RegexFlags::None,
                                 std::uint64_t step_budget = kDefaultRegexSteps);

    const std::string& pattern() const;
    std::size_t group_count() const;   // capture groups, excluding group 0
    std::optional<std::size_t> group_index(std::string_view name) const;

    // First match at or after `from`.
    std::optional<RegexMatch> search(std::string_view subject, std::size_t from = 0) const;
    // A match that begins exactly at `from`.
    std::optional<RegexMatch> match(std::string_view subject, std::size_t from = 0) const;
    // A match that covers the whole subject.
    std::optional<RegexMatch> full_match(std::string_view subject) const;
    // Whether anything matches (search without building the groups' text).
    bool test(std::string_view subject) const { return search(subject).has_value(); }

    // The same, reporting an exceeded step budget as a Result error instead of throwing.
    Result<std::optional<RegexMatch>> try_search(std::string_view subject, std::size_t from = 0) const;
    Result<std::optional<RegexMatch>> try_match(std::string_view subject, std::size_t from = 0) const;
    Result<std::optional<RegexMatch>> try_full_match(std::string_view subject) const;

    // Call fn for each non-overlapping match, left to right; stop when fn returns false. An empty match is
    // reported once per position and the scan moves one byte on. Returns how many matches fn saw.
    std::size_t for_each_match(std::string_view subject, const std::function<bool(const RegexMatch&)>& fn) const;
    // Every match's whole text.
    std::vector<std::string> find_all(std::string_view subject) const;

    // Replace matches. The template understands `$1`..`$9`, `${name}`, `$&` (the whole match) and `$$`;
    // a group that did not take part is empty. UsageError if it names a group the pattern does not have.
    std::string replace_all(std::string_view subject, std::string_view replacement) const;
    std::string replace_first(std::string_view subject, std::string_view replacement) const;
    // Replace each match by what fn returns.
    std::string replace_all(std::string_view subject, const std::function<std::string(const RegexMatch&)>& fn) const;

    // The pieces of the subject between matches (captured groups are not included; empty matches do not split).
    std::vector<std::string> split(std::string_view subject) const;

    struct Program;   // the compiled form (an implementation detail; public only so regex.cpp can name it)

private:
    std::shared_ptr<const Program> prog_;
};

}  // namespace cb
