// The passes that fold what the model leaves of the machine's registers back into the statements that use them: a
// scratch register copied from a variable, a call's value held in ax, a temporary read once, a register returned right
// after it was given.
#include <algorithm>
#include <map>
#include <set>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/shapes.hpp"
#include "cb/exact/draft/shapes_scan.hpp"

namespace cb::exact::draft::shapes {
namespace {

namespace dt = cb::exact::draft::text;
namespace txt = cb::text;
using namespace scan;
using cb::Regex;
using cb::RegexFlags;
using cb::RegexMatch;
using std::string;
using std::string_view;

string str(string_view s) { return string(s); }

bool is_wide(string_view reg) { return reg == "ax" || reg == "bx" || reg == "cx" || reg == "dx"; }

// The words a register is read as: itself and, for ax..dx, its low byte (`(?:ax|al)`).
using Words = std::vector<string>;
Words words_of(string_view reg) {
    Words w{str(reg)};
    if (is_wide(reg)) w.push_back(cat(reg.substr(0, 1), "l"));
    return w;
}
std::vector<string_view> as_views(const Words& w) { return std::vector<string_view>(w.begin(), w.end()); }

std::size_t count_any(string_view s, const Words& words) {
    std::size_t n = 0;
    for (const string& w : words) n += count_word(s, w);
    return n;
}
bool has_any(string_view s, const Words& words) { return count_any(s, words) > 0; }
string remove_all(string_view s, const Words& words) {
    string out = str(s);
    for (const string& w : words) out = replace_word_all(out, w, "");
    return out;
}
string replace_all_of(string_view s, const Words& words, string_view with) {
    string out = str(s);
    for (const string& w : words) out = replace_word_all(out, w, with);
    return out;
}
// The first occurrence of any of the words, replaced.
string replace_first_of(string_view s, const Words& words, string_view with) {
    std::optional<std::size_t> best;
    std::size_t length = 0;
    for (const string& w : words) {
        const auto at = find_word(s, w);
        if (at && (!best || *at < *best)) {
            best = at;
            length = w.size();
        }
    }
    if (!best) return str(s);
    return cat(s.substr(0, *best), with, s.substr(*best + length));
}
bool assigned_any(string_view s, const Words& words) {
    return std::ranges::any_of(words, [&](const string& w) { return is_assigned(s, w); });
}

// `\w\(`: a name's last byte, then a parenthesis.
bool has_word_paren(string_view s) {
    for (std::size_t i = 1; i < s.size(); ++i)
        if (s[i] == '(' && is_word_char(s[i - 1])) return true;
    return false;
}

// Does the line, past its blanks, begin with one of the names and ` = `? (`\s*(?:a|b) = `)
bool begins_assigning(string_view line, const Words& names) {
    const string_view s = txt::ltrim(line);
    return std::ranges::any_of(names, [&](const string& n) { return s.starts_with(n) && s.substr(n.size()).starts_with(" = "); });
}

bool all_word(string_view s) { return !s.empty() && std::ranges::all_of(s, is_word_char); }

bool starts_statement_end(string_view line) {   // `\s*(?:\}|L_\w+:|else\b)` at the start
    static const Regex re(R"re(^\s*(?:\}|L_\w+:|else\b))re");
    return re.match(line).has_value();
}

}  // namespace

// ---- a scratch register copied from a variable -------------------------------------------------------------------

string copy_props(string_view text) {
    static const Regex copy(R"re(^(\s*)(ax|bx|cx|dx) = ([A-Za-z_]\w*);\s*$)re");
    static const Regex label(R"re(^\s*L_\w+:)re");
    static const Regex call(R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\s*\()re");
    const Lines lines = split_lines(text);
    MaybeLines out = maybe(lines);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = copy.match(lines[k]);
        if (!m) continue;
        const string r = str(m->group(2)), x = str(m->group(3));
        if (is_wide(x)) continue;   // (si, di: register variables - copied as variables)
        const string copies_again = cat(r, " = ", x, ";");
        const string r_or_halves[] = {r, cat(r.substr(0, 1), "l"), cat(r.substr(0, 1), "h")};
        int depth = 0;
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            if (!out[j]) continue;   // (a copy dropped before: nothing)
            const string y = *out[j];
            depth += static_cast<int>(std::ranges::count(y, '{')) - static_cast<int>(std::ranges::count(y, '}'));
            if (depth < 0 || label.match(y)) break;
            if (txt::trim(y) == copies_again) {   // (the same copy again: no change)
                out[j].reset();
                continue;
            }
            const std::size_t reads = count_word(y, r);
            // (or a half of it)
            const bool sets_r = std::ranges::any_of(r_or_halves, [&](const string& n) {
                const string_view s = txt::ltrim(y).substr(0);
                if (!s.starts_with(n)) return false;
                std::size_t q = n.size();
                while (q < s.size() && txt::is_space(s[q])) ++q;
                return q + 1 < s.size() && s[q] == '=' && s[q + 1] != '=';
            });
            if (reads > 0 && !sets_r) out[j] = replace_word_all(y, r, x);
            if (sets_r || is_updated(y, x) || (r == "ax" && call.search(y))) break;
        }
        if (!live_from(out, k + 1, r)) out[k].reset();   // (read no more before it is given again: gone)
    }
    return join_live(out);
}

// ---- a call's value held in a register, stored or read once ----------------------------------------------------

string stored_calls(string_view text) {
    static const Regex with_call(R"re(^(\s*)(ax|bx|cx|dx|si|di) = (.*\w\(.*\).*);\s*$)re");
    static const Regex label_start(R"re(^\s*L_\w+:)re");
    static const Regex any_call(R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\()re");
    static const Regex call_args(
        R"re(\b(?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re");
    // `(\s*)(.+?) = (?:R|Rl);\s*$` for each register: the statement storing it
    static const std::map<string, Regex> stores = [] {
        std::map<string, Regex> m;
        for (const char* reg : {"ax", "bx", "cx", "dx", "si", "di"}) {
            const Words w = words_of(reg);
            m.emplace(reg, Regex(cat(R"re(^(\s*)(.+?) = (?:)re", w.size() > 1 ? cat(w[0], "|", w[1]) : w[0], R"re();\s*$)re")));
        }
        return m;
    }();

    Lines lines = split_lines(text);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = with_call.match(lines[k]);   // (a value with a call in it)
        if (m && k + 1 < lines.size()) {
            const string ind = str(m->group(1)), reg = str(m->group(2)), call = str(m->group(3));
            const Words rd = words_of(reg);   // (or its low byte: a char)
            const auto st = stores.at(reg).match(lines[k + 1]);
            // (its reads there gone - not a value it is given)
            string stripped;
            {
                const string& next_line = lines[k + 1];
                const string_view s = txt::ltrim(next_line);
                const string_view lead = string_view(next_line).substr(0, next_line.size() - s.size());
                if (s.starts_with(reg) && s.substr(reg.size()).starts_with(" = "))
                    stripped = cat(lead, reg, " = ", remove_all(s.substr(reg.size() + 3), rd));
                else
                    stripped = remove_all(next_line, rd);
            }
            Lines modified = lines;
            modified[k + 1] = stripped;
            bool later = !live_from(modified, k + 1, reg, as_views(rd));
            // (read before it is given a value that does not read it, on any path: still wanted)
            if (st && !has_any(st->group(2), rd) && later) {
                out.push_back(cat(st->group(1), st->group(2), " = ", call, ";"));
                k += 2;
                continue;
            }
            if (st && !has_any(st->group(2), rd) && all_word(st->group(2))) {
                const string lv = str(st->group(2));   // (read again after: the variable it was stored in -
                std::size_t j = k + 2;                 //  until it is given another value, a call, a label)
                Lines fixed = lines;
                while (j < fixed.size() && !begins_assigning(fixed[j], rd) && !label_start.match(fixed[j]) && !has_word_paren(fixed[j]) &&
                       !is_assigned(fixed[j], lv)) {
                    fixed[j] = replace_all_of(fixed[j], rd, lv);
                    ++j;
                }
                if (!live_from(fixed, k + 2, reg, as_views(rd))) {
                    lines = fixed;
                    out.push_back(cat(st->group(1), lv, " = ", call, ";"));
                    k += 2;
                    continue;
                }
            }
            const string nxt = lines[k + 1];   // (read once, anywhere in the next statement - which
            if (reg == "si" || reg == "di") later = false;   //  (a register variable's value: kept in it)
            if (later && count_any(nxt, rd) == 1 && !has_word_paren(nxt) && !starts_statement_end(nxt) && !assigned_any(nxt, rd)) {
                out.push_back(replace_first_of(nxt, rd, call));   // makes no call of its own: the order stays)
                k += 2;
                continue;
            }
            std::size_t calls = 0;
            any_call.for_each_match(nxt, [&](const RegexMatch&) {
                ++calls;
                return true;
            });
            const auto ca = call_args.search(nxt);
            string outside;
            if (ca) outside = cat(nxt.substr(0, ca->start()), nxt.substr(ca->end()));
            if (ca) {   // (the statement's own `REG = ` at its start gone)
                const string head = cat(reg, " = ");
                std::size_t q = 0;
                while (q < outside.size() && txt::is_space(outside[q])) ++q;
                if (outside.compare(q, head.size(), head) == 0) outside = outside.substr(q + head.size());
            }
            if (later && calls == 1 && ca && count_any(ca->group(1), rd) == 1 && !has_any(outside, rd) && !starts_statement_end(nxt)) {
                const bool byte = has_word(ca->group(1), cat(reg.substr(0, 1), "l")) && reg != cat(reg.substr(0, 1), "l");   // (an argument of the one
                const string v = byte ? cat("(char)(", call, ")") : call;                                                    //  call there: its value
                const string arg = replace_first_of(ca->group(1), rd, v);                                                    //  goes first anyway; its
                out.push_back(cat(nxt.substr(0, ca->start(1)), arg, nxt.substr(ca->end(1))));                               //  low byte, a char)
                k += 2;
                continue;
            }
        }
        out.push_back(lines[k]);
        ++k;
    }
    return join_lines(out);
}

// ---- the model's temporaries -------------------------------------------------------------------------------------

string temporaries(string_view text_in) {
    static const Regex temp_assign(R"re(^(\s*)(t\d+) = (.+);$)re");
    static const Regex declaration(R"re(^[ \t]*(?:int|char|long|unsigned|register)\b)re");
    static const Regex int_temps(R"re([ \t]*int t\d+(?:, t\d+)*;)re");
    static const Regex plain_value(R"re([\w\[\]\->.]+)re");
    static const Regex temp_use(R"re(\b(t\d+)\b)re");
    Lines lines = split_lines(text_in);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = temp_assign.match(lines[k]);
        if (m) {
            const string x = str(m->group(2));
            // (the reads of it, not its declaration)
            std::vector<std::size_t> uses;
            for (std::size_t j = 0; j < lines.size(); ++j)
                if (j != k && has_word(lines[j], x) && !declaration.match(lines[j])) uses.push_back(j);
            std::size_t given_again = lines.size();
            for (std::size_t j = k + 1; j < lines.size(); ++j)
                if (begins_assigning(lines[j], {x})) {
                    given_again = j;
                    break;
                }
            // (this value's reads: up to where it is given again. What stands after that point is the next value's.)
            std::vector<std::size_t> mine;
            for (std::size_t j : uses)
                if (k < j && j < given_again) mine.push_back(j);
            if (mine.size() == 1 && mine[0] == k + 1) uses = {k + 1};   // (read once before it is given again: that value's)
            if (uses.size() == 1 && uses[0] == k + 1 && count_word(lines[k + 1], x) == 1) {
                const string e = str(m->group(3));
                lines[k + 1] = replace_word_all(lines[k + 1], x, plain_value.full_match(e) ? e : cat("(", e, ")"));
                lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
                continue;
            }
        }
        ++k;
    }
    // (declared again below: the ones still read)
    Lines kept;
    for (const string& l : lines)
        if (!int_temps.full_match(l)) kept.push_back(l);
    string text = join_lines(kept);
    const std::size_t brace = text.find('{');
    const string_view after_brace = brace == string::npos ? string_view(text) : string_view(text).substr(brace + 1);
    std::set<string> left;
    temp_use.for_each_match(after_brace, [&](const RegexMatch& m) {
        left.insert(str(m.group(1)));
        return true;
    });
    if (!left.empty()) {
        const dt::BodySplit s = dt::split_body(text);
        if (!s.brace.empty()) text = cat(s.head, s.brace, "\n    int ", txt::join(left, ", "), ";", s.body);
    }
    return text;
}

// ---- registers returned, stored, called ------------------------------------------------------------------

namespace {

// `\s*EXPR = (REG op rest);\s*` for the statement putting a changed register back: its leading blanks and the
// parenthesised value.
struct PutBack {
    string lead, value;
};
std::optional<PutBack> put_back(string_view line, string_view expr, string_view reg) {
    const string_view s = txt::ltrim(line);
    const string head = cat(expr, " = (", reg, " ");
    if (!s.starts_with(head)) return std::nullopt;
    string_view rest = s.substr(head.size());
    if (rest.size() < 2 || string_view("-+&|^").find(rest[0]) == string_view::npos || rest[1] != ' ') return std::nullopt;
    const char op = rest[0];
    rest.remove_prefix(2);
    const std::size_t semi = rest.find(';');
    if (semi == string_view::npos || semi < 2 || rest[semi - 1] != ')' || !blank(rest.substr(semi + 1))) return std::nullopt;
    return PutBack{str(line.substr(0, line.size() - s.size())), cat("(", reg, " ", string(1, op), " ", rest.substr(0, semi))};
}

}  // namespace

string fold_registers(string_view text) {
    static const Regex assign(R"re(^(\s*)(ax|al|bx|cx|dx|si|di|bl|cl|dl|ah|bh|ch|dh) = (.*);\s*$)re");
    static const Regex pure_value(
        R"re([A-Za-z_]\w*|-?\d+|[A-Za-z_]\w*(?:->f\d+)+|\*[A-Za-z_]\w*|[A-Za-z_]\w*\[[A-Za-z_0-9]+\])re");
    static const Regex effects(R"re(\w\s*\(|\+\+|--|[^=!<>]=[^=])re");
    static const Regex operation(R"re([A-Za-z_][\w\->.]* (?:[-+&|^*]|<<|>>) (?:[A-Za-z_][\w\->.]*|\d+))re");
    static const Regex call_expr(R"re([A-Za-z_]\w*\(.*\))re");
    static const Regex adds_to_local(R"re(^(\s*)([a-z]|v\d+|r_\w+|si|di) = \2 ([-+&|^]) (\w+);\s*$)re");
    static const Regex copy_to_name(R"re(^\s*[A-Za-z_]\w* = (\w+);\s*$)re");
    static const Regex test_value(R"re(!?\(.*\))re");
    static const Regex identifiers(R"re([A-Za-z_]\w*)re");
    static const Regex call_in(R"re(\b(?!if\b|while\b|sizeof\b)[A-Za-z_]\w*\s*\(|\(\*[^()]+\)\s*\()re");
    static const Regex word_paren(R"re(\w\s*\()re");

    Lines lines = split_lines(text);
    Lines out;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const string l = lines[k];
        if (const auto m = assign.match(l)) {
            const string ind = str(m->group(1)), reg = str(m->group(2));
            string expr = str(m->group(3));
            const bool variable = reg == "si" || reg == "di";   // (a register variable: given a value it keeps)
            // (the next statement, and what follows it)
            string nxt;
            for (std::size_t j = k + 1; j < lines.size(); ++j)
                if (!blank(lines[j])) {
                    nxt = lines[j];
                    break;
                }
            std::optional<std::size_t> at;   // (where it stands: the first line from k + 1 that is it)
            for (std::size_t j = k + 1; j < lines.size(); ++j)
                if (lines[j] == nxt) {
                    at = j;
                    break;
                }
            const string later = at ? join_lines(std::span<const string>(lines).subspan(*at + 1)) : string();
            const string before_given = until_given(later, reg, false);   // (later, up to where reg is given again)
            bool pure = pure_value.full_match(expr) || (!effects.search(expr) && expr.starts_with("(") && expr.ends_with(")"));
            if (!pure && operation.full_match(expr)) {   // (a plain operation: in parentheses where it goes)
                expr = cat("(", expr, ")");
                pure = true;
            }
            const auto upd = adds_to_local.full_match(nxt);
            if (at && upd && upd->group(4) == reg && !variable && call_expr.full_match(expr) && !has_word(before_given, reg) &&
                !has_word(expr.substr(0, expr.find('(')), upd->group(2))) {
                // (a call's value added to a local, a parameter: the local updated by it)
                lines[*at] = cat(upd->group(1), upd->group(2), " = ", upd->group(2), " ", upd->group(3), " ", expr, ";");
                continue;
            }
            if (!pure && !effects.search(expr) && test_value.full_match(expr)) {   // (a test's value: no call, no store)
                expr = cat("(", expr, ")");
                pure = true;
            }
            // (a value with no effect: a name, a field, an element, or no call and no store in it)
            bool names_stepped = false;
            identifiers.for_each_match(expr, [&](const RegexMatch& n) {
                if (is_updated(nxt, n.group(0))) names_stepped = true;
                return !names_stepped;
            });
            const auto copy = copy_to_name.full_match(nxt);
            if (at && pure && !variable && count_word(nxt, reg) == 1 &&
                ((copy && copy->group(1) == reg) || dt::plain_store(nxt, reg) || !names_stepped) && !has_word(before_given, reg) &&
                (dt::plain_store(nxt, reg) || !is_updated(nxt, expr))) {
                // (a plain value read once, at once: in its place)
                if (reg == "al" || reg == "bl" || reg == "cl" || reg == "dl") {
                    // (a byte read, changed in a register, put back: the value cast - else done in place)
                    if (const auto bk = put_back(nxt, expr, reg)) nxt = cat(bk->lead, expr, " = (char)", bk->value, ";");
                }
                lines[*at] = replace_word_all(nxt, reg, expr);
                continue;
            }
            if (at && txt::trim(nxt) == cat("return ", reg, ";") && (!variable || !word_paren.search(expr))) {
                out.push_back(cat(ind, "return ", expr, ";"));
                lines[*at] = string(1, '\0');
                continue;
            }
            const Words reads = reg == "ax" ? Words{"ax", "al"} : Words{reg};
            const bool used = live_from(lines, k + 1, reg, as_views(reads)) || variable;   // (a register variable: a loop may read it on its next pass)
            if (!used) {
                const bool call = call_in.search(expr).has_value();   // (a call: a name, or a pointer's, then its arguments)
                if (call && !out.empty() && txt::trim(out.back()) == cat(expr, ";")) continue;   // (the call written twice)
                if (call) out.push_back(cat(ind, expr, ";"));
                continue;   // (a load nothing reads)
            }
        }
        if (l != string(1, '\0')) out.push_back(l);
    }
    return join_lines(out);
}

}  // namespace cb::exact::draft::shapes
