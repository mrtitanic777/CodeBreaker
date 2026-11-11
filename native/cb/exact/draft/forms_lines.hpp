// cb/exact/draft/forms_lines.hpp -- the small readers of text that forms.cpp and forms_flow.cpp share: words bounded the
// way `\b` bounds them, lines split and joined, indentation, numbers. Internal to the forms module.
#pragma once

#include <algorithm>
#include <cctype>
#include <format>
#include <functional>
#include <optional>
#include <set>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/error.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/forms.hpp"

namespace cb::exact::draft::forms::ln {

namespace dt = cb::exact::draft::text;
namespace ct = cb::text;

using Lines = std::vector<std::string>;
using sv = std::string_view;
inline constexpr std::size_t npos = std::string::npos;


inline bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
inline bool blank(sv s) { return ct::trim(s).empty(); }
inline std::size_t indent_of(sv l) { return l.size() - ct::ltrim(l).size(); }
inline std::string lead_ws(sv l) { return std::string(l.substr(0, indent_of(l))); }
inline std::string str(sv s) { return std::string(s); }

// `\b` at position i of s.
inline bool boundary_at(sv s, std::size_t i) {
    const bool before = i > 0 && is_word(s[i - 1]);
    const bool after = i < s.size() && is_word(s[i]);
    return before != after;
}

// The first `\bw\b` at or after `from` (w non-empty).
inline std::size_t find_word(sv s, sv w, std::size_t from = 0) {
    for (std::size_t p = s.find(w, from); p != npos; p = s.find(w, p + 1))
        if (boundary_at(s, p) && boundary_at(s, p + w.size())) return p;
    return npos;
}
inline bool has_word(sv s, sv w) { return find_word(s, w) != npos; }
inline std::size_t count_word(sv s, sv w) {
    std::size_t n = 0;
    for (std::size_t p = find_word(s, w); p != npos; p = find_word(s, w, p + w.size())) ++n;
    return n;
}

// s with `\bw\b` replaced by `with` (the first only, or every one).
inline std::string replace_word(sv s, sv w, sv with, bool first_only = false) {
    std::string out;
    std::size_t at = 0;
    for (std::size_t p = find_word(s, w); p != npos; p = find_word(s, w, at)) {
        out.append(s.substr(at, p - at));
        out.append(with);
        at = p + w.size();
        if (first_only) break;
    }
    out.append(s.substr(at));
    return out;
}

inline Lines split_nl(sv s) {
    Lines out;
    for (sv part : ct::split(s, '\n')) out.emplace_back(part);
    return out;
}
inline std::string join_nl(const Lines& lines) { return ct::join(lines, "\n"); }

// The lines that are not marked gone, joined.
inline std::string join_live(const Lines& lines, const std::vector<char>& gone) {
    std::string out;
    bool first = true;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (gone[i]) continue;
        if (!first) out += '\n';
        out += lines[i];
        first = false;
    }
    return out;
}

inline std::string dedent4(sv x) { return str(x.starts_with("    ") ? x.substr(4) : x); }

// The text with the first match of `re` replaced by fn's result.
template <class F>
inline std::string sub_first(const Regex& re, sv s, F fn) {
    auto m = re.search(s);
    if (!m) return str(s);
    std::string out(s.substr(0, m->start()));
    out += fn(*m);
    out.append(s.substr(m->end()));
    return out;
}

// Every non-overlapping match of `re` replaced by fn's result.
template <class F>
inline std::string sub_all(const Regex& re, sv s, F fn) {
    return re.replace_all(s, std::function<std::string(const RegexMatch&)>(fn));
}

// Identifiers of the text (each maximal run of word characters that starts with a letter or underscore).
inline std::set<std::string> identifiers(sv s) {
    std::set<std::string> out;
    for (std::size_t i = 0; i < s.size();) {
        if (!is_word(s[i])) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < s.size() && is_word(s[e])) ++e;
        if (!is_digit(s[i])) out.emplace(s.substr(i, e - i));
        i = e;
    }
    return out;
}

inline std::int64_t to_int(sv digits) {
    const auto v = ct::parse_int<std::int64_t>(digits);
    CB_ASSERT(v.has_value(), "a number the pattern matched");
    return *v;
}

inline std::string hex_name(std::int64_t n) { return std::format("g_{:04x}", n); }

// Is s `^\s*` + word? i.e. the line (after leading whitespace) begins with `w` as text.
inline bool starts_after_ws(sv line, sv w) { return ct::ltrim(line).starts_with(w); }

inline bool byte_site(const FunctionFacts& facts, sv pointer, std::int64_t offset) {
    return facts.byte_sites.contains(dt::Site{str(pointer), offset});
}

// `\bw\b` is not enough where w starts or ends with a non-word character: this is the `\b`-bounded literal.
inline std::size_t find_bounded(sv s, sv w, std::size_t from = 0) {
    for (std::size_t p = s.find(w, from); p != npos; p = s.find(w, p + 1))
        if (boundary_at(s, p) && boundary_at(s, p + w.size())) return p;
    return npos;
}

inline std::string drop4(sv x) { return x.size() > 4 ? str(x.substr(4)) : std::string(); }

inline std::int64_t brace_balance(sv l) {
    return std::ranges::count(l, '{') - std::ranges::count(l, '}');
}

}  // namespace cb::exact::draft::forms::ln
