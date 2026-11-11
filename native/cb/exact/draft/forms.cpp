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

namespace {

// ---- the frame --------------------------------------------------------------------------------------------------

// A line `[ \t]*char NAME[N];` that ends there: the length of what precedes the `[`, or nullopt.
std::optional<std::size_t> char_array_decl(sv line, sv name) {
    std::size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
    if (!line.substr(i).starts_with("char ")) return std::nullopt;
    i += 5;
    if (!line.substr(i).starts_with(name)) return std::nullopt;
    i += name.size();
    const std::size_t prefix = i;
    if (i >= line.size() || line[i] != '[') return std::nullopt;
    ++i;
    const std::size_t digits = i;
    while (i < line.size() && is_digit(line[i])) ++i;
    if (i == digits || line.substr(i) != "];") return std::nullopt;
    return prefix;
}

struct FrameDecl {
    std::string name;
    bool reg = false;
    std::int64_t size = 0;
    friend bool operator==(const FrameDecl&, const FrameDecl&) = default;
};

}  // namespace

// the locals renamed so that the compiler gives each the frame slot the program shows. rule (the toolchain profile's
// "frame_order"): {"mod": M} - the compiler gives the locals of a function (register ones too) their slots in the
// order of their names' buckets in its table of names, (the sum of the name's characters) mod M, ascending, the first
// next to BP. An auto the model names vN is at BP-N; a filler _pN after vN; a register local takes a free slot. A name
// is kept where its bucket falls in order, else a letter is put after it.
std::optional<std::string> frame_names(sv text, const FrameRule& rule, FrameOptions options) {
    if (!rule.mod || *rule.mod == 0 || text.find('{') == npos) return std::nullopt;
    const std::int64_t mod = *rule.mod;
    const dt::BodySplit split = dt::split_body(text);
    const std::string head = split.head;
    std::string body = split.body;
    const Lines lines = split_nl(body);
    static const Regex decl_re(
        R"(\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short)\b|struct\s+\w+)\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$)");
    std::vector<FrameDecl> decls;
    for (std::size_t k = 1; k < lines.size(); ++k) {
        const auto m = decl_re.match(lines[k]);
        if (!m) {
            if (!blank(lines[k])) break;
            continue;
        }
        const std::string_view type = (*m)[2];
        const bool star = !(*m)[3].empty();
        if (type.starts_with("struct") && !star) return std::nullopt;   // (a struct's size the text does not say)
        const auto words = ct::split_ws(type);
        std::int64_t size = 2;
        if (!star) size = dt::size_of_type(words.back(), 2) * (m->has(5) ? to_int((*m)[5]) : 1);
        FrameDecl d{str((*m)[4]), m->has(1), 0};
        d.size = !m->has(5) ? std::max<std::int64_t>(size, 2) : size + (size & 1);
        decls.push_back(std::move(d));
    }
    static const Regex auto_re(R"(v\d+)");
    static const Regex filler_re(R"(_p\d+)");
    std::vector<FrameDecl> autos, fill, regs;
    for (const FrameDecl& d : decls) {
        if (!d.reg && auto_re.full_match(d.name)) autos.push_back(d);
        if (filler_re.full_match(d.name)) {   // (by name: a name declared twice is the later declaration, where the first stood)
            if (const auto same = std::ranges::find(fill, d.name, &FrameDecl::name); same != fill.end()) *same = d;
            else fill.push_back(d);
        }
        if (d.reg) regs.push_back(d);
    }
    std::ranges::stable_sort(autos, {}, [](const FrameDecl& d) { return to_int(std::string_view(d.name).substr(1)); });
    if (autos.empty()) return std::nullopt;

    std::vector<FrameDecl> order, added;
    std::set<std::string> drop;
    std::map<std::string, std::int64_t> resize;
    std::int64_t cum = 0;
    const auto find_fill = [&](const std::string& name) {
        return std::ranges::find(fill, name, &FrameDecl::name);
    };
    for (const FrameDecl& d : autos) {   // (each auto at its place; what lies between: register locals, then a filler)
        const std::int64_t n = to_int(std::string_view(d.name).substr(1));
        std::int64_t gap = n - d.size - cum;
        int took = 0;
        while (gap >= 2 && !regs.empty() && !options.registers_last) {
            order.push_back(regs.front());
            regs.erase(regs.begin());
            gap -= 2;
            ++took;
        }
        const std::string filler = "_p" + std::to_string(cum);
        if (const auto f = find_fill(filler); f != fill.end()) {
            if (gap <= 0) {   // (the register locals take the room: no filler)
                drop.insert(filler);
                fill.erase(f);
            } else {
                order.push_back(*f);
                if (took) resize[filler] = gap;   // (the filler what they leave of it)
            }
        } else if (gap > 0 && options.register_local_has_slot) {   // (room no local takes: an unused local of the source's)
            FrameDecl made{filler, false, gap};
            order.push_back(made);
            added.push_back(made);
        }
        order.push_back(d);
        cum = n;
    }
    for (const FrameDecl& r : regs) order.push_back(r);
    for (const FrameDecl& f : fill)
        if (std::ranges::find(order, f) == order.end()) order.push_back(f);
    if (static_cast<std::int64_t>(order.size()) > mod) return std::nullopt;

    const std::set<std::string> taken = identifiers(text);
    const auto bucket = [mod](sv s) {
        std::int64_t sum = 0;
        for (const char c : s) sum += static_cast<unsigned char>(c);
        return sum % mod;
    };
    std::vector<std::pair<std::string, std::string>> renames;
    std::int64_t prev = -1;
    for (std::size_t k = 0; k < order.size(); ++k) {
        const std::int64_t room = mod - static_cast<std::int64_t>(order.size() - k);   // (the last bucket this one may take)
        std::string name = order[k].name;
        if (!(prev < bucket(name) && bucket(name) <= room)) {
            const std::int64_t want = prev + 1;
            std::optional<std::string> found;
            for (char c = 'a'; c <= 'z' && !found; ++c) {
                const std::string candidate = order[k].name + c;
                if (bucket(candidate) == want && !taken.contains(candidate)) found = candidate;
            }
            if (!found) return std::nullopt;
            const auto again = std::ranges::find(renames, order[k].name, &std::pair<std::string, std::string>::first);
            if (again != renames.end()) again->second = *found;   // (a name declared twice: the later letter)
            else renames.emplace_back(order[k].name, *found);
            name = *found;
        }
        prev = bucket(name);
    }
    if (renames.empty() && drop.empty() && resize.empty() && added.empty()) return std::nullopt;

    if (!added.empty()) {   // (the fillers made: declared after the others)
        static const Regex decl_line(R"(\s+(?:register\s+)?[\w\s*]+?\w+(?:\[\d+\])?;)");
        Lines bl = split_nl(body);
        std::size_t j = 1;
        while (j < bl.size() && (blank(bl[j]) || decl_line.full_match(bl[j]))) ++j;
        const std::string ind = bl.size() > 1 && !blank(bl[1]) ? lead_ws(bl[1]) : "    ";
        Lines made;
        for (const FrameDecl& f : added) made.push_back(std::format("{}char {}[{}];", ind, f.name, f.size));
        bl.insert(bl.begin() + static_cast<std::ptrdiff_t>(j), made.begin(), made.end());
        body = join_nl(bl);
    }
    if (!drop.empty() || !resize.empty()) {
        Lines bl = split_nl(body);
        std::vector<char> gone(bl.size(), 0);
        for (std::size_t i = 0; i < bl.size(); ++i) {
            for (const std::string& f : drop)   // (a whole line, with its newline: the body's last line has none)
                if (i + 1 < bl.size() && char_array_decl(bl[i], f)) gone[i] = 1;
            for (const auto& [f, size] : resize)
                if (const auto prefix = char_array_decl(bl[i], f); prefix && !gone[i])
                    bl[i] = bl[i].substr(0, *prefix) + std::format("[{}];", size);
        }
        body = join_live(bl, gone);
    }
    for (const auto& [from, to] : renames) body = replace_word(body, from, to);
    return head + "{" + body;
}

// ---- pointers ---------------------------------------------------------------------------------------------------

namespace {

// `\(\(?ST A OP ST B\)` where ST is an optional ++ or --: A compared with B in parentheses.
bool compared_in_parens(sv body, sv a, sv b) {
    const auto skip_step = [&](std::size_t i) {
        return body.substr(i).starts_with("++") || body.substr(i).starts_with("--") ? i + 2 : i;
    };
    static constexpr sv ops[] = {"==", "!=", "<=", ">=", "<", ">"};
    const auto try_at = [&](std::size_t pos) {
        std::size_t i = skip_step(pos);
        if (!body.substr(i).starts_with(a)) return false;
        i += a.size();
        if (!body.substr(i).starts_with(' ')) return false;
        ++i;
        for (const sv op : ops) {
            if (!body.substr(i).starts_with(op)) continue;
            std::size_t j = i + op.size();
            if (!body.substr(j).starts_with(' ')) continue;
            j = skip_step(j + 1);
            if (!body.substr(j).starts_with(b)) continue;
            j += b.size();
            if (j < body.size() && body[j] == ')') return true;
        }
        return false;
    };
    for (std::size_t i = body.find('('); i != npos; i = body.find('(', i + 1)) {
        if (try_at(i + 1)) return true;
        if (i + 1 < body.size() && body[i + 1] == '(' && try_at(i + 2)) return true;
    }
    return false;
}

// A line that is `[ws]x = p;` (the value itself: not a field of it).
bool given_value(sv body, sv x, sv p) {
    const std::string want = str(x) + " = " + str(p) + ";";
    for (const sv line : ct::split(body, '\n'))
        if (ct::ltrim(line).starts_with(want)) return true;
    return false;
}

}  // namespace

// the pointers a function's names are, from those it declares: a name compared with a pointer, given one's value, or
// giving its value to one is one too (a pointer compares without its sign: the code shows which)
std::string pointer_flow(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    static const Regex decl(R"((?m)^([ \t]*)(register\s+)?int\s+(\w+);$)");
    static const Regex char_ptr(R"((?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);)");
    std::set<std::string> ptr;
    char_ptr.for_each_match(text, [&](const RegexMatch& m) {
        ptr.insert(str(m[1]));
        return true;
    });
    if (ptr.empty()) return str(text);
    std::set<std::string> ints;
    decl.for_each_match(text, [&](const RegexMatch& m) {
        ints.insert(str(m[3]));
        return true;
    });
    const std::string& body = split.body;
    // The pointers a name is tied to by a compare or a copy: the closure is the same in any order.
    for (bool changed = true; changed;) {
        changed = false;
        for (const std::string& x : std::vector<std::string>(ints.begin(), ints.end())) {
            if (ptr.contains(x)) continue;
            for (const std::string& p : ptr) {
                if (compared_in_parens(body, x, p) || compared_in_parens(body, p, x) || given_value(body, x, p) ||
                    given_value(body, p, x)) {
                    ptr.insert(x);
                    changed = true;
                    break;
                }
            }
        }
    }
    const auto renew = [&](const RegexMatch& m) {
        if (!ptr.contains(str(m[3]))) return str(m.whole());
        return str(m[1]) + str(m[2]) + "char *" + str(m[3]) + ";";
    };
    return sub_all(decl, split.head, renew) + split.brace + sub_all(decl, body, renew);
}

// the text with one of its int parameters a char (each, at most `most`): one compared with a byte, or given one, may
// be a char the caller widened (a variant: the compiler decides)
std::vector<std::string> char_param_variants(sv text, std::size_t most) {
    const dt::BodySplit split = dt::split_body(text);
    std::vector<std::string> out;
    static const Regex decl(R"((?m)^([ \t]*)int ([\w, ]+);$)");
    const auto m = decl.search(split.head);
    if (!m) return out;
    std::vector<std::string> names;
    for (const sv n : ct::split(m->group(2), ',')) names.push_back(str(ct::trim(n)));
    const std::string ind = str((*m)[1]);
    const auto declare = [&](const std::vector<std::string>& rest, const std::string& chars) {
        std::string d;
        if (!rest.empty()) d += ind + "int " + ct::join(rest, ", ") + ";\n";
        return d + ind + "char " + chars + ";";
    };
    const auto variant = [&](const std::string& decl_text) {
        return split.head.substr(0, m->start()) + decl_text + split.head.substr(m->end()) + split.brace + split.body;
    };
    for (std::size_t i = 0; i < names.size() && i < most; ++i) {
        const std::string& p = names[i];
        if (!has_word(split.body, p)) continue;
        std::vector<std::string> rest;
        for (const std::string& x : names)
            if (x != p) rest.push_back(x);
        out.push_back(variant(declare(rest, p)));
    }
    std::vector<std::string> used;
    for (const std::string& p : names)
        if (has_word(split.body, p)) used.push_back(p);
    if (used.size() > 1) {   // (and all of them chars)
        std::vector<std::string> first(used.begin(), used.begin() + static_cast<std::ptrdiff_t>(std::min(most, used.size())));
        std::vector<std::string> rest;
        for (const std::string& x : names)
            if (std::ranges::find(first, x) == first.end()) rest.push_back(x);
        out.push_back(variant(declare(rest, ct::join(first, ", "))));
    }
    return out;
}

namespace {

// `\bP = P ([+-]) (\d+);` with an even step: P stepped by half as many words.
std::string pointer_steps_in_words(sv t, sv p) {
    std::string out;
    std::size_t at = 0, from = 0;
    const std::string lead = str(p) + " = " + str(p) + " ";
    for (std::size_t pos = t.find(lead, from); pos != npos; pos = t.find(lead, from)) {
        from = pos + 1;
        if (!boundary_at(t, pos)) continue;
        const std::size_t i = pos + lead.size();
        if (i + 1 >= t.size() || (t[i] != '+' && t[i] != '-') || t[i + 1] != ' ') continue;
        std::size_t d = i + 2;
        const std::size_t digits = d;
        while (d < t.size() && is_digit(t[d])) ++d;
        if (d == digits || d >= t.size() || t[d] != ';') continue;
        const std::int64_t n = to_int(t.substr(digits, d - digits));
        if (n % 2 != 0) continue;   // (an odd step is left as it is)
        out.append(t.substr(at, pos - at));
        out += std::format("{} = {} {} {};", p, p, t[i], n / 2);
        at = from = d + 1;
    }
    out.append(t.substr(at));
    return out;
}

// `register char *p;` as `register int *p;` (each line that is exactly that declaration).
std::string widen_pointer_decl(sv t, sv p) {
    Lines lines = split_nl(t);
    const std::string tail = "register char *" + str(p) + ";";
    for (std::string& l : lines) {
        const std::size_t ind = l.find_first_not_of(" \t");
        if (ind != npos && l.substr(ind) == tail) l = l.substr(0, ind) + "register int *" + str(p) + ";";
    }
    return join_nl(lines);
}

}  // namespace

// the text with its register pointers (`register char *p;`, at most `most` of them) made int * - each, then all: what
// the model read through them may be words (the text alone does not say)
std::vector<std::string> pointer_widths(sv text, std::size_t most) {
    static const Regex ptr_re(R"((?m)^[ \t]*register char \*(\w+);)");
    std::vector<std::string> ps;
    ptr_re.for_each_match(text, [&](const RegexMatch& m) {
        if (ps.size() < most) ps.push_back(str(m[1]));
        return ps.size() < most;
    });
    const auto words = [](sv t, const std::string& p) { return pointer_steps_in_words(widen_pointer_decl(t, p), p); };
    std::vector<std::string> out;
    for (const std::string& p : ps) out.push_back(words(text, p));
    if (ps.size() > 1) {
        std::string t = str(text);
        for (const std::string& p : ps) t = words(t, p);
        out.push_back(t);
    }
    return out;
}

// the text with one of its int globals unsigned (each): a variable compared, the compiler's code says how
std::vector<std::string> unsigned_globals(sv text, std::size_t most) {
    std::vector<std::string> out;
    static const Regex decl(R"((?m)^extern int ([\w, ]+);$)");
    const auto m = decl.search(text);
    if (!m) return out;
    std::vector<std::string> names;
    for (const sv n : ct::split(m->group(1), ',')) names.push_back(str(ct::trim(n)));
    for (std::size_t i = 0; i < names.size() && i < most; ++i) {
        std::vector<std::string> rest;
        for (const std::string& x : names)
            if (x != names[i]) rest.push_back(x);
        std::string d;
        if (!rest.empty()) d += "extern int " + ct::join(rest, ", ") + ";\n";
        d += "extern unsigned " + names[i] + ";";
        out.push_back(str(text.substr(0, m->start())) + d + str(text.substr(m->end())));
    }
    return out;
}

// ---- steps ------------------------------------------------------------------------------------------------------

namespace {

// The statement is `if (` or `while (` at the start of the line, after any whitespace, then the pointer `x`
// (parenthesised once more, perhaps): the offset just after the cut `x`, and the offset where x starts.
struct CondStart {
    std::size_t x_at = 0, rest_at = 0;
};
std::optional<CondStart> cond_starts_with(sv line, sv x) {
    std::size_t i = indent_of(line);
    if (line.substr(i).starts_with("if (")) i += 4;
    else if (line.substr(i).starts_with("while (")) i += 7;
    else return std::nullopt;
    if (i < line.size() && line[i] == '(') ++i;
    if (!line.substr(i).starts_with(x) || !boundary_at(line, i + x.size())) return std::nullopt;
    return CondStart{i, i + x.size()};
}

// `\s*x\s*=` at the start of the line.
bool starts_with_assign_to(sv line, sv x) {
    sv rest = ct::ltrim(line);
    if (!rest.starts_with(x)) return false;
    rest = ct::ltrim(rest.substr(x.size()));
    return rest.starts_with('=');
}

}  // namespace

// X--; (X++; X = X - 1;) then a test of X: the step in the test - if (--X ..) - a variant: the compiler tests the
// flags of the step there, not a compare
std::optional<std::string> pre_steps(sv text) {
    static const Regex step_post(R"(\s*(\w+(?:->f\d+)?)(\+\+|--);\s*$)");
    static const Regex step_assign(R"(\s*(\w+) = \1 ([+-]) [12];\s*$)");
    static const Regex not_a_read(R"(\s*(?:\}|else\b|L_\w+:))");
    Lines lines = split_nl(text);
    std::vector<char> gone(lines.size(), 0);
    bool changed = false;
    std::size_t k = 0;
    while (k + 1 < lines.size()) {
        auto m = step_post.match(lines[k]);
        if (!m) m = step_assign.match(lines[k]);
        if (m) {
            const std::string x = str((*m)[1]);
            const std::string_view raw_op = (*m)[2];
            const std::string op = raw_op == "++" || raw_op == "+" ? "++" : "--";
            // (by 2: a pointer to words stepped once - declared so where the compiler asks)
            if (const auto t = cond_starts_with(lines[k + 1], x)) {
                lines[k + 1] = lines[k + 1].substr(0, t->x_at) + op + x + lines[k + 1].substr(t->rest_at);
                gone[k] = 1;
                changed = true;
                k += 2;
                continue;
            }
            const std::string& next = lines[k + 1];   // (or read once by the next statement: --x in place)
            if (count_word(next, x) == 1 && !starts_with_assign_to(next, x) && !not_a_read.match(next)) {
                lines[k + 1] = replace_word(next, x, op + x, true);
                gone[k] = 1;
                changed = true;
                k += 2;
                continue;
            }
        }
        ++k;
    }
    if (!changed) return std::nullopt;
    return join_live(lines, gone);
}

// X = X + n; then *X read once: *++X in its place - X a pointer to things of n bytes (a variant)
std::optional<std::string> pre_increments(sv text, FunctionFacts& facts) {
    static const Regex step(R"(\s*(\w+) = \1 \+ ([12]);\s*$)");
    Lines lines = split_nl(text);
    std::vector<char> gone(lines.size(), 0);
    bool changed = false;
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        if (gone[k]) continue;
        const auto m = step.match(lines[k]);
        if (!m) continue;
        const std::string x = str((*m)[1]);
        const int n = static_cast<int>(to_int((*m)[2]));
        const std::string& next = lines[k + 1];
        if (count_word(next, x) != 1) continue;
        // `*x` not followed by ->, [, ++ or --: the read the step goes into
        const std::string star = "*" + x;
        std::size_t at = npos;
        for (std::size_t p = next.find(star); p != npos; p = next.find(star, p + 1)) {
            if (!boundary_at(next, p + star.size())) continue;
            sv after = ct::ltrim(sv(next).substr(p + star.size()));
            if (after.starts_with("->") || after.starts_with("[") || after.starts_with("++") || after.starts_with("--")) continue;
            at = p;
            break;
        }
        if (at == npos) continue;
        lines[k + 1] = next.substr(0, at) + "*++" + x + next.substr(at + star.size());
        gone[k] = 1;
        if (n == 2) {
            facts.word_pointers.insert(x);
            const std::string decl = "char *" + x + ";";
            static constexpr sv reg = "register ";
            for (std::size_t i = 0; i < lines.size(); ++i) {   // (`^(\s*register )char *x;` as `int *x;`)
                if (gone[i]) continue;
                const sv l = lines[i];
                const std::size_t p = indent_of(l);
                if (l.substr(p).starts_with(reg) && l.substr(p + reg.size()).starts_with(decl))
                    lines[i] = str(l.substr(0, p + reg.size())) + "int *" + x + ";" + str(l.substr(p + reg.size() + decl.size()));
            }
        }
        changed = true;
    }
    if (!changed) return std::nullopt;
    return join_live(lines, gone);
}

// ---- address variants -------------------------------------------------------------------------------------------

namespace {

// `(?<![\w.])LIT(?![\w.])` replaced by `with`.
std::string replace_number_literal(sv text, sv lit, sv with) {
    std::string out;
    std::size_t at = 0;
    for (std::size_t p = text.find(lit); p != npos; p = text.find(lit, at)) {
        const bool before_ok = p == 0 || !(is_word(text[p - 1]) || text[p - 1] == '.');
        const std::size_t e = p + lit.size();
        const bool after_ok = e >= text.size() || !(is_word(text[e]) || text[e] == '.');
        if (before_ok && after_ok) {
            out.append(text.substr(at, p - at));
            out.append(with);
            at = e;
        } else {
            out.append(text.substr(at, p + 1 - at));
            at = p + 1;
        }
    }
    out.append(text.substr(at));
    return out;
}

}  // namespace

// a number a pointer is compared with, or given (`p < 14708`, `p = 14708`): the place of data there - an array at that
// offset, g_XXXX (a variant each: the compiler decides; a pointer compares without its sign)
std::vector<AddressVariant> address_variants(sv text, std::uint32_t data_size, std::size_t most, std::uint32_t lo) {
    static const Regex ptr_decl(R"((?m)^[ \t]*(?:register\s+)?(?:char|int|struct \w+)\s*\*\s*(\w+);)");
    std::set<std::string> ptrs;
    ptr_decl.for_each_match(text, [&](const RegexMatch& m) {
        ptrs.insert(str(m[1]));
        return true;
    });
    std::vector<AddressVariant> out;
    if (ptrs.empty()) return out;
    // `\b(PTR)\s*(?:[<>]=?|==|!=|=)\s*(-?\d{3,5})\b`: a pointer compared with, or given, a number of 3 to 5 digits.
    std::vector<std::pair<std::int64_t, std::string>> nums;
    for (std::size_t i = 0; i < text.size();) {
        if (!is_word(text[i]) || !boundary_at(text, i)) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < text.size() && is_word(text[e])) ++e;
        const std::size_t after_name = e;
        if (ptrs.contains(str(text.substr(i, e - i)))) {
            std::size_t j = after_name;
            while (j < text.size() && ct::is_space(text[j])) ++j;
            bool op = false;
            if (j < text.size() && (text[j] == '<' || text[j] == '>')) {
                ++j;
                if (j < text.size() && text[j] == '=') ++j;
                op = true;
            } else if (text.substr(j).starts_with("==") || text.substr(j).starts_with("!=")) {
                j += 2;
                op = true;
            } else if (j < text.size() && text[j] == '=') {
                ++j;
                op = true;
            }
            if (op) {
                while (j < text.size() && ct::is_space(text[j])) ++j;
                const std::size_t lit_at = j;
                if (j < text.size() && text[j] == '-') ++j;
                const std::size_t d0 = j;
                while (j < text.size() && is_digit(text[j])) ++j;
                if (j - d0 >= 3 && j - d0 <= 5 && (j >= text.size() || !is_word(text[j]))) {
                    const std::string lit = str(text.substr(lit_at, j - lit_at));
                    const std::int64_t n = to_int(lit) & 0xFFFF;   // (a place above 32K the model writes negative)
                    if (n >= static_cast<std::int64_t>(lo) && n < static_cast<std::int64_t>(data_size) &&
                        std::ranges::find(nums, std::make_pair(n, lit)) == nums.end())
                        nums.emplace_back(n, lit);
                }
            }
        }
        i = e;
    }
    for (std::size_t k = 0; k < nums.size() && k < most; ++k) {
        const std::string name = hex_name(nums[k].first);
        out.push_back({name, replace_number_literal(text, nums[k].second, name)});
    }
    return out;
}

// ---- declarations -----------------------------------------------------------------------------------------------

namespace {

// The names of a declaration list `a, b, c` (each `\w+`, separated by ", "), or nullopt when it is not one.
std::optional<std::vector<std::string>> name_list(sv s) {
    std::vector<std::string> names;
    std::size_t i = 0;
    while (true) {
        const std::size_t b = i;
        while (i < s.size() && is_word(s[i])) ++i;
        if (i == b) return std::nullopt;
        names.emplace_back(s.substr(b, i - b));
        if (i == s.size()) return names;
        if (!s.substr(i).starts_with(", ")) return std::nullopt;
        i += 2;
    }
}

}  // namespace

// t without x in its `int a, x, b;` declaration (the line gone if x was all of it)
std::string drop_decl(sv t, sv x) {
    const Lines lines = split_nl(t);
    for (std::size_t i = 0; i + 1 < lines.size(); ++i) {   // (a line with its newline: the text's last has none)
        const std::string& l = lines[i];
        std::size_t p = 0;
        while (p < l.size() && (l[p] == ' ' || l[p] == '\t')) ++p;
        if (!sv(l).substr(p).starts_with("int ") || !sv(l).ends_with(';')) continue;
        const auto names = name_list(sv(l).substr(p + 4, l.size() - p - 5));
        if (!names) continue;
        const auto it = std::find(names->rbegin(), names->rend(), x);   // (greedy: the last name that is x)
        if (it == names->rend()) continue;
        std::vector<std::string> rest(names->begin(), names->end());
        rest.erase(rest.begin() + (names->rend() - it - 1));
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(i));
        if (!rest.empty()) out.push_back(l.substr(0, p) + "int " + ct::join(rest, ", ") + ";");
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(i) + 1, lines.end());
        return join_nl(out);
    }
    return str(t);
}

namespace {

struct Declaration {
    std::size_t line = 0;
    std::string indent, reg, type;   // reg: "register " (with its whitespace) or empty
    std::vector<std::string> names;
};

// `^([ \t]*)(register\s+)?(struct \w+|\w+)\s*\*?\s*((?:\w+, )*)X((?:, \w+)*);$` on one line.
std::optional<Declaration> declaration_of(sv l, sv x) {
    std::size_t p = 0;
    while (p < l.size() && (l[p] == ' ' || l[p] == '\t')) ++p;
    Declaration d;
    d.indent = str(l.substr(0, p));
    std::size_t q = p;
    if (l.substr(q).starts_with("register") && q + 8 < l.size() && ct::is_space(l[q + 8])) {
        std::size_t e = q + 8;
        while (e < l.size() && ct::is_space(l[e])) ++e;
        d.reg = str(l.substr(q, e - q));
        q = e;
    }
    if (!l.ends_with(';')) return std::nullopt;
    const auto rest_of = [&](std::size_t from, std::string type) -> std::optional<Declaration> {
        std::size_t i = from;
        while (i < l.size() && ct::is_space(l[i])) ++i;
        if (i < l.size() && l[i] == '*') ++i;
        while (i < l.size() && ct::is_space(l[i])) ++i;
        if (i >= l.size()) return std::nullopt;
        const auto names = name_list(l.substr(i, l.size() - 1 - i));
        if (!names || std::ranges::find(*names, x) == names->end()) return std::nullopt;
        Declaration out = d;
        out.type = std::move(type);
        out.names = *names;
        return out;
    };
    std::size_t e = q;
    if (l.substr(q).starts_with("struct ")) {   // (the longer alternative first)
        std::size_t w = q + 7;
        const std::size_t w0 = w;
        while (w < l.size() && is_word(l[w])) ++w;
        if (w > w0)
            if (auto r = rest_of(w, str(l.substr(q, w - q)))) return r;
    }
    while (e < l.size() && is_word(l[e])) ++e;
    if (e == q) return std::nullopt;
    return rest_of(e, str(l.substr(q, e - q)));
}

}  // namespace

// t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None
std::optional<std::string> redeclare(sv t, sv x, sv new_declaration) {
    const Lines lines = split_nl(t);
    for (std::size_t i = 0; i < lines.size(); ++i) {   // (the first line that is a declaration of x)
        const auto d = declaration_of(lines[i], x);
        if (!d) continue;
        if (d->type == "return" || d->type == "goto" || d->type == "extern") return std::nullopt;
        std::vector<std::string> rest = d->names;
        rest.erase(std::find(rest.rbegin(), rest.rend(), x).base() - 1);
        Lines out(lines.begin(), lines.begin() + static_cast<std::ptrdiff_t>(i));
        std::string rep;
        if (!rest.empty()) rep += d->indent + d->reg + d->type + " " + ct::join(rest, ", ") + ";\n";
        rep += d->indent + d->reg + str(new_declaration) + ";";
        out.push_back(rep);
        out.insert(out.end(), lines.begin() + static_cast<std::ptrdiff_t>(i) + 1, lines.end());
        return join_nl(out);
    }
    return std::nullopt;
}

// ---- scaled indexes and places in the data ----------------------------------------------------------------------

namespace {

// The loaded register's `->fN` places.
struct FieldRef {
    std::size_t pos = 0, len = 0;
    std::string digits;
};

// `\bREG->f(\d+)` in the line.
std::vector<FieldRef> field_refs(sv line, sv reg) {
    std::vector<FieldRef> out;
    const std::string lead = str(reg) + "->f";
    for (std::size_t p = line.find(lead); p != npos; p = line.find(lead, p + 1)) {
        if (!boundary_at(line, p)) continue;
        std::size_t e = p + lead.size();
        const std::size_t d0 = e;
        while (e < line.size() && is_digit(line[e])) ++e;
        if (e > d0) out.push_back({p, e - p, str(line.substr(d0, e - d0))});
    }
    return out;
}

// `\bREG\b(?!->f(?:\d{3,}))`: the register used as itself, not as the base of a place in the data.
bool bare_use(sv line, sv reg) {
    for (std::size_t p = find_word(line, reg); p != npos; p = find_word(line, reg, p + reg.size())) {
        sv tail = line.substr(p + reg.size());
        bool place = false;
        if (tail.starts_with("->f")) {
            std::size_t d = 3;
            while (d < tail.size() && is_digit(tail[d])) ++d;
            place = d - 3 >= 3;
        }
        if (!place) return true;
    }
    return false;
}

// `\bE\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*\bE\b`: E given a value, or stepped.
bool given_or_stepped(sv line, sv e) {
    for (std::size_t p = line.find(e); p != npos; p = line.find(e, p + 1)) {
        if (boundary_at(line, p)) {
            std::size_t q = p + e.size();
            while (q < line.size() && ct::is_space(line[q])) ++q;
            if (q < line.size() && line[q] == '=' && q + 1 < line.size() && line[q + 1] != '=') return true;
            if (line.substr(q).starts_with("++") || line.substr(q).starts_with("--")) return true;
        }
        // a step before it: (++|--)\s*E, E ending on a boundary
        std::size_t b = p;
        while (b > 0 && ct::is_space(line[b - 1])) --b;
        if (b >= 2 && (line.substr(b - 2, 2) == "++" || line.substr(b - 2, 2) == "--") && boundary_at(line, p + e.size()) &&
            boundary_at(line, p))
            return true;
    }
    return false;
}

// `\w\((?:[^()]*, )?M(?:, [^()]*)?\)`: M an argument of a call, at the top of its parentheses.
bool is_call_argument(sv line, sv m) {
    for (std::size_t p = line.find(m); p != npos; p = line.find(m, p + 1)) {
        const sv before = line.substr(0, p);
        const std::size_t open = before.rfind('(');
        if (open == npos || open == 0 || !is_word(line[open - 1])) continue;
        const sv args = before.substr(open + 1);
        if (args.find(')') != npos) continue;
        if (!args.empty() && !args.ends_with(", ")) continue;
        const sv after = line.substr(p + m.size());
        if (after.starts_with(")")) return true;
        if (after.starts_with(", ")) {
            const std::size_t paren = after.find_first_of("()", 2);
            if (paren != npos && after[paren] == ')') return true;
        }
    }
    return false;
}

// The line holding [start, end) of `subject`.
sv line_around(sv subject, std::size_t start, std::size_t end) {
    const std::size_t b = start == 0 ? npos : subject.substr(0, start).rfind('\n');
    const std::size_t from = b == npos ? 0 : b + 1;
    const std::size_t to = subject.find('\n', end);
    return subject.substr(from, (to == npos ? subject.size() : to) - from);
}

// An array of structs the code shows: the element's size and the type of its first field.
void struct_array_of(FunctionFacts& facts, const std::string& name, std::uint32_t size, std::string first_type) {
    facts.struct_arrays[name] = dt::StructArray{size, std::move(first_type), {}};
    facts.arrays[name] = "struct s_" + name;
}

}  // namespace

// (E << 1)->fN, N a place in the data (not a field's small offset): an array of words there indexed by E - g_XXXX[E]
// (the model reads `mov bx,E / shl bx,1 / [bx+N]` as a field of a pointer)
std::string word_arrays(sv text_in, FunctionFacts& facts, std::uint32_t lo_in) {
    const std::int64_t lo = lo_in;
    static const Regex rx4(
        R"(\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b|\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b)");
    static const Regex rx4_twice(R"(\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b)");
    static const Regex rx4_quad(R"(\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b)");
    static const Regex ax_load(R"(\s*(?:ax|bx|cx|dx) = )");

    // The places read through an index times 4, and the lines they are read on.
    const Lines ls0 = split_nl(text_in);
    std::map<std::int64_t, std::set<std::size_t>> at4;
    for (std::size_t k = 0; k < ls0.size(); ++k)
        rx4.for_each_match(ls0[k], [&](const RegexMatch& m) {
            at4[to_int(m.has(2) ? m[2] : m[4])].insert(k);
            return true;
        });
    const auto halves = [&](std::size_t x, std::size_t y) {
        if (x == y) return true;
        const std::size_t gap = x > y ? x - y : y - x;
        return gap == 1 && ax_load.match(ls0[x]) && ax_load.match(ls0[y]);
    };
    const auto near_places = [&](std::int64_t a, std::int64_t b) {   // (a long's two words together)
        const auto ia = at4.find(a), ib = at4.find(b);
        if (ia == at4.end() || ib == at4.end()) return false;
        for (const std::size_t x : ia->second)
            for (const std::size_t y : ib->second)
                if (halves(x, y)) return true;
        return false;
    };
    std::set<std::int64_t> pair4;
    for (const auto& entry : at4) {
        const std::int64_t n = entry.first;
        if ((at4.contains(n + 2) && !near_places(n, n + 2)) || (at4.contains(n - 2) && !near_places(n, n - 2))) pair4.insert(n);
    }

    // An index doubled twice: an array of longs there - two words 2 apart read on their own: of 4-byte structs.
    const auto longs = [&](const RegexMatch& m) -> std::string {
        const std::int64_t n = to_int(m[2]);
        if (n < lo) return str(m.whole());
        const std::string name = hex_name(n);
        const sv line = line_around(m.subject(), m.start(), m.end());
        static constexpr sv loaders[] = {"si", "di", "bx", "cx"};
        bool word = false;   // (one word of it loaded into a pointer register, or pushed alone as an argument)
        const sv trimmed = ct::ltrim(line);
        for (const sv r : loaders)
            if (trimmed == str(r) + " = " + str(m.whole()) + ";") word = true;
        if (!word && is_call_argument(line, m.whole())) {
            bool has_long = false;
            for (std::size_t p = line.find("LONG("); p != npos; p = line.find("LONG(", p + 1))
                if (boundary_at(line, p)) has_long = true;
            word = !has_long;
        }
        if (pair4.contains(n) || word) {
            facts.struct_arrays.try_emplace(name, dt::StructArray{4, "int", {}});
            facts.arrays[name] = "struct s_" + name;
            return name + "[" + str(m[1]) + "].f0";
        }
        facts.arrays[name] = "long";
        return name + "[" + str(m[1]) + "]";
    };
    std::string text = sub_all(rx4_twice, text_in, longs);
    text = sub_all(rx4_quad, text, longs);

    static const Regex scaled_one(
        R"(\(((?:(?:\+\+|--)?[A-Za-z_]\w*(?:->f\d+)*(?:\+\+|--)?|\d+)(?:\s*[-+]\s*\d+)?) << 1\)->f(\d+)\b)");
    std::string t = sub_all(scaled_one, text, [&](const RegexMatch& m) {
        const std::int64_t n = to_int(m[2]);
        if (n < lo) return str(m.whole());
        const std::string name = hex_name(n);
        facts.arrays[name] = "int";
        return name + "[" + str(m[1]) + "]";
    });
    static const Regex scaled_paren(R"(\((\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)) << 1\)->f(\d+)\b)");
    t = sub_all(scaled_paren, t, [&](const RegexMatch& m) {   // (an expression in parentheses the index: itself)
        const std::int64_t n = to_int(m[2]);
        if (n < lo) return str(m.whole());
        const std::string name = hex_name(n);
        facts.arrays[name] = "int";
        return name + "[" + str(m[1].substr(1, m[1].size() - 2)) + "]";
    });
    // (a scratch register's value plus a place in the data: an array of bytes there, the value its index)
    const auto byte_index = [&](const RegexMatch& m) {
        const std::int64_t n = to_int(m[2]);
        if (n < 0x100) return str(m.whole());
        const std::string name = hex_name(n);
        facts.arrays[name] = byte_site(facts, "data", n) ? "char" : "int";
        return name + "[" + str(m[1]) + "]";
    };
    static const Regex scratch_reg(R"(\b(ax|bx|cx|dx)->f(\d+)\b)");
    t = sub_all(scratch_reg, t, byte_index);
    static const Regex indexed(R"(\bg_([0-9a-f]+)\[)");
    std::set<std::string> indexed_names;   // (an element of the data the model named: an array)
    indexed.for_each_match(t, [&](const RegexMatch& m) {
        indexed_names.insert(str(m[1]));
        return true;
    });
    for (const std::string& g : indexed_names) {
        const std::int64_t v = *ct::parse_int<std::int64_t>(g, 16);
        if (v >= 0x40)
            facts.arrays.try_emplace(hex_name(v), byte_site(facts, "data", v) || byte_site(facts, "*", v) ? "char" : "int");
    }
    static const Regex variable_place(R"(\(((?:\+\+|--)?[A-Za-z_]\w*(?:\+\+|--)?)\)->f(\d+)\b)");
    t = sub_all(variable_place, t, byte_index);   // (a variable's value - stepped too - the index)

    // (a register given K * E, then an element at a place in the data through it: an array of structs of K bytes -
    //  g_N[E].f0, the struct laid out in localize)
    static const Regex struct_index_re(R"((?m)^([ \t]*)bx = \((\d+) \* (.+?)\);\n[ \t]*(.*\bg_[0-9a-f]{4}\[bx\].*)$)");
    static const Regex bx_element(R"(\b(g_[0-9a-f]{4})\[bx\])");
    t = sub_all(struct_index_re, t, [&](const RegexMatch& m) {
        const std::int64_t k = to_int(m[2]);
        const std::string e = str(m[3]), rest = str(m[4]);
        const auto g = bx_element.search(rest);
        if (!g || k < 2 || k > 0x400) return str(m.whole());
        const std::string g1 = str((*g)[1]);
        const auto known = facts.arrays.find(g1);
        struct_array_of(facts, g1, static_cast<std::uint32_t>(k), known != facts.arrays.end() ? known->second : "int");
        std::string out = str(m[1]);
        const std::string needle = g1 + "[bx]";
        const std::size_t at = rest.find(needle);
        out += rest.substr(0, at) + g1 + "[" + e + "].f0" + rest.substr(at + needle.size());
        return out;
    });
    // (K * E + N, N a place in the data: the address of the element E of an array of K-byte structs)
    static const Regex struct_place(R"(\(\((\d+) \* ([\w>.-]+)\) \+ (\d+)\)(?!->))");
    t = sub_all(struct_place, t, [&](const RegexMatch& m) {
        const std::int64_t k = to_int(m[1]), n = to_int(m[3]);
        if (n < 0x100 || k < 3 || k > 0x400) return str(m.whole());
        const std::string name = hex_name(n);
        facts.struct_arrays.try_emplace(name, dt::StructArray{static_cast<std::uint32_t>(k), "int", {}});
        facts.arrays[name] = "struct s_" + name;
        return "&" + name + "[" + str(m[2]) + "]";
    });

    // (a register given K * E read only at places in the data through it until given again: the product there)
    static const Regex product_load(R"(([ \t]*)(bx|si|di) = \((\d+) \* ([A-Za-z_]\w*)\);)");
    static const Regex ends_run(R"(\s*(?:L_\w+:|\}|case\b|default:))");
    static const Regex g_in_bx(R"(\bg_([0-9a-f]{4})\[bx\])");
    Lines ls = split_nl(t);
    std::vector<char> gone(ls.size(), 0);
    for (std::size_t k = 0; k < ls.size(); ++k) {
        const auto m = product_load.full_match(ls[k]);
        if (!m || to_int((*m)[3]) < 2) continue;
        const std::string r = str((*m)[2]), e = str((*m)[4]), factor = str((*m)[3]);
        std::vector<std::size_t> uses;
        for (std::size_t j = k + 1; j < ls.size(); ++j) {
            const std::string& y = ls[j];
            if (starts_after_ws(y, r + " = ") || starts_after_ws(y, e + " = ") || ends_run.match(y)) break;
            bool stepped = false;   // `\bE\s*(?:\+\+|--)`
            for (std::size_t p = find_word(y, e); p != npos && !stepped; p = find_word(y, e, p + 1)) {
                const sv tail = ct::ltrim(sv(y).substr(p + e.size()));
                stepped = tail.starts_with("++") || tail.starts_with("--");
            }
            if (stepped) break;
            if (has_word(y, r)) uses.push_back(j);
        }
        if (r == "bx") {   // (an element of words through it: the place there)
            Lines rewritten;
            bool all_clear = true;
            for (const std::size_t u : uses) {
                rewritten.push_back(sub_all(g_in_bx, ls[u], [](const RegexMatch& q) {
                    return std::format("bx->f{}", *ct::parse_int<std::int64_t>(q[1], 16));
                }));
                if (bare_use(rewritten.back(), "bx")) all_clear = false;
            }
            if (all_clear)
                for (std::size_t i = 0; i < uses.size(); ++i) ls[uses[i]] = rewritten[i];
        }
        bool clear = !uses.empty();
        for (const std::size_t u : uses)
            if (bare_use(ls[u], r)) clear = false;
        if (clear)
            for (const std::size_t u : uses)
                for (const FieldRef& f : field_refs(ls[u], r))
                    if (to_int(f.digits) < 0x100) clear = false;
        if (!clear || dt::live_after(ls, uses.back() + 1, r)) continue;
        for (const std::size_t u : uses) {
            std::string out;
            std::size_t at = 0;
            for (const FieldRef& f : field_refs(ls[u], r)) {
                out.append(ls[u], at, f.pos - at);
                out += "(" + factor + " * " + e + ")->f" + f.digits;
                at = f.pos + f.len;
            }
            out.append(ls[u], at, npos);
            ls[u] = out;
        }
        gone[k] = 1;
    }
    t = join_live(ls, gone);

    // (the product folded in place: (K * E)->fN)
    static const Regex struct_product(R"(\((\d+) \* ([A-Za-z_][\w>-]*)\)->f(\d+)\b)");
    t = sub_all(struct_product, t, [&](const RegexMatch& m) {
        const std::int64_t k = to_int(m[1]), n = to_int(m[3]);
        if (n < 0x100 || k < 2 || k > 0x400) return str(m.whole());
        const std::string name = hex_name(n);
        facts.struct_arrays.try_emplace(name, dt::StructArray{static_cast<std::uint32_t>(k), byte_site(facts, "data", n) ? "char" : "int", {}});
        facts.arrays[name] = "struct s_" + name;
        return name + "[" + str(m[2]) + "].f0";
    });
    // (the compiler keeps bx on: later elements through it, until it is given again - and the product itself read as
    //  a pointer to a field, (K * E)->fN, an element at N)
    static const Regex element_of(R"(\b(g_[0-9a-f]{4})\[(.+?)\]\.f0)");
    static const Regex bx_given(R"([ \t]*bx = )");
    Lines lines = split_nl(t);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        if (lines[k].find("].f0") == npos) continue;
        const auto prod = element_of.search(lines[k]);
        if (!prod) continue;
        const auto known = facts.struct_arrays.find(str((*prod)[1]));
        if (known == facts.struct_arrays.end()) continue;
        const std::uint32_t kk = known->second.size;
        const std::string e = str((*prod)[2]);
        const auto elem = [&](const std::string& g) {
            const auto a = facts.arrays.find(g);
            const std::string type = a != facts.arrays.end() && !a->second.starts_with("struct") ? a->second : "int";
            facts.struct_arrays.try_emplace(g, dt::StructArray{kk, type, {}});
            facts.arrays[g] = "struct s_" + g;
            return g + "[" + e + "].f0";
        };
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            if (bx_given.match(lines[j]) || given_or_stepped(lines[j], e)) break;
            lines[j] = sub_all(bx_element, lines[j], [&](const RegexMatch& m2) { return elem(str(m2[1])); });
            // `\(KK \* E\)->f(\d+)\b`
            const std::string lead = std::format("({} * {})->f", kk, e);
            std::string out;
            std::size_t at = 0;
            for (std::size_t p = lines[j].find(lead); p != npos; p = lines[j].find(lead, at)) {
                std::size_t d = p + lead.size();
                const std::size_t d0 = d;
                while (d < lines[j].size() && is_digit(lines[j][d])) ++d;
                if (d == d0 || (d < lines[j].size() && is_word(lines[j][d]))) {
                    out.append(lines[j], at, p + 1 - at);
                    at = p + 1;
                    continue;
                }
                out.append(lines[j], at, p - at);
                out += elem(hex_name(to_int(lines[j].substr(d0, d - d0))));
                at = d;
            }
            out.append(lines[j], at, npos);
            lines[j] = out;
        }
    }
    t = join_nl(lines);
    // (I << 1)[E]: the model's word index into the words at E - ((int *)(E))[I]
    static const Regex word_index(
        R"(\(((?:[A-Za-z_][\w>-]*|\d+|\((?:[^()]|\([^()]*\))*\))(?:\s*[-+]\s*\d+)?) << 1\)\[([^\[\]]+)\])");
    static const Regex base_plus_offset(R"(([A-Za-z_]\w*) \+ (\d+))");
    return sub_all(word_index, t, [&](const RegexMatch& m) {   // (an offset in bytes: through a char *)
        const std::string i = str(m[1]), e = str(m[2]);
        if (const auto o = base_plus_offset.full_match(e))
            return "((int *)((char *)" + str((*o)[1]) + " + " + str((*o)[2]) + "))[" + i + "]";
        return "((int *)(" + e + "))[" + i + "]";
    });
}

// (i << 1) + K, K a place in the data: the address of a word there, &g_K[i] (a variant)
std::optional<std::string> scaled_places(sv text, FunctionFacts& facts) {
    static const Regex place(R"(\(\(([\w>.-]+) << 1\) \+ (-?\d+)\))");
    std::string t = sub_all(place, text, [&](const RegexMatch& m) {
        const std::int64_t k = to_int(m[2]) & 0xFFFF;
        if (k < 0x100) return str(m.whole());
        const std::string name = hex_name(k);
        facts.arrays.try_emplace(name, "int");
        return "&" + name + "[" + str(m[1]) + "]";
    });
    static const Regex element(R"(&(g_[0-9a-f]{4})\[)");
    static const Regex function_head(R"((?m)^\w+\(.*\)$)");
    std::set<std::string> names;
    element.for_each_match(t, [&](const RegexMatch& m) {
        names.insert(str(m[1]));
        return true;
    });
    for (const std::string& g : names) {   // (declared, when the text is a whole source)
        bool declared = false;   // `^(?:extern|struct)[^\n]*\bg\b`
        for (const sv line : ct::split(t, '\n'))
            if ((line.starts_with("extern") || line.starts_with("struct")) && has_word(line, g)) declared = true;
        if (function_head.search(t) && !declared) t = "extern int " + g + "[];\n" + t;
    }
    if (t == text) return std::nullopt;
    return t;
}

// bx = p + (i << 1); .. *bx - a word of p's at i: p[i], p an int's pointer (the scaled index the model left as an
// address; a copy of the doubled index the code made on the way, `ax = (i << 1);`, nothing)
std::string scaled_index(sv text) {
    static const Regex load(R"(([ \t]*)bx = ([A-Za-z_]\w*) \+ \((\w+|\((?:[^()]|\([^()]*\))+\)) << 1\);$)");
    static const Regex deref(R"((?<![\w)])\*bx\b(?!\+\+|--))");
    Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = load.match(lines[k]);
        if (!m) continue;
        const std::string p = str((*m)[2]);
        std::string i = str((*m)[3]);
        if (i.starts_with('(') && dt::balanced(sv(i).substr(1, i.size() - 2))) i = i.substr(1, i.size() - 2);   // (an expression the index)
        std::size_t j = k + 1;
        // (a copy of the doubled index the code made on the way, `ax = (i << 1);`, nothing)
        const std::string doubled = "ax = (" + i + " << 1);";
        while (j < lines.size() && ct::ltrim(sv(lines[j])) == doubled && indent_of(lines[j]) == lines[j].find_first_not_of(" \t") &&
               !dt::live_after(lines, j + 1, "ax"))
            ++j;
        if (j >= lines.size()) continue;
        std::size_t derefs = 0;
        deref.for_each_match(lines[j], [&](const RegexMatch&) {
            ++derefs;
            return true;
        });
        if (derefs != 1 || count_word(lines[j], "bx") != 1 || dt::live_after(lines, j + 1, "bx")) continue;
        bool assigned = false;   // (`\bv\s*=[^=]` for a name v of the index)
        for (std::size_t s = 0; s < i.size() && !assigned;) {
            if (!(std::isalpha(static_cast<unsigned char>(i[s])) || i[s] == '_')) {
                ++s;
                continue;
            }
            std::size_t e = s;
            while (e < i.size() && is_word(i[e])) ++e;
            const std::string v = i.substr(s, e - s);
            for (std::size_t q = lines[j].find(v); q != npos && !assigned; q = lines[j].find(v, q + 1)) {
                if (!boundary_at(lines[j], q)) continue;
                std::size_t a = q + v.size();
                while (a < lines[j].size() && ct::is_space(lines[j][a])) ++a;
                assigned = a + 1 < lines[j].size() && lines[j][a] == '=' && lines[j][a + 1] != '=';
            }
            s = e;
        }
        if (assigned) continue;
        lines[j] = sub_all(deref, lines[j], [&](const RegexMatch&) { return p + "[" + i + "]"; });
        std::vector<char> gone(lines.size(), 0);
        for (std::size_t g = k; g < j; ++g) gone[g] = 1;
        const std::string t = join_live(lines, gone);
        return dt::declare_pointer(t, p, "int").value_or(t);
    }
    return str(text);
}

// (*x)(..) - x called through: a pointer to a function, int (*x)(); (a parameter's or a local's int)
std::string fn_pointers(sv text_in) {
    static const Regex called(R"(\(\*([A-Za-z_]\w*)\)\s*\()");
    std::set<std::string> xs;
    called.for_each_match(text_in, [&](const RegexMatch& m) {
        xs.insert(str(m[1]));
        return true;
    });
    std::string text = str(text_in);
    for (const std::string& x : xs) {
        bool declared = false;   // `^[ \t]*(?:register\s+)?int (?:\w+, )*x\b`
        for (const sv line : ct::split(text, '\n')) {
            std::size_t p = 0;
            while (p < line.size() && (line[p] == ' ' || line[p] == '\t')) ++p;
            if (line.substr(p).starts_with("register") && p + 8 < line.size() && ct::is_space(line[p + 8])) {
                p += 8;
                while (p < line.size() && ct::is_space(line[p])) ++p;
            }
            if (!line.substr(p).starts_with("int ")) continue;
            std::size_t i = p + 4;
            while (true) {
                if (line.substr(i).starts_with(x) && boundary_at(line, i + x.size())) {
                    declared = true;
                    break;
                }
                const std::size_t b = i;
                while (i < line.size() && is_word(line[i])) ++i;
                if (i == b || !line.substr(i).starts_with(", ")) break;
                i += 2;
            }
            if (declared) break;
        }
        if (declared)
            if (auto t = redeclare(text, x, "int (*" + x + ")()")) text = *t;
    }
    return text;
}

// ---- the library: headers, tables, data and macros --------------------------------------------------------------

namespace {

namespace fsys = std::filesystem;

// The files called `header` (any case) in each directory of the kit, in the order a top-down walk finds them:
// a directory's own files before its subdirectories, both in name order. One list per directory that has any.
void walk_headers(const fsys::path& dir, const std::string& lower_name, std::vector<std::vector<fsys::path>>& out) {
    std::error_code ec;
    std::vector<fsys::path> files, dirs;
    for (fsys::directory_iterator it(dir, ec), end; !ec && it != end; it.increment(ec)) {
        std::error_code type_ec;
        if (it->is_directory(type_ec)) dirs.push_back(it->path());
        else files.push_back(it->path());
    }
    std::ranges::sort(files);
    std::ranges::sort(dirs);
    std::vector<fsys::path> here;
    for (const fsys::path& f : files)
        if (ct::lower(f.filename().string()) == lower_name) here.push_back(f);
    if (!here.empty()) out.push_back(std::move(here));
    for (const fsys::path& d : dirs) walk_headers(d, lower_name, out);
}

std::vector<std::vector<fsys::path>> headers_in_kit(const KitContext& kit, std::string_view header) {
    std::vector<std::vector<fsys::path>> found;
    if (kit.kit) walk_headers(*kit.kit, ct::lower(header), found);
    return found;
}

std::string read_header(const fsys::path& path) { return ct::from_latin1(fs::read_file(path)); }

// A format with `%s` for each of `args` and `%%` for a percent sign.
std::string apply_template(sv format, const std::vector<std::string>& args) {
    std::string out;
    std::size_t used = 0;
    for (std::size_t i = 0; i < format.size(); ++i) {
        if (format[i] != '%') {
            out += format[i];
            continue;
        }
        ++i;
        if (i < format.size() && format[i] == '%') {
            out += '%';
        } else if (i < format.size() && format[i] == 's') {
            if (used >= args.size()) throw UsageError(std::format("macro format \"{}\" takes more arguments than the {} given", format, args.size()));
            out += args[used++];
        } else {
            throw UsageError(std::format("macro format \"{}\" has a conversion other than %s", format));
        }
    }
    if (used != args.size()) throw UsageError(std::format("macro format \"{}\" takes {} arguments, {} given", format, used, args.size()));
    return out;
}

std::optional<std::int64_t> parse_c_number(sv s) {
    if (s.starts_with("0x") || s.starts_with("0X")) return ct::parse_int<std::int64_t>(s.substr(2), 16);
    return ct::parse_int<std::int64_t>(s, 10);
}

const std::map<std::string, std::uint32_t>& library_places(const KitContext& kit, ProjectCaches& caches) {
    auto it = caches.library_data.find(kit.project);
    if (it == caches.library_data.end())
        it = caches.library_data.emplace(kit.project, kit.library_data ? kit.library_data() : std::map<std::string, std::uint32_t>{}).first;
    return it->second;
}

}  // namespace

// a header of the kit, as text ('' when the kit has none)
std::string header_text(const KitContext& kit, sv header) {
    const auto found = headers_in_kit(kit, header);
    return found.empty() ? std::string() : read_header(found.front().front());
}

// the functions and macros a header of the kit declares (stdio.h's fopen, freopen, getc ..): those come from the
// header, the draft does not declare them again
std::set<std::string> header_names(const KitContext& kit, ProjectCaches& caches, sv header) {
    const auto key = std::make_pair(kit.toolchain, ct::lower(header));
    if (const auto it = caches.headers.find(key); it != caches.headers.end()) return it->second;
    static const Regex declared(R"(\b([A-Za-z_]\w*)\s*\(\s*\)\s*[;,])");
    static const Regex defined(R"(#define\s+([A-Za-z_]\w*)\s*\()");
    std::set<std::string> names;
    for (const auto& in_dir : headers_in_kit(kit, header)) {   // (the first file of a directory; on, while it names nothing)
        const std::string t = read_header(in_dir.front());
        for (const Regex* re : {&declared, &defined})
            re->for_each_match(t, [&](const RegexMatch& m) {
                names.insert(str(m[1]));
                return true;
            });
        if (!names.empty()) break;
    }
    caches.headers[key] = names;
    return names;
}

namespace {

// `LEAD word TAIL` replaced by `fmt` applied to the word: ((c->f9 & 4) != 0) with lead "((", tail "->f9 & 4) != 0)".
std::string replace_masked_reads(sv text, sv lead, sv tail, const std::string& name, bool negate) {
    std::string out;
    std::size_t at = 0, i = 0;
    while ((i = text.find(lead, i)) != npos) {
        std::size_t j = i + lead.size();
        const std::size_t w0 = j;
        while (j < text.size() && is_word(text[j])) ++j;
        if (j > w0 && text.substr(j).starts_with(tail)) {
            out.append(text.substr(at, i - at));
            out += (negate ? "!" : "") + name + "(" + str(text.substr(w0, j - w0)) + ")";
            at = i = j + tail.size();
        } else {
            ++i;
        }
    }
    out.append(text.substr(at));
    return out;
}

}  // namespace

// the macros of a header that read a table of the library - ctype.h's isdigit(c) .. ( (_ctype_+1)[c] & _DIGIT ) - read
// back where the model has the table's place: (c->fN & M), N the table's place + its offset, M a macro's mask (the
// profile's "table_macros": [{"header": "ctype.h"}]: the macros and masks are the kit's header's, the table's place
// the library stage's)
std::string table_macros(const KitContext& kit, ProjectCaches& caches, FunctionFacts& facts, std::string text,
                         std::span<const TableMacroSpec> specs) {
    static const Regex mask_def(R"(#define\s+(_[A-Z]+)\s+(0x[0-9a-fA-F]+|\d+))");
    static const Regex macro_def(R"(#define\s+(\w+)\((\w)\)\s*\(\s*\((\w+)\+(\d+)\)\[\2\]\s*&\s*([^)]*\)?)\s*\))");
    static const Regex bit(R"(_[A-Z]+)");
    for (const TableMacroSpec& spec : specs) {
        const auto key = std::make_pair(kit.project, spec.header);
        auto cached = caches.tables.find(key);
        if (cached == caches.tables.end()) {
            const std::string h = header_text(kit, spec.header);
            std::map<std::string, std::int64_t> masks;
            mask_def.for_each_match(h, [&](const RegexMatch& m) {
                masks[str(m[1])] = *parse_c_number(m[2]);
                return true;
            });
            std::vector<dt::TableMacro> macs;
            macro_def.for_each_match(h, [&](const RegexMatch& m) {
                const std::vector<std::string> bits = bit.find_all(m[5]);
                bool known = !bits.empty();
                std::int64_t mask = 0;
                for (const std::string& b : bits) {
                    const auto it = masks.find(b);
                    if (it == masks.end()) known = false;
                    else mask |= it->second;
                }
                if (known) macs.push_back({str(m[1]), str(m[3]), to_int(m[4]), mask});
                return true;
            });
            library_places(kit, caches);   // (the library's places are read once for the project, here or in data_macros)
            cached = caches.tables.emplace(key, std::move(macs)).first;
        }
        bool hit = false;
        const auto places = caches.library_data.find(kit.project);
        for (const dt::TableMacro& tm : cached->second) {
            if (places == caches.library_data.end()) continue;
            const auto find = [&](const std::string& name) -> std::optional<std::uint32_t> {
                const auto it = places->second.find(name);
                return it == places->second.end() ? std::nullopt : std::optional<std::uint32_t>(it->second);
            };
            const auto underscored = find("_" + tm.table);   // (`a or b`: a zero place is none)
            const std::optional<std::uint32_t> base = underscored && *underscored != 0 ? underscored : find(tm.table);
            if (!base) continue;
            const std::int64_t n = static_cast<std::int64_t>(*base) + tm.offset;
            const std::string read = std::format("->f{} & {})", n, tm.mask);
            struct Form {
                sv lead, tail;
                bool negate;
            };
            for (const Form& f : {Form{"((", " != 0)", false}, Form{"((", " == 0)", true}, Form{"(", "", false}}) {
                const std::string next = replace_masked_reads(text, f.lead, read + std::string(f.tail), tm.macro, f.negate);
                // (the closing parenthesis of the pair is in `read`; the first two forms close the outer one too)
                if (next != text) {
                    hit = true;
                    text = next;
                }
            }
        }
        if (hit) {
            if (!facts.include) facts.include = spec.header;
            facts.include2 = spec.header;
        }
    }
    return text;
}

// a library's data a macro names - stdio's streams: stdin, stdout, stderr .. the elements of _iob, fileno(f) a byte of
// one - read back where the model has their places (g_XXXX, or a number passed: a stream's place) (the profile's
// "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"}, "include": "stdio.h"}];
// the base's place from the library stage)
std::string data_macros(const KitContext& kit, ProjectCaches& caches, FunctionFacts& facts, std::string text,
                        std::span<const DataMacroSpec> specs) {
    if (specs.empty()) return text;
    static const Regex place(R"(\bg_([0-9a-f]{4})\b)");
    static const Regex passed(R"(([(,]\s*)(\d{3,5})(?=\s*[,)]))");
    const std::map<std::string, std::uint32_t>& places = library_places(kit, caches);
    for (const DataMacroSpec& spec : specs) {
        const auto found = places.find(spec.base);
        if (found == places.end()) continue;
        const std::int64_t base = found->second;
        const std::int64_t n = static_cast<std::int64_t>(spec.names.size());
        bool hit = false;
        const auto element_of = [&](std::int64_t address, std::int64_t& index, std::int64_t& offset) {
            if (spec.size == 0) throw UsageError(std::format("data macro \"{}\" has elements of size 0", spec.base));
            const std::int64_t d = address - base;   // (below the base: no element)
            if (d < 0) return false;
            index = d / spec.size;
            offset = d % spec.size;
            return index < n;
        };
        text = sub_all(place, text, [&](const RegexMatch& m) {
            std::int64_t k = 0, off = 0;
            if (!element_of(*ct::parse_int<std::int64_t>(m[1], 16), k, off)) return str(m.whole());
            const auto field = spec.fields.find(static_cast<std::uint32_t>(off));
            if (field == spec.fields.end()) return str(m.whole());
            hit = true;
            return apply_template(field->second, {spec.names[static_cast<std::size_t>(k)]});
        });
        text = sub_all(passed, text, [&](const RegexMatch& m) {   // (a number passed that is a stream's place)
            std::int64_t k = 0, off = 0;
            if (!element_of(to_int(m[2]), k, off) || off != 0) return str(m.whole());
            hit = true;
            return str(m[1]) + spec.names[static_cast<std::size_t>(k)];
        });
        if (hit && spec.include && !spec.include->empty()) facts.include = spec.include;
    }
    return text;
}

// the locals declared in the function's body that nothing in it names any more, left out
std::string unused_locals(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    static const Regex decl(
        R"(\s*(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)\s*\**\s*(\w+)\s*(?:\[\d+\])?;\s*$)");
    const Lines lines = split_nl(split.body);
    std::vector<char> gone(lines.size(), 0);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = decl.match(lines[k]);
        if (!m) continue;
        const std::string name = str((*m)[1]);   // (named nowhere else in the body)
        if (count_word(split.body, name) == count_word(lines[k], name)) gone[k] = 1;
    }
    return split.head + split.brace + join_live(lines, gone);
}

namespace {

// `\bCALL\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\)`: the arguments of a call to `call` on the line, and where the call
// begins and ends. The arguments hold parentheses nested to two levels.
struct CallAt {
    std::size_t begin = 0, end = 0;
    std::string args;
};

std::optional<std::size_t> skip_group(sv s, std::size_t open, int depth_left) {   // after the ')' that closes s[open]
    std::size_t i = open + 1;
    while (i < s.size()) {
        if (s[i] == ')') return i + 1;
        if (s[i] == '(') {
            if (depth_left == 0) return std::nullopt;
            const auto inner = skip_group(s, i, depth_left - 1);
            if (!inner) return std::nullopt;
            i = *inner;
        } else {
            ++i;
        }
    }
    return std::nullopt;
}

std::optional<CallAt> find_call(sv line, sv call) {
    const std::string lead = str(call) + "(";
    for (std::size_t p = line.find(lead); p != npos; p = line.find(lead, p + 1)) {
        if (!boundary_at(line, p)) continue;
        const std::size_t open = p + call.size();
        if (const auto end = skip_group(line, open, 2)) return CallAt{p, *end, str(line.substr(open + 1, *end - open - 2))};
    }
    return std::nullopt;
}

// `\bF->f2(?:--| = F->f2 - 1)`: the stream F's count stepped.
bool steps_stream_count(sv line, sv f) {
    const std::string count = str(f) + "->f2";
    for (std::size_t p = line.find(count); p != npos; p = line.find(count, p + 1)) {
        if (!boundary_at(line, p)) continue;
        const sv tail = line.substr(p + count.size());
        if (tail.starts_with("--") || tail.starts_with(" = " + count + " - 1")) return true;
    }
    return false;
}

// `^[ \t]*(?:register )?int \*P;$` on some line of the text.
bool declares_int_pointer(sv text, sv p) {
    const std::string tail = "int *" + str(p) + ";";
    for (const sv line : ct::split(text, '\n')) {
        std::size_t i = 0;
        while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
        sv rest = line.substr(i);
        if (rest == tail) return true;
        if (rest.starts_with("register ") && rest.substr(9) == tail) return true;
    }
    return false;
}


}  // namespace

// a library macro the compiler expanded in line - stdio's getc(f) and putc(c, f): the count of the stream's buffer
// stepped, then its pointer, else the routine that fills or empties the buffer (the profile's "macros": [{"call":
// routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h"}]) - read back as the macro: from the line that
// steps the stream's count to the routine's call, the region is the macro's value
std::string library_macros(std::string text, std::span<const LibraryMacroSpec> macros, FunctionFacts& facts) {
    static const Regex returns(R"(\s*return\b)");
    for (const LibraryMacroSpec& mac : macros) {
        while (true) {
            Lines lines = split_nl(text);
            std::optional<std::size_t> hit_line;
            std::optional<CallAt> hit;
            for (std::size_t k = 0; k < lines.size() && !hit; ++k)
                if ((hit = find_call(lines[k], mac.call))) hit_line = k;
            if (!hit) break;
            const std::size_t k1 = *hit_line;
            std::vector<std::string> args;
            for (const std::string& a : dt::top_commas(hit->args)) args.push_back(str(ct::trim(a)));
            const std::string f = args.back();
            std::optional<std::size_t> k0;
            for (std::size_t j = k1 + 1; j-- > 0;)
                if (steps_stream_count(lines[j], f)) {
                    k0 = j;
                    break;
                }
            if (!k0) break;
            for (const std::size_t ix : mac.byte_args) {   // (an argument the macro stores as a byte - putc's c: *p of an int *p read a byte at a time)
                if (ix >= args.size() || args[ix].size() < 2 || args[ix][0] != '*') continue;
                const std::string p = args[ix].substr(1);
                if (std::ranges::all_of(p, is_word) && declares_int_pointer(text, p)) args[ix] = "*(char *)" + p;
            }
            const std::string val = apply_template(mac.as, args);
            std::size_t k2 = k1;
            std::int64_t depth = 0;
            for (std::size_t j = *k0; j <= k2; ++j) depth += brace_balance(lines[j]);
            while (depth > 0 && k2 + 1 < lines.size()) {   // (to the region's own closing brace)
                ++k2;
                depth += brace_balance(lines[k2]);
            }
            const std::string ind = lead_ws(lines[*k0]);
            Lines made{returns.match(lines[k1]) ? ind + "return " + val + ";" : ind + "ax = " + val + ";"};
            if (depth < 0) made.push_back(ind + "}");
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(*k0), lines.begin() + static_cast<std::ptrdiff_t>(k2) + 1);
            lines.insert(lines.begin() + static_cast<std::ptrdiff_t>(*k0), made.begin(), made.end());
            text = join_nl(lines);
            facts.streams.insert_or_assign(f, mac.type);
            if (mac.include) facts.include = mac.include;   // (a macro without a header leaves the one an earlier gave)
        }
    }
    return text;
}

// ---- tests and stores -------------------------------------------------------------------------------------------

namespace {

// `(?<![\w>.])N(?![\w(])`: N as a plain value (not a field, a member or a call).
bool plain_use_at(sv s, std::size_t p, sv n) {
    if (p > 0 && (is_word(s[p - 1]) || s[p - 1] == '>' || s[p - 1] == '.')) return false;
    const std::size_t e = p + n.size();
    return e >= s.size() || !(is_word(s[e]) || s[e] == '(');
}
std::size_t count_plain_uses(sv s, sv n) {
    std::size_t count = 0;
    for (std::size_t p = s.find(n); p != npos; p = s.find(n, p + 1))
        if (plain_use_at(s, p, n)) ++count;
    return count;
}
bool has_plain_use(sv s, sv n) { return count_plain_uses(s, n) > 0; }
std::string replace_first_plain_use(sv s, sv n, sv with) {
    for (std::size_t p = s.find(n); p != npos; p = s.find(n, p + 1))
        if (plain_use_at(s, p, n)) return str(s.substr(0, p)) + str(with) + str(s.substr(p + n.size()));
    return str(s);
}

// `(?<![\w>.*])N\s*(?:=[^=]|\+\+|--)`: N given a value or stepped (a `*` before it blocks the match when star_blocks).
bool assigned_or_stepped(sv s, sv n, bool star_blocks) {
    for (std::size_t p = s.find(n); p != npos; p = s.find(n, p + 1)) {
        if (p > 0 && (is_word(s[p - 1]) || s[p - 1] == '>' || s[p - 1] == '.' || (star_blocks && s[p - 1] == '*'))) continue;
        std::size_t q = p + n.size();
        while (q < s.size() && ct::is_space(s[q])) ++q;
        if (q + 1 < s.size() && s[q] == '=' && s[q + 1] != '=') return true;
        if (s.substr(q).starts_with("++") || s.substr(q).starts_with("--")) return true;
    }
    return false;
}

// `E\s*=[^=]`: E, with no boundary, then a plain assignment.
bool assigned_literal(sv s, sv e) {
    for (std::size_t p = s.find(e); p != npos; p = s.find(e, p + 1)) {
        std::size_t q = p + e.size();
        while (q < s.size() && ct::is_space(s[q])) ++q;
        if (q + 1 < s.size() && s[q] == '=' && s[q + 1] != '=') return true;
    }
    return false;
}

bool starts_blank(sv line, sv w) {   // `^[ \t]*W`
    std::size_t i = 0;
    while (i < line.size() && (line[i] == ' ' || line[i] == '\t')) ++i;
    return line.substr(i).starts_with(w);
}

bool has_call_paren(sv s) {   // `\w\(`
    for (std::size_t i = 0; i + 1 < s.size(); ++i)
        if (is_word(s[i]) && s[i + 1] == '(') return true;
    return false;
}

// `\s*register`
bool starts_with_register(sv line) {
    const sv l = ct::ltrim(line);
    return l.starts_with("register") && (l.size() == 8 || !is_word(l[8]));
}


// `if (` at the start of the line (after whitespace), then the text x, then a boundary; `while (` too when asked.
std::optional<CondStart> if_starts_with(sv line, sv x) {
    const auto at = cond_starts_with(line, x);
    if (!at) return std::nullopt;
    const std::size_t i = indent_of(line);
    if (!line.substr(i).starts_with("if (")) return std::nullopt;
    return at;
}

}  // namespace

// if (A) return E; return 0; -> return A && E; (a variant: the compiler makes one test chain of the &&)
std::optional<std::string> and_returns(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)if (\(.*\))(?: \{)?\n\s*return (.+);\n(?:\s*\}\n)?\s*return 0;$)");
    const std::string t = sub_all(rx, text, [](const RegexMatch& m) {
        std::string e = str(m[3]);
        if (e.find("||") != npos && !(e.starts_with('(') && e.ends_with(')'))) e = "(" + e + ")";
        return str(m[1]) + "return " + dt::truth(m[2]) + " && " + e + ";";
    });
    if (t == text) return std::nullopt;
    return t;
}

// a word the model reads as 32768..65535: a decimal constant over 32767 is a long to a 16-bit compiler (the compare or
// the sum goes to 32 bits) - the int it was, negative (65535 is -1)
std::string word_constants(sv text) {   // `(?<![\w.])(32768..65535)(?![\w.])`
    std::string out;
    std::size_t at = 0;
    for (std::size_t i = 0; i < text.size();) {
        if (!is_digit(text[i])) {
            ++i;
            continue;
        }
        std::size_t e = i;
        while (e < text.size() && is_digit(text[e])) ++e;
        const bool before_ok = i == 0 || !(is_word(text[i - 1]) || text[i - 1] == '.');
        const bool after_ok = e >= text.size() || !(is_word(text[e]) || text[e] == '.');
        if (before_ok && after_ok && e - i == 5) {
            const std::int64_t v = to_int(text.substr(i, 5));
            if (v >= 32768 && v <= 65535) {
                out.append(text.substr(at, i - at));
                out += std::to_string(v - 65536);
                at = e;
            }
        }
        i = e;
    }
    out.append(text.substr(at));
    return out;
}

// X = E; then a test of X: the assignment in the test - if ((X = E) == K) - a variant: the compiler compares the value
// it has, not X again
std::optional<std::string> assign_in_tests(sv text) {
    static const Regex assignment(R"(\s*([A-Za-z_]\w*(?:\[[^\]]*\])?) = ([^;]+);\s*$)");
    static const Regex statement(R"(\s*(?:if|while|return)\b)");
    Lines lines = split_nl(text);
    std::vector<char> gone(lines.size(), 0);
    bool changed = false;
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        const auto m = assignment.match(lines[k]);
        if (!m || statement.match(lines[k])) continue;
        const std::string x = str((*m)[1]), e = str((*m)[2]);
        // (an if's: a loop's test would give it again each time)
        const auto t = if_starts_with(lines[k + 1], x);
        if (t && find_bounded(e, x) == npos) {
            const std::string& l = lines[k + 1];
            lines[k + 1] = l.substr(0, t->x_at) + "(" + x + " = " + e + ")" + l.substr(t->rest_at);
            gone[k] = 1;
            changed = true;
        }
    }
    if (!changed) return std::nullopt;
    return join_live(lines, gone);
}

// a parameter the model only reads into a byte register (al = b) is a char: declared so (K&R widens it in the call;
// the function reads its low byte)
std::string char_params(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return str(text);
    static const Regex params_re(R"(\b\w+\(([\w, ]*)\))");
    std::optional<std::string> last;
    params_re.for_each_match(split.head, [&](const RegexMatch& m) {
        last = str(m[1]);
        return true;
    });
    std::vector<std::string> params;
    if (last)
        for (const sv p : ct::split(*last, ','))
            if (!blank(p)) params.push_back(str(ct::trim(p)));
    const auto assigned_from = [&](std::initializer_list<sv> regs, const std::string& p) {   // `\b(?:regs) = p;`
        for (const sv r : regs) {
            const std::string needle = str(r) + " = " + p + ";";
            for (std::size_t at = split.body.find(needle); at != npos; at = split.body.find(needle, at + 1))
                if (boundary_at(split.body, at)) return true;
        }
        return false;
    };
    const auto dereferenced = [&](const std::string& p) {   // `\bp\s*(?:->|\[)`
        for (std::size_t at = split.body.find(p); at != npos; at = split.body.find(p, at + 1)) {
            if (!boundary_at(split.body, at)) continue;
            const sv tail = ct::ltrim(sv(split.body).substr(at + p.size()));
            if (tail.starts_with("->") || tail.starts_with("[")) return true;
        }
        return false;
    };
    std::vector<std::string> chars;
    for (const std::string& p : params)
        if (assigned_from({"al", "bl", "cl", "dl"}, p) && !assigned_from({"ax", "bx", "cx", "dx", "si", "di"}, p) && !dereferenced(p))
            chars.push_back(p);
    if (chars.empty()) return str(text);
    static const Regex int_decl(R"((\s*)int (.*);$)");
    Lines lines = split_nl(split.head);
    for (std::string& l : lines) {
        const auto d = int_decl.match(l);
        if (!d) continue;
        std::vector<std::string> names, keep, mine;
        for (const sv n : ct::split((*d)[2], ',')) names.push_back(str(ct::trim(n)));
        for (const std::string& n : names) (std::ranges::find(chars, n) == chars.end() ? keep : mine).push_back(n);
        if (mine.empty()) continue;
        std::string out;
        if (!keep.empty()) out += str((*d)[1]) + "int " + ct::join(keep, ", ") + ";\n";
        l = out + str((*d)[1]) + "char " + ct::join(mine, ", ") + ";";
    }
    return join_nl(lines) + split.brace + split.body;
}

// ((A op B) ? 1 : 0) - a comparison's value: (A op B) itself, ? 0 : 1 its negation, and a call's side on the right (B
// mirror-op A) when the call was A (a variant: the compiler calls first whichever side it is on)
std::optional<std::string> compare_results(sv text) {
    static const Regex cmp(R"(\(\(([^?:\n]+?) (==|!=|<=|>=|<|>) ([^?:\n]+?)\) \? ([01]) : ([01])\))");
    const std::string t = sub_all(cmp, text, [](const RegexMatch& m) {
        std::string a = str(m[1]), b = str(m[3]);
        std::string op = str(m[2]);
        if (!(dt::balanced(a) && dt::balanced(b)) || m[4] == m[5]) return str(m.whole());
        static const std::map<std::string, std::string> mirror = {{"<", ">"}, {">", "<"}, {"<=", ">="}, {">=", "<="}, {"==", "=="}, {"!=", "!="}};
        static const std::map<std::string, std::string> neg = {{"<", ">="}, {">=", "<"}, {">", "<="}, {"<=", ">"}, {"==", "!="}, {"!=", "=="}};
        if (m[4] == "0") op = neg.at(op);
        if (has_call_paren(a) && !has_call_paren(b)) {
            std::swap(a, b);
            op = mirror.at(op);
        }
        return "(" + a + " " + op + " " + b + ")";
    });
    if (t == text) return std::nullopt;
    return t;
}

// return (C ? A : B); - as the two returns it may have been: if (C) return A; return B; (a variant: the compiler keeps
// one copy of a tail both share either way)
std::optional<std::string> split_ternaries(sv text) {
    static const Regex ret(R"((?m)^([ \t]*)return \((.*)\);$)");
    const std::string t = sub_all(ret, text, [](const RegexMatch& m) {
        const std::string ind = str(m[1]);
        const sv inner = m[2];
        std::int64_t depth = 0;
        std::optional<std::size_t> q;
        for (std::size_t i = 0; i < inner.size(); ++i) {   // (the top-level ? and :)
            const char ch = inner[i];
            depth += (ch == '(') - (ch == ')');
            if (ch == '?' && depth == 0 && !q) q = i;
            if (ch == ':' && depth == 0 && q) {
                const std::string c = str(ct::trim(inner.substr(0, *q)));
                const std::string a = str(ct::trim(inner.substr(*q + 1, i - *q - 1)));
                const std::string b = str(ct::trim(inner.substr(i + 1)));
                return ind + "if (" + (c.starts_with('(') ? c : "(" + c + ")") + ") {\n" + ind + "    return " + a + ";\n" + ind + "}\n" +
                       ind + "return " + b + ";";
            }
        }
        return str(m.whole());
    });
    if (t == text) return std::nullopt;
    return t;
}

// M = A; .. r = M; - a register the model reloads from the place a plain value was just stored in (its store-
// forwarding, written out before the pointer moves): the value itself, r = A
std::string store_reloads(sv text) {
    static const Regex reload(R"(([ \t]*)(ax|dx|bx|cx) = (\*\w+|\w+->f\d+);)");
    static const Regex store(R"([ \t]*(.+?) = ([A-Za-z_]\w*|-?\d+);)");
    static const Regex base_re(R"(\*?(\w+))");
    Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = reload.full_match(lines[k]);
        if (!m) continue;
        const std::string place = str((*m)[3]);
        const std::string base = str((*base_re.match(place))[1]);
        for (std::size_t j = k; j-- > 0 && k - j <= 5;) {   // (the store just before, nothing between it and here giving its value or base another)
            const auto s = store.full_match(lines[j]);
            if (!s) break;
            if ((*s)[1] == place) {
                const std::string value = str((*s)[2]);
                bool clobbered = false;
                for (std::size_t q = j + 1; q < k; ++q)
                    if (starts_blank(lines[q], value + " = ") || starts_blank(lines[q], base + " = ")) clobbered = true;
                if (value != base && !clobbered) lines[k] = str((*m)[1]) + str((*m)[2]) + " = " + value + ";";
                break;
            }
        }
    }
    return join_nl(lines);
}

// r = E; S(E->f0 ..) - the next statement reading E again where the compiler had it in r: through r (*r, r->fN) (a
// variant: the source read it through the variable, as switch ((k = n->left)->op))
std::optional<std::string> register_reads(sv text) {
    static const Regex load(R"(([ \t]*)(r_\w+) = (\w+(?:->f\d+)+);)");
    Lines lines = split_nl(text);
    for (std::size_t k = 0; k + 1 < lines.size(); ++k) {
        const auto m = load.full_match(lines[k]);
        if (!m) continue;
        const std::string r = str((*m)[2]), e = str((*m)[3]);
        bool next_reads = false;   // `E\b`
        for (std::size_t p = lines[k + 1].find(e); p != npos && !next_reads; p = lines[k + 1].find(e, p + 1))
            next_reads = boundary_at(lines[k + 1], p + e.size());
        if (!next_reads) continue;
        // `^[ \t]*register struct \w+ \*R;$`: the register declared a pointer to a struct
        bool is_struct_pointer = false;
        for (const sv line : ct::split(text, '\n')) {
            sv rest = line.substr(std::min(line.find_first_not_of(" \t"), line.size()));
            if (!rest.starts_with("register struct ")) continue;
            rest.remove_prefix(16);
            std::size_t w = 0;
            while (w < rest.size() && is_word(rest[w])) ++w;
            if (w > 0 && rest.substr(w) == " *" + r + ";") is_struct_pointer = true;
        }
        const std::size_t ind = indent_of(lines[k]);
        std::size_t base_end = 0;
        while (base_end < e.size() && is_word(e[base_end])) ++base_end;
        const std::string base = e.substr(0, base_end);
        bool changed = false;
        for (std::size_t j = k + 1; j < lines.size(); ++j) {   // (on through the block: until r or E's pointer is given again, or a call could change E)
            const std::string x = lines[j];
            if (!blank(x) && indent_of(x) < ind) break;
            // `E(?:->f(\d+))?\b(?!->)`
            std::string out;
            std::size_t at = 0;
            for (std::size_t p = x.find(e); p != npos; p = x.find(e, at)) {
                const std::size_t q = p + e.size();
                std::optional<std::int64_t> field;
                std::size_t end = q;
                if (sv(x).substr(q).starts_with("->f")) {
                    std::size_t d = q + 3;
                    const std::size_t d0 = d;
                    while (d < x.size() && is_digit(x[d])) ++d;
                    if (d > d0 && boundary_at(x, d) && !sv(x).substr(d).starts_with("->")) {
                        field = to_int(sv(x).substr(d0, d - d0));
                        end = d;
                    }
                }
                if (!field && !(boundary_at(x, q) && !sv(x).substr(q).starts_with("->"))) {
                    out.append(x, at, p + 1 - at);
                    at = p + 1;
                    continue;
                }
                out.append(x, at, p - at);
                if (!field) out += r;   // (the value itself: the register)
                else if (is_struct_pointer) out += std::format("{}->f{}", r, *field);
                else if (*field == 0) out += "*" + r;
                else out.append(x, p, end - p);
                at = end;
            }
            out.append(x, at, npos);
            if (out != x) {
                lines[j] = out;
                changed = true;
            }
            if (starts_blank(x, r + " = ") || starts_blank(x, base + " = ")) break;
        }
        if (changed) return join_nl(lines);
    }
    return std::nullopt;
}

// r = x = E; with the register variable r given nothing else: x itself where r is read (a variant: the compiler went
// on through the value it still held - the register its own scratch, not a variable)
std::optional<std::string> register_copies(sv text, const FunctionFacts& facts) {
    static const Regex copy(R"((?m)^([ \t]*)(r_\w+) = (\w+) = ([^;\n]+);$)");
    std::optional<std::string> result;
    copy.for_each_match(text, [&](const RegexMatch& m) {
        const std::string r = str(m[2]), x = str(m[3]);
        const sv rest = text.substr(m.end());
        bool steps_after = assigned_or_stepped(rest, r, true) || assigned_or_stepped(rest, x, true);
        for (const char* step : {"++", "--"}) {   // `(?:\+\+|--)R\b`
            const std::string needle = step + r;
            for (std::size_t p = rest.find(needle); p != npos && !steps_after; p = rest.find(needle, p + 1))
                steps_after = boundary_at(rest, p + needle.size());
        }
        if (steps_after) return true;
        std::string t = str(text.substr(0, m.start())) + str(m[1]) + x + " = " + str(m[4]) + ";" + replace_word(rest, r, x);
        Lines lines = split_nl(t);
        for (std::size_t i = 0; i + 1 < lines.size(); ++i) {   // `^[ \t]*register [^;\n]*\bR;\n`: its declaration, gone
            sv l = lines[i];
            l.remove_prefix(std::min(l.find_first_not_of(" \t"), l.size()));
            if (!l.starts_with("register ") || !l.ends_with(r + ";")) continue;
            const sv middle = l.substr(9, l.size() - 9 - 1);
            if (middle.find(';') != npos || !boundary_at(l, l.size() - 1 - r.size())) continue;
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(i));
            t = join_nl(lines);
            break;
        }
        const std::string reg = r.substr(2);   // (words read through the register: an int's pointer)
        if (byte_site(facts, reg + "w", 0) && !byte_site(facts, reg + "*", 0)) t = redeclare(t, x, "int *" + x).value_or(t);
        result = t;
        return false;
    });
    return result;
}

// the text with its int register locals unsigned (a variant: compared, the compiler's jumps say which)
std::optional<std::string> unsigned_registers(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)register int (r_\w+);$)");
    const std::string t = rx.replace_all(text, "${1}register unsigned ${2};");
    if (t == text) return std::nullopt;
    return t;
}

// switch (..) { .. default: return X; } - a switch with no default, the return after it (a variant: the compiler's
// default jump goes to the code after the switch either way, laid out after the cases)
std::optional<std::string> no_default(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)default:\n[ \t]*(return [^;\n]*;)\n([ \t]*)\}$)");
    const std::string t = sub_all(rx, text, [](const RegexMatch& m) {
        if (m[1].size() != m[3].size()) return str(m.whole());
        return str(m[3]) + "}\n" + str(m[3]) + str(m[2]);
    });
    if (t == text) return std::nullopt;
    return t;
}

// (A == (B & 0xff)) - a byte the model masked compared with another: the bytes compared (a variant: the compiler
// compares bytes of two chars as they are)
std::optional<std::string> unmasked(sv text) {
    static const Regex before(R"((==|!=) \(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\))");
    static const Regex after(R"(\(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\) (==|!=))");
    static const Regex negated(R"(!\(([\w>.*\[\]-]+) (==|!=) ([\w>.*\[\]-]+)\))");
    static const Regex high_byte(R"(\b(\w+->f\d+|\*\w+) (==|!=) (12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b)");
    std::string t = before.replace_all(text, "$1 $2");
    t = after.replace_all(t, "$1 $2");
    t = sub_all(negated, t, [](const RegexMatch& m) {
        return "(" + str(m[1]) + (m[2] == "==" ? " != " : " == ") + str(m[3]) + ")";
    });   // (a byte compared with 128..255: the char's)
    const std::string u = sub_all(high_byte, t, [](const RegexMatch& m) {
        return std::format("{} {} (char)0x{:x}", m[1], m[2], to_int(m[3]));
    });
    if (u == text) return std::nullopt;
    return u;
}

// f(a, b): a never read, b only masked (b & K) - a long's high word tested alone: long a; (a & 0xK0000L) (a variant:
// the compiler tests just the high word of a long masked so)
std::optional<std::string> high_words(sv text) {
    static const Regex head_re(R"((?m)^(\w+)\(([\w, ]+)\)\n)");
    static const Regex declaration_free(R"((?m)^[ \t]*(?:register )?\w+ [\w, *]+;$)");
    const auto h = head_re.search(text);
    if (!h) return std::nullopt;
    std::vector<std::string> ps;
    for (const sv p : ct::split((*h)[2], ',')) ps.push_back(str(ct::trim(p)));
    const std::string body = str(text.substr(h->end()));
    for (std::size_t i = 0; i + 1 < ps.size(); ++i) {
        const std::string& p = ps[i];
        const std::string& q = ps[i + 1];
        const std::size_t uses = count_plain_uses(body, q);
        // `\(Q & (-?\d+|0x[0-9a-f]+)\)`: Q masked
        struct Mask {
            std::size_t pos, end;
            std::int64_t value;
        };
        std::vector<Mask> masks;
        const std::string lead = "(" + q + " & ";
        for (std::size_t at = body.find(lead); at != npos; at = body.find(lead, at + 1)) {
            std::size_t d = at + lead.size();
            const std::size_t n0 = d;
            if (d < body.size() && body[d] == '-') ++d;
            const std::size_t digits = d;
            while (d < body.size() && is_digit(body[d])) ++d;
            std::optional<std::int64_t> v;
            if (d > digits && d < body.size() && body[d] == ')') v = to_int(sv(body).substr(n0, d - n0));
            if (!v && body.substr(n0).starts_with("0x")) {
                d = n0 + 2;
                const std::size_t hx = d;
                while (d < body.size() && (is_digit(body[d]) || (body[d] >= 'a' && body[d] <= 'f'))) ++d;
                if (d > hx && d < body.size() && body[d] == ')') v = *ct::parse_int<std::int64_t>(sv(body).substr(hx, d - hx), 16);
            }
            if (v) masks.push_back({at, d + 1, *v});
        }
        std::size_t decl = 0;   // `^[ \t]*(?:register )?\w+ [^;\n]*\bQ\b[^;\n]*;$`
        for (const sv line : ct::split(body, '\n')) {
            if (!line.ends_with(';') || std::ranges::count(line, ';') != 1) continue;
            sv rest = line.substr(std::min(line.find_first_not_of(" \t"), line.size()));
            bool is_decl = false;
            for (const bool reg : {true, false}) {
                sv r = rest;
                if (reg) {
                    if (!r.starts_with("register ")) continue;
                    r.remove_prefix(9);
                }
                std::size_t w = 0;
                while (w < r.size() && is_word(r[w])) ++w;
                if (w > 0 && w < r.size() && r[w] == ' ' && find_word(r.substr(w + 1), q) != npos) is_decl = true;
            }
            if (is_decl) ++decl;
        }
        const std::string stripped = declaration_free.replace_all(body, std::function<std::string(const RegexMatch&)>([](const RegexMatch&) { return std::string(); }));
        if (has_plain_use(stripped, p) || masks.empty() || uses != masks.size() + decl) continue;
        std::string t;
        std::size_t at = 0;
        for (const Mask& m : masks) {
            t.append(body, at, m.pos - at);
            t += std::format("({} & 0x{:x}0000L)", p, m.value & 0xffff);
            at = m.end;
        }
        t.append(body, at, npos);
        t = drop_decl(t, q);
        const auto redeclared = redeclare(t, p, "long " + p);
        if (!redeclared) return std::nullopt;
        std::vector<std::string> rest;
        for (const std::string& x : ps)
            if (x != q) rest.push_back(x);
        return str(text.substr(0, h->start())) + str((*h)[1]) + "(" + ct::join(rest, ", ") + ")\n" + *redeclared;
    }
    return std::nullopt;
}

// X = E; f(..X..); - the value passed from the register it was stored from: f(..(X = E)..) (a variant)
std::optional<std::string> assign_in_args(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)([A-Za-z_][\w>\[\]-]*) = ([^;\n=]+);\n[ \t]*((?:return )?[A-Za-z_]\w*\(.*\);)$)");
    const std::string t = sub_all(rx, text, [](const RegexMatch& m) {
        const std::string x = str(m[2]), e = str(m[3]), call = str(m[4]);
        bool address_taken = false;   // `&\s*X(?![\w(])`
        for (std::size_t p = call.find('&'); p != npos && !address_taken; p = call.find('&', p + 1)) {
            std::size_t q = p + 1;
            while (q < call.size() && ct::is_space(call[q])) ++q;
            if (sv(call).substr(q).starts_with(x)) {
                const std::size_t after = q + x.size();
                address_taken = after >= call.size() || !(is_word(call[after]) || call[after] == '(');
            }
        }
        if (count_plain_uses(call, x) != 1 || address_taken) return str(m.whole());
        return str(m[1]) + replace_first_plain_use(call, x, "(" + x + " = " + e + ")");
    });
    if (t == text) return std::nullopt;
    return t;
}

// X = Y; return X; - the value still in the register it was stored from: return Y (a variant: a reload of X is other
// code)
std::optional<std::string> return_sources(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)([^=;\n]+?) = ([A-Za-z_]\w*);\n([ \t]*)return \2;$)");
    const std::string t = rx.replace_all(text, "$1$2 = $3;\n$4return $3;");
    if (t == text) return std::nullopt;
    return t;
}

// X = X op E; - the compiler's `op [X], reg` - as C's compound assignment X op= E (a variant: MS C gives the two
// different code)
std::optional<std::string> compound_assigns(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)([A-Za-z_*][\w>\[\]*-]*) = \2 ([-+*/%&|^]|<<|>>) (.+);$)");
    const std::string t = sub_all(rx, text, [](const RegexMatch& m) {
        std::string e = str(m[4]);
        if (e.starts_with('(') && e.ends_with(')') && dt::strip_parens("(" + e + ")") == e) e = e.substr(1, e.size() - 2);
        return str(m[1]) + str(m[2]) + " " + str(m[3]) + "= " + e + ";";
    });
    if (t == text) return std::nullopt;
    return t;
}

// A = E; B = A; - the value given twice from the register it is in: B = A = E (a variant: a reload of A reads the
// same)
std::optional<std::string> chain_assigns(sv text) {
    static const Regex rx(R"((?m)^([ \t]*)([^\s=;][^=;]*?) = ([^;\n]+);\n\1([^\s=;][^=;]*?) = \2;$)");
    static const Regex effects(R"(\+\+|--|\w\()");
    const std::string t = sub_all(rx, text, [](const RegexMatch& m) {
        const std::string a = str(m[2]), e = str(m[3]), b = str(m[4]);
        if (b == a || find_word(e, b) != npos || effects.test(a + b)) return str(m.whole());
        return str(m[1]) + b + " = " + a + " = " + e + ";";
    });
    if (t == text) return std::nullopt;
    return t;
}

namespace {

// `register [\w ]+\**\s*\bX;` on the line (after any whitespace): the register declaration of exactly X.
bool declares_register(sv line, sv x) {
    sv l = line.substr(std::min(line.find_first_not_of(" \t\r\n\f\v"), line.size()));
    if (!l.starts_with("register ") || !l.ends_with(str(x) + ";")) return false;
    sv before = l.substr(9, l.size() - 9 - x.size() - 1);
    if (!before.empty() && is_word(before.back())) return false;   // (the name is a whole word)
    while (!before.empty() && ct::is_space(before.back())) before.remove_suffix(1);
    while (!before.empty() && before.back() == '*') before.remove_suffix(1);
    return !before.empty() && std::ranges::all_of(before, [](char c) { return is_word(c) || c == ' '; });
}

}  // namespace

// a register variable given a call's value and read once, by the next statement, and never again: the compiler's own
// keeping of a value it computed first (strlen(b) of strlen(a) + strlen(b)) - the call in its place, the variable gone
// (a variant)
std::optional<std::string> register_temps(sv text) {
    const dt::BodySplit split = dt::split_body(text);
    if (split.brace.empty()) return std::nullopt;
    static const Regex assign(R"((\s*)(\w+) = (\w+\((?:[^()]|\([^()]*\))*\));$)");
    Lines lines = split_nl(split.body);
    bool changed = false;
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = assign.match(lines[k]);
        if (!m) continue;
        const std::string x = str((*m)[2]);
        bool declared = false;
        for (const std::string& l : lines)
            if (declares_register(l, x)) declared = true;
        if (!declared) continue;
        std::vector<std::size_t> rest;
        for (std::size_t j = 0; j < lines.size(); ++j)
            if (j != k && has_word(lines[j], x) && !starts_with_register(lines[j])) rest.push_back(j);
        if (rest.size() != 1 || rest[0] != k + 1 || count_word(lines[k + 1], x) != 1) continue;
        lines[k + 1] = replace_word(lines[k + 1], x, str((*m)[3]));
        std::vector<char> gone(lines.size(), 0);
        gone[k] = 1;
        for (std::size_t j = 0; j < lines.size(); ++j)
            if (j != k && declares_register(lines[j], x)) gone[j] = 1;
        changed = true;
        return split.head + split.brace + join_live(lines, gone);
    }
    if (!changed) return std::nullopt;
    return std::nullopt;
}

// a byte register stored into a local (vN = al) is a char local; a test of the register at once is a test of the store
// - if ((vN = al) != K): the compiler compares what it has - and the register read later, before it is given another
// value, is the local's value. A constant 128..255 it is compared with: (char)0xNN, a byte's
std::string char_stores(sv text_in) {
    static const Regex store(R"((\s*)(v\d+) = ([abcd]l);\s*$)");
    static const Regex label(R"(\s*L_\w+:)");
    Lines lines = split_nl(text_in);
    std::set<std::string> chars;
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = store.match(lines[k]);
        if (!m) {
            ++k;
            continue;
        }
        const std::string v = str((*m)[2]), r = str((*m)[3]);
        const std::string w = std::string(1, r[0]) + "x";
        std::optional<std::size_t> nx;
        for (std::size_t j = k + 1; j < lines.size() && !nx; ++j)
            if (!blank(lines[j])) nx = j;
        if (!nx) break;
        std::size_t next = *nx;
        const sv rest = ct::ltrim(sv(lines[next]));
        const bool in_test = (rest.starts_with("if (") || rest.starts_with("while (")) && has_word(rest.substr(rest.find('(')), r);
        if (in_test) {
            lines[next] = replace_word(lines[next], r, "(" + v + " = " + r + ")", true);
            lines.erase(lines.begin() + static_cast<std::ptrdiff_t>(k));
            --next;
        } else {
            ++k;
            next = k;
        }
        chars.insert(v);
        const auto assigns_reg = [](sv line, sv reg) {   // `\bREG = `
            const std::string needle = str(reg) + " = ";
            for (std::size_t p = line.find(needle); p != npos; p = line.find(needle, p + 1))
                if (boundary_at(line, p)) return true;
            return false;
        };
        for (std::size_t j = next + 1; j < lines.size(); ++j) {   // (read again before it changes: the local)
            if (label.match(lines[j]) || assigns_reg(lines[j], r) || assigns_reg(lines[j], w) || has_call_paren(lines[j])) break;
            lines[j] = replace_word(lines[j], r, v);
        }
    }
    if (chars.empty()) return str(text_in);
    std::string text = join_nl(lines);
    for (const std::string& v : chars) {
        Lines ls = split_nl(text);
        for (std::string& l : ls) {   // `^([ \t]*)int V;` as char
            const std::size_t p = l.find_first_not_of(" \t");
            if (p != npos && sv(l).substr(p).starts_with("int " + v + ";")) l = l.substr(0, p) + "char " + l.substr(p + 4);
        }
        text = join_nl(ls);
        // `(\(V = [abcd]l\)|\bV) ([!=]=|[<>]=?) (128..255)\b`: a byte compared with a constant the char's
        std::string out;
        std::size_t at = 0;
        for (std::size_t p = 0; p < text.size(); ++p) {
            std::size_t after = npos;
            if (text.compare(p, v.size() + 4, "(" + v + " = ") == 0 && p + v.size() + 7 <= text.size() &&
                std::string_view("abcd").find(text[p + v.size() + 4]) != npos && text.compare(p + v.size() + 5, 2, "l)") == 0) {
                after = p + v.size() + 7;
            } else if (boundary_at(text, p) && text.compare(p, v.size(), v) == 0 && is_word(text[p])) {
                after = p + v.size();
            }
            if (after == npos || after >= text.size() || text[after] != ' ') continue;
            std::size_t q = after + 1;
            std::size_t op_len = 0;
            if (text.compare(q, 2, "!=") == 0 || text.compare(q, 2, "==") == 0) op_len = 2;
            else if (q < text.size() && (text[q] == '<' || text[q] == '>')) op_len = q + 1 < text.size() && text[q + 1] == '=' ? 2 : 1;
            if (op_len == 0 || q + op_len >= text.size() || text[q + op_len] != ' ') continue;
            const std::size_t n0 = q + op_len + 1;
            std::size_t n1 = n0;
            while (n1 < text.size() && is_digit(text[n1])) ++n1;
            if (n1 - n0 != 3 || (n1 < text.size() && is_word(text[n1]))) continue;
            const std::int64_t n = to_int(sv(text).substr(n0, 3));
            if (n < 128 || n > 255) continue;
            out.append(text, at, p - at);
            out += std::format("{} {} (char)0x{:02x}", text.substr(p, after - p), text.substr(q, op_len), n);
            at = n1;
            p = n1 - 1;
        }
        out.append(text, at, npos);
        text = out;
    }
    return text;
}

// *p = X; p = p + 1; (X not reading p) - the store and the step as one: *p++ = X (a variant: the compiler makes other
// code of the two)
std::optional<std::string> store_increments(sv text) {
    static const Regex store(R"((\s*)\*(\w+) = (.*);\s*$)");
    Lines lines = split_nl(text);
    Lines out;
    std::size_t k = 0;
    while (k < lines.size()) {
        const auto m = store.match(lines[k]);
        if (m && k + 1 < lines.size() && ct::trim(sv(lines[k + 1])) == str((*m)[2]) + " = " + str((*m)[2]) + " + 1;" &&
            find_word((*m)[3], (*m)[2]) == npos) {
            out.push_back(str((*m)[1]) + "*" + str((*m)[2]) + "++ = " + str((*m)[3]) + ";");
            k += 2;
            continue;
        }
        out.push_back(lines[k]);
        ++k;
    }
    const std::string t = join_nl(out);
    if (t == text) return std::nullopt;
    return t;
}

// r = E; (E a variable, a field, *p) read more than once after, nothing between changing E or calling: E at each read
// - the compiler kept E in the register and read it there (r a machine register left over)
std::string register_values(sv text) {
    static const Regex load(R"(([ \t]*)(ax|al|bx|bl|cx|cl|dx|dl) = ([A-Za-z_]\w*(?:->f\d+)*|\*[A-Za-z_]\w*);)");
    static const Regex label(R"([ \t]*L_\w+:)");
    static const Regex keyword_open(R"(\b(?:if|while|for|switch|return)\s*\()");
    static const Regex call(R"(\w\s*\()");
    Lines lines = split_nl(text);
    for (std::size_t k = 0; k < lines.size(); ++k) {
        const auto m = load.full_match(lines[k]);
        if (!m) continue;
        const std::string reg = str((*m)[2]), e = str((*m)[3]);
        std::vector<std::string> names;   // `[A-Za-z_]\w*` anywhere in e
        for (std::size_t i = 0; i < e.size();) {
            if (!(std::isalpha(static_cast<unsigned char>(e[i])) || e[i] == '_')) {
                ++i;
                continue;
            }
            std::size_t j = i;
            while (j < e.size() && is_word(e[j])) ++j;
            names.push_back(e.substr(i, j - i));
            i = j;
        }
        std::int64_t depth = 0;
        std::vector<std::size_t> uses;
        std::optional<std::size_t> end = lines.size();
        for (std::size_t j = k + 1; j < lines.size(); ++j) {
            const std::string& x = lines[j];
            depth += brace_balance(x);
            if (depth < 0 || starts_blank(x, reg + " = ") || label.match(x)) {
                end = j;
                break;
            }
            bool blocked = call.search(keyword_open.replace_all(x, std::function<std::string(const RegexMatch&)>([](const RegexMatch&) { return std::string(); }))).has_value() ||
                           assigned_literal(x, e);
            for (const std::string& n : names)
                if (assigned_or_stepped(x, n, false)) blocked = true;
            if (blocked) {
                end = std::nullopt;
                break;
            }
            if (has_word(x, reg)) uses.push_back(j);
        }
        if (!end || uses.size() < 2 || dt::live_after(lines, *end, reg)) continue;
        for (const std::size_t j : uses) lines[j] = replace_word(lines[j], reg, e);
        std::vector<char> gone(lines.size(), 0);
        gone[k] = 1;
        return join_live(lines, gone);
    }
    return str(text);
}

}  // namespace cb::exact::draft::forms
