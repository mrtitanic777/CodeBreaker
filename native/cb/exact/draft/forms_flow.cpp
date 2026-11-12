#include "cb/exact/draft/forms.hpp"

#include <algorithm>
#include <cctype>
#include <format>
#include <functional>
#include <map>
#include <memory>
#include <set>

#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/regex.hpp"
#include "cb/core/text.hpp"
#include "cb/exact/draft/forms_lines.hpp"

namespace cb::exact::draft::forms {

using namespace ln;

// ---- control flow -----------------------------------------------------------------------------------------------

namespace {

std::optional<dt::IfParts> if_at(const Lines& lines, std::size_t k) {
    if (k >= lines.size()) return std::nullopt;
    return dt::if_parts(lines, k);
}

bool has_content(const std::vector<std::string>& arm) {
    return std::ranges::any_of(arm, [](const std::string& x) { return !blank(x); });
}

std::vector<std::string> content_of(const std::vector<std::string>& arm) {
    std::vector<std::string> out;
    for (const std::string& x : arm)
        if (!blank(x)) out.push_back(x);
    return out;
}

// `ind(?:case [^:]+|default):` the whole line.
bool is_case_label(sv line, sv ind, bool with_default = true) {
    if (!line.starts_with(ind)) return false;
    const sv rest = line.substr(ind.size());
    if (with_default && rest == "default:") return true;
    return rest.starts_with("case ") && rest.ends_with(':') && rest.size() > 6 && rest.substr(5, rest.size() - 6).find(':') == npos;
}

// `\s*(?:case [^:]+|default):$`
bool is_any_case_label(sv line) { return is_case_label(ct::ltrim(line), ""); }

bool goto_mentioned(const Lines& lines, std::size_t from, std::size_t to, sv label) {   // `\bgoto L;` in lines[from:to]
    const std::string needle = "goto " + str(label) + ";";
    for (std::size_t i = from; i < to && i < lines.size(); ++i)
        for (std::size_t p = lines[i].find(needle); p != npos; p = lines[i].find(needle, p + 1))
            if (boundary_at(lines[i], p)) return true;
    return false;
}
bool goto_mentioned(const Lines& lines, sv label) { return goto_mentioned(lines, 0, lines.size(), label); }

std::string guard_condition(const dt::IfParts& p) {   // the negated test, parenthesised for `if %s`
    const std::string neg = dt::negate("(" + p.cond + ")");
    return neg.starts_with('(') && dt::balanced(sv(neg).substr(1, neg.size() - 2)) ? neg : "(" + neg + ")";
}

}  // namespace

// the gotos back to a loop's top: a move shared at the label (`ax = E; L: for (;;) { r = ax;` - every `ax = V; goto
// L;` too) given in each place (r = E; r = V), a for (;;) left holding one while and a break that while, and a goto to
// the top of the loop it is in a continue
std::string loop_gotos(sv text_in) {
    static const Regex label_re(R"(([ \t]*)(L_\w+):$)");
    static const Regex move_re(R"([ \t]*(\w+) = ax;$)");
    static const Regex ax_load(R"([ \t]*ax = [^;]+;$)");
    static const Regex while_re(R"([ \t]*while \(.*\) \{$)");
    static const Regex loop_head(R"([ \t]*(?:while \(.*\)|for \(.*\)) \{$)");
    static const Regex any_loop(R"([ \t]*(?:while \(.*\)|for \(.*\)|do) \{$)");
    std::string text = str(text_in);
    for (int round = 0; round < 4; ++round) {
        Lines lines = split_nl(text);
        bool done = false;
        for (std::size_t k = 0; k < lines.size() && !done; ++k) {
            const auto m = label_re.match(lines[k]);
            if (!m) continue;
            const std::string lab = str((*m)[2]);
            std::vector<std::size_t> gos;
            for (std::size_t j = 0; j < lines.size(); ++j)
                if (ct::trim(sv(lines[j])) == "goto " + lab + ";") gos.push_back(j);
            // (the move shared at the label: into each place that comes to it)
            std::size_t j = k + 1;
            if (j < lines.size() && ct::trim(sv(lines[j])) == "for (;;) {") ++j;
            std::optional<RegexMatch> mv;
            if (j < lines.size()) mv = move_re.match(lines[j]);
            bool all_loads = !gos.empty();
            for (const std::size_t g : gos)
                if (g == 0 || !ax_load.match(lines[g - 1])) all_loads = false;
            if (mv && k > 0 && ax_load.match(lines[k - 1]) && all_loads && !dt::live_after(lines, j + 1, "ax")) {
                const std::string r = str((*mv)[1]);
                std::vector<std::size_t> targets{k};
                targets.insert(targets.end(), gos.begin(), gos.end());
                for (const std::size_t g : targets) {
                    std::string& l = lines[g - 1];
                    const std::size_t p = l.find_first_not_of(" \t");
                    l = l.substr(0, p) + r + " = " + l.substr(p + 5);
                }
                std::vector<char> gone(lines.size(), 0);
                gone[j] = 1;
                text = join_live(lines, gone);
                done = true;
                break;
            }
            // (a for (;;) holding one while and a break: the while)
            if (j < lines.size() && ct::trim(sv(lines[k + 1])) == "for (;;) {") {
                const auto e = dt::block_end(lines, k + 1);
                const std::size_t w = k + 2;
                if (e && *e != 0 && w < lines.size() && while_re.match(lines[w])) {
                    const auto we = dt::block_end(lines, w);
                    if (we && *we != 0 && *we + 2 == *e && ct::trim(sv(lines[*we + 1])) == "break;") {
                        for (std::size_t q = w; q <= *we; ++q) lines[q] = dedent4(lines[q]);
                        std::vector<char> gone(lines.size(), 0);
                        gone[k + 1] = gone[*we + 1] = gone[*e] = 1;
                        text = join_live(lines, gone);
                        done = true;
                        break;
                    }
                }
            }
            // (a goto to the top of the loop it is in: a continue)
            if (k + 1 < lines.size() && loop_head.match(lines[k + 1]) && !gos.empty()) {
                const auto e = dt::block_end(lines, k + 1);
                if (e && *e != 0 && std::ranges::all_of(gos, [&](std::size_t g) { return k + 1 < g && g < *e; })) {
                    bool ok = true;
                    for (const std::size_t g : gos)   // (not inside a loop of its own)
                        for (std::size_t q = k + 2; q < g; ++q)
                            if (any_loop.match(lines[q])) {
                                const auto qe = dt::block_end(lines, q);
                                if (qe && *qe != 0 && q < g && g < *qe) ok = false;
                            }
                    if (ok) {
                        for (const std::size_t g : gos) lines[g] = ct::replace_all(lines[g], "goto " + lab + ";", "continue;");
                        std::vector<char> gone(lines.size(), 0);
                        gone[k] = 1;
                        text = join_live(lines, gone);
                        done = true;
                        break;
                    }
                }
            }
        }
        if (!done) break;
    }
    return text;
}

// a switch's default arm, written first, moved to the end (a variant: the compiler puts the default's code after the
// tests wherever it is written, so the code alone does not say where)
std::optional<std::string> default_last(sv text) {
    static const Regex switch_re(R"((\s*)switch \(.*\) \{)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = switch_re.full_match(lines[k]);
        if (!m || k + 1 >= lines.size() || lines[k + 1] != str((*m)[1]) + "default:") continue;
        const std::string ind = str((*m)[1]);
        std::optional<std::size_t> next, end;
        for (std::size_t j = k + 2; j < lines.size() && !next; ++j)
            if (is_case_label(lines[j], ind)) next = j;
        for (std::size_t j = k + 1; j < lines.size() && !end; ++j)
            if (lines[j] == ind + "}") end = j;
        if (!next || !end || *next > *end) continue;
        const Lines arm(lines.begin() + static_cast<std::ptrdiff_t>(k) + 1, lines.begin() + static_cast<std::ptrdiff_t>(*next));
        static const Regex leaves(R"(\s*(?:break;|return\b|goto\b))");
        if (!leaves.match(content_of(arm).back())) continue;   // (it falls into the next: not movable)
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k) + 1);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*next), lines.begin() + static_cast<std::ptrdiff_t>(*end));
        out.insert(out.end(), arm.begin(), arm.end());
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*end), lines.end());
        return join_nl(out);
    }
    return std::nullopt;
}

// a switch's arms that only break (case K: break;) moved to its top (a variant: such an arm has no code of its own -
// where it is written decides the layout of the others)
std::optional<std::string> empty_cases_first(sv text) {
    static const Regex switch_re(R"((\s*)switch \(.*\) \{)");
    static const Regex leaves(R"(\s*(?:break;|return\b|goto\b|continue;))");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = switch_re.full_match(lines[k]);
        if (!m) continue;
        const std::string ind = str((*m)[1]);
        std::optional<std::size_t> end;
        for (std::size_t j = k + 1; j < lines.size() && !end; ++j)
            if (lines[j] == ind + "}") end = j;
        if (!end) continue;
        Lines moved, rest;
        std::size_t j = k + 1;
        while (j < *end) {
            if (j > k + 1 && is_case_label(lines[j], ind, false) && j + 1 < *end && ct::trim(sv(lines[j + 1])) == "break;" &&
                (leaves.match(lines[j - 1]) || ct::trim(sv(lines[j - 1])).ends_with('}'))) {
                moved.push_back(lines[j]);
                moved.push_back(lines[j + 1]);
                j += 2;
                continue;
            }
            rest.push_back(lines[j]);
            ++j;
        }
        if (!moved.empty()) {
            Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k) + 1);
            out.insert(out.end(), moved.begin(), moved.end());
            out.insert(out.end(), rest.begin(), rest.end());
            out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*end), lines.end());
            return join_nl(out);
        }
    }
    return std::nullopt;
}

// for (;;) { if (A) { L: S; continue; } if (B) { goto L; } else { break; } } - a test or-ed with the first: while (A
// || B) { S } (a variant)
std::optional<std::string> or_loops(sv text) {
    static const Regex for_re(R"(([ \t]*)for \(;;\) \{)");
    static const Regex label_re(R"([ \t]*(L_\w+):)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = for_re.full_match(lines[k]);
        if (!m) continue;
        const auto e = dt::block_end(lines, k);
        if (!e || ct::trim(sv(lines[*e])) != "}") continue;
        const std::string ind = str((*m)[1]), i4 = ind + "    ";
        const auto p1 = if_at(lines, k + 1);
        if (!p1 || p1->else_arm) continue;
        const Lines& arm = p1->then_arm;
        const auto lab = arm.empty() ? std::nullopt : label_re.full_match(arm[0]);
        if (!lab || ct::trim(sv(arm.back())) != "continue;") continue;
        const auto p2 = if_at(lines, p1->end);
        const std::string label = str((*lab)[1]);
        if (!p2 || p2->end != *e) continue;
        std::vector<std::string> then_stmts, else_stmts;
        for (const std::string& x : content_of(p2->then_arm)) then_stmts.push_back(str(ct::trim(x)));
        if (p2->else_arm)
            for (const std::string& x : content_of(*p2->else_arm)) else_stmts.push_back(str(ct::trim(x)));
        if (then_stmts != std::vector<std::string>{"goto " + label + ";"} || else_stmts != std::vector<std::string>{"break;"}) continue;
        if (goto_mentioned(lines, 0, k, label) || goto_mentioned(lines, *e + 1, lines.size(), label)) continue;
        const std::string a = dt::truth("(" + p1->cond + ")"), b = dt::truth("(" + p2->cond + ")");
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back(ind + "while (" + a + " || " + b + ") {");
        for (std::size_t q = 1; q + 1 < arm.size(); ++q) out.push_back(arm[q].starts_with(i4 + "    ") ? arm[q].substr(4) : arm[q]);
        out.push_back(ind + "}");
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*e) + 1, lines.end());
        return join_nl(out);
    }
    return std::nullopt;
}

// a switch last in a loop's body whose arms go on with `continue;`: break (the same - the loop's end), and a default
// that only goes on gone (a variant: which the source wrote decides the layout)
std::optional<std::string> switch_breaks(sv text) {
    static const Regex loop_re(R"(([ \t]*)(?:while \(.*\)|for \(.*\)) \{)");
    static const Regex case_follows(R"(\s*(?:case [^:]+|default):$)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = loop_re.full_match(lines[k]);
        if (!m) continue;
        const auto e = dt::block_end(lines, k);
        if (!e) continue;
        const std::string i4 = str((*m)[1]) + "    ";
        std::optional<std::size_t> sw;
        for (std::size_t j = *e; j-- > k + 1 && !sw;)
            if (!blank(lines[j])) sw = j;
        if (!sw || lines[*sw] != i4 + "}") continue;
        std::optional<std::size_t> st;
        for (std::size_t j = *sw; j-- > k + 1 && !st;)
            if (lines[j].starts_with(i4 + "switch (") && lines[j].ends_with('{')) st = j;
        if (!st) continue;
        bool deep_continue = false;
        for (std::size_t j = *st; j < *sw; ++j)
            if (ct::trim(sv(lines[j])) == "continue;" && indent_of(lines[j]) > i4.size() + 4) deep_continue = true;
        if (deep_continue) continue;
        Lines next = lines;
        std::vector<char> gone(lines.size(), 0);
        bool changed = false;
        for (std::size_t j = *st + 1; j < *sw; ++j)
            if (next[j] == i4 + "    continue;") {
                next[j] = i4 + "    break;";
                changed = true;
            }
        for (std::size_t j = *st + 1; j < *sw;) {   // (a default that only breaks: none)
            if (next[j] == i4 + "default:" && j + 1 < *sw && next[j + 1] == i4 + "    break;" &&
                (j + 2 >= *sw || case_follows.match(next[j + 2]))) {
                gone[j] = gone[j + 1] = 1;
                changed = true;
                j += 2;
                continue;
            }
            ++j;
        }
        if (changed) return join_live(next, gone);
    }
    return std::nullopt;
}

// switch (..) { .. default: S; case K: return V; } - the default falling into a case that only returns: the case
// written first, S after the switch with its own return V (a variant: the source's layout of both)
std::optional<std::string> default_after(sv text) {
    static const Regex return_re(R"((?:\s*)return [^;\n]*;)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        const sv line = lines[k];
        std::size_t p = 0;
        while (p < line.size() && (line[p] == ' ' || line[p] == '\t')) ++p;
        if (!line.substr(p).starts_with("switch (") || !line.ends_with(") {") || line.size() < p + 8 + 3) continue;
        const std::string ind = str(line.substr(0, p));
        std::optional<std::size_t> end;
        for (std::size_t j = k + 1; j < lines.size() && !end; ++j)
            if (lines[j] == ind + "}") end = j;
        if (!end) continue;
        const Lines body(lines.begin() + static_cast<std::ptrdiff_t>(k) + 1, lines.begin() + static_cast<std::ptrdiff_t>(*end));
        const std::string deeper = ind + "    ";
        for (std::size_t i = 0; i < body.size(); ++i) {
            if (body[i] != ind + "default:") continue;
            std::size_t j = i + 1;
            while (j < body.size() && body[j].starts_with(deeper)) ++j;   // (the default's statements)
            if (j == i + 1 || j + 1 >= body.size() || !is_case_label(body[j], ind) || body[j].find("default") != npos) continue;
            const std::string label = body[j].substr(ind.size());
            const std::string& ret = body[j + 1];
            if (!ret.starts_with(deeper + "return ") || !ret.ends_with(';') || std::ranges::count(ret, ';') != 1) continue;
            bool leaves = false;
            for (std::size_t q = i + 1; q < j; ++q)
                for (const sv word : {"break", "return", "goto"})
                    if (sv(body[q]).substr(deeper.size()).starts_with(word) && boundary_at(body[q], deeper.size() + word.size())) leaves = true;
            if (leaves) continue;
            bool more = false;   // (the return is the last statement of the switch: only blank lines follow it)
            for (std::size_t q = j + 2; q < body.size(); ++q)
                if (!blank(body[q])) more = true;
            if (more) continue;
            const std::string statement = ret.substr(deeper.size());
            Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k) + 1);
            out.push_back(ind + label);
            out.push_back(deeper + statement);
            out.insert(out.end(), body.begin(), body.begin() + static_cast<std::ptrdiff_t>(i));
            out.push_back(ind + "}");
            for (std::size_t q = i + 1; q < j; ++q) out.push_back(drop4(body[q]));
            out.push_back(ind + statement);
            out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*end) + 1, lines.end());
            return join_nl(out);
        }
    }
    return std::nullopt;
}

namespace {

// The run of `r = V;` statements an arm ends with: {r: (V, indentation)} in the order met from the end, and the line
// where the run starts.
struct ArmTail {
    std::vector<std::pair<std::string, std::pair<std::string, std::string>>> got;
    std::size_t start = 0;
    bool has(const std::string& r) const {
        return std::ranges::any_of(got, [&](const auto& g) { return g.first == r; });
    }
    const std::pair<std::string, std::string>& at(const std::string& r) const {
        return std::ranges::find(got, r, &decltype(got)::value_type::first)->second;
    }
};

ArmTail arm_tail(const Lines& arm) {
    static const Regex assign(R"((\s*)(ax|bx|cx|dx|t\d+) = (.+);)");
    ArmTail t;
    std::ptrdiff_t j = static_cast<std::ptrdiff_t>(arm.size()) - 1;
    while (j >= 0 && blank(arm[static_cast<std::size_t>(j)])) --j;
    while (j >= 0) {
        const auto m = assign.full_match(arm[static_cast<std::size_t>(j)]);
        if (!m || t.has(str((*m)[2]))) break;
        t.got.push_back({str((*m)[2]), {str((*m)[3]), str((*m)[1])}});
        --j;
    }
    t.start = static_cast<std::size_t>(j + 1);
    return t;
}

}  // namespace

// if (C) { ..; r = A; } else { ..; r = B; } S(r); - the statement after, taking what each arm leaves in r, in each arm
// with its value: the compiler's one copy of a tail both arms ended with (a variant)
std::optional<std::string> push_tails(sv text) {
    static const Regex statement_start(R"(\s*(?:if|while|for|switch|\}|L_\w+:))");
    static const Regex plain(R"([\w>.-]+)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm || p->end >= lines.size()) continue;
        const ArmTail ga = arm_tail(p->then_arm), gb = arm_tail(*p->else_arm);
        const std::string& next = lines[p->end];
        std::vector<std::string> regs;
        for (const auto& g : ga.got)
            if (gb.has(g.first)) regs.push_back(g.first);
        bool same_set = ga.got.size() == gb.got.size();
        for (const auto& g : ga.got)
            if (!gb.has(g.first)) same_set = false;
        if (regs.empty() || !same_set || statement_start.match(next)) continue;
        bool blocked = false;
        for (const std::string& r : regs)
            if (count_word(next, r) != 1 || dt::live_after(lines, p->end + 1, r)) blocked = true;
        if (blocked) continue;
        const auto put = [&](const ArmTail& got) {
            std::string s = str(ct::trim(next));
            for (const std::string& r : regs) {
                const std::string& v = got.at(r).first;
                s = replace_word(s, r, plain.full_match(v) ? v : "(" + v + ")", true);
            }
            return got.at(regs.front()).second + s;
        };
        Lines then(p->then_arm.begin(), p->then_arm.begin() + static_cast<std::ptrdiff_t>(ga.start));
        then.push_back(put(ga));
        Lines els(p->else_arm->begin(), p->else_arm->begin() + static_cast<std::ptrdiff_t>(gb.start));
        els.push_back(put(gb));
        const std::string i0 = lead_ws(lines[k]);
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back(lines[k]);
        out.insert(out.end(), then.begin(), then.end());
        out.push_back(i0 + "} else {");
        out.insert(out.end(), els.begin(), els.end());
        out.push_back(i0 + "}");
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end) + 1, lines.end());
        std::string t = join_nl(out);
        for (const std::string& r : regs)
            if (r.starts_with('t')) t = drop_decl(t, r);
        return t;
    }
    return std::nullopt;
}

// if (A) { if (B) { S } } else { S } - S reached both ways (the compiler's one copy): if (!A || B) { S }
std::string shared_arms(sv text) {
    Lines lines = split_nl(text);
    for (bool changed = true; changed;) {
        changed = false;
        for (std::size_t k = 0; k < lines.size(); ++k) {
            const auto p = dt::if_parts(lines, k);
            if (!p || !p->else_arm) continue;
            const Lines th = content_of(p->then_arm);
            if (th.empty()) continue;
            const auto q = dt::if_parts(th, 0);
            if (!q || q->else_arm || q->end != th.size()) continue;
            Lines inner;
            for (const std::string& x : q->then_arm)
                if (!blank(x)) inner.push_back(drop4(x));
            if (inner != content_of(*p->else_arm)) continue;
            const std::string ind = lead_ws(lines[k]);
            const std::string cond = dt::join_tests("||", dt::truth(dt::negate("(" + p->cond + ")")), dt::truth("(" + q->cond + ")"));
            Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
            out.push_back(ind + "if (" + cond + ") {");
            out.insert(out.end(), p->else_arm->begin(), p->else_arm->end());
            out.push_back(ind + "}");
            out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
            lines = std::move(out);
            changed = true;
            break;
        }
    }
    return join_nl(lines);
}

namespace {

// `    if (..) {` at the function body's own level.
bool is_body_if(sv line) { return line.starts_with("    if (") && line.ends_with('{'); }

// The statements of an arm, one level out.
Lines dedent_arm(const Lines& arm) {
    Lines out;
    for (const std::string& x : arm) out.push_back(x.starts_with("        ") ? x.substr(4) : x);
    return out;
}

}  // namespace

// if (C) { S } return X; last in the function: if (!C) return X; S return X; (a variant: a guard returning at once is
// laid out its own way)
std::optional<std::string> guard_returns(sv text) {
    Lines lines = split_nl(text);
    static const Regex return_line(R"(    return [^;]+;)");
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!is_body_if(lines[k])) continue;
        const auto p = dt::if_parts(lines, k);
        if (!p || p->else_arm) continue;
        Lines rest;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) rest.push_back(lines[i]);
        if (rest.size() != 2 || !return_line.full_match(rest[0]) || rest[1] != "}") continue;
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back("    if " + guard_condition(*p) + " " + str(ct::trim(sv(rest[0]))));
        for (const std::string& x : dedent_arm(p->then_arm)) out.push_back(x);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        return join_nl(out);
    }
    // (if (C) { S } else { return x; } last: the same guard; then an arm ending in a return its else flattened after it)
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!is_body_if(lines[k])) continue;
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) continue;
        Lines tail;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) tail.push_back(lines[i]);
        if (tail != Lines{"}"}) continue;
        Lines el;
        for (const std::string& x : *p->else_arm)
            if (!blank(x)) el.push_back(str(ct::trim(sv(x))));
        const Lines th = content_of(p->then_arm);
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        if (el.size() == 1 && el[0].starts_with("return")) {
            out.push_back("    if " + guard_condition(*p) + " " + el[0]);
            for (const std::string& x : dedent_arm(p->then_arm)) out.push_back(x);
        } else if (!th.empty() && ct::ltrim(sv(th.back())).starts_with("return") && th.back().starts_with("        return")) {
            out.push_back(lines[k]);
            out.insert(out.end(), p->then_arm.begin(), p->then_arm.end());
            out.push_back("    }");
            for (const std::string& x : dedent_arm(*p->else_arm)) out.push_back(x);
        } else {
            continue;
        }
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        const std::string t = join_nl(out);
        return guard_returns(t).value_or(t);
    }
    return std::nullopt;
}

// the first guard guard_returns makes (if (C) { S } else { return x; } last: if (!C) return x; S), the returns after
// it only flattened - not made guards again (a variant)
std::optional<std::string> guard_flat(sv text, const FlatReturns& flat_returns) {
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!is_body_if(lines[k])) continue;
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) continue;
        Lines tail;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) tail.push_back(lines[i]);
        if (tail != Lines{"}"}) continue;
        Lines el;
        for (const std::string& x : *p->else_arm)
            if (!blank(x)) el.push_back(str(ct::trim(sv(x))));
        if (el.size() != 1 || !el[0].starts_with("return")) continue;
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back("    if " + guard_condition(*p) + " " + el[0]);
        for (const std::string& x : dedent_arm(p->then_arm)) out.push_back(x);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        const std::string t = join_nl(out);
        return flat_returns(t).value_or(t);
    }
    return std::nullopt;
}

// if (C) { S } [else { }] last, the function falling off its end the other way - S returning K somewhere: if (!C)
// return K; S (a variant: the way out the compiler shares with S's return of K, the model reading the jump to it as
// the end)
std::optional<std::string> guard_shared(sv text, const FlatReturns& flat_returns) {
    static const Regex return_const(R"(\s*return (-?\d+);$)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (!is_body_if(lines[k])) continue;
        const auto p = dt::if_parts(lines, k);
        if (!p || (p->else_arm && has_content(*p->else_arm))) continue;
        Lines tail;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) tail.push_back(lines[i]);
        if (tail != Lines{"}"}) continue;
        std::optional<std::string> k0;
        for (const std::string& x : p->then_arm)
            if (const auto m = return_const.match(x)) {
                k0 = str((*m)[1]);
                break;
            }
        if (!k0) continue;
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back("    if " + guard_condition(*p) + " return " + *k0 + ";");
        for (const std::string& x : dedent_arm(p->then_arm)) out.push_back(x);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        const std::string t = join_nl(out);
        return flat_returns(t).value_or(t);
    }
    return std::nullopt;
}

namespace {

// What an if-tree gives a register: 0 or 1, a goto to a labelled part of the tree, or a test with two such parts.
struct BoolExpr;
using BoolExprP = std::shared_ptr<const BoolExpr>;
struct BoolExpr {
    enum class Kind { Const, Goto, If } kind = Kind::Const;
    std::string text;   // the constant ("0"/"1"), the label, or the test
    BoolExprP then_part, else_part;
};
bool same_expr(const BoolExprP& a, const BoolExprP& b) {
    if (a == b) return true;
    if (!a || !b || a->kind != b->kind || a->text != b->text) return false;
    return same_expr(a->then_part, b->then_part) && same_expr(a->else_part, b->else_part);
}

struct BoolTree {
    std::map<std::string, BoolExprP> labels;

    struct Stmt {
        enum class Kind { Label, If, Line } kind;
        std::string text;
        std::optional<dt::IfParts> parts;
    };
    static std::vector<Stmt> statements(const Lines& block) {   // (the statements of a block)
        static const Regex label(R"([ \t]*(L_\w+):)");
        std::vector<Stmt> out;
        for (std::size_t j = 0; j < block.size();) {
            if (blank(block[j])) {
                ++j;
                continue;
            }
            if (const auto m = label.full_match(block[j])) {
                out.push_back({Stmt::Kind::Label, str((*m)[1]), std::nullopt});
                ++j;
                continue;
            }
            if (auto p = dt::if_parts(block, j)) {
                const std::size_t end = p->end;
                out.push_back({Stmt::Kind::If, "", std::move(p)});
                j = end;
                continue;
            }
            out.push_back({Stmt::Kind::Line, str(ct::trim(sv(block[j]))), std::nullopt});
            ++j;
        }
        return out;
    }

    // The expression a block gives `reg`, or null.
    BoolExprP node(const Lines& block, const std::string& reg) {
        static const Regex go(R"(goto (L_\w+);)");
        std::vector<Stmt> ss = statements(block);
        std::optional<std::string> lab;
        if (!ss.empty() && ss[0].kind == Stmt::Kind::Label) {
            lab = ss[0].text;
            ss.erase(ss.begin());
        }
        if (ss.size() != 1) return nullptr;
        const Stmt& s = ss[0];
        BoolExprP e;
        if (s.kind == Stmt::Kind::Label) {   // (a label and then only a label: no statement, no expression)
            return nullptr;
        } else if (s.kind == Stmt::Kind::Line) {
            if (s.text == reg + " = 0;" || s.text == reg + " = 1;") {
                e = std::make_shared<BoolExpr>(BoolExpr{BoolExpr::Kind::Const, s.text.substr(reg.size() + 3, 1), nullptr, nullptr});
            } else if (const auto g = go.full_match(s.text)) {
                e = std::make_shared<BoolExpr>(BoolExpr{BoolExpr::Kind::Goto, str((*g)[1]), nullptr, nullptr});
            } else {
                return nullptr;
            }
        } else {
            const dt::IfParts& d = *s.parts;
            if (!d.else_arm) return nullptr;
            const BoolExprP a = node(d.then_arm, reg), b = node(*d.else_arm, reg);
            if (!a || !b) return nullptr;
            e = std::make_shared<BoolExpr>(BoolExpr{BoolExpr::Kind::If, d.cond, a, b});
        }
        if (lab) labels[*lab] = e;
        return e;
    }

    // The expression as a C test, or nullopt.
    std::optional<std::string> text_of(const BoolExprP& e, int depth = 0) const {
        if (depth > 200) return std::nullopt;   // (labels that go round: no test)
        if (e->kind == BoolExpr::Kind::Const) return e->text;
        if (e->kind == BoolExpr::Kind::Goto) {
            const auto it = labels.find(e->text);
            if (it == labels.end() || same_expr(it->second, e)) return std::nullopt;
            return text_of(it->second, depth + 1);
        }
        const std::string c = e->text.starts_with('(') ? e->text : "(" + dt::strip_parens("(" + e->text + ")") + ")";
        const auto a = text_of(e->then_part, depth + 1), b = text_of(e->else_part, depth + 1);
        if (!a || !b) return std::nullopt;
        const std::string t = dt::truth(c), f = dt::truth(dt::negate(c));
        if (*b == "0") return *a == "1" ? t : *a == "0" ? std::string("0") : dt::join_tests("&&", t, *a);
        if (*a == "1") return dt::join_tests("||", t, *b);
        if (*a == "0") return *b == "1" ? f : dt::join_tests("&&", f, *b);
        if (*b == "1") return dt::join_tests("||", f, *a);
        std::vector<std::string> parts, tail;
        for (const std::string& x : dt::top_split(*a, "&&")) parts.push_back(str(ct::trim(x)));
        for (const std::string& x : dt::top_split(*b, "&&")) tail.push_back(str(ct::trim(x)));
        if (parts.size() > tail.size() && std::equal(tail.begin(), tail.end(), parts.end() - static_cast<std::ptrdiff_t>(tail.size()))) {
            // (if (c) { X && T } else { T }: (!c || X) && T)
            const std::string x = ct::join(std::vector<std::string>(parts.begin(), parts.end() - static_cast<std::ptrdiff_t>(tail.size())), " && ");
            return dt::join_tests("&&", "(" + dt::join_tests("||", f, x) + ")", *b);
        }
        return std::nullopt;
    }
};

}  // namespace

// an if-tree whose every way ends giving r 1 or 0 (a goto to a labelled part of it the same part again): if (E) { r =
// 1; } else { r = 0; } - E the && / || of its tests, a part both ways share factored out (if (c) { X && T } else { T
// }: (!c || X) && T) - the compiler's one copy of the tests read back
std::string bool_trees(sv text) {
    static const Regex return_reg(R"([ \t]*return (\w+);)");
    Lines lines(split_nl(text));
    dt::else_ifs(lines);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) continue;
        std::string next;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) {
                next = lines[i];
                break;
            }
        const auto r = return_reg.full_match(next);
        if (!r) continue;
        BoolTree tree;
        const std::string reg = str((*r)[1]);
        const BoolExprP e = tree.node(Lines(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(p->end)), reg);
        // (one test alone: as written - its sense is the code's)
        if (!e || e->kind != BoolExpr::Kind::If ||
            (e->then_part->kind == BoolExpr::Kind::Const && e->else_part->kind == BoolExpr::Kind::Const))
            continue;
        const auto x = tree.text_of(e);
        if (!x || *x == "0" || *x == "1" || x->find("goto") != npos) continue;
        const std::string ind = lead_ws(lines[k]);
        const bool wrapped = x->starts_with('(') && x->ends_with(')') && dt::balanced(sv(*x).substr(1, x->size() - 2));
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back(ind + "if " + (wrapped ? *x : "(" + *x + ")") + " {");
        out.push_back(ind + "    " + reg + " = 1;");
        out.push_back(ind + "} else {");
        out.push_back(ind + "    " + reg + " = 0;");
        out.push_back(ind + "}");
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        return join_nl(out);
    }
    return str(text);
}

// if (A) { X; if (B) { T } } else { T } at the function's end - the compiler's one copy of T both ways reach: if (A) {
// X; if (!B) return; } T (a variant)
std::optional<std::string> shared_tails(sv text) {
    static const Regex return_line(R"([ \t]*return( \w+)?;)");
    const Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm || p->then_arm.empty()) continue;
        const std::string ind = lead_ws(lines[k]);
        Lines rest;
        for (std::size_t i = p->end; i < lines.size(); ++i)
            if (!blank(lines[i])) rest.push_back(lines[i]);
        if (rest.empty() || !(rest[0] == "}" || (return_line.full_match(rest[0]) && rest.size() > 1 && rest[1] == "}"))) continue;
        const Lines& then = p->then_arm;
        const auto is_inner_if = [&](std::size_t j) {
            return then[j].starts_with(ind + "    if (") && then[j].ends_with('{') && indent_of(then[j]) == ind.size() + 4;
        };
        std::optional<std::size_t> q;
        std::optional<dt::IfParts> pi;
        for (std::size_t j = 0; j < then.size() && !q; ++j) {   // (the last if of the then-arm, nothing after it)
            if (!is_inner_if(j)) continue;
            pi = dt::if_parts(then, j);
            if (pi && !pi->else_arm && pi->end == then.size()) q = j;
        }
        if (!q) continue;
        Lines inner, wanted;
        for (const std::string& x : pi->then_arm)
            if (const std::string d = drop4(x); !blank(d)) inner.push_back(str(ct::rtrim(d)));
        for (const std::string& x : *p->else_arm)
            if (!blank(x)) wanted.push_back(str(ct::rtrim(sv(x))));
        if (inner != wanted) continue;
        const std::string neg = dt::negate("(" + pi->cond + ")");
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
        out.push_back(lines[k]);
        out.insert(out.end(), then.begin(), then.begin() + static_cast<std::ptrdiff_t>(*q));
        out.push_back(ind + "    if (" + (neg.starts_with('(') ? neg : "(" + neg + ")") + ") return;");
        out.push_back(ind + "}");
        for (const std::string& x : *p->else_arm) out.push_back(drop4(x));
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
        return join_nl(out);
    }
    return std::nullopt;
}

// a goto past statements under a test - the compiler's || of a test that skips them: if (C1) { if (C2) goto L; } S..
// L: -> if (!C1 || !C2) { S.. } if (C) goto L; S.. L: -> if (!C) { S.. } (L then wanted by none else: dropped)
std::string guard_gotos(sv text) {
    static const Regex block_if(R"((\s*)if \((.*)\) \{$)");
    static const Regex goto_if(R"((\s*)if \((.*)\) goto (L_\w+);$)");
    static const Regex label_line(R"(\s*L_\w+:)");
    Lines lines = split_nl(text);
    for (bool changed = true; changed;) {
        changed = false;
        for (std::size_t k = 0; k < lines.size(); ++k) {
            const auto m = block_if.match(lines[k]);
            std::optional<RegexMatch> g;
            if (m && k + 2 < lines.size()) g = goto_if.match(lines[k + 1]);
            const auto one = goto_if.match(lines[k]);
            std::string ind, lab, cond;
            std::size_t start = 0;
            if (m && g && lines[k + 2] == str((*m)[1]) + "}") {
                ind = str((*m)[1]);
                lab = str((*g)[3]);
                cond = str(dt::truth(dt::negate("(" + str((*m)[2]) + ")"))) + " || " + dt::truth(dt::negate("(" + str((*g)[2]) + ")"));
                start = k + 3;
            } else if (one) {
                ind = str((*one)[1]);
                lab = str((*one)[3]);
                cond = dt::truth(dt::negate("(" + str((*one)[2]) + ")"));
                start = k + 1;
            } else {
                continue;
            }
            std::optional<std::size_t> end;
            for (std::size_t j = start; j < lines.size() && !end; ++j)
                if (lines[j] == ind + lab + ":") end = j;
            if (!end || *end == start) continue;
            bool ok = true;
            std::int64_t depth = 0;
            for (std::size_t j = start; j < *end; ++j) {
                const std::string& l = lines[j];
                if ((!blank(l) && !l.starts_with(ind)) || label_line.match(l)) ok = false;
                depth += brace_balance(l);   // (the statements between: whole ones)
                if (depth < 0) ok = false;
            }
            if (!ok || depth != 0) continue;
            Lines made{ind + "if (" + cond + ") {"};
            for (std::size_t j = start; j < *end; ++j) made.push_back(blank(lines[j]) ? lines[j] : "    " + lines[j]);
            made.push_back(ind + "}");
            Lines next(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
            next.insert(next.end(), made.begin(), made.end());
            Lines after(lines.begin() + static_cast<std::ptrdiff_t>(*end) + 1, lines.end());
            Lines with_label(lines.begin() + static_cast<std::ptrdiff_t>(*end), lines.end());
            Lines rest = next;
            rest.insert(rest.end(), after.begin(), after.end());
            if (goto_mentioned(rest, lab)) {   // (wanted elsewhere: kept)
                rest = next;
                rest.insert(rest.end(), with_label.begin(), with_label.end());
            }
            lines = std::move(rest);
            changed = true;
            break;
        }
    }
    return join_nl(lines);
}

// ifs nested as the compiler's || and && leave them, joined again: if (A) { if (B) { P } else { Q } } else { Q } -> if
// (A && B) { P } else { Q } if (A) { if (B) { P } else { Q } } else { P } -> if (!A || B) { P } else { Q } if (A) { if
// (B) { P } } -> if (A && B) { P } (the arms the model wrote twice, where both ways go)
std::string merge_conditions(sv text) {
    static const Regex goto_re(R"(\bgoto (L_\w+);)");
    static const Regex label_re(R"([ \t]*(L_\w+):)");
    Lines lines(split_nl(text));
    dt::else_ifs(lines);
    for (bool changed = true; changed;) {
        changed = false;
        for (std::size_t k = 0; k < lines.size(); ++k) {
            const auto p = dt::if_parts(lines, k);
            if (!p) continue;
            const std::string ind = lead_ws(lines[k]);
            if (!has_content(p->then_arm)) continue;
            std::size_t first = 0;
            while (blank(p->then_arm[first])) ++first;
            const auto q = dt::if_parts(p->then_arm, first);
            if (!q) continue;
            if (q->end != p->then_arm.size() &&
                std::any_of(p->then_arm.begin() + static_cast<std::ptrdiff_t>(q->end), p->then_arm.end(), [](const std::string& l) { return !blank(l); }))
                continue;
            const auto dd = [](const Lines& ls) {
                Lines out;
                for (const std::string& l : ls) out.push_back(dedent4(l));
                return out;
            };
            const std::string c1 = "(" + p->cond + ")", c2 = "(" + q->cond + ")";
            const Lines& els = *(p->else_arm ? &*p->else_arm : &p->then_arm);   // (placeholder: only used when else_arm is set)
            std::optional<Lines> made;
            const auto build = [&](const std::string& cond, const Lines& then_lines, const Lines* else_lines) {
                Lines out{ind + "if (" + cond + ") {"};
                out.insert(out.end(), then_lines.begin(), then_lines.end());
                if (else_lines) {
                    out.push_back(ind + "} else {");
                    out.insert(out.end(), else_lines->begin(), else_lines->end());
                }
                out.push_back(ind + "}");
                return out;
            };
            if (!p->else_arm && !q->else_arm) {
                made = build(dt::join_tests("&&", dt::truth(c1), dt::truth(c2)), dd(q->then_arm), nullptr);
            } else if (p->else_arm && q->else_arm && dt::same_arm(els, dd(*q->else_arm))) {
                const Lines other = dt::unlabelled(dd(*q->else_arm), els);
                made = build(dt::join_tests("&&", dt::truth(c1), dt::truth(c2)), dd(q->then_arm), &other);
            } else if (p->else_arm && q->else_arm && dt::same_arm(els, dd(q->then_arm))) {
                const Lines first_arm = dt::unlabelled(dd(q->then_arm), els);
                const Lines second = dd(*q->else_arm);
                made = build(dt::join_tests("||", dt::truth(dt::negate(c1)), dt::truth(c2)), first_arm, &second);
            }
            if (!made) continue;
            Lines cand(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(k));
            cand.insert(cand.end(), made->begin(), made->end());
            cand.insert(cand.end(), lines.begin() + static_cast<std::ptrdiff_t>(p->end), lines.end());
            // (a label another goto still wants: kept, else not this way)
            std::set<std::string> labels;
            for (const std::string& l : cand)
                if (const auto lm = label_re.full_match(l)) labels.insert(str((*lm)[1]));
            bool all_wanted_kept = true;
            const std::string whole = join_nl(cand);
            goto_re.for_each_match(whole, [&](const RegexMatch& gm) {
                if (!labels.contains(str(gm[1]))) all_wanted_kept = false;
                return true;
            });
            if (!all_wanted_kept) continue;
            lines = std::move(cand);
            changed = true;
            break;
        }
    }
    return join_nl(lines);
}

// a case that goes on into the next (`goto L;` to the label the next case starts with): it falls into it - an arm's
// last `goto L;` is dropped, and `if (C) { X } else { goto L; } break;` is `if (C) { X break; }`
std::string case_falls(sv text) {
    static const Regex label_re(R"((\s*)(L_\w+):$)");
    static const Regex leaves(R"(\s*(?:return|break|continue|goto)\b)");
    static const Regex goto_line(R"((\s*)goto (L_\w+);)");
    static const Regex next_is_case_or_close(R"(\s*(?:case [^:]+|default):$|\s*\}$)");
    static const Regex ends_run(R"(\s*(?:break;|return\b))");
    static const Regex inside_run(R"(\s*(?:case [^:]+|default):$|\s*L_\w+:)");
    Lines lines = split_nl(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = label_re.match(lines[k]);
        std::size_t c = k;
        while (c > 0 && is_any_case_label(lines[c - 1])) --c;
        if (!m || c == k || c == 0) {   // (a case label on the very first line has nothing before it to drop)
            ++k;
            continue;
        }
        const std::string lab = str((*m)[2]);
        const std::string i4 = lead_ws(lines[c]) + "    ";
        if (lines[c - 1] == i4 + "goto " + lab + ";") {
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(c) - 1);
            --k;
        } else if (c >= 5 && lines[c - 4] == i4 + "} else {" && lines[c - 3] == i4 + "    goto " + lab + ";" && lines[c - 2] == i4 + "}" &&
                   lines[c - 1] == i4 + "break;" && !leaves.match(lines[c - 5])) {
            lines[c - 4] = i4 + "    break;";
            lines[c - 3] = i4 + "}";
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(c) - 2, lines.begin() + static_cast<std::ptrdiff_t>(c));
            k -= 2;
        } else {
            ++k;
            continue;
        }
        if (!goto_mentioned(lines, lab)) lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
        ++k;
    }
    // (an arm's last goto to another case's start, not the next: the statements there to their break, copied - the
    //  compiler's one copy of a tail both cases end with)
    for (int round = 0; round < 4; ++round) {
        bool hit = false;
        for (std::size_t j = 0; j < lines.size() && !hit; ++j) {
            const auto g = goto_line.full_match(lines[j]);
            if (!g || j + 1 >= lines.size() || !next_is_case_or_close.match(lines[j + 1])) continue;
            const std::string target = str((*g)[2]);
            std::optional<std::size_t> at;
            for (std::size_t i = 0; i < lines.size() && !at; ++i)
                if (ct::trim(sv(lines[i])) == target + ":") at = i;
            if (!at || *at <= j) continue;
            std::optional<std::size_t> end;
            for (std::size_t i = *at + 1; i < lines.size() && !end; ++i)
                if (ends_run.match(lines[i])) end = i;
            if (!end) continue;
            bool crosses = false;
            for (std::size_t i = *at + 1; i < *end; ++i)
                if (inside_run.match(lines[i])) crosses = true;
            if (crosses) continue;
            const std::size_t src_ind = *at + 1 <= *end ? indent_of(lines[*at + 1]) : 0;
            Lines copy;
            for (std::size_t i = *at + 1; i <= *end; ++i)
                copy.push_back(indent_of(lines[i]) >= src_ind ? str((*g)[1]) + lines[i].substr(src_ind) : lines[i]);
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(j));
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(j), copy.begin(), copy.end());
            if (!goto_mentioned(lines, target)) {
                Lines kept;
                for (const std::string& x : lines)
                    if (ct::trim(sv(x)) != target + ":") kept.push_back(x);
                lines = std::move(kept);
            }
            hit = true;
        }
        if (!hit) break;
    }
    return join_nl(lines);
}

// a register variable first given a value after other statements: declared in a block from there (to before the
// function's last return) - a variant: before the block the compiler has the register free for its own use
std::optional<std::string> inner_registers(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    static const Regex decl(R"(    register ([\w ]+?\**) ?(\w+);$)");
    static const Regex block_start(R"(\s*(?:register |int |char |unsigned |long |struct ))");
    const Lines lines = split_nl(split.body);
    for (std::size_t j = 0; j < lines.size(); ++j) {
        const auto m = decl.match(lines[j]);
        if (!m) continue;
        const std::string x = str((*m)[2]);
        std::optional<std::size_t> first;
        for (std::size_t k = j + 1; k < lines.size() && !first; ++k)
            if (has_word(lines[k], x)) first = k;
        if (!first || !lines[*first].starts_with("    " + x + " = ")) continue;
        bool any_stmt = false;
        for (std::size_t k = j + 1; k < *first; ++k)
            if (!blank(lines[k]) && !block_start.match(lines[k])) any_stmt = true;
        if (!any_stmt) continue;
        std::optional<std::size_t> end;
        for (std::size_t k = 0; k < lines.size(); ++k)
            if (lines[k].starts_with("    return") && (lines[k].size() == 10 || !is_word(lines[k][10]))) end = k;
        if (!end || *end <= *first) continue;
        const std::string type = str((*m)[1]);
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(j));
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(j) + 1, lines.begin() + static_cast<std::ptrdiff_t>(*first));
        out.push_back("    {");
        out.push_back("        register " + (type.ends_with('*') ? type : type + " ") + x + ";");
        for (std::size_t k = *first; k < *end; ++k) out.push_back(blank(lines[k]) ? lines[k] : "    " + lines[k]);
        out.push_back("    }");
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(*end), lines.end());
        return split.head + split.brace + join_nl(out);
    }
    return std::nullopt;
}

// the shapes a compiler gives a statement, read back: a loop whose test it moved to the bottom (`goto L; do {..} while
// (c);` with no L - the jump goes to the test) is `while (c) {..}`; a step then a read one back (`p = p + 1; ..
// p[-1]`) is `*p++`
std::string shapes(sv text_in) {
    static const Regex goto_re(R"((\s*)goto (L_\w+);\s*$)");
    static const Regex while_tail(R"((\s*)\} while \((.*)\);\s*$)");
    static const Regex return_re(R"(\s*return\b.*;\s*$)");
    static const Regex label_re(R"(\s*L_\w+:)");
    static const Regex step_re(R"(\s*(\w+) = \1 \+ 1;\s*$)");
    // a loop whose test the compiler moved to the bottom: `goto L; do { .. } while (c);` with no L: while (c) { .. }
    const std::string text0 = str(text_in);
    Lines lines = split_nl(text0);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        const std::string& l = lines[k];
        const auto m = goto_re.match(l);
        if (m && k + 1 < lines.size() && ct::trim(sv(lines[k + 1])) == "do {") {
            const std::string label_here = str((*m)[2]) + ":";   // `^\s*L:` anywhere in the text
            bool defined = false;
            for (const std::string& x : lines)
                if (ct::ltrim(sv(x)).starts_with(label_here)) defined = true;
            if (!defined) {
                std::int64_t depth = 0;
                std::optional<std::size_t> close;
                for (std::size_t j = k + 1; j < lines.size(); ++j) {   // (the do's own closing line)
                    depth += brace_balance(lines[j]);
                    if (depth == 0) {
                        close = j;
                        break;
                    }
                }
                if (close) {
                    if (const auto mm = while_tail.match(lines[*close])) {
                        out.push_back(str((*m)[1]) + "while (" + str((*mm)[2]) + ") {");
                        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(k) + 2, lines.begin() + static_cast<std::ptrdiff_t>(*close));
                        out.push_back(str((*m)[1]) + "}");
                        k = *close + 1;
                        continue;
                    }
                }
            }
        }
        out.push_back(l);
        ++k;
    }
    // after a return, what stands in the same block before its end (no label) is never reached
    Lines keep;
    std::optional<std::size_t> dead;
    for (const std::string& l : out) {
        const std::size_t ind = l.find_first_not_of(' ') == npos ? l.size() : l.find_first_not_of(' ');
        if (dead) {
            if (ind < *dead || label_re.match(l)) dead = std::nullopt;   // (out of the return's block, or the next statement after a braceless if's)
            else continue;
        }
        keep.push_back(l);
        if (return_re.match(l)) dead = ind;
    }
    // a step, then the value one back read: the post-increment (in a do's test, or the next statement)
    lines = std::move(keep);
    std::vector<char> gone(lines.size(), 0);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const auto m = step_re.match(lines[i]);
        if (!m) continue;
        const std::string v = str((*m)[1]);
        for (std::size_t j = i + 1; j < std::min(i + 4, lines.size()); ++j) {
            const std::string back = v + "[-1]";
            std::size_t at = npos;
            for (std::size_t p = lines[j].find(back); p != npos && at == npos; p = lines[j].find(back, p + 1))
                if (boundary_at(lines[j], p)) at = p;
            if (at != npos) {
                lines[j] = lines[j].substr(0, at) + "*" + v + "++" + lines[j].substr(at + back.size());
                gone[i] = 1;
                break;
            }
            const sv stripped = ct::trim(sv(lines[j]));
            if (stripped != "}" && !stripped.empty() && !stripped.starts_with("} while")) break;
        }
    }
    return join_live(lines, gone);
}

// ---- the frame's filler -----------------------------------------------------------------------------------------

// the frame one word bigger: the trailing filler two bytes more, or one added after the last local (a variant: a local
// the model's frame left out)
std::optional<std::string> frame_more(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    static const Regex decl_line(
        R"((?m)^[ \t]*(?!return\b)(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;\n)");
    static const Regex first_statement(R"((?m)^[ \t]*(?!register|unsigned|int|char|long|short|struct)\S)");
    static const Regex filler(R"(([ \t]*)char (_p\w*)\[(\d+)\];\n)");
    const std::string& body = split.body;
    const auto limit_match = first_statement.search(body);
    const std::size_t limit = limit_match ? limit_match->start() : body.size();
    std::optional<RegexMatch> last;
    decl_line.for_each_match(body, [&](const RegexMatch& m) {
        if (m.start() < limit) last = m;
        return true;
    });
    if (!last) return std::nullopt;
    std::string replacement;
    if (const auto f = filler.full_match(last->whole()))
        replacement = std::format("{}char {}[{}];\n", (*f)[1], (*f)[2], to_int((*f)[3]) + 2);
    else
        replacement = str(last->whole()) + "    char _pm[2];\n";
    return split.head + split.brace + body.substr(0, last->start()) + replacement + body.substr(last->end());
}

// the frame's first local vN (bp-N) with room between it and bp: a filler declared before it (a variant: unused locals
// the source declared first, the model's layout taking the first one for read)
std::optional<std::string> lead_filler(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    static const Regex local(R"((?m)^([ \t]*)((?:unsigned\s+)?(int|char|long|short)|(?:\w+ )?\w+ \*) ?v(\d+);$)");
    static const Regex first_decl(
        R"((?m)^[ \t]*(?!register\b)(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;$)");
    const std::string& body = split.body;
    const auto m = local.search(body);
    const auto first = first_decl.search(body);
    if (!m || !first || first->start() != m->start()) return std::nullopt;
    const sv type = m->has(3) ? (*m)[3] : sv();
    const std::int64_t size = type == "long" ? 4 : type == "char" ? 1 : 2;
    const std::int64_t gap = to_int((*m)[4]) - size;
    if (gap <= 0) return std::nullopt;
    std::string fill;   // (words: an array goes elsewhere in the frame)
    for (std::int64_t j = 0; j < gap / 2; ++j) fill += std::format("{}int _q{};\n", (*m)[1], 2 * j);
    if (gap % 2) fill += std::format("{}char _q{};\n", (*m)[1], gap);
    return split.head + split.brace + body.substr(0, m->start()) + fill + body.substr(m->start());
}

}  // namespace cb::exact::draft::forms
