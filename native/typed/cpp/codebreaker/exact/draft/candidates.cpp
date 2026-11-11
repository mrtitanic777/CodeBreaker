// codebreaker/exact/draft/candidates.cpp -- typed C++ from codebreaker.exact.draft.candidates (see candidates.hpp).
#include "codebreaker/exact/draft/candidates.hpp"

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <variant>
#include <vector>

#include "codebreaker/exact/data.hpp"
#include "codebreaker/exact/draft/shapes.hpp"
#include "codebreaker/exact/x86.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::draft::candidates {

namespace {

using py::str;
using Lines = py::list<str>;
using OLines = py::list<std::optional<str>>;
using Match = py::re::Match;
using OMatch = std::optional<Match>;
using text::_balanced;
using text::_changed;
using text::_declare_pointer;
using text::_if_parts;
using text::_same_arm;
using text::_strip;
using text::_top_commas;
using text::_top_split;
using text::split_body;
using forms::_drop_decl;
using forms::_redeclare;
namespace S = shapes;
namespace F = forms;
namespace T = text;

str operator""_s(const char* p, size_t n) { return str(std::string(p, n)); }

Lines split_nl(const str& t) { return py::split(t, "\n"_s); }
str join_nl(const Lines& l) { return py::join("\n"_s, l); }

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

bool nb(const str& x) { return !x.strip().empty(); }
str esc(const str& s) { return py::re::escape(s); }
int64_t gi(const Match& m, int64_t g) { return py::int_from_str(m.group(g)); }
str gopt(const Match& m, int64_t g) { return m.group_opt(g).value_or(str("")); }
str lead(const str& l) { return py::re::match(R"re(\s*)re"_s, l)->group(0); }

// `x or y` for a pass that gives Optional[str] (an empty text is false too)
str or_str(const std::optional<str>& a, const str& b) { return (a && !a->empty()) ? *a : b; }

// the pattern with %s ... filled (py::percent)
template <class... A> str pf(const char* fmt, const A&... a) { return py::percent(str(std::string(fmt)), a...); }

// sorted(set(xs)) of strings, as a list
Lines sorted_unique(const Lines& xs) {
    std::vector<str> v;
    for (const str& x : xs)
        if (std::find(v.begin(), v.end(), x) == v.end()) v.push_back(x);
    std::sort(v.begin(), v.end());
    return Lines(v);
}
// the distinct strings, first seen first (a dict.fromkeys / a set read in a fixed order)
Lines unique(const Lines& xs) {
    Lines out;
    for (const str& x : xs)
        if (!out.contains(x)) out.append(x);
    return out;
}

// s.split(sep, 1)[-1]
str last_after(const str& s, const str& sep) {
    int64_t i = s.find(sep);
    return i < 0 ? s : s.slice_from(i + sep.size());
}

// an insertion-ordered dict of strings to values (Python's dict: the order a draft's names were met in)
template <class V> struct OrdMap {
    std::vector<std::pair<str, V>> items;
    V* find(const str& k) {
        for (auto& kv : items)
            if (kv.first == k) return &kv.second;
        return nullptr;
    }
    V& at_or_add(const str& k) {   // d.setdefault(k, V())
        if (V* v = find(k)) return *v;
        items.push_back({k, V()});
        return items.back().second;
    }
    void erase(const str& k) {
        for (size_t i = 0; i < items.size(); ++i)
            if (items[i].first == k) {
                items.erase(items.begin() + i);
                return;
            }
    }
    bool contains(const str& k) { return find(k) != nullptr; }
};

// set.clear()
template <class K> void clear_set(const py::set<K>& s) {
    std::vector<K> ks(s.begin(), s.end());
    for (const K& k : ks) s.discard(k);
}

str hex4(int64_t n) { return pf("g_%04x", n); }

bool has_uchar_off(int64_t o) {
    for (const auto& u : T::UCHAR)
        if (u.index() == 0 && std::get<1>(std::get<0>(u)) == o) return true;
    return false;
}

bool longfield(int64_t o) { return T::LONGINFO.fields && T::LONGINFO.fields->contains(o); }
void add_field(int64_t o) {
    if (!T::LONGINFO.fields) T::LONGINFO.fields = py::set<int64_t>();
    T::LONGINFO.fields->add(o);
}
void add_global(const str& g) {
    if (!T::LONGINFO.globals) T::LONGINFO.globals = py::set<str>();
    T::LONGINFO.globals->add(g);
}
void add_func(const str& f) {
    if (!T::LONGINFO.funcs) T::LONGINFO.funcs = py::set<str>();
    T::LONGINFO.funcs->add(f);
}

// \w+ at the start of s (re.match(r"\w+", s).group(0): AttributeError when none)
str word_prefix(const str& s) {
    OMatch m = py::re::match(R"re(\w+)re"_s, s);
    if (!m) throw py::AttributeError("'NoneType' object has no attribute 'group'");
    return m->group(0);
}

using SA = std::variant<str, py::list<std::tuple<int64_t, str>>>;
std::variant<std::tuple<str, int64_t>, str> SAv(const str& d) { return std::variant<std::tuple<str, int64_t>, str>(d); }

}  // namespace

FpInfo FPINFO;
const py::dict<str, str> STRUCTPTR;
const py::set<std::tuple<int64_t, str>> FRAMEW;
const py::list<int64_t> FRAME = py::list<int64_t>{0};
const py::list<str> FILLNAME = py::list<str>{str("_pf")};

// the pointers the text goes through to fields (`p->fN`, N its offset) declared pointers to a struct of
// those fields: a char where the code reads a byte (widths: {(pointer, offset)} of the byte ones), else an
// int, and a pad between
str structs(str text, const Widths& widths) {
    auto W = [&](const str& p, int64_t o) { return widths.contains(std::make_tuple(p, o)); };
    text = py::re::sub(R"re(&(\w+)->f(\d+)\b)re"_s, R"re((\1 + \2))re"_s, text);   // (lea ax,[p+N] - the model's &p->fN - is p + N)
    text = py::re::sub(R"re(&(\w+)\[(-?)(\d+)\])re"_s, [](const Match& m) -> str {
        return pf("(%s %s %s)", m.group(1), m.group(2).empty() ? "+"_s : "-"_s, m.group(3));
    }, text);
    // (a routine's value gone through to a field: a pointer to a struct it returns - cast)
    const str callrx = R"re(\b((?!if\b|while\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\([^()]*\))*)\)->f(\d+)\b)re"_s;
    std::map<std::string, std::set<int64_t>> rets;
    for (const Match& m : py::re::finditer(callrx, text)) rets[m.group(1).utf8()].insert(gi(m, 3));
    Lines pre;
    for (const auto& kv : rets) {
        Lines fl;
        int64_t pos = 0;
        for (int64_t o : kv.second) {
            if (o > pos) fl.append(pf("char _%d[%d];", pos, o - pos));
            bool b_ = W("bx*"_s, o) && !W("bxw"_s, o);
            fl.append(pf("%s f%d;", b_ ? "char"_s : "int"_s, o));
            pos = o + (b_ ? 1 : 2);
        }
        pre.append(pf("struct s_r_%s { %s };", str(kv.first), py::join(" "_s, fl)));
    }
    if (!rets.empty()) {
        text = py::re::sub(callrx, [](const Match& m) -> str {
            return pf("((struct s_r_%s *)%s(%s))->f%s", m.group(1), m.group(1), m.group(2), m.group(3));
        }, text);
        text = join_nl(pre) + "\n"_s + text;
    }
    if (W("bx*"_s, 0) && !W("bxw"_s, 0))   // (*p->fA->fB read a byte: a char there)
        text = py::re::sub(R"re((?<![\w>)\]])\*(\w+(?:->f\d+){2,})\b(?!\s*(?:->|\[|\.|\+\+|--)))re"_s, R"re(*(char *)\1)re"_s, text);
    OrdMap<std::set<int64_t>> uses;
    for (const Match& m : py::re::finditer(R"re(\b(\w+)->f(\d+)\b)re"_s, text)) {
        if (m.group(1).startswith("s_r_"_s)) continue;
        uses.at_or_add(m.group(1)).insert(gi(m, 2));
    }
    {
        // (a pointer read through twice: a struct whose first field points on - a list)
        py::list<Match> twice = py::re::finditer(R"re(\*\(\*(\w+)\)|\*\*(\w+))re"_s, text);
        for (const Match& m : twice) {
            str p = !gopt(m, 1).empty() ? m.group(1) : m.group(2);
            if (py::re::search(pf(R"re((?m)^[ \t]*(?:register )?int %s;|^[ \t]*int (?:\w+, )*%s\b)re", p, p), text)) {
                text = py::re::sub(pf(R"re(\*\(\*%s\)|\*\*%s\b)re", p, p), pf("(%s->f0)->f0", p), text);
                uses.at_or_add(p).insert(0);
            }
        }
    }
    if (uses.items.empty()) return text;
    for (auto& kv : uses.items) {   // (p + N, the place of a field - lea - is &p->fN)
        const str& p = kv.first;
        Lines ns = unique(py::re::findall(pf(R"re(\(%s \+ (\d+)\))re", p), text));
        for (const str& n : ns)
            if (py::int_from_str(n) < 0x100) {
                kv.second.insert(py::int_from_str(n));
                text = text.replace(pf("(%s + %s)", p, n), pf("&%s->f%s", p, n));
            }
    }
    const py::re::Pattern declre = py::re::compile(R"re(\s*(?:extern\s+|register\s+|static\s+)*(?:unsigned\s+)?(?:char|int|long|short|struct\s+\w+)\b)re"_s);
    for (auto& kv : uses.items) {   // (*p of a struct pointer: its first field)
        const str& p = kv.first;
        py::re::Pattern whole = py::re::compile(pf(R"re((?<![\w>)\]])\*%s\b(?!\s*(?:->|\[|\+\+|--|\.)))re", p));
        Lines lines = split_nl(text);
        bool any_ = false;
        for (const str& l : lines)
            if (!declre.match(l) && whole.search(l)) any_ = true;
        if (any_) {
            Lines out;
            for (const str& l : lines) out.append(declre.match(l) ? l : whole.sub(pf("%s->f0", p), l));
            text = join_nl(out);
            kv.second.insert(0);
        }
    }
    for (const auto& kv0 : std::vector<std::pair<str, std::set<int64_t>>>(uses.items)) {   // (a pointer stepped and read as *p++, its places all bytes: a char *, the places p[N])
        const str& p = kv0.first;
        const std::set<int64_t>& offs = kv0.second;
        auto bytes_ = [&](int64_t o) {
            return W(p, o) || W("*"_s, o) || (p.startswith("r_"_s) && W(p.slice_from(2) + "*"_s, o) && !W(p.slice_from(2) + "w"_s, o));
        };
        bool allb = true;
        for (int64_t o : offs)
            if (!bytes_(o)) allb = false;
        if (py::re::search(pf(R"re(\*(?:%s(?:\+\+|--)|(?:\+\+|--)%s\b))re", esc(p), esc(p)), text) && !offs.empty() && *offs.rbegin() < 16 && allb) {
            str t2 = py::re::sub(pf(R"re(\b%s->f(\d+)\b)re", esc(p)), [&](const Match& m) -> str {
                return m.group(1) == "0" ? pf("*%s", p) : pf("%s[%s]", p, m.group(1));
            }, text);
            std::optional<str> t3 = !py::re::search(pf(R"re((?m)^[ \t]*(?:register )?char \*%s;)re", esc(p)), t2) ? _declare_pointer(t2, p, "char"_s)
                                                                                                                      : std::optional<str>(t2);
            if (t3 && !t3->empty()) {
                text = *t3;
                uses.erase(p);
                continue;
            }
        }
    }
    for (const auto& kv0 : std::vector<std::pair<str, std::set<int64_t>>>(uses.items)) {   // (a byte read inside a word read: no struct - the words through an int *, the bytes through a char * of it)
        const str& p = kv0.first;
        const std::set<int64_t>& offs = kv0.second;
        auto byte = [&](int64_t o) {
            return W(p, o) || W("*"_s, o) || (p.startswith("r_"_s) && W(p.slice_from(2) + "*"_s, o) && !W(p.slice_from(2) + "w"_s, o));   // (a register variable's bytes: its register's)
        };
        std::set<int64_t> words;
        for (int64_t o : offs)
            if (!byte(o)) words.insert(o);
        bool a1 = false, a2 = true;
        for (int64_t o : offs)
            if (byte(o) && (o % 2) && words.count(o - 1)) a1 = true;
        for (int64_t o : words)
            if (o % 2 != 0) a2 = false;
        if (a1 && a2) {
            str t2 = py::re::sub(pf(R"re(\b%s->f(\d+)\b)re", p), [&](const Match& m) -> str {
                int64_t n = gi(m, 1);
                if (byte(n) && n % 2) return pf("((char *)%s)[%s]", p, m.group(1));
                return m.group(1) == "0" ? pf("*%s", p) : pf("%s[%d]", p, n / 2);
            }, text);
            str t3 = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?char \*%s;$)re", p), R"re(\1\2int *%s;)re"_s.replace("%s"_s, p), t2);
            std::optional<str> t2o = t3 != t2 ? std::optional<str>(t3) : _declare_pointer(t2, p, "int"_s);
            if (t2o && !t2o->empty()) {
                text = *t2o;
                uses.erase(p);
                continue;
            }
        }
        // (stepped by one, bytes alone read through it: a char *, its bytes p[N])
        std::set<std::string> steps;
        for (const str& s : py::re::findall(pf(R"re(\b%s = %s [+-] (\d+);)re", p, p), text)) steps.insert(s.utf8());
        if (py::re::search(pf(R"re((?<![\w>.])%s(?:\+\+|--)|(?:\+\+|--)%s\b)re", p, p), text)) steps.insert("1");
        bool allbyte = true;
        for (int64_t o : offs)
            if (!byte(o)) allbyte = false;
        if (steps == std::set<std::string>{"1"} && allbyte && !offs.empty() && *offs.rbegin() < 0x10 &&
            !py::re::search(pf(R"re(\b%s->f\d+->)re", p), text)) {
            str t2 = py::re::sub(pf(R"re(\b%s->f(\d+)\b)re", p), [&](const Match& m) -> str {
                return m.group(1) == "0" ? pf("*%s", p) : pf("%s[%s]", p, m.group(1));
            }, text);
            std::optional<str> t2o = t2;
            if (!py::re::search(pf(R"re((?m)^[ \t]*(?:register\s+)?char \*%s;$)re", p), t2)) t2o = _declare_pointer(t2, p, "char"_s);
            if (t2o && !t2o->empty()) {
                text = *t2o;
                uses.erase(p);
            }
        }
    }
    // (a field through which the code goes on - p->fN->fM, (p->fN)->fM, *(p->fN): a pointer to a struct of its
    //  own; the same struct when what it reaches are fields p has too - a list, a tree)
    std::map<std::pair<std::string, int64_t>, std::set<int64_t>> inner;
    text = py::re::sub(R"re(\*\((\w+)->f(\d+)\)(?!\s*(?:->|\[)))re"_s, R"re((\1->f\2)->f0)re"_s, text);
    for (const Match& m : py::re::finditer(R"re(\b(\w+)->f(\d+)\)?->f(\d+)\b)re"_s, text))
        if (uses.contains(m.group(1))) inner[{m.group(1).utf8(), gi(m, 2)}].insert(gi(m, 3));
    text = py::re::sub(R"re(\((\w+->f\d+)\)->)re"_s, R"re(\1->)re"_s, text);
    // (a whole deref of a field chain that goes on as a pointer elsewhere - *p->fN, *p->fN->fM: its first field)
    for (int rep = 0; rep < 3; ++rep) {
        std::vector<std::pair<str, str>> chains;
        for (const Match& m : py::re::finditer(R"re((?<![\w>)\]])\*(\w+)((?:->f\d+)+)\b(?!\s*(?:->|\[|\.)))re"_s, text)) {
            std::pair<str, str> pc{m.group(1), m.group(2)};
            if (std::find(chains.begin(), chains.end(), pc) == chains.end()) chains.push_back(pc);
        }
        for (const auto& pc : chains) {
            const str& p = pc.first;
            const str& chain = pc.second;
            Lines fs = py::re::findall(R"re(->f(\d+))re"_s, chain);
            int64_t n = py::int_from_str(fs[-1]);
            if (uses.contains(p) && (py::re::search(pf(R"re(\b%s%s->f\d+)re", p, esc(chain)), text) || py::split(chain, "->"_s).size() > 2)) {
                text = py::re::sub(pf(R"re((?<![\w>)\]])\*%s%s\b(?!\s*(?:->|\[|\.)))re", p, esc(chain)), pf("%s%s->f0", p, chain), text);
                if (py::split(chain, "->"_s).size() == 2) inner[{p.utf8(), n}].insert(0);
            }
        }
    }
    std::map<std::pair<std::string, int64_t>, str> ptrfield;
    Lines defs;
    std::map<std::string, std::set<int64_t>> deeper;   // (what chains reach further down: p->fA->fB->fC..)
    for (const Match& m : py::re::finditer(R"re(\b(\w+)((?:->f\d+){3,}))re"_s, text))
        if (uses.contains(m.group(1))) {
            Lines os = py::re::findall(R"re(->f(\d+))re"_s, m.group(2));
            for (int64_t i = 2; i < os.size(); ++i) deeper[m.group(1).utf8()].insert(py::int_from_str(os[i]));
        }
    for (const auto& kv : inner) {
        str p(kv.first.first);
        int64_t n = kv.first.second;
        const std::set<int64_t>& subs = kv.second;
        std::set<int64_t> bytes_;   // (a byte the code reads where p has a word: another struct)
        for (int64_t o : subs)
            if (W("*"_s, o)) bytes_.insert(o);
        // (what bx - the register the model goes on through - reads at an offset, beside what the others read
        //  there: a byte through one and a word through the other is two structs)
        auto width = [&](std::initializer_list<const char*> regs, int64_t o) {
            std::set<std::string> out;
            for (const char* r : regs) {
                std::string w = std::string(W(str(std::string(r) + "*"), o) ? "b" : "") + (W(str(std::string(r) + "w"), o) ? "w" : "");
                if (!w.empty()) out.insert(w);
            }
            return out;
        };
        std::set<int64_t> both;
        for (int64_t o : subs) {
            auto wb = width({"bx"}, o), wsd = width({"si", "di"}, o);
            if (!wb.empty() && !wsd.empty() && wb != wsd) both.insert(o);
        }
        std::set<int64_t> nb2;
        for (int64_t o : subs) {
            auto wb = width({"bx"}, o);
            std::string joined;
            for (const auto& w : wb) joined += w;
            if (joined.find('b') != std::string::npos || (wb.empty() && bytes_.count(o))) nb2.insert(o);
        }
        bytes_ = nb2;
        const std::set<int64_t>& up = *uses.find(p);
        bool sub_of_up = std::includes(up.begin(), up.end(), subs.begin(), subs.end());
        std::set<int64_t> up0 = up;
        up0.insert(0);
        bool sub_of_up0 = std::includes(up0.begin(), up0.end(), subs.begin(), subs.end());
        std::set<int64_t> kset{n};
        for (const auto& k2 : inner)
            if (k2.first.first == kv.first.first) kset.insert(k2.first.second);
        bool inter = false;
        for (int64_t o : bytes_)
            if (kset.count(o)) inter = true;
        if ((sub_of_up || (n == 0 && sub_of_up0)) && both.empty() && !inter) {
            ptrfield[kv.first] = pf("struct s_%s *", p);
            auto d = deeper.find(p.utf8());
            if (d != deeper.end()) uses.find(p)->insert(d->second.begin(), d->second.end());   // (p's own struct: what the chains reach further too)
            continue;
        }
        Lines fl;
        int64_t pos = 0;
        for (int64_t o : subs) {
            if (o > pos) fl.append(pf("char _%d[%d];", pos, o - pos));
            bool b_ = bytes_.count(o) > 0;   // (a byte read: a char - an unsigned one widened with zeros)
            OMatch pg = py::re::search(pf(R"re(\b%s->f%d->f%d = (\w+);)re", p, n, o), text);   // (given a pointer variable: a pointer)
            OMatch pty = pg ? py::re::search(pf(R"re((?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$)re", pg->group(1)), text)
                            : std::nullopt;
            std::set<int64_t> deepset;
            for (const str& x : py::re::findall(pf(R"re(\b%s->f%d->f%d->f(\d+)\b)re", p, n, o), text)) deepset.insert(py::int_from_str(x));
            std::vector<int64_t> deep(deepset.begin(), deepset.end());
            if (!deep.empty() && !b_) {   // (gone through once more: a pointer to a struct of its own)
                Lines dl;
                int64_t dp = 0;
                bool last_long = false;
                for (int64_t x : deep) {
                    if (x < dp) continue;
                    if (x > dp) dl.append(pf("char _%d[%d];", dp, x - dp));
                    last_long = longfield(x) && !deepset.count(x + 2);
                    dl.append(pf("%s f%d;", last_long ? "long"_s : "int"_s, x));
                    dp = x + (last_long ? 4 : 2);
                }
                defs.append(pf("struct s_%s_%d_%d { %s };", p, n, o, py::join(" "_s, dl)));
                pty = std::nullopt;
            }
            bool lng_ = !b_ && deep.empty() && !pty && longfield(o) && !subs.count(o + 2);   // (a long one)
            str ty = (has_uchar_off(o) && b_) ? "unsigned char"_s
                     : b_                    ? "char"_s
                     : !deep.empty()         ? pf("struct s_%s_%d_%d *", p, n, o)
                     : pty                   ? pf("%s *", pty->group(1))
                     : lng_                  ? "long"_s
                                             : "int"_s;
            fl.append(pf("%s f%d;", ty, o));
            pos = o + (b_ ? 1 : lng_ ? 4 : 2);
        }
        defs.append(pf("struct s_%s_%d { %s };", p, n, py::join(" "_s, fl)));
        ptrfield[kv.first] = pf("struct s_%s_%d *", p, n);
    }
    {
        std::vector<std::pair<str, std::set<int64_t>>> sorted_uses(uses.items);
        std::sort(sorted_uses.begin(), sorted_uses.end(), [](const auto& a, const auto& b) { return a.first < b.first; });
        for (const auto& kv0 : sorted_uses) {
            const str& p = kv0.first;
            const std::set<int64_t>& offs = kv0.second;
            Lines fields;
            int64_t pos = 0;
            bool selfref = py::re::search(pf(R"re(\b%s = %s->f0;)re", p, p), text).has_value();   // (p = p->f0: a list - the field points on)
            for (int64_t off : offs) {
                if (off < pos) continue;   // (overlaps the field before: not this way)
                if (off > pos) fields.append(pf("char _%d[%d];", pos, off - pos));
                bool uch = has_uchar_off(off);   // (widened with zeros: unsigned)
                bool byte = W(p, off) || W("*"_s, off) || uch ||
                            (p.startswith("r_"_s) && W(p.slice_from(2) + "*"_s, off) && !W(p.slice_from(2) + "w"_s, off));   // (a register variable's)
                if (byte && !uch && !W(p, off) && p.startswith("g_"_s) && W("bxw"_s, off) && !W("bx*"_s, off))
                    byte = false;   // (data's pointer, gone through in bx: bx's width there)
                uch = uch || (byte && py::re::search(pf(R"re(\b%s->f%d \S{1,2} (?:12[89]|1[3-9]\d|2[0-4]\d|25[0-5])\b)re", p, off), text));   // (a byte compared with 128..255: unsigned)
                bool deref = py::re::search(pf(R"re((?<![\w>)\]])\*%s->f%d\b(?!\s*(?:->|\[|\.)))re", p, off), text).has_value();   // (*p->fN: a pointer)
                bool lng = longfield(off) && !offs.count(off + 2);   // (a long field: two words given as one)
                if (lng && byte && !W(p, off) && !W(p.lstrip("r_"_s) + "*"_s, off)) byte = false;   // (a byte another pointer reads there: not this one's)
                OMatch pg = py::re::search(pf(R"re(\b%s->f%d = (\w+);)re", p, off), text);
                if (!pg) pg = py::re::search(pf(R"re((?m)^[ \t]*(\w+) = %s->f%d = )re", p, off), text);   // (given a pointer variable, or given with one in a chain: a pointer - not another's byte)
                OMatch pty = pg ? py::re::search(pf(R"re((?m)^[ \t]*(?:register\s+)?((?:unsigned\s+)?(?:char|int)|struct \w+) \*%s;$)re", pg->group(1)), text)
                                : std::nullopt;
                if (pty && byte && !W(p, off)) byte = false;
                deref = deref || (pty && pty->group(1) == "int" && !byte && !lng);
                std::pair<std::string, int64_t> key{p.utf8(), off};
                if (pty && pty->group(1) != "int" && !byte && !lng && !ptrfield.count(key) && !(off == 0 && selfref))
                    ptrfield[key] = pf("%s *", pty->group(1));
                str ty = (off == 0 && selfref)     ? pf("struct s_%s *", p)
                         : ptrfield.count(key)    ? ptrfield[key]
                         : uch                    ? "unsigned char"_s
                         : byte                   ? "char"_s
                         : deref                  ? ((W("bx*"_s, 0) && !W("bxw"_s, 0)) ? "char *"_s : "int *"_s)   // (*p->fN read a byte: a char *)
                         : lng                    ? "long"_s
                                                  : "int"_s;
                fields.append(pf("%s f%d;", ty, off));
                pos = off + (ptrfield.count(key) ? 2 : byte ? 1 : lng ? 4 : 2);
            }
            str tag = "s_"_s + p;
            std::set<int64_t> steps;
            for (const str& n : py::re::findall(pf(R"re(\b%s = %s [+-] (\d+);)re", p, p), text)) steps.insert(py::int_from_str(n));
            if (steps.size() == 1) {   // (stepped through an array of them: the step is the struct's size, and in C a step of one)
                int64_t n = *steps.begin();
                if (n >= pos && n < 0x100) {
                    if (n > pos) fields.append(pf("char _%d[%d];", pos, n - pos));
                    text = py::re::sub(pf(R"re(\b%s = %s ([+-]) %d;)re", p, p, n), pf(R"re(%s = %s \1 1;)re", p, p), text);
                }
            }
            defs.append(pf("struct %s { %s };", tag, py::join(" "_s, fields)));
            // the declarations of p: a parameter, a register variable, a local, or data (declared by localize)
            text = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?(?:int|char \*) ?%s;)re", p), pf(R"re(\1\2struct %s *%s;)re", tag, p), text);
            text = py::re::sub(pf(R"re((?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);)re", p), [&](const Match& m) -> str {
                str rest = (m.group(2) + m.group(3).lstrip(", "_s)).strip(", "_s);
                return !rest.empty() ? pf("%sint %s;\n%sstruct %s *%s;", m.group(1), rest, m.group(1), tag, p) : pf("%sstruct %s *%s;", m.group(1), tag, p);
            }, text);
            STRUCTPTR.set(p, tag);
            for (const auto& pt : ptrfield)   // (a struct pointer field plus bytes: through a char *)
                if (pt.first.first == p.utf8())
                    text = py::re::sub(pf(R"re((?<![\w>)])%s->f%d \+ (\d+)\b)re", p, pt.first.second), pf(R"re((char *)%s->f%d + \1)re", p, pt.first.second), text);
            for (const str& f_ : py::re::findall(R"re(\bchar f(\d+);)re"_s, defs[-1]))   // (a byte field masked to a byte: itself)
                text = py::re::sub(pf(R"re(\(%s->f%s & 0xff\))re", p, f_), pf("%s->f%s", p, f_), text);
            bool anystar = W("*"_s, 0);
            for (const auto& w : widths)
                if (std::get<0>(w).endswith("*"_s)) anystar = true;
            if (anystar) text = py::re::sub(pf(R"re((?<![\w>.])%s\[)re", p), pf("((char *)%s)[", p), text);   // (indexed by bytes: through a char *)
        }
    }
    return join_nl(defs) + "\n"_s + text;
}

// the fields the code reads or writes a byte of: the model's checker mode marks byte-wide memory as BYTE(x)
ByteFields byte_fields(const Inv& inv, const py::bytes& code) {
    ByteFields out;
    if (!inv.has_mark) return out;
    std::optional<str> t = inv.checked_c(code);
    if (!t) return out;
    for (const Match& m : py::re::finditer(R"re(BYTE\(\s*\(?(\w+)->f(\d+)\)?\s*\))re"_s, *t)) out.fields.add(std::make_tuple(m.group(1), gi(m, 2)));
    for (const str& p : py::re::findall(R"re(BYTE\(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)?\))re"_s, *t))   // (*p a byte: a char *)
        out.fields.add(std::make_tuple(p, int64_t(0)));
    // (and from the code itself: a byte read or written
    clear_set(FRAMEW);
    clear_set(T::RANGES);
    py::list<x86::Insn> insns = x86::disasm(code, 0, code.size(), false);
    for (int64_t k = 0; k < insns.size() - 4; ++k) {   // (a register loaded and tested against a range: a switch's)
        const x86::Insn &a_ = insns[k + 1], &b_ = insns[k + 2], &c_ = insns[k + 3], &d_ = insns[k + 4];
        std::optional<str> lo_;
        if (a_.mnemonic == "or" && a_.op_str == "ax, ax") lo_ = "0"_s;
        else if (a_.mnemonic == "cmp") lo_ = a_.op_str.slice_from(4);
        if (insns[k].mnemonic == "mov" && insns[k].op_str.startswith("ax, "_s) && lo_ && c_.mnemonic == "cmp" && a_.op_str.startswith("ax, "_s) &&
            c_.op_str.startswith("ax, "_s) && (b_.mnemonic == "jl" || b_.mnemonic == "jb") &&
            (d_.mnemonic == "jg" || d_.mnemonic == "ja" || d_.mnemonic == "jle" || d_.mnemonic == "jbe")) {
            try {   // (jg/ja to the default: the cases' code first)
                T::RANGES.add(std::make_tuple(py::int_parse(*lo_, 0), py::int_parse(c_.op_str.slice_from(4), 0), d_.mnemonic == "jg" || d_.mnemonic == "ja"));
            } catch (const py::ValueError&) {
            }
        }
    }
    for (int64_t k = 0; k < insns.size(); ++k) {   // (a slot's byte loaded and widened with zeros: unsigned)
        const x86::Insn& i = insns[k];
        OMatch z = i.mnemonic == "mov" ? py::re::fullmatch(R"re(al, byte ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\])re"_s, i.op_str) : std::nullopt;
        if (!z) continue;
        bool any_ = false;
        for (int64_t j = k + 1; j < std::min<int64_t>(k + 4, insns.size()); ++j) {
            const x86::Insn& x = insns[j];
            if ((x.op_str == "ah, ah" || x.op_str == "ah, 0") && (x.mnemonic == "sub" || x.mnemonic == "xor" || x.mnemonic == "mov")) any_ = true;
        }
        if (any_) FRAMEW.add(std::make_tuple((z->group(1) == "+" ? 1 : -1) * py::int_parse(z->group(2), 0), "zx"_s));
    }
    for (const x86::Insn& i : insns) {   // through a register at an offset - whatever the draft names the pointer)
        OMatch m = py::re::search(R"re(byte ptr \[(?:bx|si|di) \+ (0x[0-9a-f]+|\d+)\])re"_s, i.op_str);
        if (m && py::int_parse(m->group(1), 0) < 0x100) out.fields.add(std::make_tuple("*"_s, py::int_parse(m->group(1), 0)));
        else if (m) out.fields.add(std::make_tuple("data"_s, py::int_parse(m->group(1), 0)));   // (a byte in the data indexed by a register)
        for (const Match& w : py::re::finditer(R"re((byte|word) ptr \[(bx|si|di)(?: \+ (0x[0-9a-f]+|\d+))?\])re"_s, i.op_str)) {
            str o0 = gopt(w, 3);
            int64_t o_ = py::int_parse(o0.empty() ? "0"_s : o0, 0);   // (each width at each offset through each register: an offset read both ways is two structs')
            if (o_ < 0x100) out.fields.add(std::make_tuple(w.group(2) + (w.group(1) == "byte" ? "*"_s : "w"_s), o_));
        }
        OMatch g = py::re::search(R"re(byte ptr \[(0x[0-9a-f]+|\d+)\])re"_s, i.op_str);   // (a global read or written a byte at a time - never a word: a word's low byte)
        if (g) {
            bool word = false;
            for (const x86::Insn& x : insns)
                if (x.op_str.contains(pf("word ptr [%s]", g->group(1)))) word = true;
            if (!word) out.globals.add(hex4(py::int_parse(g->group(1), 0)));
        }
        OMatch f = py::re::search(R"re((byte|word) ptr \[bp ([+-]) (0x[0-9a-f]+|\d+)\])re"_s, i.op_str);   // (a frame slot by its width: FRAMEW: (+-N, width))
        if (f) FRAMEW.add(std::make_tuple((f->group(2) == "+" ? 1 : -1) * py::int_parse(f->group(3), 0), f->group(1)));
    }
    return out;
}

// the locals and parameters the code only ever reads or writes a byte of (its frame slots, by width): chars
str frame_chars(str text) {
    std::map<int64_t, std::set<std::string>> slots;
    for (const auto& ow : FRAMEW) slots[std::get<0>(ow)].insert(std::get<1>(ow).utf8());
    std::vector<std::pair<str, str>> chars;
    for (const auto& kv : slots) {
        int64_t off = kv.first;
        std::set<std::string> ws = kv.second;
        ws.erase("zx");
        if (ws != std::set<std::string>{"byte"}) continue;
        std::optional<str> name;
        if (off < 0) name = pf("v%d", -off);
        else if (off >= 4 && off % 2 == 0) name = py::chr('a' + (off - 4) / 2);
        if (name) chars.push_back({*name, kv.second.count("zx") ? "unsigned char"_s : "char"_s});   // (widened with zeros where it is read: unsigned)
    }
    for (const auto& xt : chars) {
        const str& x = xt.first;
        const str& ty = xt.second;
        text = py::re::sub(pf(R"re((?m)^([ \t]*)int %s;$)re", x), R"re(\1%s %s;)re"_s.replace("%s %s"_s, ty + " "_s + x), text);
        text = py::re::sub(pf(R"re((?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$)re", x), [&](const Match& m) -> str {
            return pf("%sint %s;\n%s%s %s;", m.group(1), (m.group(2) + m.group(3).lstrip(", "_s)).strip(", "_s), m.group(1), ty, x);
        }, text);
        if (ty != "char") text = py::re::sub(pf(R"re((?m)^([ \t]*)char %s;$)re", x), R"re(\1%s %s;)re"_s.replace("%s %s"_s, ty + " "_s + x), text);
    }
    return text;
}

// the locals whose address the text takes (`&vN`) as char arrays, each as long as the frame has room before
// the next local (vN is at bp-N); a store into one goes through an int
std::optional<str> buffers(str text) {
    std::set<int64_t> bps;
    for (const str& x : py::re::findall(R"re(\bbp - (\d+)\b)re"_s, text)) bps.insert(py::int_from_str(x));
    for (int64_t n : bps) {   // (the frame's place bp-N: a local there, in place of the filler that stood for it)
        if (!py::re::search(pf(R"re((?m)^[ \t]*int v%d;)re", n), text)) {
            py::list<Match> f = py::re::finditer(R"re((?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n)re"_s, text);
            if (f.empty()) continue;
            text = text.slice(0, f[-1].start()) + pf("%sint v%d;\n", f[-1].group(1), n) + text.slice_from(f[-1].end());
        }
        text = py::re::sub(pf(R"re(\bbp - %d\b)re", n), pf("&v%d", n), text);
    }
    std::set<int64_t> locs;
    for (const str& x : py::re::findall(R"re((?m)^[ \t]*int v(\d+);)re"_s, text)) locs.insert(py::int_from_str(x));
    std::vector<int64_t> taken;
    for (int64_t n : locs)
        if (py::re::search(pf(R"re(&v%d\b)re", n), text)) taken.push_back(n);
    if (taken.empty()) return std::nullopt;
    for (int64_t n : taken) {
        std::optional<int64_t> mx;
        for (int64_t m : locs)
            if (m < n) mx = mx ? std::max(*mx, m) : m;
        int64_t size = n - (mx ? *mx : 0);
        text = py::re::sub(pf(R"re((?m)^([ \t]*)int v%d;)re", n), pf(R"re(\1char v%d[%d];)re", n, size), text);
        text = py::re::sub(pf(R"re(&v%d\b)re", n), pf("v%d", n), text);
        text = py::re::sub(pf(R"re((?m)^([ \t]*)v%d = )re", n), pf(R"re(\1*(int *)v%d = )re", n), text);
    }
    return text;
}

// a function whose every return gives dx too (the high half: dx:ax) returns a long: `dx = vM; return vN;`
// (M = N - 2, the two halves of one local) is `return vN;` of a long vN; `dx = (0 - (E >> 15)); return E;` (the
// sign of E spread: cwd) and `dx = 0; return E;` the value widened - C does that itself
str long_returns(const str& text) {
    auto [head, sep, body] = split_body(text);
    if (sep.empty()) return text;
    py::list<Match> rets = py::re::finditer(R"re((?m)^([ \t]*)return ([^;]+);$)re"_s, body);
    if (rets.empty()) return text;
    for (const Match& r : rets)
        if (!py::re::search(R"re((?m)^[ \t]*dx = [^;]+;\n\s*\Z)re"_s, body.slice(0, r.start()))) return text;
    std::vector<std::pair<str, str>> longs;
    body = py::re::sub(R"re((?m)^([ \t]*)dx = ([^;\n]+);\n\s*return ([^;\n]+);$)re"_s, [&](const Match& m) -> str {
        str ind = m.group(1), hi = m.group(2), lo = m.group(3);
        OMatch h = py::re::fullmatch(R"re(v(\d+))re"_s, hi), l = py::re::fullmatch(R"re(v(\d+))re"_s, lo);
        if (h && l && gi(*h, 1) == gi(*l, 1) - 2) {
            std::pair<str, str> p{lo, hi};
            if (std::find(longs.begin(), longs.end(), p) == longs.end()) longs.push_back(p);
            return pf("%sreturn %s;", ind, lo);
        }
        if (hi == pf("(0 - (%s >> 15))", lo) || hi == pf("(0 - (%s >> 15))", lo.strip("()"_s)) || hi == "0") return pf("%sreturn %s;", ind, lo);
        return m.group(0);
    }, body);
    for (const auto& lh : longs) {
        body = py::re::sub(pf(R"re((?m)^([ \t]*)int %s;\n)re", lh.second), ""_s, body);
        body = py::re::sub(pf(R"re((?m)^([ \t]*)int %s;)re", lh.first), pf(R"re(\1long %s;)re", lh.first), body);
    }
    OMatch m = py::re::search(R"re((?m)^(\w+\())re"_s, head);
    if (m) head = head.slice(0, m->start()) + "long\n"_s + head.slice_from(m->start());
    return head + sep + body;
}

// text with each two lines in a row that rx (over both) matches as fn makes them - tried at every line,
// a pair that does not change left for the next (no overlapping match lost)
static str _line_pairs(const str& text, const str& rx, const std::function<str(const Match&)>& fn) {
    Lines lines = split_nl(text);
    Lines out;
    int64_t k = 0;
    while (k < lines.size()) {
        if (k + 1 < lines.size()) {
            str two = lines[k] + "\n"_s + lines[k + 1];
            OMatch m = py::re::fullmatch(rx, two);
            if (m) {
                str nw = fn(*m);
                if (nw != two) {
                    out.append(nw);
                    k += 2;
                    continue;
                }
            }
        }
        out.append(lines[k]);
        ++k;
    }
    return join_nl(out);
}

namespace {
// "the high word of the long whose low word is x": x the next letter, vN the local two below, g_N the data two above
std::optional<str> hi_of(const str& x) {
    if (py::re::fullmatch(R"re([a-y])re"_s, x)) return py::chr(py::ord(x) + 1);
    if (py::re::fullmatch(R"re(v\d+)re"_s, x)) return pf("v%d", py::int_from_str(x.slice_from(1)) - 2);
    if (py::re::fullmatch(R"re(g_[0-9a-f]+)re"_s, x)) return pf("g_%x", py::int_parse(x.slice_from(2), 16) + 2);   // (a word of the data: the one after)
    return std::nullopt;
}
// hi0: the same without the data case
std::optional<str> hi0(const str& x) {
    if (py::re::fullmatch(R"re([a-y])re"_s, x)) return py::chr(py::ord(x) + 1);
    if (py::re::fullmatch(R"re(v\d+)re"_s, x)) return pf("v%d", py::int_from_str(x.slice_from(1)) - 2);
    return std::nullopt;
}
bool is_g(const str& x) { return py::re::fullmatch(R"re(g_[0-9a-f]+)re"_s, x).has_value(); }
bool eqo(const std::optional<str>& a, const str& b) { return a && *a == b; }
}  // namespace

// two words the code takes as one long - `*(long *)&x`, `LONG(y, x)`, `(x | y)` tested - x the low word: a
// parameter and the next (a, b) or a local and the one above it (vN, vN-2): x declared long, y gone, the
// pair written x. A function whose code widens its value last (cwd: FPINFO longret) returns a long
str long_pairs(const str& text) {
    auto sb = split_body(text);
    str head = std::get<0>(sb), sep = std::get<1>(sb), body = std::get<2>(sb);
    if (sep.empty()) return text;
    std::set<std::string> pairs_;   // (a set of names: sorted where Python sorts it)
    auto pairs_add = [&](const str& s) { pairs_.insert(s.utf8()); };
    for (const str& x : py::re::findall(R"re(\*\(long \*\)&(\w+)\b(?!->|\[))re"_s, body)) pairs_add(x);   // (a variable's own two words - not a field's)
    for (const Match& m : py::re::finditer(R"re(\bLONG\((\w+), (\w+)\))re"_s, body)) pairs_add(m.group(2));
    for (const Match& m : py::re::finditer(R"re(\((\w+) \| (\w+)\))re"_s, body))
        if (eqo(hi0(m.group(1)), m.group(2))) pairs_add(m.group(1));   // (a pair's two words or-ed: tested as one long)

    // (a long field updated: t = LONG(hi, lo) op E; lo = LO(t); hi = HI(t) - one assignment, op=)
    body = py::re::sub(R"re((?m)^([ \t]*)(t\d+) = LONG\((\w+)->f(\d+), (\*\3|\3->f(\d+))\) ([-+&|^]) (.+);\n[ \t]*\5 = LO\(\2\);\n[ \t]*\3->f\4 = HI\(\2\);$)re"_s,
                       [](const Match& m) -> str {
        str ind = m.group(1), p = m.group(3), lo = m.group(5), op = m.group(7), e = m.group(8);
        int64_t nl = lo.startswith("*"_s) ? 0 : gi(m, 6);
        if (gi(m, 4) != nl + 2) return m.group(0);
        add_field(nl);
        return pf("%s*(long *)&%s->f%d %s= %s;", ind, p, nl, op, e);
    }, body);

    // (a pair compared with a number the compiler's way, the high word first: the long compared)
    auto long_eq = [&](const Match& m) -> str {
        str h = m.group(1), l = m.group(3), op = m.group(4);
        int64_t k = gi(m, 5);
        if (!eqo(hi0(l), h) || k >= 0x8000) return m.group(0);
        pairs_add(l);
        return pf("(%s %s %d)", l, op, k);
    };
    body = py::re::sub(R"re(\(\(?(\w+)(?: != (0))?\)? \|\| \((\w+) (!=) (\d+)\)\))re"_s, long_eq, body);
    body = py::re::sub(R"re(\(\((\w+) == (0)\) && \((\w+) (==) (\d+)\)\))re"_s, long_eq, body);
    body = py::re::sub(R"re(\(!(\w+)() && \((\w+) (==) (\d+)\)\))re"_s, long_eq, body);

    // (a pair given a pair: `p->fN = x; p->fN+2 = y;` - a long field given a long - written as one store)
    std::function<str(const Match&)> pair_store = [&](const Match& m) -> str {
        str ind = m.group(1), p = m.group(2), n = m.group(3), x = m.group(4), p2 = m.group(5), n2 = m.group(6), y = m.group(7);
        int64_t ni = py::int_from_str(n), n2i = py::int_from_str(n2);
        bool byte = T::BYTEPTR.contains(std::make_tuple("*"_s, ni)) || T::BYTEPTR.contains(std::make_tuple("*"_s, n2i)) ||
                    T::BYTEPTR.contains(std::make_tuple(p, ni)) || T::BYTEPTR.contains(std::make_tuple(p, n2i));
        Lines bl = split_nl(body);
        bool alone = false;
        for (const str& l : bl)
            if (py::re::search(pf(R"re(\b%s->f%s\b)re", p, n), l) && !py::re::search(pf(R"re(\b%s->f%s\b)re", p, n2), l) &&
                !py::re::fullmatch(pf(R"re([ \t]*%s->f%s = %s;)re", p, n, x), l))
                alone = true;
        bool n2_other = false;
        for (const str& l : bl)
            if (!py::re::fullmatch(pf(R"re([ \t]*%s->f%s = %s;)re", p, n2, y), l) && py::re::search(pf(R"re(\b%s->f%s\b)re", p, n2), l)) n2_other = true;
        if (alone && !n2_other) return m.group(0);   // (the low word read alone, the high never: two ints)
        if (p2 == p && n2i == ni + 2 && eqo(hi_of(x), y) && !byte) {   // (words both: a byte is no long's)
            pairs_add(x);
            add_field(ni);
            return pf("%s%s->f%s = %s;", ind, p, n, x);
        }
        return m.group(0);
    };
    body = py::re::sub(R"re((?m)^([ \t]*)\*(\w+) = (\w+);\n(\s*)\2->f2 = (\w+);$)re"_s, [](const Match& m) -> str {   // (*p the first word: p->f0)
        return eqo(hi_of(m.group(3)), m.group(5)) ? pf("%s%s->f0 = %s;\n%s%s->f2 = %s;", m.group(1), m.group(2), m.group(3), m.group(4), m.group(2), m.group(5))
                                                  : m.group(0);
    }, body);
    body = _line_pairs(body, R"re(([ \t]*)(\w+)->f(\d+) = (\w+);\n\s*(\w+)->f(\d+) = (\w+);)re"_s, pair_store);

    // (a routine's long value, dx:ax, kept in a pair: the routine returns a long)
    auto call_store = [&](const Match& m) -> str {
        str ind = m.group(1), lo = m.group(2), call = m.group(3), hi = m.group(4);
        if (!eqo(hi_of(lo), hi)) return m.group(0);
        pairs_add(lo);
        add_func(word_prefix(call));
        str nxt = gopt(m, 5);
        return pf("%s%s = %s;%s", ind, lo, call, nxt.replace("(ax | dx)"_s, lo));   // (the pair tested at once: the long)
    };
    // (a long passed: its two words pushed - the pair, or an int widened)
    auto call_args = [&](const Match& m) -> str {
        Lines args;
        if (nb(m.group(2))) for (const str& x : _top_commas(m.group(2))) args.append(x.strip());
        Lines out;
        int64_t i = 0;
        while (i < args.size()) {
            const str& x = args[i];
            std::optional<str> y = i + 1 < args.size() ? std::optional<str>(args[i + 1]) : std::nullopt;
            if (y && py::re::fullmatch(R"re(\w+)re"_s, x) && eqo(hi_of(x), *y) && pairs_.count(x.utf8())) {   // (a long the code shows elsewhere)
                out.append(x);
                i += 2;
                continue;
            }
            if (y && (*y == pf("(0 - (%s >> 15))", x) || *y == pf("(0 - ((%s) >> 15))", x))) {
                out.append(py::re::fullmatch(R"re(\w+)re"_s, x) ? pf("(long)%s", x) : pf("(long)(%s)", x));
                i += 2;
                continue;
            }
            out.append(x);
            ++i;
        }
        return pf("%s(%s)", m.group(1), py::join(", "_s, out));
    };

    // (the model's temporaries holding an int and its sign spread, read once by a call a line or two on: in their
    //  places - the call passes a long; the sign in a register too)
    {
        OLines bl = opt_of(split_nl(body));
        for (int64_t k = 0; k < bl.size(); ++k) {
            OMatch m = py::re::fullmatch(R"re([ \t]*(t\d+) = (\(0 - \((\w+) >> 15\)\)|(\w+));)re"_s, bl[k].value_or(""_s));
            if (!m) m = py::re::fullmatch(R"re([ \t]*(bx|cx|dx|di) = (\(0 - \((\w+) >> 15\)\))()();)re"_s, bl[k].value_or(""_s));
            if (!m) continue;
            str t = m->group(1), e = m->group(2);
            str x = !gopt(*m, 3).empty() ? m->group(3) : gopt(*m, 4);
            std::vector<int64_t> uses;
            for (int64_t j = k + 1; j < std::min<int64_t>(k + 4, bl.size()); ++j)
                if (bl[j] && py::re::search(pf(R"re(\b%s\b)re", t), *bl[j])) uses.push_back(j);
            if (uses.size() != 1) continue;
            bool bad = false;
            for (int64_t j = uses[0] + 1; j < bl.size(); ++j)
                if (py::re::search(pf(R"re(\b%s\b)re", t), bl[j].value_or(""_s))) bad = true;
            if (bad || py::re::findall(pf(R"re(\b%s\b)re", t), *bl[uses[0]]).size() != 1 || !py::re::search(R"re(\w\()re"_s, *bl[uses[0]])) continue;
            for (int64_t j = k + 1; j < uses[0]; ++j)
                if (py::re::match(pf(R"re([ \t]*%s = )re", x), bl[j].value_or(""_s))) bad = true;
            for (int64_t j = k + 1; j < uses[0] + 1; ++j)
                if (bl[j].value_or(""_s).contains("{"_s) || bl[j].value_or(""_s).contains("}"_s)) bad = true;   // (one block)
            if (bad) continue;
            bl[uses[0]] = py::re::sub(pf(R"re(\b%s\b)re", t), [&](const Match&) { return e; }, *bl[uses[0]]);
            bl[k] = std::nullopt;
        }
        body = join_opt(bl);
    }
    for (const str& t : unique(py::re::findall(R"re((?m)^[ \t]*int ((?:t\d+, )*t\d+);$)re"_s, body)))
        for (const str& t1 : py::split(t, ", "_s)) {
            str after = last_after(last_after(body, "int "_s + t), ";"_s);
            if (!py::re::search(pf(R"re(\b%s\b)re", t1), after)) body = _drop_decl(body, t1);
        }
    body = py::re::sub(R"re(\bLONG\(\(0 - \((\w+) >> 15\)\), \1\))re"_s, R"re((long)\1)re"_s, body);   // (an int's sign spread: widened)
    body = py::re::sub(R"re(\bLONG\(0, (\w+)\))re"_s, R"re((long)(unsigned)\1)re"_s, body);
    body = py::re::sub(R"re(\bLONG\((\w+(?:->f\d+)*)->f(\d+), \1->f(\d+)\))re"_s, [](const Match& m) -> str {   // (a long field's two words: the field)
        if (gi(m, 2) == gi(m, 3) + 2) {
            add_field(gi(m, 3));
            return pf("%s->f%s", m.group(1), m.group(3));
        }
        return m.group(0);
    }, body);
    if (T::LONGINFO.ret && *T::LONGINFO.ret)   // (a long function's value its long's low word: the long)
        body = py::re::sub(R"re(\bLO\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re"_s, R"re((\1))re"_s, body);

    // (a pair given a pair: x = y, the high words too)
    auto pair_copy = [&](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), y = m.group(3), hx = m.group(4), hy = m.group(5);
        auto ghi = [](const str& g) -> std::optional<str> {
            return is_g(g) ? std::optional<str>(pf("g_%x", py::int_parse(g.slice_from(2), 16) + 2)) : std::nullopt;
        };
        auto gb = [&](const str& g) {
            if (!is_g(g)) return false;
            str want = hex4(py::int_parse(g.slice_from(2), 16));
            for (const str& b_ : T::BYTEG)
                if (is_g(b_) && hex4(py::int_parse(b_.slice_from(2), 16)) == want) return true;
            return false;
        };
        if (gb(x) || gb(hx) || gb(y) || gb(hy)) return m.group(0);   // (bytes of the data given: no long's words)
        if (eqo(hi_of(x), hx) && eqo(hi_of(y), hy)) {
            pairs_add(x);
            pairs_add(y);
            return pf("%s%s = %s;", ind, x, y);
        }
        bool first = eqo(hi_of(x), hx);
        if (!first && ghi(x)) first = py::int_parse(hx.startswith("g_"_s) ? hx.slice_from(2) : "0"_s, 16) == py::int_parse(x.slice_from(2), 16) + 2;
        if (first && is_g(y) && is_g(hy) && py::int_parse(hy.slice_from(2), 16) == py::int_parse(y.slice_from(2), 16) + 2) {
            pairs_add(x);   // (a long in the data copied: one long)
            add_global(hex4(py::int_parse(y.slice_from(2), 16)));
            if (x.startswith("g_"_s)) add_global(hex4(py::int_parse(x.slice_from(2), 16)));
            return pf("%s%s = %s;", ind, x, y);
        }
        return m.group(0);
    };
    body = _line_pairs(body, R"re(([ \t]*)(\w+) = (\w+);\n\s*(\w+) = (\w+);)re"_s, pair_copy);

    // (a pair given two numbers: one long number)
    auto const_pair = [&](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), hx = m.group(4);
        int64_t k1 = gi(m, 3), k2 = gi(m, 5);
        if (!eqo(hi_of(x), hx) || x.startswith("g_"_s)) return m.group(0);   // (two words of the data given numbers: two ints)
        pairs_add(x);
        int64_t v = ((k2 & 0xFFFF) << 16) | (k1 & 0xFFFF);
        if (v & 0x80000000LL) v = v - (int64_t(1) << 32);
        return pf("%s%s = %s;", ind, x, std::abs(v) > 0x7FFF ? pf("%dL", v) : py::to_str(v));
    };
    body = _line_pairs(body, R"re(([ \t]*)(\w+) = (-?\d+);\n\s*(\w+) = (-?\d+);)re"_s, const_pair);

    // (two words' expressions, the low and the high, as the one long expression they are, or None: names, fields
    //  and data in pairs, ~, & | ^, a number masking a half)
    std::function<std::optional<str>(const str&, const str&)> long_of = [&](const str& lo_in, const str& hi_in) -> std::optional<str> {
        str lo = _strip(lo_in.strip()), hi = _strip(hi_in.strip());
        auto bare = [](const str& x) { return (x.startswith("("_s) && x.endswith(")"_s) && _balanced(x.slice(1, -1))) ? x.slice(1, -1) : x; };
        lo = bare(lo);
        hi = bare(hi);
        OMatch fl = py::re::fullmatch(R"re((\w+)->f(\d+))re"_s, lo), fh = py::re::fullmatch(R"re((\w+)->f(\d+))re"_s, hi);
        if (fl && fh && fl->group(1) == fh->group(1) && gi(*fh, 2) == gi(*fl, 2) + 2) {
            add_field(gi(*fl, 2));
            return lo;
        }
        if (is_g(lo) && is_g(hi) && py::int_parse(hi.slice_from(2), 16) == py::int_parse(lo.slice_from(2), 16) + 2) {
            add_global(hex4(py::int_parse(lo.slice_from(2), 16)));
            return lo;
        }
        if (py::re::fullmatch(R"re(\w+)re"_s, lo) && eqo(hi_of(lo), hi)) {
            pairs_add(lo);
            return lo;
        }
        if (lo.startswith("~"_s) && hi.startswith("~"_s)) {
            std::optional<str> x = long_of(lo.slice_from(1), hi.slice_from(1));
            return (x && !x->empty()) ? std::optional<str>(pf("(~%s)", *x)) : std::nullopt;
        }
        std::function<Lines(const str&, const str&)> flat = [&](const str& x, const str& op) -> Lines {   // (a & (b & c): a, b, c)
            Lines out;
            for (const str& p_ : _top_split(x, op)) {
                str pp = p_.strip();
                str b_ = bare(pp);
                if (b_ != pp && _top_split(b_, op).size() > 1) out.extend(flat(b_, op));
                else out.append(pp);
            }
            return out;
        };
        for (const char* opc : {"&", "|", "^"}) {
            str op(opc);
            Lines pl = flat(lo, op), ph = flat(hi, op);
            if (ph.size() > 1 && ph.size() == pl.size() + 1 && py::re::fullmatch(R"re(\d+|0x[0-9a-f]+)re"_s, ph[-1].strip()) && op == "&") {
                std::optional<str> x = long_of(pl.size() > 1 ? py::join(" & "_s, pl) : pl[0], ph.size() > 2 ? py::join(" & "_s, ph.slice(0, -1)) : ph[0]);
                int64_t k = py::int_parse(ph[-1].strip(), 0) & 0xFFFF;   // (the high word masked alone: the low word whole)
                return (x && !x->empty()) ? std::optional<str>(pf("(%s & 0x%04xffffL)", *x, k)) : std::nullopt;
            }
            if (pl.size() > 1 && pl.size() == ph.size()) {
                Lines xs;
                bool all_ = true;
                for (int64_t i = 0; i < pl.size(); ++i) {
                    std::optional<str> x = long_of(pl[i], ph[i]);
                    if (!x || x->empty()) all_ = false;
                    else xs.append(*x);
                }
                return all_ ? std::optional<str>(pf("(%s)", py::join(pf(" %s ", op), xs))) : std::nullopt;
            }
        }
        return std::nullopt;
    };

    // ((lo) | (hi)) != 0: a long tested
    body = py::re::sub(R"re(\((\((?:[^()]|\((?:[^()]|\((?:[^()]|\([^()]*\))*\))*\))*\)) (!=|==) 0\))re"_s, [&](const Match& m) -> str {
        str inner = m.group(1).slice(1, -1);
        Lines parts = _balanced(inner) ? _top_split(inner, "|"_s) : Lines();
        if (parts.size() != 2 || !_balanced(parts[0]) || !_balanced(parts[1])) return m.group(0);
        std::optional<str> x = long_of(parts[0], parts[1]);
        return (x && !x->empty()) ? pf("(%s %s 0)", *x, m.group(2)) : m.group(0);
    }, body);

    // (a long field given a long a word at a time - another long field's, a long in the data, ..)
    auto field_pair = [&](const Match& m) -> str {
        str ind = m.group(1), p = m.group(2), n = m.group(3), x = m.group(4), p2 = m.group(5), y = m.group(7);
        if (p2 != p || gi(m, 6) != gi(m, 3) + 2 || py::re::fullmatch(R"re([a-z]|v\d+)re"_s, x.strip())) return m.group(0);   // (two variables: pair_store's, with its guards)
        std::optional<str> lx = long_of(x, y);
        if (!lx || lx->empty()) return m.group(0);
        add_field(py::int_from_str(n));
        return pf("%s%s->f%s = %s;", ind, p, n, *lx);
    };
    body = _line_pairs(body, R"re(([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);\n\s*(\w+(?:->f\d+)*)->f(\d+) = ([^;\n]+);)re"_s, field_pair);

    auto lfield = [](const str& p, const str& n) -> str {   // (a long field: offset n of what p points to)
        add_field(py::int_from_str(n));
        return pf("%s->f%s", p, n);
    };
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+)->f(\d+) = \(~\2->f\3\);\n[ \t]*\2->f(\d+) = \(~\2->f\4\);$)re"_s, [&](const Match& m) -> str {   // (both words complemented)
        if (gi(m, 4) != gi(m, 3) + 2) return m.group(0);
        str a = lfield(m.group(2), m.group(3));
        str b = lfield(m.group(2), m.group(3));
        return pf("%s%s = ~%s;", m.group(1), a, b);
    }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+)->f(\d+) = (\w+);\n[ \t]*\2->f(\d+) = \(0 - \(\4 >> 15\)\);$)re"_s, [&](const Match& m) -> str {   // (an int widened into it)
        return gi(m, 5) == gi(m, 3) + 2 ? pf("%s%s = %s;", m.group(1), lfield(m.group(2), m.group(3)), m.group(4)) : m.group(0);
    }, body);
    body = py::re::sub(R"re(\((\w+(?:->f\d+)*)->f(\d+) \| \1->f(\d+)\))re"_s, [&](const Match& m) -> str {   // (its words tested together)
        return gi(m, 3) == gi(m, 2) + 2 ? lfield(m.group(1), m.group(2)) : m.group(0);
    }, body);
    body = py::re::sub(R"re(\*\(long \*\)&(\w+(?:->f\d+)*)->f(\d+)\b)re"_s, [&](const Match& m) { return lfield(m.group(1), m.group(2)); }, body);   // (read whole)

    // (a long field's two words pushed together: the field)
    auto field_args = [&](const Match& m) -> str {
        Lines args;
        if (nb(m.group(2))) for (const str& x : _top_commas(m.group(2))) args.append(x.strip());
        Lines out;
        int64_t i = 0;
        while (i < args.size()) {
            OMatch a1 = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, args[i]);
            OMatch a2 = i + 1 < args.size() ? py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, args[i + 1]) : std::nullopt;
            bool together = a1 && i + 1 < args.size() &&
                            py::re::findall(pf(R"re(%s\b(?!->))re", esc(args[i + 1])), body).size() ==
                                py::re::findall(pf(R"re(%s, %s\b)re", esc(args[i]), esc(args[i + 1])), body).size();   // (the high word never alone)
            if (a1 && a2 && a1->group(1) == a2->group(1) && gi(*a2, 2) == gi(*a1, 2) + 2 && together &&
                !T::BYTEPTR.contains(std::make_tuple(a1->group(1), gi(*a1, 2))) && longfield(gi(*a1, 2))) {
                out.append(lfield(a1->group(1), a1->group(2)));
                i += 2;
                continue;
            }
            out.append(args[i]);
            ++i;
        }
        return pf("%s(%s)", m.group(1), py::join(", "_s, out));
    };
    const str callpat = R"re(\b((?!if\b|while\b|for\b|switch\b|return\b)[A-Za-z_]\w*)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re"_s;
    body = py::re::sub(callpat, field_args, body);

    // (a long and-ed with a long field and tested: the high words in dx first, then the low words or-ed in)
    body = py::re::sub(R"re((?m)^([ \t]*)dx = (\w+) & (\w+)->f(\d+);\n(.*\(\((\w+) & \3->f(\d+)\) \| dx\).*)$)re"_s, [&](const Match& m) -> str {
        str hi = m.group(2), p = m.group(3), n2 = m.group(4), rest = m.group(5), lo = m.group(6), n1 = m.group(7);
        if (!eqo(hi_of(lo), hi) || py::int_from_str(n2) != py::int_from_str(n1) + 2) return m.group(0);
        pairs_add(lo);
        add_field(py::int_from_str(n1));
        return rest.replace(pf("((%s & %s->f%s) | dx)", lo, p, n1), pf("(%s & %s->f%s)", lo, p, n1));
    }, body);

    // (a long in the data and-ed, or-ed, xor-ed with a long: a word at a time, the high words after - one op)
    auto gpair_op = [&](const Match& m) -> str {
        str ind = m.group(1), g = m.group(2), op = m.group(3), x = m.group(4), g2 = m.group(5), op2 = m.group(6), x2 = m.group(7);
        if (op != op2 || py::int_parse(g2.slice_from(2), 16) != py::int_parse(g.slice_from(2), 16) + 2) return m.group(0);
        std::optional<str> lx = long_of(x, x2);
        if (!lx || lx->empty()) return m.group(0);
        add_global(hex4(py::int_parse(g.slice_from(2), 16)));
        return pf("%s%s = %s %s %s;", ind, g, g, op, *lx);
    };
    body = _line_pairs(body, R"re(([ \t]*)(g_[0-9a-f]+) = \2 ([|&^]) ([^;\n]+);\n\s*(g_[0-9a-f]+) = \5 ([|&^]) ([^;\n]+);)re"_s, gpair_op);

    // (a pair given an int and its sign spread: the int widened to the long)
    OrdMap<str> widened;   // (a long given an int: its low word that int)
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+) = ([^;\n]+);\n[ \t]*(\w+) = \(0 - \((.+) >> 15\)\);$)re"_s, [&](const Match& m) -> str {
        str ind = m.group(1), x = m.group(2), e = m.group(3), hx = m.group(4), e2 = m.group(5);
        if (!eqo(hi_of(x), hx) || (e2.strip() != e && e2.strip() != pf("(%s)", e))) return m.group(0);
        pairs_add(x);
        if (py::re::fullmatch(R"re([A-Za-z_]\w*)re"_s, e)) {
            if (str* w = widened.find(x)) *w = e;
            else widened.items.push_back({x, e});
        }
        return pf("%s%s = %s;", ind, x, e);
    }, body);
    for (const auto& xe : widened.items) {   // (its one word tested - the int's, still in a register)
        const str& x = xe.first;
        const str& e = xe.second;
        if (!py::re::search(pf(R"re((?m)^[ \t]*(?:%s|%s) = )re", x, e), last_after(body, pf("%s = %s;", x, e))))
            body = py::re::sub(pf(R"re(\(%s (==|!=) 0\))re", x), pf(R"re((%s \1 0))re", e), body);
    }

    // (both halves given 0, or -1: the long 0, -1)
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+) = (0|-1);\n\s*(\w+) = \2;$)re"_s, [&](const Match& m) -> str {
        str ind = m.group(1), a = m.group(2), v = m.group(3), b = m.group(4);
        std::optional<str> lo;
        if (eqo(hi_of(b), a)) lo = b;
        else if (eqo(hi_of(a), b)) lo = a;
        if (!lo || lo->empty() || lo->startswith("g_"_s)) return m.group(0);
        pairs_add(*lo);
        return pf("%s%s = %s;", ind, *lo, v);
    }, body);

    // (a routine's dx:ax read as one long by the next statement)
    body = py::re::sub(R"re((?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n(.*\bLONG\(dx, ax\).*)$)re"_s, [](const Match& m) -> str {
        add_func(word_prefix(m.group(2)));
        return m.group(3).replace("LONG(dx, ax)"_s, m.group(2));
    }, body);

    // (a long given to a pair a half at a time: one store)
    auto halves = [&](const str& ind, const str& e, const str& lo, const str& hi, const str& whole) -> str {
        if (!eqo(hi_of(lo), hi)) return whole;
        pairs_add(lo);
        return pf("%s%s = %s;", ind, lo, e);
    };
    body = py::re::sub(R"re((?m)^([ \t]*)(t\d+) = (.+);\n\s*(\w+) = LO\(\2\);\n\s*(\w+) = HI\(\2\);$)re"_s,
                       [&](const Match& m) { return halves(m.group(1), m.group(3), m.group(4), m.group(5), m.group(0)); }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)dx = HI\((.+)\);\n[ \t]*(\w+) = LO\(\2\);\n[ \t]*(\w+) = dx;$)re"_s, [&](const Match& m) -> str {   // (the high half kept in dx first)
        if (eqo(hi_of(m.group(3)), m.group(4))) {
            pairs_add(m.group(3));
            return pf("%s%s = %s;", m.group(1), m.group(3), m.group(2));
        }
        return m.group(0);
    }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = LO\((.+)\);\n[ \t]*\2->f(\d+) = HI\(\4\);$)re"_s, [&](const Match& m) -> str {   // (a long field given a long)
        return gi(m, 5) == gi(m, 3) + 2 ? pf("%s%s = %s;", m.group(1), lfield(m.group(2), m.group(3)), m.group(4)) : m.group(0);
    }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)()(\w+) = LO\((.+)\);\n\s*(\w+) = HI\(\4\);$)re"_s,
                       [&](const Match& m) { return halves(m.group(1), m.group(4), m.group(3), m.group(5), m.group(0)); }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)ax = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = ax;\n(?=\s*\w+ = dx;))re"_s, [](const Match& m) -> str {   // (through ax: the same - when dx goes with it)
        return pf("%s%s = %s;\n", m.group(1), m.group(3), m.group(2));
    }, body);
    body = py::re::sub(R"re((?m)^([ \t]*)(\w+) = ((?:sub_\w+|[A-Za-z_]\w*)\((?:[^()]|\([^()]*\))*\));\n\s*(\w+) = dx;(\n.*)?$)re"_s, call_store, body);
    for (const std::string& g : std::vector<std::string>(pairs_.begin(), pairs_.end()))   // (data a pair is made of: a long there)
        if (is_g(str(g))) add_global(hex4(py::int_parse(str(g).slice_from(2), 16)));

    // (two words of the data pushed together, the high one first: a long there - the same pushes)
    auto data_args = [&](const Match& m) -> str {
        Lines args;
        if (nb(m.group(2))) for (const str& x : _top_commas(m.group(2))) args.append(x.strip());
        Lines out;
        int64_t i = 0;
        while (i < args.size()) {
            if (i + 1 < args.size() && is_g(args[i]) && eqo(hi_of(args[i]), args[i + 1])) {
                add_global(hex4(py::int_parse(args[i].slice_from(2), 16)));
                out.append(args[i]);
                i += 2;
                continue;
            }
            out.append(args[i]);
            ++i;
        }
        return pf("%s(%s)", m.group(1), py::join(", "_s, out));
    };
    body = py::re::sub(callpat, call_args, body);
    body = py::re::sub(callpat, data_args, body);
    Lines params = py::re::findall(R"re((?m)^\w+\(([\w, ]*)\))re"_s, head);
    Lines plist;
    if (!params.empty())
        for (const str& p : py::split(params[-1], ","_s))
            if (nb(p)) plist.append(p.strip());
    for (const str& x : py::re::findall(R"re((?m)^[ \t]*(?:unsigned )?long (v\d+);$)re"_s, body)) {   // (a long a pass made, its high word still read by name: the pair)
        std::optional<str> h = hi_of(x);
        if (h && !h->empty() && py::re::search(pf(R"re(\b%s\b)re", *h), last_after(body, "{"_s))) pairs_add(x);
    }
    for (const std::string& lo_s : std::vector<std::string>(pairs_.begin(), pairs_.end())) {
        str lo(lo_s);
        std::optional<str> hio = hi_of(lo);
        if (!hio || hio->empty()) continue;
        str hi = *hio;
        bool isparam = plist.contains(lo) && plist.contains(hi);
        bool islocal = py::re::search(pf(R"re((?m)^[ \t]*(?:int|long) (?:\w+, )*%s\b)re", lo), body) &&
                       (py::re::search(pf(R"re((?m)^[ \t]*int (?:\w+, )*%s\b)re", hi), body) || py::re::search(pf(R"re((?m)^[ \t]*long %s;)re", lo), body));
        if (!isparam && !islocal) continue;
        str uns = (T::LONGINFO.unsigned_ && T::LONGINFO.unsigned_->contains(lo)) ? "unsigned long"_s : "long"_s;
        body = py::re::sub(pf(R"re(\*\(long \*\)&%s\b)re", lo), lo, body);
        body = py::re::sub(pf(R"re(\bLONG\(%s, %s\))re", hi, lo), lo, body);
        body = py::re::sub(pf(R"re(\(%s \| %s\)|\(%s \| %s\))re", lo, hi, hi, lo), lo, body);
        body = py::re::sub(pf(R"re(\(%s (<|>=) 0\))re", hi), pf(R"re((%s \1 0))re", lo), body);   // (the high word's sign: the long's)
        for (const std::string& lo2_s : std::vector<std::string>(pairs_.begin(), pairs_.end())) {   // (two longs compared a half at a time)
            str lo2(lo2_s);
            if (lo2 == lo || !hi_of(lo2) || hi_of(lo2)->empty()) continue;
            str hi2 = *hi_of(lo2);
            body = body.replace(pf("(%s != %s) || (%s != %s)", hi, hi2, lo, lo2), pf("(%s != %s)", lo, lo2));
            body = body.replace(pf("(%s == %s) && (%s == %s)", hi, hi2, lo, lo2), pf("(%s == %s)", lo, lo2));
            body = body.replace(pf("((%s & %s) == %s) && ((%s & %s) == %s)", hi, hi2, hi2, lo, lo2, lo2), pf("((%s & %s) == %s)", lo, lo2, lo2));
            // (one masked with the other and compared with it: dx the high words' and)
            body = py::re::sub(pf(R"re((?m)^([ \t]*)dx = %s & %s;\n([ \t]*if \(.*)\(dx == %s\) && \(\(%s & %s\) == %s\))re", hi, hi2, hi2, lo, lo2, lo2),
                               pf(R"re(\2((%s & %s) == %s))re", lo, lo2, lo2), body);
        }
        body = py::re::sub(pf(R"re((?m)^([ \t]*)%s = (\w+(?:->f\d+)*)->f(\d+);\n[ \t]*%s = \2->f(\d+);$)re", lo, hi), [&](const Match& m) -> str {   // (a long field read)
            return gi(m, 4) == gi(m, 3) + 2 ? pf("%s%s = *(long *)&%s->f%s;", m.group(1), lo, m.group(2), m.group(3)) : m.group(0);
        }, body);
        body = py::re::sub(pf(R"re(\(%s & (-?\d+)\))re", hi), [&](const Match& m) -> str { return pf("(%s & 0x%xL)", lo, (gi(m, 1) & 0xFFFF) << 16); }, body);
        if (isparam) {
            Lines np;
            for (const str& p : plist)
                if (p != hi) np.append(p);
            plist = np;
            head = py::re::sub(R"re((?m)^(\w+)\([\w, ]*\))re"_s, [&](const Match& m) { return pf("%s(%s)", m.group(1), py::join(", "_s, plist)); }, head, 1);
            head = _drop_decl(head, hi);
            head = or_str(_redeclare(head, lo, pf("%s %s", uns, lo)), head);
        } else {
            body = _drop_decl(body, hi);
            body = or_str(_redeclare(body, lo, pf("%s %s", uns, lo)), body);
        }
    }
    std::set<std::string> longs_;
    for (const str& x : py::re::findall(R"re((?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);)re"_s, body)) longs_.insert(x.utf8());

    // (a long value - one of a long's - stored a word at a time into a field, dx the high one)
    auto lstore = [&](const str& whole, const str& ind, const str& e, const str& p, int64_t n, const str& op, int64_t n2) -> str {
        bool any_ = false;
        for (const std::string& x : longs_)
            if (py::re::search(pf(R"re(\b%s\b)re", str(x)), e)) any_ = true;
        if (n2 != n + 2 || !any_) return whole;
        add_field(n);
        return pf("%s*(long *)&%s->f%d = %s%s;", ind, p, n, op, (op.empty() || e.startswith("("_s)) ? e : pf("(%s)", e));
    };
    if (!longs_.empty()) {
        body = py::re::sub(R"re((?m)^([ \t]*)ax = (.+);\n[ \t]*(\w+(?:->f\d+)*)->f(\d+) = \(?(~?)ax\)?;\n[ \t]*\3->f(\d+) = \(?~?dx\)?;$)re"_s, [&](const Match& m) {
            return lstore(m.group(0), m.group(1), m.group(2), m.group(3), gi(m, 4), gopt(m, 5), gi(m, 6));
        }, body);
        body = py::re::sub(R"re((?m)^([ \t]*)(\w+(?:->f\d+)*)->f(\d+) = (\(.+\));\n[ \t]*\2->f(\d+) = dx;$)re"_s, [&](const Match& m) {
            return lstore(m.group(0), m.group(1), m.group(4), m.group(2), gi(m, 3), ""_s, gi(m, 5));
        }, body);
    }
    {
        bool lret = false;
        for (const str& x : py::re::findall(R"re((?m)^[ \t]*return (\w+);$)re"_s, body))
            if (py::re::search(pf(R"re((?m)^[ \t]*(?:unsigned )?long (?:\w+, )*%s\b)re", x), body)) lret = true;
        if (lret && !(T::LONGINFO.ret && !*T::LONGINFO.ret) && !py::re::search(R"re((?m)^[ \t]*return [^;]*\w\()re"_s, body))
            T::LONGINFO.ret = true;   // (a long local returned: the function's a long)
    }
    if (T::LONGINFO.ret && *T::LONGINFO.ret) {
        OMatch m = py::re::search(R"re((?m)^\w+\()re"_s, head);
        if (m && !py::re::search(R"re((?m)^(?:unsigned )?long\s*$)re"_s, head)) head = head.slice(0, m->start()) + "long\n"_s + head.slice_from(m->start());
        for (const str& n : py::re::findall(R"re((?m)^[ \t]*return \w+(?:->f\d+)*->f(\d+);$)re"_s, body))   // (a field returned whole: a long one)
            add_field(py::int_from_str(n));
    }
    return head + sep + body;
}

// two longs compared the compiler's way - the high words, then the low ones - left as ifs of the halves:
//     if ((H <= H2)) { if ((H < H2) || (L < L2)) { A } else { L_x: B } } else { goto L_x; }  ->  if ((L < L2)) A else B
// (L, L2 longs - declared so - H, H2 their high words; the same for >= and >)
str long_ladders(const str& text) {
    std::set<std::string> longs;
    for (const str& x : py::re::findall(R"re((?m)^[ \t]*(?:register )?(?:unsigned )?long (\w+);)re"_s, text)) longs.insert(x.utf8());
    if (longs.empty()) return text;
    Lines lines = split_nl(text);
    for (int64_t k = 0; k < lines.size(); ++k) {
        auto p = _if_parts(lines, k);
        if (!p || !std::get<2>(*p)) continue;
        OMatch m = py::re::fullmatch(R"re(\((\w+) ([<>])= (\w+)\))re"_s, _strip("("_s + std::get<0>(*p) + ")"_s));
        auto q = !std::get<1>(*p).empty() ? _if_parts(std::get<1>(*p), 0) : std::nullopt;
        if (!m || !q || std::get<3>(*q) != std::get<1>(*p).size() || !std::get<2>(*q)) continue;
        OMatch n = py::re::fullmatch(R"re(\(?\((\w+) ([<>]) (\w+)\) \|\| \((\w+) ([<>]=?) (\w+)\)\)?)re"_s, std::get<0>(*q));
        if (!n || n->group(1) != m->group(1) || n->group(2) != m->group(2) || n->group(3) != m->group(3)) continue;
        str L = n->group(4), L2 = n->group(6);
        if (!longs.count(L.utf8()) || !eqo(hi_of(L), m->group(1)) || (longs.count(L2.utf8()) && !eqo(hi_of(L2), m->group(3))) ||
            n->group(5).slice(0, 1) != m->group(2))
            continue;
        auto dd = [](const Lines& ls) {
            Lines out;
            for (const str& l : ls) out.append(l.startswith("    "_s) ? l.slice_from(4) : l);
            return out;
        };
        if (!_same_arm(*std::get<2>(*p), dd(*std::get<2>(*q)))) continue;
        str ind = lead(lines[k]);
        Lines nw;
        nw.append(pf("%sif ((%s %s %s)) {", ind, L, n->group(5), L2));
        nw.extend(dd(std::get<1>(*q)));
        nw.append(ind + "} else {"_s);
        nw.extend(dd(*std::get<2>(*q)));
        nw.append(ind + "}"_s);
        Lines cand = lines.slice(0, k) + nw + lines.slice_from(std::get<3>(*p));
        str whole = join_nl(cand);
        Lines kept;
        for (const str& l : cand)
            if (!(py::re::fullmatch(R"re(\s*L_\w+:)re"_s, l) && !py::re::search(pf(R"re(\bgoto %s;)re", l.strip().slice(0, -1)), whole))) kept.append(l);
        return long_ladders(join_nl(kept));
    }
    return text;
}

// a register given the place of a local (`r = bp - N`, the model's lea of [bp-N]): a char pointer into it,
// `r = (char *)&vN;`, its fields the bytes it reaches (`r->fK` is r[K])
str local_pointers(str text) {
    text = py::re::sub(R"re((?m)^([ \t]*)(\w+) = bp;\n\s*\2 = \2 - (\d+);$)re"_s, R"re(\1\2 = bp - \3;)re"_s, text);   // (mov r,bp; sub r,N)
    for (const Match& rn : py::re::finditer(R"re((?m)^[ \t]*(\w+) = bp - (\d+);)re"_s, text)) {
        str reg = rn.group(1), n = rn.group(2);
        std::optional<std::pair<str, str>> buf;   // (the start of a filler the frame keeps: a buffer there)
        for (const Match& kc : py::re::finditer(R"re((?m)^[ \t]*char _p(\d+)\[(\d+)\];)re"_s, text))
            if (gi(kc, 1) + gi(kc, 2) == py::int_from_str(n)) {
                buf = std::make_pair(kc.group(1), kc.group(2));
                break;
            }
        if (buf) {
            text = py::re::sub(pf(R"re((?m)^([ \t]*)char _p%s\[%s\];)re", buf->first, buf->second), pf(R"re(\1char v%s[%s];)re", n, buf->second), text);
            text = py::re::sub(pf(R"re((?m)^([ \t]*)%s = bp - %s;)re", reg, n), pf(R"re(\1%s = v%s;)re", reg, n), text);
            text = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?int %s;)re", reg), pf(R"re(\1\2char *%s;)re", reg), text);
            continue;
        }
        if (!py::re::search(pf(R"re((?m)^[ \t]*(?:int|long) v%s;)re", n), text)) continue;
        text = py::re::sub(pf(R"re((?m)^([ \t]*)%s = bp - %s;)re", reg, n), pf(R"re(\1%s = (char *)&v%s;)re", reg, n), text);
        text = py::re::sub(pf(R"re(\b%s->f(\d+)\b)re", reg), pf(R"re(%s[\1])re", reg), text);
        text = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?int %s;)re", reg), pf(R"re(\1\2char *%s;)re", reg), text);
    }
    return text;
}

// the draft without its last `return X;` (a function of no value whose last call's result the model kept)
std::optional<str> void_variant(const str& text) {
    py::list<Match> m = py::re::finditer(R"re((?m)^([ \t]*)return (.+);\s*$)re"_s, text);
    if (m.empty()) return std::nullopt;
    const Match& x = m[-1];
    if (!py::re::match(R"re(\s*\}\s*$)re"_s, text.slice_from(x.end())) && x.group(1).size() > 4) return std::nullopt;   // (a return inside the body, not its end: a value)
    return text.slice(0, x.start()) + (x.group(2).contains("("_s) ? pf("%s%s;", x.group(1), x.group(2)) : ""_s) + text.slice_from(x.end());
}

// the registers a compiler keeps its register variables in (MS C 3.00: SI, then DI), made those variables:
// declared in that order, pointers where the text goes through them; a register a parameter is loaded into
// at the start (regparam {register: parameter index}) is that parameter, declared register
str register_variables(const str& text, const Lines& regs, const py::dict<str, int64_t>& regparam) {
    auto sb = split_body(text);
    str head = std::get<0>(sb), sep = std::get<1>(sb), body = std::get<2>(sb);
    if (sep.empty()) return text;
    Lines rdecl;   // (declared in the order of their registers: the compiler gives the first declared the first)
    std::vector<std::pair<str, int64_t>> rp;
    for (const auto& kv : regparam.items()) rp.push_back({std::get<0>(kv), std::get<1>(kv)});
    std::stable_sort(rp.begin(), rp.end(), [&](const auto& a, const auto& b) {
        auto key = [&](const str& r) -> int64_t { return regs.contains(r) ? regs.index(r) : 9; };
        return key(a.first) < key(b.first);
    });
    for (const auto& rix : rp) {
        const str& r = rix.first;
        str p = py::chr(py::ord("a"_s) + rix.second);
        body = py::re::sub(pf(R"re(\b%s\b)re", r), p, body);
        bool ptr = py::re::search(pf(R"re(\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*\[)re", p, p), body).has_value();
        OLines lines = opt_of(split_nl(head));   // (out of the plain declaration of the parameters)
        for (int64_t j = 0; j < lines.size(); ++j) {
            OMatch m = py::re::match(R"re((\s*)int (.*);$)re"_s, *lines[j]);
            if (m && py::re::search(pf(R"re(\b%s\b)re", p), m->group(2))) {
                Lines rest;
                for (const str& x : py::split(m->group(2), ","_s))
                    if (x.strip() != p) rest.append(x.strip());
                if (!rest.empty()) lines[j] = m->group(1) + "int "_s + py::join(", "_s, rest) + ";"_s;
                else lines[j] = std::nullopt;
            }
        }
        head = join_opt(lines);
        bool pp = py::re::search(pf(R"re(\*\(\*%s\)|\*\*%s\b)re", p, p), body) && !py::re::search(pf(R"re(\b%s->)re", p), body);   // (read through twice: a pointer to pointers)
        rdecl.append(pf("register %s%s;", pp ? "char **"_s : ptr ? "char *"_s : "int "_s, p));
    }
    if (!rdecl.empty()) {
        head = head.rstrip("\n"_s) + "\n"_s + py::join("\n"_s, rdecl) + "\n"_s;
        OMatch sig = py::re::search(R"re((?m)^(\w+)\(([\w, ]*)\)\s*$)re"_s, head);   // (the parameters in the signature, all of them)
        if (sig) {
            Lines have;
            for (const str& x : py::split(sig->group(2), ","_s))
                if (nb(x)) have.append(x.strip());
            std::set<std::string> ws;
            for (const str& x : have) ws.insert(x.utf8());
            for (const auto& kv : regparam.items()) ws.insert(py::chr(py::ord("a"_s) + std::get<1>(kv)).utf8());
            Lines want;
            for (const std::string& w : ws) want.append(str(w));
            if (want != have) head = head.slice(0, sig->start()) + pf("%s(%s)", sig->group(1), py::join(", "_s, want)) + head.slice_from(sig->end());
        }
    }
    Lines used;
    for (const str& r : regs)
        if (py::re::search(pf(R"re(\b%s\b)re", r), body)) used.append(r);
    if (used.empty()) return head + sep + body;
    Lines decls;
    Lines pnames = py::re::findall(R"re((?m)^[ \t]*(?:register\s+)?char\s*\*\s*(\w+);)re"_s, head);   // (pointer parameters)
    for (const str& r : used) {
        bool ptr = py::re::search(pf(R"re(\*\s*%s\b|\b%s\s*\[)re", r, r), body).has_value();
        for (const str& p : pnames)   // (given a pointer's value)
            if (py::re::search(pf(R"re((?m)^[ \t]*%s = %s;)re", r, p), body)) ptr = true;
        bool pp = py::re::search(pf(R"re(\*\(\*%s\)|\*\*%s\b)re", r, r), body) && !py::re::search(pf(R"re(\b%s->)re", r), body);
        decls.append(pf("    register %s%s;", pp ? "char **"_s : ptr ? "char *"_s : "int "_s, "r_"_s + r));
        body = py::re::sub(pf(R"re(\b%s\b)re", r), "r_"_s + r, body);
    }
    return head + "{\n"_s + py::join("\n"_s, decls) + body;
}

// the model's trailing frame filler less two bytes for each register local (a variant: MS C 3.00 gives some
// register locals a slot in the frame - which ones the compiler alone says)
std::optional<str> register_slots(const str& text) {
    if (!F::REGSLOTS[0]) return std::nullopt;
    int64_t n_regs = py::re::findall(R"re((?m)^[ \t]*register [^;(]*\br_\w+;$)re"_s, text).size();
    py::list<Match> fl = py::re::finditer(R"re((?m)^([ \t]*)char (_p\w*)\[(\d+)\];\n)re"_s, text);
    OMatch nxt = !fl.empty() ? py::re::match(R"re([ \t]*(\S+))re"_s, text.slice_from(fl[-1].end())) : std::nullopt;
    if (n_regs == 0 || fl.empty() || !nxt || py::re::match(R"re((?:register|int|char|long|unsigned|struct|short)\b)re"_s, nxt->group(1))) return std::nullopt;
    const Match& m = fl[-1];   // (the trailing one, after the last local)
    int64_t n = gi(m, 3) - 2 * n_regs;
    return text.slice(0, m.start()) + (n > 0 ? pf("%schar %s[%d];\n", m.group(1), m.group(2), n) : ""_s) + text.slice_from(m.end());
}

// [the text with one int local unsigned] - a variant each, for the locals compared (<, <=, >, >=): the
// compiler's unsigned jumps
Lines unsigned_locals(const str& text, int64_t most) {
    Lines out;
    Lines names = py::re::findall(R"re((?m)^[ \t]*int (v\d+);$)re"_s, text);
    for (int64_t i = 0; i < std::min<int64_t>(most, names.size()); ++i) {
        const str& name = names[i];
        if (py::re::search(pf(R"re((?:[<>]=? %s\b|\b%s [<>]=?)(?![<>]))re", name, name), text))
            out.append(py::re::sub(pf(R"re((?m)^([ \t]*)int %s;$)re", name), pf(R"re(\1unsigned %s;)re", name), text));
    }
    return out;
}

// the register locals declared after the others (a variant: the slots MS C 3.00 gives some register locals
// below the frame's locals, in the order declared)
std::optional<str> registers_last(const str& text) {
    if (!F::REGSLOTS[0]) return std::nullopt;
    auto sb = split_body(text);
    str head = std::get<0>(sb), sep = std::get<1>(sb), body = std::get<2>(sb);
    if (sep.empty()) return std::nullopt;
    Lines lines = split_nl(body);
    int64_t k = 0;
    while (k < lines.size() && (!nb(lines[k]) || lines[k].strip() == "{")) ++k;
    int64_t j = k;
    while (j < lines.size() && py::re::fullmatch(R"re([ \t]*(?:register |unsigned |int |char |long |short |struct \w+ )[^;(=]*;)re"_s, lines[j])) ++j;
    Lines decl = lines.slice(k, j), regs, rest;
    for (const str& x : decl) (x.strip().startswith("register "_s) ? regs : rest).append(x);
    if (regs.empty() || rest.empty() || decl == rest + regs) return std::nullopt;
    return head + sep + join_nl(lines.slice(0, k) + rest + regs + lines.slice_from(j));
}

namespace {
// data given a string (`g = "..";`, `g[i] = "..";`) declared int: a char * (an array of them)
str string_decls(str text) {
    for (const Match& gm : py::re::finditer(R"re(\b(g_[0-9a-f]{4})(\[[^\]]*\])? = ")re"_s, text)) {
        str g = gm.group(1);
        text = py::re::sub(pf(R"re((?m)^extern int ((?:\w+(?:\[\])?, )*)%s(\[\])?((?:, \w+(?:\[\])?)*);$)re", g), [&](const Match& d) -> str {
            str rest = (d.group(1) + d.group(3).lstrip(", "_s)).strip(", "_s);
            return (!rest.empty() ? pf("extern int %s;\n", rest) : ""_s) + pf("extern char *%s%s;", g, gopt(d, 2));
        }, text);
    }
    return text;
}
}  // namespace

// the drafts where a number passed or given is the place of a string in the data: that string (the model
// reads a literal's address as the number it is); the compiler decides
Lines string_variants(const F::Prj& prj, const str& text) {
    const std::string& all = prj.image.raw();
    std::string ds = prj.dgroup <= static_cast<int64_t>(all.size()) ? all.substr(static_cast<size_t>(prj.dgroup)) : std::string();
    std::set<int64_t> nums;
    for (const str& x : py::re::findall(R"re((?<![\w.])(\d{2,5})(?![\w.]))re"_s, last_after(text, "{"_s))) nums.insert(py::int_from_str(x));
    std::vector<std::pair<int64_t, str>> cands;
    int64_t dn = static_cast<int64_t>(ds.size());
    for (int64_t v : nums) {
        if (0 < v && v < dn - 1 && 32 <= static_cast<unsigned char>(ds[v]) && static_cast<unsigned char>(ds[v]) < 127 && (v == 0 || ds[v - 1] == 0)) {
            size_t e = ds.find('\0', static_cast<size_t>(v));
            int64_t end = e == std::string::npos ? dn - 1 : static_cast<int64_t>(e);   // (ds.find -> -1: ds[v:-1])
            std::string s = end > v ? ds.substr(static_cast<size_t>(v), static_cast<size_t>(end - v)) : std::string();
            bool ok = !s.empty() && s.size() < 200;
            if (ok)
                for (unsigned char c : s)
                    if (!((32 <= c && c < 127) || c == 9 || c == 10 || c == 13)) ok = false;
            if (ok) cands.push_back({v, data::cstr(py::bytes(s))});
        }
    }
    Lines out;
    for (const auto& vl : cands) {   // (each alone, then all together)
        const str lit = vl.second;
        out.append(string_decls(py::re::sub(pf(R"re((?<![\w.])%d(?![\w.]))re", vl.first), [&](const Match&) { return lit; }, text)));
    }
    if (cands.size() > 1) {
        str t = text;
        for (const auto& vl : cands) {
            const str lit = vl.second;
            t = py::re::sub(pf(R"re((?<![\w.])%d(?![\w.]))re", vl.first), [&](const Match&) { return lit; }, t);
        }
        out.append(string_decls(t));
    }
    return out;
}

namespace {
// {data offset: name} of the data SYMBOLS names (where a name differs from the place's own g_XXXX)
const py::dict<int64_t, str>& data_names(const F::Prj& prj) {
    if (!prj.dnames) {
        py::dict<int64_t, str> d;
        for (const auto& kv : prj.symbols.items()) {
            const str& n = std::get<0>(kv);
            const str& t = std::get<0>(std::get<1>(kv));
            int64_t a = std::get<1>(std::get<1>(kv));
            if (t == "D" && !n.contains(":"_s) && py::re::fullmatch(R"re(_?\w+)re"_s, n) && n.lstrip("_"_s) != hex4(a)) d.set(a, n.lstrip("_"_s));
        }
        prj.dnames = d;
    }
    return *prj.dnames;
}
}  // namespace

// the model's text with the engine's names and the declarations it needs; None if a machine register is left
std::tuple<str, Lines> localize(const F::Prj& prj, str text, int64_t at, const py::dict<int64_t, str>& names) {
    auto call = [&](const Match& m) -> str {
        int64_t rel = py::int_parse(m.group(1), 16);
        if (rel >= 0x80000000LL) rel -= int64_t(1) << 32;
        int64_t t = at + rel >= 0 ? ((at + rel) & 0xFFFFF) : at + rel + 0x10000;   // (a near call wraps in its 64K: the segment's)
        if (!names.contains(t)) {   // (the 64K it wraps in: where a routine is)
            for (int64_t x : {t - 0x10000, t + 0x10000})
                if (names.contains(x)) {
                    t = x;
                    break;
                }
        }
        std::optional<str> nm = names.contains(t) ? std::optional<str>(names.at(t)) : std::nullopt;
        return ((nm && !nm->empty()) ? *nm : pf("f%05x", t)) + "("_s;
    };
    const str subrx = R"re(\bsub_(-?[0-9a-f]+)\()re"_s;
    std::set<std::string> longf;   // (routines of a long value)
    if (T::LONGINFO.funcs)
        for (const str& f : *T::LONGINFO.funcs) longf.insert(py::re::sub(subrx, call, f + "("_s).slice(0, -1).utf8());
    text = py::re::sub(subrx, call, text);
    text = py::re::sub(R"re(\bg_([0-9a-f]{1,4})\b)re"_s, [](const Match& m) { return hex4(py::int_parse(m.group(1), 16)); }, text);
    text = py::re::sub(R"re((?m)^([ \t]*)(L_\w+:)\n(?=[ \t]*\}))re"_s, R"re(\1\2\n\1;\n)re"_s, text);   // (a label last in its block: on an empty statement)
    const py::list<F::LibMacro>& macros = prj.macros;
    text = py::re::sub(R"re(\*\((\w+)(\+\+|--)\))re"_s, R"re(*\1\2)re"_s, text);   // (*(p++): *p++ - the same, and mendable)
    text = fp_conversions(text, prj.fp_conversions);
    text = long_helpers(text, prj.long_helpers);
    text = F::library_macros(text, macros);   // (the routines named: the macros they show)
    text = S::call_tests(F::data_macros(prj, text, prj.data_macros));   // (a macro's value tested, switched on)
    if (!T::FILEPTR.empty()) {   // (the stream's fields the struct pass read: the macro's now)
        auto norm = [](const str& n) { return py::re::fullmatch(R"re(g_[0-9a-f]{1,4})re"_s, n) ? hex4(py::int_parse(n.slice_from(2), 16)) : n; };
        std::set<std::string> streams;
        for (const auto& kv : T::FILEPTR.items())
            if (!std::get<0>(kv).startswith("#"_s)) streams.insert(norm(std::get<0>(kv)).utf8());
        text = py::re::sub(R"re((?m)^struct s_(\w+) \{[^}]*\};\n)re"_s, [&](const Match& m) -> str {
            return streams.count(norm(m.group(1)).utf8()) ? ""_s : m.group(0);
        }, text);
        text = S::fold_registers(S::stored_calls(text));
        text = F::unused_locals(text);
    }
    std::set<std::string> macnames;
    for (const F::LibMacro& m : macros) macnames.insert(word_prefix(m.as).utf8());
    for (const F::DataMacro& sp : prj.data_macros)
        for (const auto& kv : sp.fields.items()) macnames.insert(word_prefix(std::get<1>(kv)).utf8());
    auto body_of = [](const str& t) { return t.contains("{"_s) ? last_after(t, "{"_s) : t; };   // text.split("{", 1)[1] if "{" in text else text
    str body = body_of(text);
    // registers the model left: a draft for a person, not for the compiler
    Lines left = sorted_unique(py::re::findall(T::REGS, py::re::sub(R"re("(?:[^"\\]|\\.)*")re"_s, ""_s, body)));
    Lines data = sorted_unique(py::re::findall(R"re(\bg_[0-9a-f]{4}\b)re"_s, text));
    str head;   // (its own name: after a type)
    {
        Lines before = py::split(py::split(text, "("_s, 1)[0]);
        head = !before.empty() ? before[-1] : ""_s;
    }
    Lines funcs;
    {
        std::set<std::string> skip{head.utf8(), "if", "while", "for", "switch", "return", "sizeof"};
        for (const str& f : sorted_unique(py::re::findall(R"re(\b([A-Za-z_]\w*)\()re"_s, body)))
            if (!skip.count(f.utf8()) && !macnames.count(f.utf8())) funcs.append(f);
    }
    Lines decl;
    Lines fptr;   // (data called through)
    for (const str& d : data)
        if (py::re::search(pf(R"re(\(\s*\*\s*%s\s*\)\s*\()re", d), body)) fptr.append(d);
    for (const str& d : fptr) {   // (a number given it that is a function's place: it)
        text = py::re::sub(pf(R"re((?m)^(\s*%s = )(\d+);)re", d), [&](const Match& m) -> str {
            int64_t a = gi(m, 2);
            std::optional<str> nm = names.contains(a) ? std::optional<str>(names.at(a)) : std::nullopt;
            return m.group(1) + ((nm && !nm->empty()) ? *nm : pf("f%05x", a)) + ";"_s;
        }, text);
        body = body_of(text);
    }
    {
        std::set<std::string> fs;
        for (const str& f : funcs) fs.insert(f.utf8());
        for (const str& d : fptr)
            for (const str& w : py::re::findall(pf(R"re((?m)^[ \t]*%s = ([A-Za-z_]\w*);)re", d), body)) fs.insert(w.utf8());
        funcs = Lines();
        for (const std::string& f : fs) funcs.append(str(f));
    }
    for (const str& a_ : [&] {
             Lines r;
             for (const str& d : data)
                 if (T::ARRAYS.contains(d) && T::ARRAYS.at(d) == "int") r.append(d);
             return r;
         }()) {   // (an array of words named whole, and the words just after it by their own places: its elements)
        int64_t base = py::int_parse(a_.slice_from(2), 16);
        Lines near_;
        for (const str& d : data) {
            int64_t dv = py::int_parse(d.slice_from(2), 16);
            if (d != a_ && !T::ARRAYS.contains(d) && 0 < dv - base && dv - base < 0x40 && (dv - base) % 2 == 0) near_.append(d);
        }
        if (py::re::search(pf(R"re(\b%s\b(?!\[))re", a_), body) || !near_.empty()) {
            auto [hd, sp_, bd] = text.partition("{"_s);
            str bd2 = py::re::sub(pf(R"re(\b%s\b(?!\[))re", a_), pf("%s[0]", a_), bd);
            for (const str& d : near_) bd2 = py::re::sub(pf(R"re(\b%s\b)re", d), pf("%s[%d]", a_, (py::int_parse(d.slice_from(2), 16) - base) / 2), bd2);
            text = hd + sp_ + bd2;
            body = bd2;
            Lines nd;
            for (const str& d : data)
                if (!near_.contains(d)) nd.append(d);
            data = nd;
        }
    }
    Lines arrs;   // (indexed in place: an array, not a pointer)
    for (const str& d : data)
        if (T::ARRAYS.contains(d) && !fptr.contains(d)) arrs.append(d);
    std::set<std::string> pvars;   // (pointers the draft declares)
    for (const str& p : py::re::findall(R"re((?m)^[ \t]*(?:register\s+)?(?:unsigned\s+)?(?:char|int)\s*\*\s*(\w+);)re"_s, text)) pvars.insert(p.utf8());
    OrdMap<str> sptrs;
    for (const Match& m : py::re::finditer(R"re((?m)^[ \t]*(?:register\s+)?(struct \w+)\s*\*\s*(\w+);)re"_s, text)) {
        if (str* v = sptrs.find(m.group(2))) *v = m.group(1);
        else sptrs.items.push_back({m.group(2), m.group(1)});
    }
    for (const auto& kv : sptrs.items) pvars.insert(kv.first.utf8());
    auto flows = [&](const str& d) {
        for (const std::string& p : pvars)
            if (py::re::search(pf(R"re((?m)^[ \t]*%s = %s\b|^\s*%s = %s\b)re", str(p), d, d, str(p)), body)) return true;
        return false;
    };
    for (const str& d : data) {   // (data given a struct pointer's value: a pointer to it)
        std::optional<str> src_;
        for (const auto& kv : sptrs.items)
            if (py::re::search(pf(R"re((?m)^[ \t]*(?:\w+ = )*%s = (?:\w+ = )*%s;$)re", d, kv.first), body)) {
                src_ = kv.first;
                break;
            }
        if (src_ && !STRUCTPTR.contains(d)) STRUCTPTR.set(d, py::split(*sptrs.find(*src_))[1]);
    }
    Lines ptrs;
    for (const str& d : data)
        if (!fptr.contains(d) && !arrs.contains(d) &&
            (py::re::search(pf(R"re(\*\s*\(?\s*(?:\+\+|--)?\s*%s\b|\b%s\s*(?:\+\+|--)?\s*(?:->|\[))re", d, d), body) || flows(d)))   // (or given a pointer's value, or giving one)
            ptrs.append(d);
    Lines ints;
    for (const str& d : data)
        if (!ptrs.contains(d) && !fptr.contains(d) && !arrs.contains(d)) ints.append(d);
    for (const Match& m : py::re::finditer(R"re(\*\(struct _b(\d+) \*\)(\w+) = \*\(struct _b\1 \*\)(\w+);)re"_s, text)) {
        str nb_ = m.group(1);
        for (const str& v_ : {m.group(2), m.group(3)}) {   // (the pointers copied through: pointers to the block)
            std::optional<str> t_ = _redeclare(text, v_, pf("struct _b%s *%s", nb_, v_));
            if (t_ && !t_->empty() && !py::re::search(pf(R"re(\b%s\s*(?:->|\[))re", v_), text))
                text = t_->replace(pf("*(struct _b%s *)%s", nb_, v_), pf("*%s", v_));
        }
    }
    {
        std::vector<int64_t> nbs;   // (a block of bytes copied whole)
        for (const str& x : py::re::findall(R"re(\bstruct _b(\d+) \*)re"_s, text)) {
            int64_t v = py::int_from_str(x);
            if (std::find(nbs.begin(), nbs.end(), v) == nbs.end()) nbs.push_back(v);
        }
        std::sort(nbs.begin(), nbs.end());
        // (sorted(.., key=int) of the distinct texts: "08" and "8" would be two; the same numbers here)
        for (int64_t nbv : nbs) decl.insert(0, pf("struct _b%d { char _c[%d]; };", nbv, nbv));
    }
    if (!arrs.empty()) {
        // (arrays of structs of one size whose places lie within one element: one array, its fields at the
        //  places' distances from the first)
        std::vector<std::pair<int64_t, str>> sa;
        for (const str& d : arrs)
            if (T::STRUCTARR.contains(d)) sa.push_back({py::int_parse(d.slice_from(2), 16), d});
        std::sort(sa.begin(), sa.end());
        std::vector<std::pair<int64_t, Lines>> merged;
        for (const auto& bd : sa) {
            int64_t b = bd.first;
            const str& d = bd.second;
            int64_t sz = std::get<0>(T::STRUCTARR.at(d));
            std::optional<size_t> base;
            for (size_t i = 0; i < merged.size(); ++i)
                if (std::get<0>(T::STRUCTARR.at(merged[i].second[0])) == sz && 0 < b - merged[i].first && b - merged[i].first < sz) {
                    base = i;
                    break;
                }
            if (!base) merged.push_back({b, Lines{d}});
            else merged[*base].second.append(d);
        }
        for (const auto& bds : merged) {
            int64_t base = bds.first;
            const Lines& ds = bds.second;
            if (ds.size() < 2) continue;
            str bn = ds[0];
            for (int64_t i = 1; i < ds.size(); ++i) {
                const str& d = ds[i];
                int64_t off = py::int_parse(d.slice_from(2), 16) - base;
                text = py::re::sub(pf(R"re(\b%s\[([^\[\]]+)\]\.f0\b)re", d), [&](const Match& m) { return pf("%s[%s].f%d", bn, m.group(1), off); }, text);
                Lines na;
                for (const str& x : arrs)
                    if (x != d) na.append(x);
                arrs = na;
            }
            py::list<std::tuple<int64_t, str>> elems;
            for (const str& d : ds) {
                const SA& ty = std::get<1>(T::STRUCTARR.at(d));
                // (an element's type: its own, or an int where it is a struct already - a list is a struct's too)
                str et = std::holds_alternative<str>(ty) && !std::get<str>(ty).startswith("struct"_s) ? std::get<str>(ty) : "int"_s;
                elems.append(std::make_tuple(py::int_parse(d.slice_from(2), 16) - base, et));
            }
            T::STRUCTARR.set(bn, std::make_tuple(std::get<0>(T::STRUCTARR.at(bn)), SA(elems)));
        }
        for (const str& d : arrs) {   // (an array of structs: its element laid out - the field the code reads first, the rest of its size after)
            if (!T::STRUCTARR.contains(d)) continue;
            int64_t k = std::get<0>(T::STRUCTARR.at(d));
            SA tyv = std::get<1>(T::STRUCTARR.at(d));
            Lines fl;
            int64_t pos = 0;
            std::vector<std::pair<int64_t, str>> elems;
            if (std::holds_alternative<str>(tyv)) elems.push_back({0, std::get<str>(tyv)});
            else
                for (const auto& e : std::get<py::list<std::tuple<int64_t, str>>>(tyv)) elems.push_back({std::get<0>(e), std::get<1>(e)});
            for (const auto& ot : elems) {
                int64_t off = ot.first;
                str t_ = ot.second;
                if (off > pos) fl.append(pf("char _%d[%d];", pos, off - pos));
                if (t_ == "int" && py::re::search(pf(R"re(\b%s\[[^\]]*\]\.f%d->)re", d, off), text) && py::re::search(pf(R"re((?m)^struct s_f%d \{)re", off), text))
                    t_ = pf("struct s_f%d *", off);   // (a field gone through: a pointer to what it reads)
                else if (t_ == "int" && py::re::search(pf(R"re((?<![\w)\]])\*%s\[[^\]]*\]\.f%d\b(?!\s*(?:->|\[|\.)))re", d, off), text))
                    t_ = "int *"_s;
                fl.append(pf("%s f%d;", t_, off));
                pos = off + (t_ == "char" ? 1 : 2);
            }
            if (k > pos) fl.append(pf("char _%d[%d];", pos, k - pos));
            decl.append(pf("struct s_%s { %s };", d, py::join(" "_s, fl)));
        }
        for (const str& d : arrs) decl.append(pf("extern %s %s[];", T::ARRAYS.at(d), d));
    }
    if (!fptr.empty()) {
        Lines xs;
        for (const str& d : fptr) xs.append(pf("(*%s)()", d));
        decl.append(pf("extern int %s;", py::join(", "_s, xs)));
    }
    Lines streams;   // (a stream a library macro reads)
    for (const str& d : data)
        if (T::FILEPTR.contains(d)) streams.append(d);
    {
        Lines np, ni;
        for (const str& d : ptrs)
            if (!streams.contains(d)) np.append(d);
        for (const str& d : ints)
            if (!streams.contains(d)) ni.append(d);
        ptrs = np;
        ints = ni;
    }
    for (const str& d : streams) {
        const std::optional<str>& ft = T::FILEPTR.at(d);
        decl.append(pf("extern %s *%s;", ft ? *ft : "None"_s, d));
    }
    str head2, sep2, body2;
    {
        auto pt = text.partition("{"_s);
        head2 = std::get<0>(pt);
        sep2 = std::get<1>(pt);
        body2 = std::get<2>(pt);
    }
    {
        Lines pnames;   // (a parameter that is one)
        for (const auto& kv : T::FILEPTR.items()) {
            const str& n = std::get<0>(kv);
            if (!n.startswith("#"_s) && !data.contains(n)) pnames.append(n);
        }
        for (const str& p : pnames) {
            const std::optional<str>& ft = T::FILEPTR.at(p);
            str fts = ft ? *ft : "None"_s;
            head2 = py::re::sub(pf(R"re((?m)^([ \t]*)(?:register )?(?:int|char \*|struct \w+ \*) ?%s;$)re", p), pf(R"re(\1%s *%s;)re", fts, p), head2);
            head2 = py::re::sub(pf(R"re((?m)^([ \t]*)int ((?:\w+, )*)%s((?:, \w+)*);$)re", p), [&](const Match& m) -> str {
                str rest = (m.group(2) + m.group(3).lstrip(", "_s)).strip(", "_s);
                return (!rest.empty() ? pf("%sint %s;\n", m.group(1), rest) : ""_s) + pf("%s%s *%s;", m.group(1), fts, p);
            }, head2);
        }
    }
    text = head2 + sep2 + body2;
    Lines incs;
    for (const char* k : {"#include", "#include2"}) {
        auto it = T::FILEPTR.get(str(k));
        if (it && *it && !(*it)->empty() && !incs.contains(**it)) incs.append(**it);
    }
    for (int64_t i = incs.size() - 1; i >= 0; --i) decl.insert(0, pf("#include <%s>", incs[i]));
    if (!ptrs.empty()) {   // (data the draft goes through: pointers)
        OrdMap<str> sp_;
        for (const auto& kv : STRUCTPTR.items()) {
            str k = py::re::fullmatch(R"re(g_[0-9a-f]{1,4})re"_s, std::get<0>(kv)) ? hex4(py::int_parse(std::get<0>(kv).slice_from(2), 16)) : std::get<0>(kv);
            if (str* v = sp_.find(k)) *v = std::get<1>(kv);
            else sp_.items.push_back({k, std::get<1>(kv)});
        }
        for (const str& d : ptrs) {   // (a word read through it, never a byte: an int *)
            bool anyw = false, anyb = false;
            for (const char* r : {"bx", "si", "di"}) {
                if (T::BYTEPTR.contains(std::make_tuple(str(std::string(r) + "w"), int64_t(0)))) anyw = true;
                if (T::BYTEPTR.contains(std::make_tuple(str(std::string(r) + "*"), int64_t(0)))) anyb = true;
            }
            bool word = anyw && !anyb && py::re::search(pf(R"re(\*%s\b)re", d), body);
            auto ps = T::PTRSIZE.get(d);
            bool ps2 = ps && *ps == 2;
            str ty = sp_.contains(d) ? "struct "_s + *sp_.find(d)
                     : (ps2 && py::re::search(pf(R"re(\*\+\+%s\b)re", d), body)) ? "char *"_s
                     : (word || ps2)         ? "int"_s
                                             : "char"_s;
            decl.append(pf("extern %s *%s;", ty, d));
        }
    }
    Lines uch, chs, ints2;
    for (const str& d : ints) {
        bool in_u = T::UCHAR.contains(SAv(d)), in_b = T::BYTEG.contains(d);
        if (in_u) uch.append(d);
        if (in_b && !in_u) chs.append(d);
        if (!in_u && !in_b) ints2.append(d);
    }
    ints = ints2;
    if (!uch.empty()) decl.append(pf("extern unsigned char %s;", py::join(", "_s, uch)));
    if (!chs.empty()) decl.append(pf("extern char %s;", py::join(", "_s, chs)));
    Lines lngs;
    {
        Lines rest;
        for (const str& d : ints) {
            if (T::LONGINFO.globals && T::LONGINFO.globals->contains(d)) lngs.append(d);   // (a long in the data: its two words one)
        }
        for (const str& d : ints)
            if (!lngs.contains(d)) rest.append(d);
        ints = rest;
    }
    if (!lngs.empty()) decl.append(pf("extern long %s;", py::join(", "_s, lngs)));
    if (!ints.empty()) decl.append(pf("extern int %s;", py::join(", "_s, ints)));
    for (const str& h : incs) {   // (what an included header declares: its)
        Lines nf;
        for (const str& f : funcs)   // (the header's names read at the first routine: a comprehension asks for them per element)
            if (!F::header_names(prj, h).contains(f)) nf.append(f);
        funcs = nf;
    }
    {
        Lines lf, rest;
        for (const str& f : funcs) (longf.count(f.utf8()) ? lf : rest).append(f);
        if (!lf.empty()) {
            Lines xs;
            for (const str& f : lf) xs.append(pf("%s()", f));
            decl.append(pf("extern long %s;", py::join(", "_s, xs)));
        }
        funcs = rest;
    }
    if (!funcs.empty()) {
        Lines xs;
        for (const str& f : funcs) xs.append(pf("%s()", f));
        decl.append(pf("extern %s;", py::join(", "_s, xs)));
    }
    text = fp_types(text);
    // (a byte through a char pointer compared with 128..255: the compiler compares bytes only of an unsigned char)
    for (const str& p : unique(py::re::findall(R"re(\*\(?(?:\+\+|--)?(\w+)(?:\+\+|--)?\)? [!=]= (?:1(?:2[89]|[3-9]\d)|2[0-4]\d|25[0-5])\b)re"_s, text)))
        text = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?char \*%s;$)re", p), pf(R"re(\1\2unsigned char *%s;)re", p), text);
    for (const Match& pken : py::re::finditer(R"re((?m)^[ \t]*(\w+) = \((\d+) \* ([^()]+?)\) \+ (\d+);$)re"_s, text)) {
        str p_ = pken.group(1), k_ = pken.group(2), e_ = pken.group(3), n_ = pken.group(4);
        OMatch tag = py::re::search(pf(R"re((?m)^[ \t]*(?:register )?struct (\w+) \*%s;$)re", p_), text);   // (a struct pointer given K * E + N: the address of an element of an array there)
        OMatch dm = tag ? py::re::search(pf(R"re((?m)^struct %s \{([^}]*)\};$)re", tag->group(1)), text) : std::nullopt;
        if (dm && py::int_from_str(n_) >= 0x100) {
            int64_t k = py::int_from_str(k_), n = py::int_from_str(n_);
            int64_t size = 0;
            for (const Match& f : py::re::finditer(R"re((char|int|long|\*) ?_?f?(\d+)(?:\[(\d+)\])?;)re"_s, dm->group(1))) {
                str c = gopt(f, 3);
                int64_t w = !c.empty() ? py::int_from_str(c) : f.group(1) == "long" ? 4 : f.group(1) == "char" ? 1 : 2;
                size = std::max(size, gi(f, 2) + w);
            }
            if (size <= k) {
                str body_ = dm->group(1) + (size < k ? pf(" char _%d[%d];", size, k - size) : ""_s);
                str name = hex4(n);
                text = text.replace(dm->group(0), pf("struct %s {%s };\nextern struct %s %s[];", tag->group(1), body_.rstrip(), tag->group(1), name));
                text = py::re::sub(pf(R"re((?m)^([ \t]*)%s = \(%s \* %s\) \+ %s;$)re", p_, k_, esc(e_), n_), pf(R"re(\1%s = &%s[%s];)re", p_, name, e_), text);
            }
        }
    }
    {
        std::vector<std::pair<str, str>> tx;   // (only ever read through one cast: a pointer to it)
        for (const Match& m : py::re::finditer(R"re(\*\((char|int|unsigned char) \*\)(\w+)\b)re"_s, text)) {
            std::pair<str, str> p{m.group(1), m.group(2)};
            if (std::find(tx.begin(), tx.end(), p) == tx.end()) tx.push_back(p);
        }
        for (const auto& tyx : tx) {
            const str& ty = tyx.first;
            const str& x = tyx.second;
            Lines others = py::re::findall(pf(R"re(\*\((?:char|int|unsigned char) \*\)%s\b)re", x), text);
            bool all_ = true;
            for (const str& o : others)
                if (o != pf("*(%s *)%s", ty, x)) all_ = false;
            if (all_ && !py::re::search(pf(R"re((?<![)\w])\*\s*%s\b|\b%s\s*(?:->|\[))re", x, x), text)) {
                std::optional<str> t2 = py::re::search(pf(R"re((?m)^[ \t]*(?:register )?int (?:\w+, )*%s\b)re", x), text)
                                            ? _redeclare(text, x, pf("%s *%s", ty, x))
                                            : std::nullopt;
                if (t2 && !t2->empty()) text = t2->replace(pf("*(%s *)%s", ty, x), pf("*%s", x));
            }
        }
    }
    str out = (!decl.empty() ? py::join("\n"_s, decl) + "\n\n"_s : ""_s) + text;
    const py::dict<int64_t, str>& dn = data_names(prj);   // (data the program has names for: those, by place - a name of the form g_XXXX may be another place's)
    if (!dn.empty())
        out = py::re::sub(R"re(\bg_([0-9a-f]{4})\b)re"_s, [&](const Match& m) -> str {
            int64_t a = py::int_parse(m.group(1), 16);
            return dn.contains(a) ? dn.at(a) : m.group(0);
        }, out);
    return {out, left};
}

// the library's long arithmetic (the profile's "long_helpers": routine -> C operator) as the operator:
// lmul(X, Y) is (X * Y); an assigning one, almul(&X, Y), is X *= Y
str long_helpers(const str& text, const py::dict<str, str>& helpers) {
    if (helpers.empty()) return text;
    Lines hs;
    for (const auto& kv : helpers.items()) hs.append(esc(std::get<0>(kv)));
    const str rx = pf(R"re(\b(%s)\(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*)\))re", py::join("|"_s, hs));
    std::set<std::string> uns;
    str t = py::re::sub(rx, [&](const Match& m) -> str {
        str op = helpers.at(m.group(1));
        Lines args;
        for (const str& x : _top_commas(m.group(2))) args.append(x.strip());
        if (args.size() == 3) {   // (the long operand pushed a word at a time: its words a field's two - the long there)
            OMatch f1 = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, args[1]);
            OMatch f2 = py::re::fullmatch(R"re((\w+(?:->f\d+)*)->f(\d+))re"_s, args[2]);
            if (f1 && f2 && f1->group(1) == f2->group(1) && gi(*f2, 2) == gi(*f1, 2) + 2) args = Lines{args[0], pf("*(long *)&%s", args[1])};
        }
        if (args.size() != 2) return m.group(0);
        if (py::re::match(R"re(a?ul)re"_s, m.group(1)) && py::re::fullmatch(R"re(&?\w+)re"_s, args[0])) uns.insert(args[0].lstrip("&"_s).utf8());   // (an unsigned one's operand: an unsigned long)
        if (op.endswith("="_s) && op != "==" && op != "<=" && op != ">=" && op != "!=") {
            if (!args[0].startswith("&"_s)) return m.group(0);
            return pf("(%s %s %s)", args[0].slice_from(1), op, args[1]);
        }
        str v = pf("(%s %s %s)", args[0], op, args[1]);
        str before = text.slice(0, m.start()).rstrip(), after = text.slice_from(m.end()).lstrip();
        if (py::re::search(R"re(\w\($|,$)re"_s, before) && (after.slice(0, 1) == ")" || after.slice(0, 1) == ","))   // (an argument pushed as one word: the long's low word - an int)
            return pf("(int)%s", v);
        return v;
    }, text);
    for (const std::string& x : uns)
        t = py::re::sub(pf(R"re((?m)^([ \t]*)(register )?long %s;$)re", str(x)), pf(R"re(\1\2unsigned long %s;)re", str(x)), t);
    t = py::re::sub(R"re((?m)^([ \t]*)\((.+ [*/%<>+-]{1,2}= .+)\);$)re"_s, R"re(\1\2;)re"_s, t);   // (a statement of its own: no parens)
    // (the long it leaves in dx:ax tested at once - `ax = (ax | dx)`: the assignment's value)
    str t2 = py::re::sub(R"re((?m)^([ \t]*)(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n\s*ax = \(ax \| dx\);$)re"_s, R"re(\1ax = (\2);)re"_s, t);
    t2 = py::re::sub(R"re((?m)^[ \t]*(\w+ (?:[*/%+-]|<<|>>)= [^;\n]+);\n(\s*)\} while \(\(\(ax \| dx\) != 0\)\);$)re"_s, R"re(\2} while (((\1) != 0));)re"_s, t2);   // (a do's test of it)
    return t2 != t ? S::fold_registers(t2) : t;
}

// the library's conversion of the 8087's top to an integer (the profile's "fp_conversions": its names - ftol)
// is C's own conversion in an assignment: `X = ftol(E);` is `X = E;`, and `p->f0 = ftol(E); p->f2 = dx;` (the
// high half too) `*p = E;` of a long *p
str fp_conversions(str text, const Lines& routines) {
    if (routines.empty()) return text;
    Lines es;
    for (const str& r : routines) es.append(esc(r));
    str rx = py::join("|"_s, es);
    const str arg = R"re(((?:[^()]|\((?:[^()]|\([^()]*\))*\))*))re"_s;
    text = py::re::sub(pf(R"re((?m)^([ \t]*)(\w+)->f0 = (?:%s)\(%s\);\n\s*\2->f2 = dx;$)re", rx, arg), [](const Match& m) -> str {
        FPINFO.types.set("*"_s + m.group(2), "long"_s);
        return pf("%s*%s = %s;", m.group(1), m.group(2), m.group(3));
    }, text);
    text = py::re::sub(pf(R"re((?m)^([ \t]*)(.+?) = (?:%s)\(%s\);$)re", rx, arg), R"re(\1\2 = \3;)re"_s, text);
    return text;
}

// the types the 8087's operands showed (FPINFO): `*p` a float/double/long - p a pointer to one; vN or a
// parameter one itself; data declared so; a function giving a double back declared double
str fp_types(const str& text) {
    if (FPINFO.types.empty() && !(FPINFO.ret && !FPINFO.ret->empty())) return text;
    auto sb = split_body(text);
    str head = std::get<0>(sb), sep = std::get<1>(sb), body = std::get<2>(sb);
    if (sep.empty()) return text;
    str pre, fn = head;
    OMatch m = py::re::search(R"re((?m)^(?:\w+\s*\n)?\w+\()re"_s, head);   // (the declarations before the function: data's)
    if (m) {
        pre = head.slice(0, m->start());
        fn = head.slice_from(m->start());
    }
    for (const auto& nty : FPINFO.types.items()) {
        const str& nm = std::get<0>(nty);
        const str& ty = std::get<1>(nty);
        OMatch mm = py::re::fullmatch(R"re(\*\(?(\w+)\)?)re"_s, nm);
        str x = mm ? mm->group(1) : nm;
        bool ptr = mm.has_value();
        if (!py::re::fullmatch(R"re(\w+)re"_s, x)) continue;
        str nw = pf("%s %s%s", ty, ptr ? "*"_s : ""_s, x);
        std::optional<str> f2 = _redeclare(fn, x, nw);
        if (f2 && !f2->empty()) {
            fn = *f2;
            continue;
        }
        Lines lines = split_nl(body);   // (a local: among the declarations at the top)
        bool broke = false;
        for (int64_t k = 0; k < lines.size(); ++k) {
            const str& l = lines[k];
            if (nb(l) && !py::re::match(R"re(\s*(?:register\s+)?(?:int|char|long|unsigned|short|struct|float|double)\b)re"_s, l)) {
                broke = true;
                break;
            }
            std::optional<str> l2 = nb(l) ? _redeclare(l, x, nw) : std::nullopt;
            if (l2 && !l2->empty()) {
                lines[k] = *l2;
                broke = true;
                break;
            }
        }
        if (broke) body = join_nl(lines);   // (the loop's else: no break - lines None: body kept)
        if (x.startswith("g_"_s)) {
            pre = py::re::sub(pf(R"re((?m)^extern int ((?:\w+, )*)%s((?:, \w+)*);$)re", x), [&](const Match& d) -> str {
                str rest = (d.group(1) + d.group(2).lstrip(", "_s)).strip(", "_s);
                return (!rest.empty() ? pf("extern int %s;\n", rest) : ""_s) + pf("extern %s;", nw);
            }, pre);
        }
    }
    if (FPINFO.ret && !FPINFO.ret->empty()) {
        OMatch mf = py::re::search(R"re((?m)^\w+\()re"_s, fn);
        if (mf && !py::re::search(R"re((?m)^double\s*$)re"_s, fn)) fn = fn.slice(0, mf->start()) + *FPINFO.ret + "\n"_s + fn.slice_from(mf->start());
    }
    return pre + fn + sep + body;
}

// locals declared for the part of the frame no declared local takes (a local the source declared and never
// used still has its place; MS C 3.00 gives a register local one too): a filler `char _pf[n]` at the end
str fill_frame(const str& text, int64_t fs, bool regs_count) {
    if (fs == 0 || !text.contains("{"_s)) return text;
    auto sb = split_body(text);
    str head = std::get<0>(sb), sep = std::get<1>(sb), body = std::get<2>(sb);
    int64_t size = 0;
    Lines lines = split_nl(body);
    int64_t last = 0;
    std::vector<std::pair<int64_t, int64_t>> gaps;   // ((line, bytes): a filler before a local vN the declarations above would put nearer bp than N)
    for (int64_t k = 1; k < lines.size(); ++k) {
        const str& l = lines[k];
        OMatch m = py::re::match(R"re(\s+(register\s+)?((?:unsigned\s+)?(?:int|char|long|short|struct\s+\w+))\s*(\**)\s*(\w+)\s*(?:\[(\d+)\])?;\s*$)re"_s, l);
        if (!m) {
            if (nb(l)) break;
            continue;
        }
        str reg = gopt(*m, 1), ty = m->group(2), star = m->group(3), name = m->group(4), cnt = gopt(*m, 5);
        last = k;
        if (!reg.empty()) {
            size += regs_count ? 2 : 0;
            continue;
        }
        if (ty.startswith("struct"_s) && star.empty()) return text;   // (a struct's size the text does not say)
        Lines tw = py::split(ty);
        int64_t one = !star.empty() ? 2 : T::SIZES.get(tw[-1], 2);
        int64_t n = one * (cnt.empty() ? 1 : py::int_from_str(cnt));
        OMatch v = py::re::fullmatch(R"re(v(\d+))re"_s, name);
        if (v && star.empty() && cnt.empty() && tw[-1] == "long" && size + n < gi(*v, 1)) {
            gaps.push_back({k, gi(*v, 1) - size - n});   // (a long the pair made of vN and the word above it: where vN was)
            size = gi(*v, 1) - n;
        }
        size += n + (n & 1);
    }
    for (int64_t i = static_cast<int64_t>(gaps.size()) - 1; i >= 0; --i) {
        lines.insert(gaps[i].first, pf("    char _pg%d[%d];", gaps[i].first, gaps[i].second));
        last += 1;
    }
    if (!gaps.empty() && fs > size) {
        lines.insert(last + 1, pf("    char _pf[%d];", fs - size));
        return head + sep + join_nl(lines);
    }
    if (fs <= size || py::re::search(R"re((?m)^\s+(?:int|char|long|short)\s+v\d+)re"_s, body))
        return gaps.empty() ? text : head + sep + join_nl(lines);   // (the model's autos: the frame model lays those out)
    str fn = FILLNAME[0];
    lines.insert(last + 1, pf("    char %s[%d];", fn.contains("%"_s) ? py::percent(fn, size) : fn, fs - size));
    return head + sep + join_nl(lines);
}

// [(source, registers left)] - the model's text as the project's source, then the variants the compiler
// is to decide between: as read, without its last return (a function of no value), with the locals whose
// address it takes as the buffers they are - and both
py::list<std::tuple<str, Lines>> candidates(const F::Prj& prj, const str& text, int64_t at, const py::dict<int64_t, str>& names,
                                            const Lines& regs, const py::dict<str, int64_t>& regparam, const ByteFields& widths) {
    Lines base;
    base.append(text);
    {
        std::optional<str> v = void_variant(text);
        if (v && !v->empty()) base.append(*v);
    }
    Lines bases = base.copy();
    for (const str& t : base) {
        std::optional<str> v = buffers(t);
        if (v && !v->empty()) bases.append(*v);
    }
    std::vector<std::vector<std::tuple<str, Lines>>> out;
    F::REGSLOTS[0] = prj.frame_order.has_value();
    auto tidy = [](const std::optional<str>& o) { return o && !o->empty(); };
    for (str t : bases) {
        STRUCTPTR.clear();
        T::PTRSIZE.clear();
        T::ARRAYS.clear();
        T::STRUCTARR.clear();
        clear_set(T::UCHAR);
        clear_set(T::BYTEG);
        for (const str& g : widths.globals) T::BYTEG.add(g);
        T::FILEPTR.clear();
        t = py::re::sub(R"re(\btarg(\d+)\b)re"_s, R"re(t9\1)re"_s, t);   // (the model's temporaries of a shared tail: temporaries)
        t = S::zero_extends(S::high_zero(S::copy_props(F::table_macros(prj, F::case_falls(F::char_stores(F::char_params(long_pairs(long_returns(F::store_reloads(S::fall_gotos(t))))))), prj.table_macros))));
        t = py::re::sub(R"re(&(\w+)\[(-?)(\d+)\])re"_s, [](const Match& m) -> str { return pf("(%s %s %s)", m.group(1), m.group(2).empty() ? "+"_s : "-"_s, m.group(3)); }, t);
        t = py::re::sub(R"re(&(\w+)->f(\d+)\b)re"_s, R"re((\1 + \2))re"_s, t);   // (lea: &p[N], &p->fN - the place, p + N)
        {
            str gt = or_str(S::goto_tails(t), t);
            t = S::fold_registers(S::stored_calls(S::store_loops(S::post_increments(S::widen_bytes(S::compare_values(S::chain_returns(S::branch_returns(S::push_returns(
                S::conditional_values(S::logical_values(S::step_tests(F::bool_trees(S::range_switches(F::loop_gotos(S::empty_loops(S::tested_zero(S::continue_loops(S::first_breaks(
                    S::loop_registers(S::fall_dups(S::fall_gotos(gt))))))))))))))))))))));
        }
        t = S::stored_calls(S::fold_registers(S::push_returns(t)));   // (returns again, as the passes left the arms)
        t = S::stored_reads(F::scaled_index(S::register_chains(F::word_arrays(S::pointer_index(S::test_first_loops(S::copy_tests(S::post_steps(S::selector_reads(S::lvalue_choices(t))))))))));
        t = S::fold_registers(S::stored_values(S::conditional_values(S::store_joins(S::ternary_joins(S::switch_ifs(S::loop_collapse(S::two_case_switches(S::long_tests(t)))))))));   // (the ?: values again, the elements folded)
        t = S::fold_registers(S::stored_tests(S::push_returns(S::temporaries(S::compare_values(S::goto_returns(S::sunk_tails(S::sunk_pairs(S::empty_loops(
            F::shared_arms(F::merge_conditions(F::guard_gotos(S::self_updates(S::switch_follow(t))))))))))))));
        Widths wd;   // (a parameter kept in a register: the bytes read through that register its own)
        for (const auto& qo : widths.fields) wd.add(qo);
        for (const auto& ri : regparam.items()) {
            const str& r_ = std::get<0>(ri);
            int64_t i_ = std::get<1>(ri);
            if (0 <= i_ && i_ < 26) {
                std::vector<std::tuple<str, int64_t>> add;
                for (const auto& qo : widths.fields) {
                    const str& q = std::get<0>(qo);
                    int64_t o = std::get<1>(qo);
                    if ((q == r_ || q == r_ + "*"_s) && !widths.fields.contains(std::make_tuple(r_ + "w"_s, o))) add.push_back(std::make_tuple(py::chr(97 + i_), o));
                }
                for (const auto& a : add) wd.add(a);
            }
        }
        t = structs(F::pointer_flow(local_pointers(register_variables(F::shapes(long_pairs(t)), regs, regparam))), wd);
        t = py::re::sub(R"re((?m)^[ \t]*(\w+) = \1;\n)re"_s, ""_s, t);   // (a register variable given itself: nothing)
        t = S::switch_ifs(S::lvalue_choices(F::register_values(long_ladders(long_pairs(t)))));   // (the longs again, as the passes left them)
        if (py::re::search(R"re(\bbp - \d+\b)re"_s, t)) {   // (a frame place the passes left as bp - N: the local)
            t = or_str(buffers(t), t);
            const str old = t;
            t = py::re::sub(R"re(\bbp - (\d+)\b)re"_s, [&](const Match& m) -> str {
                if (py::re::search(pf(R"re((?m)^[ \t]*char v%s\[)re", m.group(1)), old)) return pf("v%s", m.group(1));
                if (py::re::search(pf(R"re((?m)^[ \t]*\w[\w ]*\bv%s;)re", m.group(1)), old)) return pf("(char *)&v%s", m.group(1));
                return m.group(0);
            }, old);
        }
        t = frame_chars(F::fn_pointers(t));
        str t0_ = t;
        t = fill_frame(t0_, FRAME[0], prj.frame_order.has_value());
        FILLNAME.set_slice(0, 1, Lines{"_p%d"_s});   // (the filler named by its place: the frame's order lays it out - a variant)
        str tp_ = fill_frame(t0_, FRAME[0], prj.frame_order.has_value());
        FILLNAME.set_slice(0, 1, Lines{"_pf"_s});
        Lines variants;
        variants.append(t);
        {
            // [v for v in [tp_ if tp_ != t else None, store_increments(t), ..] + case_guards(t) if v]
            std::vector<std::optional<str>> vs;
            vs.push_back(tp_ != t ? std::optional<str>(tp_) : std::nullopt);
            vs.push_back(F::store_increments(t));
            vs.push_back(F::pre_steps(t));
            vs.push_back(F::pre_increments(t));
            vs.push_back(F::assign_in_tests(t));
            vs.push_back(F::and_returns(t));
            vs.push_back(F::inner_registers(t));
            vs.push_back(S::for_loops(t));
            vs.push_back(F::register_temps(t));
            vs.push_back(F::compound_assigns(t));
            vs.push_back(F::return_sources(t));
            vs.push_back(F::assign_in_args(t));
            vs.push_back(F::split_ternaries(t));
            vs.push_back(F::default_last(t));
            vs.push_back(F::push_tails(t));
            vs.push_back(F::high_words(t));
            vs.push_back(F::unmasked(t));
            vs.push_back(F::no_default(t));
            vs.push_back(F::unsigned_registers(t));
            vs.push_back(F::empty_cases_first(t));
            vs.push_back(register_slots(t));
            vs.push_back(F::register_copies(t));
            vs.push_back(F::lead_filler(t));
            vs.push_back(F::scaled_places(t));
            vs.push_back(F::shared_tails(t));
            vs.push_back(F::guard_returns(t));
            vs.push_back(F::default_after(t));
            vs.push_back(F::frame_more(t));
            vs.push_back(S::loop_tests(t));
            vs.push_back(F::switch_breaks(t));
            vs.push_back(F::register_reads(t));
            vs.push_back(F::compare_results(t));
            vs.push_back(F::or_loops(t));
            vs.push_back(S::loop_guards(t));
            vs.push_back(S::const_tests(t));
            vs.push_back(S::flat_returns(t));
            vs.push_back(S::return_cases_first(t));
            vs.push_back(registers_last(t));
            vs.push_back(F::guard_shared(t));
            vs.push_back(F::guard_flat(t));
            vs.push_back(_changed([](const str& x) { return S::common_tails(x); }, t));
            vs.push_back(_changed([](const str& x) { return S::byte_heads(x); }, t));
            vs.push_back(_changed([](const str& x) { return S::word_offsets(x); }, t));
            vs.push_back(S::chain_tails(t));
            vs.push_back(S::heads_variant(t));
            vs.push_back(S::shared_exit(t));
            vs.push_back(S::call_pairs(t));
            vs.push_back(S::label_sinks(t));
            vs.push_back(S::assign_loops(t));
            vs.push_back(S::char_widens(t));
            vs.push_back(_changed([](const str& x) { return S::tail_continues(x); }, t));
            for (const auto& v : vs)
                if (tidy(v)) variants.append(*v);
            for (const str& v : S::case_guards(t))
                if (!v.empty()) variants.append(v);
        }
        std::optional<str> both = F::pre_steps(t);
        if (tidy(both)) both = F::pre_increments(*both);
        else both = std::nullopt;
        if (tidy(both)) variants.append(*both);
        {
            Lines first4 = variants.slice(0, 4);   // (a value given twice: chained)
            for (const str& v : first4) {
                std::optional<str> c = F::chain_assigns(v);
                if (tidy(c)) variants.insert(variants.index(v) + 1, *c);
            }
        }
        for (const str& v : variants.copy())   // (a pointer register variable through which words are read: int *, the compiler decides)
            for (const str& w : F::pointer_widths(v)) variants.append(w);
        if (py::re::search(R"re(& 0xff\)|\b[a-z] [<>=!]=? (?:3[2-9]|[4-9]\d|1[01]\d|12[0-6])\b)re"_s, t))   // (a byte compared, or a char: a char param)
            variants.extend(F::char_param_variants(t));
        for (const str& v : variants.copy()) {   // (the names the frame's order asks for)
            std::optional<str> fn = prj.frame_order ? F::frame_names(v, *prj.frame_order) : std::nullopt;
            if (tidy(fn)) variants.insert(variants.index(v) + 1, *fn);
            std::optional<str> fl = (prj.frame_order && F::REGSLOTS[0]) ? F::frame_names(v, *prj.frame_order, true) : std::nullopt;   // (the register locals' slots last)
            if (tidy(fl) && fl != fn) variants.append(*fl);
        }
        const int64_t dsize = 0x10000;   // (the data and the BSS after it: any place a pointer is compared with)
        for (const str& v : variants.slice(0, 2)) {   // (a number that is a place in the data)
            for (const auto& nav : F::address_variants(v, dsize)) {
                const str& name = std::get<0>(nav);
                const str& av = std::get<1>(nav);
                T::ARRAYS.setdefault(name, "char"_s);
                variants.append(av);
                Lines pw = F::pointer_widths(av);
                if (!pw.empty()) variants.append(pw[0]);   // (and its pointer stepping words through it)
            }
        }
        std::vector<std::tuple<str, Lines>> loc;
        for (const str& v : variants) loc.push_back(localize(prj, F::word_constants(v), at, names));
        const size_t first3 = std::min<size_t>(3, loc.size());
        for (size_t i = 0; i < first3; ++i) {   // (a global compared: unsigned, a variant)
            std::tuple<str, Lines> sl = loc[i];
            if (std::get<1>(sl).empty() && py::re::search(R"re([<>=]=?\s*-?\d)re"_s, std::get<0>(sl)))
                for (const str& u : F::unsigned_globals(std::get<0>(sl))) loc.push_back(std::make_tuple(u, std::get<1>(sl)));
        }
        out.push_back(loc);
    }
    // (the bases' readings taken in turn - each base's first, then each one's second, .. - so that no base's
    //  many variants keep another's first readings from the compiler)
    py::list<std::tuple<str, Lines>> res;
    size_t longest = 0;
    for (const auto& g : out) longest = std::max(longest, g.size());
    for (size_t i = 0; i < longest; ++i)
        for (const auto& g : out)
            if (i < g.size()) res.append(g[i]);
    return res;
}

}  // namespace codebreaker::exact::draft::candidates
