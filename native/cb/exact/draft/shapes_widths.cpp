// The passes that read back widths and pairs: a byte widened with zeros (an unsigned char), a char widened for a test, a
// word element's offset, a long that is the two words dx:ax - and the swaps of two stores and the calls an operator joins.
#include <algorithm>
#include <charconv>
#include <format>
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

constexpr RegexFlags kLines = RegexFlags::Multiline;

string str(string_view s) { return string(s); }

const Regex& field_place() {   // `(\w+(?:->f\d+)*)->f(\d+)`: a field of a struct reached through pointers
    static const Regex re(R"re((\w+(?:->f\d+)*)->f(\d+))re");
    return re;
}

const Regex& global_byte() {   // `g_xxxx`: a global by its address
    static const Regex re(R"re(g_([0-9a-f]{1,4}))re");
    return re;
}

std::optional<std::uint64_t> hex_number(string_view s) {
    std::uint64_t v = 0;
    const auto r = std::from_chars(s.data(), s.data() + s.size(), v, 16);
    if (r.ec != std::errc() || r.ptr != s.data() + s.size()) return std::nullopt;
    return v;
}

// A byte the code widens with zeros is an unsigned char: a global `g_xxxx` by its address, a field of a struct by its
// pointer and offset.
void note_unsigned_global(FunctionFacts& facts, string_view hex) {
    if (const auto v = hex_number(hex)) facts.unsigned_globals.insert(std::format("g_{:04x}", *v));
}
void note_unsigned_field(FunctionFacts& facts, const RegexMatch& f) {
    if (const auto off = number(f.group(2))) facts.unsigned_sites.insert(dt::Site{str(f.group(1)), *off});
}

bool all_word(string_view s) { return !s.empty() && std::ranges::all_of(s, is_word_char); }

// `[\w\->.\[\]*]+`: what a store's left side is made of.
bool is_lvalue_text(string_view s) {
    return !s.empty() && std::ranges::all_of(s, [](char c) { return is_word_char(c) || c == '-' || c == '>' || c == '.' || c == '[' || c == ']' || c == '*'; });
}

}  // namespace

// ---- bytes widened with zeros ----------------------------------------------------------------------------------

string zero_extends(string_view text, FunctionFacts& facts) {
    static const Regex load(R"re(^(\s*)([abcd])[lx] = (.+);\s*$)re");
    static const Regex chained(R"re(^([ \t]*)([^=;\n]+?) = ([^;\n]+);\n[ \t]*([abcd])h = 0;\n[ \t]*([^=;\n]+?) = \4x;$)re", kLines);
    const Lines original = split_lines(text);
    MaybeLines lines = maybe(original);
    for (std::size_t k = 0; k + 1 < original.size(); ++k) {   // (al, bl.. - or the word the model wrote)
        const string current = lines[k] ? *lines[k] : string();
        const auto m = load.match(current);
        if (!m) continue;
        const string letter = str(m->group(2));
        if (txt::trim(lines[k + 1] ? *lines[k + 1] : string()) != cat(letter, "h = 0;")) continue;
        string e = str(m->group(3));
        const string ind = str(m->group(1));
        // (a byte masked first: al = X; al = (al & K))
        const string mask_head = cat("(", letter, "l & ");
        if (e.starts_with(mask_head) && e.ends_with(")") && k > 0 && lines[k - 1]) {
            const string_view digits = string_view(e).substr(mask_head.size(), e.size() - mask_head.size() - 1);
            const string_view prior = txt::ltrim(*lines[k - 1]);
            const string prior_head = cat(letter, "l = ");
            if (!digits.empty() && std::ranges::all_of(digits, [](char c) { return c >= '0' && c <= '9'; }) && prior.starts_with(prior_head) &&
                prior.ends_with(";") && prior.size() > prior_head.size() + 1) {
                const string value = str(prior.substr(prior_head.size(), prior.size() - prior_head.size() - 1));
                e = cat("(", value, " & ", digits, ")");
                lines[k - 1].reset();
                if (const auto f0 = field_place().full_match(value)) note_unsigned_field(facts, *f0);
            }
        }
        lines[k] = cat(ind, letter, "x = ", e, ";");
        lines[k + 1].reset();
        const auto g = global_byte().full_match(e);
        const auto f = field_place().full_match(e);   // (through a field too: its own struct's)
        if (g) note_unsigned_global(facts, g->group(1));
        else if (f) note_unsigned_field(facts, *f);
    }
    const string widened = join_live(lines);
    // (a byte stored, then widened with zeros and stored again: y = x = e, x an unsigned char)
    return chained.replace_all(widened, [&](const RegexMatch& m) {
        const string_view x = m.group(2);
        if (const auto g = global_byte().full_match(x)) note_unsigned_global(facts, g->group(1));
        else if (const auto f = field_place().full_match(x)) note_unsigned_field(facts, *f);
        return cat(m.group(1), m.group(5), " = ", x, " = ", m.group(3), ";");
    });
}

string high_zero(string_view text, FunctionFacts& facts) {
    static const Regex zero_high(R"re(\s*ah = 0;)re");
    static const Regex disturbs(R"re(\w\(|\bah\b|^\s*ax = |\b(?:if|while|for|switch)\b|[{}]|L_\w+:)re");
    static const Regex clobbers(R"re(\w\(|\bah\b|^\s*ax = |[{}]|L_\w+:)re");
    static const Regex stores_ax(R"re(\s*\w+ = ax;)re");
    static const Regex load_al(R"re((\s*)al = (.+);)re");
    static const Regex store(R"re((\s*)(\w+) = ax;)re");
    MaybeLines lines = maybe(split_lines(text));
    bool zero = false;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!lines[k]) continue;
        const string l = *lines[k];
        if (zero_high.full_match(l)) {
            zero = true;
            continue;
        }
        if (disturbs.search(l)) {
            zero = stores_ax.full_match(l) ? zero : false;
            if (clobbers.search(l)) zero = false;
            continue;
        }
        const auto m = load_al.full_match(l);
        if (zero && m && k + 1 < lines.size()) {
            const auto n = store.full_match(*lines[k + 1]);
            if (n) {
                lines[k] = cat(n->group(1), n->group(2), " = ", m->group(2), ";");
                lines[k + 1].reset();
                if (const auto f = field_place().full_match(m->group(2))) note_unsigned_field(facts, *f);
            }
        }
    }
    return join_live(lines);
}

string widen_bytes(string_view text) {
    static const Regex byte_load(R"re(^(\s*)(al|bl|cl|dl) = (.*);\s*$)re");
    Lines lines = split_lines(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = byte_load.match(lines[k]);
        if (!m) continue;
        const string b8 = str(m->group(2)), w = cat(b8.substr(0, 1), "x");
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            const string& x = lines[j];
            if (has_word(x, b8)) break;
            if (has_word(x, w)) {
                if (!txt::ltrim(x).starts_with(cat(w, " = "))) lines[k] = cat(m->group(1), w, " = ", m->group(3), ";");
                break;
            }
        }
    }
    return join_lines(lines);
}

std::optional<string> char_widens(string_view text) {
    static const Regex char_locals(R"re(^[ \t]*char (v\d+|[a-z]|r_\w+);)re", kLines);
    static const Regex test_line(R"re(^(\s*)(if|while|for|switch) \((.*)\)( \{| break;| continue;| return [^;]*;)?$)re");
    std::vector<string> names;
    char_locals.for_each_match(text, [&](const RegexMatch& m) {
        names.push_back(str(m.group(1)));
        return true;
    });
    if (names.empty()) return std::nullopt;
    // `(?<![\w])(?<!\(char\))(NAMES)\b(?!\s*=[^=])`: a name not already cast, not a word's tail, not stored to
    const auto widen = [&](string_view s) {
        string out;
        std::size_t i = 0;
        while (i < s.size()) {
            std::size_t matched = 0;
            if (i == 0 || !is_word_char(s[i - 1])) {
                const bool cast = i >= 6 && s.substr(i - 6, 6) == "(char)";
                for (const string& n : names) {
                    if (cast || s.compare(i, n.size(), n) != 0 || !boundary_at(s, i + n.size())) continue;
                    std::size_t q = i + n.size();
                    while (q < s.size() && txt::is_space(s[q])) ++q;
                    if (q + 1 < s.size() && s[q] == '=' && s[q + 1] != '=') continue;   // (stored to: not read)
                    matched = n.size();
                    break;
                }
            }
            if (matched > 0) {
                out += "(int)";
                out.append(s.substr(i, matched));
                i += matched;
            } else {
                out += s[i++];
            }
        }
        return out;
    };
    Lines out;
    for (const string& l : split_lines(text)) {
        const auto m = test_line.match(l);
        out.push_back(m ? cat(m->group(1), m->group(2), " (", widen(m->group(3)), ")", m->has(4) ? m->group(4) : string_view()) : l);
    }
    string t = join_lines(out);
    if (t == text) return std::nullopt;
    return t;
}

// ---- words and bytes ----------------------------------------------------------------------------------------------

namespace {

// Is `p` declared `int *p;` (or `register int *p;`) on a line of its own?
bool declares_int_pointer(string_view text, string_view p) {
    const string tail = cat("int *", p, ";");
    for (string_view l : txt::lines(text)) {
        std::size_t i = 0;
        while (i < l.size() && (l[i] == ' ' || l[i] == '\t')) ++i;
        l.remove_prefix(i);
        if (l.starts_with("register ")) l.remove_prefix(9);
        if (l.starts_with(tail)) return true;
    }
    return false;
}

}  // namespace

std::vector<string> unsigned_fields(string_view text, std::size_t most) {
    static const Regex structure(R"re(struct (\w+) \{[^}]*\})re");
    static const Regex int_field(R"re((?<![\w ])int f(\d+);|(?<=[{;] )int f(\d+);)re");
    static const Regex digits(R"re(\d+)re");
    struct Field {
        string tag;
        std::size_t begin, end;
        string number;
    };
    std::vector<Field> fields;
    structure.for_each_match(text, [&](const RegexMatch& s) {
        const string_view body = s.group(0);
        int_field.for_each_match(body, [&](const RegexMatch& f) {
            const std::size_t a = s.start() + f.start(), b = s.start() + f.end();
            const string n = f.has(1) ? str(f.group(1)) : str(digits.search(text.substr(a, b - a))->group(0));
            fields.push_back({str(s.group(1)), a, b, n});
            return true;
        });
        return true;
    });
    std::vector<string> out;
    for (std::size_t i = 0; i < fields.size() && i < 16; ++i) {
        const Field& f = fields[i];
        // the pointers and arrays of that struct
        std::set<string> ptrs;
        const string star = cat("struct ", f.tag, " *");
        for (std::size_t at = text.find(star); at != string_view::npos; at = text.find(star, at + 1)) {
            std::size_t q = at + star.size();
            while (q < text.size() && txt::is_space(text[q])) ++q;
            std::size_t e = q;
            while (e < text.size() && is_word_char(text[e])) ++e;
            if (e > q) {
                ptrs.insert(str(text.substr(q, e - q)));
            }
        }
        const string elem = cat("struct ", f.tag, " ");
        for (std::size_t at = text.find(elem); at != string_view::npos; at = text.find(elem, at + 1)) {
            std::size_t q = at + elem.size(), e = q;
            while (e < text.size() && is_word_char(text[e])) ++e;
            if (e > q && e < text.size() && text[e] == '[') ptrs.insert(str(text.substr(q, e - q)));
        }
        // `\bP(?:\[[^\]]*\])?(?:->|\.)fN\b\)? (?:>>|[<>]=?(?![<>]))`: P (or an element of it) read, then shifted or compared
        const auto compared = [&](const string& p) {
            for (std::size_t at = 0; at + p.size() <= text.size(); ++at) {
                if (text.compare(at, p.size(), p) != 0 || !boundary_at(text, at)) continue;
                std::size_t q = at + p.size();
                if (q < text.size() && text[q] == '[') {
                    const std::size_t close = text.find(']', q);
                    if (close == string_view::npos) continue;
                    q = close + 1;
                }
                if (text.substr(q, 2) == "->") q += 2;
                else if (q < text.size() && text[q] == '.') q += 1;
                else continue;
                const string field = cat("f", f.number);
                if (text.compare(q, field.size(), field) != 0 || !boundary_at(text, q + field.size())) continue;
                q += field.size();
                if (q < text.size() && text[q] == ')') ++q;
                if (q >= text.size() || text[q] != ' ') continue;
                ++q;
                if (text.substr(q, 2) == ">>") return true;
                if (q < text.size() && (text[q] == '<' || text[q] == '>')) {
                    if (q + 1 >= text.size() || text[q + 1] == '=' || (text[q + 1] != '<' && text[q + 1] != '>')) return true;
                }
            }
            return false;
        };
        if (!std::ranges::any_of(ptrs, compared)) continue;
        out.push_back(cat(text.substr(0, f.begin), "unsigned f", f.number, ";", text.substr(f.end)));
        if (out.size() >= most) break;
    }
    return out;
}

string word_offsets(string_view text) {
    static const Regex through_char(R"re(\(\(int \*\)\(\(char \*\)(\w+) \+ (\d+)\)\)\[([^\[\]]+)\])re");
    return through_char.replace_all(text, [&](const RegexMatch& m) -> string {
        const string_view p = m.group(1), e = m.group(3);
        const auto n = number(m.group(2));
        if (!n) return str(m.group(0));
        if (*n % 2 != 0) return str(m.group(0));
        if (declares_int_pointer(text, p)) return cat(p, "[", e, " + ", std::to_string(*n / 2), "]");
        return cat("((int *)", p, ")[", e, " + ", std::to_string(*n / 2), "]");   // (another pointer: through an int * of it)
    });
}

string byte_heads(string_view text) {
    static const Regex first_word(R"re(^([ \t]*)\*\(int \*\)(\w+) = ([^;]+);$)re", kLines);
    return first_word.replace_all(text, "$1$2[0] = $3;");
}

string pointer_index(string_view text, FunctionFacts& facts) {
    static const Regex indexed(
        R"re(^([ \t]*)bx = (.+);\n[ \t]*bx = \(bx << 1\) \+ (g_[0-9a-f]+|[a-z]|v\d+);\n[ \t]*(.*)$)re", kLines);
    static const Regex plain_index(R"re([\w\->.]+)re");
    return indexed.replace_all(text, [&](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), e = m.group(2), p = m.group(3), rest = m.group(4);
        std::size_t stars = 0;
        for (std::size_t at = rest.find("*bx"); at != string_view::npos; at = rest.find("*bx", at + 1))
            if (boundary_at(rest, at + 3)) ++stars;
        if (stars != 1 || has_word(txt::replace_all(rest, "*bx", ""), "bx")) return str(m.group(0));
        if (p.starts_with("g_")) {
            const auto v = hex_number(p.substr(2));
            if (!v) return str(m.group(0));
            facts.word_pointers.insert(std::format("g_{:04x}", *v));
        } else {
            facts.word_pointers.insert(str(p));
        }
        return cat(ind, txt::replace_all(rest, "*bx", cat("*(", p, " + ", plain_index.full_match(e) ? str(e) : cat("(", e, ")"), ")")));
    });
}

// ---- longs: the two words dx:ax ------------------------------------------------------------------------------

std::optional<string> pair_long(string_view lo, string_view hi, FunctionFacts& facts) {
    static const Regex global(R"re(g_([0-9a-f]+))re");
    static const Regex local(R"re(v(\d+))re");
    static const Regex plain_place(R"re([\w\->.\[\]]+)re");
    {
        const auto f = field_place().full_match(lo), g = field_place().full_match(hi);
        if (f && g && f->group(1) == g->group(1)) {
            const auto fo = number(f->group(2)), go = number(g->group(2));
            if (fo && go && *go == *fo + 2) {
                facts.longs.fields.insert(*fo);
                return cat("*(long *)&", f->group(1), "->f", f->group(2));
            }
        }
    }
    {
        const auto f = global.full_match(lo), g = global.full_match(hi);
        if (f && g) {
            const auto fa = hex_number(f->group(1)), ga = hex_number(g->group(1));
            if (fa && ga && *ga == *fa + 2) {
                facts.longs.globals.insert(std::format("g_{:04x}", *fa));
                return str(lo);
            }
        }
    }
    {
        const auto f = local.full_match(lo), g = local.full_match(hi);
        if (f && g) {
            const auto fn = number(f->group(1)), gn = number(g->group(1));
            if (fn && gn && *gn == *fn - 2) return cat("*(long *)&", lo);
        }
    }
    // (an int and its sign spread: `0 - (lo >> 15)`, in parentheses or not)
    if (plain_place.full_match(lo)) {
        const string core = cat("0 - (", lo, " >> 15)");
        for (const string& candidate : {core, cat("(", core), cat(core, ")"), cat("(", core, ")")})
            if (hi == candidate) return cat("(long)", lo);
    }
    return std::nullopt;
}

string long_tests(string_view text, FunctionFacts& facts) {
    static const Regex given_and_tested(
        R"re(^([ \t]*)ax = ([^;\n]+);\n[ \t]*([\w\->.]+) = ax;\n[ \t]*([\w\->.]+) = dx;\n[ \t]*ax = \(ax \| dx\);\n[ \t]*if \(\(ax ([!=]=) 0\)\)(.*)$)re",
        kLines);
    return given_and_tested.replace_all(text, [&](const RegexMatch& m) -> string {
        const auto x = pair_long(m.group(3), m.group(4), facts);
        if (!x || x->starts_with("(long)")) return str(m.group(0));
        return cat(m.group(1), "if (((", *x, " = ", m.group(2), ") ", m.group(5), " 0))", m.group(6));
    });
}

string sunk_pairs(string_view text, FunctionFacts& facts) {
    static const Regex store_ax(R"re((\s*)([\w\->.]+) = ax;)re");
    static const Regex store_dx(R"re(\s*([\w\->.]+) = dx;)re");
    static const Regex word_pair(R"re(\s*(ax|dx) = ([^;]+);)re");
    static const Regex ax_value(R"re(\s*ax = (.+);)re");
    static const Regex wide_value(R"re(\(double \*\)|\(float \*\)|\(long\)|\(double\))re");
    static const Regex gives_dx(R"re(^\s*dx = )re");
    Lines lines = split_lines(text);
    const std::string_view reads[] = {"ax", "al", "ah", "dx"};
    std::size_t k = 0;
    // (the long an arm leaves, and the arm without it)
    struct Value {
        string value;
        Lines rest;
    };
    const auto value_of = [&](const Lines& arm) -> std::optional<Value> {
        Lines body;
        for (const string& x : arm)
            if (!blank(x)) body.push_back(x);
        const auto first_at = [&](const string& line) { return static_cast<std::size_t>(std::ranges::find(arm, line) - arm.begin()); };
        if (body.size() >= 2) {
            const auto a = word_pair.full_match(body[body.size() - 2]);
            const auto b = word_pair.full_match(body.back());
            if (a && b && a->group(1) != b->group(1)) {
                const auto& lo = a->group(1) == "ax" ? *a : *b;
                const auto& hi = a->group(1) == "ax" ? *b : *a;
                if (const auto v = pair_long(lo.group(2), hi.group(2), facts))
                    return Value{*v, Lines(arm.begin(), arm.begin() + static_cast<std::ptrdiff_t>(first_at(body[body.size() - 2])))};
            }
        }
        if (!body.empty()) {
            const auto e = ax_value.full_match(body.back());
            bool dx_given = false;
            for (std::size_t i = body.size() >= 3 ? body.size() - 3 : 0; i < body.size(); ++i)
                if (gives_dx.match(body[i])) dx_given = true;
            if (e && wide_value.search(e->group(1)) && !dx_given)
                return Value{str(e->group(1)), Lines(arm.begin(), arm.begin() + static_cast<std::ptrdiff_t>(first_at(body.back())))};
        }
        return std::nullopt;
    };
    while (k < lines.size()) {
        const auto p = dt::if_parts(lines, k);
        ++k;
        if (!p || !p->else_arm || p->end + 1 >= lines.size()) continue;
        const auto st = store_ax.full_match(lines[p->end]);
        const auto st2 = store_dx.full_match(lines[p->end + 1]);
        const auto dst_opt = st && st2 ? pair_long(st->group(2), st2->group(1), facts) : std::nullopt;
        if (!dst_opt || dst_opt->starts_with("(long)")) continue;
        const string dst = *dst_opt;
        if (live_from(lines, p->end + 2, "ax", reads)) continue;
        const auto a = value_of(p->then_arm), b = value_of(*p->else_arm);
        if (!a || !b) continue;
        const string ind = str(st->group(1));
        const auto same = [&](const string& v) {
            return v == dst || txt::replace_all(v, "*(long *)&", "") == txt::replace_all(dst, "*(long *)&", "");
        };
        const auto any_text = [](const Lines& x, const Lines& y) {
            return std::ranges::any_of(x, [](const string& l) { return !blank(l); }) || std::ranges::any_of(y, [](const string& l) { return !blank(l); });
        };
        Lines replacement;
        if (!any_text(a->rest, b->rest) && (same(a->value) || same(b->value))) {
            // (one way the long kept: the other's value or it - a ?:)
            const string c = p->cond.starts_with("(") ? p->cond : cat("(", p->cond, ")");
            replacement = {cat(ind, dst, " = ", c, " ? ", a->value, " : ", b->value, ";")};
        } else {
            replacement.push_back(lines[k - 1]);
            replacement.insert(replacement.end(), a->rest.begin(), a->rest.end());
            replacement.push_back(cat(ind, "    ", dst, " = ", a->value, ";"));
            replacement.push_back(cat(ind, "} else {"));
            replacement.insert(replacement.end(), b->rest.begin(), b->rest.end());
            replacement.push_back(cat(ind, "    ", dst, " = ", b->value, ";"));
            replacement.push_back(cat(ind, "}"));
        }
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k - 1), lines.begin() + static_cast<std::ptrdiff_t>(p->end + 2));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k - 1), replacement.begin(), replacement.end());
    }
    return join_lines(lines);
}

string sunk_tails(string_view text) {
    static const Regex register_value(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re");
    static const Regex statement(R"re(\s*[^;{}]+;)re");
    static const Regex leaves(R"re(^\s*(?:return|goto|break|continue|case|default)\b)re");
    static const Regex plain_or_call(R"re([\w\->.\[\]]+|[\w]+\(.*\))re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto p = dt::if_parts(lines, k);
        ++k;
        if (!p || !p->else_arm || p->then_arm.empty() || p->else_arm->empty() || p->end >= lines.size()) continue;
        const string ind = str(indent_text(lines[k - 1]));
        const auto a = register_value.full_match(p->then_arm.back());
        const auto b = register_value.full_match(p->else_arm->back());
        const string s = lines[p->end];
        if (!a || !b || a->group(2) != b->group(2) || !s.starts_with(ind) || (s.size() > ind.size() && txt::is_space(s[ind.size()]))) continue;
        const string r = str(a->group(2));
        // `\s*NAME = REG;`: the statement only stores the register
        const string_view s_left = txt::ltrim(s);
        const string store_tail = cat(" = ", r, ";");
        const bool stores_r = s_left.size() > store_tail.size() && s_left.ends_with(store_tail) &&
                              is_lvalue_text(s_left.substr(0, s_left.size() - store_tail.size()));
        const std::string_view reads_byte[] = {"ax", "al", "ah"};
        const bool wide_or_low = r == "ax" || r == "al";
        if (!statement.full_match(s) || stores_r || leaves.match(s) || count_word(s, r) != 1 ||
            (wide_or_low ? live_from(lines, p->end + 1, r, reads_byte) : live_from(lines, p->end + 1, r)))
            continue;
        bool read_in_arms = false;
        for (std::size_t i = 0; i + 1 < p->then_arm.size(); ++i) read_in_arms = read_in_arms || has_word(p->then_arm[i], r);
        for (std::size_t i = 0; i + 1 < p->else_arm->size(); ++i) read_in_arms = read_in_arms || has_word((*p->else_arm)[i], r);
        if (read_in_arms) continue;
        const auto put = [&](string_view e) { return plain_or_call.full_match(e) ? str(e) : cat("(", e, ")"); };
        const string ia = str(a->group(1)), ib = str(b->group(1));
        const string trimmed = str(txt::trim(s));
        Lines replacement{lines[k - 1]};
        replacement.insert(replacement.end(), p->then_arm.begin(), p->then_arm.end() - 1);
        replacement.push_back(cat(ia, replace_word_all(trimmed, r, put(a->group(3)))));
        replacement.push_back(cat(ind, "} else {"));
        replacement.insert(replacement.end(), p->else_arm->begin(), p->else_arm->end() - 1);
        replacement.push_back(cat(ib, replace_word_all(trimmed, r, put(b->group(3)))));
        replacement.push_back(cat(ind, "}"));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k - 1), lines.begin() + static_cast<std::ptrdiff_t>(p->end + 1));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k - 1), replacement.begin(), replacement.end());
    }
    return join_lines(lines);
}

// ---- calls an operator joins, two stores swapped ------------------------------------------------------------------

std::optional<string> call_pairs(string_view text) {
    static const Regex pair(R"re(^([ \t]*)(t\d+) = ([^;\n]+);\n[ \t]*(v\d+) = (\w+\([^;\n]*\));\n[ \t]*(.*)$)re", kLines);
    static const Regex int_v(R"re(^[ \t]*int (v\d+);$)re", kLines);
    static const Regex int_ts(R"re(^[ \t]*int ((?:t\d+, )*t\d+);$)re", kLines);
    static const Regex blank_runs(R"re(\n\n+)re");
    string t = pair.replace_all(text, [&](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), tmp = m.group(2), a = m.group(3), v = m.group(4), f = m.group(5), rest = m.group(6);
        // `\b(\w+)\(T\) ([-+*&|^]) V\b`: the call the temporary is the argument of, joined to V
        std::optional<std::size_t> from, to;
        string_view callee, op;
        const string open = cat("(", tmp, ") ");
        for (std::size_t p = 0; p < rest.size() && !from; ++p) {
            if (!is_word_char(rest[p]) || !boundary_at(rest, p)) continue;
            std::size_t q = p;
            while (q < rest.size() && is_word_char(rest[q])) ++q;
            if (rest.compare(q, open.size(), open) != 0) continue;
            const std::size_t o = q + open.size();
            if (o + 2 > rest.size() || string_view("-+*&|^").find(rest[o]) == string_view::npos || rest[o + 1] != ' ') continue;
            if (rest.compare(o + 2, v.size(), v) != 0 || !boundary_at(rest, o + 2 + v.size())) continue;
            from = p;
            to = o + 2 + v.size();
            callee = rest.substr(p, q - p);
            op = rest.substr(o, 1);
        }
        const std::size_t uses = count_word(text, v);
        if (!from || (uses != 3 && uses != 2)) return str(m.group(0));
        return cat(ind, rest.substr(0, *from), callee, "(", a, ") ", op, " ", f, rest.substr(*to));
    });
    if (t == text) return std::nullopt;
    // (the temporary's local: gone)
    std::vector<string> locals;
    int_v.for_each_match(t, [&](const RegexMatch& m) {
        locals.push_back(str(m.group(1)));
        return true;
    });
    for (const string& v : locals) {
        if (count_word(t, v) != 1) continue;
        Lines lines = split_lines(t);
        Lines kept;
        for (std::size_t i = 0; i < lines.size(); ++i) {
            const string_view s = lines[i];
            const std::size_t lead = indent_of(s);
            const bool is_decl = s.substr(lead) == cat("int ", v, ";") && s.substr(0, lead).find_first_not_of(" \t") == string_view::npos;
            if (is_decl && i + 1 < lines.size()) continue;   // (the pattern takes its newline: not the last line)
            kept.push_back(lines[i]);
        }
        t = join_lines(kept);
    }
    std::vector<string> declared;
    int_ts.for_each_match(t, [&](const RegexMatch& m) {
        declared.push_back(str(m.group(1)));
        return true;
    });
    for (const string& x : declared) {
        std::vector<string> left;
        for (string_view y : txt::split(x, ", "))
            if (count_word(t, y) > 1) left.push_back(str(y));
        Lines lines = split_lines(t);
        for (string& l : lines) {
            const std::size_t lead = indent_of(l);
            if (l.substr(lead) == cat("int ", x, ";") && l.substr(0, lead).find_first_not_of(" \t") == string::npos)
                l = left.empty() ? string() : cat(l.substr(0, lead), "int ", txt::join(left, ", "), ";");
        }
        t = join_lines(lines);
    }
    return blank_runs.replace_all(t, "\n");
}

string lvalue_choices(string_view text) {
    static const Regex store_to_choice(R"re(^([ \t]*)\*\((.+\?.+:.+)\) = ([^;]+);$)re", kLines);
    static const Regex effect(R"re(\w\s*\(|\+\+|--)re");
    return store_to_choice.replace_all(text, [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), inner = m.group(2), e = m.group(3);
        const auto parts = dt::top_split(inner, "?");
        if (parts.size() != 2) return str(m.group(0));
        string c = str(txt::trim(parts[0]));
        const auto ab = dt::top_split(parts[1], ":");
        if (ab.size() != 2 || effect.search(e)) return str(m.group(0));
        const string a = str(txt::trim(ab[0])), b = str(txt::trim(ab[1]));
        if (!c.starts_with("(")) c = cat("(", c, ")");
        return cat(ind, "if ", c, " {\n", ind, "    *", a, " = ", e, ";\n", ind, "} else {\n", ind, "    *", b, " = ", e, ";\n", ind, "}");
    });
}

std::vector<string> adjacent_swaps(string_view text, std::size_t most) {
    static const Regex store(R"re(([ \t]*)([\w\->.\[\]*]+) = ([^;]+);)re");
    static const Regex call(R"re(\w\s*\()re");
    const Lines lines = split_lines(text);
    // the variable a store goes through: its place up to the first `->` or `[`, without its stars
    const auto root = [](string_view place) {
        std::size_t cut = place.size();
        for (string_view mark : {"->", "["})
            if (const std::size_t at = place.find(mark); at != string_view::npos) cut = std::min(cut, at);
        place = place.substr(0, cut);
        while (place.starts_with("*")) place.remove_prefix(1);
        return str(place);
    };
    std::vector<string> out;
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        const auto a = store.full_match(lines[k]);
        const auto b = store.full_match(lines[k + 1]);
        if (!a || !b || a->group(1) != b->group(1) || call.search(cat(a->group(3), b->group(3)))) continue;
        if (has_word(lines[k + 1], root(a->group(2))) || has_word(lines[k], root(b->group(2)))) continue;
        Lines swapped = lines;
        std::swap(swapped[k], swapped[k + 1]);
        out.push_back(join_lines(swapped));
        if (out.size() >= most) break;
    }
    return out;
}

}  // namespace cb::exact::draft::shapes
