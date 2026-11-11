// cb-unity: alone (file-local helpers shared by name with the other shapes_*.cpp files)
// The passes that read back returns and gotos: the compiler's one exit jumped to from every way out, the tails two arms
// share, the stores two arms meet at.
#include <algorithm>
#include <functional>
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

constexpr RegexFlags kLines = RegexFlags::Multiline;

string str(string_view s) { return string(s); }

bool starts_with_return(string_view line) {   // `\s*return\b`
    const string_view s = txt::ltrim(line);
    return s.starts_with("return") && (s.size() == 6 || !is_word_char(s[6]));
}

bool starts_with_leave(string_view line) {   // `\s*(?:break|continue);` at the start
    const string_view s = txt::ltrim(line);
    return s.starts_with("break;") || s.starts_with("continue;");
}

const Regex& if_header() {   // `\s*if \(.*\) \{$`
    static const Regex re(R"re(^\s*if \(.*\) \{$)re");
    return re;
}

// `\s*(?:break|continue|return\b|goto\b)` at the start of a line.
bool starts_with_jump(string_view line) {
    static const Regex re(R"re(^\s*(?:break|continue|return\b|goto\b))re");
    return re.match(line).has_value();
}

}  // namespace

// ---- the returns -------------------------------------------------------------------------------------------------

string branch_returns(string_view text_in) {
    static const Regex if_open(R"re(^(\s*)if (\(.*\)) \{\s*$)re");
    static const Regex first_store(R"re(^\s*(ax|al|bx|cx|dx|si|di) = (.*);\s*$)re");
    string text = str(text_in);
    for (;;) {
        const Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k < lines.size() && !again; ++k) {
            const auto m = if_open.match(lines[k]);
            if (!m || k + 3 >= lines.size()) continue;
            const string ind = str(m->group(1));
            const auto a = first_store.match(lines[k + 1]);
            if (!a || lines[k + 2] != ind + "} else {") continue;
            const string reg = str(a->group(1));
            std::optional<std::size_t> close;
            for (std::size_t j = k + 3; j < lines.size(); ++j)
                if (lines[j] == ind + "}") {
                    close = j;
                    break;
                }
            if (!close || *close + 1 >= lines.size() || txt::trim(lines[*close + 1]) != cat("return ", reg, ";")) continue;
            const auto last = value_given(lines[*close - 1], reg, false);
            if (!last) continue;
            Lines next = slice(lines, 0, k);
            next.push_back(cat(ind, "if ", m->group(2)));
            next.push_back(cat(ind, "    return ", a->group(2), ";"));
            for (std::size_t j = k + 3; j + 1 < *close; ++j) next.push_back(dedent(lines[j], ind));
            next.push_back(cat(ind, "return ", *last, ";"));
            for (std::size_t j = *close + 2; j < lines.size(); ++j) next.push_back(lines[j]);
            text = join_lines(next);
            again = true;
        }
        if (!again) return text;
    }
}

string push_returns(string_view text) {
    static const Regex store_ax(R"re(([ \t]*)ax = ([^;\n]+);)re");
    static const Regex closes(R"re([ \t]*\}(?: else \{)?)re");
    static const Regex arm_header(R"re([ \t]*(?:if \(.*\) \{|\} else \{|else \{))re");
    static const Regex final_return(R"re(    return (ax|bx|cx|dx|si|di);)re");
    static const Regex return_value(R"re(\s*return (\w+);)re");
    static const Regex return_any(R"re(\s*return\b[^;]*;)re");
    static const Regex switch_header(R"re(^\s*switch \(.*\) \{$)re");
    static const Regex loop_header(R"re(^\s*(?:while|for|do|switch)\b.*\{$)re");

    Lines lines = split_lines(text);
    // (an if's arm ending r = ax, r a register variable: r's)
    for (std::size_t k = 0; k + 2 < lines.size(); ++k) {
        const auto m = store_ax.full_match(lines[k]);
        if (!m) continue;
        const string ind_text = str(m->group(1));
        if ((lines[k + 1] != cat(ind_text, "si = ax;") && lines[k + 1] != cat(ind_text, "di = ax;")) || !closes.full_match(lines[k + 2])) continue;
        if (ind_text.size() < 4) continue;   // (no line is indented a negative amount)
        const std::size_t ind = ind_text.size() - 4;
        std::optional<std::size_t> op;
        for (std::size_t j = k; j-- > 0;)
            if (!blank(lines[j]) && indent_of(lines[j]) == ind) {
                op = j;
                break;
            }
        if (!op || !arm_header.match(lines[*op])) continue;
        const string r = str(txt::trim(lines[k + 1]).substr(0, 2));
        const string value = str(m->group(2));
        lines[k] = cat(ind_text, r, " = ", value, ";");
        lines[k + 1] = cat(ind_text, "ax = ", r, ";");
    }
    dt::else_ifs(lines);
    std::optional<std::size_t> end;
    for (std::size_t k = 0; k < lines.size(); ++k)
        if (final_return.full_match(lines[k])) end = k;
    if (!end || *end + 1 >= lines.size() || lines[*end + 1] != "}") return str(text);
    const string reg = str(return_value.full_match(lines[*end])->group(1));

    std::set<std::size_t> breaks;   // (a switch's breaks after the values become returns)
    std::size_t leaves = 0;
    const auto gives_reg = [&](const string& l) { return plain_value(l, reg).has_value(); };

    // (the statements lines[lo:hi] end giving reg a value: [indexes of those assignments], or None)
    std::function<std::optional<std::vector<std::size_t>>(std::ptrdiff_t, std::ptrdiff_t)> tail =
        [&](std::ptrdiff_t lo, std::ptrdiff_t hi) -> std::optional<std::vector<std::size_t>> {
        std::ptrdiff_t k = hi - 1;
        while (k >= lo && blank(lines[static_cast<std::size_t>(k)])) --k;
        if (k < lo) return std::nullopt;
        const std::size_t ku = static_cast<std::size_t>(k);
        if (gives_reg(lines[ku])) return std::vector<std::size_t>{ku};
        if (return_any.full_match(lines[ku]) && ku != *end) {
            ++leaves;   // (an arm returning its own value: nothing to give)
            return std::vector<std::size_t>{};
        }
        if (txt::trim(lines[ku]) != "}") return std::nullopt;
        std::optional<std::size_t> sw;
        for (std::ptrdiff_t j = k - 1; j >= lo; --j) {
            const string& x = lines[static_cast<std::size_t>(j)];
            if (!blank(x) && !is_case_label(x) && indent_of(x) == indent_of(lines[ku])) {
                sw = static_cast<std::size_t>(j);
                break;
            }
        }
        if (sw && switch_header.match(lines[*sw])) {   // (a switch each arm of which gives reg and breaks, a default among them)
            std::vector<std::size_t> got, arm;
            bool ok = true;
            for (std::size_t j = *sw + 1; j <= ku; ++j) {
                if (is_case_label(lines[j]) || j == ku) {
                    if (!arm.empty()) {
                        std::vector<std::size_t> idx;
                        for (std::size_t x : arm)
                            if (!blank(lines[x])) idx.push_back(x);
                        std::optional<std::vector<std::size_t>> sub;
                        if (idx.size() >= 2 && txt::trim(lines[idx.back()]) == "break;" && txt::trim(lines[idx[idx.size() - 2]]) == "}")
                            sub = tail(static_cast<std::ptrdiff_t>(idx.front()), static_cast<std::ptrdiff_t>(idx.back()));
                        if (idx.size() >= 2 && txt::trim(lines[idx.back()]) == "break;" && gives_reg(lines[idx[idx.size() - 2]])) {
                            got.push_back(idx[idx.size() - 2]);
                            breaks.insert(idx.back());
                        } else if (sub && !sub->empty()) {
                            got.insert(got.end(), sub->begin(), sub->end());
                            breaks.insert(idx.back());
                        } else {
                            ok = false;
                        }
                    }
                    arm.clear();
                } else {
                    arm.push_back(j);
                }
            }
            // (no default: the function falls off its end there, as the source does)
            if (ok && !got.empty()) return got;
            return std::nullopt;
        }
        std::optional<std::size_t> start;
        for (std::ptrdiff_t j = k - 1; j >= lo; --j)
            if (if_header().match(lines[static_cast<std::size_t>(j)]) &&
                indent_of(lines[static_cast<std::size_t>(j)]) == indent_of(lines[ku])) {
                start = static_cast<std::size_t>(j);
                break;
            }
        if (!start) return std::nullopt;
        const auto p = dt::if_parts(lines, *start);
        if (!p || p->end != ku + 1 || !p->else_arm) return std::nullopt;
        const std::ptrdiff_t then_lo = static_cast<std::ptrdiff_t>(*start) + 1;
        const std::ptrdiff_t then_hi = then_lo + static_cast<std::ptrdiff_t>(p->then_arm.size());
        auto a = tail(then_lo, then_hi);
        auto b = tail(then_hi + 1, k);
        if (!a || !b) return std::nullopt;
        a->insert(a->end(), b->begin(), b->end());
        return a;
    };

    const auto as_return = [&](const string& line) {   // `^(\s*)REG = (.*);$` -> `\1return \2;`
        const string_view s = txt::ltrim(line);
        const string head = cat(reg, " = ");
        return cat(line.substr(0, line.size() - s.size()), "return ", s.substr(head.size(), s.size() - head.size() - 1), ";");
    };

    auto ks = tail(0, static_cast<std::ptrdiff_t>(*end));
    if (!ks || ks->empty()) {   // (a switch last, each arm giving it: case ..: return X;)
        if (*end == 0) return str(text);
        const std::size_t k = *end - 1;
        if (txt::trim(lines[k]) != "}") return str(text);
        const string ind = str(indent_text(lines[k]));
        std::optional<std::size_t> st;
        for (std::size_t j = k; j-- > 0;)
            if (lines[j].starts_with(ind + "switch (") && lines[j].ends_with("{")) {
                st = j;
                break;
            }
        if (!st) return str(text);
        std::vector<std::size_t> arm;
        bool ok = true;
        std::vector<std::pair<std::size_t, std::optional<std::size_t>>> rets;
        for (std::size_t j = *st + 1; j <= k; ++j) {
            if (is_case_label(lines[j]) || j == k) {
                if (!arm.empty()) {
                    std::vector<std::size_t> idx;
                    for (std::size_t x : arm)
                        if (!blank(lines[x])) idx.push_back(x);
                    if (idx.size() >= 2 && txt::trim(lines[idx.back()]) == "break;" && gives_reg(lines[idx[idx.size() - 2]])) {
                        rets.emplace_back(idx[idx.size() - 2], idx.back());
                    } else if (!idx.empty() && gives_reg(lines[idx.back()]) && j == k) {
                        rets.emplace_back(idx.back(), std::nullopt);
                    } else if (!idx.empty() && j != k && !starts_with_jump(lines[idx.back()]) &&
                               [&] {
                                   string joined;
                                   for (std::size_t x : idx) joined += (joined.empty() ? "" : " ") + lines[x];
                                   return !has_word(joined, reg);
                               }()) {
                        // (an arm falling into the next: that one returns)
                    } else {
                        ok = false;
                    }
                }
                arm.clear();
            } else {
                arm.push_back(j);
            }
        }
        if (ok && !rets.empty()) {
            std::set<std::size_t> drop;
            for (const auto& [a_, b_] : rets) {
                lines[a_] = as_return(lines[a_]);
                if (b_) drop.insert(*b_);
            }
            Lines kept;
            for (std::size_t j = 0; j < lines.size(); ++j)
                if (!drop.contains(j) && j != *end) kept.push_back(lines[j]);
            return join_lines(kept);
        }
        // (each `r = V; break;` anywhere in the arms, outside loops of their own, a return of V; when no break is left and
        //  every arm ends in a return or falls on, the last return is gone)
        std::vector<std::pair<std::size_t, std::size_t>> spans;
        for (std::size_t j = *st + 1; j < k; ++j)
            if (loop_header.match(lines[j])) {
                const auto b = dt::block_end(lines, j);
                spans.emplace_back(j, b && *b != 0 ? *b : k);
            }
        std::vector<std::size_t> pairs;
        for (std::size_t j = *st + 1; j + 1 < k; ++j) {
            if (!gives_reg(lines[j]) || txt::trim(lines[j + 1]) != "break;") continue;
            const bool inside = std::ranges::any_of(spans, [&](const auto& s) { return s.first < j && j < s.second; });
            if (!inside) pairs.push_back(j);
        }
        if (!pairs.empty()) {
            MaybeLines next = maybe(lines);
            for (std::size_t j : pairs) {
                next[j] = as_return(*next[j]);
                next[j + 1].reset();
            }
            Lines rest;
            for (std::size_t j = *st + 1; j < k; ++j)
                if (next[j]) rest.push_back(*next[j]);
            bool left = std::ranges::any_of(rest, [](const string& x) { return txt::trim(x) == "break;"; });
            if (!left) left = has_word(txt::join(rest, "\n"), reg);
            string last;
            for (auto it = rest.rbegin(); it != rest.rend(); ++it)
                if (!blank(*it)) {
                    last = *it;
                    break;
                }
            if (!left && starts_with_return(last)) {
                next[*end].reset();
                return join_live(next);
            }
        }
        return str(text);
    }
    if (ks->size() + leaves < 2) return str(text);
    MaybeLines next = maybe(lines);
    for (std::size_t k : *ks) next[k] = as_return(lines[k]);
    next[*end].reset();
    for (std::size_t b_ : breaks) next[b_].reset();
    return join_live(next);
}

string chain_returns(string_view text_in) {
    static const Regex if_open(R"re(^(\s*)if \(.*\) \{\s*$)re");
    static const Regex return_reg(R"re(\s*return (ax|bx|cx|dx|si|di);\s*)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k < lines.size() && !again; ++k) {
            const auto m = if_open.match(lines[k]);
            if (!m) continue;
            const string ind = str(m->group(1));
            std::vector<std::size_t> arms_end;   // (the closing line of each arm)
            for (std::size_t j = k + 1; j < lines.size(); ++j) {
                const string& x = lines[j];
                if ((x.starts_with(ind + "} else if (") && x.ends_with("{")) || x == ind + "} else {") {
                    arms_end.push_back(j);
                } else if (x == ind + "}") {
                    arms_end.push_back(j);
                    break;
                } else if (x.starts_with(ind) && !x.starts_with(ind + " ") && !blank(x)) {
                    break;
                }
            }
            if (arms_end.empty() || lines[arms_end.back()] != ind + "}" || arms_end.back() + 1 >= lines.size()) continue;
            const auto ret = return_reg.full_match(lines[arms_end.back() + 1]);
            if (!ret || arms_end.size() < 2) continue;
            const string reg = str(ret->group(1));
            const bool all_given = std::ranges::all_of(arms_end, [&](std::size_t e) { return value_given(lines[e - 1], reg, true).has_value(); });
            if (!all_given) continue;
            MaybeLines next = maybe(lines);
            for (std::size_t e : arms_end) {
                const std::size_t q = e - 1;
                next[q] = cat(indent_text(lines[q]), "return ", *value_given(lines[q], reg, true), ";");
            }
            next[arms_end.back() + 1].reset();
            text = join_live(next);
            again = true;
        }
        if (!again) return text;
    }
}

std::optional<string> flat_returns(string_view text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    Lines lines = split_lines(split.body);
    const Lines seen = lines;
    bool changed = true;
    while (changed) {
        changed = false;
        // (an else after an arm that returns)
        for (std::size_t k = 0; k < lines.size() && !changed; ++k) {
            if (txt::trim(lines[k]) != "} else {") continue;
            string prev;
            for (std::size_t j = k; j-- > 0;)
                if (!blank(lines[j])) {
                    prev = lines[j];
                    break;
                }
            const string ind = str(indent_text(lines[k]));
            if (!is_return_at(prev, ind + "    ")) continue;
            const auto op = dt::opener(lines, k);
            if (!op || !if_header().match(lines[*op])) continue;
            const auto p = dt::if_parts(lines, *op);
            if (!p || !p->else_arm) continue;
            Lines next = slice(lines, 0, k);
            next.push_back(ind + "}");
            for (const string& x : *p->else_arm) next.push_back(dedent(x, ind));
            for (std::size_t j = p->end; j < lines.size(); ++j) next.push_back(lines[j]);
            lines = std::move(next);
            changed = true;
        }
        if (changed) continue;
        // (a default arm first, every path returning)
        static const Regex switch_open(R"re(^(\s*)switch \(.*\) \{$)re");
        for (std::size_t k = 0; k < lines.size() && !changed; ++k) {
            const auto m = switch_open.match(lines[k]);
            if (!m) continue;
            const string ind = str(m->group(1));
            std::optional<std::size_t> end;
            for (std::size_t j = k + 1; j < lines.size(); ++j)
                if (lines[j] == ind + "}") {
                    end = j;
                    break;
                }
            if (!end || lines[k + 1] != ind + "default:") continue;
            std::vector<std::size_t> labels;
            for (std::size_t j = k + 1; j < *end; ++j)
                if (is_case_label_at(lines[j], ind)) labels.push_back(j);
            std::vector<std::pair<std::size_t, std::size_t>> arms;
            for (std::size_t i = 0; i < labels.size(); ++i) arms.emplace_back(labels[i], i + 1 < labels.size() ? labels[i + 1] : *end);
            std::vector<string> ends;
            for (const auto& [a, b] : arms) {
                string last;
                for (std::size_t j = b; j-- > a + 1;)
                    if (!blank(lines[j])) {
                        last = lines[j];
                        break;
                    }
                ends.push_back(last);
            }
            bool all_return = true;
            for (const string& e : ends)
                if (!e.empty() && !is_return_at(e, ind + "    ")) all_return = false;
            bool leaves = false;
            for (std::size_t j = k + 1; j < *end; ++j) leaves = leaves || starts_with_leave(lines[j]);
            if (arms.size() < 2 || ends[0].empty() || !all_return || leaves) continue;
            const auto [d0, d1] = arms[0];
            Lines next = slice(lines, 0, k + 1);
            for (std::size_t j = d1; j <= *end; ++j) next.push_back(lines[j]);
            for (std::size_t j = d0 + 1; j < d1; ++j) next.push_back(dedent(lines[j], ind));
            for (std::size_t j = *end + 1; j < lines.size(); ++j) next.push_back(lines[j]);
            lines = std::move(next);
            changed = true;
        }
        if (changed) continue;
        // (a return the arm's end reaches at once again)
        static const Regex bare_return(R"re(\s*(return\b[^;]*;))re");
        static const Regex arm_header(R"re(^\s*(?:if \(.*\) \{|\} else \{)$)re");
        static const Regex else_or_if(R"re(^\s*(?:if|else)\b.*\{$)re");
        for (std::size_t k = 0; k < lines.size() && !changed; ++k) {
            const auto m = bare_return.full_match(lines[k]);
            if (!m) continue;
            std::size_t j = k + 1;
            bool ok = true;
            while (j < lines.size() && txt::trim(lines[j]) == "}") {
                const auto op = dt::opener(lines, j);
                if (!op || !arm_header.match(lines[*op])) {
                    ok = false;
                    break;
                }
                ++j;
            }
            // (a first line has no line before it: nothing there opens a block either)
            const string before = k > 0 ? lines[k - 1] : string();
            if (ok && j > k + 1 && j < lines.size() && txt::trim(lines[j]) == m->group(1) && txt::trim(before) != "{" &&
                txt::trim(before) != "} else {" && !else_or_if.match(before)) {
                lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
                changed = true;
            }
        }
    }
    if (lines == seen) return std::nullopt;
    return cat(split.head, split.brace, join_lines(lines));
}

string goto_returns(string_view text) {
    static const Regex return_after_label(R"re(^[ \t]*(L_\w+):\n[ \t]*(return(?: [\w]+| -?\d+)?;)$)re", kLines);
    static const Regex jump(R"re(^([ \t]*)goto (L_\w+);$)re", kLines);
    std::map<string, string> rets;   // (a label met twice keeps the later return: the lookup only)
    return_after_label.for_each_match(text, [&](const RegexMatch& m) {
        rets[str(m.group(1))] = str(m.group(2));
        return true;
    });
    if (rets.empty()) return str(text);
    string out = jump.replace_all(text, [&](const RegexMatch& m) -> string {
        const auto it = rets.find(str(m.group(2)));
        return it != rets.end() ? cat(m.group(1), it->second) : str(m.group(0));
    });
    for (const auto& [label, ret] : rets) {
        (void)ret;
        if (goes_to(out, label)) continue;
        out = join_lines(without_label(split_lines(out), label));
    }
    return out;
}

string tested_zero(string_view text_in) {
    static const Regex returns_ax(R"re(^[ \t]*return ax;$)re", kLines);
    static const Regex test_of_call(R"re(([ \t]*)if \(\(([A-Za-z_]\w*\(.*\)) != 0\)\) \{)re");
    static const Regex closes(R"re([ \t]*\}(?: else \{)?)re");
    string text = str(text_in);
    if (!returns_ax.search(text)) return text;
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t k = 0; k < lines.size() && !again; ++k) {
            const auto m = test_of_call.full_match(lines[k]);
            if (!m) continue;
            const auto p = dt::if_parts(lines, k);
            if (!p || p->else_arm || p->end >= lines.size()) continue;
            string last;
            for (auto it = p->then_arm.rbegin(); it != p->then_arm.rend(); ++it)
                if (!blank(*it)) {
                    last = *it;
                    break;
                }
            if (last.empty() || !starts_with_return(last) || !closes.full_match(lines[p->end])) continue;
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(p->end), cat(m->group(1), "ax = 0;"));
            text = join_lines(lines);
            again = true;
        }
        if (!again) return text;
    }
}

// ---- gotos ---------------------------------------------------------------------------------------------------

std::optional<string> goto_tails(string_view text, std::size_t most) {
    static const Regex jump(R"re(([ \t]*)goto (L_\w+);)re");
    static const Regex bare_label(R"re(L_\w+:)re");
    static const Regex case_start(R"re((?:case |default))re");
    static const Regex leaving(R"re((?:goto|break|continue|return)\b)re");
    Lines lines = split_lines(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = jump.full_match(lines[k]);
        if (!m) continue;
        const string target = cat(m->group(2), ":");
        std::optional<std::size_t> at;
        for (std::size_t j = 0; j < lines.size(); ++j)
            if (txt::trim(lines[j]) == target) {
                at = j;
                break;
            }
        // (a goto that only falls there: fall_gotos')
        if (!at) continue;
        if (*at > k) {
            bool falls = true;
            for (std::size_t j = k + 1; j < *at; ++j)
                if (!blank(lines[j]) && !txt::trim(lines[j]).ends_with(":")) falls = false;
            if (falls) continue;
        }
        const std::size_t ind = indent_of(lines[*at]);
        std::optional<Lines> tail = Lines{};
        std::size_t j = *at + 1;
        while (j < lines.size() && !blank(lines[j]) && indent_of(lines[j]) == ind) {
            const string x = str(txt::trim(lines[j]));
            if (x.ends_with("{") || (x.ends_with(":") && !bare_label.full_match(x)) || case_start.match(x)) {
                tail.reset();
                break;
            }
            if (bare_label.full_match(x)) {
                ++j;
                continue;
            }
            tail->push_back(x);
            ++j;
        }
        if (!tail || tail->empty() || tail->size() > most || j >= lines.size() || indent_of(lines[j]) >= ind ||
            std::ranges::any_of(*tail, [&](const string& x) { return leaving.match(x).has_value(); }))
            continue;
        Lines replacement;
        for (const string& x : *tail) replacement.push_back(cat(m->group(1), x));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        const string t = join_lines(lines);
        return join_lines(drop_unused_labels(split_lines(t), used_labels(t)));
    }
    return std::nullopt;
}

string fall_gotos(string_view text) {
    static const Regex jump(R"re([ \t]*goto (L_\w+);)re");
    static const Regex opens_arm(R"re(^[ \t]*(?:if \(.*\)|\} else|else(?: if \(.*\))?) \{$)re");
    static const Regex opens_arm_flow(R"re(^[ \t]*(?:if \(.*\)|\} else|else) \{$)re");
    static const Regex bare_label(R"re(L_\w+:)re");
    static const Regex if_open(R"re(([ \t]*)if (\(.*\)) \{)re");
    static const Regex case_line(R"re([ \t]*(?:case [^:]+|default):)re");
    MaybeLines lines = maybe(split_lines(text));
    const auto at_text = [&](std::size_t j) { return lines[j] ? *lines[j] : string(); };
    // (the line opening the block line j closes: the nearest before it at its indentation that ends in a brace)
    const auto opener = [&](std::size_t j) -> std::optional<std::size_t> {
        const std::size_t ind = indent_of(at_text(j));
        for (std::size_t q = j; q-- > 0;)
            if (lines[q] && !blank(*lines[q]) && indent_of(*lines[q]) == ind && txt::rtrim(*lines[q]).ends_with("{")) return q;
        return std::nullopt;
    };
    const auto blank_for_none = [&] {
        Lines v;
        for (const auto& x : lines) v.push_back(x ? *x : string());
        return v;
    };
    // (where the flow goes from line q on: past ends of ifs and the else arms beside them, to a statement)
    const auto after = [&](std::size_t q) -> std::optional<std::size_t> {
        while (q < lines.size()) {
            const string x = str(txt::trim(at_text(q)));
            if (!lines[q] || x.empty() || bare_label.full_match(x)) {
                ++q;
            } else if (x == "}") {
                const auto o = opener(q);
                if (!o || !opens_arm_flow.match(*lines[*o])) return std::nullopt;
                ++q;
            } else if (x == "} else {") {
                const auto e = dt::block_end(blank_for_none(), q);
                q = e && *e != 0 ? *e : lines.size();
            } else {
                return q;
            }
        }
        return std::nullopt;
    };
    bool changed = true;
    while (changed) {
        changed = false;
        for (std::size_t k = 0; k < lines.size() && !changed; ++k) {
            if (!lines[k]) continue;
            const auto m = jump.full_match(*lines[k]);
            if (!m) continue;
            const string label = str(m->group(1));
            std::size_t j = k + 1;
            while (j < lines.size()) {
                const string x = str(txt::trim(at_text(j)));
                if (!lines[j] || x.empty()) {
                    ++j;
                    continue;
                }
                if (x == "}") {
                    const auto q = opener(j);
                    if (!q || !opens_arm.match(*lines[*q])) break;
                    ++j;
                    continue;
                }
                if (is_case_label(x)) {
                    ++j;
                    continue;
                }
                break;
            }
            if (j < lines.size() && txt::trim(at_text(j)) == cat(label, ":")) {
                lines[k].reset();
                changed = true;
                break;
            }
            std::optional<std::size_t> at;
            for (std::size_t q = 0; q < lines.size(); ++q)
                if (lines[q] && txt::trim(*lines[q]) == cat(label, ":")) {
                    at = q;
                    break;
                }
            if (at) {
                const auto here = after(k + 1);
                if (here && here == after(*at + 1)) {   // (the goto goes where its own way goes anyway)
                    lines[k].reset();
                    changed = true;
                    break;
                }
            }
            // (if (C) { goto L; } else { S } with L just after: if (!C) { S }, falling into L)
            const string before = k > 0 ? at_text(k - 1) : string();
            const auto c = k > 0 ? if_open.full_match(before) : std::nullopt;
            if (c && k + 1 < lines.size() && lines[k + 1] && *lines[k + 1] == cat(c->group(1), "} else {")) {
                const auto e = dt::block_end(blank_for_none(), k + 1);
                if (e && txt::trim(at_text(*e)) == "}") {
                    std::size_t q = *e + 1;
                    while (q < lines.size() && case_line.full_match(at_text(q))) ++q;
                    if (q < lines.size() && txt::trim(at_text(q)) == cat(label, ":")) {
                        const string negated = dt::negate(c->group(2));
                        const string stripped = dt::strip_parens(negated);
                        lines[k - 1] = cat(c->group(1), "if (", stripped.starts_with("(") ? stripped : cat("(", negated, ")"), ") {");
                        lines[k].reset();
                        lines[k + 1].reset();
                        changed = true;
                        break;
                    }
                }
            }
        }
    }
    const string rest = join_live(lines);
    return join_lines(drop_unused_labels(live_lines(lines), used_labels(rest)));
}

// ---- tails and heads two arms share -----------------------------------------------------------------------------

string common_tails(string_view text) {
    static const Regex not_a_tail(R"re(^\s*(?:L_\w+:|case\b|default:|\}|else\b))re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) {
            ++k;
            continue;
        }
        const Lines& a = p->then_arm;
        const Lines& b = *p->else_arm;
        std::size_t n = 0;
        while (n < std::min(a.size(), b.size()) && a[a.size() - 1 - n] == b[b.size() - 1 - n] && !blank(a[a.size() - 1 - n])) ++n;
        bool unfit = n == 0;
        for (std::size_t i = a.size() - n; i < a.size() && !unfit; ++i)
            unfit = not_a_tail.match(a[i]).has_value() || txt::rtrim(a[i]).ends_with("{");
        if (unfit) {
            ++k;
            continue;
        }
        const string ind = str(indent_text(lines[k]));
        Lines tail;
        for (std::size_t i = a.size() - n; i < a.size(); ++i) tail.push_back(dedent(a[i], ind));
        const Lines ra = slice(a, 0, a.size() - n), rb = slice(b, 0, b.size() - n);
        const auto has_text = [](const Lines& v) { return std::ranges::any_of(v, [](const string& x) { return !blank(x); }); };
        if (!has_text(ra) && !has_text(rb)) {
            ++k;
            continue;
        }
        Lines replacement;
        if (!has_text(rb)) {
            replacement = {lines[k]};
            replacement.insert(replacement.end(), ra.begin(), ra.end());
            replacement.push_back(ind + "}");
        } else if (!has_text(ra)) {
            const string neg = dt::negate(cat("(", p->cond, ")"));
            const bool wrapped = neg.starts_with("(") && dt::balanced(string_view(neg).substr(1, neg.size() - 2));
            replacement = {cat(ind, "if ", wrapped ? neg : cat("(", neg, ")"), " {")};
            replacement.insert(replacement.end(), rb.begin(), rb.end());
            replacement.push_back(ind + "}");
        } else {
            replacement = {lines[k]};
            replacement.insert(replacement.end(), ra.begin(), ra.end());
            replacement.push_back(ind + "} else {");
            replacement.insert(replacement.end(), rb.begin(), rb.end());
            replacement.push_back(ind + "}");
        }
        replacement.insert(replacement.end(), tail.begin(), tail.end());
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(p->end));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        ++k;
    }
    return join_lines(lines);
}

string common_heads(string_view text) {
    static const Regex store(R"re(\s*([\w\->.\[\]*]+) = ([^;]+);)re");
    static const Regex call_or_step(R"re(\w\(|\+\+|--)re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) {
            ++k;
            continue;
        }
        Lines a, b;
        for (const string& x : p->then_arm)
            if (!blank(x)) a.push_back(x);
        for (const string& x : *p->else_arm)
            if (!blank(x)) b.push_back(x);
        std::optional<RegexMatch> m;
        if (!a.empty() && !b.empty() && txt::trim(a[0]) == txt::trim(b[0])) m = store.full_match(a[0]);
        if (m) {
            // the variable the store goes through: its place up to the first `->` or `[`, without its stars
            string_view place = m->group(1);
            std::size_t cut = place.size();
            for (string_view mark : {"->", "["})
                if (const std::size_t at = place.find(mark); at != string_view::npos) cut = std::min(cut, at);
            place = place.substr(0, cut);
            while (place.starts_with("*")) place.remove_prefix(1);
            if (call_or_step.search(a[0]) || has_word(p->cond, place)) m.reset();
        }
        if (!m) {
            ++k;
            continue;
        }
        const string ind = str(indent_text(lines[k]));
        const std::size_t ia = static_cast<std::size_t>(std::ranges::find(p->then_arm, a[0]) - p->then_arm.begin());
        const std::size_t ib = static_cast<std::size_t>(std::ranges::find(*p->else_arm, b[0]) - p->else_arm->begin());
        Lines replacement{cat(ind, txt::trim(a[0])), lines[k]};
        for (std::size_t i = 0; i < p->then_arm.size(); ++i)
            if (i != ia) replacement.push_back(p->then_arm[i]);
        replacement.push_back(ind + "} else {");
        for (std::size_t i = 0; i < p->else_arm->size(); ++i)
            if (i != ib) replacement.push_back((*p->else_arm)[i]);
        replacement.push_back(ind + "}");
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(p->end));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
        k += 2;
    }
    return join_lines(lines);
}

std::optional<string> heads_variant(string_view text) {
    const string h = common_heads(text);
    if (h == text) return std::nullopt;
    return temporaries(fold_registers(fold_registers(conditional_values(h))));
}

std::optional<string> chain_tails(string_view text) {
    static const Regex tail_start(R"re((?:L_\w+:|case\b|default:))re");
    Lines lines = split_lines(text);

    struct Stripped {
        Lines body;
        bool removed;
    };
    // does the arm leave (return, goto, break, continue)?
    const auto leaves = [](const Lines& ls) {
        string last;
        for (auto it = ls.rbegin(); it != ls.rend(); ++it)
            if (!blank(*it)) {
                last = *it;
                break;
            }
        static const Regex leaving(R"re(^\s*(?:return\b|goto\b|break;|continue;))re");
        return !ls.empty() && !last.empty() && leaving.match(last).has_value();
    };
    // the arm without the tail t (its lines stripped of their indentation), or nothing
    std::function<std::optional<Stripped>(const Lines&, const Lines&)> strip = [&](const Lines& arm, const Lines& t) -> std::optional<Stripped> {
        Lines body = arm;
        while (!body.empty() && blank(body.back())) body.pop_back();
        if (body.size() >= t.size()) {
            bool same = true;
            for (std::size_t i = 0; i < t.size() && same; ++i) same = txt::trim(body[body.size() - t.size() + i]) == t[i];
            if (same) return Stripped{slice(body, 0, body.size() - t.size()), true};
        }
        const auto p = body.empty() ? std::nullopt : dt::if_parts(body, 0);
        if (p && p->end == body.size() && p->else_arm) {
            const auto a = strip(p->then_arm, t), b = strip(*p->else_arm, t);
            const string ind = str(indent_text(body[0]));
            if (a && b && (a->removed || b->removed) && (a->removed || leaves(p->then_arm)) && (b->removed || leaves(*p->else_arm))) {
                Lines joined{body[0]};
                joined.insert(joined.end(), a->body.begin(), a->body.end());
                joined.push_back(ind + "} else {");
                joined.insert(joined.end(), b->body.begin(), b->body.end());
                joined.push_back(ind + "}");
                return Stripped{joined, true};
            }
        }
        if (leaves(body)) return Stripped{body, false};
        return std::nullopt;
    };

    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm) continue;
        Lines th;
        for (const string& x : p->then_arm)
            if (!blank(x)) th.push_back(x);
        struct Best {
            Lines t;
            Stripped a, b;
        };
        std::optional<Best> best;
        for (std::size_t n = 1; n < 7; ++n) {
            if (th.size() < n || !starts_with_return(th.back())) break;
            Lines t;
            for (std::size_t i = th.size() - n; i < th.size(); ++i) t.push_back(str(txt::trim(th[i])));
            const bool unfit = std::ranges::any_of(t, [](const string& x) { return x.ends_with("{") || x.starts_with("}") || tail_start.match(x); });
            if (unfit) break;
            const auto a = strip(p->then_arm, t), b = strip(*p->else_arm, t);
            if (a && b && b->removed) best = Best{t, *a, *b};
        }
        if (!best) continue;
        const string ind = str(indent_text(lines[k]));
        Lines next = slice(lines, 0, k);
        next.push_back(lines[k]);
        next.insert(next.end(), best->a.body.begin(), best->a.body.end());
        next.push_back(ind + "} else {");
        next.insert(next.end(), best->b.body.begin(), best->b.body.end());
        next.push_back(ind + "}");
        for (const string& x : best->t) next.push_back(ind + x);
        for (std::size_t j = p->end; j < lines.size(); ++j) next.push_back(lines[j]);
        return join_lines(next);
    }
    return std::nullopt;
}

// ---- stores two arms meet at -------------------------------------------------------------------------------------

string store_joins(string_view text_in) {
    static const Regex label_only(R"re(\s*(L_\w+):)re");
    static const Regex store_of_reg(R"re((\s*)([\w\->.\[\]]+) = (ax|al|bx|cx|dx);)re");
    string text = str(text_in);
    for (;;) {
        Lines lines = split_lines(text);
        bool again = false;
        for (std::size_t i = 1; i + 1 < lines.size() && !again; ++i) {
            const auto m = label_only.full_match(lines[i]);
            if (!m) continue;
            const auto st = store_of_reg.full_match(lines[i + 1]);
            if (!st) continue;
            const string reg = str(st->group(3));
            const auto a = plain_value(lines[i - 1], reg);
            if (!a) continue;
            const string label = str(m->group(1));
            std::vector<std::size_t> gs;
            for (std::size_t j = 0; j < lines.size(); ++j)
                if (txt::trim(lines[j]) == cat("goto ", label, ";")) gs.push_back(j);
            if (gs.size() != 1) continue;
            const std::size_t g = gs[0];
            if (g == 0) continue;   // (no line before it)
            const auto b = plain_value(lines[g - 1], reg);
            if (!b) continue;
            const std::vector<std::size_t> ol = dt::openers(lines, i), og = dt::openers(lines, g);
            const bool common = std::ranges::any_of(ol, [&](std::size_t x) { return std::ranges::find(og, x) != og.end(); });
            if (!common) continue;
            // (the two ways: the arms of one if below that block; after each store only blocks closing to it)
            const auto closes_to = [&](std::size_t k) {
                std::size_t j = k + 1;
                while (j < lines.size() && (txt::trim(lines[j]) == "}" || txt::trim(lines[j]) == "} else {")) {
                    if (txt::trim(lines[j]) == "} else {") return j;
                    ++j;
                }
                return j;
            };
            std::optional<std::size_t> el;
            for (std::size_t j = std::min(i, g); j < std::max(i, g); ++j) {
                if (txt::trim(lines[j]) != "} else {") continue;
                const auto o = dt::opener(lines, j);
                if (o && (std::ranges::find(ol, *o) != ol.end() || std::ranges::find(og, *o) != og.end())) {
                    el = j;
                    break;
                }
            }
            if (!el) continue;
            const std::size_t el_opener = *dt::opener(lines, *el);
            bool fits = true;
            for (std::size_t e : {closes_to(i + 1), closes_to(g)}) {
                if (e >= lines.size()) {   // (the end of the text: nothing there to close to)
                    fits = false;
                    break;
                }
                const bool this_else = txt::trim(lines[e]) == "} else {" && e == *el;
                if (!this_else && !(indent_of(lines[e]) <= indent_of(lines[el_opener]))) fits = false;
            }
            if (!fits) continue;
            const string ind_s = str(st->group(1)), ind_g = str(indent_text(lines[g]));
            const string store_name = str(st->group(2));
            std::size_t pos = i;
            lines[g - 1] = cat(ind_g, store_name, " = ", *b, ";");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(g));
            if (g < pos) --pos;
            lines[pos - 1] = cat(ind_s, store_name, " = ", *a, ";");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(pos), lines.begin() + static_cast<std::ptrdiff_t>(pos + 2));
            text = join_lines(lines);
            again = true;
        }
        if (!again) return text;
    }
}

string ternary_joins(string_view text) {
    static const Regex first_store(R"re((\s*)(ax|al|bx|cx|dx) = ([^;]+);)re");
    static const Regex label_line(R"re(\s*(L_\w+):)re");
    Lines lines = split_lines(text);
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto p = dt::if_parts(lines, k);
        if (!p || !p->else_arm || p->then_arm.size() < 3) {
            ++k;
            continue;
        }
        const auto a = first_store.full_match(p->then_arm[0]);
        const auto lab = a ? label_line.full_match(p->then_arm[1]) : std::nullopt;
        std::optional<string> stored;   // `\s*([\w\->.\[\]]+) = REG;`
        if (lab) {
            const string_view s = txt::ltrim(p->then_arm[2]);
            const string tail = cat(" = ", a->group(2), ";");
            if (s.ends_with(tail) && s.size() > tail.size() &&
                std::ranges::all_of(s.substr(0, s.size() - tail.size()), [](char c) { return is_word_char(c) || c == '-' || c == '>' || c == '.' || c == '[' || c == ']'; }))
                stored = str(s.substr(0, s.size() - tail.size()));
        }
        Lines el;
        for (const string& x : *p->else_arm)
            if (!blank(x)) el.push_back(str(txt::trim(x)));
        if (!stored || el.size() != 2) {
            ++k;
            continue;
        }
        const auto b = [&]() -> std::optional<string> {
            const string head = cat(a->group(2), " = ");
            if (!el[0].starts_with(head) || !el[0].ends_with(";")) return std::nullopt;
            const string value = el[0].substr(head.size(), el[0].size() - head.size() - 1);
            if (value.empty() || value.find(';') != string::npos) return std::nullopt;
            return value;
        }();
        if (!b || el[1] != cat("goto ", lab->group(1), ";") || count_gotos(join_lines(lines), lab->group(1)) != 1) {
            ++k;
            continue;
        }
        const string ind = str(indent_text(lines[k]));
        const string c = p->cond.starts_with("(") ? p->cond : cat("(", p->cond, ")");
        Lines replacement{cat(ind, a->group(2), " = (", c, " ? ", a->group(3), " : ", *b, ");"),   // (the register still holds it)
                          cat(ind, *stored, " = ", a->group(2), ";")};
        for (std::size_t i = 3; i < p->then_arm.size(); ++i) replacement.push_back(dedent(p->then_arm[i], ind));
        lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k), lines.begin() + static_cast<std::ptrdiff_t>(p->end));
        lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(k), replacement.begin(), replacement.end());
    }
    return join_lines(lines);
}

std::optional<string> label_sinks(string_view text) {
    static const Regex label(R"re((\s*)(L_\w+):)re");
    static const Regex label_line(R"re(\s*L_\w+:)re");
    static const Regex jump(R"re(\s*goto (L_\w+);)re");
    static const Regex while_open(R"re(^\s*while \(.*\) \{$)re");
    Lines lines = split_lines(text);
    for (std::size_t i = 0; i < lines.size(); ++i) {
        const auto m = label.full_match(lines[i]);
        if (!m) continue;
        const string ind = str(m->group(1)), lab = str(m->group(2));
        std::size_t j = i + 1;
        Lines body;
        while (j < lines.size() && lines[j].starts_with(ind) && !blank(lines[j]) && !lines[j].starts_with(ind + "}") && !label_line.full_match(lines[j])) {
            body.push_back(lines[j]);
            ++j;
        }
        // (the block's last statements, inside an arm)
        if (ind.size() < 8 || body.empty() || j >= lines.size() || !lines[j].starts_with(ind.substr(0, ind.size() - 4) + "}")) continue;
        const auto g = jump.full_match(body.back());
        if (!g || std::ranges::any_of(body, [](const string& x) { return txt::rtrim(x).ends_with("{") || txt::trim(x).starts_with("}"); })) continue;
        const string h = str(g->group(1));
        std::optional<std::size_t> hj;
        for (std::size_t k = 0; k < i; ++k)
            if (txt::trim(lines[k]) == cat(h, ":")) {
                hj = k;
                break;
            }
        if (!hj || !while_open.match(lines[*hj + 1])) continue;
        const string hind = str(indent_text(lines[*hj + 1]));
        std::optional<std::size_t> end;
        for (std::size_t k = *hj + 2; k < lines.size(); ++k)
            if (lines[k] == hind + "}") {
                end = k;
                break;
            }
        if (!end || !(*hj + 1 < i && i < *end)) continue;
        Lines moved{cat(hind, "    ", lab, ":")};
        for (std::size_t x = 0; x + 1 < body.size(); ++x) moved.push_back(cat(hind, "    ", txt::trim(body[x])));
        Lines next = slice(lines, 0, i);
        next.push_back(cat(ind, "goto ", lab, ";"));
        for (std::size_t x = j; x < *end; ++x) next.push_back(lines[x]);
        next.insert(next.end(), moved.begin(), moved.end());
        for (std::size_t x = *end; x < lines.size(); ++x) next.push_back(lines[x]);
        string t = join_lines(next);
        if (!goes_to(t, h)) t = join_lines(without_label(split_lines(t), h));
        return t;
    }
    return std::nullopt;
}

std::optional<string> shared_exit(string_view text) {
    static const Regex indented_return(R"re(\s+return [a-z]\w*;)re");
    static const Regex return_var(R"re(return [a-z]\w*;)re");
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    Lines lines = split_lines(split.body);
    std::optional<std::size_t> last;
    for (std::size_t k = lines.size(); k-- > 0;)
        if (!blank(lines[k])) {
            last = k;
            break;
        }
    if (!last || txt::trim(lines[*last]) != "}") return std::nullopt;
    std::optional<std::size_t> prev;
    for (std::size_t k = *last; k-- > 0;)
        if (!blank(lines[k])) {
            prev = k;
            break;
        }
    if (!prev || starts_with_return(lines[*prev])) return std::nullopt;
    std::optional<std::size_t> ret;
    for (std::size_t k = 0; k < lines.size(); ++k)
        if (indented_return.full_match(lines[k]) && indent_of(lines[k]) > 4) ret = k;
    if (!ret) return std::nullopt;
    const string v = str(txt::trim(lines[*ret]));
    lines[*ret] = return_var.replace_all(lines[*ret], "goto out_;");
    lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(*last), {"out_:", "    " + v});
    return cat(split.head, split.brace, join_lines(lines));
}

}  // namespace cb::exact::draft::shapes
