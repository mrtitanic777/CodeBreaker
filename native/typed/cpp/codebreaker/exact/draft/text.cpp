// codebreaker/exact/draft/text.cpp -- typed C++ from codebreaker.exact.draft.text (see text.hpp).
#include "codebreaker/exact/draft/text.hpp"

#include "py_os.hpp"

namespace codebreaker::exact::draft::text {

namespace {

// len(l) - len(l.lstrip()): the indentation of a line
int64_t indent_of(const py::str& l) { return l.size() - l.lstrip().size(); }

// s.startswith(p, k)
bool starts_at(const py::str& s, const py::str& p, int64_t k) {
    if (k < 0 || k + p.size() > s.size()) return false;
    return s.u32().compare(static_cast<size_t>(k), static_cast<size_t>(p.size()), p.u32()) == 0;
}

int64_t paren(char32_t ch) { return (ch == U'(' ? 1 : 0) - (ch == U')' ? 1 : 0); }

py::str ps(const char* s) { return py::str(s); }

// re.compile(pattern): the runtime compiles a pattern at its first use, and reports a bad one as the l2
// helpers' PyError("ValueError") rather than re.error; it is turned into py::PatternError here
py::re::Pattern compile_checked(const py::str& pattern) {
    try {
        return py::re::compile(pattern);
    } catch (const py::Exception&) {
        throw;
    } catch (const std::runtime_error& e) {   // (the l2 PyError, in a header py.hpp cannot share)
        throw py::PatternError(e.what());
    }
}

}  // namespace

const py::str REGS = py::str("\\b(ax|bx|cx|dx|si|di|al|ah|bl|bh|cl|ch|dl|dh|es)\\b");
const py::str WREG = py::str("(ax|bx|cx|dx|si|di)");

// ((X)) -> (X): the parentheses a test is printed in, once
py::str _strip(const py::str& c_in) {
    py::str c = c_in;
    while (c.startswith(ps("((")) && c.endswith(ps("))"))) {
        int64_t depth = 0;
        py::str mid = c.slice(1, -1);
        for (char32_t ch : mid.u32()) {
            depth += paren(ch);
            if (depth < 0) return c;
        }
        if (depth) return c;
        c = c.slice(1, -1);
    }
    return c;
}

// a test as the value it tests: (X == 0) -> !X, (X != 0) -> X
py::str _truth(const py::str& c_in) {
    py::str c = _strip(c_in);
    auto m = py::re::fullmatch(ps("\\((.*) != 0\\)"), c);
    if (m) return m->group(1);
    m = py::re::fullmatch(ps("\\((.*) == 0\\)"), c);
    if (m) {
        if (py::re::fullmatch(ps("[\\w.\\->\\[\\]()]+"), m->group(1)))
            return py::percent(ps("!%s"), m->group(1));
        return py::percent(ps("!(%s)"), m->group(1));
    }
    return c;
}

bool _balanced(const py::str& s) {
    int64_t depth = 0;
    for (char32_t ch : s.u32()) {
        depth += paren(ch);
        if (depth < 0) return false;
    }
    return depth == 0;
}

// c split at op (&& or ||) outside parentheses, or [c]
py::list<py::str> _top_split(const py::str& c, const py::str& op) {
    py::list<py::str> out;
    int64_t depth = 0;
    std::u32string cur;
    int64_t k = 0;
    py::str sep = py::percent(ps(" %s "), op);
    while (k < c.size()) {
        char32_t ch = c.at_(k);
        depth += paren(ch);
        if (depth == 0 && starts_at(c, sep, k)) {
            out.append(py::str(cur));
            cur.clear();
            k += op.size() + 2;
            continue;
        }
        cur.push_back(ch);
        k += 1;
    }
    out.append(py::str(cur));
    return out;
}

py::str _not(const py::str& c_in) {
    py::str c = _strip(c_in);
    py::str inner = (c.startswith(ps("(")) && c.endswith(ps(")")) && _balanced(c.slice(1, -1))) ? c.slice(1, -1) : c;
    auto m = py::re::fullmatch(ps("!\\((.*)\\)"), inner);                // (!(X): X)
    if (m && _balanced(m->group(1))) return py::percent(ps("(%s)"), m->group(1));
    if (py::re::fullmatch(ps("![\\w\\->\\[\\].]+"), inner))              // (!x: x)
        return inner.slice_from(1);
    const std::pair<const char*, const char*> ops[] = {{"&&", "||"}, {"||", "&&"}};
    for (const auto& pr : ops) {                                         // (De Morgan, a level down)
        py::list<py::str> parts = _top_split(inner, ps(pr.first));
        if (parts.size() > 1) {
            py::list<py::str> items;
            for (const py::str& p : parts) items.append(_truth(_not(ps("(") + p + ps(")"))));
            return py::percent(ps("(%s)"), py::join(py::percent(ps(" %s "), ps(pr.second)), items));
        }
    }
    m = py::re::fullmatch(ps("\\((.*?) (==|!=|<=|>=|<|>) (.*)\\)"), c);
    if (m && !m->group(2).empty() && !m->group(3).contains(ps("(")) &&
        m->group(1).count(ps("(")) == m->group(1).count(ps(")"))) {
        py::str op = m->group(2);
        py::str flip = op == "==" ? ps("!=") : op == "!=" ? ps("==") : op == "<" ? ps(">=") : op == ">=" ? ps("<")
                     : op == ">" ? ps("<=") : ps(">");
        if (m->group(3) != "0" || !(op == "==" || op == "!="))           // (x == 0: !x below; a relation to 0 flips)
            return py::percent(ps("(%s %s %s)"), m->group(1), flip, m->group(3));
    }
    m = py::re::fullmatch(ps("\\((.*) == 0\\)"), c);
    if (m) return m->group(1);
    m = py::re::fullmatch(ps("\\((.*) != 0\\)"), c);
    if (m) return py::percent(ps("!%s"), m->group(1));
    if (py::re::fullmatch(ps("[\\w\\->\\[\\].]+"), inner)) return py::percent(ps("!%s"), inner);
    return py::percent(ps("!%s"), c);
}

// (bytes the code widens with zeros: unsigned char)
const py::set<std::variant<std::tuple<py::str, int64_t>, py::str>> UCHAR;
// (globals the code reads or writes a byte of: char)
const py::set<py::str> BYTEG;

// the line opening the block line k (`}` or `} else {`) closes: the nearest before it at its indentation
std::optional<int64_t> _opener(const py::list<py::str>& lines, int64_t k) {
    int64_t ind = indent_of(lines[k]);
    for (int64_t j = k - 1; j >= 0; --j)
        if (py::truthy(lines[j].strip()) && indent_of(lines[j]) == ind) return j;
    return std::nullopt;
}

// is line j inside a loop or a switch opened after line lo, deeper than the indentation ind
bool _in_loop(const py::list<py::str>& lines, int64_t lo, int64_t j, int64_t ind) {
    int64_t want = indent_of(lines[j]);
    for (int64_t x = j - 1; x > lo; --x) {
        py::str l = lines[x];
        if (!py::truthy(l.strip())) continue;
        int64_t d = indent_of(l);
        if (d < want && l.rstrip().endswith(ps("{"))) {
            if (d <= ind) return false;
            if (py::re::match(ps("\\s*(?:while|for|do|switch)\\b"), l) || l.strip() == "do {") return true;
            want = d;
        }
    }
    return false;
}

// f(text), or None when it changes nothing (a pass as a variant)
std::optional<py::str> _changed(const std::function<py::str(const py::str&)>& f, const py::str& text) {
    py::str t = f(text);
    if (t != text) return t;
    return std::nullopt;
}

// [the line opening each block around line k, the innermost first]
py::list<int64_t> _openers(const py::list<py::str>& lines, int64_t k) {
    py::list<int64_t> out;
    int64_t want = indent_of(lines[k]);
    for (int64_t j = k - 1; j >= 0; --j) {
        py::str x = lines[j];
        if (!py::truthy(x.strip())) continue;
        int64_t d = indent_of(x);
        if (d < want && x.rstrip().endswith(ps("{"))) {
            out.append(j);
            want = d;
        }
    }
    return out;
}

const py::list<bool> GOTO_LIVE = {true};

// is reg (or what `read` matches: its byte too) read after line k before it is given a new value - on any
// path: a value given it inside a nested block (an arm, a loop) is not on every path, so it ends nothing; one
// given at k's own depth, in a bare block, or out of k's block does (lines None: dropped)
bool _live_after(const py::list<std::optional<py::str>>& lines, int64_t k, const py::str& reg,
                 const std::optional<py::str>& read) {
    py::re::Pattern rx = compile_checked(read && py::truthy(*read) ? *read : py::percent(ps("\\b%s\\b"), reg));
    py::re::Pattern kill = compile_checked(py::percent(ps("\\s*%s = (?![^;]*\\b%s\\b)"), reg, reg));
    std::vector<bool> stack;                       // (the blocks open since k: True a conditional one)
    std::optional<int64_t> skip;                   // (the depth of an arm beside k's, not on its path)
    std::optional<int64_t> killed;                 // (the depth of a block that gave it a new value)
    auto any_open = [&] {
        for (bool b : stack)
            if (b) return true;
        return false;
    };
    for (const std::optional<py::str>& line : lines.slice_from(k + 1)) {
        if (!line) continue;
        const py::str& l = *line;
        int64_t opens = l.count(ps("{")), closes = l.count(ps("}"));
        if (skip) {
            *skip += opens - closes;
            if (*skip < 0) skip.reset();           // (the other arm closed: past the whole if)
            continue;
        }
        if (py::re::match(ps("\\s*\\} else\\b.*\\{\\s*$"), l) && stack.empty()) {   // (leaving k's arm into the other: skipped)
            skip = 0;
            continue;
        }
        if (stack.empty() && py::re::match(ps("\\s*return\\b"), l) && !rx.search(l)) return false;   // (the path ends)
        std::optional<py::re::Match> g_;
        if (GOTO_LIVE[0] && !killed) g_ = py::re::match(ps("\\s*goto (L_\\w+);"), l);
        if (g_) {                                  // (a jump elsewhere: read there before it is given?)
            std::optional<int64_t> at;
            py::str label = g_->group(1) + ps(":");
            for (int64_t j = 0; j < lines.size(); ++j) {
                const std::optional<py::str>& x = lines[j];
                if (x && x->strip() == label) { at = j; break; }
            }
            if (at) {
                for (const std::optional<py::str>& x : lines.slice(*at + 1, *at + 40)) {
                    if (!x) continue;
                    if (kill.match(*x)) break;     // (given there first: not this value)
                    if (rx.search(*x)) return true;
                    if (py::re::match(ps("\\s*(?:return|goto|break|continue)\\b"), *x)) break;
                }
            }
        }
        if (stack.empty() && py::re::match(ps("\\s*(?:break|continue);"), l)) {
            skip = 0;                              // (to the end of the block it leaves: the rest of
            continue;                              //  it - a switch's other cases - not on its path)
        }
        if (killed && static_cast<int64_t>(stack.size()) < *killed) killed.reset();   // (out of the block that gave it a new value)
        if (kill.match(l)) {
            if (!any_open()) return static_cast<bool>(rx.search(py::split(l, ps("="), 1)[1]));
            if (!killed && rx.search(py::split(l, ps("="), 1)[1])) return true;   // (given a value on some paths: read only in it)
            int64_t depth = static_cast<int64_t>(stack.size());
            killed = !killed ? depth : std::min(*killed, depth);
        } else if (rx.search(l) && !killed) {
            return true;
        }
        for (int64_t i = 0; i < closes; ++i)
            if (!stack.empty()) stack.pop_back();  // (out of k's block: what follows is on its path)
        for (int64_t i = 0; i < opens; ++i) stack.push_back(l.strip() != "{");
    }
    return false;
}

const py::dict<py::str, int64_t> SIZES = {{ps("char"), 1}, {ps("int"), 2}, {ps("unsigned"), 2}, {ps("short"), 2},
                                          {ps("long"), 4}};
const py::dict<py::str, int64_t> PTRSIZE;
const py::dict<py::str, std::tuple<int64_t, std::variant<py::str, py::list<std::tuple<int64_t, py::str>>>>> STRUCTARR;
const py::dict<py::str, py::str> ARRAYS;
const py::dict<py::str, std::optional<py::str>> FILEPTR;
const py::dict<py::str, py::dict<py::str, int64_t>> _LIBDATA;
const py::dict<std::tuple<py::str, py::str>, py::set<py::str>> _HEADERS;
const py::dict<std::tuple<py::str, py::str>, py::list<std::tuple<py::str, py::str, int64_t, int64_t>>> _TABLES;

// s split at its commas outside parentheses and brackets
py::list<py::str> _top_commas(const py::str& s) {
    py::list<py::str> out;
    int64_t depth = 0;
    std::u32string cur;
    for (char32_t ch : s.u32()) {
        if (ch == U'(' || ch == U'[') depth += 1;
        else if (ch == U')' || ch == U']') depth -= 1;
        if (ch == U',' && depth == 0) {
            out.append(py::str(cur));
            cur.clear();
        } else {
            cur.push_back(ch);
        }
    }
    out.append(py::str(cur));
    return out;
}

// the index of the line closing the block line k opens (by indentation: the first line back at its indent)
std::optional<int64_t> _block_end(const py::list<py::str>& lines, int64_t k) {
    int64_t ind = indent_of(lines[k]);
    for (int64_t j = k + 1; j < lines.size(); ++j)
        if (py::truthy(lines[j].strip()) && indent_of(lines[j]) <= ind) return j;
    return std::nullopt;
}

// `} else if (C) {` chains written out as nested ifs, each inside the else before it
py::list<py::str> _else_ifs(py::list<py::str> lines) {
    int64_t k = 0;
    while (k < lines.size()) {
        auto m = py::re::match(ps("(\\s*)\\} else if \\((.*)\\) \\{$"), lines[k]);
        if (!m) {
            k += 1;
            continue;
        }
        py::str ind = m->group(1);
        int64_t e = k + 1;           // (the chain's end: the `}` at the if's indent that
        while (e < lines.size() && !(lines[e] == ind + ps("}"))) e += 1;   //  closes the whole chain)
        if (e >= lines.size()) return lines;
        py::list<py::str> inner;
        inner.append(ind + ps("    if (") + m->group(2) + ps(") {"));
        for (const py::str& l : lines.slice(k + 1, e)) inner.append(py::truthy(l.strip()) ? ps("    ") + l : l);
        inner.append(ind + ps("    }"));
        py::list<py::str> repl;
        repl.append(ind + ps("} else {"));
        repl.extend(inner);
        repl.append(ind + ps("}"));
        lines.set_slice(k, e + 1, repl);
        k += 1;
    }
    return lines;
}

// (cond, then, else, end) of the if at line k - `if (C) {`, its arms' lines, the line after it - or None
std::optional<IfParts> _if_parts(const py::list<py::str>& lines, int64_t k) {
    auto m = py::re::match(ps("(\\s*)if \\((.*)\\) \\{$"), lines[k]);
    if (!m) return std::nullopt;
    py::str ind = m->group(1);
    int64_t j = k + 1;
    while (j < lines.size() && !lines[j].startswith(ind + ps("}"))) {
        if (py::truthy(lines[j].strip()) && !lines[j].startswith(ind + ps(" "))) return std::nullopt;
        j += 1;
    }
    if (j >= lines.size()) return std::nullopt;
    py::list<py::str> then = lines.slice(k + 1, j);
    if (lines[j] == ind + ps("}")) return IfParts{m->group(2), then, std::nullopt, j + 1};
    if (lines[j] != ind + ps("} else {")) return std::nullopt;
    int64_t e = j + 1;
    while (e < lines.size() && !lines[e].startswith(ind + ps("}"))) {
        if (py::truthy(lines[e].strip()) && !lines[e].startswith(ind + ps(" "))) return std::nullopt;
        e += 1;
    }
    if (e >= lines.size() || lines[e] != ind + ps("}")) return std::nullopt;
    return IfParts{m->group(2), then, lines.slice(j + 1, e), e + 1};
}

// arm without the label the other arm's goto went to (the only label it loses: others are wanted)
py::list<py::str> _unlabelled(const py::list<py::str>& arm, const std::optional<py::list<py::str>>& other) {
    std::optional<py::re::Match> g;
    if (other && !other->empty()) g = py::re::match(ps("\\s*goto (L_\\w+);$"), (*other)[-1]);
    if (other && !g) return arm;
    py::list<py::str> out;
    for (const py::str& l : arm) {
        bool keep = !py::re::match(ps("\\s*L_\\w+:$"), l) || (g && l.strip() != g->group(1) + ps(":"));
        if (keep) out.append(l);
    }
    return out;
}

// a op b, an operand that is the other kind of join parenthesized (|| inside &&)
py::str _conj(const py::str& op, const py::str& a, const py::str& b) {
    auto wrap = [&](const py::str& x) -> py::str {
        if (op == "&&" && _top_split(x.startswith(ps("((")) ? _strip(x) : x, ps("||")).size() > 1)
            return py::percent(ps("(%s)"), x);
        return x;
    };
    return py::percent(ps("%s %s %s"), wrap(a), op, wrap(b));
}

// do the arms x and y run the same: equal, or x the statements y begins with and then a goto to y's label
// there (the compiler's one copy of a tail both reach) - and the label then wanted by none else
bool _same_arm(const py::list<py::str>& x, const py::list<py::str>& y) {
    if (x == y) return true;
    if (x.empty()) return false;
    auto g = py::re::match(ps("\\s*goto (L_\\w+);$"), x[-1]);
    if (!g) return false;
    py::str lab = g->group(1);
    std::optional<int64_t> at;
    for (int64_t j = 0; j < y.size(); ++j)
        if (y[j].strip() == lab + ps(":")) { at = j; break; }
    if (!at) return false;
    py::list<py::str> head = x.slice(0, -1);
    return head == y.slice(0, *at) && _unlabelled(head) == head;
}

// `X = E;` with reg read in E alone and E giving nothing a value: reg's value read before the store
bool _plain_store(const py::str& l, const py::str& reg) {
    auto m = py::re::fullmatch(ps("\\s*([^=;]+?) = ([^;]+);\\s*"), l);
    return static_cast<bool>(m) && !py::re::search(py::percent(ps("\\b%s\\b"), reg), m->group(1)) &&
           !py::re::search(ps("\\+\\+|--|[^=!<>]=[^=]"), m->group(2)) &&
           !py::re::search(ps("\\w\\s*\\("), m->group(2) + m->group(1));
}

// (the ranges a loaded register is tested against: (lo, hi, flag))
const py::set<std::tuple<int64_t, int64_t, bool>> RANGES;

LongInfo LONGINFO;

// the source with x (an int parameter or local) declared a pointer to ty, or None
std::optional<py::str> _declare_pointer(const py::str& src, const py::str& x, const py::str& ty) {
    auto [head, sep, body] = split_body(src);
    for (int part = 0; part < 2; ++part) {
        py::str t = part == 0 ? head : body;
        auto m = py::re::search(py::percent(ps("(?m)^([ \\t]*)(register\\s+)?int ((?:\\w+, )*)%s((?:, \\w+)*);$"),
                                            py::re::escape(x)), t);
        if (m) {
            py::str reg = m->group_opt(2).value_or(ps(""));
            py::str rest = (m->group(3) + m->group(4).lstrip(ps(", "))).strip(ps(", "));
            py::str decl = py::truthy(rest) ? py::percent(ps("%s%sint %s;\n"), m->group(1), reg, rest) : ps("");
            py::str added = decl + py::percent(ps("%s%s%s *%s;"), m->group(1), reg, ty, x);
            t = t.slice(0, m->start()) + added + t.slice_from(m->end());
            py::str out = part == 0 ? (t + sep + body) : (head + sep + t);
            if (ty == "int") {                     // (its byte offsets, now in words: x + N/2)
                out = py::re::sub(py::percent(ps("\\(\\(int \\*\\)\\(\\(char \\*\\)%s \\+ (\\d+)\\)\\)"), py::re::escape(x)),
                                  [&](const py::re::Match& m2) -> py::str {
                                      int64_t n = py::int_from_str(m2.group(1));
                                      return py::mod(n, 2) == 0 ? py::percent(ps("(%s + %d)"), x, py::floordiv(n, 2))
                                                                : m2.group(0);
                                  }, out);
                out = py::re::sub(py::percent(ps("\\b%s\\[\\((.+?) << 1\\) \\+ (\\d+)\\]"), py::re::escape(x)),   // (an index the code doubles)
                                  [&](const py::re::Match& m2) -> py::str {
                                      int64_t n = py::int_from_str(m2.group(2));
                                      return py::mod(n, 2) == 0 ? py::percent(ps("%s[%s + %d]"), x, m2.group(1), py::floordiv(n, 2))
                                                                : m2.group(0);
                                  }, out);
                out = py::re::sub(py::percent(ps("\\b%s\\[\\((.+?) << 1\\)\\]"), py::re::escape(x)),
                                  py::percent(ps("%s[\\1]"), x), out);
            }
            return out;
        }
    }
    return std::nullopt;
}

// (the byte-wide places the code reads: (pointer, offset))
const py::set<std::tuple<py::str, int64_t>> BYTEPTR;

// (head, "{", body) at the function body's own brace - a line of "{" alone, after its signature - not a
// struct's that a pass declared before it
std::tuple<py::str, py::str, py::str> split_body(const py::str& text) {
    auto m = py::re::search(ps("(?m)^\\{\\s*$"), text);
    if (!m) return text.partition(ps("{"));
    return {text.slice(0, m->start()), ps("{"), text.slice_from(m->start() + 1)};
}

}  // namespace codebreaker::exact::draft::text
