// The passes that read back loops: a loop's test moved to its head, a register carried round into a variable, a step
// made the for's, loops of tests alone, an outer loop that only restarts the inner one.
#include <algorithm>
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

bool starts_with_break_or_return(string_view line) {   // `\s*(?:break|return)\b`
    static const Regex re(R"re(^\s*(?:break|return)\b)re");
    return re.match(line).has_value();
}

// `while (C)` the way the passes print a test as a loop's: the test in its own parentheses.
string parenthesised(const string& test) {
    return test.starts_with("(") && dt::balanced(string_view(test).substr(1, test.size() - 2)) ? test : cat("(", test, ")");
}

}  // namespace

// ---- a register carried round a loop -------------------------------------------------------------------------

string loop_registers(string_view text_in) {
    static const Regex loop_head(R"re([ \t]*(?:for \(;;\)|while \(1\)) \{)re");
    static const Regex carried(R"re([ \t]*(\w+) = (ax|bx|cx|dx);)re");
    static const Regex preset(R"re(([ \t]*)(ax|bx|cx|dx) = ([^;]+);)re");
    string text = str(text_in);
    for (;;) {
        const Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 1; k < lines.size() && !again; ++k) {
            if (!loop_head.full_match(lines[k])) continue;
            const auto e = dt::block_end(lines, k);
            const auto f = e ? carried.full_match(lines[k + 1]) : std::nullopt;
            const auto pre = preset.full_match(lines[k - 1]);
            if (!f || !pre || pre->group(2) != f->group(2)) continue;
            const string v = str(f->group(1)), r = str(f->group(2));
            const Lines body = slice(lines, k + 2, *e);
            bool ok = true;   // (r read only as v's value; given only before going round)
            for (std::size_t j = 0; j < body.size(); ++j) {
                // `[ \t]*R = `
                const std::size_t lead = body[j].find_first_not_of(" \t");
                const bool gives = lead != string::npos && string_view(body[j]).substr(lead).starts_with(cat(r, " = "));
                if (gives && !(j + 1 < body.size() && txt::trim(body[j + 1]) == "continue;")) ok = false;
            }
            if (!ok || live_from(lines, *e + 1, r)) continue;
            Lines next = slice(lines, 0, k - 1);
            next.push_back(cat(pre->group(1), v, " = ", pre->group(3), ";"));
            next.push_back(lines[k]);
            for (const string& x : body) next.push_back(replace_word_all(x, r, v));
            next.push_back(lines[*e]);
            for (std::size_t j = *e + 1; j < lines.size(); ++j) next.push_back(lines[j]);
            text = join_lines(next);
            again = true;
        }
        if (!again) return text;
    }
}

std::optional<string> const_tests(string_view text) {
    static const Regex set_then_test(R"re(^([ \t]*)(\w+) = (-?\d+);\n\1(if|while) \((.*)\) \{$)re", kLines);
    static const Regex constant(R"re(([ \t]*)(\w+) = (-?\d+);)re");
    static const Regex barrier(R"re(^\s*(?:L_\w+:|\}\s*else))re");
    static const Regex loop_start(R"re(^\s*(?:while|for|do)\b)re");
    static const Regex test_line(R"re((\s*)if \((.*)\) \{)re");
    const string t = set_then_test.replace_all(text, [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), x = m.group(2), k = m.group(3), kw = m.group(4), test = m.group(5);
        if (count_word(test, x) != 1) return str(m.group(0));
        return cat(ind, x, " = ", k, ";\n", ind, kw, " (", replace_word_all(test, x, k), ") {");
    });
    // (and on past statements not giving it, to the first test of it in the block or one inside)
    Lines lines = split_lines(t);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const string l = lines[k];
        const auto m = constant.full_match(l);
        if (!m) continue;
        const string x = str(m->group(2)), kv = str(m->group(3));
        const std::size_t ind = m->group(1).size();
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            const string y = lines[j];
            if ((!blank(y) && indent_of(y) < ind) || barrier.match(y)) break;
            if (is_assigned_after_non_name(y, x) || loop_start.match(y)) break;
            const auto tm = test_line.full_match(y);
            if (tm && has_word(tm->group(2), x)) {
                if (count_word(tm->group(2), x) == 1) lines[j] = cat(tm->group(1), "if (", replace_word_all(tm->group(2), x, kv), ") {");
                break;
            }
            if (has_word(y, x)) break;
        }
    }
    string result = join_lines(lines);
    if (result == text) return std::nullopt;
    return result;
}

std::optional<string> loop_guards(string_view text) {
    static const Regex loop_head(R"re(([ \t]*)(?:while \(.*\)|for \(.*\)) \{)re");
    const Lines lines = split_lines(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = loop_head.full_match(lines[k]);
        if (!m) continue;
        const auto e = dt::block_end(lines, k);
        if (!e) continue;
        const string i4 = str(m->group(1)) + "    ";
        std::optional<std::size_t> j0;
        for (std::size_t j = k + 1; j < *e; ++j)
            if (lines[j].starts_with(i4 + "if (") && lines[j].ends_with("{")) j0 = j;
        const auto p = j0 ? dt::if_parts(lines, *j0) : std::nullopt;
        if (!p || !p->else_arm || p->end != *e) continue;
        Lines th, el;
        for (const string& x : p->then_arm)
            if (!blank(x)) th.push_back(x);
        for (const string& x : *p->else_arm)
            if (!blank(x)) el.push_back(x);
        if (th.empty() || txt::trim(th.back()) != "continue;" || el.empty() || !starts_with_break_or_return(el.back())) continue;
        const string neg = dt::truth(dt::negate(cat("(", p->cond, ")")));
        Lines next = slice(lines, 0, *j0);
        next.push_back(cat(i4, "if (", neg, ") {"));
        next.insert(next.end(), p->else_arm->begin(), p->else_arm->end());
        next.push_back(i4 + "}");
        for (std::size_t x = 0; x + 1 < th.size(); ++x) next.push_back(th[x].substr(std::min<std::size_t>(4, th[x].size())));
        for (std::size_t x = *e; x < lines.size(); ++x) next.push_back(lines[x]);
        const string t = join_lines(next);
        const auto fl = for_loops(t);
        return fl && !fl->empty() ? *fl : t;
    }
    return std::nullopt;
}

string first_breaks(string_view text_in) {
    static const Regex for_head(R"re(([ \t]*)for \(;;\) \{)re");
    static const Regex break_test(R"re([ \t]*if (\(.*\)) break;)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k + 1 < lines.size() && !again; ++k) {
            const auto m = for_head.full_match(lines[k]);
            const auto b = m ? break_test.full_match(lines[k + 1]) : std::nullopt;
            if (!b || !dt::balanced(b->group(1))) continue;
            const string cond = dt::truth(dt::negate(b->group(1)));
            const string header = cat(m->group(1), "while ", parenthesised(cond), " {");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(k + 2));
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), header);
            text = join_lines(lines);
            again = true;
        }
        if (!again) return text;
    }
}

std::optional<string> loop_tests(string_view text) {
    static const Regex while_head(R"re(([ \t]*)while (\(.*\)) \{)re");
    const Lines lines = split_lines(text);
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        const auto m = while_head.full_match(lines[k]);
        if (!m) continue;
        const auto e = dt::block_end(lines, k);
        if (!e || txt::trim(lines[*e]) != "}") continue;
        const auto p = dt::if_parts(lines, k + 1);
        if (!p || p->end != *e) continue;
        const string ind = str(m->group(1));
        const auto only_break = [](const Lines& arm) {
            Lines t;
            for (const string& x : arm)
                if (!blank(x)) t.push_back(str(txt::trim(x)));
            return t == Lines{"break;"};
        };
        if (!(p->else_arm && only_break(*p->else_arm))) continue;   // (if (D) break; and nothing after: no body to keep)
        const string d = p->cond.starts_with("(") ? p->cond : cat("(", p->cond, ")");
        const string cond = dt::join_tests("&&", dt::truth(m->group(2)), dt::truth(d));
        Lines next = slice(lines, 0, k);
        next.push_back(cat(ind, "while (", cond, ") {"));
        for (const string& x : p->then_arm) next.push_back(x.starts_with(ind + "        ") ? x.substr(4) : x);
        next.push_back(cat(ind, "}"));
        for (std::size_t x = *e + 1; x < lines.size(); ++x) next.push_back(lines[x]);
        const string t = join_lines(next);
        const auto fl = for_loops(t);   // (and its step a for's, when it has one)
        return fl && !fl->empty() ? *fl : t;
    }
    return std::nullopt;
}

// ---- loops of tests -----------------------------------------------------------------------------------------

string continue_loops(string_view text) {
    static const Regex for_head(R"re(([ \t]*)for \(;;\) \{)re");
    static const Regex first_store(R"re((\w+) = ([^;=]+);)re");
    static const Regex test_open(R"re(if (\(.*\)) \{)re");
    static const Regex test_continue(R"re(if (\(.*\)) continue;)re");
    static const Regex leave_stmt(R"re((?:return -?\d+|break);)re");
    static const Regex simple_part(R"re(!?[\w\->.\[\]]+|!?\(.*\))re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = for_head.full_match(lines[k]);
        const auto e = m ? dt::block_end(lines, k) : std::nullopt;
        if (!m || !e || txt::trim(lines[*e]) != "}") {
            ++k;
            continue;
        }
        Lines body;
        for (std::size_t j = k + 1; j < *e; ++j)
            if (!blank(lines[j])) body.push_back(str(txt::trim(lines[j])));
        const auto first = body.empty() ? std::nullopt : first_store.full_match(body[0]);
        std::size_t q = first ? 1 : 0;
        std::vector<string> parts;
        std::optional<string> leave;
        bool ok = true;
        const auto rest_is = [&](std::size_t from_q, const Lines& want) {
            return from_q <= body.size() && Lines(body.begin() + static_cast<std::ptrdiff_t>(from_q), body.end()) == want;
        };
        const auto next_two_are = [&](std::size_t from_q, const Lines& want) {
            return from_q + want.size() <= body.size() && Lines(body.begin() + static_cast<std::ptrdiff_t>(from_q), body.begin() + static_cast<std::ptrdiff_t>(from_q + want.size())) == want;
        };
        while (q < body.size()) {
            const auto c = test_open.full_match(body[q]);
            const auto c1 = test_continue.full_match(body[q]);
            if (c1) {
                parts.push_back(dt::truth(dt::negate(c1->group(1))));
                ++q;
                continue;
            }
            if (c && next_two_are(q + 1, {"continue;", "}"})) {
                parts.push_back(dt::truth(dt::negate(c->group(1))));
                q += 3;
                continue;
            }
            if (c && q + 2 < body.size() && leave_stmt.full_match(body[q + 1]) &&
                (rest_is(q + 2, {"}"}) || rest_is(q + 2, {"} else {", "continue;", "}"}))) {
                parts.push_back(dt::truth(c->group(1)));
                leave = body[q + 1];
                q = body.size();
                break;
            }
            ok = false;
            break;
        }
        if (!ok || !leave || parts.size() < 2) {
            ++k;
            continue;
        }
        if (first) {
            const string x = str(first->group(1)), v = str(first->group(2));
            if (count_word(parts[0], x) != 1) {
                ++k;
                continue;
            }
            parts[0] = replace_word_first(parts[0], x, cat("(", x, " = ", v, ")"));
        }
        string cond;
        for (const string& p : parts) {
            if (!cond.empty()) cond += " && ";
            cond += simple_part.full_match(p) && dt::balanced(p) ? p : cat("(", p, ")");
        }
        const string ind = str(m->group(1));
        Lines replacement{cat(ind, "while (!(", cond, "))"), cat(ind, "    ;")};
        if (*leave != "break;") replacement.push_back(cat(ind, *leave));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(*e + 1));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        k += replacement.size();
    }
    return join_lines(lines);
}

string empty_loops(string_view text) {
    static const Regex for_head(R"re(^([ \t]*)for \(;;\) \{$)re");
    static const Regex first_store(R"re((\w+) = ([^;=]+);)re");
    static const Regex break_test(R"re(if (\(.*\)) break;)re");
    static const Regex test_open(R"re(if (\(.*\)) \{)re");
    static const Regex simple_cond(R"re([\w\->.\[\]]+|\(.*\))re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = for_head.match(lines[k]);
        const auto e = m ? dt::block_end(lines, k) : std::nullopt;
        if (!m || !e || txt::trim(lines[*e]) != "}") {
            ++k;
            continue;
        }
        Lines body;
        for (std::size_t j = k + 1; j < *e; ++j) body.push_back(str(txt::trim(lines[j])));
        std::vector<string> conds;
        std::optional<RegexMatch> first;
        std::size_t q = 0;
        if (!body.empty()) first = first_store.full_match(body[0]);
        if (first) q = 1;
        const auto window_is = [&](std::size_t at, const Lines& want) {
            Lines have;
            for (std::size_t i = at; i < std::min(body.size(), at + want.size()); ++i) have.push_back(body[i]);
            return have == want;
        };
        bool ok = true;
        while (q < body.size()) {
            if (const auto t = break_test.full_match(body[q])) {
                conds.push_back(str(t->group(1)));
                ++q;
                continue;
            }
            const auto t = test_open.full_match(body[q]);
            if (t && window_is(q + 1, {"break;", "}"})) {
                conds.push_back(str(t->group(1)));
                q += 3;
                continue;
            }
            if (t && window_is(q + 1, {"break;", "} else {", "continue;", "}"})) {
                conds.push_back(str(t->group(1)));
                q += 5;
                continue;
            }
            if (body[q] == "continue;" && q == body.size() - 1) {
                ++q;
                continue;
            }
            ok = false;
            break;
        }
        if (!ok && first && body.size() > 2 &&
            (body[1] == cat("if ((", first->group(1), " == 0)) break;") || body[1] == cat("if (!", first->group(1), ") break;"))) {
            // (for (;;) { x = E; if (!x) break; S } - the value tested first, a body after: while ((x = E)) { S })
            const string header = cat(m->group(1), "while ((", first->group(1), " = ", first->group(2), ")) {");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(k + 3));
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), header);
            ++k;
            continue;
        }
        if (!ok || conds.empty() || std::ranges::any_of(conds, [](const string& c) { return !dt::balanced(c); })) {
            ++k;
            continue;
        }
        std::vector<string> gs;
        for (const string& c : conds) gs.push_back(dt::truth(dt::negate(c)));
        if (first) {
            const string x = str(first->group(1)), v = str(first->group(2));
            const string head = dt::strip_parens(conds[0]);
            if (head == cat("(", x, " == 0)") || head == cat("!", x)) {
                gs[0] = cat("(", x, " = ", v, ")");
            } else {
                ++k;
                continue;
            }
        }
        string g;
        for (const string& c : gs) {
            const string_view inner = c.starts_with("(") ? string_view(c).substr(1, c.size() - 2) : string_view(c);
            if (!g.empty()) g += " && ";
            g += simple_cond.full_match(c) && dt::balanced(inner) ? c : cat("(", c, ")");
        }
        const string ind = str(m->group(1));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(*e + 1));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), {cat(ind, "while (", g, ")"), cat(ind, "    ;")});
        k += 2;
    }
    return join_lines(lines);
}

std::optional<string> assign_loops(string_view text) {
    static const Regex for_head(R"re((\s*)for \(;;\) \{)re");
    static const Regex assign(R"re(([a-z]|v\d+|r_\w+) = ([^;]+);)re");
    static const Regex break_test(R"re(if \((.+)\) break;)re");
    Lines lines = split_lines(text);
    for (std::size_t k = 0; k + 2 < lines.size(); ++k) {
        const auto m = for_head.full_match(lines[k]);
        if (!m) continue;
        const string ind = str(m->group(1));
        const string step = ind + "    ";
        if (!lines[k + 1].starts_with(step) || !lines[k + 2].starts_with(step)) continue;
        const auto a = assign.full_match(string_view(lines[k + 1]).substr(step.size()));
        const auto b = break_test.full_match(string_view(lines[k + 2]).substr(step.size()));
        if (!a || !b || count_word(b->group(1), a->group(1)) != 1) continue;
        std::optional<std::size_t> end;
        for (std::size_t j = k + 3; j < lines.size(); ++j)
            if (lines[j] == ind + "}") {
                end = j;
                break;
            }
        if (!end) continue;
        bool continues = false;
        for (std::size_t j = k + 3; j < *end; ++j)
            if (txt::trim(lines[j]) == "continue;" && indent_of(lines[j]) == ind.size() + 4) continues = true;
        if (continues) continue;
        const string cond = replace_word_first(b->group(1), a->group(1), cat("(", a->group(1), " = ", a->group(2), ")"));
        const string neg = dt::negate(cat("(", cond, ")"));
        const bool wrapped = neg.starts_with("(") && dt::balanced(string_view(neg).substr(1, neg.size() - 2));
        const string header = cat(ind, "while ", wrapped ? neg : cat("(", neg, ")"), " {");
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(k + 3));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), header);
        return join_lines(lines);
    }
    return std::nullopt;
}

string store_loops(string_view text) {
    static const Regex copy_loop(
        R"re(^([ \t]*)do \{\n[ \t]*(ax|al|bx|bl|cx|cl|dx|dl|t\d+) = ([^;\n]+);\n(?:[ \t]*(\w+) = \4 \+ 1;\n)?[ \t]*([^;\n=]+?) = \2;\n[ \t]*\} while \(\(?\2(?: != 0)?\)?\);)re",
        kLines);
    return copy_loop.replace_all(text, [](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), reg = m.group(2), dst = m.group(5);
        string e = str(m.group(3));
        if (m.has(4)) {
            const string_view p = m.group(4);
            // `*(char *)p`, `*(unsigned char *)p` or `*p`
            const bool deref = e == cat("*(char *)", p) || e == cat("*(unsigned char *)", p) || e == cat("*", p);
            if (!deref || has_word(dst, p)) return str(m.group(0));
            e = cat("*", p, "++");
        }
        if (has_word(cat(dst, e), reg)) return str(m.group(0));
        return cat(ind, "while (", dst, " = ", e, ")\n", ind, "    ;");
    });
}

string test_first_loops(string_view text) {
    static const Regex for_head(R"re(^(\s*)for \(;;\) \{\s*$)re");
    static const Regex break_test(R"re(^\s*if (\(.*\)) break;\s*$)re");
    static const Regex step_assign(R"re(^\s*(\w+) = \1 ([+-]) ([12]);\s*$)re");
    static const Regex step_incr(R"re(^\s*(\w+)(\+|-)[+-]()\s*;\s*$)re");
    MaybeLines lines = maybe(split_lines(text));
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!lines[k]) continue;
        const auto m = for_head.match(*lines[k]);
        if (!m || k + 1 >= lines.size() || !lines[k + 1]) continue;
        const string ind = str(m->group(1));
        if (const auto b = break_test.match(*lines[k + 1])) {
            const string header = cat(ind, "while (", dt::negate(b->group(1)), ") {");
            lines[k] = header;
            lines[k + 1].reset();
            continue;
        }
        auto s = step_assign.match(*lines[k + 1]);
        if (!s) s = step_incr.match(*lines[k + 1]);   // (a step, then the test: --p in it; by 2: words)
        std::optional<RegexMatch> b;
        if (s && k + 2 < lines.size() && lines[k + 2] && !lines[k + 2]->empty()) b = break_test.match(*lines[k + 2]);
        if (b && count_word(b->group(1), s->group(1)) == 1) {
            const string op = s->group(2) == "+" ? "++" : "--";
            const string c = replace_word_all(b->group(1), s->group(1), cat(op, s->group(1)));
            const string header = cat(ind, "while (", dt::truth(dt::negate(c)), ") {");
            lines[k] = header;
            lines[k + 1].reset();
            lines[k + 2].reset();
        }
    }
    return join_live(lines);
}

// ---- for loops ------------------------------------------------------------------------------------------------

std::optional<string> for_loops(string_view text) {
    static const Regex while_head(R"re(^(\s*)while \((.*)\) \{$)re");
    static const Regex step_text(R"re(([A-Za-z_]\w*) = \1 [+-] \d+;|[A-Za-z_]\w*(?:\+\+|--);)re");
    static const Regex init_line(R"re([A-Za-z_]\w* = [^;]+;)re");
    static const Regex init_parts(R"re(([A-Za-z_]\w*) = ([^;]+);)re");
    static const Regex variable(R"re(^[A-Za-z_]\w*)re");
    Lines lines = split_lines(text);
    bool changed = false;
    std::size_t k = 0;
    // `<prefix>STEP`: the line is the prefix and then a step of a variable
    const auto is_step_at = [&](const string& line, const string& prefix) {
        return line.starts_with(prefix) && step_text.full_match(string_view(line).substr(prefix.size()));
    };
    while (k < lines.size()) {
        const auto m = while_head.match(lines[k]);
        const auto e = m ? dt::block_end(lines, k) : std::nullopt;
        if (!m || !e || txt::trim(lines[*e]) != "}") {
            ++k;
            continue;
        }
        const string ind = str(m->group(1)), i4 = ind + "    ";
        Lines body = slice(lines, k + 1, *e);
        Lines own;
        for (const string& l : body)
            if (l.starts_with(i4) && !l.starts_with(i4 + " ")) own.push_back(l);
        const bool own_continue = std::ranges::find(own, i4 + "continue;") != own.end();
        string step;
        if (!body.empty() && is_step_at(body.back(), i4) && !own_continue) {
            step = str(txt::trim(body.back()));
            body.pop_back();
        } else if (body.size() >= 4 && body[body.size() - 4] == i4 + "} else {" && is_step_at(body[body.size() - 3], i4 + "    ") &&
                   body[body.size() - 2] == i4 + "    continue;" && body.back() == i4 + "}" && !own_continue &&
                   starts_with_break_or_return(body.size() >= 5 ? body[body.size() - 5] : string())) {
            step = str(txt::trim(body[body.size() - 3]));
            body.resize(body.size() - 4);
            body.push_back(i4 + "}");
        } else {   // (the other way about: if (C) { X = X + K; continue; } else { ..leaves.. })
            std::optional<std::size_t> j0;
            for (std::size_t j = 0; j < body.size(); ++j)
                if (body[j].starts_with(i4 + "if (")) j0 = j;
            const auto q = j0 ? dt::if_parts(body, *j0) : std::nullopt;
            if (q && q->end == body.size() && q->else_arm && !q->else_arm->empty() && q->then_arm.size() == 2 &&
                is_step_at(q->then_arm[0], i4 + "    ") && q->then_arm[1] == i4 + "    continue;" &&
                starts_with_break_or_return(q->else_arm->back()) && !own_continue) {
                step = str(txt::trim(q->then_arm[0]));
                Lines next = slice(body, 0, *j0);
                next.push_back(cat(i4, "if (", dt::truth(dt::negate(cat("(", q->cond, ")"))), ") {"));
                next.insert(next.end(), q->else_arm->begin(), q->else_arm->end());
                next.push_back(i4 + "}");
                body = std::move(next);
            } else if (q && q->end == body.size() && q->else_arm && q->else_arm->size() >= 3 &&
                       is_step_at((*q->else_arm)[q->else_arm->size() - 2], i4 + "    ") && q->else_arm->back() == i4 + "    continue;" &&
                       !q->then_arm.empty() && starts_with_break_or_return(q->then_arm.back()) && !own_continue &&
                       !std::ranges::any_of(slice(*q->else_arm, 0, q->else_arm->size() - 1), [](const string& x) { return txt::trim(x) == "continue;"; })) {
                // (if (C) { ..leaves.. } else { S; X = X + K; continue; })
                step = str(txt::trim((*q->else_arm)[q->else_arm->size() - 2]));
                Lines next = slice(body, 0, *j0 + 1);
                next.insert(next.end(), q->then_arm.begin(), q->then_arm.end());
                next.push_back(i4 + "}");
                for (std::size_t x = 0; x + 2 < q->else_arm->size(); ++x) next.push_back((*q->else_arm)[x].substr(std::min<std::size_t>(4, (*q->else_arm)[x].size())));
                body = std::move(next);
            }
        }
        if (step.empty()) {
            ++k;
            continue;
        }
        string init;
        std::ptrdiff_t p = static_cast<std::ptrdiff_t>(k) - 1;   // (the init: the line before, or one a few before with nothing between reading it)
        while (p >= 0 && p > static_cast<std::ptrdiff_t>(k) - 4 && lines[static_cast<std::size_t>(p)].starts_with(ind) &&
               init_line.full_match(string_view(lines[static_cast<std::size_t>(p)]).substr(ind.size()))) {
            const auto im = init_parts.full_match(string_view(lines[static_cast<std::size_t>(p)]).substr(ind.size()));
            const string sv = str(variable.match(step)->group(0));   // (the variable stepped)
            bool read_between = false;
            for (std::size_t j = static_cast<std::size_t>(p) + 1; j < k; ++j) read_between = read_between || has_word(lines[j], sv);
            if (im->group(1) == sv && !read_between) {
                init = cat(im->group(1), " = ", im->group(2));
                break;
            }
            --p;
        }
        const string stepped = step.ends_with(";") ? step.substr(0, step.size() - 1) : step;
        Lines replacement{cat(ind, "for (", init, "; ", dt::truth(cat("(", m->group(2), ")")), "; ", stepped, ") {")};
        replacement.insert(replacement.end(), body.begin(), body.end());
        replacement.push_back(lines[*e]);
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(*e + 1));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        if (!init.empty()) lines.erase(lines.begin() + p);
        else ++k;
        changed = true;
    }
    if (!changed) return std::nullopt;
    return join_lines(lines);
}

string loop_collapse(string_view text) {
    static const Regex label(R"re((\s*)(L_\w+):)re");
    static const Regex loop_start(R"re(^\s*(?:while|for|do)\b)re");
    const Lines lines = split_lines(text);
    for (std::size_t k = 0; k + 3 < lines.size(); ++k) {
        const auto m = label.full_match(lines[k]);
        if (!m || lines[k + 1] != cat(m->group(1), "for (;;) {") || lines[k + 2] != cat(m->group(1), "    for (;;) {")) continue;
        const string ind = str(m->group(1)), lab = str(m->group(2));
        std::optional<std::size_t> e_in;
        for (std::size_t j = k + 3; j < lines.size(); ++j)
            if (lines[j] == ind + "    }") {
                e_in = j;
                break;
            }
        if (!e_in || *e_in + 2 >= lines.size() || txt::trim(lines[*e_in + 1]) != "break;" || lines[*e_in + 2] != ind + "}") continue;
        Lines body = slice(lines, k + 3, *e_in);
        std::vector<std::size_t> gotos;
        for (std::size_t j = 0; j < body.size(); ++j)
            if (txt::trim(body[j]) == cat("goto ", lab, ";")) gotos.push_back(j);
        // (a loop of its own around the goto: a continue would be that loop's - a switch is no loop for a continue)
        const auto in_inner_loop = [&](std::size_t j) {
            std::size_t want = indent_of(body[j]);
            for (std::size_t i = j; i-- > 0;) {
                const string& x = body[i];
                const std::size_t d = indent_of(x);
                if (!blank(x) && d < want && txt::rtrim(x).ends_with("{")) {
                    if (loop_start.match(x) || txt::trim(x) == "do {") return true;
                    want = d;
                }
            }
            return false;
        };
        if (gotos.empty()) continue;
        Lines outside = slice(lines, 0, k);
        for (std::size_t j = *e_in; j < lines.size(); ++j) outside.push_back(lines[j]);
        if (goes_to(join_lines(outside), lab) || std::ranges::any_of(gotos, in_inner_loop)) continue;
        for (std::size_t j : gotos) body[j] = txt::replace_all(body[j], cat("goto ", lab, ";"), "continue;");
        Lines next = slice(lines, 0, k);
        next.push_back(ind + "for (;;) {");
        for (const string& x : body) next.push_back(x.starts_with(ind + "        ") ? x.substr(4) : x);
        next.push_back(ind + "}");
        for (std::size_t j = *e_in + 3; j < lines.size(); ++j) next.push_back(lines[j]);
        return join_lines(next);
    }
    return str(text);
}

string tail_continues(string_view text) {
    static const Regex loop_open(R"re(^\s*(?:while|for)\b.*\{$)re");
    static const Regex arm_open(R"re(^\s*(?:if \(.*\) \{|\} else \{)$)re");
    Lines lines = split_lines(text);
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t k = 0; k < lines.size() && !changed; ++k) {
            if (txt::trim(lines[k]) != "continue;" || k + 1 >= lines.size()) continue;
            std::size_t j = k + 1;   // (only closing braces after it, to the loop's own)
            while (j < lines.size() && txt::trim(lines[j]) == "}") {
                const auto op = dt::opener(lines, j);
                if (!op) break;
                if (loop_open.match(lines[*op])) {
                    if (j > k + 1) {   // (inside an if at the body's end, not the body's own)
                        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
                        changed = true;
                    }
                    break;
                }
                if (!arm_open.match(lines[*op])) break;
                ++j;
            }
        }
    }
    return join_lines(lines);
}

}  // namespace cb::exact::draft::shapes
