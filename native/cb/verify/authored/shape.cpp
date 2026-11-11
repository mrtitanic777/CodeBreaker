#include "cb/verify/authored/shape.hpp"

#include <algorithm>
#include <format>
#include <set>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"

namespace cb::verify::authored::shape {

namespace {

// std::string::contains is C++23; this answers for strings and for sets and maps alike
template <class Haystack, class Needle>
bool contains(const Haystack& haystack, const Needle& needle) {
    if constexpr (requires { haystack.find(needle) != Haystack::npos; })
        return haystack.find(needle) != Haystack::npos;
    else
        return haystack.contains(needle);
}

constexpr std::size_t kNone = std::string_view::npos;

bool is_word(char c) { return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '_'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool is_hex(char c) { return is_digit(c) || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

std::size_t skip_space(std::string_view s, std::size_t i) {
    while (i < s.size() && text::is_space(s[i])) ++i;
    return i;
}

// `word` at s[i], ending at a word boundary (the end of `s` counts as one): the index after it, else kNone.
std::size_t word_at(std::string_view s, std::size_t i, std::string_view word) {
    if (!s.substr(i).starts_with(word)) return kNone;
    const std::size_t after = i + word.size();
    return after < s.size() && is_word(s[after]) ? kNone : after;
}

// [A-Za-z_]\w* at s[i]: the index after it, else kNone.
std::size_t name_at(std::string_view s, std::size_t i) {
    if (i >= s.size() || !(std::isalpha(static_cast<unsigned char>(s[i])) || s[i] == '_')) return kNone;
    while (i < s.size() && is_word(s[i])) ++i;
    return i;
}

// -?0x[0-9a-fA-F]+ or -?\d+ at s[i]: the index after it, else kNone.
std::size_t number_at(std::string_view s, std::size_t i) {
    if (i < s.size() && s[i] == '-') ++i;
    if (s.substr(i).starts_with("0x") && i + 2 < s.size() && is_hex(s[i + 2])) {
        i += 2;
        while (i < s.size() && is_hex(s[i])) ++i;
        return i;
    }
    if (i >= s.size() || !is_digit(s[i])) return kNone;
    while (i < s.size() && is_digit(s[i])) ++i;
    return i;
}

// The index of the bracket that closes the one at `i`, or kNone.
std::size_t close_of(std::string_view s, std::size_t i, char open = '{', char close = '}') {
    int depth = 0;
    for (std::size_t j = i; j < s.size(); ++j) {
        if (s[j] == open) {
            ++depth;
        } else if (s[j] == close) {
            --depth;
            if (depth == 0) return j;
        }
    }
    return kNone;
}

// the number of times `name` stands as a word of `s`
std::size_t count_word(std::string_view s, std::string_view name) {
    std::size_t n = 0;
    for (std::size_t at = s.find(name); at != kNone; at = s.find(name, at + 1)) {
        const bool before = at == 0 || !is_word(s[at - 1]);
        const bool after = at + name.size() >= s.size() || !is_word(s[at + name.size()]);
        n += before && after;
    }
    return n;
}

bool contains_word(std::string_view s, std::string_view word) { return count_word(s, word) != 0; }

// A constant of C source as the unsigned 32 bits the compiler keeps of it. nullopt for one that is octal (a leading 0:
// text::parse_int would read it as decimal) or that does not fit 64 bits.
std::optional<std::uint32_t> constant_bits(std::string_view s) {
    std::string_view digits = s;
    if (digits.starts_with('-')) digits.remove_prefix(1);
    if (digits.size() > 1 && digits[0] == '0' && is_digit(digits[1]) && digits.find_first_not_of('0') != kNone) return std::nullopt;   // octal (zeros alone are zero)
    const auto v = text::parse_int<std::int64_t>(s, 0);
    if (!v) return std::nullopt;
    return static_cast<std::uint32_t>(*v);
}

struct SwitchCase {
    std::string_view value;
    std::size_t open, close;   // the braces of its block
};

// `else if (v == N) {` at s[at] (white space may come first): the constant and the brace, if so
std::optional<SwitchCase> else_if_at(std::string_view s, std::size_t at, std::string_view v) {
    std::size_t i = skip_space(s, at);
    constexpr std::string_view kLead = "else if (";
    if (!s.substr(i).starts_with(kLead)) return std::nullopt;
    i += kLead.size();
    if (!s.substr(i).starts_with(v)) return std::nullopt;
    i += v.size();
    if (!s.substr(i).starts_with(" == ")) return std::nullopt;
    i += 4;
    const std::size_t end = number_at(s, i);
    if (end == kNone || !s.substr(end).starts_with(") {")) return std::nullopt;
    return SwitchCase{s.substr(i, end - i), end + 2, 0};
}

// `else {` at s[at] (white space may come first): the brace
std::optional<std::size_t> else_at(std::string_view s, std::size_t at) {
    const std::size_t i = skip_space(s, at);
    if (!s.substr(i).starts_with("else {")) return std::nullopt;
    return i + 5;
}

// Every `r = x;` that is the last thing done before control reaches the end of the function, found in the statement at
// [start, end) and in the last statements of the branches of an if.
void tail_edits(std::string_view body, std::string_view shadow, std::size_t start, std::size_t end, std::string_view r,
                std::vector<Rewrite>& edits) {
    const std::string_view statement = shadow.substr(start, end - start);
    // `r = <anything but ; { }>;` and nothing after it
    if (statement.starts_with(r) && statement.substr(r.size()).starts_with(" = ") && !contains(statement, '{')) {
        const std::size_t value_at = r.size() + 3;
        const std::size_t semicolon = statement.find(';', value_at);
        const bool plain = semicolon != kNone && semicolon > value_at && !contains(statement.substr(value_at, semicolon - value_at), '}') &&
                           text::trim(statement.substr(semicolon + 1)).empty();
        if (plain) {
            edits.push_back({start, end, "return " + std::string(text::trim(body.substr(start + value_at, semicolon - value_at))) + ";"});
            return;
        }
    }
    // an if
    if (!statement.starts_with("if")) return;
    std::size_t paren = skip_space(statement, 2);
    if (paren >= statement.size() || statement[paren] != '(') return;
    std::size_t at = start + paren;
    std::size_t close = close_of(shadow, at, '(', ')');
    while (close != kNone) {
        std::size_t brace = close + 1;
        while (brace < end && text::is_space(shadow[brace])) ++brace;
        if (brace >= end || shadow[brace] != '{') return;
        std::size_t stop = close_of(shadow, brace);
        if (stop == kNone) return;
        const auto inner = statements(shadow, brace + 1, stop);
        if (!inner.empty()) tail_edits(body, shadow, inner.back().begin, inner.back().end, r, edits);
        // `else`, then a block or another if
        const std::string_view after = shadow.substr(stop + 1, end > stop + 1 ? end - stop - 1 : 0);
        const std::size_t kw = word_at(after, skip_space(after, 0), "else");
        if (kw == kNone) return;
        at = stop + 1 + skip_space(after, kw);
        if (at < shadow.size() && shadow[at] == '{') {
            stop = close_of(shadow, at);
            const auto block = stop != kNone ? statements(shadow, at + 1, stop) : std::vector<Span>{};
            if (!block.empty()) tail_edits(body, shadow, block.back().begin, block.back().end, r, edits);
            return;
        }
        const std::string_view rest = shadow.substr(at, end - at);
        if (!rest.starts_with("if")) return;
        const std::size_t p = skip_space(rest, 2);
        if (p >= rest.size() || rest[p] != '(') return;
        at += p;
        close = close_of(shadow, at, '(', ')');
    }
}

// the type word of a declaration `T name;`, when `d` is exactly that for `name`
std::optional<std::string_view> declared_type_of(std::string_view d, std::string_view name) {
    d = text::ltrim(d);
    std::size_t i = 0;
    while (i < d.size() && is_word(d[i])) ++i;
    if (i == 0) return std::nullopt;
    const std::string_view type = d.substr(0, i);
    const std::size_t after_type = skip_space(d, i);
    if (after_type == i) return std::nullopt;   // white space must separate the type from the name
    if (!d.substr(after_type).starts_with(name)) return std::nullopt;
    const std::size_t after_name = after_type + name.size();
    if (after_name >= d.size() || d[after_name] != ';' || !text::trim(d.substr(after_name + 1)).empty()) return std::nullopt;
    return type;
}

// the type word of a parameter `T name` of the heading (after the `(` or `,` before it, and before the `,` or `)` after it)
std::optional<std::string_view> parameter_type_of(std::string_view head, std::string_view name) {
    for (std::size_t at = 0; at < head.size(); ++at) {
        if (head[at] != '(' && head[at] != ',') continue;
        std::size_t i = skip_space(head, at + 1);
        const std::size_t word_begin = i;
        while (i < head.size() && is_word(head[i])) ++i;
        if (i == word_begin) continue;
        const std::string_view type = head.substr(word_begin, i - word_begin);
        const std::size_t after_type = skip_space(head, i);
        if (after_type == i || !head.substr(after_type).starts_with(name)) continue;
        const std::size_t after_name = skip_space(head, after_type + name.size());
        if (after_name < head.size() && (head[after_name] == ',' || head[after_name] == ')')) return type;
    }
    return std::nullopt;
}

}  // namespace

std::vector<Span> statements(std::string_view shadow, std::size_t lo, std::size_t hi) {
    std::vector<Span> out;
    hi = std::min(hi, shadow.size());   // (the Python indexed past the end and raised IndexError)
    std::size_t i = lo;
    while (i < hi) {
        i = std::min(skip_space(shadow.substr(0, hi), i), hi);
        if (i >= hi) break;
        const std::size_t start = i;
        int depth = 0;
        const bool is_do = word_at(shadow.substr(0, hi), i, "do") != kNone;
        while (i < hi) {
            const char c = shadow[i];
            if (c == '(') {
                ++depth;
            } else if (c == ')') {
                --depth;
            } else if (c == ';' && depth == 0) {
                ++i;
                break;
            } else if (c == '{' && depth == 0) {
                const std::size_t j = close_of(shadow, i);
                if (j == kNone || j >= hi) return out;   // not well formed: say nothing more
                i = j + 1;
                const std::string_view rest = shadow.substr(i, hi - i);
                const std::size_t else_at_ = word_at(rest, skip_space(rest, 0), "else");
                if (else_at_ != kNone) {
                    i += else_at_;
                    continue;
                }
                if (is_do && word_at(rest, skip_space(rest, 0), "while") != kNone) continue;   // do { } while (...);
                break;
            }
            ++i;
        }
        out.push_back({start, i});
    }
    return out;
}

std::vector<Rewrite> switches(std::string_view body, std::string_view shadow) {
    static const Regex chain_head(R"(\bif \(([A-Za-z_]\w*) == (-?0x[0-9a-fA-F]+|-?\d+)\) \{)");
    std::vector<Rewrite> out;
    chain_head.for_each_match(shadow, [&](const RegexMatch& m) {
        const std::string_view before = text::rtrim(shadow.substr(0, m.start()));
        if (before.ends_with("else") || (!before.empty() && !contains(std::string_view(";{}"), before.back()))) return true;
        const std::string_view v = m.group(1);
        std::vector<SwitchCase> cases;
        std::optional<std::pair<std::size_t, std::size_t>> default_block;
        std::string_view value = m.group(2);
        std::size_t brace = m.end() - 1, at = m.start();
        for (;;) {
            const std::size_t end = close_of(shadow, brace);
            if (end == kNone) {
                cases.clear();
                break;
            }
            cases.push_back({value, brace, end});
            at = end + 1;
            if (const auto again = else_if_at(shadow, at, v)) {
                value = again->value;
                brace = again->open;
                continue;
            }
            if (const auto last = else_at(shadow, at)) {
                const std::size_t last_end = close_of(shadow, *last);
                if (last_end == kNone) {
                    cases.clear();
                    break;
                }
                default_block = {*last, last_end};
                at = last_end + 1;
            }
            break;
        }
        if (cases.size() < 2) return true;
        std::set<std::uint32_t> distinct;
        for (const SwitchCase& c : cases) {
            const auto bits = constant_bits(c.value);
            if (!bits) return true;
            distinct.insert(*bits);
        }
        if (distinct.size() != cases.size()) return true;
        if (contains_word(shadow.substr(m.start(), at - m.start()), "break")) return true;
        std::vector<std::string> lines{std::format("switch ({}) {{", v)};
        for (const SwitchCase& c : cases)
            lines.push_back(std::format("case {}:\n{{{}}}\nbreak;", c.value, body.substr(c.open + 1, c.close - c.open - 1)));
        if (default_block)
            lines.push_back(std::format("default:\n{{{}}}", body.substr(default_block->first + 1, default_block->second - default_block->first - 1)));
        lines.push_back("}");
        out.push_back({m.start(), at, text::join(lines, "\n")});
        return true;
    });
    return out;
}

std::optional<std::string> returned_type(std::string_view head) {
    std::string_view heading_line;
    for (const std::string_view line : text::split(head, '\n')) {
        if (contains(line, '(')) heading_line = line;
    }
    if (heading_line.empty()) return std::nullopt;
    std::vector<std::string_view> words;
    for (const std::string_view w : text::split_ws(heading_line.substr(0, heading_line.find('(')))) {
        if (!w.starts_with("__")) words.push_back(w);
    }
    if (words.size() < 2) return std::nullopt;
    return std::string(words[words.size() - 2]);
}

std::optional<std::string> returns(std::string_view head, std::span<const std::string> decls, std::string_view body,
                                   std::string_view shadow) {
    std::string_view trimmed = text::rtrim(shadow);
    while (trimmed.ends_with('}')) trimmed.remove_suffix(1);
    const auto top = statements(shadow, 0, trimmed.size());
    if (top.size() < 2) return std::nullopt;
    // the last statement is `return NAME;`
    const std::string_view last = shadow.substr(top.back().begin, top.back().end - top.back().begin);
    if (!last.starts_with("return ")) return std::nullopt;
    const std::size_t name_end = name_at(last, 7);
    if (name_end == kNone || name_end >= last.size() || last[name_end] != ';' || !text::trim(last.substr(name_end + 1)).empty())
        return std::nullopt;
    const std::string_view r = last.substr(7, name_end - 7);

    std::optional<std::string_view> own;
    for (const std::string& d : decls) {
        for (const std::string_view line : text::split(d, '\n')) {   // (a declaration may hold several lines)
            if ((own = declared_type_of(line, r))) break;
        }
        if (own) break;
    }
    if (!own) own = parameter_type_of(head, r);
    const auto returned = returned_type(head);
    if (!own || !returned || *own != *returned) return std::nullopt;

    std::vector<Rewrite> edits;
    tail_edits(body, shadow, top[top.size() - 2].begin, top[top.size() - 2].end, r, edits);
    if (edits.empty()) return std::nullopt;
    std::ranges::sort(edits, [](const Rewrite& a, const Rewrite& b) { return std::tie(a.begin, a.end, a.text) > std::tie(b.begin, b.end, b.text); });
    std::string result(body);
    for (const Rewrite& e : edits) result.replace(e.begin, e.end - e.begin, e.text);
    return result;
}

std::vector<Rewrite> folds(std::span<const std::string> decls, std::string_view body, std::string_view shadow) {
    // the names declared `int4 name;` and nothing more
    std::set<std::string_view> plain;
    for (const std::string& d : decls) {
        std::string_view s = text::ltrim(d);
        if (!s.starts_with("int4 ")) continue;
        const std::size_t end = name_at(s, 5);
        if (end != kNone && end < s.size() && s[end] == ';' && text::trim(s.substr(end + 1)).empty()) plain.insert(s.substr(5, end - 5));
    }
    static const Regex assigned_then_tested(
        R"(^([ \t]*)([A-Za-z_]\w*) = ([A-Za-z_]\w*\([^;\n]*\));\n[ \t]*if \(\2 (==|!=|<=|>=|<|>) (-?0x[0-9a-fA-F]+|-?\d+)\) \{)",
        RegexFlags::Multiline);
    std::vector<Rewrite> out;
    assigned_then_tested.for_each_match(shadow, [&](const RegexMatch& m) {
        const std::string_view t = m.group(2);
        if (!contains(plain, t) || count_word(shadow, t) != 2) return true;
        const std::size_t call_begin = m.start(3), call_end = m.end(3);
        const std::size_t paren = shadow.find('(', call_begin);
        if (close_of(shadow, paren, '(', ')') != call_end - 1) return true;   // more than the one call on that line
        out.push_back({m.start(), m.end(), std::format("{}if ({} {} {}) {{", m.group(1), body.substr(call_begin, call_end - call_begin), m.group(4), m.group(5))});
        return true;
    });
    return out;
}

}  // namespace cb::verify::authored::shape
