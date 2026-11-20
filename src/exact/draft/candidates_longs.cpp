// The longs the model's words make: two words the code takes as one, a function that returns dx:ax, the compiler's
// ladder of a long compare, and a register given the place of a local. Part of cb/exact/draft/candidates.hpp.
#include <algorithm>
#include <format>
#include <map>
#include <set>

#include "core/regex.hpp"
#include "core/text.hpp"
#include "exact/draft/candidates_internal.hpp"
#include "exact/draft/shapes_scan.hpp"

namespace cb::exact::draft::candidates {

using namespace inner;

namespace {

// The word above x in the model's naming of a long's halves: a parameter a..y and its next, a local vN and vN-2.
std::optional<std::string> longs_high_of_name(sv x) {
    if (x.size() == 1 && x[0] >= 'a' && x[0] <= 'y') return std::string(1, static_cast<char>(x[0] + 1));
    if (x.size() > 1 && x[0] == 'v' && std::ranges::all_of(x.substr(1), is_digit)) return std::format("v{}", to_int(x.substr(1)) - 2);
    return std::nullopt;
}

// `g_` and one or more hex digits.
bool longs_is_data_name(sv x) {
    return x.size() > 2 && x.starts_with("g_") && std::ranges::all_of(x.substr(2), [](char c) { return std::isxdigit(static_cast<unsigned char>(c)) != 0; });
}

std::int64_t longs_data_place(sv x) { return *ct::parse_int<std::int64_t>(x.substr(2), 16); }

// The word above x: as a name of a long's halves, or a word of the data (the next two bytes: g_%x).
std::optional<std::string> longs_high_of(sv x) {
    if (auto h = longs_high_of_name(x)) return h;
    if (longs_is_data_name(x)) return std::format("g_{:x}", longs_data_place(x) + 2);
    return std::nullopt;
}

std::string longs_data_key(std::int64_t place) { return std::format("g_{:04x}", place); }

bool longs_is_word(sv s) { return !s.empty() && std::ranges::all_of(s, is_word); }

// The text of the lines `[from, to)` joined, as the regexes of the two-line passes read them.
using LongsLinePairFn = std::function<std::string(const RegexMatch&)>;

// The text with each two lines in a row that `re` (over both) matches as `fn` makes them - tried at every line, a pair
// that does not change left for the next (no overlapping match lost).
std::string longs_line_pairs(sv text, const Regex& re, const LongsLinePairFn& fn) {
    const Lines lines = split_nl(text);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        if (k + 1 < lines.size()) {
            const std::string two = lines[k] + "\n" + lines[k + 1];
            if (const auto m = re.full_match(two)) {
                const std::string made = fn(*m);
                if (made != two) {
                    out.push_back(made);
                    k += 2;
                    continue;
                }
            }
        }
        out.push_back(lines[k]);
        ++k;
    }
    return join_nl(out);
}

// `[ \t]*X = ` at the start of the line.
bool longs_gives_value(sv line, sv x) {
    std::size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
    return line.substr(i).starts_with(str(x) + " = ");
}

// Non-overlapping occurrences of `needle` followed by a word boundary (and not by "->" when `not_arrow`).
std::size_t longs_count_bounded(sv s, sv needle, bool not_arrow) {
    std::size_t n = 0;
    for (std::size_t p = s.find(needle); p != npos; p = s.find(needle, p + needle.size())) {
        const std::size_t after = p + needle.size();
        if (boundary_at(s, after) && !(not_arrow && s.substr(after).starts_with("->"))) ++n;
    }
    return n;
}

// The number a C text gives: decimal, or hex with 0x. A leading zero is no octal here.
std::optional<std::int64_t> longs_c_number(sv s) {
    if (s.starts_with("0x")) return ct::parse_int<std::int64_t>(s.substr(2), 16);
    if (s.size() > 1 && s[0] == '0') return std::nullopt;
    return ct::parse_int<std::int64_t>(s, 10);
}

// ---- the argument lists passes rewrite -----------------------------------------------------------------------------

// The call `name(args)` of the text, the arguments cut at their top-level commas and trimmed.
const Regex& longs_call_regex() {
    static const Regex call(R"re(\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re");
    return call;
}

std::vector<std::string> longs_args(sv list) {
    std::vector<std::string> out;
    if (ct::trim(list).empty()) return out;
    for (const std::string& a : dt::top_commas(list)) out.emplace_back(ct::trim(a));
    return out;
}

}  // namespace

std::string long_returns(std::string_view text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    const std::string& head_in = split.head;
    std::string body = split.body;
    static const Regex returns(R"re((?m)^([ \t]*)return ([^;]+);$)re");
    static const Regex gives_dx(R"re((?m)^[ \t]*dx = [^;]+;\n\s*\z)re");
    std::vector<std::size_t> starts;
    returns.for_each_match(body, [&](const RegexMatch& m) {
        starts.push_back(m.start());
        return true;
    });
    if (starts.empty() || !std::ranges::all_of(starts, [&](std::size_t s) { return gives_dx.search(sv(body).substr(0, s)).has_value(); })) return str(text);
    std::set<std::pair<std::string, std::string>> longs;   // (lo, hi)
    static const Regex dx_return(R"re((?m)^([ \t]*)dx = ([^;\n]+);\n\s*return ([^;\n]+);$)re");
    body = sub_all(dx_return, body, [&](const RegexMatch& m) {
        const std::string ind = str(m[1]), hi = str(m[2]), lo = str(m[3]);
        const auto var = [](sv s) -> std::optional<std::int64_t> {
            if (s.size() > 1 && s[0] == 'v' && std::ranges::all_of(s.substr(1), is_digit)) return to_int(s.substr(1));
            return std::nullopt;
        };
        const auto h = var(hi), l = var(lo);
        if (h && l && *h == *l - 2) {
            longs.insert({lo, hi});
            return ind + "return " + lo + ";";
        }
        const std::string stripped(ct::trim(lo, "()"));
        if (hi == "(0 - (" + lo + " >> 15))" || hi == "(0 - (" + stripped + " >> 15))" || hi == "0") return ind + "return " + lo + ";";
        return str(m.whole());
    });
    for (const auto& [lo, hi] : longs) {
        const Regex hi_decl(R"re((?m)^([ \t]*)int )re" + re_escape(hi) + ";\n");
        body = hi_decl.replace_all(body, "");
        const Regex lo_decl(R"re((?m)^([ \t]*)int )re" + re_escape(lo) + ";");
        body = lo_decl.replace_all(body, "$1long " + lo + ";");
    }
    static const Regex name_start(R"re((?m)^(\w+\())re");
    std::string head = head_in;
    if (const auto m = name_start.search(head)) head = head.substr(0, m->start()) + "long\n" + head.substr(m->start());
    return head + split.brace + body;
}

std::string local_pointers(std::string_view text_in) {
    static const Regex copy_then_sub(R"re((?m)^([ \t]*)(\w+) = bp;\n\s*\2 = \2 - (\d+);$)re");
    std::string text = copy_then_sub.replace_all(text_in, "$1$2 = bp - $3;");
    static const Regex place(R"re((?m)^[ \t]*(\w+) = bp - (\d+);)re");
    std::vector<std::pair<std::string, std::string>> given;
    place.for_each_match(text, [&](const RegexMatch& m) {
        given.emplace_back(str(m[1]), str(m[2]));
        return true;
    });
    static const Regex filler(R"re((?m)^[ \t]*char _p(\d+)\[(\d+)\];)re");
    for (const auto& [reg, n] : given) {
        std::optional<std::pair<std::string, std::string>> buffer;   // (the start of a filler the frame keeps: a buffer there)
        filler.for_each_match(text, [&](const RegexMatch& m) {
            if (to_int(m[1]) + to_int(m[2]) == to_int(n)) {
                buffer = {str(m[1]), str(m[2])};
                return false;
            }
            return true;
        });
        const std::string r = re_escape(reg);
        const Regex reg_decl(R"re((?m)^([ \t]*)(register )?int )re" + r + ";");
        if (buffer) {
            const Regex fill_decl(R"re((?m)^([ \t]*)char _p)re" + buffer->first + R"re(\[)re" + buffer->second + R"re(\];)re");
            text = fill_decl.replace_all(text, "$1char v" + n + "[" + buffer->second + "];");
            const Regex assign(R"re((?m)^([ \t]*))re" + r + " = bp - " + n + ";");
            text = assign.replace_all(text, "$1" + reg + " = v" + n + ";");
            text = reg_decl.replace_all(text, "$1$2char *" + reg + ";");
            continue;
        }
        const Regex local(R"re((?m)^[ \t]*(?:int|long) v)re" + n + ";");
        if (!local.search(text)) continue;
        const Regex assign(R"re((?m)^([ \t]*))re" + r + " = bp - " + n + ";");
        text = assign.replace_all(text, "$1" + reg + " = (char *)&v" + n + ";");
        const Regex field(R"re(\b)re" + r + R"re(->f(\d+)\b)re");
        text = field.replace_all(text, reg + "[$1]");
        text = reg_decl.replace_all(text, "$1$2char *" + reg + ";");
    }
    return text;
}

std::optional<std::string> void_variant(std::string_view text) {
    static const Regex returns(R"re((?m)^([ \t]*)return (.+);\s*$)re");
    std::optional<RegexMatch> last;
    returns.for_each_match(text, [&](const RegexMatch& m) {
        last = m;
        return true;
    });
    if (!last) return std::nullopt;
    static const Regex closes(R"re(\s*\}\s*$)re");
    if (!closes.match(text.substr(last->end())) && last->group(1).size() > 4) return std::nullopt;   // (a return inside the body, not its end: a value)
    const std::string value = str(last->group(2));
    return str(text.substr(0, last->start())) + (value.find('(') != npos ? str(last->group(1)) + value + ";" : std::string()) + str(text.substr(last->end()));
}

std::string long_ladders(std::string_view text_in) {
    std::string text = str(text_in);
    static const Regex long_decl(R"re((?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);)re");
    static const Regex high_low(R"re(\((\w+) ([<>])= (\w+)\))re");
    static const Regex halves(R"re(\(?\((\w+) ([<>]) (\w+)\) \|\| \((\w+) ([<>]=?) (\w+)\)\)?)re");
    static const Regex label_only(R"re(\s*L_\w+:)re");
    for (;;) {
        std::set<std::string> longs;
        long_decl.for_each_match(text, [&](const RegexMatch& m) {
            longs.insert(str(m[1]));
            return true;
        });
        if (longs.empty()) return text;
        const Lines lines = split_nl(text);
        bool rewritten = false;
        for (std::size_t k = 0; k < lines.size() && !rewritten; ++k) {
            const auto p = dt::if_parts(lines, k);
            if (!p || !p->else_arm) continue;
            const auto m = high_low.full_match(dt::strip_parens("(" + p->cond + ")"));
            if (!m || p->then_arm.empty()) continue;
            const auto q = dt::if_parts(p->then_arm, 0);
            if (!q || q->end != p->then_arm.size() || !q->else_arm) continue;
            const auto n = halves.full_match(q->cond);
            if (!n || n->group(1) != m->group(1) || n->group(2) != m->group(2) || n->group(3) != m->group(3)) continue;
            const std::string L = str(n->group(4)), L2 = str(n->group(6));
            const auto hi = longs_high_of_name(L);
            const auto hi2 = longs_high_of_name(L2);
            if (!longs.contains(L) || !hi || *hi != m->group(1) || (longs.contains(L2) && (!hi2 || *hi2 != m->group(3))) || n->group(5)[0] != m->group(2)[0])
                continue;
            const auto dedent_all = [](const std::vector<std::string>& ls) {
                Lines out;
                for (const std::string& l : ls) out.push_back(l.starts_with("    ") ? l.substr(4) : l);
                return out;
            };
            const Lines inner_else = dedent_all(*q->else_arm);
            if (!dt::same_arm(*p->else_arm, inner_else)) continue;
            const std::string ind = lead_ws(lines[k]);   // (\s* - the line's blanks)
            Lines made{std::format("{}if (({} {} {})) {{", ind, L, n->group(5), L2)};
            for (const std::string& l : dedent_all(q->then_arm)) made.push_back(l);
            made.push_back(ind + "} else {");
            for (const std::string& l : inner_else) made.push_back(l);
            made.push_back(ind + "}");
            Lines cand(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
            cand.insert(cand.end(), made.begin(), made.end());
            cand.insert(cand.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
            const std::string whole = join_nl(cand);
            Lines kept;
            for (const std::string& l : cand) {
                if (label_only.full_match(l)) {
                    const sv trimmed = ct::trim(l);
                    if (!shapes::scan::goes_to(whole, trimmed.substr(0, trimmed.size() - 1))) continue;   // (a label nothing goes to)
                }
                kept.push_back(l);
            }
            text = join_nl(kept);
            rewritten = true;
        }
        if (!rewritten) return text;
    }
}

std::string long_pairs(std::string_view text, State& state) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    std::string head = split.head;
    const std::string sep = split.brace;
    std::string body = split.body;
    dt::LongInfo& longinfo = state.facts.longs;
    const std::set<dt::Site>& byte_sites = state.facts.byte_sites;
    const std::set<std::string>& byte_globals = state.facts.byte_globals;

    std::set<std::string> pairs;
    // (a variable's own two words - not a field's)
    static const Regex long_cast(R"re(\*\(long \*\)&(\w+)\b(?!->|\[))re");
    long_cast.for_each_match(body, [&](const RegexMatch& m) {
        pairs.insert(str(m[1]));
        return true;
    });
    static const Regex long_macro(R"re(\bLONG\((\w+), (\w+)\))re");
    long_macro.for_each_match(body, [&](const RegexMatch& m) {
        pairs.insert(str(m[2]));
        return true;
    });
    static const Regex or_pair(R"re(\((\w+) \| (\w+)\))re");
    or_pair.for_each_match(body, [&](const RegexMatch& m) {   // (a pair's two words or-ed: tested as one long)
        const auto h = longs_high_of_name(m[1]);
        if (h && *h == m[2]) pairs.insert(str(m[1]));
        return true;
    });
    const auto hi_of = [](sv x) { return longs_high_of(x); };
    const auto add_field = [&](std::int64_t n) { longinfo.fields.insert(n); };
    const auto add_global = [&](std::int64_t place) { longinfo.globals.insert(longs_data_key(place)); };

    // (a long field updated: t = LONG(hi, lo) op E; lo = LO(t); hi = HI(t) - one assignment, op=)
    static const Regex long_update(R"re((?m)^([ \t]*)(t\d+) = LONG\((\w+)->f(\d+), (\*\3|\3->f(\d+))\) ([-+&|^]) (.+);\n[ \t]*\5 = LO\(\2\);\n[ \t]*\3->f\4 = HI\(\2\);$)re");
    body = sub_all(long_update, body, [&](const RegexMatch& m) {
        const std::int64_t nl = m[5].starts_with("*") ? 0 : to_int(m[6]);
        if (to_int(m[4]) != nl + 2) return str(m.whole());
        add_field(nl);
        return std::format("{}*(long *)&{}->f{} {}= {};", m[1], m[3], nl, m[7], m[8]);
    });

    // (a pair compared with a number the compiler's way, the high word first: the long compared)
    const auto long_eq = [&](const RegexMatch& m) {
        const sv h = m[1], l = m[3], op = m[4];
        const auto k = longs_c_number(m[5]);
        const auto hi = longs_high_of_name(l);
        if (!hi || *hi != h || !k || *k >= 0x8000) return str(m.whole());
        pairs.insert(str(l));
        return std::format("({} {} {})", l, op, *k);
    };
    static const Regex eq_or(R"re(\(\(?(\w+)(?: != (0))?\)? \|\| \((\w+) (!=) (\d+)\)\))re");
    body = sub_all(eq_or, body, long_eq);
    static const Regex eq_and(R"re(\(\((\w+) == (0)\) && \((\w+) (==) (\d+)\)\))re");
    body = sub_all(eq_and, body, long_eq);
    static const Regex eq_not(R"re(\(!(\w+)() && \((\w+) (==) (\d+)\)\))re");
    body = sub_all(eq_not, body, long_eq);

    // (a pair given a pair: `p->fN = x; p->fN+2 = y;` - a long field given a long - written as one store)
    static const Regex first_word(R"re((?m)^([ \t]*)\*(\w+) = (\w+);\n(\s*)\2->f2 = (\w+);$)re");   // (*p the first word: p->f0)
    body = sub_all(first_word, body, [&](const RegexMatch& m) {
        const auto h = hi_of(m[3]);
        if (h && *h == m[5]) return std::format("{}{}->f0 = {};\n{}{}->f2 = {};", m[1], m[2], m[3], m[4], m[2], m[5]);
        return str(m.whole());
    });
    static const Regex field_pair_line(R"re(([ \t]*)(\w+)->f(\d+) = (\w+);\n\s*(\w+)->f(\d+) = (\w+);)re");
    const std::string before_pairs = body;
    body = longs_line_pairs(body, field_pair_line, [&](const RegexMatch& m) {
        const std::string ind = str(m[1]), p = str(m[2]), n = str(m[3]), x = str(m[4]), p2 = str(m[5]), n2 = str(m[6]), y = str(m[7]);
        const bool byte = has_site(byte_sites, "*", to_int(n)) || has_site(byte_sites, "*", to_int(n2)) || has_site(byte_sites, p, to_int(n)) ||
                          has_site(byte_sites, p, to_int(n2));
        const std::string pn = p + "->f" + n, pn2 = p + "->f" + n2;
        bool alone = false;
        bool high_elsewhere = false;
        for (const std::string& l : split_nl(before_pairs)) {
            const bool has_low = has_word(l, pn), has_high = has_word(l, pn2);
            if (has_low && !has_high && !(ct::ltrim(l) == pn + " = " + x + ";" && l.find_first_not_of(" \t") == l.find(pn))) alone = true;
            if (has_high && !(l.find_first_not_of(" \t") == l.find(pn2) && ct::ltrim(l) == pn2 + " = " + y + ";")) high_elsewhere = true;
        }
        if (alone && !high_elsewhere) return str(m.whole());   // (the low word read alone, the high never: two ints)
        const auto hi = hi_of(x);
        if (p2 == p && to_int(n2) == to_int(n) + 2 && hi && *hi == y && !byte) {   // (words both: a byte is no long's)
            pairs.insert(x);
            add_field(to_int(n));
            return ind + pn + " = " + x + ";";
        }
        return str(m.whole());
    });

    // (a routine's long value, dx:ax, kept in a pair: the routine returns a long)
    const auto call_store = [&](const RegexMatch& m) {
        const std::string ind = str(m[1]), lo = str(m[2]), call = str(m[3]), hi = str(m[4]);
        const auto h = hi_of(lo);
        if (!h || *h != hi) return str(m.whole());
        pairs.insert(lo);
        static const Regex word(R"re(\w+)re");
        longinfo.funcs.insert(str(word.match(call)->whole()));
        const std::string next = str(m.group(5));
        return ind + lo + " = " + call + ";" + ct::replace_all(next, "(ax | dx)", lo);   // (the pair tested at once: the long)
    };
    // (a long passed: its two words pushed - the pair, or an int widened)
    const auto call_args = [&](const RegexMatch& m) {
        const std::vector<std::string> args = longs_args(m[2]);
        std::vector<std::string> out;
        for (std::size_t i = 0; i < args.size();) {
            const std::string& x = args[i];
            const std::string* y = i + 1 < args.size() ? &args[i + 1] : nullptr;
            if (y && longs_is_word(x)) {
                const auto h = hi_of(x);
                if (h && *y == *h && pairs.contains(x)) {   // (a long the code shows elsewhere)
                    out.push_back(x);
                    i += 2;
                    continue;
                }
            }
            if (y && (*y == "(0 - (" + x + " >> 15))" || *y == "(0 - ((" + x + ") >> 15))")) {
                out.push_back(longs_is_word(x) ? "(long)" + x : "(long)(" + x + ")");
                i += 2;
                continue;
            }
            out.push_back(x);
            ++i;
        }
        return str(m[1]) + "(" + ct::join(out, ", ") + ")";
    };

    // (the model's temporaries holding an int and its sign spread, read once by a call a line or two on: in their
    // place - the call passes a long; the sign in a register too)
    {
        static const Regex sign_temp(R"re([ \t]*(t\d+) = (\(0 - \((\w+) >> 15\)\)|(\w+));)re");
        static const Regex sign_reg(R"re([ \t]*(bx|cx|dx|di) = (\(0 - \((\w+) >> 15\)\));)re");
        static const Regex call_word(R"re(\w\()re");
        std::vector<std::optional<std::string>> bl;
        for (const std::string& l : split_nl(body)) bl.emplace_back(l);
        for (std::size_t k = 0; k < bl.size(); ++k) {
            const std::string line = bl[k].value_or("");
            std::string t, e, x;
            if (const auto m = sign_temp.full_match(line)) {
                t = str((*m)[1]);
                e = str((*m)[2]);
                x = m->has(3) ? str((*m)[3]) : str((*m)[4]);
            } else if (const auto m2 = sign_reg.full_match(line)) {
                t = str((*m2)[1]);
                e = str((*m2)[2]);
                x = str((*m2)[3]);
            } else {
                continue;
            }
            std::vector<std::size_t> uses;
            for (std::size_t j = k + 1; j < std::min(k + 4, bl.size()); ++j)
                if (bl[j] && has_word(*bl[j], t)) uses.push_back(j);
            if (uses.size() != 1) continue;
            const std::size_t u = uses[0];
            bool later = false;
            for (std::size_t j = u + 1; j < bl.size(); ++j) later = later || has_word(bl[j].value_or(""), t);
            bool given = false, braces = false;
            for (std::size_t j = k + 1; j < u; ++j) given = given || longs_gives_value(bl[j].value_or(""), x);
            for (std::size_t j = k + 1; j <= u; ++j) braces = braces || bl[j].value_or("").find_first_of("{}") != npos;   // (one block)
            if (later || count_word(*bl[u], t) != 1 || !call_word.search(*bl[u]) || given || braces) continue;
            bl[u] = replace_word(*bl[u], t, e);
            bl[k] = std::nullopt;
        }
        Lines live;
        for (const auto& l : bl)
            if (l) live.push_back(*l);
        body = join_nl(live);
    }
    {
        static const Regex temps_decl(R"re((?m)^[ \t]*int ((?:t\d+, )*t\d+);$)re");
        std::set<std::string> lists;
        temps_decl.for_each_match(body, [&](const RegexMatch& m) {
            lists.insert(str(m[1]));
            return true;
        });
        for (const std::string& list : lists) {
            for (const sv t1 : ct::split(list, ", ")) {
                std::string after = body;   // body.split("int " + t, 1)[-1].split(";", 1)[-1]
                if (const std::size_t at = after.find("int " + list); at != npos) after = after.substr(at + 4 + list.size());
                if (const std::size_t semi = after.find(';'); semi != npos) after = after.substr(semi + 1);
                if (!has_word(after, t1)) body = forms::drop_decl(body, t1);
            }
        }
    }
    static const Regex sign_long(R"re(\bLONG\(\(0 - \((\w+) >> 15\)\), \1\))re");
    body = sign_long.replace_all(body, "(long)$1");   // (an int's sign spread: widened)
    static const Regex zero_long(R"re(\bLONG\(0, (\w+)\))re");
    body = zero_long.replace_all(body, "(long)(unsigned)$1");
    static const Regex field_long(R"re(\bLONG\((\w+(?:->f\d+)*)->f(\d+), \1->f(\d+)\))re");
    body = sub_all(field_long, body, [&](const RegexMatch& m) {   // (a long field's two words: the field)
        if (to_int(m[2]) != to_int(m[3]) + 2) return str(m.whole());
        add_field(to_int(m[3]));
        return str(m[1]) + "->f" + str(m[3]);
    });
    if (longinfo.ret.value_or(false)) {   // (a long function's value its long's low word: the long)
        static const Regex low_of(R"re(\bLO\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re");
        body = low_of.replace_all(body, "($1)");
    }

    // (a pair given a pair: x = y, the high words too)
    static const Regex copy_pair(R"re(([ \t]*)(\w+) = (\w+);\n\s*(\w+) = (\w+);)re");
    body = longs_line_pairs(body, copy_pair, [&](const RegexMatch& m) {
        const std::string ind = str(m[1]), x = str(m[2]), y = str(m[3]), hx = str(m[4]), hy = str(m[5]);
        const auto byte_data = [&](const std::string& g) {   // (bytes of the data given: no long's words)
            if (!longs_is_data_name(g)) return false;
            const std::string key = longs_data_key(longs_data_place(g));
            return std::ranges::any_of(byte_globals, [&](const std::string& b) { return longs_is_data_name(b) && longs_data_key(longs_data_place(b)) == key; });
        };
        if (byte_data(x) || byte_data(hx) || byte_data(y) || byte_data(hy)) return str(m.whole());
        const auto hix = hi_of(x), hiy = hi_of(y);
        if (hix && *hix == hx && hiy && *hiy == hy) {
            pairs.insert(x);
            pairs.insert(y);
            return ind + x + " = " + y + ";";
        }
        const bool x_high = (hix && *hix == hx) ||
                            (longs_is_data_name(x) && (hx.starts_with("g_") ? ct::parse_int<std::int64_t>(sv(hx).substr(2), 16) : std::optional<std::int64_t>(0)) ==
                                                          std::optional<std::int64_t>(longs_data_place(x) + 2));
        if (x_high && longs_is_data_name(y) && longs_is_data_name(hy) && longs_data_place(hy) == longs_data_place(y) + 2) {   // (a long in the data copied: one long)
            pairs.insert(x);
            add_global(longs_data_place(y));
            if (longs_is_data_name(x)) add_global(longs_data_place(x));
            return ind + x + " = " + y + ";";
        }
        return str(m.whole());
    });

    // (a pair given two numbers: one long number)
    static const Regex number_pair(R"re(([ \t]*)(\w+) = (-?\d+);\n\s*(\w+) = (-?\d+);)re");
    body = longs_line_pairs(body, number_pair, [&](const RegexMatch& m) {
        const std::string x = str(m[2]), hx = str(m[4]);
        const auto h = hi_of(x);
        if (!h || *h != hx || x.starts_with("g_")) return str(m.whole());   // (two words of the data given numbers: two ints)
        const auto k1 = ct::parse_int<std::int64_t>(m[3]), k2 = ct::parse_int<std::int64_t>(m[5]);
        if (!k1 || !k2) return str(m.whole());
        pairs.insert(x);
        std::int64_t v = ((*k2 & 0xFFFF) << 16) | (*k1 & 0xFFFF);
        if (v & 0x80000000LL) v -= (1LL << 32);
        return str(m[1]) + x + " = " + (std::abs(v) > 0x7FFF ? std::format("{}L", v) : std::to_string(v)) + ";";
    });

    // (two words' expressions, the low and the high, as the one long expression they are, or none: names, fields and
    // data in pairs, ~, & | ^, a number masking a half)
    std::function<std::optional<std::string>(sv, sv)> long_of = [&](sv lo_in, sv hi_in) -> std::optional<std::string> {
        const auto bare = [](const std::string& x) {
            return x.starts_with("(") && x.ends_with(")") && dt::balanced(sv(x).substr(1, x.size() - 2)) ? x.substr(1, x.size() - 2) : x;
        };
        std::string lo = bare(dt::strip_parens(ct::trim(lo_in))), hi = bare(dt::strip_parens(ct::trim(hi_in)));
        static const Regex field(R"re((\w+)->f(\d+))re");
        const auto fl = field.full_match(lo), fh = field.full_match(hi);
        if (fl && fh && (*fl)[1] == (*fh)[1] && to_int((*fh)[2]) == to_int((*fl)[2]) + 2) {
            add_field(to_int((*fl)[2]));
            return lo;
        }
        if (longs_is_data_name(lo) && longs_is_data_name(hi) && longs_data_place(hi) == longs_data_place(lo) + 2) {
            add_global(longs_data_place(lo));
            return lo;
        }
        if (longs_is_word(lo)) {
            const auto h = hi_of(lo);
            if (h && *h == hi) {
                pairs.insert(lo);
                return lo;
            }
        }
        if (lo.starts_with("~") && hi.starts_with("~")) {
            const auto x = long_of(sv(lo).substr(1), sv(hi).substr(1));
            return x ? std::optional<std::string>("(~" + *x + ")") : std::nullopt;
        }
        std::function<std::vector<std::string>(const std::string&, const std::string&)> flat = [&](const std::string& x, const std::string& op) {   // (a & (b & c): a, b, c)
            std::vector<std::string> out;
            for (const std::string& part : dt::top_split(x, op)) {
                const std::string p_ = std::string(ct::trim(part));
                const std::string b_ = bare(p_);
                if (b_ != p_ && dt::top_split(b_, op).size() > 1) {
                    for (std::string& s : flat(b_, op)) out.push_back(std::move(s));
                } else {
                    out.push_back(p_);
                }
            }
            return out;
        };
        static const Regex mask_number(R"re(\d+|0x[0-9a-f]+)re");
        for (const std::string op : {"&", "|", "^"}) {
            const std::vector<std::string> pl = flat(lo, op), ph = flat(hi, op);
            if (ph.size() > 1 && ph.size() == pl.size() + 1 && mask_number.full_match(ct::trim(ph.back())) && op == "&") {
                std::vector<std::string> low_parts = pl, high_parts(ph.begin(), ph.end() - 1);
                const auto x = long_of(pl.size() > 1 ? ct::join(low_parts, " & ") : pl[0], ph.size() > 2 ? ct::join(high_parts, " & ") : ph[0]);
                const auto k = longs_c_number(ct::trim(ph.back()));
                if (!x || !k) return std::nullopt;
                return std::format("({} & 0x{:04x}ffffL)", *x, *k & 0xFFFF);
            }
            if (pl.size() > 1 && pl.size() == ph.size()) {
                std::vector<std::string> xs;
                for (std::size_t i = 0; i < pl.size(); ++i) {
                    const auto x = long_of(pl[i], ph[i]);
                    if (!x) return std::nullopt;
                    xs.push_back(*x);
                }
                return "(" + ct::join(xs, " " + std::string(op) + " ") + ")";
            }
        }
        return std::nullopt;
    };

    // ((lo) | (hi)) != 0: a long tested
    static const Regex long_test_re(R"re(\((\((?:[^()]|\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\))*\)) (!=|==) 0\))re");
    body = sub_all(long_test_re, body, [&](const RegexMatch& m) {
        const std::string inner_text = str(m[1].substr(1, m[1].size() - 2));
        std::vector<std::string> parts;
        if (dt::balanced(inner_text)) parts = dt::top_split(inner_text, "|");
        if (parts.size() != 2 || !dt::balanced(parts[0]) || !dt::balanced(parts[1])) return str(m.whole());
        const auto x = long_of(parts[0], parts[1]);
        return x ? "(" + *x + " " + str(m[2]) + " 0)" : str(m.whole());
    });

    // (a long field given a long a word at a time - another long field's, a long in the data, ..)
    static const Regex field_line(R"re(([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);\n\s*(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);)re");
    body = longs_line_pairs(body, field_line, [&](const RegexMatch& m) {
        const std::string ind = str(m[1]), p = str(m[2]), n = str(m[3]), x = str(m[4]), p2 = str(m[5]), n2 = str(m[6]), y = str(m[7]);
        static const Regex variable(R"re([a-z]|v\d+)re");
        if (p2 != p || to_int(n2) != to_int(n) + 2 || variable.full_match(ct::trim(x))) return str(m.whole());   // (two variables: the pair store's, with its guards)
        const auto lx = long_of(x, y);
        if (!lx) return str(m.whole());
        add_field(to_int(n));
        return ind + p + "->f" + n + " = " + *lx + ";";
    });

    const auto long_field = [&](sv p, sv n) {   // (a long field: offset n of what p points to)
        add_field(to_int(n));
        return str(p) + "->f" + str(n);
    };
    static const Regex both_complemented(R"re((?m)^([ \t]*)(\w+)->f(\d+) = \(~\2->f\3\);\n[ \t]*\2->f(\d+) = \(~\2->f\4\);$)re");
    body = sub_all(both_complemented, body, [&](const RegexMatch& m) {
        if (to_int(m[4]) != to_int(m[3]) + 2) return str(m.whole());
        return str(m[1]) + long_field(m[2], m[3]) + " = ~" + long_field(m[2], m[3]) + ";";
    });
    static const Regex int_widened(R"re((?m)^([ \t]*)(\w+)->f(\d+) = (\w+);\n[ \t]*\2->f(\d+) = \(0 - \(\4 >> 15\)\);$)re");
    body = sub_all(int_widened, body, [&](const RegexMatch& m) {
        if (to_int(m[5]) != to_int(m[3]) + 2) return str(m.whole());
        return str(m[1]) + long_field(m[2], m[3]) + " = " + str(m[4]) + ";";
    });
    static const Regex words_tested(R"re(\((\w+(?:->f\d+)*)->f(\d+) \| \1->f(\d+)\))re");
    body = sub_all(words_tested, body, [&](const RegexMatch& m) {   // (its words tested together)
        return to_int(m[3]) == to_int(m[2]) + 2 ? long_field(m[1], m[2]) : str(m.whole());
    });
    static const Regex read_whole(R"re(\*\(long \*\)&(\w+(?:->f\d+)*)->f(\d+)\b)re");
    body = sub_all(read_whole, body, [&](const RegexMatch& m) { return long_field(m[1], m[2]); });

    // (a long field's two words pushed together: the field)
    const std::string before_fields = body;
    static const Regex field_ref(R"re((\w+(?:->f\d+)*)->f(\d+))re");
    body = sub_all(longs_call_regex(), body, [&](const RegexMatch& m) {
        const std::vector<std::string> args = longs_args(m[2]);
        std::vector<std::string> out;
        for (std::size_t i = 0; i < args.size();) {
            const auto a1 = field_ref.full_match(args[i]);
            const auto a2 = i + 1 < args.size() ? field_ref.full_match(args[i + 1]) : std::nullopt;
            bool together = false;   // (the high word never alone)
            if (a1 && i + 1 < args.size())
                together = longs_count_bounded(before_fields, args[i + 1], true) == longs_count_bounded(before_fields, args[i] + ", " + args[i + 1], false);
            if (a1 && a2 && (*a1)[1] == (*a2)[1] && to_int((*a2)[2]) == to_int((*a1)[2]) + 2 && together &&
                !has_site(byte_sites, (*a1)[1], to_int((*a1)[2])) && longinfo.fields.contains(to_int((*a1)[2]))) {
                out.push_back(long_field((*a1)[1], (*a1)[2]));
                i += 2;
                continue;
            }
            out.push_back(args[i]);
            ++i;
        }
        return str(m[1]) + "(" + ct::join(out, ", ") + ")";
    });

    // (a long and-ed with a long field and tested: the high words in dx first, then the low words or-ed in)
    static const Regex masked(R"re((?m)^([ \t]*)dx = (\w+) & (\w+)->f(\d+);\n(.*\(\((\w+) & \3->f(\d+)\) \| dx\).*)$)re");
    body = sub_all(masked, body, [&](const RegexMatch& m) {
        const std::string hi = str(m[2]), p = str(m[3]), rest = str(m[5]), lo = str(m[6]);
        const auto h = hi_of(lo);
        if (!h || *h != hi || to_int(m[4]) != to_int(m[7]) + 2) return str(m.whole());
        pairs.insert(lo);
        add_field(to_int(m[7]));
        return ct::replace_all(rest, "((" + lo + " & " + p + "->f" + str(m[7]) + ") | dx)", "(" + lo + " & " + p + "->f" + str(m[7]) + ")");
    });

    // (a long in the data and-ed, or-ed, xor-ed with a long: a word at a time, the high words after - one op)
    static const Regex data_op(R"re(([ \t]*)(g_[0-9a-f]+) = \2 ([|&^]) ([^;\n]+);\n\s*(g_[0-9a-f]+) = \5 ([|&^]) ([^;\n]+);)re");
    body = longs_line_pairs(body, data_op, [&](const RegexMatch& m) {
        if (m[3] != m[6] || longs_data_place(m[5]) != longs_data_place(m[2]) + 2) return str(m.whole());
        const auto lx = long_of(m[4], m[7]);
        if (!lx) return str(m.whole());
        add_global(longs_data_place(m[2]));
        return str(m[1]) + str(m[2]) + " = " + str(m[2]) + " " + str(m[3]) + " " + *lx + ";";
    });

    // (a pair given an int and its sign spread: the int widened to the long)
    OrderedMap<std::string, std::string> widened;   // (a long given an int: its low word that int)
    static const Regex sign_pair(R"re((?m)^([ \t]*)(\w+) = ([^;\n]+);\n[ \t]*(\w+) = \(0 - \((.+) >> 15\)\);$)re");
    body = sub_all(sign_pair, body, [&](const RegexMatch& m) {
        const std::string x = str(m[2]), e = str(m[3]);
        const auto h = hi_of(x);
        const std::string e2(ct::trim(m[5]));
        const std::string spread = std::string(ct::trim(m[5]));
        // the second statement's value is (0 - (E2 >> 15)): its E2 must be E (or (E))
        if (!h || *h != m[4] || (spread != e && spread != "(" + e + ")")) return str(m.whole());
        pairs.insert(x);
        static const Regex name(R"re([A-Za-z_]\w*)re");
        if (name.full_match(e)) widened.insert_or_assign(x, e);
        return str(m[1]) + x + " = " + e + ";";
    });
    for (const auto& [x, e] : widened.entries()) {   // (its one word tested - the int's, still in a register)
        std::string after = body;
        if (const std::size_t at = after.find(x + " = " + e + ";"); at != npos) after = after.substr(at + (x + " = " + e + ";").size());
        const Regex given(R"re((?m)^[ \t]*(?:)re" + re_escape(x) + "|" + re_escape(e) + ") = ");
        if (!given.search(after)) {
            const Regex tested(R"re(\()re" + re_escape(x) + R"re( (==|!=) 0\))re");
            body = tested.replace_all(body, "(" + e + " $1 0)");
        }
    }

    // (both halves given 0, or -1: the long 0, -1)
    static const Regex zero_pair(R"re((?m)^([ \t]*)(\w+) = (0|-1);\n\s*(\w+) = \2;$)re");
    body = sub_all(zero_pair, body, [&](const RegexMatch& m) {
        const std::string a = str(m[2]), b = str(m[4]);
        const auto hb = hi_of(b), ha = hi_of(a);
        std::string lo;
        if (hb && *hb == a) lo = b;
        else if (ha && *ha == b) lo = a;
        if (lo.empty() || lo.starts_with("g_")) return str(m.whole());
        pairs.insert(lo);
        return str(m[1]) + lo + " = " + str(m[3]) + ";";
    });

    // (a routine's dx:ax read as one long by the next statement)
    static const Regex ax_call(R"re((?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n(.*\bLONG\(dx, ax\).*)$)re");
    body = sub_all(ax_call, body, [&](const RegexMatch& m) {
        static const Regex word(R"re(\w+)re");
        longinfo.funcs.insert(str(word.match(m[2])->whole()));
        return ct::replace_all(m[3], "LONG(dx, ax)", m[2]);
    });

    // (a long given to a pair a half at a time: one store)
    const auto halves_of = [&](sv ind, sv e, sv lo, sv hi, sv whole) {
        const auto h = hi_of(lo);
        if (!h || *h != hi) return str(whole);
        pairs.insert(str(lo));
        return str(ind) + str(lo) + " = " + str(e) + ";";
    };
    static const Regex temp_halves(R"re((?m)^([ \t]*)(t\d+) = (.+);\n\s*(\w+) = LO\(\2\);\n\s*(\w+) = HI\(\2\);$)re");
    body = sub_all(temp_halves, body, [&](const RegexMatch& m) { return halves_of(m[1], m[3], m[4], m[5], m.whole()); });
    static const Regex dx_halves(R"re((?m)^([ \t]*)dx = HI\((.+)\);\n[ \t]*(\w+) = LO\(\2\);\n[ \t]*(\w+) = dx;$)re");   // (the high half kept in dx first)
    body = sub_all(dx_halves, body, [&](const RegexMatch& m) {
        const auto h = hi_of(m[3]);
        if (!h || *h != m[4]) return str(m.whole());
        pairs.insert(str(m[3]));
        return str(m[1]) + str(m[3]) + " = " + str(m[2]) + ";";
    });
    static const Regex field_halves(R"re((?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = LO\((.+)\);\n[ \t]*\2->f(\d+) = HI\(\4\);$)re");   // (a long field given a long)
    body = sub_all(field_halves, body, [&](const RegexMatch& m) {
        if (to_int(m[5]) != to_int(m[3]) + 2) return str(m.whole());
        return str(m[1]) + long_field(m[2], m[3]) + " = " + str(m[4]) + ";";
    });
    static const Regex plain_halves(R"re((?m)^([ \t]*)()(\w+) = LO\((.+)\);\n\s*(\w+) = HI\(\4\);$)re");
    body = sub_all(plain_halves, body, [&](const RegexMatch& m) { return halves_of(m[1], m[4], m[3], m[5], m.whole()); });
    static const Regex through_ax(R"re((?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = ax;\n(?=\s*\w+ = dx;))re");   // (through ax: the same - when dx goes with it)
    body = sub_all(through_ax, body, [](const RegexMatch& m) { return str(m[1]) + str(m[3]) + " = " + str(m[2]) + ";\n"; });
    static const Regex call_then_dx(R"re((?m)^([ \t]*)(\w+) = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = dx;(\n.*)?$)re");
    body = sub_all(call_then_dx, body, call_store);
    for (const std::string& g : pairs)   // (data a pair is made of: a long there)
        if (longs_is_data_name(g)) add_global(longs_data_place(g));

    // (two words of the data pushed together, the high one first: a long there - the same pushes)
    body = sub_all(longs_call_regex(), body, call_args);
    body = sub_all(longs_call_regex(), body, [&](const RegexMatch& m) {
        const std::vector<std::string> args = longs_args(m[2]);
        std::vector<std::string> out;
        for (std::size_t i = 0; i < args.size();) {
            if (i + 1 < args.size() && longs_is_data_name(args[i])) {
                const auto h = hi_of(args[i]);
                if (h && args[i + 1] == *h) {
                    add_global(longs_data_place(args[i]));
                    out.push_back(args[i]);
                    i += 2;
                    continue;
                }
            }
            out.push_back(args[i]);
            ++i;
        }
        return str(m[1]) + "(" + ct::join(out, ", ") + ")";
    });

    static const Regex signature(R"re((?m)^\w+\(([\w, ]*)\))re");
    std::vector<std::string> plist;
    {
        std::optional<std::string> last_params;
        signature.for_each_match(head, [&](const RegexMatch& m) {
            last_params = str(m[1]);
            return true;
        });
        if (last_params)
            for (const sv p : ct::split(*last_params, ','))
                if (!ct::trim(p).empty()) plist.emplace_back(ct::trim(p));
    }
    static const Regex long_local(R"re((?m)^[ \t]*(?:unsigned )?long (v\d+);$)re");
    {
        std::vector<std::string> made;
        long_local.for_each_match(body, [&](const RegexMatch& m) {
            made.push_back(str(m[1]));
            return true;
        });
        const std::size_t brace = body.find('{');
        const sv after_brace = brace == npos ? sv(body) : sv(body).substr(brace + 1);   // (a long a pass made, its high word still read by name: the pair)
        for (const std::string& x : made)
            if (const auto h = hi_of(x); h && has_word(after_brace, *h)) pairs.insert(x);
    }
    for (const std::string& lo : std::vector<std::string>(pairs.begin(), pairs.end())) {
        const auto hi_opt = hi_of(lo);
        if (!hi_opt) continue;
        const std::string hi = *hi_opt;
        const std::string elo = re_escape(lo), ehi = re_escape(hi);
        const bool isparam = std::ranges::find(plist, lo) != plist.end() && std::ranges::find(plist, hi) != plist.end();
        bool islocal = false;
        {
            const Regex lo_decl(R"re((?m)^[ \t]*(?:int|long) (?:\w+, )*)re" + elo + R"re(\b)re");
            if (lo_decl.search(body)) {
                const Regex hi_decl(R"re((?m)^[ \t]*int (?:\w+, )*)re" + ehi + R"re(\b)re");
                const Regex lo_long(R"re((?m)^[ \t]*long )re" + elo + ";");
                islocal = hi_decl.search(body) || lo_long.search(body);
            }
        }
        if (!isparam && !islocal) continue;
        const std::string uns = longinfo.unsigned_names.contains(lo) ? "unsigned long" : "long";
        body = Regex(R"re(\*\(long \*\)&)re" + elo + R"re(\b)re").replace_all(body, lo);
        body = Regex(R"re(\bLONG\()re" + ehi + ", " + elo + R"re(\))re").replace_all(body, lo);
        body = Regex(R"re(\()re" + elo + R"re( \| )re" + ehi + R"re(\)|\()re" + ehi + R"re( \| )re" + elo + R"re(\))re").replace_all(body, lo);
        body = Regex(R"re(\()re" + ehi + R"re( (<|>=) 0\))re").replace_all(body, "(" + lo + " $1 0)");   // (the high word's sign: the long's)
        for (const std::string& lo2 : std::vector<std::string>(pairs.begin(), pairs.end())) {   // (two longs compared a half at a time)
            const auto hi2_opt = hi_of(lo2);
            if (lo2 == lo || !hi2_opt) continue;
            const std::string hi2 = *hi2_opt;
            body = ct::replace_all(body, "(" + hi + " != " + hi2 + ") || (" + lo + " != " + lo2 + ")", "(" + lo + " != " + lo2 + ")");
            body = ct::replace_all(body, "(" + hi + " == " + hi2 + ") && (" + lo + " == " + lo2 + ")", "(" + lo + " == " + lo2 + ")");
            body = ct::replace_all(body, "((" + hi + " & " + hi2 + ") == " + hi2 + ") && ((" + lo + " & " + lo2 + ") == " + lo2 + ")",
                                   "((" + lo + " & " + lo2 + ") == " + lo2 + ")");
            // (one masked with the other and compared with it: dx the high words' and)
            const Regex masked_pair(R"re((?m)^([ \t]*)dx = )re" + ehi + " & " + re_escape(hi2) + R"re(;\n([ \t]*if \(.*)\(dx == )re" + re_escape(hi2) +
                                    R"re(\) && \(\()re" + elo + " & " + re_escape(lo2) + R"re(\) == )re" + re_escape(lo2) + R"re(\))re");
            body = masked_pair.replace_all(body, "$2((" + lo + " & " + lo2 + ") == " + lo2 + ")");
        }
        const Regex field_read(R"re((?m)^([ \t]*))re" + elo + R"re( = (\w+(?:->f\d+)*)->f(\d+);\n[ \t]*)re" + ehi + R"re( = \2->f(\d+);$)re");
        body = sub_all(field_read, body, [&](const RegexMatch& m) {   // (a long field read)
            return to_int(m[4]) == to_int(m[3]) + 2 ? str(m[1]) + lo + " = *(long *)&" + str(m[2]) + "->f" + str(m[3]) + ";" : str(m.whole());
        });
        const Regex high_mask(R"re(\()re" + ehi + R"re( & (-?\d+)\))re");
        body = sub_all(high_mask, body, [&](const RegexMatch& m) { return std::format("({} & 0x{:x}L)", lo, (to_int(m[1]) & 0xFFFF) << 16); });
        if (isparam) {
            plist.erase(std::remove(plist.begin(), plist.end(), hi), plist.end());
            static const Regex own_signature(R"re((?m)^(\w+)\([\w, ]*\))re");
            if (const auto m = own_signature.search(head)) head = head.substr(0, m->start()) + str((*m)[1]) + "(" + ct::join(plist, ", ") + ")" + head.substr(m->end());
            head = forms::drop_decl(head, hi);
            head = forms::redeclare(head, lo, uns + " " + lo).value_or(head);
        } else {
            body = forms::drop_decl(body, hi);
            body = forms::redeclare(body, lo, uns + " " + lo).value_or(body);
        }
    }
    std::set<std::string> longs_;
    {
        static const Regex long_names(R"re((?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);)re");
        long_names.for_each_match(body, [&](const RegexMatch& m) {
            longs_.insert(str(m[1]));
            return true;
        });
    }
    // (a long value - one of a long's - stored a word at a time into a field, dx the high one)
    const auto long_store = [&](sv whole, sv ind, sv e, sv p, sv n_text, sv op, sv n2_text) {
        const std::int64_t n = to_int(n_text), n2 = to_int(n2_text);
        if (n2 != n + 2 || !std::ranges::any_of(longs_, [&](const std::string& x) { return has_word(e, x); })) return str(whole);
        add_field(n);
        return std::format("{}*(long *)&{}->f{} = {}{};", ind, p, n, op, op.empty() || e.starts_with("(") ? str(e) : "(" + str(e) + ")");
    };
    if (!longs_.empty()) {
        static const Regex ax_store(R"re((?m)^([ \t]*)ax = (.+);\n[ \t]*(\w+(?:->f\d+)*)->f(\d+) = \(?(~?)ax\)?;\n[ \t]*\3->f(\d+) = \(?~?dx\)?;$)re");
        body = sub_all(ax_store, body, [&](const RegexMatch& m) { return long_store(m.whole(), m[1], m[2], m[3], m[4], m[5], m[6]); });
        static const Regex dx_store(R"re((?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = (\(.+\));\n[ \t]*\2->f(\d+) = dx;$)re");
        body = sub_all(dx_store, body, [&](const RegexMatch& m) { return long_store(m.whole(), m[1], m[4], m[2], m[3], "", m[5]); });
    }
    static const Regex returned_name(R"re((?m)^[ \t]*return (\w+);)re");
    bool long_returned = false;
    returned_name.for_each_match(body, [&](const RegexMatch& m) {
        const Regex as_long(R"re((?m)^[ \t]*(?:unsigned )?long (?:\w+, )*)re" + re_escape(m[1]) + R"re(\b)re");
        if (as_long.search(body)) long_returned = true;
        return !long_returned;
    });
    static const Regex returns_call(R"re((?m)^[ \t]*return [^;]*\w\()re");
    if (long_returned && !(longinfo.ret.has_value() && !*longinfo.ret) && !returns_call.search(body)) longinfo.ret = true;   // (a long local returned: the function's a long)
    if (longinfo.ret.value_or(false)) {
        static const Regex name_start(R"re((?m)^\w+\()re");
        static const Regex already_long(R"re((?m)^(?:unsigned )?long\s*$)re");
        if (const auto m = name_start.search(head); m && !already_long.search(head)) head = head.substr(0, m->start()) + "long\n" + head.substr(m->start());
        static const Regex field_returned(R"re((?m)^[ \t]*return \w+(?:->f\d+)*->f(\d+);$)re");   // (a field returned whole: a long one)
        field_returned.for_each_match(body, [&](const RegexMatch& m) {
            add_field(to_int(m[1]));
            return true;
        });
    }
    return head + sep + body;
}

}  // namespace cb::exact::draft::candidates
