#include "cb/exact/draft/shapes_scan.hpp"

#include <algorithm>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/text.hpp"

namespace cb::exact::draft::shapes::scan {

namespace txt = cb::text;
using cb::Regex;
using cb::RegexMatch;
using std::string;

Lines split_lines(std::string_view text) {
    Lines out;
    std::size_t from = 0;
    for (;;) {
        const std::size_t nl = text.find('\n', from);
        if (nl == std::string_view::npos) {
            out.emplace_back(text.substr(from));
            return out;
        }
        out.emplace_back(text.substr(from, nl - from));
        from = nl + 1;
    }
}

std::string join_lines(std::span<const std::string> lines) { return txt::join(lines, "\n"); }

std::string join_live(std::span<const std::optional<std::string>> lines) {
    std::string out;
    bool first = true;
    for (const auto& l : lines) {
        if (!l) continue;
        if (!first) out += '\n';
        out += *l;
        first = false;
    }
    return out;
}

Lines live_lines(std::span<const std::optional<std::string>> lines) {
    Lines out;
    for (const auto& l : lines)
        if (l) out.push_back(*l);
    return out;
}

MaybeLines maybe(std::span<const std::string> lines) { return MaybeLines(lines.begin(), lines.end()); }

std::size_t indent_of(std::string_view line) { return line.size() - txt::ltrim(line).size(); }
std::string_view indent_text(std::string_view line) { return line.substr(0, indent_of(line)); }

std::string dedent(std::string_view line, std::string_view ind) {
    if (line.starts_with(ind) && line.substr(ind.size()).starts_with("    ")) return std::string(line.substr(4));
    return std::string(line);
}

bool blank(std::string_view line) { return txt::trim(line).empty(); }

bool is_word_char(char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_'; }

bool boundary_at(std::string_view s, std::size_t pos) {
    const bool before = pos > 0 && is_word_char(s[pos - 1]);
    const bool after = pos < s.size() && is_word_char(s[pos]);
    return before != after;
}

std::optional<std::size_t> find_word(std::string_view s, std::string_view name, std::size_t from) {
    for (std::size_t p = from; p + name.size() <= s.size(); ++p)
        if (s.compare(p, name.size(), name) == 0 && boundary_at(s, p) && boundary_at(s, p + name.size())) return p;
    return std::nullopt;
}

std::size_t count_word(std::string_view s, std::string_view name) {
    std::size_t n = 0;
    for (std::size_t p = 0; p + name.size() <= s.size();) {
        const auto at = find_word(s, name, p);
        if (!at) break;
        ++n;
        p = *at + std::max<std::size_t>(name.size(), 1);   // (an empty name matches at each boundary: step on)
    }
    return n;
}

bool has_word(std::string_view s, std::string_view name) { return find_word(s, name).has_value(); }

std::string replace_word_first(std::string_view s, std::string_view name, std::string_view with) {
    const auto at = find_word(s, name);
    if (!at) return std::string(s);
    return std::string(s.substr(0, *at)) + std::string(with) + std::string(s.substr(*at + name.size()));
}

std::string replace_word_all(std::string_view s, std::string_view name, std::string_view with) {
    std::string out;
    std::size_t from = 0;
    for (;;) {
        const auto at = find_word(s, name, from);
        if (!at) break;
        out.append(s.substr(from, *at - from));
        out.append(with);
        from = *at + name.size();
        if (name.empty()) {   // (an empty name: one match per boundary; keep the byte there)
            if (from < s.size()) out += s[from];
            ++from;
        }
    }
    if (from <= s.size()) out.append(s.substr(from));
    return out;
}

namespace {

// After the name at `q`: blanks, then `=` and one more byte that is not `=`.
bool assignment_follows(std::string_view s, std::size_t q) {
    while (q < s.size() && txt::is_space(s[q])) ++q;
    return q + 1 < s.size() && s[q] == '=' && s[q + 1] != '=';
}

bool step_follows(std::string_view s, std::size_t q) {
    while (q < s.size() && txt::is_space(s[q])) ++q;
    return q + 1 < s.size() && (s.substr(q, 2) == "++" || s.substr(q, 2) == "--");
}

}  // namespace

bool is_assigned(std::string_view s, std::string_view name) {
    for (std::size_t p = 0; p + name.size() <= s.size(); ++p)
        if (s.compare(p, name.size(), name) == 0 && boundary_at(s, p) && assignment_follows(s, p + name.size())) return true;
    return false;
}

bool is_assigned_after_non_name(std::string_view s, std::string_view name) {
    for (std::size_t p = 0; p + name.size() <= s.size(); ++p) {
        if (s.compare(p, name.size(), name) != 0) continue;
        if (p > 0 && (is_word_char(s[p - 1]) || s[p - 1] == '>' || s[p - 1] == '.')) continue;
        if (assignment_follows(s, p + name.size()) || step_follows(s, p + name.size())) return true;
    }
    return false;
}

bool is_updated(std::string_view s, std::string_view name) {
    for (std::size_t p = 0; p + name.size() <= s.size(); ++p) {
        if (s.compare(p, name.size(), name) != 0) continue;
        if (boundary_at(s, p) && (assignment_follows(s, p + name.size()) || step_follows(s, p + name.size()))) return true;
    }
    // ++name / --name: the step, blanks, the name as a word
    for (std::size_t p = 0; p + 2 <= s.size(); ++p) {
        if (s.substr(p, 2) != "++" && s.substr(p, 2) != "--") continue;
        std::size_t q = p + 2;
        while (q < s.size() && txt::is_space(s[q])) ++q;
        if (s.compare(q, name.size(), name) == 0 && boundary_at(s, q + name.size())) return true;
    }
    return false;
}

std::string until_given(std::string_view text_in, std::string_view reg, bool unless_read) {
    const std::string text(text_in);
    const std::string head = std::string(reg) + " = ";
    for (std::size_t p = 0; p < text.size(); ++p) {
        if (text[p] != '\n') continue;
        std::size_t q = p + 1;
        while (q < text.size() && txt::is_space(text[q])) ++q;
        if (text.compare(q, head.size(), head) != 0) continue;
        if (unless_read) {
            const std::size_t from = q + head.size();
            const std::size_t end = text.find_first_of(";\n", from);
            const std::string_view run = std::string_view(text).substr(from, end == std::string::npos ? std::string::npos : end - from);
            if (has_word(run, reg)) continue;
        }
        return text.substr(0, p);
    }
    return text;
}

bool live_from(std::span<const std::string> lines, std::size_t first, std::string_view reg,
               std::span<const std::string_view> reads) {
    return cb::exact::draft::text::live_after(lines, first, reg, reads);
}

bool live_from(std::span<const std::optional<std::string>> lines, std::size_t first, std::string_view reg,
               std::span<const std::string_view> reads) {
    return cb::exact::draft::text::live_after(lines, first, reg, reads);
}

bool is_case_label_at(std::string_view line, std::string_view ind) {
    if (!line.starts_with(ind)) return false;
    const std::string_view s = line.substr(ind.size());
    if (s.empty() || s.back() != ':') return false;
    if (s == "default:") return true;
    if (!s.starts_with("case ")) return false;
    const std::string_view mid = s.substr(5, s.size() - 6);
    return !mid.empty() && mid.find(':') == std::string_view::npos;
}

bool is_case_label(std::string_view line) {
    const std::string_view s = txt::ltrim(line);
    if (s.empty() || s.back() != ':') return false;
    if (s == "default:") return true;
    if (!s.starts_with("case ")) return false;
    const std::string_view mid = s.substr(5, s.size() - 6);
    return !mid.empty() && mid.find(':') == std::string_view::npos;
}

std::optional<std::int64_t> number(std::string_view s) { return txt::parse_int<std::int64_t>(s, 10); }


Lines slice(const Lines& v, std::size_t from, std::size_t to) {
    from = std::min(from, v.size());
    to = std::min(std::max(to, from), v.size());
    return Lines(v.begin() + static_cast<std::ptrdiff_t>(from), v.begin() + static_cast<std::ptrdiff_t>(to));
}

// The text where each `goto L;` is used: the labels something goes to.
std::set<string> used_labels(std::string_view text) {
    static const Regex jump(R"re(goto (L_\w+);)re");
    std::set<string> used;
    jump.for_each_match(text, [&](const RegexMatch& m) {
        used.insert(std::string(m.group(1)));
        return true;
    });
    return used;
}

bool goes_to(std::string_view text, std::string_view label) {   // `\bgoto L;`
    const std::string needle = cat("goto ", label, ";");
    for (std::size_t at = text.find(needle); at != std::string_view::npos; at = text.find(needle, at + 1))
        if (boundary_at(text, at)) return true;
    return false;
}

std::size_t count_gotos(std::string_view text, std::string_view label) {
    const std::string needle = cat("goto ", label, ";");
    std::size_t n = 0;
    for (std::size_t at = text.find(needle); at != std::string_view::npos; at = text.find(needle, at + 1))
        if (boundary_at(text, at)) ++n;
    return n;
}

// A label line `[ \t]*LABEL:` removed, with its newline (the last line has none to take).
Lines without_label(const Lines& lines, std::string_view label) {
    const std::string want = cat(label, ":");
    Lines out;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const std::string_view l = lines[i];
        const std::size_t lead = l.find_first_not_of(" \t");
        const bool is_label = lead != std::string_view::npos && l.substr(lead) == want;
        if (is_label && i + 1 < lines.size()) continue;
        out.push_back(lines[i]);
    }
    return out;
}

// The lines that are an unused `L_xxx:` label are dropped.
Lines drop_unused_labels(const Lines& lines, const std::set<string>& used) {
    static const Regex label_only(R"re([ \t]*(L_\w+):)re");
    Lines out;
    for (const string& x : lines) {
        if (label_only.full_match(x) && !used.contains(std::string(txt::trim(x).substr(0, txt::trim(x).size() - 1)))) continue;
        out.push_back(x);
    }
    return out;
}

// `\s*REG = (.*);\s*$` (with `.+` when `nonempty`): the value a line gives the register, up to the last `;`.
std::optional<std::string> value_given(std::string_view line, std::string_view reg, bool nonempty) {
    const std::string_view s = txt::ltrim(line);
    const std::string head = cat(reg, " = ");
    if (!s.starts_with(head)) return std::nullopt;
    const std::string_view rest = txt::rtrim(s.substr(head.size()));
    if (rest.empty() || rest.back() != ';') return std::nullopt;
    const std::string_view value = rest.substr(0, rest.size() - 1);
    if (nonempty && value.empty()) return std::nullopt;
    return std::string(value);
}

// `\s*REG = [^;]+;` as a whole line: the statement giving the register a value, nothing after its `;`.
std::optional<std::string> plain_value(std::string_view line, std::string_view reg) {
    const std::string_view s = txt::ltrim(line);
    const std::string head = cat(reg, " = ");
    if (!s.starts_with(head) || s.back() != ';') return std::nullopt;
    const std::string_view value = s.substr(head.size(), s.size() - head.size() - 1);
    if (value.empty() || value.find(';') != std::string_view::npos) return std::nullopt;
    return std::string(value);
}

// `<ind>return\b[^;]*;` as a whole line.
bool is_return_at(std::string_view line, std::string_view prefix) {
    if (!line.starts_with(prefix)) return false;
    std::string_view s = line.substr(prefix.size());
    if (!s.starts_with("return")) return false;
    s.remove_prefix(6);
    if (!s.empty() && is_word_char(s[0])) return false;
    const std::size_t semi = s.find(';');
    return semi != std::string_view::npos && semi + 1 == s.size();
}


}  // namespace cb::exact::draft::shapes::scan
