// codebreaker/exact/draft/forms.cpp -- typed C++ from codebreaker.exact.draft.forms (see forms.hpp).
#include "codebreaker/exact/draft/forms.hpp"

#include <algorithm>
#include <cstdlib>
#include <functional>
#include <map>
#include <set>
#include <variant>
#include <vector>

#include "codebreaker/exact/draft/shapes.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::draft::forms {

namespace {

using py::str;
using Lines = py::list<str>;
using OLines = py::list<std::optional<str>>;
using Match = py::re::Match;
using OMatch = std::optional<Match>;
using text::_balanced;
using text::_block_end;
using text::_conj;
using text::_else_ifs;
using text::_if_parts;
using text::_live_after;
using text::_not;
using text::_same_arm;
using text::_strip;
using text::_top_commas;
using text::_top_split;
using text::_truth;
using text::_unlabelled;
using text::split_body;

str operator""_s(const char* p, size_t n) { return str(std::string(p, n)); }

Lines split_nl(const str& t) { return py::split(t, "\n"_s); }
str join_nl(const Lines& l) { return py::join("\n"_s, l); }

// "\n".join(x for x in lines if x is not None)
str join_opt(const OLines& l) {
    Lines out;
    for (int64_t i = 0; i < l.size(); ++i)
        if (l[i]) out.append(*l[i]);
    return join_nl(out);
}

OLines opt_of(const Lines& l) {
    OLines out;
    for (int64_t i = 0; i < l.size(); ++i) out.append(l[i]);
    return out;
}

// len(l) - len(l.lstrip()): the indentation of a line
int64_t indent_of(const str& l) { return l.size() - l.lstrip().size(); }

// bool(x.strip())
bool nb(const str& x) { return !x.strip().empty(); }

str esc(const str& s) { return py::re::escape(s); }

// re.match(r"\s*", l).group(0)
str lead(const str& l) { return py::re::match(R"re(\s*)re"_s, l)->group(0); }

// the n-th group's int
int64_t gi(const Match& m, int64_t g) { return py::int_from_str(m.group(g)); }

// s.replace(a, b, 1)
str replace1(const str& s, const str& a, const str& b) {
    int64_t i = s.find(a);
    if (i < 0) return s;
    return s.slice(0, i) + b + s.slice_from(i + a.size());
}

// x[4:] if x.startswith("    ") else x
str dedent4(const str& x) { return x.startswith("    "_s) ? x.slice_from(4) : x; }

// the next line index of lines[from:] satisfying pred, or none
template <class F> std::optional<int64_t> next_line(const Lines& lines, int64_t from, int64_t to, F pred) {
    for (int64_t j = from; j < to; ++j)
        if (pred(lines[j])) return j;
    return std::nullopt;
}

// [x for x in names] unique, order kept (dict.fromkeys)
Lines uniq(const Lines& xs) {
    Lines out;
    for (int64_t i = 0; i < xs.size(); ++i)
        if (!out.contains(xs[i])) out.append(xs[i]);
    return out;
}

// the sorted set of strings
Lines sorted_strs(const py::set<str>& s) {
    std::vector<str> v;
    for (const str& x : s) v.push_back(x);
    std::sort(v.begin(), v.end());
    return Lines(v);
}

// `m.group(n)` of an optional group that may be absent -> ""
str gopt(const Match& m, int64_t g) { return m.group_opt(g).value_or(str("")); }

}  // namespace

const py::list<bool> REGSLOTS = py::list<bool>{false};

// the locals renamed so that the compiler gives each the frame slot the program shows. rule (the toolchain
// profile's "frame_order"): {"mod": M} - the compiler gives the locals of a function (register ones too) their
// slots in the order of their names' buckets in its table of names, (the sum of the name's characters) mod M,
// ascending, the first next to BP. An auto the model names vN is at BP-N; a filler _pN after vN; a register
// local takes a free slot. A name is kept where its bucket falls in order, else a letter is put after it.
std::optional<str> frame_names(const str& text, const FrameRule& rule, bool regs_last) {
    struct Decl {
        str name;
        bool reg;
        int64_t size;
        bool operator==(const Decl& o) const { return name == o.name && reg == o.reg && size == o.size; }
    };
    if (!rule.mod || *rule.mod == 0 || !text.contains("{"_s)) return std::nullopt;
    const int64_t mod = *rule.mod;
    auto [head, _sep, body] = split_body(text);
    Lines lines = split_nl(body);
    std::vector<Decl> decls;
    for (int64_t k = 1; k < lines.size(); ++k) {
        const str& l = lines[k];
        auto m = py::re::match(
            R"re(\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short)\b|struct\s+\w+)\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$)re"_s, l);
        if (!m) {
            if (nb(l)) break;
            continue;
        }
        str reg = gopt(*m, 1), ty = m->group(2), star = m->group(3), name = m->group(4), cnt = gopt(*m, 5);
        if (ty.startswith("struct"_s) && star.empty()) return std::nullopt;   // (a struct's size the text does not say)
        Lines tw = py::split(ty);
        int64_t size = !star.empty() ? 2 : text::SIZES.get(tw[-1], 2) * (cnt.empty() ? 1 : py::int_from_str(cnt));
        decls.push_back(Decl{name, !reg.empty(), cnt.empty() ? std::max<int64_t>(size, 2) : size + (size & 1)});
    }
    std::vector<Decl> autos;
    for (const Decl& d : decls)
        if (!d.reg && py::re::fullmatch(R"re(v\d+)re"_s, d.name)) autos.push_back(d);
    if (autos.empty()) return std::nullopt;
    std::stable_sort(autos.begin(), autos.end(), [](const Decl& a, const Decl& b) {
        return py::int_from_str(a.name.slice_from(1)) < py::int_from_str(b.name.slice_from(1));
    });
    std::vector<Decl> fill;   // (a dict: insertion order kept, a name given again takes the new value in its place)
    auto fill_find = [&](const str& n) -> int64_t {
        for (size_t i = 0; i < fill.size(); ++i)
            if (fill[i].name == n) return static_cast<int64_t>(i);
        return -1;
    };
    for (const Decl& d : decls)
        if (py::re::fullmatch(R"re(_p\d+)re"_s, d.name)) {
            int64_t at = fill_find(d.name);
            if (at >= 0) fill[at] = d;
            else fill.push_back(d);
        }
    std::vector<Decl> regs;
    for (const Decl& d : decls)
        if (d.reg) regs.push_back(d);
    std::vector<Decl> order;
    std::set<std::string> drop;
    std::vector<std::pair<str, int64_t>> resize;
    std::vector<Decl> added;
    int64_t cum = 0;
    for (const Decl& d : autos) {   // (each auto at its place; what lies between: register locals, then a filler)
        int64_t n = py::int_from_str(d.name.slice_from(1));
        int64_t gap = n - d.size - cum;
        int64_t took = 0;
        while (gap >= 2 && !regs.empty() && !regs_last) {
            order.push_back(regs.front());
            regs.erase(regs.begin());
            gap -= 2;
            ++took;
        }
        str pn = py::percent("_p%d"_s, cum);
        int64_t fi = fill_find(pn);
        if (fi >= 0) {
            if (gap <= 0) {   // (the register locals take the room: no filler)
                drop.insert(pn.utf8());
                fill.erase(fill.begin() + fi);
            } else {
                order.push_back(fill[fi]);
                if (took) {   // (the filler what they leave of it)
                    bool seen = false;
                    for (auto& r : resize)
                        if (r.first == pn) {
                            r.second = gap;
                            seen = true;
                        }
                    if (!seen) resize.push_back({pn, gap});
                }
            }
        } else if (gap > 0 && REGSLOTS[0]) {   // (room no local or register takes: a filler there - an unused local of the source's)
            Decl f_{pn, false, gap};
            order.push_back(f_);
            added.push_back(f_);
        }
        order.push_back(d);
        cum = n;
    }
    {
        std::vector<Decl> rest;
        for (const Decl& f : fill)
            if (std::find(order.begin(), order.end(), f) == order.end()) rest.push_back(f);
        for (const Decl& r : regs) order.push_back(r);
        for (const Decl& r : rest) order.push_back(r);
    }
    if (static_cast<int64_t>(order.size()) > mod) return std::nullopt;
    py::set<str> taken;
    for (const str& w : py::re::findall(R"re(\b[A-Za-z_]\w*\b)re"_s, text)) taken.add(w);
    auto bucket = [&](const str& s) {
        int64_t t = 0;
        for (int64_t i = 0; i < s.size(); ++i) t += static_cast<int64_t>(s.at_(i));
        return py::mod(t, mod);
    };
    std::vector<std::pair<str, str>> new_;
    int64_t prev = -1;
    for (size_t k = 0; k < order.size(); ++k) {
        int64_t room = mod - (static_cast<int64_t>(order.size()) - static_cast<int64_t>(k));   // (the last bucket this one may take)
        str nm = order[k].name;
        if (!(prev < bucket(nm) && bucket(nm) <= room)) {
            int64_t want = prev + 1;
            std::optional<str> got;
            for (char c = 'a'; c <= 'z'; ++c) {
                str cand = order[k].name + str(std::string(1, c));
                if (bucket(cand) == want && !taken.contains(cand)) {
                    got = cand;
                    break;
                }
            }
            if (!got) return std::nullopt;
            nm = *got;
            bool seen = false;
            for (auto& r : new_)
                if (r.first == order[k].name) {
                    r.second = nm;
                    seen = true;
                }
            if (!seen) new_.push_back({order[k].name, nm});
        }
        prev = bucket(nm);
    }
    if (new_.empty() && drop.empty() && resize.empty() && added.empty()) return std::nullopt;
    str bodyv = body;
    if (!added.empty()) {   // (the fillers made: declared after the others)
        Lines bl = split_nl(bodyv);
        int64_t j = 1;
        while (j < bl.size() && (!nb(bl[j]) || py::re::fullmatch(R"re(\s+(?:register\s+)?[\w\s*]+?\w+(?:\[\d+\])?;)re"_s, bl[j]))) ++j;
        str ind_ = (bl.size() > 1 && nb(bl[1])) ? lead(bl[1]) : "    "_s;
        Lines ins;
        for (const Decl& f_ : added) ins.append(py::percent("%schar %s[%d];"_s, ind_, f_.name, f_.size));
        bl.set_slice(j, j, ins);
        bodyv = join_nl(bl);
    }
    for (const std::string& f : drop)
        bodyv = py::re::sub(py::percent(R"re((?m)^[ \t]*char %s\[\d+\];\n)re"_s, str(f)), ""_s, bodyv);
    for (const auto& fz : resize)
        bodyv = py::re::sub(py::percent(R"re((?m)^([ \t]*char %s)\[\d+\];$)re"_s, fz.first),
                            [&](const Match& m) { return py::percent("%s[%d];"_s, m.group(1), fz.second); }, bodyv);
    for (const auto& ab : new_)
        bodyv = py::re::sub(py::percent(R"re(\b%s\b)re"_s, ab.first), ab.second, bodyv);
    return head + "{"_s + bodyv;
}

// the pointers a function's names are, from those it declares: a name compared with a pointer, given one's
// value, or giving its value to one is one too (a pointer compares without its sign: the code shows which)
str pointer_flow(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return text;
    const str decl = R"re((?m)^([ \t]*)(register\s+)?int\s+(\w+);$)re"_s;
    py::set<str> ptr;
    for (const str& p : py::re::findall(R"re((?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);)re"_s, text)) ptr.add(p);
    if (ptr.empty()) return text;
    Lines ints;
    for (const Match& m : py::re::finditer(decl, text))
        if (!ints.contains(m.group(3))) ints.append(m.group(3));
    bool changed = true;
    while (changed) {
        changed = false;
        Lines cands;
        for (const str& x : ints)
            if (!ptr.contains(x)) cands.append(x);
        for (const str& x : cands) {
            std::vector<str> ps;
            for (const str& p : ptr) ps.push_back(p);
            for (const str& p : ps) {
                const str st = R"re((?:\+\+|--)?)re"_s;
                if (py::re::search(py::percent(R"re(\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\)|\(\(?%s%s (?:==|!=|<|>|<=|>=) %s%s\))re"_s, st, x, st, p,
                                               st, p, st, x), body) ||
                    py::re::search(py::percent(R"re((?m)^[ \t]*%s = %s;|^\s*%s = %s;)re"_s, x, p, p, x), body)) {   // (the value itself: not a field of it)
                    ptr.add(x);
                    changed = true;
                    break;
                }
            }
        }
    }
    auto new_ = [&](const Match& m) -> str {
        return ptr.contains(m.group(3)) ? py::percent("%s%schar *%s;"_s, m.group(1), gopt(m, 2), m.group(3)) : m.group(0);
    };
    return py::re::sub(decl, new_, head) + sep + py::re::sub(decl, new_, body);
}

// the text with one of its int parameters a char (each, at most `most`): one compared with a byte, or given
// one, may be a char the caller widened (a variant: the compiler decides)
Lines char_param_variants(const str& text, int64_t most) {
    auto [head, sep, body] = split_body(text);
    Lines out;
    py::list<Match> ms = py::re::finditer(R"re((?m)^([ \t]*)int ([\w, ]+);$)re"_s, head);
    for (int64_t mi = 0; mi < std::min<int64_t>(ms.size(), 1); ++mi) {
        const Match& m = ms[mi];
        Lines names;
        for (const str& x : py::split(m.group(2), ","_s)) names.append(x.strip());
        for (int64_t i = 0; i < std::min<int64_t>(most, names.size()); ++i) {
            const str& p = names[i];
            if (!py::re::search(py::percent(R"re(\b%s\b)re"_s, p), body)) continue;
            Lines rest;
            for (const str& x : names)
                if (x != p) rest.append(x);
            str nw = (!rest.empty() ? m.group(1) + "int "_s + py::join(", "_s, rest) + ";\n"_s : ""_s) + m.group(1) +
                     py::percent("char %s;"_s, p);
            out.append(head.slice(0, m.start()) + nw + head.slice_from(m.end()) + sep + body);
        }
        Lines used;
        for (const str& p : names)
            if (py::re::search(py::percent(R"re(\b%s\b)re"_s, p), body)) used.append(p);
        if (used.size() > 1) {   // (and all of them chars)
            Lines first = used.slice(0, most), rest;
            for (const str& x : names)
                if (!first.contains(x)) rest.append(x);
            str nw = (!rest.empty() ? m.group(1) + "int "_s + py::join(", "_s, rest) + ";\n"_s : ""_s) + m.group(1) +
                     py::percent("char %s;"_s, py::join(", "_s, first));
            out.append(head.slice(0, m.start()) + nw + head.slice_from(m.end()) + sep + body);
        }
    }
    return out;
}

// the text with its register pointers (`register char *p;`, at most `most` of them) made int * - each, then
// all: what the model read through them may be words (the text alone does not say)
Lines pointer_widths(const str& text, int64_t most) {
    Lines all = py::re::findall(R"re((?m)^[ \t]*register char \*(\w+);)re"_s, text);
    Lines ps = all.slice(0, most);
    Lines out;
    auto words = [&](str t, const str& p) -> str {   // (its steps in bytes now in words)
        t = py::re::sub(py::percent(R"re((?m)^([ \t]*)register char \*%s;)re"_s, p), R"re(\1register int *%s;)re"_s.replace("%s"_s, p), t);
        return py::re::sub(py::percent(R"re(\b%s = %s ([+-]) (\d+);)re"_s, p, p), [&](const Match& m) -> str {
            int64_t n = gi(m, 2);
            return n % 2 == 0 ? py::percent("%s = %s %s %d;"_s, p, p, m.group(1), n / 2) : m.group(0);
        }, t);
    };
    for (const str& p : ps) out.append(words(text, p));
    if (ps.size() > 1) {
        str t = text;
        for (const str& p : ps) t = words(t, p);
        out.append(t);
    }
    return out;
}

// X--; (X++; X = X - 1;) then a test of X: the step in the test - if (--X ..) - a variant: the compiler
// tests the flags of the step there, not a compare
std::optional<str> pre_steps(const str& text) {
    OLines lines = opt_of(split_nl(text));
    bool changed = false;
    int64_t k = 0;
    while (k + 1 < lines.size()) {
        OMatch m = py::re::match(R"re(\s*(\w+(?:->f\d+)?)(\+\+|--);\s*$)re"_s, *lines[k]);
        if (!m) m = py::re::match(R"re(\s*(\w+) = \1 ([+-]) [12];\s*$)re"_s, *lines[k]);
        // (by 2: a pointer to words stepped once - declared so where the compiler asks)
        if (m) {
            str x = m->group(1);
            str op = m->group(2);
            op = (op == "++" || op == "+") ? "++"_s : "--"_s;
            OMatch t = py::re::match(py::percent(R"re((\s*(?:if|while) \(\(?)(%s)\b(.*)$)re"_s, esc(x)), *lines[k + 1]);
            if (t) {
                lines[k + 1] = t->group(1) + op + x + t->group(3);
                lines[k] = std::nullopt;
                changed = true;
                k += 2;
                continue;
            }
            const str& nxt = *lines[k + 1];   // (or read once by the next statement: --x in place)
            if (py::re::findall(py::percent(R"re(\b%s\b)re"_s, esc(x)), nxt).size() == 1 &&
                !py::re::match(py::percent(R"re(\s*%s\s*=)re"_s, esc(x)), nxt) &&
                !py::re::match(R"re(\s*(?:\}|else\b|L_\w+:))re"_s, nxt)) {
                lines[k + 1] = py::re::sub(py::percent(R"re(\b%s\b)re"_s, esc(x)), op + x, nxt, 1);
                lines[k] = std::nullopt;
                changed = true;
                k += 2;
                continue;
            }
        }
        k += 1;
    }
    if (!changed) return std::nullopt;
    return join_opt(lines);
}

// a number a pointer is compared with, or given (`p < 14708`, `p = 14708`): the place of data there - an array
// at that offset, g_XXXX (a variant each: the compiler decides; a pointer compares without its sign)
py::list<std::tuple<str, str>> address_variants(const str& text, int64_t dsize, int64_t most, int64_t lo) {
    py::set<str> ptrs;
    for (const str& p :
         py::re::findall(R"re((?m)^[ \t]*(?:register\s+)?(?:char|int|struct \w+)\s*\*\s*(\w+);)re"_s, text))
        ptrs.add(p);
    py::list<std::tuple<str, str>> out;
    if (ptrs.empty()) return out;
    Lines alts;
    for (const str& p : ptrs) alts.append(esc(p));
    std::vector<std::tuple<int64_t, str>> nums;
    for (const Match& m : py::re::finditer(
             py::percent(R"re(\b(%s)\s*(?:[<>]=?|==|!=|=)\s*(-?\d{3,5})\b)re"_s, py::join("|"_s, alts)), text)) {
        int64_t n = gi(m, 2) & 0xFFFF;   // (a place above 32K the model writes negative)
        if (lo <= n && n < dsize && std::find(nums.begin(), nums.end(), std::make_tuple(n, m.group(2))) == nums.end())
            nums.push_back({n, m.group(2)});
    }
    for (size_t i = 0; i < nums.size() && static_cast<int64_t>(i) < most; ++i) {
        auto [n, lit] = nums[i];
        str name = py::percent("g_%04x"_s, n);
        out.append({name, py::re::sub(py::percent(R"re((?<![\w.])%s(?![\w.]))re"_s, esc(lit)), name, text)});
    }
    return out;
}

// (E << 1)->fN, N a place in the data (not a field's small offset): an array of words there indexed by E -
// g_XXXX[E] (the model reads `mov bx,E / shl bx,1 / [bx+N]` as a field of a pointer)
str word_arrays(const str& text_in, int64_t lo) {
    str text = text_in;
    const str rx4 =
        R"re(\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b|\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b)re"_s;
    std::map<int64_t, std::set<int64_t>> at4;   // (the places read through an index times 4: the lines)
    Lines ls_ = split_nl(text);
    for (int64_t k_ = 0; k_ < ls_.size(); ++k_)
        for (const Match& m_ : py::re::finditer(rx4, ls_[k_])) {
            str g2 = m_.group(2);
            at4[py::int_from_str(g2.empty() ? m_.group(4) : g2)].insert(k_);
        }
    auto halves = [&](int64_t x, int64_t y) {
        return x == y || (std::abs(x - y) == 1 && py::re::match(R"re(\s*(?:ax|bx|cx|dx) = )re"_s, ls_[x]) &&
                          py::re::match(R"re(\s*(?:ax|bx|cx|dx) = )re"_s, ls_[y]));
    };
    auto near_ = [&](int64_t a, int64_t b) {   // (a long's two words together)
        auto ia = at4.find(a), ib = at4.find(b);
        if (ia == at4.end() || ib == at4.end()) return false;
        for (int64_t x : ia->second)
            for (int64_t y : ib->second)
                if (halves(x, y)) return true;
        return false;
    };
    std::set<int64_t> pair4;
    for (const auto& kv : at4) {
        int64_t n = kv.first;
        if ((at4.count(n + 2) && !near_(n, n + 2)) || (at4.count(n - 2) && !near_(n, n - 2))) pair4.insert(n);
    }
    // (an index doubled twice: an array of longs there - two words 2 apart read on their own: of 4-byte structs)
    auto longs = [&](const str& subject) {
        return [&, subject](const Match& m) -> str {
            int64_t n = gi(m, 2);
            if (n < lo) return m.group(0);
            str name = py::percent("g_%04x"_s, n);
            int64_t a = subject.rfind("\n"_s, 0, m.start()) + 1;
            str tail = subject.slice_from(m.end());
            int64_t b = tail.contains("\n"_s) ? subject.find("\n"_s, m.end()) : subject.size();
            str ln_ = subject.slice(a, b);
            bool word = py::re::fullmatch(py::percent(R"re(\s*(?:si|di|bx|cx) = %s;)re"_s, esc(m.group(0))), ln_).has_value() ||
                        (py::re::search(py::percent(R"re(\w\((?:[^()]*, )?%s(?:, [^()]*)?\))re"_s, esc(m.group(0))), ln_) &&
                         !py::re::search(R"re(\bLONG\()re"_s, ln_));
            if (pair4.count(n) || word) {   // (one word of it loaded into a pointer register, or pushed alone as an argument)
                text::STRUCTARR.setdefault(name, std::make_tuple(int64_t(4), std::variant<str, py::list<std::tuple<int64_t, str>>>(str("int"))));
                text::ARRAYS.set(name, py::percent("struct s_%s"_s, name));
                return py::percent("%s[%s].f0"_s, name, m.group(1));
            }
            text::ARRAYS.set(name, "long"_s);
            return py::percent("%s[%s]"_s, name, m.group(1));
        };
    };
    {
        str cur = text;
        text = py::re::sub(R"re(\(\(([A-Za-z_]\w*(?:->f\d+)*) << 1\) << 1\)->f(\d+)\b)re"_s, longs(cur), cur);
        cur = text;
        text = py::re::sub(R"re(\(([A-Za-z_]\w*(?:->f\d+)*) << 2\)->f(\d+)\b)re"_s, longs(cur), cur);
    }
    const str rx =
        R"re(\(((?:(?:\+\+|--)?[A-Za-z_]\w*(?:->f\d+)*(?:\+\+|--)?|\d+)(?:\s*[-+]\s*\d+)?) << 1\)->f(\d+)\b)re"_s;
    auto one = [&](const Match& m) -> str {
        int64_t n = gi(m, 2);
        if (n < lo) return m.group(0);
        str name = py::percent("g_%04x"_s, n);
        text::ARRAYS.set(name, "int"_s);
        return py::percent("%s[%s]"_s, name, m.group(1));
    };
    str t = py::re::sub(rx, one, text);
    auto paren = [&](const Match& m) -> str {   // (an expression in parentheses the index: itself)
        int64_t n = gi(m, 2);
        if (n < lo) return m.group(0);
        str name = py::percent("g_%04x"_s, n);
        text::ARRAYS.set(name, "int"_s);
        return py::percent("%s[%s]"_s, name, m.group(1).slice(1, -1));
    };
    t = py::re::sub(R"re(\((\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\)) << 1\)->f(\d+)\b)re"_s, paren, t);
    auto byte_index = [&](const Match& m) -> str {   // (a scratch register's value plus a place in the data: an array of bytes there, the value its index)
        int64_t n = gi(m, 2);   // (n < 0x100: left)
        if (n < 0x100) return m.group(0);
        str name = py::percent("g_%04x"_s, n);
        text::ARRAYS.set(name, text::BYTEPTR.contains(std::make_tuple("data"_s, n)) ? "char"_s : "int"_s);
        return py::percent("%s[%s]"_s, name, m.group(1));
    };
    t = py::re::sub(R"re(\b(ax|bx|cx|dx)->f(\d+)\b)re"_s, byte_index, t);
    {
        py::set<str> gs;
        for (const str& g : py::re::findall(R"re(\bg_([0-9a-f]+)\[)re"_s, t)) gs.add(g);   // (an element of the data the model named: an array)
        for (const str& g : gs) {
            int64_t gv = py::int_parse(g, 16);
            if (gv >= 0x40) {
                str nm = py::percent("g_%04x"_s, gv);
                if (!text::ARRAYS.contains(nm))
                    text::ARRAYS.set(nm, (text::BYTEPTR.contains(std::make_tuple("data"_s, gv)) || text::BYTEPTR.contains(std::make_tuple("*"_s, gv)))
                                             ? "char"_s : "int"_s);
            }
        }
    }
    t = py::re::sub(R"re(\(((?:\+\+|--)?[A-Za-z_]\w*(?:\+\+|--)?)\)->f(\d+)\b)re"_s, byte_index, t);   // (a variable's value - stepped too - the index)
    // (a register given K * E, then an element at a place in the data through it: an array of structs of K
    //  bytes - g_N[E].f0, the struct laid out in localize)
    auto struct_index = [&](const Match& m) -> str {
        str ind = m.group(1), e = m.group(3), rest = m.group(4);
        int64_t k = gi(m, 2);
        OMatch g = py::re::search(R"re(\b(g_[0-9a-f]{4})\[bx\])re"_s, rest);
        if (!g || k < 2 || k > 0x400) return m.group(0);
        str g1 = g->group(1);
        text::STRUCTARR.set(g1, std::make_tuple(k, std::variant<str, py::list<std::tuple<int64_t, str>>>(text::ARRAYS.get(g1, "int"_s))));
        text::ARRAYS.set(g1, py::percent("struct s_%s"_s, g1));
        return ind + replace1(rest, py::percent("%s[bx]"_s, g1), py::percent("%s[%s].f0"_s, g1, e));
    };
    t = py::re::sub(R"re((?m)^([ \t]*)bx = \((\d+) \* (.+?)\);\n[ \t]*(.*\bg_[0-9a-f]{4}\[bx\].*)$)re"_s, struct_index, t);
    auto struct_place = [&](const Match& m) -> str {   // (K * E + N, N a place in the data: the address of the element E of an array of K-byte structs)
        int64_t k = gi(m, 1), n = gi(m, 3);
        str e = m.group(2);
        if (n < 0x100 || k < 3 || k > 0x400) return m.group(0);
        str name = py::percent("g_%04x"_s, n);
        text::STRUCTARR.setdefault(name, std::make_tuple(k, std::variant<str, py::list<std::tuple<int64_t, str>>>(str("int"))));
        text::ARRAYS.set(name, py::percent("struct s_%s"_s, name));
        return py::percent("&%s[%s]"_s, name, e);
    };
    t = py::re::sub(R"re(\(\((\d+) \* ([\w\->.]+)\) \+ (\d+)\)(?!->))re"_s, struct_place, t);

    OLines lso = opt_of(split_nl(t));   // (a register given K * E read only at places in the data through it until given again: the product there)
    for (int64_t k_ = 0; k_ < lso.size(); ++k_) {
        if (!lso[k_]) continue;
        OMatch m_ = py::re::fullmatch(R"re(([ \t]*)(bx|si|di) = \((\d+) \* ([A-Za-z_]\w*)\);)re"_s, *lso[k_]);
        if (!m_ || gi(*m_, 3) < 2) continue;
        str r_ = m_->group(2), e_ = m_->group(4);
        std::vector<int64_t> uses;
        int64_t j_ = k_ + 1;
        while (j_ < lso.size()) {
            const str& y_ = *lso[j_];
            if (py::re::match(py::percent(R"re(\s*(?:%s|%s) = |\s*(?:L_\w+:|\}|case\b|default:))re"_s, r_, esc(e_)), y_) ||
                py::re::search(py::percent(R"re(\b%s\s*(?:\+\+|--))re"_s, esc(e_)), y_))
                break;
            if (py::re::search(py::percent(R"re(\b%s\b)re"_s, r_), y_)) uses.push_back(j_);
            ++j_;
        }
        if (r_ == "bx") {   // (an element of words through it: the place there)
            std::vector<str> uses_;
            for (int64_t u : uses)
                uses_.push_back(py::re::sub(R"re(\bg_([0-9a-f]{4})\[bx\])re"_s, [](const Match& q) -> str {
                    return py::percent("bx->f%d"_s, py::int_parse(q.group(1), 16));
                }, *lso[u]));
            bool all_ = true;
            for (const str& x : uses_)
                if (py::re::search(R"re(\bbx\b(?!->f(?:\d{3,})))re"_s, x)) all_ = false;
            if (all_)
                for (size_t i = 0; i < uses.size(); ++i) lso[uses[i]] = uses_[i];
        }
        if (uses.empty()) continue;
        bool skip = false;
        for (int64_t u : uses)
            if (py::re::search(py::percent(R"re(\b%s\b(?!->f(?:\d{3,})))re"_s, r_), *lso[u])) skip = true;
        if (skip) continue;
        for (int64_t u : uses)
            for (const str& n_ : py::re::findall(py::percent(R"re(\b%s->f(\d+))re"_s, r_), *lso[u]))
                if (py::int_from_str(n_) < 0x100) skip = true;
        if (skip || _live_after(lso, uses.back(), r_)) continue;
        for (int64_t u : uses)
            lso[u] = py::re::sub(py::percent(R"re(\b%s->f(\d+))re"_s, r_), [&](const Match& q) -> str {
                return py::percent("(%s * %s)->f%s"_s, m_->group(3), e_, q.group(1));
            }, *lso[u]);
        lso[k_] = std::nullopt;
    }
    t = join_opt(lso);

    auto struct_product = [&](const Match& m) -> str {   // (the product folded in place: (K * E)->fN)
        int64_t k = gi(m, 1), n = gi(m, 3);
        str e = m.group(2);
        if (n < 0x100 || k < 2 || k > 0x400) return m.group(0);
        str name = py::percent("g_%04x"_s, n);
        text::STRUCTARR.setdefault(name, std::make_tuple(k, std::variant<str, py::list<std::tuple<int64_t, str>>>(
                                                                text::BYTEPTR.contains(std::make_tuple("data"_s, n)) ? "char"_s : "int"_s)));
        text::ARRAYS.set(name, py::percent("struct s_%s"_s, name));
        return py::percent("%s[%s].f0"_s, name, e);
    };
    t = py::re::sub(R"re(\((\d+) \* ([A-Za-z_][\w\->]*)\)->f(\d+)\b)re"_s, struct_product, t);
    // (the compiler keeps bx on: later elements through it, until it is given again - and the product itself
    //  read as a pointer to a field, (K * E)->fN, an element at N)
    Lines lines = split_nl(t);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch prod = py::re::search(R"re(\b(g_[0-9a-f]{4})\[(.+?)\]\.f0)re"_s, lines[k]);
        if (!prod || !text::STRUCTARR.contains(prod->group(1))) continue;
        int64_t kk = std::get<0>(text::STRUCTARR.at(prod->group(1)));
        str e = prod->group(2);
        for (int64_t j = k + 1; j < lines.size(); ++j) {
            if (py::re::match(R"re([ \t]*bx = )re"_s, lines[j]) ||
                py::re::search(py::percent(R"re(\b%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)\s*%s\b)re"_s, esc(e), esc(e)), lines[j]))
                break;
            auto elem = [&](const str& g1) -> str {
                std::variant<str, py::list<std::tuple<int64_t, str>>> ty = str("int");
                str cur = text::ARRAYS.get(g1, ""_s);
                if (!cur.startswith("struct"_s)) ty = text::ARRAYS.get(g1, "int"_s);
                text::STRUCTARR.setdefault(g1, std::make_tuple(kk, ty));
                text::ARRAYS.set(g1, py::percent("struct s_%s"_s, g1));
                return py::percent("%s[%s].f0"_s, g1, e);
            };
            lines[j] = py::re::sub(R"re(\b(g_[0-9a-f]{4})\[bx\])re"_s, [&](const Match& m2) { return elem(m2.group(1)); }, lines[j]);
            lines[j] = py::re::sub(py::percent(R"re(\(%d \* %s\)->f(\d+)\b)re"_s, kk, esc(e)),
                                   [&](const Match& m2) { return elem(py::percent("g_%04x"_s, gi(m2, 1))); }, lines[j]);
        }
    }
    t = join_nl(lines);
    // (I << 1)[E]: the model's word index into the words at E - ((int *)(E))[I]
    auto word_index = [&](const Match& m) -> str {   // (an offset in bytes: through a char *)
        str i = m.group(1), e = m.group(2);
        OMatch o = py::re::fullmatch(R"re(([A-Za-z_]\w*) \+ (\d+))re"_s, e);
        return o ? py::percent("((int *)((char *)%s + %s))[%s]"_s, o->group(1), o->group(2), i)
                 : py::percent("((int *)(%s))[%s]"_s, e, i);
    };
    t = py::re::sub(R"re(\(((?:[A-Za-z_][\w\->]*|\d+|\((?:[^()]|\([^()]*\))*\))(?:\s*[-+]\s*\d+)?) << 1\)\[([^\[\]]+)\])re"_s, word_index, t);
    return t;
}

// X = X + n; then *X read once: *++X in its place - X a pointer to things of n bytes (a variant)
std::optional<str> pre_increments(const str& text) {
    OLines lines = opt_of(split_nl(text));
    bool changed = false;
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        OMatch m = py::re::match(R"re(\s*(\w+) = \1 \+ ([12]);\s*$)re"_s, lines[k].value_or(""_s));
        if (!m) continue;
        str x = m->group(1);
        int64_t n = gi(*m, 2);
        std::optional<str> nxt = lines[k + 1];
        if (!nxt || py::re::findall(py::percent(R"re(\b%s\b)re"_s, x), *nxt).size() != 1 ||
            !py::re::search(py::percent(R"re(\*%s\b(?!\s*(?:->|\[|\+\+|--)))re"_s, x), *nxt))
            continue;
        lines[k + 1] = py::re::sub(py::percent(R"re(\*%s\b)re"_s, x), "*++"_s + x, *nxt, 1);
        lines[k] = std::nullopt;
        if (n == 2) {
            text::PTRSIZE.set(x, 2);
            for (int64_t i = 0; i < lines.size(); ++i)
                if (lines[i])
                    lines[i] = py::re::sub(py::percent(R"re(^(\s*register )char \*%s;)re"_s, x),
                                           R"re(\1int *%s;)re"_s.replace("%s"_s, x), *lines[i]);
        }
        changed = true;
    }
    if (!changed) return std::nullopt;
    return join_opt(lines);
}

// a header of the kit, as text ('' when the kit has none)
str header_text(const Prj& prj, const str& header) {
    str root;
    try {
        if (!prj.kit) throw py::KeyError("kit");
        root = prj.kit_dir(*prj.kit);
    } catch (const std::exception&) {
        return ""_s;
    }
    for (const auto& dfs : py::os::walk(root)) {
        const Lines& fs = std::get<2>(dfs);
        for (const str& f : fs)
            if (f.lower() == header.lower()) {
                py::file fh = py::open(py::os::path::join(std::get<0>(dfs), f), "r"_s, std::optional<str>("latin1"_s));
                return fh.read_text();
            }
    }
    return ""_s;
}

// the macros of a header that read a table of the library - ctype.h's isdigit(c) .. ( (_ctype_+1)[c] & _DIGIT ) -
// read back where the model has the table's place: (c->fN & M), N the table's place + its offset, M a macro's
// mask (the profile's "table_macros": [{"header": "ctype.h"}]: the macros and masks are the kit's header's, the
// table's place the library stage's)
str table_macros(const Prj& prj, str text, const py::list<TableMacro>& specs) {
    for (const TableMacro& sp : specs) {
        std::tuple<str, str> key{prj.dir, sp.header};
        if (!text::_TABLES.contains(key)) {
            str h = header_text(prj, sp.header);
            py::dict<str, int64_t> masks;
            for (const Match& m : py::re::finditer(R"re(#define\s+(_[A-Z]+)\s+(0x[0-9a-fA-F]+|\d+))re"_s, h))
                masks.set(m.group(1), py::int_parse(m.group(2), 0));
            py::list<std::tuple<str, str, int64_t, int64_t>> macs;
            for (const Match& m :
                 py::re::finditer(R"re(#define\s+(\w+)\((\w)\)\s*\(\s*\((\w+)\+(\d+)\)\[\2\]\s*&\s*([^)]*\)?)\s*\))re"_s, h)) {
                str name = m.group(1), tab = m.group(3), expr = m.group(5);
                Lines bits = py::re::findall(R"re(_[A-Z]+)re"_s, expr);
                bool all_ = !bits.empty();
                for (const str& b : bits)
                    if (!masks.contains(b)) all_ = false;
                if (all_) {
                    int64_t mk = 0;
                    for (const str& b : bits) mk |= masks.at(b);
                    macs.append({name, tab, gi(m, 4), mk});
                }
            }
            if (!text::_LIBDATA.contains(prj.dir)) {
                py::list<DataMacro> dummy;
                DataMacro dm;
                dm.base = "-"_s;
                dm.size = 1;
                dummy.append(dm);
                data_macros(prj, ""_s, dummy);
            }
            text::_TABLES.set(key, macs);
        }
        bool hit = false;
        for (const auto& nt : text::_TABLES.at(key)) {
            str name = std::get<0>(nt), tab = std::get<1>(nt);
            int64_t off = std::get<2>(nt), mask = std::get<3>(nt);
            std::optional<int64_t> base;
            if (text::_LIBDATA.contains(prj.dir)) {
                const auto& d = text::_LIBDATA.at(prj.dir);
                std::optional<int64_t> v1, v2;
                if (d.contains("_"_s + tab)) v1 = d.at("_"_s + tab);
                if (d.contains(tab)) v2 = d.at(tab);
                base = (v1 && *v1 != 0) ? v1 : v2;
            }
            if (!base) continue;
            int64_t n = *base + off;
            std::vector<std::pair<str, str>> forms = {
                {py::percent(R"re(\(\((\w+)->f%d & %d\) != 0\))re"_s, n, mask), name + "("_s},
                {py::percent(R"re(\(\((\w+)->f%d & %d\) == 0\))re"_s, n, mask), "!"_s + name + "("_s},
                {py::percent(R"re(\((\w+)->f%d & %d\))re"_s, n, mask), name + "("_s}};
            for (const auto& ff : forms) {
                str nw = py::re::sub(ff.first, [&](const Match& m) { return ff.second + m.group(1) + ")"_s; }, text);
                if (nw != text) {
                    hit = true;
                    text = nw;
                }
            }
        }
        if (hit) {
            text::FILEPTR.setdefault("#include"_s, std::optional<str>(sp.header));
            text::FILEPTR.set("#include2"_s, std::optional<str>(sp.header));
        }
    }
    return text;
}

// the functions and macros a header of the kit declares (stdio.h's fopen, freopen, getc ..): those come from
// the header, the draft does not declare them again
py::set<str> header_names(const Prj& prj, const str& header) {
    std::tuple<str, str> key{prj.toolchain_name, header.lower()};
    if (!text::_HEADERS.contains(key)) {
        py::set<str> names;
        try {
            if (!prj.kit) throw py::KeyError("kit");
            str root = prj.kit_dir(*prj.kit);
            for (const auto& dfs : py::os::walk(root)) {
                for (const str& f : std::get<2>(dfs))
                    if (f.lower() == header.lower()) {
                        py::file fh = py::open(py::os::path::join(std::get<0>(dfs), f), "r"_s, std::optional<str>("latin1"_s));
                        str t = fh.read_text();
                        names.update(py::re::findall(R"re(\b([A-Za-z_]\w*)\s*\(\s*\)\s*[;,])re"_s, t));
                        names.update(py::re::findall(R"re(#define\s+([A-Za-z_]\w*)\s*\()re"_s, t));
                        break;
                    }
                if (!names.empty()) break;
            }
        } catch (const std::exception&) {
        }
        text::_HEADERS.set(key, names);
    }
    return text::_HEADERS.at(key);
}

// a library's data a macro names - stdio's streams: stdin, stdout, stderr .. the elements of _iob, fileno(f)
// a byte of one - read back where the model has their places (g_XXXX, or a number passed: a stream's place)
// (the profile's "data_macros": [{"base": "__iob", "size": 8, "names": [..], "fields": {"7": "fileno(%s)"},
// "include": "stdio.h"}]; the base's place from the library stage)
str data_macros(const Prj& prj, str text, const py::list<DataMacro>& specs) {
    if (specs.empty()) return text;
    if (!text::_LIBDATA.contains(prj.dir)) {
        try {
            text::_LIBDATA.set(prj.dir, prj.library_places());
        } catch (const std::exception&) {
            text::_LIBDATA.set(prj.dir, py::dict<str, int64_t>());
        }
    }
    py::dict<str, int64_t> places = text::_LIBDATA.at(prj.dir);
    for (const DataMacro& sp : specs) {
        if (!places.contains(sp.base)) continue;
        int64_t base = places.at(sp.base);
        int64_t n = sp.names.size(), size = sp.size;
        bool hit = false;
        text = py::re::sub(R"re(\bg_([0-9a-f]{4})\b)re"_s, [&](const Match& m) -> str {
            int64_t a = py::int_parse(m.group(1), 16);
            int64_t k = py::floordiv(a - base, size), off = py::mod(a - base, size);
            if (0 <= k && k < n && sp.fields.contains(py::to_str(off))) {
                hit = true;
                return py::percent(sp.fields.at(py::to_str(off)), sp.names[k]);
            }
            return m.group(0);
        }, text);
        // (a number passed that is a stream's place)
        text = py::re::sub(R"re(([(,]\s*)(\d{3,5})(?=\s*[,)]))re"_s, [&](const Match& m) -> str {
            int64_t a = gi(m, 2);
            int64_t k = py::floordiv(a - base, size), off = py::mod(a - base, size);
            if (0 <= k && k < n && off == 0) {
                hit = true;
                return m.group(1) + sp.names[k];
            }
            return m.group(0);
        }, text);
        if (hit && sp.include && !sp.include->empty()) text::FILEPTR.set("#include"_s, sp.include);
    }
    return text;
}

// the locals declared in the function's body that nothing in it names any more, left out
str unused_locals(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return text;
    Lines lines = split_nl(body);
    std::set<int64_t> drop;
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::match(
            R"re(\s*(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)\s*\**\s*(\w+)\s*(?:\[\d+\])?;\s*$)re"_s, lines[k]);
        if (m && !py::re::search(py::percent(R"re(\b%s\b)re"_s, m->group(1)), join_nl(lines.slice(0, k) + lines.slice_from(k + 1))))
            drop.insert(k);
    }
    Lines keep;
    for (int64_t k = 0; k < lines.size(); ++k)
        if (!drop.count(k)) keep.append(lines[k]);
    return head + sep + join_nl(keep);
}

// a library macro the compiler expanded in line - stdio's getc(f) and putc(c, f): the count of the stream's
// buffer stepped, then its pointer, else the routine that fills or empties the buffer (the profile's "macros":
// [{"call": routine, "as": "getc(%s)", "type": "FILE", "include": "stdio.h"}]) - read back as the macro: from
// the line that steps the stream's count to the routine's call, the region is the macro's value
str library_macros(str text, const py::list<LibMacro>& macros) {
    if (macros.empty()) return text;
    // "as" % tuple(args): each %s the next argument (a count that does not agree is Python's TypeError)
    auto fmt_args = [](const str& f, const Lines& args) -> str {
        str out;
        int64_t ai = 0;
        for (int64_t i = 0; i < f.size(); ++i) {
            char32_t c = f.at_(i);
            if (c == U'%' && i + 1 < f.size()) {
                char32_t d = f.at_(i + 1);
                if (d == U'%') {
                    out = out + "%"_s;
                    ++i;
                    continue;
                }
                if (d == U's') {
                    if (ai >= args.size()) throw py::TypeError("not enough arguments for format string");
                    out = out + args[ai++];
                    ++i;
                    continue;
                }
            }
            out = out + f.slice(i, i + 1);
        }
        if (ai < args.size()) throw py::TypeError("not all arguments converted during string formatting");
        return out;
    };
    for (const LibMacro& mac : macros) {
        const str rx = py::percent(R"re(\b%s\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re"_s, esc(mac.call));
        while (true) {
            Lines lines = split_nl(text);
            std::optional<std::pair<int64_t, Match>> hit;
            for (int64_t k = 0; k < lines.size() && !hit; ++k) {
                OMatch m = py::re::search(rx, lines[k]);
                if (m) hit = std::make_pair(k, *m);
            }
            if (!hit) break;
            int64_t k1 = hit->first;
            const Match& m = hit->second;
            Lines args;
            for (const str& x : _top_commas(m.group(1))) args.append(x.strip());
            str f = args[-1];
            std::optional<int64_t> k0;
            for (int64_t j = k1; j >= 0; --j)
                if (py::re::search(py::percent(R"re(\b%s->f2(?:--| = %s->f2 - 1))re"_s, esc(f), esc(f)), lines[j])) {
                    k0 = j;
                    break;
                }
            if (!k0) break;
            for (int64_t ix : mac.byte_args) {   // (an argument the macro stores as a byte - putc's c: *p of an int *p read a byte at a time)
                OMatch p_ = ix < args.size() ? py::re::fullmatch(R"re(\*(\w+))re"_s, args[ix]) : std::nullopt;
                if (p_ && py::re::search(py::percent(R"re((?m)^[ \t]*(?:register )?int \*%s;$)re"_s, p_->group(1)), text))
                    args[ix] = py::percent("*(char *)%s"_s, p_->group(1));
            }
            str val = fmt_args(mac.as, args);
            int64_t k2 = k1;
            int64_t depth = 0;
            for (int64_t i = *k0; i <= k2; ++i) depth += lines[i].count("{"_s) - lines[i].count("}"_s);
            while (depth > 0 && k2 + 1 < lines.size()) {   // (to the region's own closing brace)
                ++k2;
                depth += lines[k2].count("{"_s) - lines[k2].count("}"_s);
            }
            str ind = lead(lines[*k0]);
            bool r = py::re::match(R"re(\s*return\b)re"_s, lines[k1]).has_value();
            Lines nw;
            nw.append(r ? py::percent("%sreturn %s;"_s, ind, val) : py::percent("%sax = %s;"_s, ind, val));
            if (depth < 0) nw.append(ind + "}"_s);
            lines.set_slice(*k0, k2 + 1, nw);
            text = join_nl(lines);
            text::FILEPTR.set(f, std::optional<str>(mac.type.value_or("FILE"_s)));
            text::FILEPTR.set("#include"_s, mac.include);
        }
    }
    return text;
}

// if (A) return E; return 0;  ->  return A && E;  (a variant: the compiler makes one test chain of the &&)
std::optional<str> and_returns(const str& text) {
    const str rx = R"re((?m)^([ \t]*)if (\(.*\))(?: \{)?\n\s*return (.+);\n(?:\s*\}\n)?\s*return 0;$)re"_s;
    str t = py::re::sub(rx, [](const Match& m) -> str {
        str e = m.group(3);
        if (py::re::search(R"re(\|\|)re"_s, e) && !(e.startswith("("_s) && e.endswith(")"_s))) e = py::percent("(%s)"_s, e);
        return py::percent("%sreturn %s && %s;"_s, m.group(1), _truth(m.group(2)), e);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// a word the model reads as 32768..65535: a decimal constant over 32767 is a long to a 16-bit compiler (the
// compare or the sum goes to 32 bits) - the int it was, negative (65535 is -1)
str word_constants(const str& text) {
    return py::re::sub(
        R"re((?<![\w.])(3276[89]|327[7-9]\d|32[89]\d\d|3[3-9]\d{3}|[4-5]\d{4}|6[0-4]\d{3}|65[0-4]\d\d|655[0-2]\d|6553[0-5])(?![\w.]))re"_s,
        [](const Match& m) { return py::to_str(py::int_from_str(m.group(1)) - 65536); }, text);
}

// the text with one of its int globals unsigned (each): a variable compared, the compiler's code says how
Lines unsigned_globals(const str& text, int64_t most) {
    Lines out;
    OMatch m = py::re::search(R"re((?m)^extern int ([\w, ]+);$)re"_s, text);
    if (!m) return out;
    Lines names;
    for (const str& x : py::split(m->group(1), ","_s)) names.append(x.strip());
    for (int64_t i = 0; i < std::min<int64_t>(most, names.size()); ++i) {
        const str& g = names[i];
        Lines rest;
        for (const str& x : names)
            if (x != g) rest.append(x);
        str nw = (!rest.empty() ? py::percent("extern int %s;\n"_s, py::join(", "_s, rest)) : ""_s) + py::percent("extern unsigned %s;"_s, g);
        out.append(text.slice(0, m->start()) + nw + text.slice_from(m->end()));
    }
    return out;
}

// X = E; then a test of X: the assignment in the test - if ((X = E) == K) - a variant: the compiler
// compares the value it has, not X again
std::optional<str> assign_in_tests(const str& text) {
    OLines lines = opt_of(split_nl(text));
    bool changed = false;
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        OMatch m = py::re::match(R"re((\s*)([A-Za-z_]\w*(?:\[[^\]]*\])?) = ([^;]+);\s*$)re"_s, lines[k].value_or(""_s));
        if (!m || py::re::match(R"re(\s*(?:if|while|return)\b)re"_s, *lines[k])) continue;
        str x = m->group(2), e = m->group(3);
        // (an if's: a loop's test would give it again each time)
        OMatch t = py::re::match(py::percent(R"re((\s*if \(\(?)(%s)\b(.*)$)re"_s, esc(x)), lines[k + 1].value_or(""_s));
        if (t && !py::re::search(py::percent(R"re(\b%s\b)re"_s, esc(x)), e)) {
            lines[k + 1] = py::percent("%s(%s = %s)%s"_s, t->group(1), x, e, t->group(3));
            lines[k] = std::nullopt;
            changed = true;
        }
    }
    if (!changed) return std::nullopt;
    return join_opt(lines);
}

// a parameter the model only reads into a byte register (al = b) is a char: declared so (K&R widens it
// in the call; the function reads its low byte)
str char_params(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return text;
    Lines params0 = py::re::findall(R"re(\b\w+\(([\w, ]*)\))re"_s, head);
    Lines params;
    if (!params0.empty())
        for (const str& p : py::split(params0[-1], ","_s))
            if (nb(p)) params.append(p.strip());
    Lines chars;
    for (const str& p : params)
        if (py::re::search(py::percent(R"re(\b(?:al|bl|cl|dl) = %s;)re"_s, p), body) &&
            !py::re::search(py::percent(R"re(\b(?:ax|bx|cx|dx|si|di) = %s;)re"_s, p), body) &&
            !py::re::search(py::percent(R"re(\b%s\s*(?:->|\[))re"_s, p), body))
            chars.append(p);
    if (chars.empty()) return text;
    Lines lines = split_nl(head);
    for (int64_t j = 0; j < lines.size(); ++j) {
        OMatch d = py::re::match(R"re((\s*)int (.*);$)re"_s, lines[j]);
        if (!d) continue;
        Lines names, keep, ch;
        for (const str& x : py::split(d->group(2), ","_s)) names.append(x.strip());
        for (const str& x : names) (chars.contains(x) ? ch : keep).append(x);
        if (!ch.empty())
            lines[j] = (!keep.empty() ? d->group(1) + "int "_s + py::join(", "_s, keep) + ";\n"_s : ""_s) + d->group(1) + "char "_s +
                       py::join(", "_s, ch) + ";"_s;
    }
    return join_nl(lines) + sep + body;
}

// the gotos back to a loop's top: a move shared at the label (`ax = E; L: for (;;) { r = ax;` - every
// `ax = V; goto L;` too) given in each place (r = E; r = V), a for (;;) left holding one while and a break that
// while, and a goto to the top of the loop it is in a continue
str loop_gotos(str text) {
    for (int rep = 0; rep < 4; ++rep) {
        Lines lines = split_nl(text);
        bool done = false;
        for (int64_t k = 0; k < lines.size() && !done; ++k) {
            OMatch m = py::re::match(R"re(([ \t]*)(L_\w+):$)re"_s, lines[k]);
            if (!m) continue;
            str lab = m->group(2);
            std::vector<int64_t> gos;
            for (int64_t j = 0; j < lines.size(); ++j)
                if (lines[j].strip() == py::percent("goto %s;"_s, lab)) gos.push_back(j);
            // (the move shared at the label: into each place that comes to it)
            int64_t j = k + 1;
            if (j < lines.size() && lines[j].strip() == "for (;;) {"_s) ++j;
            OMatch mv = j < lines.size() ? py::re::match(R"re([ \t]*(\w+) = ax;$)re"_s, lines[j]) : std::nullopt;
            bool allgo = true;
            for (int64_t g : gos)
                if (!(g != 0 && py::re::match(R"re([ \t]*ax = [^;]+;$)re"_s, lines[g - 1]))) allgo = false;
            if (mv && k != 0 && py::re::match(R"re([ \t]*ax = [^;]+;$)re"_s, lines[k - 1]) && !gos.empty() && allgo &&
                !_live_after(opt_of(lines), j, "ax"_s)) {
                str r = mv->group(1);
                std::vector<int64_t> gs{k};
                gs.insert(gs.end(), gos.begin(), gos.end());
                for (int64_t g : gs)
                    lines[g - 1] = py::re::sub(R"re(^([ \t]*)ax = )re"_s, R"re(\1%s = )re"_s.replace("%s"_s, r), lines[g - 1]);
                OLines ol = opt_of(lines);
                ol[j] = std::nullopt;
                text = join_opt(ol);
                done = true;
                break;
            }
            // (a for (;;) holding one while and a break: the while)
            if (j < lines.size() && lines[k + 1].strip() == "for (;;) {"_s) {
                std::optional<int64_t> e = _block_end(lines, k + 1);
                int64_t w = k + 2;
                if (e && *e != 0 && py::re::match(R"re([ \t]*while \(.*\) \{$)re"_s, lines[w])) {
                    std::optional<int64_t> we = _block_end(lines, w);
                    if (we && *we != 0 && *we + 2 == *e && lines[*we + 1].strip() == "break;"_s) {
                        for (int64_t q = w; q < *we + 1; ++q) lines[q] = dedent4(lines[q]);
                        OLines ol = opt_of(lines);
                        ol[k + 1] = std::nullopt;
                        ol[*we + 1] = std::nullopt;
                        ol[*e] = std::nullopt;
                        text = join_opt(ol);
                        done = true;
                        break;
                    }
                }
            }
            // (a goto to the top of the loop it is in: a continue)
            if (k + 1 < lines.size() && py::re::match(R"re([ \t]*(?:while \(.*\)|for \(.*\)) \{$)re"_s, lines[k + 1]) && !gos.empty()) {
                std::optional<int64_t> e = _block_end(lines, k + 1);
                bool inside = e && *e != 0;
                if (inside)
                    for (int64_t g : gos)
                        if (!(k + 1 < g && g < *e)) inside = false;
                if (inside) {
                    bool ok = true;
                    for (int64_t g : gos)   // (not inside a loop of its own)
                        for (int64_t q = k + 2; q < g; ++q) {
                            if (py::re::match(R"re([ \t]*(?:while \(.*\)|for \(.*\)|do) \{$)re"_s, lines[q])) {
                                std::optional<int64_t> qe = _block_end(lines, q);
                                if (qe && *qe != 0 && q < g && g < *qe) ok = false;
                            }
                        }
                    if (ok) {
                        for (int64_t g : gos) lines[g] = lines[g].replace(py::percent("goto %s;"_s, lab), "continue;"_s);
                        OLines ol = opt_of(lines);
                        ol[k] = std::nullopt;
                        text = join_opt(ol);
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

// a switch's default arm, written first, moved to the end (a variant: the compiler puts the default's code
// after the tests wherever it is written, so the code alone does not say where)
std::optional<str> default_last(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re((\s*)switch \(.*\) \{)re"_s, lines[k]);
        if (!m || k + 1 >= lines.size() || lines[k + 1] != m->group(1) + "default:"_s) continue;
        str ind = m->group(1);
        str pat = py::percent(R"re(%s(?:case [^:]+|default):)re"_s, esc(ind));
        std::optional<int64_t> nxt, end;
        for (int64_t j = k + 2; j < lines.size(); ++j)
            if (py::re::fullmatch(pat, lines[j])) { nxt = j; break; }
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!nxt || !end || *nxt > *end) continue;
        Lines arm = lines.slice(k + 1, *nxt);
        Lines nonblank;
        for (const str& x : arm)
            if (nb(x)) nonblank.append(x);
        if (!py::re::match(R"re(\s*(?:break;|return\b|goto\b))re"_s, nonblank[-1])) continue;   // (it falls into the next: not movable)
        return join_nl(lines.slice(0, k + 1) + lines.slice(*nxt, *end) + arm + lines.slice_from(*end));
    }
    return std::nullopt;
}

// a switch's arms that only break (case K: break;) moved to its top (a variant: such an arm has no code of
// its own - where it is written decides the layout of the others)
std::optional<str> empty_cases_first(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re((\s*)switch \(.*\) \{)re"_s, lines[k]);
        if (!m) continue;
        str ind = m->group(1);
        std::optional<int64_t> end;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (lines[j] == ind + "}"_s) { end = j; break; }
        if (!end) continue;
        Lines moved, rest;
        int64_t j = k + 1;
        str pat = py::percent(R"re(%scase [^:]+:)re"_s, esc(ind));
        while (j < *end) {
            if (py::re::fullmatch(pat, lines[j]) && j + 1 < *end && lines[j + 1].strip() == "break;"_s &&
                (j == k + 1 || py::re::match(R"re(\s*(?:break;|return\b|goto\b|continue;))re"_s, lines[j - 1]) ||
                 lines[j - 1].strip().endswith("}"_s)) &&
                j > k + 1) {
                moved.extend(lines.slice(j, j + 2));
                j += 2;
                continue;
            }
            rest.append(lines[j]);
            ++j;
        }
        if (!moved.empty()) return join_nl(lines.slice(0, k + 1) + moved + rest + lines.slice_from(*end));
    }
    return std::nullopt;
}

// for (;;) { if (A) { L: S; continue; } if (B) { goto L; } else { break; } } - a test or-ed with the first:
// while (A || B) { S } (a variant)
std::optional<str> or_loops(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re(([ \t]*)for \(;;\) \{)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!e || lines[*e].strip() != "}"_s) continue;
        str ind = m->group(1);
        str i4 = ind + "    "_s;
        auto p1 = _if_parts(lines, k + 1);
        if (!p1 || std::get<2>(*p1)) continue;
        const Lines& arm = std::get<1>(*p1);
        OMatch lab = !arm.empty() ? py::re::fullmatch(R"re([ \t]*(L_\w+):)re"_s, arm[0]) : std::nullopt;
        if (!lab || arm[-1].strip() != "continue;"_s) continue;
        auto p2 = _if_parts(lines, std::get<3>(*p1));
        if (!p2 || std::get<3>(*p2) != *e) continue;
        Lines g1, g2;
        for (const str& x : std::get<1>(*p2))
            if (nb(x)) g1.append(x.strip());
        if (std::get<2>(*p2))
            for (const str& x : *std::get<2>(*p2))
                if (nb(x)) g2.append(x.strip());
        if (g1 != Lines{py::percent("goto %s;"_s, lab->group(1))} || g2 != Lines{"break;"_s}) continue;
        if (py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab->group(1)), join_nl(lines.slice(0, k) + lines.slice_from(*e + 1)))) continue;
        str a = _truth(py::percent("(%s)"_s, std::get<0>(*p1))), b = _truth(py::percent("(%s)"_s, std::get<0>(*p2)));
        Lines body;
        for (int64_t i = 1; i < arm.size() - 1; ++i) body.append(arm[i].startswith(i4 + "    "_s) ? arm[i].slice_from(4) : arm[i]);
        Lines nw;
        nw.append(py::percent("%swhile (%s || %s) {"_s, ind, a, b));
        nw.extend(body);
        nw.append(py::percent("%s}"_s, ind));
        return join_nl(lines.slice(0, k) + nw + lines.slice_from(*e + 1));
    }
    return std::nullopt;
}

// ((A op B) ? 1 : 0) - a comparison's value: (A op B) itself, ? 0 : 1 its negation, and a call's side on the
// right (B mirror-op A) when the call was A (a variant: the compiler calls first whichever side it is on)
std::optional<str> compare_results(const str& text) {
    static const std::map<std::string, std::string> mirror = {{"<", ">"}, {">", "<"}, {"<=", ">="}, {">=", "<="}, {"==", "=="}, {"!=", "!="}};
    static const std::map<std::string, std::string> neg = {{"<", ">="}, {">=", "<"}, {">", "<="}, {"<=", ">"}, {"==", "!="}, {"!=", "=="}};
    str t = py::re::sub(R"re(\(\(([^?:\n]+?) (==|!=|<=|>=|<|>) ([^?:\n]+?)\) \? ([01]) : ([01])\))re"_s, [&](const Match& m) -> str {
        str a = m.group(1), op = m.group(2), b = m.group(3), tt = m.group(4), f = m.group(5);
        if (!(_balanced(a) && _balanced(b)) || tt == f) return m.group(0);
        if (tt == "0") op = str(neg.at(op.utf8()));
        if (py::re::search(R"re(\w\()re"_s, a) && !py::re::search(R"re(\w\()re"_s, b)) {
            str x = a;
            a = b;
            b = x;
            op = str(mirror.at(op.utf8()));
        }
        return py::percent("(%s %s %s)"_s, a, op, b);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// r = E; S(E->f0 ..) - the next statement reading E again where the compiler had it in r: through r (*r, r->fN)
// (a variant: the source read it through the variable, as switch ((k = n->left)->op))
std::optional<str> register_reads(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size() - 1; ++k) {
        OMatch m = py::re::fullmatch(R"re(([ \t]*)(r_\w+) = (\w+(?:->f\d+)+);)re"_s, lines[k]);
        if (!m) continue;
        str r = m->group(2), e = m->group(3);
        const str& nxt = lines[k + 1];
        if (!py::re::search(esc(e) + R"re(\b)re"_s, nxt)) continue;
        bool st = py::re::search(py::percent(R"re((?m)^[ \t]*register struct \w+ \*%s;$)re"_s, r), text).has_value();
        auto via = [&](const Match& mm) -> str {
            std::optional<str> g1 = mm.group_opt(1);
            if (!g1) return r;   // (the value itself: the register)
            int64_t n = py::int_from_str(*g1);
            return st ? py::percent("%s->f%d"_s, r, n) : (n == 0 ? py::percent("*%s"_s, r) : mm.group(0));
        };
        int64_t ind = indent_of(lines[k]);
        str base = py::re::match(R"re(\w+)re"_s, e)->group(0);
        bool changed = false;
        for (int64_t j = k + 1; j < lines.size(); ++j) {   // (on through the block: until r or E's pointer is given again, or a call could change E)
            str x = lines[j];
            if (nb(x) && indent_of(x) < ind) break;
            str nw = py::re::sub(esc(e) + R"re((?:->f(\d+))?\b(?!->))re"_s, via, x);
            if (nw != x) {
                lines[j] = nw;
                changed = true;
            }
            if (py::re::match(py::percent(R"re([ \t]*(?:%s|%s) = )re"_s, r, base), x)) break;
        }
        if (changed) return join_nl(lines);
    }
    return std::nullopt;
}

// a switch last in a loop's body whose arms go on with `continue;`: break (the same - the loop's end), and a
// default that only goes on gone (a variant: which the source wrote decides the layout)
std::optional<str> switch_breaks(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re(([ \t]*)(?:while \(.*\)|for \(.*\)) \{)re"_s, lines[k]);
        std::optional<int64_t> e = m ? _block_end(lines, k) : std::nullopt;
        if (!e) continue;
        str i4 = m->group(1) + "    "_s;
        std::optional<int64_t> sw, st;
        for (int64_t j = *e - 1; j > k; --j)
            if (nb(lines[j])) { sw = j; break; }
        if (!sw || lines[*sw] != i4 + "}"_s) continue;
        for (int64_t j = *sw - 1; j > k; --j)
            if (lines[j].startswith(i4 + "switch ("_s) && lines[j].endswith("{"_s)) { st = j; break; }
        bool bad = false;
        if (st)
            for (int64_t j = *st; j < *sw; ++j)
                if (lines[j].strip() == "continue;"_s && indent_of(lines[j]) > i4.size() + 4) bad = true;
        if (!st || bad) continue;
        OLines nw = opt_of(lines);
        bool changed = false;
        for (int64_t j = *st + 1; j < *sw; ++j)
            if (*nw[j] == i4 + "    continue;"_s) {
                nw[j] = i4 + "    break;"_s;
                changed = true;
            }
        int64_t j = *st + 1;
        while (j < *sw) {   // (a default that only breaks: none)
            if (*nw[j] == i4 + "default:"_s && j + 1 < *sw && *nw[j + 1] == i4 + "    break;"_s &&
                (j + 2 >= *sw || py::re::match(R"re(\s*(?:case [^:]+|default):$)re"_s, *nw[j + 2]))) {
                nw[j] = std::nullopt;
                nw[j + 1] = std::nullopt;
                changed = true;
                j += 2;
                continue;
            }
            ++j;
        }
        if (changed) return join_opt(nw);
    }
    return std::nullopt;
}

// switch (..) { .. default: S; case K: return V; } - the default falling into a case that only returns: the
// case written first, S after the switch with its own return V (a variant: the source's layout of both)
std::optional<str> default_after(const str& text) {
    for (const Match& m : py::re::finditer(R"re((?m)^([ \t]*)switch \(.*\) \{\n)re"_s, text)) {
        str ind = m.group(1);
        OMatch end;
        for (const Match& j : py::re::finditer(py::percent(R"re((?m)^%s\}$)re"_s, esc(ind)), text))
            if (j.start() > m.end()) { end = j; break; }
        if (!end) continue;
        str body = text.slice(m.end(), end->start());
        str ie = esc(ind);
        OMatch d = py::re::search(py::percent(R"re((?m)^%sdefault:\n((?:%s    [^\n]*\n)+)%s(case [^:\n]+:)\n%s    (return [^;\n]*;)\n$)re"_s, ie, ie, ie, ie), body);
        if (d && !py::re::search(py::percent(R"re((?m)^%s    (?:break|return|goto)\b)re"_s, ie), d->group(1))) {
            str s_;
            for (const str& x : py::split(d->group(1).rstrip("\n"_s), "\n"_s)) s_ = s_ + x.slice_from(4) + "\n"_s;
            str new_body = py::percent("%s%s\n%s    %s\n"_s, ind, d->group(2), ind, d->group(3)) + body.slice(0, d->start());
            return text.slice(0, m.end()) + new_body + py::percent("%s}\n"_s, ind) + s_ + py::percent("%s%s"_s, ind, d->group(3)) +
                   text.slice_from(end->end());
        }
    }
    return std::nullopt;
}

// if (C) { ..; r = A; } else { ..; r = B; } S(r); - the statement after, taking what each arm leaves in r, in
// each arm with its value: the compiler's one copy of a tail both arms ended with (a variant)
std::optional<str> push_tails(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p) || std::get<3>(*p) >= lines.size()) continue;
        using Got = std::vector<std::pair<str, std::pair<str, str>>>;   // {reg: (value, indent)}, in order
        auto tail = [&](const Lines& arm) -> std::pair<Got, int64_t> {   // (the arm's last run of r = V; - {r: V}, its start)
            Got got;
            int64_t j = arm.size() - 1;
            while (j >= 0 && !nb(arm[j])) --j;
            while (j >= 0) {
                OMatch m_ = py::re::fullmatch(R"re((\s*)(ax|bx|cx|dx|t\d+) = (.+);)re"_s, arm[j]);   // (or the model's temporary)
                bool in = false;
                if (m_)
                    for (const auto& g : got)
                        if (g.first == m_->group(2)) in = true;
                if (!m_ || in) break;
                got.push_back({m_->group(2), {m_->group(3), m_->group(1)}});
                --j;
            }
            return {got, j + 1};
        };
        auto [ga, ja] = tail(std::get<1>(*p));
        auto [gb, jb] = tail(*std::get<2>(*p));
        const str& nxt = lines[std::get<3>(*p)];
        auto has = [](const Got& g, const str& r) {
            for (const auto& x : g)
                if (x.first == r) return true;
            return false;
        };
        auto val = [](const Got& g, const str& r) -> const std::pair<str, str>& {
            for (const auto& x : g)
                if (x.first == r) return x.second;
            throw py::KeyError(r.utf8());
        };
        Lines regs;
        for (const auto& x : ga)
            if (has(gb, x.first)) regs.append(x.first);
        bool same = ga.size() == gb.size();
        if (same)
            for (const auto& x : ga)
                if (!has(gb, x.first)) same = false;
        if (regs.empty() || !same || py::re::match(R"re(\s*(?:if|while|for|switch|\}|L_\w+:))re"_s, nxt)) continue;
        bool skip = false;
        for (const str& r : regs)
            if (py::re::findall(py::percent(R"re(\b%s\b)re"_s, r), nxt).size() != 1 || _live_after(opt_of(lines), std::get<3>(*p), r)) skip = true;
        if (skip) continue;
        auto pv = [](const str& v) { return py::re::fullmatch(R"re([\w\->.]+)re"_s, v) ? v : py::percent("(%s)"_s, v); };
        auto put = [&](const Got& got) -> str {
            str s_ = nxt.strip();
            for (const str& r : regs)
                s_ = py::re::sub(py::percent(R"re(\b%s\b)re"_s, r), [&](const Match&) { return pv(val(got, r).first); }, s_, 1);
            return val(got, regs[0]).second + s_;
        };
        Lines then = std::get<1>(*p).slice(0, ja);
        then.append(put(ga));
        Lines els = std::get<2>(*p)->slice(0, jb);
        els.append(put(gb));
        str i0 = lead(lines[k]);
        Lines nw;
        nw.append(lines[k]);
        nw.extend(then);
        nw.append(i0 + "} else {"_s);
        nw.extend(els);
        nw.append(i0 + "}"_s);
        str t = join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p) + 1));
        for (const str& r : regs)
            if (r.startswith("t"_s)) t = _drop_decl(t, r);
        return t;
    }
    return std::nullopt;
}

// return (C ? A : B); - as the two returns it may have been: if (C) return A; return B; (a variant: the
// compiler keeps one copy of a tail both share either way)
std::optional<str> split_ternaries(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)return \((.*)\);$)re"_s, [](const Match& m) -> str {
        str ind = m.group(1), inner = m.group(2);
        int64_t depth = 0;
        std::optional<int64_t> q;
        for (int64_t i = 0; i < inner.size(); ++i) {   // (the top-level ? and :)
            char32_t ch = inner.at_(i);
            depth += (ch == U'(') - (ch == U')');
            if (ch == U'?' && depth == 0 && !q) q = i;
            if (ch == U':' && depth == 0 && q) {
                str c = inner.slice(0, *q).strip(), a = inner.slice(*q + 1, i).strip(), b = inner.slice_from(i + 1).strip();
                return py::percent("%sif (%s) {\n%s    return %s;\n%s}\n%sreturn %s;"_s, ind, c.startswith("("_s) ? c : py::percent("(%s)"_s, c),
                                   ind, a, ind, ind, b);
            }
        }
        return m.group(0);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// M = A; .. r = M; - a register the model reloads from the place a plain value was just stored in (its
// store-forwarding, written out before the pointer moves): the value itself, r = A
str store_reloads(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re(([ \t]*)(ax|dx|bx|cx) = (\*\w+|\w+->f\d+);)re"_s, lines[k]);
        if (!m) continue;
        str place = m->group(3);
        str base = py::re::match(R"re(\*?(\w+))re"_s, place)->group(1);
        for (int64_t j = k - 1; j > std::max<int64_t>(k - 6, -1); --j) {   // (the store just before, nothing between it and here giving its value or base another)
            OMatch s = py::re::fullmatch(R"re([ \t]*(.+?) = ([A-Za-z_]\w*|-?\d+);)re"_s, lines[j]);
            if (!s) break;
            if (s->group(1) == place) {
                bool any_ = false;
                for (int64_t q = j + 1; q < k; ++q)
                    if (py::re::match(py::percent(R"re([ \t]*(?:%s|%s) = )re"_s, esc(s->group(2)), base), lines[q])) any_ = true;
                if (s->group(2) != base && !any_) lines[k] = py::percent("%s%s = %s;"_s, m->group(1), m->group(2), s->group(2));
                break;
            }
        }
    }
    return join_nl(lines);
}

// (*x)(..) - x called through: a pointer to a function, int (*x)(); (a parameter's or a local's int)
str fn_pointers(str text) {
    Lines xs = py::re::findall(R"re(\(\*([A-Za-z_]\w*)\)\s*\()re"_s, text);
    std::vector<str> v;
    for (const str& x : xs)
        if (std::find(v.begin(), v.end(), x) == v.end()) v.push_back(x);
    std::sort(v.begin(), v.end());
    for (const str& x : v)
        if (py::re::search(py::percent(R"re((?m)^[ \t]*(?:register\s+)?int (?:\w+, )*%s\b)re"_s, x), text)) {
            std::optional<str> t = _redeclare(text, x, py::percent("int (*%s)()"_s, x));
            if (t && !t->empty()) text = *t;
        }
    return text;
}

// bx = p + (i << 1); .. *bx - a word of p's at i: p[i], p an int's pointer (the scaled index the model left
// as an address; a copy of the doubled index the code made on the way, `ax = (i << 1);`, nothing)
str scaled_index(const str& text) {
    OLines lines = opt_of(split_nl(text));
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::match(R"re(([ \t]*)bx = ([A-Za-z_]\w*) \+ \((\w+|\((?:[^()]|\([^()]*\))+\)) << 1\);$)re"_s, lines[k].value_or(""_s));
        if (!m) continue;
        str p = m->group(2), i = m->group(3);
        if (i.startswith("("_s) && _balanced(i.slice(1, -1))) i = i.slice(1, -1);   // (an expression the index: as it is)
        int64_t j = k + 1;
        while (j < lines.size() && py::re::fullmatch(py::percent(R"re([ \t]*ax = \(%s << 1\);)re"_s, esc(i)), lines[j].value_or(""_s)) &&
               !_live_after(lines, j, "ax"_s))
            ++j;
        if (j >= lines.size() || py::re::findall(R"re((?<![\w)])\*bx\b(?!\+\+|--))re"_s, *lines[j]).size() != 1 ||
            py::re::findall(R"re(\bbx\b)re"_s, *lines[j]).size() != 1 || _live_after(lines, j, "bx"_s))
            continue;
        bool assigned = false;
        for (const str& v : py::re::findall(R"re([A-Za-z_]\w*)re"_s, i))
            if (py::re::search(py::percent(R"re(\b%s\s*=[^=])re"_s, v), *lines[j])) assigned = true;
        if (assigned) continue;
        lines[j] = py::re::sub(R"re((?<![\w)])\*bx\b)re"_s, py::percent("%s[%s]"_s, p, i), *lines[j]);
        for (int64_t q = k + 1; q < j; ++q) lines[q] = std::nullopt;
        lines[k] = std::nullopt;
        str t = join_opt(lines);
        std::optional<str> d = text::_declare_pointer(t, p, "int"_s);
        return (d && !d->empty()) ? *d : t;
    }
    return text;
}

// if (A) { if (B) { S } } else { S } - S reached both ways (the compiler's one copy): if (!A || B) { S }
str shared_arms(const str& text) {
    Lines lines = split_nl(text);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            auto p = _if_parts(lines, k);
            if (!p || !std::get<2>(*p)) continue;
            Lines th;
            for (const str& x : std::get<1>(*p))
                if (nb(x)) th.append(x);
            auto q = !th.empty() ? _if_parts(th, 0) : std::nullopt;
            if (!q || std::get<2>(*q) || std::get<3>(*q) != th.size()) continue;
            Lines inner, other;
            for (const str& x : std::get<1>(*q))
                if (nb(x)) inner.append(x.slice_from(4));
            for (const str& x : *std::get<2>(*p))
                if (nb(x)) other.append(x);
            if (inner != other) continue;
            str ind = lead(lines[k]);
            str cond = _conj("||"_s, _truth(_not(py::percent("(%s)"_s, std::get<0>(*p)))), _truth(py::percent("(%s)"_s, std::get<0>(*q))));
            Lines nw;
            nw.append(py::percent("%sif (%s) {"_s, ind, cond));
            nw.extend(*std::get<2>(*p));
            nw.append(ind + "}"_s);
            lines.set_slice(k, std::get<3>(*p), nw);
            changed = true;
            break;
        }
    }
    return join_nl(lines);
}

namespace {
// "(neg)" as a test: neg itself when it is a parenthesized whole, else parenthesized
str as_test(const str& neg) { return (neg.startswith("("_s) && _balanced(neg.slice(1, -1))) ? neg : py::percent("(%s)"_s, neg); }
Lines dd8(const Lines& ls) {   // [x[4:] if x.startswith("        ") else x for x in ls]
    Lines out;
    for (const str& x : ls) out.append(x.startswith("        "_s) ? x.slice_from(4) : x);
    return out;
}
Lines nonblank_of(const Lines& ls) {
    Lines out;
    for (const str& x : ls)
        if (nb(x)) out.append(x);
    return out;
}
}  // namespace

// if (C) { S } return X; last in the function: if (!C) return X; S return X; (a variant: a guard returning
// at once is laid out its own way)
std::optional<str> guard_returns(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        if (!lines[k].startswith("    if ("_s) || !lines[k].endswith("{"_s)) continue;
        auto p = _if_parts(lines, k);
        if (!p || std::get<2>(*p)) continue;
        Lines rest = nonblank_of(lines.slice_from(std::get<3>(*p)));
        if (rest.size() != 2 || !py::re::fullmatch(R"re(    return [^;]+;)re"_s, rest[0]) || rest[1] != "}"_s) continue;
        str neg = _not(py::percent("(%s)"_s, std::get<0>(*p)));
        Lines nw;
        nw.append(py::percent("    if %s %s"_s, as_test(neg), rest[0].strip()));
        nw.extend(dd8(std::get<1>(*p)));
        return join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
    }
    for (int64_t k = 0; k < lines.size(); ++k) {   // (if (C) { S } else { return x; } last: the same guard; then an arm ending in a return its else flattened after it)
        if (!lines[k].startswith("    if ("_s) || !lines[k].endswith("{"_s)) continue;
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p) || nonblank_of(lines.slice_from(std::get<3>(*p))) != Lines{"}"_s}) continue;
        Lines el;
        for (const str& x : *std::get<2>(*p))
            if (nb(x)) el.append(x.strip());
        Lines th = nonblank_of(std::get<1>(*p));
        Lines nw;
        if (el.size() == 1 && el[0].startswith("return"_s)) {
            str neg = _not(py::percent("(%s)"_s, std::get<0>(*p)));
            nw.append(py::percent("    if %s %s"_s, as_test(neg), el[0]));
            nw.extend(dd8(std::get<1>(*p)));
        } else if (!th.empty() && py::re::match(R"re(\s*return\b)re"_s, th[-1]) && th[-1].startswith("        return"_s)) {
            nw.append(lines[k]);
            nw.extend(std::get<1>(*p));
            nw.append("    }"_s);
            nw.extend(dd8(*std::get<2>(*p)));
        } else {
            continue;
        }
        str t = join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
        std::optional<str> g = guard_returns(t);
        return (g && !g->empty()) ? *g : t;
    }
    return std::nullopt;
}

// the first guard guard_returns makes (if (C) { S } else { return x; } last: if (!C) return x; S), the
// returns after it only flattened - not made guards again (a variant)
std::optional<str> guard_flat(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        if (!lines[k].startswith("    if ("_s) || !lines[k].endswith("{"_s)) continue;
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p) || nonblank_of(lines.slice_from(std::get<3>(*p))) != Lines{"}"_s}) continue;
        Lines el;
        for (const str& x : *std::get<2>(*p))
            if (nb(x)) el.append(x.strip());
        if (el.size() != 1 || !el[0].startswith("return"_s)) continue;
        str neg = _not(py::percent("(%s)"_s, std::get<0>(*p)));
        Lines nw;
        nw.append(py::percent("    if %s %s"_s, as_test(neg), el[0]));
        nw.extend(dd8(std::get<1>(*p)));
        str t = join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
        std::optional<str> f = shapes::flat_returns(t);
        return (f && !f->empty()) ? *f : t;
    }
    return std::nullopt;
}

// if (C) { S } [else { }] last, the function falling off its end the other way - S returning K somewhere:
// if (!C) return K; S (a variant: the way out the compiler shares with S's return of K, the model reading the
// jump to it as the end)
std::optional<str> guard_shared(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        if (!lines[k].startswith("    if ("_s) || !lines[k].endswith("{"_s)) continue;
        auto p = _if_parts(lines, k);
        if (!p) continue;
        bool has_else = false;
        if (std::get<2>(*p))
            for (const str& x : *std::get<2>(*p))
                if (nb(x)) has_else = true;
        if (has_else || nonblank_of(lines.slice_from(std::get<3>(*p))) != Lines{"}"_s}) continue;
        Lines ks;
        for (const str& x : std::get<1>(*p)) {
            OMatch m = py::re::match(R"re(\s*return (-?\d+);$)re"_s, x);
            if (m) ks.append(m->group(1));
        }
        if (ks.empty()) continue;
        str neg = _not(py::percent("(%s)"_s, std::get<0>(*p)));
        Lines nw;
        nw.append(py::percent("    if %s return %s;"_s, as_test(neg), ks[0]));
        nw.extend(dd8(std::get<1>(*p)));
        str t = join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
        std::optional<str> f = shapes::flat_returns(t);
        return (f && !f->empty()) ? *f : t;
    }
    return std::nullopt;
}

namespace {
// bool_trees' expression: a "0"/"1" text, a goto to a label, or an if of a test and two expressions
struct Expr {
    enum Kind { Text, Goto, If } kind = Text;
    str s;   // the text / the label
    str cond;
    std::shared_ptr<Expr> a, b;
    bool operator==(const Expr& o) const {
        if (kind != o.kind || s != o.s || cond != o.cond) return false;
        auto eq = [](const std::shared_ptr<Expr>& x, const std::shared_ptr<Expr>& y) {
            if (!x || !y) return !x && !y;
            return *x == *y;
        };
        return eq(a, o.a) && eq(b, o.b);
    }
};
using ExprP = std::shared_ptr<Expr>;
}  // namespace

// an if-tree whose every way ends giving r 1 or 0 (a goto to a labelled part of it the same part again):
// if (E) { r = 1; } else { r = 0; } - E the && / || of its tests, a part both ways share factored out
// (if (c) { X && T } else { T }: (!c || X) && T) - the compiler's one copy of the tests read back
// (a block of a label and then only a label is where Python reads a string as an if's parts: here, no tree)
str bool_trees(const str& text) {
    Lines lines = _else_ifs(split_nl(text));
    std::vector<std::pair<str, ExprP>> labels;
    auto label_get = [&](const str& l) -> ExprP {
        for (const auto& x : labels)
            if (x.first == l) return x.second;
        return nullptr;
    };
    auto label_set = [&](const str& l, const ExprP& e) {
        for (auto& x : labels)
            if (x.first == l) {
                x.second = e;
                return;
            }
        labels.push_back({l, e});
    };
    struct St {
        enum K { Label, If, Line } kind;
        str s;
        text::IfParts ip;
    };
    auto stmts = [&](const Lines& block) {   // (the statements of a block)
        std::vector<St> out;
        int64_t j = 0;
        while (j < block.size()) {
            const str& l = block[j];
            if (!nb(l)) {
                ++j;
                continue;
            }
            OMatch m = py::re::fullmatch(R"re([ \t]*(L_\w+):)re"_s, l);
            if (m) {
                out.push_back(St{St::Label, m->group(1), {}});
                ++j;
                continue;
            }
            auto p = _if_parts(block, j);
            if (p) {
                out.push_back(St{St::If, ""_s, *p});
                j = std::get<3>(*p);
                continue;
            }
            out.push_back(St{St::Line, l.strip(), {}});
            ++j;
        }
        return out;
    };
    std::function<ExprP(const Lines&, const str&)> node = [&](const Lines& block, const str& reg) -> ExprP {   // (the expression a block gives reg, or None)
        std::vector<St> ss = stmts(block);
        std::optional<str> lab;
        if (!ss.empty() && ss[0].kind == St::Label) {
            lab = ss[0].s;
            ss.erase(ss.begin());
        }
        if (ss.size() != 1) return nullptr;
        ExprP e = std::make_shared<Expr>();
        if (ss[0].kind == St::Line) {
            OMatch m = py::re::fullmatch(py::percent(R"re(%s = ([01]);)re"_s, reg), ss[0].s);
            if (m) {
                e->kind = Expr::Text;
                e->s = m->group(1);
            } else {
                OMatch g = py::re::fullmatch(R"re(goto (L_\w+);)re"_s, ss[0].s);
                if (!g) return nullptr;
                e->kind = Expr::Goto;
                e->s = g->group(1);
            }
        } else if (ss[0].kind == St::If) {
            const text::IfParts& d = ss[0].ip;
            if (!std::get<2>(d)) return nullptr;
            ExprP a = node(std::get<1>(d), reg), b = node(*std::get<2>(d), reg);
            if (!a || !b) return nullptr;
            e->kind = Expr::If;
            e->cond = std::get<0>(d);
            e->a = a;
            e->b = b;
        } else {
            return nullptr;
        }
        if (lab) label_set(*lab, e);
        return e;
    };
    std::function<std::optional<str>(const ExprP&)> text_of = [&](const ExprP& e) -> std::optional<str> {
        if (e->kind == Expr::Text) return e->s;
        if (e->kind == Expr::Goto) {
            ExprP l = label_get(e->s);
            if (l && !(*l == *e)) return text_of(l);
            return std::nullopt;
        }
        str c = !e->cond.startswith("("_s) ? py::percent("(%s)"_s, _strip(py::percent("(%s)"_s, e->cond))) : e->cond;
        std::optional<str> a_ = text_of(e->a), b_ = text_of(e->b);
        if (!a_ || !b_) return std::nullopt;
        str t = _truth(c), f = _truth(_not(c));
        if (*b_ == "0") return *a_ == "1" ? t : (*a_ == "0" ? "0"_s : _conj("&&"_s, t, *a_));
        if (*a_ == "1") return _conj("||"_s, t, *b_);
        if (*a_ == "0") return *b_ == "1" ? f : _conj("&&"_s, f, *b_);
        if (*b_ == "1") return _conj("||"_s, f, *a_);
        Lines parts, tail;
        for (const str& x : _top_split(*a_, "&&"_s)) parts.append(x.strip());
        for (const str& x : _top_split(*b_, "&&"_s)) tail.append(x.strip());
        if (parts.size() > tail.size() && parts.slice(parts.size() - tail.size(), parts.size()) == tail) {   // (if (c) { X && T } else { T }: (!c || X) && T)
            str x = py::join(" && "_s, parts.slice(0, parts.size() - tail.size()));
            return _conj("&&"_s, py::percent("(%s)"_s, _conj("||"_s, f, x)), *b_);
        }
        return std::nullopt;
    };
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p)) continue;
        str nxt;
        for (const str& x : lines.slice_from(std::get<3>(*p)))
            if (nb(x)) { nxt = x; break; }
        OMatch r = py::re::fullmatch(R"re([ \t]*return (\w+);)re"_s, nxt);
        if (!r) continue;
        labels.clear();
        ExprP e = node(lines.slice(k, std::get<3>(*p)), r->group(1));
        if (!e || e->kind != Expr::If || (e->a->kind == Expr::Text && e->b->kind == Expr::Text)) continue;   // (one test alone: as written - its sense is the code's)
        std::optional<str> x = text_of(e);
        if (!x) x = text_of(e);   // (a goto to a label met later: again)
        if (!x || *x == "0" || *x == "1" || x->contains("goto"_s)) continue;
        str ind = lead(lines[k]);
        str reg = r->group(1);
        Lines nw;
        nw.append(py::percent("%sif %s {"_s, ind, (x->startswith("("_s) && x->endswith(")"_s) && _balanced(x->slice(1, -1))) ? *x : py::percent("(%s)"_s, *x)));
        nw.append(py::percent("%s    %s = 1;"_s, ind, reg));
        nw.append(py::percent("%s} else {"_s, ind));
        nw.append(py::percent("%s    %s = 0;"_s, ind, reg));
        nw.append(py::percent("%s}"_s, ind));
        return join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
    }
    return text;
}

// if (A) { X; if (B) { T } } else { T } at the function's end - the compiler's one copy of T both ways
// reach: if (A) { X; if (!B) return; } T (a variant)
std::optional<str> shared_tails(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p) || std::get<1>(*p).empty()) continue;
        str ind = lead(lines[k]);
        Lines rest = nonblank_of(lines.slice_from(std::get<3>(*p)));
        if (rest.empty() ||
            !(rest[0] == "}"_s || (py::re::fullmatch(R"re([ \t]*return( \w+)?;)re"_s, rest[0]) && rest.size() > 1 && rest[1] == "}"_s)))
            continue;
        const Lines& then = std::get<1>(*p);
        auto is_if = [&](const str& x) {
            return x.startswith(ind + "    if ("_s) && x.endswith("{"_s) && indent_of(x) == ind.size() + 4;
        };
        std::optional<int64_t> q;
        for (int64_t j = 0; j < then.size(); ++j)
            if (is_if(then[j])) { q = j; break; }
        std::optional<text::IfParts> pi;
        while (q) {   // (the last if of the then-arm, nothing after it)
            pi = _if_parts(then, *q);
            if (pi && !std::get<2>(*pi) && std::get<3>(*pi) == then.size()) break;
            std::optional<int64_t> nq;
            for (int64_t j = *q + 1; j < then.size(); ++j)
                if (is_if(then[j])) { nq = j; break; }
            q = nq;
        }
        if (!q) continue;
        Lines inner, other;
        for (const str& x : std::get<1>(*pi))
            if (nb(x)) inner.append(x.slice_from(4).rstrip());
        for (const str& x : *std::get<2>(*p))
            if (nb(x)) other.append(x.rstrip());
        if (inner != other) continue;
        str neg = _not(py::percent("(%s)"_s, std::get<0>(*pi)));
        Lines nw;
        nw.append(lines[k]);
        nw.extend(then.slice(0, *q));
        nw.append(ind + py::percent("    if (%s) return;"_s, neg.startswith("("_s) ? neg : py::percent("(%s)"_s, neg)));
        nw.append(ind + "}"_s);
        for (const str& x : *std::get<2>(*p)) nw.append(x.slice_from(4));
        return join_nl(lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p)));
    }
    return std::nullopt;
}

// (i << 1) + K, K a place in the data: the address of a word there, &g_K[i] (a variant)
std::optional<str> scaled_places(const str& text) {
    str t = py::re::sub(R"re(\(\(([\w\->.]+) << 1\) \+ (-?\d+)\))re"_s, [](const Match& m) -> str {
        int64_t k = gi(m, 2) & 0xFFFF;
        if (k < 0x100) return m.group(0);
        str name = py::percent("g_%04x"_s, k);
        text::ARRAYS.setdefault(name, "int"_s);
        return py::percent("&%s[%s]"_s, name, m.group(1));
    }, text);
    Lines gs0 = py::re::findall(R"re(&(g_[0-9a-f]{4})\[)re"_s, t);
    std::vector<str> gs;   // sorted(set(..))
    for (const str& g : gs0)
        if (std::find(gs.begin(), gs.end(), g) == gs.end()) gs.push_back(g);
    std::sort(gs.begin(), gs.end());
    for (const str& g : gs)   // (declared, when the text is a whole source)
        if (py::re::search(R"re((?m)^\w+\(.*\)$)re"_s, t) && !py::re::search(py::percent(R"re((?m)^(?:extern|struct)[^\n]*\b%s\b)re"_s, g), t))
            t = py::percent("extern int %s[];\n"_s, g) + t;
    if (t != text) return t;
    return std::nullopt;
}

// the frame one word bigger: the trailing filler two bytes more, or one added after the last local (a
// variant: a local the model's frame left out)
std::optional<str> frame_more(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return std::nullopt;
    std::vector<Match> decls;
    int64_t stop;
    {
        OMatch s = py::re::search(R"re((?m)^[ \t]*(?!register|unsigned|int|char|long|short|struct)\S)re"_s, body);
        if (!s) s = py::re::search("$"_s, body);
        stop = s->start();
    }
    for (const Match& d : py::re::finditer(
             R"re((?m)^[ \t]*(?!return\b)(?:register\s+)?(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;\n)re"_s, body))
        if (d.start() < stop) decls.push_back(d);
    if (decls.empty()) return std::nullopt;
    const Match& last = decls.back();
    OMatch f = py::re::fullmatch(R"re(([ \t]*)char (_p\w*)\[(\d+)\];\n)re"_s, last.group(0));
    str nw = f ? py::percent("%schar %s[%d];\n"_s, f->group(1), f->group(2), gi(*f, 3) + 2) : last.group(0) + "    char _pm[2];\n"_s;
    return head + sep + body.slice(0, last.start()) + nw + body.slice_from(last.end());
}

// the frame's first local vN (bp-N) with room between it and bp: a filler declared before it (a variant:
// unused locals the source declared first, the model's layout taking the first one for read)
std::optional<str> lead_filler(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return std::nullopt;
    OMatch m = py::re::search(R"re((?m)^([ \t]*)((?:unsigned\s+)?(int|char|long|short)|(?:\w+ )?\w+ \*) ?v(\d+);$)re"_s, body);
    OMatch first = py::re::search(
        R"re((?m)^[ \t]*(?!register\b)(?:unsigned\s+)?(?:int|char|long|short|struct \w+)[ \t*]+\w+(?:\[\d+\])?;$)re"_s, body);
    if (!m || !first || first->start() != m->start()) return std::nullopt;
    std::optional<str> base = m->group_opt(3);
    int64_t size = (base && *base == "long") ? 4 : (base && *base == "char") ? 1 : 2;
    int64_t gap = gi(*m, 4) - size;
    if (gap <= 0) return std::nullopt;
    str fill;
    for (int64_t j = 0; j < gap / 2; ++j) fill = fill + py::percent("%sint _q%d;\n"_s, m->group(1), 2 * j);   // (words: an array goes elsewhere in the frame)
    if (gap % 2) fill = fill + py::percent("%schar _q%d;\n"_s, m->group(1), gap);
    return head + sep + body.slice(0, m->start()) + fill + body.slice_from(m->start());
}

// r = x = E; with the register variable r given nothing else: x itself where r is read (a variant: the
// compiler went on through the value it still held - the register its own scratch, not a variable)
std::optional<str> register_copies(const str& text) {
    for (const Match& m : py::re::finditer(R"re((?m)^([ \t]*)(r_\w+) = (\w+) = ([^;\n]+);$)re"_s, text)) {
        str r = m.group(2), x = m.group(3);
        str rest = text.slice_from(m.end());
        if (py::re::search(py::percent(R"re((?<![\w>.*])%s\s*(?:=[^=]|\+\+|--)|(?:\+\+|--)%s\b)re"_s, r, r), rest) ||
            py::re::search(py::percent(R"re((?<![\w>.*])%s\s*(?:=[^=]|\+\+|--))re"_s, x), rest))
            continue;
        str t = text.slice(0, m.start()) + py::percent("%s%s = %s;"_s, m.group(1), x, m.group(4)) +
                py::re::sub(py::percent(R"re(\b%s\b)re"_s, r), x, rest);
        OMatch d = py::re::search(py::percent(R"re((?m)^[ \t]*register [^;\n]*\b%s;\n)re"_s, r), t);
        if (d) t = t.slice(0, d->start()) + t.slice_from(d->end());
        str r2 = r.slice_from(2);
        if (text::BYTEPTR.contains(std::make_tuple(r2 + "w"_s, int64_t(0))) && !text::BYTEPTR.contains(std::make_tuple(r2 + "*"_s, int64_t(0)))) {   // (words read through the register: an int's pointer)
            std::optional<str> t2 = _redeclare(t, x, py::percent("int *%s"_s, x));
            if (t2 && !t2->empty()) t = *t2;
        }
        return t;
    }
    return std::nullopt;
}

// the text with its int register locals unsigned (a variant: compared, the compiler's jumps say which)
std::optional<str> unsigned_registers(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)register int (r_\w+);$)re"_s, R"re(\1register unsigned \2;)re"_s, text);
    if (t != text) return t;
    return std::nullopt;
}

// switch (..) { .. default: return X; } - a switch with no default, the return after it (a variant: the
// compiler's default jump goes to the code after the switch either way, laid out after the cases)
std::optional<str> no_default(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)default:\n[ \t]*(return [^;\n]*;)\n([ \t]*)\}$)re"_s, [](const Match& m) -> str {
        return m.group(1).size() == m.group(3).size() ? py::percent("%s}\n%s%s"_s, m.group(3), m.group(3), m.group(2)) : m.group(0);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// (A == (B & 0xff)) - a byte the model masked compared with another: the bytes compared (a variant: the
// compiler compares bytes of two chars as they are)
std::optional<str> unmasked(const str& text) {
    str t = py::re::sub(R"re((==|!=) \(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\))re"_s, R"re(\1 \2)re"_s, text);
    t = py::re::sub(R"re(\(((?:\*|\b)[A-Za-z_]\w*(?:->f\d+|\[[^\]]+\])?) & 0xff\) (==|!=))re"_s, R"re(\1 \2)re"_s, t);
    t = py::re::sub(R"re(!\(([\w\->.*\[\]]+) (==|!=) ([\w\->.*\[\]]+)\))re"_s, [](const Match& m) -> str {
        return py::percent("(%s %s %s)"_s, m.group(1), m.group(2) == "==" ? "!="_s : "=="_s, m.group(3));
    }, t);
    str u = py::re::sub(R"re(\b(\w+->f\d+|\*\w+) (==|!=) (12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b)re"_s, [](const Match& m) -> str {   // (a byte compared with 128..255: the char's)
        return py::percent("%s %s (char)0x%x"_s, m.group(1), m.group(2), gi(m, 3));
    }, t);
    if (u != text) return u;
    return std::nullopt;
}

// f(a, b): a never read, b only masked (b & K) - a long's high word tested alone: long a; (a & 0xK0000L)
// (a variant: the compiler tests just the high word of a long masked so)
std::optional<str> high_words(const str& text) {
    OMatch h = py::re::search(R"re((?m)^(\w+)\(([\w, ]+)\)\n)re"_s, text);
    if (!h) return std::nullopt;
    Lines ps;
    for (const str& x : py::split(h->group(2), ","_s)) ps.append(x.strip());
    str body = text.slice_from(h->end());
    for (int64_t i = 0; i + 1 < ps.size(); ++i) {
        const str& p = ps[i];
        const str& q = ps[i + 1];
        int64_t uses = py::re::findall(py::percent(R"re((?<![\w>.])%s(?![\w(]))re"_s, q), body).size();
        int64_t masks = py::re::findall(py::percent(R"re(\(%s & (-?\d+|0x[0-9a-f]+)\))re"_s, q), body).size();
        int64_t decl = py::re::findall(py::percent(R"re((?m)^[ \t]*(?:register )?\w+ [^;\n]*\b%s\b[^;\n]*;$)re"_s, q), body).size();
        if (py::re::search(py::percent(R"re((?<![\w>.])%s(?![\w(]))re"_s, p),
                           py::re::sub(R"re((?m)^[ \t]*(?:register )?\w+ [\w, *]+;$)re"_s, ""_s, body)) ||
            masks == 0 || uses != masks + decl)
            continue;
        str t = py::re::sub(py::percent(R"re(\(%s & (-?\d+|0x[0-9a-f]+)\))re"_s, q), [&](const Match& m) -> str {
            int64_t k = py::int_parse(m.group(1), 0) & 0xffff;
            return py::percent("(%s & 0x%x0000L)"_s, p, k);
        }, body);
        t = _drop_decl(t, q);
        std::optional<str> t2 = _redeclare(t, p, py::percent("long %s"_s, p));
        if (!t2) return std::nullopt;
        Lines rest;
        for (const str& x : ps)
            if (x != q) rest.append(x);
        return text.slice(0, h->start()) + py::percent("%s(%s)\n"_s, h->group(1), py::join(", "_s, rest)) + *t2;
    }
    return std::nullopt;
}

// X = E; f(..X..); - the value passed from the register it was stored from: f(..(X = E)..) (a variant)
std::optional<str> assign_in_args(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)([A-Za-z_][\w\->\[\]]*) = ([^;\n=]+);\n[ \t]*((?:return )?[A-Za-z_]\w*\(.*\);)$)re"_s,
                        [](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), e = m.group(3), call = m.group(4);
        str pat = py::percent(R"re((?<![\w>.])%s(?![\w(]))re"_s, esc(x));
        if (py::re::findall(pat, call).size() != 1 || py::re::search(py::percent(R"re(&\s*%s(?![\w(]))re"_s, esc(x)), call))   // (its address passed: no value there)
            return m.group(0);
        return ind + py::re::sub(pat, [&](const Match&) { return py::percent("(%s = %s)"_s, x, e); }, call, 1);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// X = Y; return X; - the value still in the register it was stored from: return Y (a variant: a reload of X
// is other code)
std::optional<str> return_sources(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)([^=;\n]+?) = ([A-Za-z_]\w*);\n([ \t]*)return \2;$)re"_s,
                        R"re(\1\2 = \3;\n\4return \3;)re"_s, text);
    if (t != text) return t;
    return std::nullopt;
}

// X = X op E; - the compiler's `op [X], reg` - as C's compound assignment X op= E (a variant: MS C gives the
// two different code)
std::optional<str> compound_assigns(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)([A-Za-z_*][\w\->\[\]*]*) = \2 ([-+*/%&|^]|<<|>>) (.+);$)re"_s, [](const Match& m) -> str {
        str e = m.group(4);
        if (e.startswith("("_s) && e.endswith(")"_s) && _strip(py::percent("(%s)"_s, e)) == e) e = e.slice(1, -1);
        return py::percent("%s%s %s= %s;"_s, m.group(1), m.group(2), m.group(3), e);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// A = E; B = A; - the value given twice from the register it is in: B = A = E (a variant: a reload of A
// reads the same)
std::optional<str> chain_assigns(const str& text) {
    str t = py::re::sub(R"re((?m)^([ \t]*)([^\s=;][^=;]*?) = ([^;\n]+);\n\1([^\s=;][^=;]*?) = \2;$)re"_s, [](const Match& m) -> str {
        str ind = m.group(1), a = m.group(2), e = m.group(3), b = m.group(4);
        if (b == a || py::re::search(py::percent(R"re(\b%s\b)re"_s, esc(b)), e) || py::re::search(R"re(\+\+|--|\w\()re"_s, a + b)) return m.group(0);
        return py::percent("%s%s = %s = %s;"_s, ind, b, a, e);
    }, text);
    if (t != text) return t;
    return std::nullopt;
}

// a goto past statements under a test - the compiler's || of a test that skips them:
//     if (C1) { if (C2) goto L; } S.. L:   ->   if (!C1 || !C2) { S.. }
//     if (C) goto L; S.. L:                ->   if (!C) { S.. }
// (L then wanted by none else: dropped)
str guard_gotos(const str& text) {
    Lines lines = split_nl(text);
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            OMatch m = py::re::match(R"re((\s*)if \((.*)\) \{$)re"_s, lines[k]);
            OMatch g = (m && k + 2 < lines.size()) ? py::re::match(R"re((\s*)if \((.*)\) goto (L_\w+);$)re"_s, lines[k + 1]) : std::nullopt;
            OMatch one = py::re::match(R"re((\s*)if \((.*)\) goto (L_\w+);$)re"_s, lines[k]);
            str ind, lab, cond;
            int64_t start;
            if (m && g && lines[k + 2] == m->group(1) + "}"_s) {
                ind = m->group(1);
                lab = g->group(3);
                cond = py::percent("%s || %s"_s, _truth(_not("("_s + m->group(2) + ")"_s)), _truth(_not("("_s + g->group(2) + ")"_s)));
                start = k + 3;
            } else if (one) {
                ind = one->group(1);
                lab = one->group(3);
                cond = _truth(_not("("_s + one->group(2) + ")"_s));
                start = k + 1;
            } else {
                continue;
            }
            std::optional<int64_t> end;
            for (int64_t j = start; j < lines.size(); ++j)
                if (lines[j] == ind + lab + ":"_s) { end = j; break; }
            if (!end || *end == start) continue;
            Lines body = lines.slice(start, *end);
            bool bad = false;
            for (const str& l : body)
                if ((nb(l) && !l.startswith(ind)) || py::re::match(R"re(\s*L_\w+:)re"_s, l)) bad = true;
            if (bad) continue;
            int64_t depth = 0;
            bool ok = true;
            for (const str& l : body) {   // (the statements between: whole ones)
                depth += l.count("{"_s) - l.count("}"_s);
                if (depth < 0) ok = false;
            }
            if (!ok || depth) continue;
            Lines nw;
            nw.append(py::percent("%sif (%s) {"_s, ind, cond));
            for (const str& l : body) nw.append(nb(l) ? "    "_s + l : l);
            nw.append(ind + "}"_s);
            Lines rest = lines.slice(0, k) + nw + lines.slice_from(*end + 1);
            if (py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab), join_nl(rest)))
                rest = lines.slice(0, k) + nw + lines.slice_from(*end);   // (wanted elsewhere: kept)
            lines = rest;
            changed = true;
            break;
        }
    }
    return join_nl(lines);
}

// ifs nested as the compiler's || and && leave them, joined again:
//     if (A) { if (B) { P } else { Q } } else { Q }   ->  if (A && B) { P } else { Q }
//     if (A) { if (B) { P } else { Q } } else { P }   ->  if (!A || B) { P } else { Q }
//     if (A) { if (B) { P } }                         ->  if (A && B) { P }
// (the arms the model wrote twice, where both ways go)
str merge_conditions(const str& text) {
    Lines lines = _else_ifs(split_nl(text));
    bool changed = true;
    while (changed) {
        changed = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            auto p = _if_parts(lines, k);
            if (!p) continue;
            const str& c1 = std::get<0>(*p);
            const Lines& then = std::get<1>(*p);
            const std::optional<Lines>& els = std::get<2>(*p);
            int64_t end = std::get<3>(*p);
            str ind = lead(lines[k]);
            std::optional<int64_t> first;
            for (int64_t j = 0; j < then.size(); ++j)
                if (nb(then[j])) { first = j; break; }
            if (!first) continue;
            auto q = _if_parts(then, *first);
            if (!q) continue;
            if (std::get<3>(*q) != then.size()) {
                bool any_ = false;
                for (const str& l : then.slice_from(std::get<3>(*q)))
                    if (nb(l)) any_ = true;
                if (any_) continue;
            }
            const str& c2 = std::get<0>(*q);
            const Lines& p2 = std::get<1>(*q);
            const std::optional<Lines>& q2 = std::get<2>(*q);
            auto dd = [](const Lines& ls) {
                Lines out;
                for (const str& l : ls) out.append(dedent4(l));
                return out;
            };
            std::optional<Lines> nw;
            if (!els && !q2) {
                Lines n;
                n.append(py::percent("%sif (%s) {"_s, ind, _conj("&&"_s, _truth("("_s + c1 + ")"_s), _truth("("_s + c2 + ")"_s))));
                n.extend(dd(p2));
                n.append(ind + "}"_s);
                nw = n;
            } else if (els && q2 && _same_arm(*els, dd(*q2))) {
                Lines n;
                n.append(py::percent("%sif (%s) {"_s, ind, _conj("&&"_s, _truth("("_s + c1 + ")"_s), _truth("("_s + c2 + ")"_s))));
                n.extend(dd(p2));
                n.append(ind + "} else {"_s);
                n.extend(_unlabelled(dd(*q2), *els));
                n.append(ind + "}"_s);
                nw = n;
            } else if (els && q2 && _same_arm(*els, dd(p2))) {
                Lines n;
                n.append(py::percent("%sif (%s) {"_s, ind, _conj("||"_s, _truth(_not("("_s + c1 + ")"_s)), _truth("("_s + c2 + ")"_s))));
                n.extend(_unlabelled(dd(p2), *els));
                n.append(ind + "} else {"_s);
                n.extend(dd(*q2));
                n.append(ind + "}"_s);
                nw = n;
            }
            if (nw) {
                Lines cand = lines.slice(0, k) + *nw + lines.slice_from(end);
                str whole = join_nl(cand);   // (a label another goto still wants: kept, else not this way)
                bool bad = false;
                for (const str& g : py::re::findall(R"re(\bgoto (L_\w+);)re"_s, whole))
                    if (!py::re::search(py::percent(R"re((?m)^[ \t]*%s:$)re"_s, g), whole)) bad = true;
                if (bad) continue;
                lines = cand;
                changed = true;
                break;
            }
        }
    }
    return join_nl(lines);
}

// a register variable given a call's value and read once, by the next statement, and never again: the
// compiler's own keeping of a value it computed first (strlen(b) of strlen(a) + strlen(b)) - the call in its
// place, the variable gone (a variant)
std::optional<str> register_temps(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return std::nullopt;
    Lines lines = split_nl(body);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::match(R"re((\s*)(\w+) = (\w+\((?:[^()]|\([^()]*\))*\));$)re"_s, lines[k]);
        if (!m || !py::re::search(py::percent(R"re((?m)^[ \t]*register [\w ]+\**\s*%s;$)re"_s, m->group(2)), body)) continue;
        str x = m->group(2);
        std::vector<int64_t> rest;
        for (int64_t j = 0; j < lines.size(); ++j)
            if (j != k && py::re::search(py::percent(R"re(\b%s\b)re"_s, x), lines[j]) && !py::re::match(R"re(\s*register\b)re"_s, lines[j]))
                rest.push_back(j);
        if (rest.size() != 1 || rest[0] != k + 1 || py::re::findall(py::percent(R"re(\b%s\b)re"_s, x), lines[k + 1]).size() != 1) continue;
        str call = m->group(3);
        lines[k + 1] = py::re::sub(py::percent(R"re(\b%s\b)re"_s, x), [&](const Match&) { return call; }, lines[k + 1]);
        Lines keep;
        str decl = py::percent(R"re(\s*register [\w ]+\**\s*%s;$)re"_s, x);
        for (int64_t j = 0; j < lines.size(); ++j) {
            if (j == k) continue;   // (the line given None)
            if (!py::re::match(decl, lines[j])) keep.append(lines[j]);
        }
        return head + sep + join_nl(keep);
    }
    return std::nullopt;
}

// a case that goes on into the next (`goto L;` to the label the next case starts with): it falls into it -
// an arm's last `goto L;` is dropped, and `if (C) { X } else { goto L; } break;` is `if (C) { X break; }`
str case_falls(const str& text) {
    Lines lines = split_nl(text);
    int64_t k = 0;
    while (k < lines.size()) {
        OMatch m = py::re::match(R"re((\s*)(L_\w+):$)re"_s, lines[k]);
        int64_t c = k;
        while (c > 0 && py::re::match(R"re(\s*(?:case [^:]+|default):$)re"_s, lines[c - 1])) --c;
        if (!m || c == k) {
            ++k;
            continue;
        }
        str lab = m->group(2), ind = lead(lines[c]);
        str i4 = ind + "    "_s;
        if (lines[c - 1] == i4 + py::percent("goto %s;"_s, lab)) {
            lines.pop(c - 1);
            --k;
        } else if (c >= 5 &&
                   lines.slice(c - 4, c) == Lines{i4 + "} else {"_s, i4 + py::percent("    goto %s;"_s, lab), i4 + "}"_s, i4 + "break;"_s} &&
                   !py::re::match(R"re(\s*(?:return|break|continue|goto)\b)re"_s, lines[c - 5])) {
            lines.set_slice(c - 4, c, Lines{i4 + "    break;"_s, i4 + "}"_s});
            k -= 2;
        } else {
            ++k;
            continue;
        }
        if (!py::re::search(py::percent(R"re(\bgoto %s;)re"_s, lab), join_nl(lines))) lines.pop(k);
        ++k;
    }
    // (an arm's last goto to another case's start, not the next: the statements there to their break, copied -
    //  the compiler's one copy of a tail both cases end with)
    for (int rep = 0; rep < 4; ++rep) {
        bool hit = false;
        for (int64_t j = 0; j < lines.size(); ++j) {
            OMatch g = py::re::fullmatch(R"re((\s*)goto (L_\w+);)re"_s, lines[j]);
            if (!g || j + 1 >= lines.size() || !py::re::match(R"re(\s*(?:case [^:]+|default):$|\s*\}$)re"_s, lines[j + 1])) continue;
            std::optional<int64_t> at, end;
            for (int64_t i = 0; i < lines.size(); ++i)
                if (lines[i].strip() == g->group(2) + ":"_s) { at = i; break; }
            if (!at || *at <= j) continue;
            for (int64_t i = *at + 1; i < lines.size(); ++i)
                if (py::re::match(R"re(\s*(?:break;|return\b))re"_s, lines[i])) { end = i; break; }
            if (!end) continue;
            bool bad = false;
            for (int64_t i = *at + 1; i < *end; ++i)
                if (py::re::match(R"re(\s*(?:case [^:]+|default):$|\s*L_\w+:)re"_s, lines[i])) bad = true;
            if (bad) continue;
            int64_t src_ind = *at + 1 <= *end ? indent_of(lines[*at + 1]) : 0;
            Lines nw;
            for (int64_t i = *at + 1; i < *end + 1; ++i) {
                const str& x = lines[i];
                nw.append(indent_of(x) >= src_ind ? g->group(1) + x.slice_from(src_ind) : x);
            }
            lines.set_slice(j, j + 1, nw);
            if (!py::re::search(py::percent(R"re(\bgoto %s;)re"_s, g->group(2)), join_nl(lines))) {
                Lines keep;
                for (const str& x : lines)
                    if (x.strip() != g->group(2) + ":"_s) keep.append(x);
                lines = keep;
            }
            hit = true;
            break;
        }
        if (!hit) break;
    }
    return join_nl(lines);
}

// a register variable first given a value after other statements: declared in a block from there (to
// before the function's last return) - a variant: before the block the compiler has the register free for
// its own use
std::optional<str> inner_registers(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return std::nullopt;
    Lines lines = split_nl(body);
    for (int64_t j = 0; j < lines.size(); ++j) {
        OMatch m = py::re::match(R"re(    register ([\w ]+?\**) ?(\w+);$)re"_s, lines[j]);
        if (!m) continue;
        str x = m->group(2);
        std::optional<int64_t> first;
        for (int64_t k = j + 1; k < lines.size(); ++k)
            if (py::re::search(py::percent(R"re(\b%s\b)re"_s, x), lines[k])) { first = k; break; }
        if (!first || !py::re::match(py::percent(R"re(    %s = )re"_s, x), lines[*first])) continue;
        bool stmts = false;
        for (int64_t k = j + 1; k < *first; ++k)
            if (nb(lines[k]) && !py::re::match(R"re(\s*(?:register |int |char |unsigned |long |struct ))re"_s, lines[k])) stmts = true;
        if (!stmts) continue;
        std::optional<int64_t> end;
        for (int64_t k = 0; k < lines.size(); ++k)
            if (py::re::match(R"re(    return\b)re"_s, lines[k])) end = k;
        if (!end || *end <= *first) continue;
        Lines nw = lines.slice(0, j) + lines.slice(j + 1, *first);
        nw.append("    {"_s);
        nw.append(py::percent("        register %s%s;"_s, !m->group(1).endswith("*"_s) ? m->group(1) + " "_s : m->group(1), x));
        for (const str& l : lines.slice(*first, *end)) nw.append(nb(l) ? "    "_s + l : l);
        nw.append("    }"_s);
        nw.extend(lines.slice_from(*end));
        return head + sep + join_nl(nw);
    }
    return std::nullopt;
}

// a byte register stored into a local (vN = al) is a char local; a test of the register at once is a test of
// the store - if ((vN = al) != K): the compiler compares what it has - and the register read later, before it is
// given another value, is the local's value. A constant 128..255 it is compared with: (char)0xNN, a byte's
str char_stores(const str& text_in) {
    Lines lines = split_nl(text_in);
    Lines chars;
    int64_t k = 0;
    while (k < lines.size()) {
        OMatch m = py::re::match(R"re((\s*)(v\d+) = ([abcd]l);\s*$)re"_s, lines[k]);
        if (!m) {
            ++k;
            continue;
        }
        str v = m->group(2), r = m->group(3);
        str w = r.slice(0, 1) + "x"_s;
        std::optional<int64_t> nx;
        for (int64_t j = k + 1; j < lines.size(); ++j)
            if (nb(lines[j])) { nx = j; break; }
        if (!nx) break;
        if (py::re::match(py::percent(R"re(\s*(?:if|while) \(.*\b%s\b)re"_s, r), lines[*nx])) {
            lines[*nx] = py::re::sub(py::percent(R"re(\b%s\b)re"_s, r), py::percent("(%s = %s)"_s, v, r), lines[*nx], 1);
            lines.pop(k);
            *nx -= 1;
        } else {
            ++k;
            nx = k;
        }
        if (!chars.contains(v)) chars.append(v);
        for (int64_t j = *nx + 1; j < lines.size(); ++j) {   // (read again before it changes: the local)
            if (py::re::match(R"re(\s*L_\w+:)re"_s, lines[j]) || py::re::search(py::percent(R"re(\b(?:%s|%s) = |\w\()re"_s, r, w), lines[j])) break;
            lines[j] = py::re::sub(py::percent(R"re(\b%s\b)re"_s, r), v, lines[j]);
        }
    }
    if (chars.empty()) return text_in;
    str text = join_nl(lines);
    for (const str& v : chars) {
        text = py::re::sub(py::percent(R"re((?m)^([ \t]*)int %s;)re"_s, v), R"re(\1char %s;)re"_s.replace("%s"_s, v), text);
        text = py::re::sub(py::percent(R"re((\(%s = [abcd]l\)|\b%s) ([!=]=|[<>]=?) (1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b)re"_s, v, v), [](const Match& m) -> str {
            return py::percent("%s %s (char)0x%02x"_s, m.group(1), m.group(2), gi(m, 3));
        }, text);
    }
    return text;
}

// *p = X; p = p + 1; (X not reading p) - the store and the step as one: *p++ = X (a variant: the compiler
// makes other code of the two)
std::optional<str> store_increments(const str& text) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        OMatch m = py::re::match(R"re((\s*)\*(\w+) = (.*);\s*$)re"_s, lines[k]);
        if (m && k + 1 < lines.size() && py::re::match(py::percent(R"re(\s*%s = %s \+ 1;\s*$)re"_s, m->group(2), m->group(2)), lines[k + 1]) &&
            !py::re::search(py::percent(R"re(\b%s\b)re"_s, m->group(2)), m->group(3))) {
            out.append(py::percent("%s*%s++ = %s;"_s, m->group(1), m->group(2), m->group(3)));
            k += 2;
            continue;
        }
        out.append(lines[k]);
        ++k;
    }
    str t = join_nl(out);
    if (t != text) return t;
    return std::nullopt;
}

// r = E; (E a variable, a field, *p) read more than once after, nothing between changing E or calling: E at
// each read - the compiler kept E in the register and read it there (r a machine register left over)
str register_values(const str& text) {
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        OMatch m = py::re::fullmatch(R"re(([ \t]*)(ax|al|bx|bl|cx|cl|dx|dl) = ([A-Za-z_]\w*(?:->f\d+)*|\*[A-Za-z_]\w*);)re"_s, lines[k]);
        if (!m) continue;
        str reg = m->group(2), e = m->group(3);
        Lines names = py::re::findall(R"re([A-Za-z_]\w*)re"_s, e);
        int64_t depth = 0;
        std::vector<int64_t> uses;
        std::optional<int64_t> end = lines.size();
        for (int64_t j = k + 1; j < lines.size(); ++j) {
            const str& x = lines[j];
            depth += x.count("{"_s) - x.count("}"_s);
            if (depth < 0 || py::re::match(py::percent(R"re([ \t]*%s = )re"_s, reg), x) || py::re::match(R"re([ \t]*L_\w+:)re"_s, x)) {
                end = j;
                break;
            }
            bool call = py::re::search(R"re(\w\s*\()re"_s, py::re::sub(R"re(\b(?:if|while|for|switch|return)\s*\()re"_s, ""_s, x)).has_value();
            bool assigns = false;
            for (const str& n : names)
                if (py::re::search(py::percent(R"re((?<![\w>.])%s\s*(?:=[^=]|\+\+|--))re"_s, esc(n)), x)) assigns = true;
            if (call || assigns || py::re::search(py::percent(R"re(%s\s*=[^=])re"_s, esc(e)), x)) {
                end = std::nullopt;
                break;
            }
            if (py::re::search(py::percent(R"re(\b%s\b)re"_s, reg), x)) uses.push_back(j);
        }
        if (!end || uses.size() < 2 || _live_after(opt_of(lines), (*end != 0) ? *end - 1 : lines.size() - 1, reg)) continue;
        for (int64_t j : uses) lines[j] = py::re::sub(py::percent(R"re(\b%s\b)re"_s, reg), e, lines[j]);
        OLines ol = opt_of(lines);
        ol[k] = std::nullopt;
        return join_opt(ol);
    }
    return text;
}

// the shapes a compiler gives a statement, read back: a loop whose test it moved to the bottom (`goto L;
// do {..} while (c);` with no L - the jump goes to the test) is `while (c) {..}`; a step then a read one back
// (`p = p + 1; .. p[-1]`) is `*p++`
str shapes(const str& text_in) {
    str text = text_in;
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        const str& l = lines[k];
        OMatch m = py::re::match(R"re((\s*)goto (L_\w+);\s*$)re"_s, l);
        if (m && k + 1 < lines.size() && lines[k + 1].strip() == "do {"_s &&
            !py::re::search(py::percent(R"re((?m)^\s*%s:)re"_s, m->group(2)), text)) {
            int64_t depth = 0;
            std::optional<int64_t> close;
            for (int64_t j = k + 1; j < lines.size(); ++j) {   // (the do's own closing line)
                depth += lines[j].count("{"_s) - lines[j].count("}"_s);
                if (depth == 0) {
                    close = j;
                    break;
                }
            }
            OMatch mm = close ? py::re::match(R"re((\s*)\} while \((.*)\);\s*$)re"_s, lines[*close]) : std::nullopt;
            if (mm) {
                out.append(py::percent("%swhile (%s) {"_s, m->group(1), mm->group(2)));
                out.extend(lines.slice(k + 2, *close));
                out.append(py::percent("%s}"_s, m->group(1)));
                k = *close + 1;
                continue;
            }
        }
        out.append(l);
        ++k;
    }
    text = join_nl(out);
    // after a return, what stands in the same block before its end (no label) is never reached
    lines = split_nl(text);
    Lines keep;
    std::optional<int64_t> dead;
    for (const str& l : lines) {
        int64_t ind = l.size() - l.lstrip(" "_s).size();
        if (dead) {
            if (ind < *dead || py::re::match(R"re(\s*L_\w+:)re"_s, l)) {   // (out of the return's block: a closing brace, or the next statement after a braceless if's)
                dead = std::nullopt;
            } else {
                continue;
            }
        }
        keep.append(l);
        if (py::re::match(R"re(\s*return\b.*;\s*$)re"_s, l)) dead = ind;
    }
    text = join_nl(keep);
    // a step, then the value one back read: the post-increment (in a do's test, or the next statement)
    OLines ol = opt_of(split_nl(text));
    for (int64_t k2 = 0; k2 < ol.size(); ++k2) {
        if (!ol[k2]) continue;
        OMatch m = py::re::match(R"re(\s*(\w+) = \1 \+ 1;\s*$)re"_s, *ol[k2]);
        if (!m) continue;
        str v = m->group(1);
        for (int64_t j = k2 + 1; j < std::min<int64_t>(k2 + 4, ol.size()); ++j) {
            if (py::re::search(py::percent(R"re(\b%s\[-1\])re"_s, v), *ol[j])) {
                ol[j] = py::re::sub(py::percent(R"re(\b%s\[-1\])re"_s, v), py::percent("*%s++"_s, v), *ol[j], 1);
                ol[k2] = std::nullopt;
                break;
            }
            str s = ol[j]->strip();
            if (s != "}" && !s.empty() && !s.startswith("} while"_s)) break;
        }
    }
    return join_opt(ol);
}

// t without x in its `int a, x, b;` declaration (the line gone if x was all of it)
str _drop_decl(const str& t, const str& x) {
    return py::re::sub(py::percent(R"re((?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);\n)re"_s, esc(x)), [](const Match& m) -> str {
        str rest = (m.group(2) + m.group(3).lstrip(", "_s)).strip(", "_s);
        return !rest.empty() ? py::percent("%sint %s;\n"_s, m.group(1), rest) : ""_s;
    }, t, 1);
}

// t with x's declaration (`int a, x, b;` - a parameter's, a local's) as `new;`, or None
std::optional<str> _redeclare(const str& t, const str& x, const str& new_) {
    OMatch d = py::re::search(
        py::percent(R"re((?m)^([ \t]*)(register\s+)?(struct \w+|\w+)\s*\*?\s*((?:\w+, )*)%s((?:, \w+)*);$)re"_s, esc(x)), t);
    if (!d || d->group(3) == "return" || d->group(3) == "goto" || d->group(3) == "extern") return std::nullopt;
    str rest = (d->group(4) + d->group(5).lstrip(", "_s)).strip(", "_s);
    str reg = gopt(*d, 2);
    str rep = (!rest.empty() ? py::percent("%s%s%s %s;\n"_s, d->group(1), reg, d->group(3), rest) : ""_s) +
              py::percent("%s%s%s;"_s, d->group(1), reg, new_);
    return t.slice(0, d->start()) + rep + t.slice_from(d->end());
}

}  // namespace codebreaker::exact::draft::forms
