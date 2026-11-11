#include "cb/exact/draft/text.hpp"

#include <algorithm>
#include <array>
#include <cctype>
#include <type_traits>

#include "cb/core/error.hpp"
#include "cb/core/text.hpp"

namespace cb::exact::draft::text {
namespace {

namespace txt = cb::text;

bool is_word(char c) { return std::isalnum(static_cast<unsigned char>(c)) != 0 || c == '_'; }
bool is_digit(char c) { return c >= '0' && c <= '9'; }
bool blank(std::string_view l) { return txt::trim(l).empty(); }
std::size_t indent_of(std::string_view l) { return l.size() - txt::ltrim(l).size(); }
bool has_newline(std::string_view s) { return s.find('\n') != std::string_view::npos; }

std::size_t count_char(std::string_view s, char c) { return static_cast<std::size_t>(std::count(s.begin(), s.end(), c)); }

// Every character of s is a word character or one of `extra`, and there is at least one.
bool all_of_class(std::string_view s, std::string_view extra) {
    return !s.empty() && std::ranges::all_of(s, [&](char c) { return is_word(c) || extra.find(c) != std::string_view::npos; });
}

// The text between a leading "(" and the given ending, when the whole of c is "(" ... ending (and the middle has no
// newline: the original's `.` did not match one).
std::optional<std::string_view> inside_parens(std::string_view c, std::string_view ending) {
    if (c.size() < 1 + ending.size() || c.front() != '(' || !c.ends_with(ending)) return std::nullopt;
    const std::string_view middle = c.substr(1, c.size() - 1 - ending.size());
    if (has_newline(middle)) return std::nullopt;
    return middle;
}

// Does `s` hold the word `w` (a whole run of word characters)?
bool has_word(std::string_view s, std::string_view w) {
    for (std::size_t i = 0; i < s.size();) {
        if (!is_word(s[i])) {
            ++i;
            continue;
        }
        std::size_t end = i;
        while (end < s.size() && is_word(s[end])) ++end;
        if (s.substr(i, end - i) == w) return true;
        i = end;
    }
    return false;
}

bool has_any_word(std::string_view s, std::span<const std::string_view> words) {
    return std::ranges::any_of(words, [&](std::string_view w) { return has_word(s, w); });
}

// `word` at the start of s, as a whole word.
bool starts_with_word(std::string_view s, std::string_view word) {
    return s.starts_with(word) && (s.size() == word.size() || !is_word(s[word.size()]));
}

}  // namespace

// `goto L_xxx;` at the start of the line (after any indent): the label. With `whole`, nothing may follow the
// semicolon.
std::optional<std::string_view> goto_target(std::string_view line, bool whole) {
    std::string_view s = txt::ltrim(line);
    if (!s.starts_with("goto L_")) return std::nullopt;
    std::size_t end = 7;
    while (end < s.size() && is_word(s[end])) ++end;
    if (end == 7 || end >= s.size() || s[end] != ';') return std::nullopt;
    if (whole && end + 1 != s.size()) return std::nullopt;
    return s.substr(5, end - 5);
}

// `L_xxx:` alone on the line (after any indent).
bool label_line(std::string_view line) {
    std::string_view s = txt::ltrim(line);
    if (!s.starts_with("L_") || s.size() < 4 || s.back() != ':') return false;
    return std::ranges::all_of(s.substr(2, s.size() - 3), is_word);
}

bool is_register(std::string_view word) { return std::ranges::find(kRegisters, word) != kRegisters.end(); }
bool is_word_register(std::string_view word) { return std::ranges::find(kWordRegisters, word) != kWordRegisters.end(); }

std::uint32_t size_of_type(std::string_view w, std::uint32_t otherwise) {
    if (w == "char") return 1;
    if (w == "int" || w == "unsigned" || w == "short") return 2;
    if (w == "long") return 4;
    return otherwise;
}

// ---- tests --------------------------------------------------------------------------------------------------

std::string strip_parens(std::string_view c) {
    while (c.starts_with("((") && c.ends_with("))")) {
        const std::string_view middle = c.substr(1, c.size() - 2);
        int depth = 0;
        for (char ch : middle) {
            depth += (ch == '(') - (ch == ')');
            if (depth < 0) return std::string(c);
        }
        if (depth != 0) return std::string(c);
        c = middle;
    }
    return std::string(c);
}

std::string truth(std::string_view c_in) {
    const std::string stripped = strip_parens(c_in);
    const std::string_view c = stripped;
    if (auto g = inside_parens(c, " != 0)")) return std::string(*g);
    if (auto g = inside_parens(c, " == 0)")) {
        // a plain operand is negated bare, anything else in parentheses
        return all_of_class(*g, ".->[]()") ? "!" + std::string(*g) : "!(" + std::string(*g) + ")";
    }
    return stripped;
}

bool balanced(std::string_view s) {
    int depth = 0;
    for (char ch : s) {
        depth += (ch == '(') - (ch == ')');
        if (depth < 0) return false;
    }
    return depth == 0;
}

std::vector<std::string> top_split(std::string_view c, std::string_view op) {
    const std::string sep = " " + std::string(op) + " ";
    std::vector<std::string> out;
    std::string cur;
    int depth = 0;
    for (std::size_t k = 0; k < c.size();) {
        depth += (c[k] == '(') - (c[k] == ')');
        if (depth == 0 && c.substr(k).starts_with(sep)) {
            out.push_back(std::move(cur));
            cur.clear();
            k += sep.size();
            continue;
        }
        cur += c[k++];
    }
    out.push_back(std::move(cur));
    return out;
}

std::vector<std::string> top_commas(std::string_view s) {
    std::vector<std::string> out;
    std::string cur;
    int depth = 0;
    for (char ch : s) {
        if (ch == '(' || ch == '[') ++depth;
        else if (ch == ')' || ch == ']') --depth;
        if (ch == ',' && depth == 0) {
            out.push_back(std::move(cur));
            cur.clear();
        } else {
            cur += ch;
        }
    }
    out.push_back(std::move(cur));
    return out;
}

namespace {

struct Comparison {
    std::string_view left, op, right;
};

// "(" LEFT " OP " RIGHT ")": the first place a spaced comparison operator is found (LEFT as short as it can be).
std::optional<Comparison> comparison(std::string_view c) {
    static constexpr std::array<std::string_view, 6> kOps = {"==", "!=", "<=", ">=", "<", ">"};
    if (c.size() < 2 || c.front() != '(' || c.back() != ')') return std::nullopt;
    for (std::size_t p = 1; p < c.size(); ++p) {
        if (c[p] == '\n') return std::nullopt;   // (LEFT may not hold one, and only grows)
        if (c[p] != ' ') continue;
        for (std::string_view op : kOps) {
            const std::size_t after = p + 1 + op.size();
            if (c.substr(p + 1).starts_with(op) && after < c.size() && c[after] == ' ' && after + 1 <= c.size() - 1) {
                const std::string_view right = c.substr(after + 1, c.size() - 1 - (after + 1));
                if (has_newline(right)) break;
                return Comparison{c.substr(1, p - 1), op, right};
            }
        }
    }
    return std::nullopt;
}

std::string_view flipped(std::string_view op) {
    if (op == "==") return "!=";
    if (op == "!=") return "==";
    if (op == "<") return ">=";
    if (op == ">=") return "<";
    if (op == ">") return "<=";
    return ">";   // "<="
}

}  // namespace

std::string negate(std::string_view c_in) {
    const std::string c = strip_parens(c_in);
    std::string_view inner = c;
    if (c.size() >= 2 && c.front() == '(' && c.back() == ')' && balanced(std::string_view(c).substr(1, c.size() - 2)))
        inner = std::string_view(c).substr(1, c.size() - 2);

    if (inner.size() >= 3 && inner.starts_with("!(") && inner.back() == ')') {   // (!(X): X)
        const std::string_view g = inner.substr(2, inner.size() - 3);
        if (!has_newline(g) && balanced(g)) return "(" + std::string(g) + ")";
    }
    if (inner.size() >= 2 && inner.front() == '!' && all_of_class(inner.substr(1), "->[].")) return std::string(inner.substr(1));   // (!x: x)
    for (const auto& [op, other] : {std::pair<std::string_view, std::string_view>{"&&", "||"}, {"||", "&&"}}) {   // (De Morgan, a level down)
        const std::vector<std::string> parts = top_split(inner, op);
        if (parts.size() > 1) {
            std::vector<std::string> negated;
            for (const std::string& p : parts) negated.push_back(truth(negate("(" + p + ")")));
            return "(" + txt::join(negated, " " + std::string(other) + " ") + ")";
        }
    }
    if (const auto m = comparison(c)) {
        if (m->right.find('(') == std::string_view::npos && count_char(m->left, '(') == count_char(m->left, ')') &&
            (m->right != "0" || (m->op != "==" && m->op != "!=")))   // (x == 0: !x below; a relation to 0 flips)
            return "(" + std::string(m->left) + " " + std::string(flipped(m->op)) + " " + std::string(m->right) + ")";
    }
    if (auto g = inside_parens(c, " == 0)")) return std::string(*g);
    if (auto g = inside_parens(c, " != 0)")) return "!" + std::string(*g);
    if (all_of_class(inner, "->[].")) return "!" + std::string(inner);
    return "!" + c;
}

std::string join_tests(std::string_view op, std::string_view a, std::string_view b) {
    const auto wrap = [&](std::string_view x) {
        const std::string probe = x.starts_with("((") ? strip_parens(x) : std::string(x);
        return op == "&&" && top_split(probe, "||").size() > 1 ? "(" + std::string(x) + ")" : std::string(x);
    };
    return wrap(a) + " " + std::string(op) + " " + wrap(b);
}

// ---- blocks -------------------------------------------------------------------------------------------------

std::optional<std::size_t> opener(std::span<const std::string> lines, std::size_t k) {
    CB_ASSERT(k < lines.size(), "opener: line out of range");
    const std::size_t ind = indent_of(lines[k]);
    for (std::size_t j = k; j-- > 0;)
        if (!blank(lines[j]) && indent_of(lines[j]) == ind) return j;
    return std::nullopt;
}

std::vector<std::size_t> openers(std::span<const std::string> lines, std::size_t k) {
    CB_ASSERT(k < lines.size(), "openers: line out of range");
    std::vector<std::size_t> out;
    std::size_t want = indent_of(lines[k]);
    for (std::size_t j = k; j-- > 0;) {
        if (blank(lines[j])) continue;
        const std::size_t d = indent_of(lines[j]);
        if (d < want && txt::rtrim(lines[j]).ends_with('{')) {
            out.push_back(j);
            want = d;
        }
    }
    return out;
}

std::optional<std::size_t> block_end(std::span<const std::string> lines, std::size_t k) {
    CB_ASSERT(k < lines.size(), "block_end: line out of range");
    const std::size_t ind = indent_of(lines[k]);
    for (std::size_t j = k + 1; j < lines.size(); ++j)
        if (!blank(lines[j]) && indent_of(lines[j]) <= ind) return j;
    return std::nullopt;
}

bool in_loop(std::span<const std::string> lines, std::ptrdiff_t lo, std::size_t j, std::size_t ind) {
    CB_ASSERT(j < lines.size(), "in_loop: line out of range");
    std::size_t want = indent_of(lines[j]);
    for (std::ptrdiff_t x = static_cast<std::ptrdiff_t>(j) - 1; x > lo && x >= 0; --x) {
        const std::string& l = lines[static_cast<std::size_t>(x)];
        if (blank(l)) continue;
        const std::size_t d = indent_of(l);
        if (d < want && txt::rtrim(l).ends_with('{')) {
            if (d <= ind) return false;
            const std::string_view s = txt::ltrim(l);
            if (starts_with_word(s, "while") || starts_with_word(s, "for") || starts_with_word(s, "do") ||
                starts_with_word(s, "switch") || txt::trim(l) == "do {")
                return true;
            want = d;
        }
    }
    return false;
}

namespace {

// `<indent>KEYWORD (COND) {` alone on the line: the indent and COND. `lead` is what precedes the parenthesis.
struct Header {
    std::string_view indent, cond;
};

std::optional<Header> header_line(std::string_view line, std::string_view lead) {
    const std::string_view s = txt::ltrim(line);
    if (!s.starts_with(lead) || !s.ends_with(") {") || s.size() < lead.size() + 3) return std::nullopt;
    const std::string_view cond = s.substr(lead.size(), s.size() - lead.size() - 3);
    if (has_newline(cond)) return std::nullopt;
    return Header{line.substr(0, line.size() - s.size()), cond};
}

}  // namespace

void else_ifs(std::vector<std::string>& lines) {
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto head = header_line(lines[k], "} else if (");
        if (!head) {
            ++k;
            continue;
        }
        const std::string ind(head->indent);
        const std::string cond(head->cond);
        std::size_t e = k + 1;   // (the chain's end: the `}` at the if's indent that closes the whole chain)
        while (e < lines.size() && lines[e] != ind + "}") ++e;
        if (e >= lines.size()) return;
        std::vector<std::string> rewritten{ind + "} else {", ind + "    if (" + cond + ") {"};
        for (std::size_t i = k + 1; i < e; ++i) rewritten.push_back(blank(lines[i]) ? lines[i] : "    " + lines[i]);
        rewritten.push_back(ind + "    }");
        rewritten.push_back(ind + "}");
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(e) + 1);
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), rewritten.begin(), rewritten.end());
        ++k;
    }
}

std::optional<IfParts> if_parts(std::span<const std::string> lines, std::size_t k) {
    CB_ASSERT(k < lines.size(), "if_parts: line out of range");
    const auto head = header_line(lines[k], "if (");
    if (!head) return std::nullopt;
    const std::string ind(head->indent);
    // (the arm's lines, up to the closing brace at the if's own indent; a line not deeper than the if ends the hope)
    const auto arm_end = [&](std::size_t from) -> std::optional<std::size_t> {
        std::size_t j = from;
        while (j < lines.size() && !lines[j].starts_with(ind + "}")) {
            if (!blank(lines[j]) && !lines[j].starts_with(ind + " ")) return std::nullopt;
            ++j;
        }
        if (j >= lines.size()) return std::nullopt;
        return j;
    };
    const auto j = arm_end(k + 1);
    if (!j) return std::nullopt;
    IfParts out;
    out.cond = std::string(head->cond);
    out.then_arm.assign(lines.begin() + static_cast<std::ptrdiff_t>(k) + 1, lines.begin() + static_cast<std::ptrdiff_t>(*j));
    if (lines[*j] == ind + "}") {
        out.end = *j + 1;
        return out;
    }
    if (lines[*j] != ind + "} else {") return std::nullopt;
    const auto e = arm_end(*j + 1);
    if (!e || lines[*e] != ind + "}") return std::nullopt;
    out.else_arm = std::vector<std::string>(lines.begin() + static_cast<std::ptrdiff_t>(*j) + 1, lines.begin() + static_cast<std::ptrdiff_t>(*e));
    out.end = *e + 1;
    return out;
}

std::vector<std::string> unlabelled(std::span<const std::string> arm, const std::optional<std::vector<std::string>>& other) {
    std::optional<std::string> wanted;   // the label the other arm's goto went to
    if (other) {
        const std::optional<std::string_view> g = other->empty() ? std::nullopt : goto_target(other->back(), true);
        if (!g) return {arm.begin(), arm.end()};
        wanted = std::string(*g);
    }
    std::vector<std::string> out;
    for (const std::string& l : arm)
        if (!label_line(l) || (wanted && txt::trim(l) != *wanted + ":")) out.push_back(l);
    return out;
}

bool same_arm(std::span<const std::string> x, std::span<const std::string> y) {
    if (std::ranges::equal(x, y)) return true;
    if (x.empty()) return false;
    const auto lab = goto_target(x.back(), true);
    if (!lab) return false;
    const std::string wanted_line = std::string(*lab) + ":";
    const auto at = std::ranges::find_if(y, [&](const std::string& l) { return txt::trim(l) == wanted_line; });
    if (at == y.end()) return false;
    const std::size_t n = static_cast<std::size_t>(at - y.begin());
    const std::span<const std::string> head = x.first(x.size() - 1);
    return std::ranges::equal(head, y.first(n)) && unlabelled(head) == std::vector<std::string>(head.begin(), head.end());
}

// ---- registers --------------------------------------------------------------------------------------------

namespace {

// `REG = ` at the line's start (after any indent), the value not naming REG again before the statement ends: the
// register is given a new value, not updated.
bool gives_new_value(std::string_view line, std::string_view reg) {
    const std::string_view s = txt::ltrim(line);
    if (!s.starts_with(reg) || s.substr(reg.size()).substr(0, 3) != " = ") return false;
    std::string_view value = s.substr(reg.size() + 3);
    value = value.substr(0, value.find(';'));
    return !has_word(value, reg);
}

// The text after the line's first '=' (the value a register is given).
std::string_view after_assign(std::string_view line) { return line.substr(line.find('=') + 1); }

// `} else {` (or `} else if ... {`): the line leaves the arm it is in for the one beside it.
bool is_else_arm(std::string_view line) {
    const std::string_view s = txt::ltrim(line);
    return starts_with_word(s, "} else") ? txt::rtrim(s).ends_with('{') : false;
}

bool is_leave(std::string_view line) {   // break; or continue;
    const std::string_view s = txt::ltrim(line);
    return s.starts_with("break;") || s.starts_with("continue;");
}

bool is_return(std::string_view line) { return starts_with_word(txt::ltrim(line), "return"); }

template <class Lines>
bool live_after_impl(const Lines& lines, std::size_t first, std::string_view reg, std::span<const std::string_view> reads_in,
                     LiveOptions options) {
    const std::string_view just_reg[] = {reg};
    const std::span<const std::string_view> reads = reads_in.empty() ? std::span<const std::string_view>(just_reg) : reads_in;
    const auto line_at = [&](std::size_t i) -> const std::string* {
        if constexpr (std::is_same_v<Lines, std::span<const std::string>>) return &lines[i];
        else return lines[i] ? &*lines[i] : nullptr;
    };
    std::vector<bool> stack;          // the blocks open since the start: true a conditional one
    std::optional<long> skip;         // the depth of an arm beside the start's, not on its path
    std::optional<std::size_t> killed;   // the depth of a block that gave it a new value
    for (std::size_t i = first; i < lines.size(); ++i) {
        const std::string* lp = line_at(i);
        if (lp == nullptr) continue;
        const std::string& l = *lp;
        const long opens = static_cast<long>(count_char(l, '{')), closes = static_cast<long>(count_char(l, '}'));
        if (skip) {
            *skip += opens - closes;
            if (*skip < 0) skip.reset();   // (the other arm closed: past the whole if)
            continue;
        }
        if (is_else_arm(l) && stack.empty()) {   // (leaving the start's arm into the other: skipped)
            skip = 0;
            continue;
        }
        if (stack.empty() && is_return(l) && !has_any_word(l, reads)) return false;   // (the path ends)
        if (options.follow_gotos && !killed) {
            if (const auto label = goto_target(l, false)) {   // (a jump elsewhere: read there before it is given?)
                const std::string target = std::string(*label) + ":";
                std::size_t at = lines.size();
                for (std::size_t j = 0; j < lines.size(); ++j)
                    if (const std::string* x = line_at(j); x != nullptr && txt::trim(*x) == target) {
                        at = j;
                        break;
                    }
                if (at < lines.size()) {
                    for (std::size_t j = at + 1; j < std::min<std::size_t>(at + 40, lines.size()); ++j) {
                        const std::string* x = line_at(j);
                        if (x == nullptr) continue;
                        if (gives_new_value(*x, reg)) break;   // (given there first: not this value)
                        if (has_any_word(*x, reads)) return true;
                        const std::string_view xs = txt::ltrim(*x);
                        if (starts_with_word(xs, "return") || starts_with_word(xs, "goto") || starts_with_word(xs, "break") ||
                            starts_with_word(xs, "continue"))
                            break;
                    }
                }
            }
        }
        if (stack.empty() && is_leave(l)) {   // (to the end of the block it leaves: the rest of it - a switch's other cases - not on its path)
            skip = 0;
            continue;
        }
        if (killed && stack.size() < *killed) killed.reset();   // (out of the block that gave it a new value)
        if (gives_new_value(l, reg)) {
            const bool reads_it = has_any_word(after_assign(l), reads);
            if (std::none_of(stack.begin(), stack.end(), [](bool b) { return b; })) return reads_it;
            if (!killed && reads_it) return true;   // (given a value on some paths: read only in it)
            killed = killed ? std::min(*killed, stack.size()) : stack.size();
        } else if (has_any_word(l, reads) && !killed) {
            return true;
        }
        for (long c = 0; c < closes; ++c)
            if (!stack.empty()) stack.pop_back();   // (out of the start's block: what follows is on its path)
        stack.insert(stack.end(), static_cast<std::size_t>(opens), txt::trim(l) != "{");
    }
    return false;
}

}  // namespace

bool live_after(std::span<const std::string> lines, std::size_t first, std::string_view reg,
                std::span<const std::string_view> reads, LiveOptions options) {
    return live_after_impl(lines, first, reg, reads, options);
}

bool live_after(std::span<const std::optional<std::string>> lines, std::size_t first, std::string_view reg,
                std::span<const std::string_view> reads, LiveOptions options) {
    return live_after_impl(lines, first, reg, reads, options);
}

bool plain_store(std::string_view line, std::string_view reg) {
    // <indent> LHS " = " RHS ";" <spaces>: LHS up to the first '=' (which must have a space each side)
    const std::string_view s = line;
    const std::size_t semi = s.find(';');
    if (semi == std::string_view::npos || !blank(s.substr(semi + 1))) return false;
    const std::size_t eq = s.find('=');
    if (eq == std::string_view::npos || eq > semi || eq < 2 || s[eq - 1] != ' ' || eq + 1 >= semi || s[eq + 1] != ' ' || eq + 2 >= semi)
        return false;
    // (the left side may be nothing but blanks: the indent is taken off it, but never all of it)
    const std::string_view lhs = txt::ltrim(s.substr(0, eq - 1)), rhs = s.substr(eq + 2, semi - (eq + 2));
    if (has_word(lhs, reg)) return false;
    if (rhs.find("++") != std::string_view::npos || rhs.find("--") != std::string_view::npos) return false;
    for (std::size_t i = 1; i + 1 < rhs.size(); ++i)   // an assignment inside the value: = after a non-comparison character
        if (rhs[i] == '=' && std::string_view("=!<>").find(rhs[i - 1]) == std::string_view::npos && rhs[i + 1] != '=') return false;
    const std::string both = std::string(rhs) + std::string(lhs);   // (a call: a word, then a parenthesis)
    for (std::size_t i = 0; i < both.size(); ++i) {
        if (both[i] != '(') continue;
        std::size_t j = i;
        while (j > 0 && txt::is_space(both[j - 1])) --j;
        if (j > 0 && is_word(both[j - 1])) return false;
    }
    return true;
}

// ---- the function's text ----------------------------------------------------------------------------------

namespace {

struct Declaration {
    std::size_t start = 0, end = 0;    // the text the declaration's line takes, without its newline
    std::string indent, register_part;
    std::vector<std::string> others;   // the other names declared with it
};

// A line `<indent>[register ]int a, b, X, c;` declaring x among plain names: the first such line of the text.
std::optional<Declaration> find_declaration(std::string_view t, std::string_view x) {
    for (std::size_t p = 0; p <= t.size();) {
        const std::size_t eol = std::min(t.find('\n', p), t.size());
        std::string_view line = t.substr(p, eol - p);
        std::size_t i = 0;
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
        Declaration d;
        d.indent = std::string(line.substr(0, i));
        if (line.substr(i).starts_with("register")) {
            std::size_t j = i + 8;
            while (j < line.size() && (line[j] == ' ' || line[j] == '\t')) ++j;
            if (j > i + 8 && line.substr(j).starts_with("int ")) {
                d.register_part = std::string(line.substr(i, j - i));
                i = j;
            }
        }
        if (line.substr(i).starts_with("int ") && line.ends_with(';')) {
            const std::string_view names = line.substr(i + 4, line.size() - (i + 4) - 1);
            std::vector<std::string_view> items = txt::split(names, std::string_view(", "));
            const bool plain = !items.empty() && std::ranges::all_of(items, [](std::string_view n) { return all_of_class(n, ""); });
            const auto last = std::ranges::find(items.rbegin(), items.rend(), x);   // (the longest run of names before X wins)
            if (plain && last != items.rend()) {
                const std::size_t at = static_cast<std::size_t>(items.rend() - last) - 1;
                for (std::size_t n = 0; n < items.size(); ++n)
                    if (n != at) d.others.emplace_back(items[n]);
                d.start = p;
                d.end = eol;
                return d;
            }
        }
        if (eol >= t.size()) break;
        p = eol + 1;
    }
    return std::nullopt;
}

// Where `pat` (a literal) occurs in t as the start of a match at or after `from`.
std::optional<std::int64_t> parse_number(std::string_view digits) { return txt::parse_int<std::int64_t>(digits); }

// The digits at the start of s: their count.
std::size_t digit_run(std::string_view s) {
    std::size_t n = 0;
    while (n < s.size() && is_digit(s[n])) ++n;
    return n;
}

// ((int *)((char *)X + N)) with N even -> (X + N/2)
std::string words_for_byte_offsets(std::string_view t, std::string_view x) {
    const std::string prefix = "((int *)((char *)" + std::string(x) + " + ";
    std::string out;
    for (std::size_t i = 0; i < t.size();) {
        if (t.substr(i).starts_with(prefix)) {
            const std::size_t digits = i + prefix.size(), n = digit_run(t.substr(digits));
            if (n != 0 && t.substr(digits + n).starts_with("))")) {
                const auto v = parse_number(t.substr(digits, n));
                const std::size_t end = digits + n + 2;
                if (v && *v % 2 == 0) out += "(" + std::string(x) + " + " + std::to_string(*v / 2) + ")";
                else out += t.substr(i, end - i);
                i = end;
                continue;
            }
        }
        out += t[i++];
    }
    return out;
}

// X[(E << 1) + N] -> X[E + N/2] (N even) and X[(E << 1)] -> X[E]: an index the code doubles. E is the shortest text
// (of one line) that leaves what follows matching.
std::string doubled_indexes(std::string_view t, std::string_view x, bool with_offset) {
    const std::string open = std::string(x) + "[(";
    std::string out;
    for (std::size_t i = 0; i < t.size();) {
        if (t.substr(i).starts_with(open) && (i == 0 || !is_word(t[i - 1]))) {
            const std::size_t inner = i + open.size();
            bool replaced = false;
            for (std::size_t e = inner + 1; e <= t.size() && t[e - 1] != '\n'; ++e) {
                const std::string_view rest = t.substr(e);
                if (with_offset) {
                    if (!rest.starts_with(" << 1) + ")) continue;
                    const std::size_t digits = e + 9, n = digit_run(t.substr(digits));
                    if (n == 0 || !t.substr(digits + n).starts_with("]")) continue;
                    const auto v = parse_number(t.substr(digits, n));
                    const std::size_t end = digits + n + 1;
                    if (v && *v % 2 == 0)
                        out += std::string(x) + "[" + std::string(t.substr(inner, e - inner)) + " + " + std::to_string(*v / 2) + "]";
                    else out += t.substr(i, end - i);
                    i = end;
                } else {
                    if (!rest.starts_with(" << 1)]")) continue;
                    out += std::string(x) + "[" + std::string(t.substr(inner, e - inner)) + "]";
                    i = e + 7;
                }
                replaced = true;
                break;
            }
            if (replaced) continue;
        }
        out += t[i++];
    }
    return out;
}

}  // namespace

std::optional<std::string> declare_pointer(std::string_view src, std::string_view x, std::string_view type) {
    const BodySplit parts = split_body(src);
    for (int which = 0; which < 2; ++which) {
        const std::string& t = which == 0 ? parts.head : parts.body;
        const auto d = find_declaration(t, x);
        if (!d) continue;
        std::string replacement;
        if (!d->others.empty()) replacement = d->indent + d->register_part + "int " + txt::join(d->others, ", ") + ";\n";
        replacement += d->indent + d->register_part + std::string(type) + " *" + std::string(x) + ";";
        const std::string changed_part = t.substr(0, d->start) + replacement + t.substr(d->end);
        std::string out = which == 0 ? changed_part + parts.brace + parts.body : parts.head + parts.brace + changed_part;
        if (type == "int") {   // (its byte offsets, now in words: x + N/2)
            out = words_for_byte_offsets(out, x);
            out = doubled_indexes(out, x, true);
            out = doubled_indexes(out, x, false);
        }
        return out;
    }
    return std::nullopt;
}

BodySplit split_body(std::string_view source) {
    // A line of "{" alone (and blanks): the body's own brace, not a struct's declared before it.
    for (std::size_t p = 0; p < source.size();) {
        const std::size_t eol = std::min(source.find('\n', p), source.size());
        const std::string_view line = source.substr(p, eol - p);
        if (!line.empty() && line.front() == '{' && blank(line.substr(1)))
            return {std::string(source.substr(0, p)), "{", std::string(source.substr(p + 1))};
        p = eol + 1;
    }
    const std::size_t brace = source.find('{');
    if (brace == std::string_view::npos) return {std::string(source), "", ""};
    return {std::string(source.substr(0, brace)), "{", std::string(source.substr(brace + 1))};
}

}  // namespace cb::exact::draft::text
