// cb-unity: alone (file-local helpers shared by name with the other shapes_*.cpp files)
// The passes that read back switches: a run of values the compiler tests as a range, an arm that falls into the next,
// the default arm that is the code after the switch, a selector kept in ax.
#include <algorithm>

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

// `case K:` / `default:` after any whitespace (`\s*(?:case [^:]+|default):`).
bool is_label(string_view line) { return is_case_label(line); }

// The lines without a four-blank step of indentation (the arms of a case written in a block of their own).
Lines dedented(const Lines& v) {
    Lines out;
    for (const string& l : v) out.push_back(l.starts_with("    ") ? l.substr(4) : l);
    return out;
}

// `^(\s*)REG = (.*);$` -> `\1return \2;` for a line that is such a store; any other line as it is.
string store_as_return(const string& line, string_view reg) {
    const string_view s = txt::ltrim(line);
    const string head = cat(reg, " = ");
    if (!s.starts_with(head) || s.back() != ';') return line;
    return cat(line.substr(0, line.size() - s.size()), "return ", s.substr(head.size(), s.size() - head.size() - 1), ";");
}

std::optional<std::size_t> closing_brace_of(const Lines& lines, std::size_t k, const string& ind) {
    for (std::size_t j = k + 1; j < lines.size(); ++j)
        if (lines[j] == ind + "}") return j;
    return std::nullopt;
}

std::vector<std::size_t> case_labels(const Lines& lines, std::size_t from, std::size_t to, const string& ind) {
    std::vector<std::size_t> labels;
    for (std::size_t j = from; j < to; ++j)
        if (is_case_label_at(lines[j], ind)) labels.push_back(j);
    return labels;
}

}  // namespace

// ---- a run of values tested as a range -------------------------------------------------------------------------

string range_switches(string_view text_in, const FunctionFacts& facts) {
    static const Regex register_load(R"re(([ \t]*)(ax|bx|cx|dx) = (.+);)re");
    static const Regex variable_test(R"re(([ \t]*)if \(\(([\w\->]+) >= (\d+)\)\) \{)re");
    static const Regex goto_line(R"re(\s*goto (L_\w+);)re");
    static const Regex digits(R"re(\d+)re");
    static const Regex returns_value(R"re(\s*return (\w+);)re");
    static const Regex bare_return(R"re(^\s*return\b)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k + 1 < lines.size() && !again; ++k) {
            string indent_, name, selector;   // (the load's indent, the register or variable tested, the switch's selector)
            std::optional<dt::IfParts> p;
            const auto a = register_load.full_match(lines[k]);
            if (a) {
                indent_ = str(a->group(1));
                name = str(a->group(2));
                selector = str(a->group(3));
                p = dt::if_parts(lines, k + 1);
            } else {   // (or a variable the code loads for the tests: ranges)
                const auto v = variable_test.full_match(lines[k]);
                if (!v) continue;
                const auto lo = number(v->group(3));
                if (!lo || !std::ranges::any_of(facts.ranges, [&](const dt::Range& r) { return r.lo == *lo; })) continue;
                indent_ = str(v->group(1));
                name = str(v->group(2));
                selector = name;
                p = dt::if_parts(lines, k);
            }
            if (!p || !p->else_arm) continue;
            string cond = p->cond;
            Lines then_arm = p->then_arm, else_arm = *p->else_arm;
            const std::size_t end = p->end;

            const string c1_head = cat("((", name, " >= ");
            const auto bounded = [&](string_view whole, string_view head, std::string_view tail_text) -> std::optional<string> {
                // `HEAD(\d+)TAIL` as a whole
                if (!whole.starts_with(head) || !whole.ends_with(tail_text) || whole.size() < head.size() + tail_text.size() + 1) return std::nullopt;
                const string_view d = whole.substr(head.size(), whole.size() - head.size() - tail_text.size());
                if (!digits.full_match(d)) return std::nullopt;
                return str(d);
            };
            const string c1_text = cat("(", cond, ")");
            const auto c1 = bounded(c1_text, c1_head, "))");
            if (c1 && !then_arm.empty()) {
                // (the selector loaded again for the next test: nothing)
                const string_view first = txt::ltrim(then_arm[0]);
                bool reload = false;
                for (string_view reg : {"ax", "bx", "cx", "dx"})
                    if (first == cat(reg, " = ", name, ";")) reload = true;
                if (reload) then_arm.erase(then_arm.begin());
            }
            std::optional<dt::IfParts> q = c1 && !then_arm.empty() ? dt::if_parts(then_arm, 0) : std::nullopt;
            if (!q || q->end != then_arm.size()) continue;
            Lines q_then = q->then_arm, q_else = q->else_arm ? *q->else_arm : Lines{};   // (the range's own arm empty: only a break)
            string q_cond = q->cond;

            std::optional<string> c2_digits;
            bool c2_is_le = false;
            const string q_text = cat("(", q_cond, ")");
            if (const auto gt = bounded(q_text, cat("((", name, " > "), "))")) {
                c2_digits = gt;
            } else if (const auto le = bounded(q_text, cat("((", name, " <= "), "))")) {
                c2_digits = le;
                c2_is_le = true;
            }
            if (c2_is_le) std::swap(q_then, q_else);   // (the range's own arm first: the same, turned about)
            const auto g_ = c2_digits && else_arm.size() == 1 ? goto_line.full_match(else_arm[0]) : std::nullopt;
            if (g_ && !dedented(q_then).empty() && txt::trim(dedented(q_then)[0]) == cat(g_->group(1), ":")) {
                // (the other way out a goto into it: the same code)
                Lines body;
                for (const string& l : dedented(q_then))
                    if (txt::trim(l) != cat(g_->group(1), ":")) body.push_back(l);
                Lines outside = slice(lines, 0, k);
                for (std::size_t j = end; j < lines.size(); ++j) outside.push_back(lines[j]);
                if (!goes_to(join_lines(outside), g_->group(1))) {
                    else_arm = body;
                    q_then.clear();
                    for (const string& l : body) q_then.push_back(blank(l) ? l : "    " + l);
                }
            }
            if (!c2_digits || dedented(q_then) != else_arm) continue;
            const auto lo = number(*c1), hi = number(*c2_digits);
            if (!lo || !hi || !(0 <= *lo && *lo <= *hi) || *hi - *lo > 16) continue;
            const string ind = indent_;
            Lines dflt{ind + "default:"};
            dflt.insert(dflt.end(), else_arm.begin(), else_arm.end());
            dflt.push_back(ind + "    break;");
            Lines cases;
            for (std::int64_t v = *lo; v <= *hi; ++v) cases.push_back(cat(ind, "case ", std::to_string(v), ":"));
            const Lines range_arm = dedented(q_else);
            cases.insert(cases.end(), range_arm.begin(), range_arm.end());
            cases.push_back(ind + "    break;");
            // (the code shows the cases' arm first: no default - what the rest do follows the switch, the cases leaving)
            const bool first = std::ranges::any_of(facts.ranges, [&](const dt::Range& r) { return r.lo == *lo && r.hi == *hi && r.above; });
            Lines ends;
            for (const string& l : range_arm)
                if (!blank(l)) ends.push_back(l);
            string after;
            for (std::size_t j = end; j < lines.size(); ++j)
                if (!blank(lines[j])) {
                    after = lines[j];
                    break;
                }
            const auto r_ = returns_value.full_match(after);
            Lines next;
            if (first && r_ && !ends.empty() && plain_value(ends.back(), r_->group(1)) &&
                (else_arm.empty() || blank(else_arm.back()) || plain_value(else_arm.back(), r_->group(1)))) {
                const auto sub = [&](const Lines& ls) {
                    Lines out;
                    for (const string& l : ls) out.push_back(store_as_return(l, r_->group(1)));
                    return out;
                };
                Lines head_part = slice(cases, 0, cases.size() - 1);
                next.push_back(cat(ind, "switch (", selector, ") {"));
                for (const string& l : sub(head_part)) next.push_back(l);
                next.push_back(ind + "}");
                for (const string& l : sub(dedented(else_arm))) next.push_back(l);
                Lines replaced = slice(lines, 0, k);
                replaced.insert(replaced.end(), next.begin(), next.end());
                const std::size_t resume = replaced.size();
                for (std::size_t j = end; j < lines.size(); ++j) replaced.push_back(lines[j]);
                std::optional<std::size_t> k2;
                for (std::size_t j = resume; j < replaced.size(); ++j)
                    if (!blank(replaced[j])) {
                        k2 = j;
                        break;
                    }
                if (!k2) continue;   // (nothing left to drop: not this way)
                replaced.erase(replaced.begin() + static_cast<std::ptrdiff_t>(*k2));   // (the return after: each way returns its own)
                text = join_lines(replaced);
                again = true;
                continue;
            }
            if (first && !ends.empty() && bare_return.match(ends.back())) {
                next.push_back(cat(ind, "switch (", selector, ") {"));
                for (std::size_t i = 0; i + 1 < cases.size(); ++i) next.push_back(cases[i]);
                next.push_back(ind + "}");
                for (const string& l : dedented(else_arm)) next.push_back(l);
            } else {
                next.push_back(cat(ind, "switch (", selector, ") {"));
                const Lines& one = first ? cases : dflt;
                const Lines& two = first ? dflt : cases;
                next.insert(next.end(), one.begin(), one.end());
                next.insert(next.end(), two.begin(), two.end());
                next.push_back(ind + "}");
            }
            Lines replaced = slice(lines, 0, k);
            replaced.insert(replaced.end(), next.begin(), next.end());
            for (std::size_t j = end; j < lines.size(); ++j) replaced.push_back(lines[j]);
            text = join_lines(replaced);
            again = true;
        }
        if (!again) return text;
    }
}

// ---- arms that fall into the next -------------------------------------------------------------------------

string fall_dups(string_view text_in) {
    static const Regex switch_open(R"re(([ \t]*)switch \(.*\) \{)re");
    static const Regex leaves_arm(R"re(^\s*(?:return\b|break;))re");
    static const Regex label_line(R"re(\s*L_\w+:)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k < lines.size() && !again; ++k) {
            const auto m = switch_open.full_match(lines[k]);
            if (!m) continue;
            const string ind = str(m->group(1));
            const auto end = closing_brace_of(lines, k, ind);
            if (!end) continue;
            const auto heads = case_labels(lines, k + 1, *end, ind);
            std::vector<std::pair<std::size_t, std::size_t>> arms;
            for (std::size_t i = 0; i < heads.size(); ++i) {
                const std::size_t stop = i + 1 < heads.size() ? heads[i + 1] : *end;
                std::size_t j = heads[i];
                while (j + 1 < stop && is_case_label_at(lines[j + 1], ind)) ++j;
                if (j + 1 < stop) arms.emplace_back(j + 1, stop);
            }
            for (std::size_t i = 0; i + 1 < arms.size(); ++i) {
                const auto [a0, a1] = arms[i];
                const auto [b0, b1] = arms[i + 1];
                const std::size_t nb = b1 - b0;
                const Lines body_b = slice(lines, b0, b1);
                if (body_b.empty() || !leaves_arm.match(body_b.back()) || a1 - a0 <= nb || slice(lines, a1 - nb, a1) != body_b) continue;
                if (std::ranges::any_of(body_b, [&](const string& x) { return label_line.full_match(x).has_value(); })) continue;
                lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(a1 - nb), lines.begin() + static_cast<std::ptrdiff_t>(a1));
                text = join_lines(lines);
                again = true;
                break;
            }
        }
        if (!again) return text;
    }
}

string switch_follow(string_view text_in) {
    static const Regex switch_open(R"re(^(\s*)switch \(.*\) \{$)re");
    static const Regex label_after_default(R"re((L_\w+):)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k < lines.size() && !again; ++k) {
            const auto m = switch_open.match(lines[k]);
            if (!m || k + 2 >= lines.size() || lines[k + 1] != cat(m->group(1), "default:")) continue;
            const string ind = str(m->group(1));
            const string label_text = lines[k + 2].starts_with(ind + "    ") ? lines[k + 2].substr(ind.size() + 4) : string();
            const auto lab = label_after_default.full_match(label_text);
            const auto end = closing_brace_of(lines, k, ind);
            if (!lab || !end) continue;
            const string label = str(lab->group(1));
            std::optional<std::size_t> nxt;
            for (std::size_t j = k + 2; j < *end; ++j)
                if (is_case_label_at(lines[j], ind)) {
                    nxt = j;
                    break;
                }
            if (!nxt) continue;
            const Lines body = slice(lines, k + 3, *nxt);
            string last;
            for (auto it = body.rbegin(); it != body.rend(); ++it)
                if (!blank(*it)) {
                    last = *it;
                    break;
                }
            std::vector<std::size_t> gotos;
            for (std::size_t j = *nxt; j < *end; ++j)
                if (txt::trim(lines[j]) == cat("goto ", label, ";")) gotos.push_back(j);
            const bool has_break = std::ranges::any_of(body, [&](const string& x) { return x == ind + "    break;"; });
            const bool in_loop = std::ranges::any_of(gotos, [&](std::size_t j) {
                return dt::in_loop(lines, static_cast<std::ptrdiff_t>(k), j, ind.size());
            });
            if (gotos.empty() || !is_return_at(last, ind + "    ") || has_break || in_loop) continue;
            Lines next = lines;
            for (std::size_t j : gotos) next[j] = txt::replace_all(next[j], cat("goto ", label, ";"), "break;");
            const Lines rest = slice(next, *nxt, *end + 1);
            Lines after;
            for (const string& x : body) after.push_back(dedent(x, ind));
            const auto assemble = [&](bool with_label) {
                Lines out = slice(next, 0, k + 1);
                out.push_back(ind + "default:");
                out.push_back(ind + "    break;");
                out.insert(out.end(), rest.begin(), rest.end());
                if (with_label) out.push_back(ind + label + ":");
                out.insert(out.end(), after.begin(), after.end());
                for (std::size_t j = *end + 1; j < next.size(); ++j) out.push_back(next[j]);
                return out;
            };
            string t = join_lines(assemble(false));
            if (goes_to(t, label)) t = join_lines(assemble(true));
            text = t;
            again = true;
        }
        if (!again) return text;
    }
}

std::optional<string> return_cases_first(string_view text) {
    static const Regex switch_open(R"re(^(\s*)switch \(.*\) \{$)re");
    const Lines lines = split_lines(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = switch_open.match(lines[k]);
        if (!m) continue;
        const string ind = str(m->group(1));
        const auto end = closing_brace_of(lines, k, ind);
        if (!end) continue;
        const auto labels = case_labels(lines, k + 1, *end, ind);
        if (labels.empty() || labels[0] != k + 1) continue;
        using Group = std::pair<std::size_t, std::size_t>;
        std::vector<Group> groups;   // (arms: their labels together, then the statements)
        for (std::size_t j = 0; j < labels.size(); ++j) {
            const std::size_t a = j;
            while (j + 1 < labels.size() && labels[j + 1] == labels[j] + 1) ++j;
            const std::size_t b = j + 1 < labels.size() ? labels[j + 1] : *end;
            groups.emplace_back(labels[a], b);
        }
        // an arm that only returns: some statement, and every line a label or `return;` / `return N;`
        const auto only_returns = [&](const Group& g) {
            bool statement = false;
            for (std::size_t j = g.first; j < g.second; ++j) {
                const string& x = lines[j];
                if (blank(x)) continue;
                if (is_label(x)) continue;
                statement = true;
                const string stem = ind + "    return";
                if (!x.starts_with(stem) || !x.ends_with(";")) return false;
                const string_view mid = string_view(x).substr(stem.size(), x.size() - stem.size() - 1);
                if (!mid.empty()) {
                    if (mid[0] != ' ' || mid.size() < 2) return false;
                    string_view d = mid.substr(1);
                    if (d.starts_with("-")) d.remove_prefix(1);
                    if (d.empty() || !std::ranges::all_of(d, [](char c) { return c >= '0' && c <= '9'; })) return false;
                }
            }
            return statement;
        };
        std::vector<Group> rets;
        for (const Group& g : groups)
            if (only_returns(g)) rets.push_back(g);
        if (rets.empty() || rets[0] == groups[0] || rets.size() == groups.size()) continue;
        bool falls = false;
        for (std::size_t i = 1; i < groups.size(); ++i) {
            if (std::ranges::find(rets, groups[i]) == rets.end()) continue;
            string last;
            for (std::size_t j = groups[i - 1].second; j-- > groups[i - 1].first;)
                if (!blank(lines[j])) {
                    last = lines[j];
                    break;
                }
            static const Regex jump(R"re(^\s*(?:break|continue|return\b|goto\b))re");
            if (!jump.match(last)) falls = true;   // (an arm falling into it: not moved)
        }
        if (falls) continue;
        std::vector<Group> order = rets;
        for (const Group& g : groups)
            if (std::ranges::find(rets, g) == rets.end()) order.push_back(g);
        Lines next = slice(lines, 0, k + 1);
        for (const Group& g : order)
            for (std::size_t j = g.first; j < g.second; ++j) next.push_back(lines[j]);
        for (std::size_t j = *end; j < lines.size(); ++j) next.push_back(lines[j]);
        return join_lines(next);
    }
    return std::nullopt;
}

string selector_reads(string_view text) {
    static const Regex switch_open(R"re(^(\s*)switch \((.+)\) \{$)re");
    static const Regex calls_or_stores(R"re(\w\s*\(|\+\+|--|[^=!<>]=[^=])re");
    static const Regex byte_selector(R"re(\*(\w+))re");
    static const Regex statement_head(R"re(^\s*(?:if|while|switch|for)\b)re");
    static const Regex word_paren(R"re(\w\s*\()re");
    static const Regex block_or_call(R"re(\w\s*\(|[{}])re");
    static const Regex label_start(R"re(^\s*L_\w+:)re");
    static const Regex ends_arm(R"re(\s*(?:break|continue|return\b.*|goto \w+);)re");
    Lines lines = split_lines(text);
    const string gone(1, '\0');
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = switch_open.match(lines[k]);
        if (!m || calls_or_stores.search(m->group(2)) || has_word(m->group(2), "ax") || has_word(m->group(2), "al") || has_word(m->group(2), "ah")) continue;
        const string ind = str(m->group(1)), sel = str(m->group(2));
        std::size_t j = k + 1;
        bool entered = true;   // (only the dispatch reaches this arm)
        while (j < lines.size() && lines[j] != ind + "}") {
            if (!is_case_label_at(lines[j], ind)) {
                ++j;
                continue;
            }
            string prev = lines[k];
            for (std::size_t i = j; i-- > k + 1;)
                if (!blank(lines[i])) {
                    prev = lines[i];
                    break;
                }
            if (!is_case_label(prev)) entered = prev == lines[k] || ends_arm.full_match(prev).has_value();
            bool ok = entered;
            ++j;
            const auto bsel = byte_selector.full_match(sel);   // (a byte the selector: its al)
            while (ok && j < lines.size() && !is_case_label_at(lines[j], ind) && lines[j] != ind + "}") {
                const string y = lines[j];
                if (bsel && txt::ltrim(y) == cat(bsel->group(1), " = ", bsel->group(1), " + 1;") && j + 1 < lines.size() &&
                    count_word(lines[j + 1], "al") == 1 && !has_word(lines[j + 1], bsel->group(1))) {
                    lines[j + 1] = replace_word_all(lines[j + 1], "al", cat("*", bsel->group(1), "++"));   // (read, then stepped: *p++)
                    lines[j] = gone;
                    break;
                }
                bool al_assigned = false;
                for (std::size_t at = y.find("al = "); at != string::npos; at = y.find("al = ", at + 1))
                    if (boundary_at(y, at)) al_assigned = true;
                if (bsel && count_word(y, "al") == 1 && !has_word(y, "ax") && !has_word(y, "ah") && !al_assigned) {
                    lines[j] = replace_word_all(y, "al", sel);
                    break;
                }
                if (label_start.match(y) || has_word(y, "al") || has_word(y, "ah")) break;
                if (has_word(y, "ax")) {
                    const string_view s = txt::ltrim(y);
                    if (s.starts_with("ax = ")) {   // (`(\s*ax = )(.*)$`)
                        const string head = y.substr(0, y.size() - s.size() + 5);
                        const string rhs = y.substr(head.size());
                        if (!has_word(rhs, "ax")) break;   // (given a value first: not the selector's)
                        lines[j] = head + replace_word_all(rhs, "ax", sel);
                    } else if (statement_head.match(y) && !word_paren.search(y)) {
                        break;
                    } else {
                        lines[j] = replace_word_all(y, "ax", sel);
                    }
                    break;
                }
                if (block_or_call.search(y)) break;   // (a call, a block: the register may hold anything)
                ++j;
            }
        }
    }
    Lines kept;
    for (const string& l : lines)
        if (l != gone) kept.push_back(l);
    return join_lines(kept);
}

std::vector<string> case_guards(string_view text) {
    static const Regex switch_open(R"re(^(\s*)switch \((.+)\) \{$)re");
    static const Regex jump(R"re(^\s*(?:break|continue|return\b|goto\b))re");
    const Lines lines = split_lines(text);
    std::vector<string> out;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = switch_open.match(lines[k]);
        if (!m) continue;
        const string ind = str(m->group(1)), sel = str(m->group(2));
        const auto end = closing_brace_of(lines, k, ind);
        if (!end) continue;
        for (std::size_t j = k + 1; j + 1 < *end; ++j) {
            // `IND case (-?\w+):`
            std::optional<string> c;
            const string head = ind + "case ";
            if (lines[j].starts_with(head) && lines[j].ends_with(":")) {
                string_view v = string_view(lines[j]).substr(head.size(), lines[j].size() - head.size() - 1);
                string_view w = v.starts_with("-") ? v.substr(1) : v;
                if (!w.empty() && std::ranges::all_of(w, is_word_char)) c = str(v);
            }
            // `IND    (return(?: [^;]+)?;)`
            std::optional<string> r;
            const string stem = ind + "    return";
            if (lines[j + 1].starts_with(stem) && lines[j + 1].ends_with(";")) {
                const string_view mid = string_view(lines[j + 1]).substr(stem.size(), lines[j + 1].size() - stem.size() - 1);
                if (mid.empty() || (mid[0] == ' ' && mid.size() > 1 && mid.find(';') == string_view::npos)) r = lines[j + 1].substr(ind.size() + 4);
            }
            const string& prev = lines[j - 1];
            if (!c || !r || is_case_label(prev) ||
                (j + 2 < *end && !is_case_label(lines[j + 2]) && lines[j + 2] != ind + "}"))
                continue;
            if (!jump.match(prev) && prev != lines[k]) continue;   // (an arm falling into it: not alone)
            Lines body = slice(lines, 0, j);
            for (std::size_t x = j + 2; x < lines.size(); ++x) body.push_back(lines[x]);
            std::vector<string> returns{*r, "return;"};
            std::sort(returns.begin(), returns.end());
            returns.erase(std::unique(returns.begin(), returns.end()), returns.end());
            for (const string& ret : returns) {
                Lines next = body;
                next.insert(next.begin() + static_cast<std::ptrdiff_t>(k), cat(ind, "if ((", sel, " == ", *c, ")) ", ret));
                out.push_back(join_lines(next));
            }
            break;
        }
    }
    return out;
}

string switch_ifs(string_view text) {
    static const Regex default_first(
        R"re(^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1    continue;\n\1case (-?\w+):\n\1    break;\n\1\}\n(\1[ \t]*\}|[ \t]*\}))re",
        kLines);
    // (switch (x) { case K: break; default: S } last: the value passed over first - if (x == K) continue; S)
    static const Regex case_first(
        R"re(^([ \t]*)switch \((.+)\) \{\n\1case (-?\w+):\n\1    break;\n\1default:\n((?:\1    .*\n)+?)\1\}\n([ \t]*\}))re", kLines);
    // (the same, the default first: switch (x) { default: S case K: break; } - S falls into the break)
    static const Regex default_falls(
        R"re(^([ \t]*)switch \((.+)\) \{\n\1default:\n((?:\1    .*\n)+?)\1case (-?\w+):\n\1    break;\n\1\}\n([ \t]*\}))re", kLines);
    const auto leading = [](string_view s) { return indent_of(s); };
    const auto passes_over = [&](string_view ind, string_view sel, string_view k, const string& body, string_view close, const string& whole) -> string {
        if (leading(close) >= ind.size()) return whole;
        // a `break;` of the arm's own in S: the value passed over would then leave the loop
        for (const string& l : split_lines(body))
            if (l == cat(ind, "    break;")) return whole;
        std::string_view tb = body;   // (every trailing newline off)
        while (!tb.empty() && tb.back() == '\n') tb.remove_suffix(1);
        string dd;
        for (const string& x : split_lines(tb)) dd += (x.starts_with(cat(ind, "    ")) ? x.substr(4) : x) + "\n";
        return cat(ind, "if ((", sel, " == ", k, ")) continue;\n", dd, close);
    };
    string t = default_first.replace_all(text, [&](const RegexMatch& m) -> string {
        const string_view ind = m.group(1), sel = m.group(2), k = m.group(4), close = m.group(5);
        if (leading(close) >= ind.size()) return str(m.group(0));
        return cat(ind, "if ((", sel, " != ", k, ")) {\n", m.group(3), ind, "}\n", close);
    });
    t = case_first.replace_all(t, [&](const RegexMatch& m) -> string {
        return passes_over(m.group(1), m.group(2), m.group(3), str(m.group(4)), m.group(5), str(m.group(0)));
    });
    return default_falls.replace_all(t, [&](const RegexMatch& m) -> string {
        return passes_over(m.group(1), m.group(2), m.group(4), str(m.group(3)), m.group(5), str(m.group(0)));
    });
}

string two_case_switches(string_view text) {
    static const Regex register_load(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re");
    static const Regex goto_only(R"re(goto (L_\w+);)re");
    Lines lines = split_lines(text);
    for (std::size_t k = 0; k + 4 < lines.size(); ++k) {
        const auto a = register_load.full_match(lines[k]);
        if (!a) continue;
        const string ind = str(a->group(1)), r = str(a->group(2)), e = str(a->group(3));
        const auto p = dt::if_parts(lines, k + 1);
        // `\(\(R OP (-?\d+)\)\)` of "(" + cond + ")"
        const auto compared = [&](const string& cond, string_view op) -> std::optional<string> {
            const string head = cat("((", r, " ", op, " ");
            const string whole = cat("(", cond, ")");
            if (!whole.starts_with(head) || !whole.ends_with("))") || whole.size() <= head.size() + 2) return std::nullopt;
            const string_view d = string_view(whole).substr(head.size(), whole.size() - head.size() - 2);
            const string_view digits = d.starts_with("-") ? d.substr(1) : d;
            if (digits.empty() || !std::ranges::all_of(digits, [](char ch) { return ch >= '0' && ch <= '9'; })) return std::nullopt;
            return str(d);
        };
        const auto c1 = p ? compared(p->cond, "!=") : std::nullopt;
        if (!c1 || !p->else_arm) continue;
        string stripped_else;
        std::size_t texty = 0;
        for (const string& x : *p->else_arm)
            if (!blank(x)) {
                ++texty;
                stripped_else += txt::trim(x);
            }
        if (texty > 1) continue;
        const auto g = goto_only.full_match(stripped_else);
        if (!g) continue;
        const string lab = str(g->group(1));
        const auto q = !p->then_arm.empty() ? dt::if_parts(p->then_arm, 0) : std::nullopt;
        const auto c2 = q ? compared(q->cond, "==") : std::nullopt;
        if (!c2 || q->else_arm || q->end != p->then_arm.size() || q->then_arm.empty() || txt::trim(q->then_arm[0]) != cat(lab, ":")) continue;
        Lines body;
        for (std::size_t i = 1; i < q->then_arm.size(); ++i) {
            const string& x = q->then_arm[i];
            body.push_back(x.starts_with(ind + "        ") ? x.substr(8) : x);
        }
        Lines outside = slice(lines, 0, k);
        outside.insert(outside.end(), body.begin(), body.end());
        for (std::size_t j = p->end; j < lines.size(); ++j) outside.push_back(lines[j]);
        const string joined_outside = join_lines(outside);
        const string joined_body = join_lines(body);
        if (goes_to(joined_outside, lab) || std::ranges::any_of(body, [](const string& x) { return txt::trim(x) == "break;"; }) ||
            has_word(joined_body, r))
            continue;
        const string selector = !e.starts_with("(") || !dt::balanced(string_view(e).substr(1, e.size() - 2)) ? e : e.substr(1, e.size() - 2);
        Lines replacement{cat(ind, "switch (", selector, ") {"), cat(ind, "case ", *c1, ":"), cat(ind, "case ", *c2, ":")};
        for (const string& x : body) replacement.push_back(blank(x) ? x : "    " + x);
        replacement.push_back(ind + "}");
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(p->end));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        return join_lines(lines);
    }
    return str(text);
}

}  // namespace cb::exact::draft::shapes
