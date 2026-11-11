// cb-unity: alone (file-local helpers shared by name with the other shapes_*.cpp files)
// The passes that read back the values the compiler leaves behind a test or a register: a comparison's 1 and 0, the
// ?: of two arms, a call's value kept in ax, a value stored and read again.
#include <algorithm>
#include <array>

#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/shapes.hpp"
#include "cb/exact/draft/shapes_scan.hpp"

namespace cb::exact::draft::shapes {
namespace {

namespace dt = cb::exact::draft::text;
using namespace scan;
using cb::Regex;
using cb::RegexFlags;
using cb::RegexMatch;
using std::string;
using std::string_view;

constexpr RegexFlags kLines = RegexFlags::Multiline;

string str(string_view s) { return string(s); }

// The text between the matches' own parentheses, as the passes print a test: ((X)) once.
string strip(string_view c) { return dt::strip_parens(c); }
string truth(string_view c) { return dt::truth(c); }
string negate(string_view c) { return dt::negate(c); }

// `\w+(?:->f\d+)*|\*\w+`: a name, a field of a struct, or what a pointer points to.
bool is_place(string_view s) {
    if (s.starts_with('*')) return s.size() > 1 && std::ranges::all_of(s.substr(1), is_word_char);
    std::size_t i = 0;
    while (i < s.size() && is_word_char(s[i])) ++i;
    if (i == 0) return false;
    while (i < s.size()) {
        if (!s.substr(i).starts_with("->f")) return false;
        i += 3;
        const std::size_t digits = i;
        while (i < s.size() && s[i] >= '0' && s[i] <= '9') ++i;
        if (i == digits) return false;
    }
    return true;
}

// `<ind>PLACE = <reg>;` -> PLACE-ish text between: `<ind>([^;=]+?) = REG;` (the middle holds neither ; nor =).
std::optional<string_view> stored_from(string_view line, string_view ind, string_view reg) {
    const string tail = cat(" = ", reg, ";");
    if (!line.starts_with(ind) || !line.ends_with(tail) || line.size() < ind.size() + tail.size() + 1) return std::nullopt;
    const string_view mid = line.substr(ind.size(), line.size() - ind.size() - tail.size());
    if (mid.find_first_of(";=") != string_view::npos) return std::nullopt;
    return mid;
}

}  // namespace

// ---- a test's value --------------------------------------------------------------------------------------------

string compare_values(string_view text) {
    static const Regex byte_choice(
        R"re(^([ \t]*)if \((.*)\) \{\n\s*al = (-?\d+);\n\s*\} else \{\n\s*al = (-?\d+);\n\s*\}\n\s*ah = 0;$)re", kLines);
    static const Regex word_choice(
        R"re(^([ \t]*)if \((.*)\) \{\n\s*(ax|al|bx|cx|dx) = ([01]);\n\s*\} else \{\n\s*\3 = ([01]);\n\s*\}$)re", kLines);
    // (a byte given one or the other, then widened with zeros: a char's ?: - an int's would be given in ax)
    const string widened = byte_choice.replace_all(text, [](const RegexMatch& m) {
        return cat(m.group(1), "ax = ((unsigned char)(", strip(cat("(", m.group(2), ")")), " ? ", m.group(3), " : ", m.group(4), "));");
    });
    return word_choice.replace_all(widened, [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), c = m.group(2), r = m.group(3), a = m.group(4), b = m.group(5);
        if (a == "1" && b == "0") return cat(ind, r, " = ", c.starts_with("(") ? str(c) : cat("(", c, ")"), ";");
        if (a == "0" && b == "1") return cat(ind, r, " = !(", c, ");");
        return str(m.group(0));
    });
}

string logical_values(string_view text) {
    static const Regex or_form(
        R"re(^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(L_\w+):\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*goto \4;\n\s*\}$)re",
        kLines);
    // (if (A) { if (B) { r = 1; } else { L: r = 0; } } else { goto L; }  ->  r = A && B)
    static const Regex and_label(
        R"re(^([ \t]*)if (\(.*\)) \{\n\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*(L_\w+):\n\s*\4 = 0;\n\s*\}\n\s*\} else \{\n\s*goto \5;\n\s*\}$)re",
        kLines);
    static const Regex and_both(
        R"re(^([ \t]*)if (\(.*\)) \{\n((?:\s*(?:ax|bx|cx|dx|si|di) = [^;\n]+;\n)*)\s*if (\(.*\)) \{\n\s*(\w+) = 1;\n\s*\} else \{\n\s*\5 = 0;\n\s*\}\n\s*\} else \{\n\s*\5 = 0;\n\s*\}$)re",
        kLines);
    static const Regex given(R"re((\w+) = ([^;\n]+);)re");
    string t = or_form.replace_all(text, [](const RegexMatch& m) {
        return cat(m.group(1), m.group(5), " = ", negate(m.group(2)), " || ", truth(m.group(3)), ";");
    });
    t = and_label.replace_all(t, [](const RegexMatch& m) {
        return cat(m.group(1), m.group(4), " = ", truth(m.group(2)), " && ", truth(m.group(3)), ";");
    });
    return and_both.replace_all(t, [](const RegexMatch& m) {
        string b = str(m.group(4));
        // (the temporaries the second test reads)
        given.for_each_match(m.group(3), [&](const RegexMatch& g) {
            const string_view value = g.group(2);
            const bool plain = std::ranges::all_of(value, is_word_char);
            b = replace_word_all(b, g.group(1), plain ? str(value) : cat("(", value, ")"));
            return true;
        });
        return cat(m.group(1), m.group(5), " = ", truth(m.group(2)), " && ", truth(b), ";");
    });
}

string ternary(string_view c, string_view a, string_view b) {
    static const Regex choice(R"re(\((.*) \? (.*) : (.*)\))re");
    struct Side {
        string_view inner, other;
        bool or_not;
    };
    for (const Side& side : {Side{a, b, true}, Side{b, a, false}}) {
        const auto m = choice.full_match(side.inner);
        if (!m || !dt::balanced(m->group(1)) || !dt::balanced(m->group(2))) continue;
        const string test = cat("(", c, ")"), inner_test = cat("(", m->group(1), ")");
        if (m->group(2) == side.other) {
            const string first = side.or_not ? truth(negate(test)) : truth(test);
            return cat(dt::join_tests("||", first, truth(inner_test)), " ? ", m->group(2), " : ", m->group(3));
        }
        if (m->group(3) == side.other) {   // (C ? (C2 ? X : Y) : Y: C && C2 ? X : Y)
            const string first = side.or_not ? truth(test) : truth(negate(test));
            return cat(dt::join_tests("&&", first, truth(inner_test)), " ? ", m->group(2), " : ", m->group(3));
        }
    }
    return cat(truth(c), " ? ", a, " : ", b);
}

string step_tests(string_view text) {
    static const Regex stepped(
        R"re(^([ \t]*)([abcd][lx]) = \((.+) ([+-]) (\d+)\);\n([ \t]*(?:if|while) \(\()\(?\2(?: & 0xff)?\)? (==|!=) 0\)\)(.*)$)re", kLines);
    return stepped.replace_all(text, [](const RegexMatch& m) -> string {
        const auto k = number(m.group(5));
        if (!k) return str(m.group(0));
        const std::int64_t v = m.group(4) == "+" ? -*k : *k;
        return cat(m.group(6), m.group(3), " ", m.group(7), " ", std::to_string(v), "))", m.group(8));
    });
}

string conditional_values(string_view text_in) {
    static const Regex arms(
        R"re(^([ \t]*)if (\(.*\)) \{\n\s*(ax|bx|cx|dx|si|di|al|bl|cl|dl) = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\s*\}\n(\s*)(.*)$)re",
        kLines);
    static const Regex statement_start(R"re((?:if|while|for|switch|\}|L_\w+:))re");
    // (the value given first and given again when a test holds - r = A; if (C) { r = B; }: C ? B : A)
    static const Regex given_twice(
        R"re(^([ \t]*)(ax|bx|cx|dx) = ([^;\n]+);\n\1if (\(.*\)) \{\n\s*\2 = ([^;\n]+);\n\1\}\n\1(.*)$)re", kLines);
    // (nested: an arm that is itself such a test - `else { if (C2) { r = B; } else { r = D; } }` - one value first,
    //  `r = (C2 ? B : D);`, then the outer test)
    static const Regex nested(
        R"re(^([ \t]*)if (\(.*\)) \{\n\s*(ax|bx|cx|dx) = ([^;\n]+);\n\s*\} else \{\n\s*\3 = ([^;\n]+);\n\1\}$)re", kLines);

    const auto one = [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), c = m.group(2), r = m.group(3), a = m.group(4), b = m.group(5), ind2 = m.group(6),
                          nxt = m.group(7);
        if (ind2 != ind || count_word(nxt, r) != 1 || statement_start.match(nxt)) return str(m.group(0));
        const string_view rest = m.subject().substr(m.end());
        // (read again later: not this way)
        if (has_word(until_given(cat("\n", rest), r, true), r) && r != "si" && r != "di") return str(m.group(0));
        if (r == "si" || r == "di") return str(m.group(0));   // (a register variable: given a value it keeps)
        const string val = ternary(c, a, b);
        if (r.ends_with('l')) return cat(ind, replace_word_first(nxt, r, cat("(char)(", val, ")")));   // (a byte register's: a char)
        return cat(ind, replace_word_first(nxt, r, cat("(", val, ")")));
    };
    const auto first = [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), r = m.group(2), a = m.group(3), c = m.group(4), b = m.group(5), nxt = m.group(6);
        const Lines tail = split_lines(m.subject().substr(m.end()));
        if (count_word(nxt, r) != 1 || has_word(c, r) || statement_start.match(nxt) || live_from(tail, 0, r)) return str(m.group(0));
        return cat(ind, replace_word_first(nxt, r, cat("(", ternary(c, b, a), ")")));
    };

    string text = given_twice.replace_all(text_in, first);
    for (;;) {
        string t = nested.replace_all(text, [](const RegexMatch& m) {
            return cat(m.group(1), m.group(3), " = (", ternary(m.group(2), m.group(4), m.group(5)), ");");
        });
        t = arms.replace_all(t, one);
        if (t == text) return t;
        text = std::move(t);
    }
}

string call_tests(string_view text) {
    static const Regex selector(R"re(^([ \t]*)ax = ([^;\n]+);\n\1(\w+) = ax;\n\1switch \(ax\) \{$)re", kLines);
    static const Regex call_then_test(R"re(^([ \t]*)([A-Za-z_]\w*\([^;\n]*\));\n\1if (\(.*\)) \{$)re", kLines);
    static const Regex plus_one(R"re(\(\(([^()]+|[A-Za-z_]\w*\([^()]*\)) \+ 1\) (==|!=) 0\))re");
    string t = selector.replace_all(text, [](const RegexMatch& m) {
        return cat(m.group(1), "switch (", m.group(3), " = ", m.group(2), ") {");
    });
    t = call_then_test.replace_all(t, [](const RegexMatch& m) -> string {
        const string_view call = m.group(2), test = m.group(3);
        if (count_word(test, "ax") != 1) return str(m.group(0));
        return cat(m.group(1), "if ", replace_word_all(test, "ax", call), " {");
    });
    return plus_one.replace_all(t, "($1 $2 -1)");
}

string stored_reads(string_view text) {
    static const Regex read_back(R"re(^([ \t]*)(\w+(?:->f\d+)+) = ([^;\n]*\w\([^;\n]*\));\n\1(\w+) = \2;$)re", kLines);
    return read_back.replace_all(text, [](const RegexMatch& m) {
        return cat(m.group(1), m.group(4), " = ", m.group(2), " = ", m.group(3), ";");
    });
}

string stored_tests(string_view text) {
    static const Regex load(R"re(^([ \t]*)(ax|al|bx|cx|dx) = ([^;]+);$)re");
    Lines lines = split_lines(text);
    MaybeLines kept = maybe(lines);
    for (std::size_t k = 0; k + 2 < lines.size(); ++k) {
        const string& l0 = kept[k] ? *kept[k] : string();
        const auto m = load.match(l0);
        if (!m) continue;
        const string ind = str(m->group(1)), r = str(m->group(2)), e = str(m->group(3));
        // `IND(place) = R;` then `IND(if|while) ((R ==|!= 0))TAIL`
        const string l1 = kept[k + 1] ? *kept[k + 1] : string(), l2 = kept[k + 2] ? *kept[k + 2] : string();
        const string tail1 = cat(" = ", r, ";");
        if (!l1.starts_with(ind) || !l1.ends_with(tail1) || l1.size() <= ind.size() + tail1.size()) continue;
        const string place = l1.substr(ind.size(), l1.size() - ind.size() - tail1.size());
        if (!is_place(place)) continue;
        if (!l2.starts_with(ind)) continue;
        string_view s2 = string_view(l2).substr(ind.size());
        string_view kw = s2.starts_with("if (") ? "if" : s2.starts_with("while (") ? "while" : string_view();
        if (kw.empty()) continue;
        s2.remove_prefix(kw.size() + 2);   // `(` after the keyword; the test's own `(` follows
        const string want = cat("(", r, " ");
        if (!s2.starts_with(want)) continue;
        s2.remove_prefix(want.size());
        if (s2.size() < 6 || (s2.substr(0, 2) != "!=" && s2.substr(0, 2) != "==") || s2.substr(2, 4) != " 0))") continue;
        const string_view op = s2.substr(0, 2), tail = s2.substr(6);
        const string joined = cat(place, e, tail);
        if (has_word(joined, r)) continue;
        if (live_from(kept, k + 3, r)) continue;
        const string v = cat("(", place, " = ", e, ")");
        kept[k] = op == "!=" ? cat(ind, kw, " (", v, ")", tail) : cat(ind, kw, " (!", v, ")", tail);
        kept[k + 1].reset();
        kept[k + 2].reset();
    }
    return join_live(kept);
}

string stored_values(string_view text_in) {
    static const Regex byte_compare(R"re(\(\(\((\w+) \+ 1\) & 0xff\) ([!=]=) 0\))re");
    static const Regex store(R"re(^\s*([a-z]|v\d+|g_[0-9a-f]+|r_\w+) = (ax|al);$)re");
    static const Regex entry(R"re(^\s*(?:L_\w+:|(?:case\b|default:|\} else\b)))re");
    static const Regex write(R"re(^(\s*)(?:ax|al|ah) = (.*)$)re");
    static const Regex word_call(R"re(\b([A-Za-z_]\w*)\s*\()re");
    static const Regex pointer_call(R"re(\(\*[^()]+\)\s*\()re");
    static const Regex leaves(R"re(^\s*(?:return|break|continue|goto)\b)re");
    // (inc al; jne: a byte compared with -1 - ((r + 1) & 0xff) == 0 is (char)r == -1)
    const string text = byte_compare.replace_all(text_in, "((char)$1 $2 -1)");
    Lines lines = split_lines(text);
    const auto reads_reg = [](string_view s, string_view r) {
        return r == "ax" ? has_word(s, "ax") || has_word(s, "al") || has_word(s, "ah") : has_word(s, "al");
    };
    static constexpr std::array<string_view, 6> kNotCalls = {"if", "while", "for", "switch", "sizeof", "return"};
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = store.full_match(lines[k]);
        if (!m) continue;
        const string v = str(m->group(1)), r = str(m->group(2));
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            const string y = lines[j];
            if (entry.match(y)) break;
            if (const auto w = write.match(y)) {   // (written: its value read first, then no more)
                const string_view rhs = w->group(2);
                if (reads_reg(rhs, r) && r == "ax") lines[j] = cat(y.substr(0, y.size() - rhs.size()), replace_word_all(rhs, "ax", v));
                break;
            }
            if (reads_reg(y, r)) {
                if (r == "ax" && (has_word(y, "al") || has_word(y, "ah"))) break;
                lines[j] = replace_word_all(y, r, v);
            }
            bool calls = false;
            word_call.for_each_match(y, [&](const RegexMatch& c) {
                if (std::ranges::find(kNotCalls, c.group(1)) == kNotCalls.end()) calls = true;
                return !calls;
            });
            // (a call - it gives ax - or V given again: done)
            if (calls || pointer_call.search(y) || is_assigned(y, v) || leaves.match(y)) break;
        }
    }
    return join_lines(lines);
}

string register_chains(string_view text) {
    static const Regex load(R"re(^([ \t]*)(ax|bx|cx|dx) = ([^;]+);$)re");
    Lines lines = split_lines(text);
    MaybeLines kept = maybe(lines);
    for (std::size_t k = 0; k + 2 < lines.size(); ++k) {
        const string l0 = kept[k] ? *kept[k] : string(), l1 = kept[k + 1] ? *kept[k + 1] : string(),
                     l2 = kept[k + 2] ? *kept[k + 2] : string();
        const auto m = load.match(l0);
        if (!m) continue;
        const string ind = str(m->group(1)), r = str(m->group(2)), e = str(m->group(3));
        const auto a = stored_from(l1, ind, r);
        const auto b = stored_from(l2, ind, r);
        if (!a || !b || has_word(cat(*a, *b, e), r) || live_from(kept, k + 3, r)) continue;
        kept[k] = cat(ind, *b, " = ", *a, " = ", e, ";");
        kept[k + 1].reset();
        kept[k + 2].reset();
    }
    return join_live(kept);
}

string copy_tests(string_view text) {
    static const Regex copy(
        R"re(^([ \t]*)(?:(\w+) = \2 - 1;\n\s*)?\*(\w+) = \*(\w+);\n(?:\s*(\3) = \3 \+ 1;\n)?\s*if \(\(\(\*(\w+) & 0xff\) == 0\)\) break;$)re",
        kLines);
    return copy.replace_all(text, [](const RegexMatch& m) -> string {
        const string_view pre = m.group(2), d = m.group(3), s = m.group(4), t = m.group(6);
        const bool has_pre = m.has(2), has_post = m.has(5);
        if (t != s || (has_pre && pre != s)) return str(m.group(0));
        const string src = has_pre ? cat("*--", s) : cat("*", s);
        const string dst = has_post ? cat("*", d, "++") : cat("*", d);
        return cat(m.group(1), "if (((", dst, " = ", src, ") == 0)) break;");
    });
}

string self_updates(string_view text) {
    static const Regex assign(R"re(^(\s*)([A-Za-z_]\w*) = ([^;]+);\s*$)re");
    static const Regex update(R"re(^\s*([A-Za-z_]\w*) = ([A-Za-z_]\w*) ([-+*&|^]|<<|>>) (.+);\s*$)re");
    static const Regex update_parenthesised(R"re(^\s*([A-Za-z_]\w*) = \(([A-Za-z_]\w*) ([-+*&|^]|<<|>>) ([^()]+)\);\s*$)re");
    static const Regex statement(R"re(^\s*(?:if|while|return)\b)re");
    static const Regex operation(R"re( [-+*/%&|^<>]{1,2} )re");
    Lines lines = split_lines(text);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = assign.match(lines[k]);
        if (m && k + 1 < lines.size()) {
            const string ind = str(m->group(1)), x = str(m->group(2));
            string e = str(m->group(3));
            auto u = update.match(lines[k + 1]);
            if (!u || u->group(1) != x || u->group(2) != x) {
                u = update_parenthesised.match(lines[k + 1]);   // (or in parentheses)
                if (u && (u->group(1) != x || u->group(2) != x)) u.reset();
            }
            if (u && !has_word(cat(e, u->group(4)), x) && !statement.match(lines[k])) {
                const string f = str(u->group(4));
                if (operation.search(e) && !(e.starts_with("(") && e.ends_with(")") && dt::balanced(string_view(e).substr(1, e.size() - 2))))
                    e = cat("(", e, ")");   // (E an expression of its own: kept one, whatever op)
                lines[k + 1] = cat(ind, x, " = ", e, " ", u->group(3), " ", f, ";");
                ++k;   // (and on: the next step of it too)
                continue;
            }
        }
        out.push_back(lines[k]);
        ++k;
    }
    return join_lines(out);
}

string post_steps(string_view text) {
    static const Regex copy(R"re(^(\s*)(ax|bx|cx|dx|al|bl|cl|dl) = ([A-Za-z_]\w*);\s*$)re");
    static const Regex step(R"re(^\s*([A-Za-z_]\w*) = ([A-Za-z_]\w*) ([+-]) 1;\s*$)re");
    Lines lines = split_lines(text);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = copy.match(lines[k]);
        if (m && k + 2 < lines.size()) {
            const string reg = str(m->group(2)), x = str(m->group(3));
            const auto st = step.match(lines[k + 1]);
            const string& nxt = lines[k + 2];
            const string rest = join_lines(std::span<const string>(lines).subspan(k + 3));
            if (st && st->group(1) == x && st->group(2) == x && count_word(nxt, reg) == 1 && !has_word(nxt, x) &&
                !has_word(until_given(rest, reg, false), reg)) {
                out.push_back(replace_word_first(nxt, reg, cat(x, st->group(3) == "+" ? "++" : "--")));
                k += 3;
                continue;
            }
        }
        out.push_back(lines[k]);
        ++k;
    }
    return join_lines(out);
}

string post_increments(string_view text) {
    static const Regex load(R"re(^(\s*)(ax|bx|cx|dx|si|di|al|bl|cl|dl|t\d+) = (.*);\s*$)re");
    static const Regex step_by(R"re(^\s*(\w+) = \1 \+ (\d)\s*;\s*$)re");
    static const Regex step_one(R"re(^\s*(\w+)\+\+()\s*;\s*$)re");
    static const Regex back_by(R"re(^\s*(\w+) = \1 - 1\s*;\s*$)re");
    static const Regex back_one(R"re(^\s*(\w+)--\s*;\s*$)re");
    Lines lines = split_lines(text);
    Lines out;
    std::size_t k = 0;
    const auto scratch = [](string_view reg) { return reg == "ax" || reg == "bx" || reg == "cx" || reg == "dx" || reg.starts_with("t"); };
    while (k < lines.size()) {
        const string& l = lines[k];
        const auto m = load.match(l);
        if (m && k + 2 < lines.size()) {
            const string reg = str(m->group(2)), e = str(m->group(3));
            auto st = step_by.match(lines[k + 1]);
            if (!st) st = step_one.match(lines[k + 1]);
            if (st) {
                const string p = str(st->group(1));
                const std::int64_t n = st->group(2).empty() ? 1 : *number(st->group(2));   // (p++: a step of one)
                // `*(T *)p`, `*(unsigned T *)p` or `*p`: what p points to; T char steps by 1, int by 2
                std::optional<int> size;
                bool deref = false;
                if (e == cat("*", p)) {
                    deref = true;
                } else {
                    for (string_view type : {"char", "int"}) {
                        if (e == cat("*(", type, " *)", p) || e == cat("*(unsigned ", type, " *)", p)) {
                            deref = true;
                            size = type == "char" ? 1 : 2;
                        }
                    }
                }
                const string& nxt = lines[k + 2];
                const string rest = join_lines(std::span<const string>(lines).subspan(k + 3));
                if (deref && (!size || *size == n) && count_word(nxt, reg) == 1 && !has_word(until_given(rest, reg, false), reg) &&
                    !has_word(nxt, p)) {
                    out.push_back(replace_word_first(nxt, reg, cat(e, "++")));
                    k += 3;
                    continue;
                }
                // (r = p; p = p + 1; X reads r once: p's old value - an index stepped, a[n++])
                if (e == p && n == 1 && scratch(reg) && count_word(nxt, reg) == 1 && !has_word(nxt, p) && !live_from(lines, k + 3, reg)) {
                    out.push_back(replace_word_first(nxt, reg, cat(p, "++")));
                    k += 3;
                    continue;
                }
            }
            auto sd = back_by.match(lines[k + 1]);
            if (!sd) sd = back_one.match(lines[k + 1]);
            if (sd && e == sd->group(1) && scratch(reg)) {   // (and p--: the old value)
                const string& nxt = lines[k + 2];
                if (count_word(nxt, reg) == 1 && !has_word(nxt, e) && !live_from(lines, k + 3, reg)) {
                    out.push_back(replace_word_first(nxt, reg, cat(e, "--")));
                    k += 3;
                    continue;
                }
            }
        }
        out.push_back(l);
        ++k;
    }
    return join_lines(out);
}

}  // namespace cb::exact::draft::shapes
