// codebreaker/port/pre_ansi_p3.cpp -- typed C++ port of codebreaker.port.pre_ansi, PART 3: the Port methods
// anon_tags .. offset_views (Python lines 2144-3077). See pre_ansi.hpp.

#include <algorithm>
#include <functional>
#include <set>

#include "codebreaker/base/ctext.hpp"
#include "codebreaker/port/pre_ansi.hpp"

namespace codebreaker::port::pre_ansi {

using asm16::CTypes;
using asm16::Layout;
using asm16::Layouts;
using asm16::Member;
using base::ctext::sub_code;
using py::list;
using py::str;

namespace {

str S(const char* s) { return str(s); }
str blank(const str& t) { return base::ctext::blank(t); }
str esc(const str& s) { return py::re::escape(s); }
// re.escape(x).replace(r"\ ", r"\s+")
str esc_ws(const str& s) { return esc(s).replace(S("\\ "), S("\\s+")).replace(S(" "), S("\\s+")); }
// re.sub(r"\s+", " ", s)
str squeeze(const str& s) { return py::re::sub(S("\\s+"), S(" "), s); }
list<str> split_lines(const str& t) { return py::split(t, S("\n")); }
int64_t int_of(const str& s) { return py::int_parse(s, 10); }
str itos(int64_t v) { return py::to_str(v); }

// self.notes[why].append(where)
void note(Notes& notes, const char* why, const str& where) { notes.setdefault(S(why), list<str>()).append(where); }

// str.replace(old, new, 1)
str replace_first(const str& s, const str& a, const str& b) {
    int64_t i = s.find(a);
    if (i < 0) return s;
    return s.slice(0, i) + b + s.slice_from(i + a.size());
}

// a set that may hold None as well (the Python sets of `x or None` values)
struct MaybeSet {
    bool none = false;
    py::set<str> vals;
    void add(const std::optional<str>& v) {
        if (v) vals.add(*v);
        else none = true;
    }
    int64_t size() const { return vals.size() + (none ? 1 : 0); }
};

using DeclList = list<std::tuple<str, str>>;

// the members' signature of a layout: [(name, offset, size)] / [(offset, size)]
std::vector<std::tuple<str, int64_t, int64_t>> sig3(const Layout& lay) {
    std::vector<std::tuple<str, int64_t, int64_t>> out;
    for (const auto& kv : lay.members) out.emplace_back(kv.first, kv.second.off, kv.second.size);
    return out;
}
std::vector<std::pair<int64_t, int64_t>> sig2(const Layout& lay) {
    std::vector<std::pair<int64_t, int64_t>> out;
    for (const auto& kv : lay.members) out.emplace_back(kv.second.off, kv.second.size);
    return out;
}

// the string's last whitespace-separated word (s.split()[-1])
str last_word(const str& s) {
    list<str> w = py::split(s);
    return w[-1];
}

// pt.split("*")[-1].strip()
str last_star(const str& pt) {
    list<str> w = py::split(pt, S("*"));
    return w[-1].strip();
}

// "TYPE" -> the C type name the word of a member's type has as a stdint one (the Python dicts of offset_views)
std::optional<str> ct_small(const str& t) {
    static const std::vector<std::pair<const char*, const char*>> m = {
        {"char", "char"},          {"unsigned char", "unsigned char"}, {"int", "int16_t"},
        {"short", "int16_t"},      {"unsigned", "uint16_t"},           {"unsigned int", "uint16_t"},
        {"unsigned short", "uint16_t"}};
    for (const auto& kv : m) if (t == kv.first) return str(kv.second);
    return std::nullopt;
}
std::optional<str> ct_long(const str& t) {
    if (t == "long") return S("int32_t");
    if (t == "unsigned long") return S("uint32_t");
    return ct_small(t);
}

}  // namespace

// ---------------------------------------------------------------- anon_tags

void Port::anon_tags() {
    // struct { char kind, flags; } t; - an anonymous structure given by value (a parameter) that a header's
    // member also is: one type, by a tag the header's gets (two anonymous ones are two types)
    auto norm = [](const str& b) -> str {                       // (the members, one a declaration: int n, f4; is
        list<str> out_;                                         //  int n; int f4;)
        for (const str& d_ : py::split(blank(b), S(";"))) {
            str ds = d_.strip();
            auto r_ = ds.empty() ? std::nullopt : declarators(ds);
            if (r_) {
                for (const auto& nd : std::get<1>(*r_)) {
                    out_.append(squeeze(std::get<0>(*r_)).strip() + S(" ") + std::get<1>(nd).replace(S(" "), S("")));
                }
            } else if (!ds.empty()) {
                out_.append(py::re::sub(S("\\s+"), S(""), d_));
            }
        }
        return py::join(S(";"), out_);
    };
    py::dict<str, std::tuple<str, str>> named;
    for (const auto& ht : files.items()) {
        const str& h = std::get<0>(ht);
        const str& t = std::get<1>(ht);
        if (!h.endswith(S(".h"))) continue;
        list<py::re::Match> ms = py::re::finditer(S("\\bstruct\\s*\\{([^{}]*)\\}\\s*(\\w+)\\s*;"), t);
        for (int64_t i = ms.size() - 1; i >= 0; --i) {
            const py::re::Match& m = ms[i];
            str tag = std::get<0>(py::os::path::splitext(h)) + S("_") + m.group(2);
            named.setdefault(norm(m.group(1)), std::make_tuple(tag, h));
        }
    }
    for (const auto& ht : files.items()) {                      // (the header's: tagged)
        const str& h = std::get<0>(ht);
        const str& t = std::get<1>(ht);
        if (h.endswith(S(".h"))) {
            str nw = py::re::sub(S("\\bstruct\\s*\\{([^{}]*)\\}(\\s*)(\\w+)\\s*;"),
                                 [&](const py::re::Match& m) -> str {
                                     str k = norm(m.group(1));
                                     if (named.contains(k) && std::get<1>(named.at(k)) == h) {
                                         return S("struct ") + std::get<0>(named.at(k)) + S(" {") + m.group(1) +
                                                S("}") + m.group(2) + m.group(3) + S(";");
                                     }
                                     return m.group(0);
                                 },
                                 t);
            files[h] = nw;
        }
    }
    for (const auto& nt : files.items()) {                      // (a C file's parameter of the same: the tag)
        const str& n = std::get<0>(nt);
        str t = std::get<1>(nt);
        if (!n.endswith(S(".c"))) continue;
        auto one = [&](const py::re::Match& m) -> str {
            str k = norm(m.group(1));
            if (!named.contains(k)) return m.group(0);
            note(notes, "an anonymous structure given by value, a header's member's too: one type, by a tag",
                 n + S(" (") + std::get<0>(named.at(k)) + S(")"));
            return S("struct ") + std::get<0>(named.at(k)) + S(" ") + m.group(2) + S(";");
        };
        t = py::re::sub(S("\\bstruct\\s*\\{([^{}]*)\\}\\s*(\\w+)\\s*;"), one, t);   // (a parameter's, a local's)
        list<str> own;
        auto two = [&](const py::re::Match& m) -> str {         // (register struct { ... } *p; of a definition's
            str k = norm(m.group(2));                           //  parameters: a tag of the header's or its own)
            str tag = named.contains(k) ? std::get<0>(named.at(k))
                                        : std::get<0>(py::os::path::splitext(n)) + S("_") + m.group(4);
            if (!named.contains(k)) own.append(S("struct ") + tag + S(" {") + m.group(2) + S("};"));
            note(notes, "an anonymous structure of a parameter: a tag (a prototype names it)",
                 n + S(" (") + tag + S(")"));
            return m.group(1) + S("struct ") + tag + S(" ") + m.group(3) + m.group(4) + S(";");
        };
        t = py::re::sub(S("(?m)^(\\s*(?:register\\s+)?)struct\\s*\\{([^{}]*)\\}\\s*(\\*+)\\s*(\\w+)\\s*;"), two, t);
        if (!own.empty()) {
            list<str> ls = split_lines(t);
            int64_t at = -1;
            for (int64_t i = 0; i < ls.size(); ++i) if (ls[i].startswith(S("#include"))) at = std::max(at, i);
            at += 1;
            for (int64_t k = 0; k < own.size(); ++k) ls.insert(at + k, own[k]);
            t = py::join(S("\n"), ls);
        }
        files[n] = t;
    }
}

// ---------------------------------------------------------------- array_tails

void Port::array_tails() {
    // struct pnode { ...; struct pnode *kid[2]; int pa; ... } - kid[k] for k up to 2 (three children), pa never
    // named: the word after an array, of its elements' old size, at the place its next element would be, is
    // that element - the array one longer, the word gone
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    list<str> blanks;
    for (const str& t : files.values()) blanks.append(blank(t));
    str allcode = py::join(S("\n"), blanks);
    for (const str& n : py::sorted(files.keys())) {
        str t = files.at(n);
        list<py::re::Match> sms = py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{([^{}]*)\\}"), blank(t));
        for (const py::re::Match& sm : sms) {
            str tag = sm.group(1);
            const Layout* lay = lays.get(S("struct ") + tag);
            if (!lay) continue;
            str body = t.slice(sm.start(2), sm.end(2));
            std::vector<std::tuple<int64_t, int64_t, str>> decls;
            for (const py::re::Match& m_ : py::re::finditer(S("[^;{}]+;"), body)) {
                decls.emplace_back(m_.start(), m_.end(), m_.group(0));
            }
            for (size_t di = 0; di + 1 < decls.size(); ++di) {
                auto [a1, b1, d1] = decls[di];
                auto [a2, b2, d2] = decls[di + 1];
                auto r1 = declarators(blank(d1).strip().rstrip(S(";")), TYPEWORDS | STDINT);
                auto r2 = declarators(blank(d2).strip().rstrip(S(";")), TYPEWORDS | STDINT);
                if (!r1 || !r2 || std::get<1>(*r1).size() != 1 || std::get<1>(*r2).size() != 1) continue;
                str an = std::get<0>(std::get<1>(*r1)[0]), ad = std::get<1>(std::get<1>(*r1)[0]);
                str wn = std::get<0>(std::get<1>(*r2)[0]), wd = std::get<1>(std::get<1>(*r2)[0]);
                auto am = py::re::fullmatch(S("(\\**)\\s*") + esc(an) + S("\\s*\\[\\s*(\\d+)\\s*\\]"), ad.strip());
                if (!am || wd.strip() != wn || !lay->has(an) || !lay->has(wn)) continue;
                int64_t count = int_of(am->group(2));
                const Member& A = *lay->find(an);
                const Member& W = *lay->find(wn);
                int64_t esz = A.size / std::max<int64_t>(1, count);
                if (W.off != A.off + A.size || W.size != esz) continue;   // (not where, nor of the size, the next element is)
                if (py::re::search(S("(?:->|\\.)\\s*") + esc(wn) + S("\\b"), allcode) ||
                    py::re::search(S("(?m)^\\s*#\\s*define\\s+\\w+.*\\b") + esc(wn) + S("\\b"), allcode)) {
                    continue;                                   // (the word named somewhere: a member of its own)
                }
                if (!py::re::search(S("(?:->|\\.)\\s*") + esc(an) + S("\\s*\\[\\s*(?![0-") + itos(count - 1) +
                                        S("]\\s*\\])[^\\]]+\\]"),
                                    allcode)) {
                    continue;                                   // (the array only ever indexed within it)
                }
                str new_d1 = py::re::sub(S("\\[\\s*") + itos(count) + S("\\s*\\]"), S("[") + itos(count + 1) + S("]"), d1, 1);
                str body2 = body.slice(0, a1) + new_d1 + body.slice(b1, a2) +
                            S("\t/* (") + wn + S(": the word after it, its element ") + itos(count) + S(") */") +
                            body.slice_from(b2);
                t = t.slice(0, sm.start(2)) + body2 + t.slice_from(sm.end(2));
                files[n] = t;
                note(notes, "a word after an array that its next element was (indexed past it, never named): the "
                            "array one longer",
                     n + S(" (struct ") + tag + S(": ") + an + S("[") + itos(count) + S("], ") + wn + S(")"));
                break;
            }
        }
    }
}

// ---------------------------------------------------------------- link_words

void Port::link_words() {
    // #define NEXT(s) (*(struct isym **)(s)) - NEXT(s) = *pp: a structure's first word its chain's link (an
    // int then, the width of a pointer) - its first member that pointer now
    py::dict<str, str> macros;                                  // (macro: the structure its link points to)
    for (const str& t : files.values()) {
        for (const py::re::Match& m :
             py::re::finditer(S("(?m)^\\s*#\\s*define\\s+(\\w+)\\s*\\(\\s*(\\w+)\\s*\\)\\s*\\(?\\s*\\*\\s*\\(\\s*struct\\s+(\\w+)"
                                "\\s*\\*\\s*\\*\\s*\\)\\s*\\(?\\s*\\2\\s*\\)?\\s*\\)?\\s*$"),
                              t)) {
            macros[m.group(1)] = m.group(3);
        }
    }
    py::dict<str, py::set<str>> uses;                           // (structure: the link's structure)
    for (const auto& nt : files.items()) {
        const str& t = std::get<1>(nt);
        str code = blank(t);
        std::vector<std::pair<str, str>> found;
        for (const str& x : macros.keys()) {
            for (const py::re::Match& mm : py::re::finditer(S("\\b(") + esc(x) + S(")\\s*\\(\\s*(\\w+)\\s*\\)"), code)) {
                found.emplace_back(mm.group(2), x);
            }
        }
        for (const py::re::Match& mm :
             py::re::finditer(S("\\*\\s*\\(\\s*struct\\s+(\\w+)\\s*\\*\\s*\\*\\s*\\)\\s*(\\w+)\\b"), code)) {
            found.emplace_back(mm.group(2), mm.group(1));
        }
        for (const auto& vx : found) {
            const str& v = vx.first;
            const str& x = vx.second;
            py::set<str> tags = py::set<str>::from(
                py::re::findall(S("\\bstruct\\s+(\\w+)\\s*\\*\\s*") + esc(v) + S("\\s*[;,=)]"), code));
            if (tags.size() == 1) {
                str tag = *tags.begin();
                uses.setdefault(tag, py::set<str>()).add(macros.get(x, x));
            }
        }
    }
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    for (const auto& yxs : uses.items()) {                      // (struct isym4 { int s0; char kind, s3; int s4; }:
        const str& y = std::get<0>(yxs);                        //  an isym's first fields - its link alike)
        const py::set<str>& xs = std::get<1>(yxs);
        if (xs.size() != 1 || !lays.contains(S("struct ") + y)) continue;
        auto whole = sig3(lays.at(S("struct ") + y));
        for (const auto& kl : lays.map.items()) {
            const str& k = std::get<0>(kl);
            str z = last_word(k);
            auto part = sig3(std::get<1>(kl));
            if (k.startswith(S("struct ")) && z != y && !uses.contains(z) && !part.empty() && part.size() < whole.size() &&
                std::equal(part.begin(), part.end(), whole.begin())) {
                uses[z] = py::set<str>::from(xs);
            }
        }
    }
    for (const auto& yxs : uses.items()) {
        const str& y = std::get<0>(yxs);
        const py::set<str>& xs = std::get<1>(yxs);
        if (xs.size() != 1) continue;
        str x = *xs.begin();
        for (const str& n : py::sorted(files.keys())) {
            str t = files.at(n);
            auto m = py::re::search(S("\\bstruct\\s+") + esc(y) +
                                        S("\\s*\\{\\s*((?:unsigned\\s+|signed\\s+)?(?:int|short|unsigned|int16_t|"
                                          "uint16_t))\\s+(\\w+)\\s*;"),
                                    blank(t));
            if (!m) continue;
            t = t.slice(0, m->start(1)) + S("struct ") + x + S(" *") + m->group(2) + S(";") +
                S("\t/* (the chain's link: NEXT) */") + t.slice_from(m->end());
            files[n] = t;
            note(notes, "a structure's first word its chain's link (read as a pointer): a pointer",
                 n + S(" (struct ") + y + S(": ") + m->group(2) + S(")"));
        }
    }
}

// ---------------------------------------------------------------- fnptr_returns

void Port::fnptr_returns() {
    // f(t, s, fn) int (*fn)(); ... (*fn)() - and every call gives it a function of a pointer (char *f09581()):
    // int was a pointer's width on the old machine, not here - the parameter a function of that pointer
    py::dict<str, py::set<str>> rets_all;                       // (function: its return type, a pointer one)
    // ((function, index): (file, its declaration's span start, end, the name))
    std::vector<std::pair<std::tuple<str, int64_t>, std::tuple<str, int64_t, int64_t, str>>> defs;
    for (const auto& nt : files.items()) {
        const str& n = std::get<0>(nt);
        str code = blank(std::get<1>(nt));
        for (const py::re::Match& m :
             py::re::finditer(S("(?m)^(?:static\\s+)?((?:struct\\s+|unsigned\\s+)?\\w+)\\s*(\\*+)\\s*(\\w+)\\s*"
                                "\\(([\\w\\s,]*)\\)\\s*\\n((?:[^{;]*;\\s*)*)\\{"),
                              code)) {
            rets_all.setdefault(m.group(3), py::set<str>()).add(m.group(1) + S(" ") + m.group(2));
        }
        for (const py::re::Match& m : py::re::finditer(S("(?m)^(?:\\w+\\s+\\**\\s*|\\w+\\s*\\*+\\s*)?(\\w+)\\s*\\(([\\w\\s,]*)\\)\\s*\\n"
                                                         "((?:[^{;]*;\\s*)*)\\{"),
                                                       code)) {
            list<str> names;
            for (const str& w : py::split(m.group(2), S(","))) if (!w.strip().empty()) names.append(w.strip());
            for (const py::re::Match& d :
                 py::re::finditer(S("(?m)^[ \\t]*(?:register\\s+)?(?:int\\s+)?\\(\\s*\\*\\s*(\\w+)\\s*\\)\\s*\\(\\s*\\)\\s*;"),
                                  m.group(3))) {
                if (names.contains(d.group(1))) {
                    std::tuple<str, int64_t> key(m.group(1), names.index(d.group(1)));
                    std::tuple<str, int64_t, int64_t, str> val(n, m.start(3) + d.start(), m.start(3) + d.end(), d.group(1));
                    bool set_ = false;
                    for (auto& kv : defs) if (kv.first == key) { kv.second = val; set_ = true; }   // (dict: the last wins)
                    if (!set_) defs.emplace_back(key, val);
                }
            }
        }
    }
    py::dict<str, str> rets;
    for (const auto& fr : rets_all.items()) {
        if (std::get<1>(fr).size() == 1) rets[std::get<0>(fr)] = *std::get<1>(fr).begin();
    }
    std::stable_sort(defs.begin(), defs.end(), [](const auto& a, const auto& b) {
        return -std::get<1>(a.second) < -std::get<1>(b.second);
    });
    for (const auto& kv : defs) {
        const str& fn = std::get<0>(kv.first);
        int64_t i = std::get<1>(kv.first);
        const str& n = std::get<0>(kv.second);
        int64_t a = std::get<1>(kv.second), b = std::get<2>(kv.second);
        const str& name = std::get<3>(kv.second);
        MaybeSet given;
        for (const str& ct : files.values()) {
            str code = blank(ct);
            for (const py::re::Match& cm : py::re::finditer(S("(?<![\\w.>])") + esc(fn) + S("\\s*\\("), code)) {
                auto first = py::re::search(S("(?m)^[^\\n]*$"), code.slice(0, cm.start()));
                if (first->group(0).strip().empty() &&
                    py::re::match(S("[\\w\\s,]*\\)\\s*\\n"), code.slice_from(cm.end()))) {
                    continue;                                   // (its definition)
                }
                str ln = code.slice_from(code.rfind(S("\n"), 0, cm.start()) + 1);
                ln = ln.contains(S("\n")) ? ln.slice(0, ln.find(S("\n"))) : ln;
                auto sp = call_args(ln, fn, false);
                if (sp && sp->size() == 1 && ln.slice(std::get<0>((*sp)[0]), std::get<1>((*sp)[0])).strip().empty()) {
                    continue;                                   // (extern f(), a declaration)
                }
                if (!sp || sp->empty() || i >= sp->size()) {
                    given.add(std::nullopt);
                    continue;
                }
                str arg = ln.slice(std::get<0>((*sp)[i]), std::get<1>((*sp)[i])).strip().lstrip(S("&"));
                if (py::re::fullmatch(S("\\w+"), arg) && rets.contains(arg)) given.add(rets.at(arg));
                else given.add(std::nullopt);
            }
        }
        if (given.size() != 1 || given.none) continue;
        str rt = *given.vals.begin();
        str t = files.at(n);
        t = t.slice(0, a) +
            py::re::sub(S("(?:int\\s+)?\\(\\s*\\*\\s*") + esc(name), rt + S("(*") + name, t.slice(a, b), 1) + t.slice_from(b);
        files[n] = t;
        note(notes, "a function parameter given functions of pointers (int was a pointer's width): a function of that "
                    "pointer",
             n + S(" (") + name + S(" of ") + fn + S(": ") + rt + S(")"));
    }
}

// ---------------------------------------------------------------- cast_twins

namespace {
// one member of a structure: (name, type, declarator)
using Mem3 = std::tuple<str, str, str>;
struct MemRes {
    py::re::Match m;
    std::vector<Mem3> out;
};
bool is_ptr3(const Mem3& d) { return std::get<2>(d).contains(S("*")) || std::get<1>(d).contains(S("*")); }
}  // namespace

void Port::cast_twins() {
    // struct opt { char *pat; int o2; ... } here, struct flagdef { char *pat; intptr_t *var; ... } there, the
    // old bytes of both alike and the one cast to the other ((struct opt *)g_399a, a struct flagdef table): one
    // structure under two names - the copy takes the other's types as the port has them (its names kept)
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    py::dict<str, list<str>> where;
    for (const auto& nt : files.items()) {
        for (const py::re::Match& m : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{"), blank(std::get<1>(nt)))) {
            where.setdefault(m.group(1), list<str>()).append(std::get<0>(nt));
        }
    }
    auto members = [&](const str& n, const str& tag) -> std::optional<MemRes> {
        str t = files.at(n);
        auto m = py::re::search(S("\\bstruct\\s+") + esc(tag) + S("\\s*\\{([^{}]*)\\}"), blank(t));
        if (!m) return std::nullopt;
        MemRes res{*m, {}};
        for (const str& d : py::split(blank(t).slice(m->start(1), m->end(1)), S(";"))) {
            auto r = d.strip().empty() ? std::nullopt : declarators(d.strip(), TYPEWORDS | STDINT);
            if (r) {
                for (const auto& nd : std::get<1>(*r)) {
                    res.out.emplace_back(std::get<0>(nd), squeeze(std::get<0>(*r)).strip(), std::get<1>(nd));
                }
            }
        }
        return res;
    };
    py::dict<std::tuple<str, int64_t>, std::tuple<str, str>> params;   // ((function, index): (file, tag)) of struct T * ones
    py::dict<str, py::set<str>> glob;                                  // (variable: tag) of the files' outer struct ones
    for (const auto& nt : files.items()) {
        const str& n = std::get<0>(nt);
        str code = blank(std::get<1>(nt));
        for (const py::re::Match& m :
             py::re::finditer(S("(?m)^(?:\\w+\\s+\\**\\s*|\\w+\\s*\\*+\\s*)?(\\w+)\\s*\\(([\\w\\s,]*)\\)\\s*\\n((?:[^{;]*;\\s*)*)\\{"),
                              code)) {
            list<str> names;
            for (const str& w : py::split(m.group(2), S(","))) if (!w.strip().empty()) names.append(w.strip());
            for (const py::re::Match& d : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\*\\s*(\\w+)\\s*[;,]"), m.group(3))) {
                if (names.contains(d.group(2))) {
                    params[std::make_tuple(m.group(1), names.index(d.group(2)))] = std::make_tuple(n, d.group(1));
                }
            }
        }
        int64_t depth = 0;
        std::u32string outer;                                   // (outside every function)
        for (char32_t c : code.u32()) {
            depth += c == U'{';
            depth -= c == U'}';
            outer.push_back(depth == 0 || c == U'{' || c == U'}' ? c : U' ');
        }
        for (const py::re::Match& d : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s+\\**\\s*(\\w+)\\s*[\\[;=,]"), str(outer))) {
            glob.setdefault(d.group(2), py::set<str>()).add(d.group(1));
        }
    }
    py::dict<str, py::set<str>> fvars;                          // (a function: the function pointers given it)
    for (const str& t : files.values()) {
        str code = blank(t);
        py::set<str> fps = py::set<str>::from(py::re::findall(S("\\(\\s*\\*\\s*(\\w+)\\s*\\)\\s*\\(\\s*\\)"), code));
        for (const py::re::Match& m : py::re::finditer(S("\\(\\s*\\*\\s*(\\w+)\\s*\\)\\s*\\(\\s*\\)\\s*=\\s*(\\w+)\\s*[;,]"), code)) {
            fvars.setdefault(m.group(2), py::set<str>()).add(m.group(1));
        }
        for (const py::re::Match& m : py::re::finditer(S("(?<![\\w.>])(\\w+)\\s*=\\s*&?\\s*(\\w+)\\s*;"), code)) {
            if (fps.contains(m.group(1))) fvars.setdefault(m.group(2), py::set<str>()).add(m.group(1));
        }
    }

    // each call of fn - by its pointers too: the line, as a call of the name, and its arguments
    auto calls = [&](const str& ct, const str& fn) {
        std::vector<std::pair<str, std::optional<list<Span>>>> out;
        str code = blank(ct);
        for (const py::re::Match& cm : py::re::finditer(S("(?<![\\w.>])") + esc(fn) + S("\\s*\\("), code)) {
            str ln = code.slice_from(code.rfind(S("\n"), 0, cm.start()) + 1);
            ln = ln.contains(S("\n")) ? ln.slice(0, ln.find(S("\n"))) : ln;
            out.emplace_back(ln, call_args(ln, fn, false));
        }
        if (fvars.contains(fn)) {
            for (const str& v : fvars.at(fn)) {
                for (const py::re::Match& cm : py::re::finditer(S("\\(\\s*\\*\\s*") + esc(v) + S("\\s*\\)\\s*\\("), code)) {
                    str ln = code.slice_from(code.rfind(S("\n"), 0, cm.start()) + 1);
                    ln = ln.contains(S("\n")) ? ln.slice(0, ln.find(S("\n"))) : ln;
                    ln = py::re::sub(S("\\(\\s*\\*\\s*") + esc(v) + S("\\s*\\)\\s*\\("), v + S("("), ln);
                    out.emplace_back(ln, call_args(ln, v, false));
                }
            }
        }
        return out;
    };

    auto kinds = [&](const str& fname, const str& var) -> std::optional<str> {
        list<str> m = py::re::findall(S("\\bstruct\\s+(\\w+)\\s*\\*\\s*") + esc(var) + S("\\s*[;,=)]"), blank(files.at(fname)));
        if (py::set<str>::from(m).size() == 1) return m[0];
        if (glob.contains(var) && glob.at(var).size() == 1) return *glob.at(var).begin();
        return std::nullopt;
    };

    for (const str& n : py::sorted(files.keys())) {
        if (!n.endswith(S(".c"))) continue;
        str t = files.at(n);
        str code = blank(t);
        for (const py::re::Match& xm : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{"), code)) {
            str x = xm.group(1);
            if (!(where.contains(x) && where.at(x).size() == 1)) continue;
            py::set<str> ys;
            for (const auto& pk : params.items()) {                      // (a struct X * parameter given a Y, or
                const str& fn = std::get<0>(std::get<0>(pk));            //  a struct Y * parameter given an X)
                int64_t i = std::get<1>(std::get<0>(pk));
                const str& pf = std::get<0>(std::get<1>(pk));
                const str& tag = std::get<1>(std::get<1>(pk));
                for (const auto& cnt : files.items()) {
                    const str& cn = std::get<0>(cnt);
                    for (const auto& lnsp : calls(std::get<1>(cnt), fn)) {
                        const str& ln = lnsp.first;
                        const auto& sp = lnsp.second;
                        if (!sp || sp->empty() || i >= sp->size()) continue;
                        str arg = ln.slice(std::get<0>((*sp)[i]), std::get<1>((*sp)[i])).strip();
                        if (!py::re::fullmatch(S("&?\\w+"), arg)) continue;
                        auto at = kinds(cn, arg.lstrip(S("&")));
                        if (tag == x && at && *at != x && pf == n) ys.add(*at);
                        else if (at && *at == x && tag != x && cn == n) ys.add(tag);
                    }
                }
            }
            // (one the other's first words)
            auto prefix = [](const std::vector<std::pair<int64_t, int64_t>>& a_, const std::vector<std::pair<int64_t, int64_t>>& b_) {
                if (a_.empty() || b_.empty()) return false;
                size_t k = std::min(a_.size(), b_.size());
                return std::equal(a_.begin(), a_.begin() + k, b_.begin());
            };
            py::set<str> ys2;
            for (const str& y : ys) {
                if (lays.contains(S("struct ") + y) && lays.contains(S("struct ") + x) &&
                    prefix(sig2(lays.at(S("struct ") + y)), sig2(lays.at(S("struct ") + x))) &&
                    (where.contains(y) ? where.at(y).size() : 0) >= 1) {
                    ys2.add(y);
                }
            }
            ys = ys2;
            if (ys.size() != 1) continue;
            str y = *ys.begin();
            auto mx = members(n, x);
            std::optional<MemRes> my;
            if (where.contains(y)) {
                list<str> ws = where.at(y);
                std::vector<str> v(ws.vec().begin(), ws.vec().end());
                std::stable_sort(v.begin(), v.end(), [](const str& a, const str& b) {
                    return (!a.endswith(S(".h"))) < (!b.endswith(S(".h")));
                });
                my = members(v[0], y);
            }
            if (!mx || !my) continue;
            size_t nz = std::min(mx->out.size(), my->out.size());
            bool alike = true;                                  // (alike already; all() of nothing is true)
            for (size_t k = 0; k < nz; ++k) {
                const Mem3& a = mx->out[k];
                const Mem3& b = my->out[k];
                if (!(std::get<1>(a) == std::get<1>(b) &&
                      std::get<2>(a).replace(std::get<0>(a), S("@")) == std::get<2>(b).replace(std::get<0>(b), S("@")))) {
                    alike = false;
                    break;
                }
            }
            if (alike) continue;
            bool anydiff = false;
            for (size_t k = 0; k < nz; ++k) if (is_ptr3(mx->out[k]) != is_ptr3(my->out[k])) anydiff = true;
            if (!anydiff) continue;                             // (no word a pointer in one, a number in the other)
            auto local_ok = [&](const Mem3& d_) {
                return !(py::re::match(S("(?:struct|union)\\s+(\\w+)$"), std::get<1>(d_)) && !std::get<2>(d_).contains(S("*")) &&
                         !([&]() {
                             str w = last_word(std::get<1>(d_));
                             if (!where.contains(w)) return false;
                             for (const str& h : where.at(w)) if (h.endswith(S(".h"))) return true;
                             return false;
                         })());
            };
            bool allok = true;
            for (size_t k = 0; k < nz; ++k) if (!(local_ok(mx->out[k]) && local_ok(my->out[k]))) allok = false;
            if (!allok) continue;                               // (a whole structure of one file's own: not moved)

            // (each word: the pointer, of whichever has it)
            auto merged = [&](const std::vector<Mem3>& mine_, const std::vector<Mem3>& other, const str& me_tag,
                              const str& other_tag) {
                list<str> ls;
                size_t k2 = std::min(mine_.size(), other.size());
                for (size_t k = 0; k < k2; ++k) {
                    const Mem3& a = mine_[k];
                    const Mem3& o = other[k];
                    if (is_ptr3(o) && !is_ptr3(a)) {
                        ls.append(S("    ") +
                                  std::get<1>(o).replace(S("struct ") + other_tag, S("struct ") + me_tag) + S(" ") +
                                  py::re::sub(S("\\b") + esc(std::get<0>(o)) + S("\\b"), std::get<0>(a), std::get<2>(o)) + S(";"));
                    } else {
                        ls.append(S("    ") + std::get<1>(a) + S(" ") + std::get<2>(a) + S(";"));
                    }
                }
                for (size_t k = other.size(); k < mine_.size(); ++k) {
                    ls.append(S("    ") + std::get<1>(mine_[k]) + S(" ") + std::get<2>(mine_[k]) + S(";"));
                }
                return py::join(S("\n"), ls);
            };
            str bx = merged(mx->out, my->out, x, y);
            bool b_ptr_only = false, a_ptr_only = false;
            for (size_t k = 0; k < nz; ++k) {
                if (is_ptr3(my->out[k]) && !is_ptr3(mx->out[k])) b_ptr_only = true;
                if (is_ptr3(mx->out[k]) && !is_ptr3(my->out[k])) a_ptr_only = true;
            }
            if (b_ptr_only) {
                const py::re::Match& m = mx->m;
                t = t.slice(0, m.start(1)) + S("\t/* (as struct ") + y + S(" is: one structure, two names) */\n") + bx +
                    S("\n") + t.slice_from(m.end(1));
                code = blank(t);
                files[n] = t;
                note(notes, "a structure that is another's, under its own name (cast to it, the old bytes alike): its "
                            "members of the other's types",
                     n + S(" (struct ") + x + S(" as struct ") + y + S(")"));
            }
            if (a_ptr_only) {
                for (const str& w : where.at(y)) {
                    auto my_ = members(w, y);
                    if (!my_) continue;
                    str tw = files.at(w);
                    tw = tw.slice(0, my_->m.start(1)) + S("\t/* (as struct ") + x + S(" is: one structure, two names) */\n") +
                         merged(my_->out, mx->out, y, x) + S("\n") + tw.slice_from(my_->m.end(1));
                    files[w] = tw;
                    note(notes, "a structure that is another's, under its own name (cast to it, the old bytes alike): its "
                                "members of the other's types",
                         w + S(" (struct ") + y + S(" as struct ") + x + S(")"));
                }
            }
        }
    }
}

// ---------------------------------------------------------------- twin_structs

void Port::twin_structs() {
    // struct macro { int f0, f2, f4; char *body; } in a file of its own - another structure's bytes (struct mac:
    // its next, name, hash...) with ints where that one has pointers: its members of the other's types, its
    // names kept
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    py::dict<str, str> hdr;
    for (const auto& ht : originals.items()) {
        if (std::get<0>(ht).endswith(S(".h"))) {
            for (const py::re::Match& m : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{"), blank(std::get<1>(ht)))) {
                hdr.setdefault(S("struct ") + m.group(1), std::get<0>(ht));
            }
        }
    }
    for (const auto& nt : files.items()) {
        const str& n = std::get<0>(nt);
        str t = std::get<1>(nt);
        if (!n.endswith(S(".c"))) continue;
        for (const py::re::Match& m : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{([^{}]*)\\}"), blank(t))) {
            str x = S("struct ") + m.group(1);
            const Layout* lx = lays.get(x);
            if (!lx || hdr.contains(x)) continue;
            list<str> twins;
            auto sx = sig2(*lx);
            for (const auto& yl : lays.map.items()) {
                const str& y = std::get<0>(yl);
                auto sy = sig2(std::get<1>(yl));
                if (y != x && hdr.contains(y) && sy.size() >= 3 && sy == sx &&
                    py::re::search(S("#\\s*include\\s*\"") + esc(hdr.at(y)) + S("\""), t)) {
                    twins.append(y);
                }
            }
            if (twins.size() != 1) continue;
            str y_tag = last_word(twins[0]);                    // (and the file takes one for the other: a function it
            bool evid = false;                                  //  declares giving an X that gives a Y, or a cast of a Y)
            for (const py::re::Match& fm :
                 py::re::finditer(S("\\bstruct\\s+") + esc(m.group(1)) + S("\\s*\\*\\s*(\\w+)\\s*\\(\\s*\\)"), blank(t))) {
                for (const auto& n2t2 : originals.items()) {
                    if (std::get<0>(n2t2) != n &&
                        py::re::search(S("(?m)^\\s*(?:static\\s+)?struct\\s+") + esc(y_tag) + S("\\s*\\*\\s*\\n?\\s*") +
                                           esc(fm.group(1)) + S("\\s*\\("),
                                       blank(std::get<1>(n2t2)))) {
                        evid = true;
                    }
                }
            }
            if (!evid) continue;
            const Layout& ly = lays.at(twins[0]);
            bool any = false;                                   // (no int where the other has a pointer: as it is)
            size_t nz = std::min(lx->members.size(), ly.members.size());
            for (size_t k = 0; k < nz; ++k) if (!lx->members[k].second.isptr && ly.members[k].second.isptr) any = true;
            if (!any) continue;
            list<str> lines;
            for (size_t k = 0; k < nz; ++k) {
                const auto& kx = lx->members[k];
                const auto& ky = ly.members[k];
                lines.append(S("    ") + ky.second.type + S(" ") +
                             py::re::sub(S("\\b") + esc(ky.first) + S("\\b"), kx.first, ky.second.decl) + S(";"));
            }
            str body = py::join(S("\n"), lines);
            t = t.slice(0, m.start(2)) + S("\t/* (as ") + twins[0] + S(" is: its bytes) */\n") + body + S("\n") +
                t.slice_from(m.end(2));
            files[n] = t;
            note(notes, "a file's own structure that is another's bytes, ints where it has pointers: its members of the "
                        "other's types",
                 n + S(" (") + x + S(" as ") + twins[0] + S(")"));
            break;
        }
    }
}

// ---------------------------------------------------------------- struct_views

void Port::struct_views() {
    // int top = 20; char *v[20]; - globals one after another that are a structure's members in order, the
    // first's place taken (and used as the structure's): one variable of the structure, their names its
    // members' (the old linker kept them in that order; today's need not)
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    struct ShapeMem {
        str k, type, decl;
    };
    std::vector<std::pair<str, std::vector<ShapeMem>>> shapes;
    for (const auto& sl : lays.map.items()) {
        const str& st_ = std::get<0>(sl);
        const Layout& lay = std::get<1>(sl);
        bool arr = false;
        for (const auto& kv : lay.members) if (kv.second.decl.contains(S("["))) arr = true;
        if (st_.startswith(S("struct ")) && lay.members.size() >= 2 && arr) {
            std::vector<ShapeMem> mem;
            for (const auto& kv : lay.members) {
                mem.push_back({kv.first, squeeze(kv.second.type).strip(),
                               py::re::sub(S("\\b") + esc(kv.first) + S("\\b"), S("@"), kv.second.decl).replace(S(" "), S(""))});
            }
            shapes.emplace_back(st_, mem);
        }
    }
    if (shapes.empty()) return;
    list<str> blanks;
    for (const str& t : files.values()) blanks.append(blank(t));
    str alltext = py::join(S("\n"), blanks);
    for (const str& n : py::sorted(files.keys())) {
        if (!n.endswith(S(".c"))) continue;
        str t = files.at(n);
        auto gl = _globals(t);
        list<str> lines = split_lines(t);
        std::set<int64_t> done;
        for (int64_t i = 0; i < gl.size(); ++i) {
            if (done.count(i) || !py::re::search(S("&\\s*") + esc(std::get<1>(gl[i])) + S("\\b"), alltext)) continue;
            for (const auto& sm : shapes) {
                const str& st_ = sm.first;
                const auto& mem = sm.second;
                std::vector<std::decay_t<decltype(gl[0])>> run;
                for (int64_t k = i; k < std::min<int64_t>(gl.size(), i + (int64_t)mem.size()); ++k) run.push_back(gl[k]);
                if (run.size() != mem.size()) continue;
                bool consecutive = true;
                for (size_t k = 1; k < run.size(); ++k) {
                    if (std::get<0>(run[k]) - std::get<0>(run[k - 1]) != 1) consecutive = false;
                }
                if (!consecutive) continue;
                bool differs = false;
                for (size_t k = 0; k < run.size(); ++k) {
                    if (!(std::get<2>(run[k]) == mem[k].type && std::get<3>(run[k]) == mem[k].decl)) differs = true;
                }
                if (differs) continue;
                str var = std::get<1>(run[0]) + S("_s");
                list<str> inits, names;
                for (const auto& g_ : run) {
                    const auto& init = std::get<4>(g_);
                    inits.append(init ? *init : (std::get<3>(g_).contains(S("[")) ? S("{ 0 }") : S("0")));
                    names.append(std::get<1>(g_));
                }
                lines[std::get<0>(run[0])] = st_ + S(" ") + var + S(" = { ") + py::join(S(", "), inits) + S(" };\t/* (") +
                                             py::join(S(", "), names) + S(": one place, as the old linker kept them) */");
                for (size_t k = 1; k < run.size(); ++k) lines[std::get<0>(run[k])] = S("");
                for (size_t k = 0; k < run.size(); ++k) {
                    aliases[std::get<1>(run[k])] = std::make_tuple(S("(") + var + S(".") + mem[k].k + S(")"), st_ + S(" ") + var + S(";"));
                }
                for (const str& g : names) view_vars.add(g);
                for (const auto& ht : files.items()) {
                    if (std::get<0>(ht).endswith(S(".h")) && py::re::search(S("\\b") + esc(st_) + S("\\s*\\{"), blank(std::get<1>(ht)))) {
                        view_headers.add(std::get<0>(ht));
                    }
                }
                note(notes, "globals one after another that are a structure's members, the first's place taken: one "
                            "variable of it",
                     n + S(" (") + st_ + S(": ") + py::join(S(", "), names) + S(")"));
                for (int64_t k = i; k < i + (int64_t)run.size(); ++k) done.insert(k);
                break;
            }
        }
        files[n] = py::join(S("\n"), lines);
    }
}

// ---------------------------------------------------------------- word_columns

str Port::_widen(const py::re::Match& m, const str& v) {
    // int a, v = 1, b;  ->  intptr_t v = 1; int a, b;
    str pre = m.group(1), before = m.group(2), init = m.group(3), end = m.group(4);
    str rest = before.strip().rstrip(S(",")).strip();
    str own = pre + S("intptr_t ") + v + init + S(";");
    if (end == ";" && rest.empty()) return own;
    str tail = !rest.empty() ? (rest + (end == "," ? S(", ") : S(""))) : S("");
    return own + S(" ") + (!pre.strip().empty() ? pre.strip() + S(" ") : S("")) + S("int ") + tail +
           (end == "," ? S("") : S(";"));
}

void Port::word_columns() {
    // a table of structures with an int * member given (int *)&p - a word's place that held a pointer in
    // some rows, a number in others: the member intptr_t *, the ints placed in it intptr_t
    Layouts lays = CTypes::_layouts(list<str>(originals.values()), 2, 4);
    py::set<str> ptrs, ints;
    for (const str& t : files.values()) {
        for (const auto& g : _globals(t)) {
            const str& nm = std::get<1>(g);
            const str& base = std::get<2>(g);
            const str& shape = std::get<3>(g);
            if (shape.contains(S("*")) || base.startswith(S("struct")) || base.startswith(S("union"))) {
                ptrs.add(nm);
            } else if ((base == "int" || base == "unsigned" || base == "short" || base == "unsigned int") && shape == "@") {
                ints.add(nm);
            }
        }
    }
    const str table_pat = S("\\bstruct\\s+(\\w+)\\s+\\w+\\s*\\[[^\\]]*\\]\\s*=\\s*\\{");
    const str cast_pat = S("\\(\\s*(?:int|unsigned|short)\\s*\\*\\s*\\)\\s*&\\s*(\\w+)");
    py::dict<str, py::set<int64_t>> cols;
    for (const auto& nt : files.items()) {
        str code = blank(std::get<1>(nt));
        for (const py::re::Match& m : py::re::finditer(table_pat, code)) {
            const Layout* lay = lays.get(S("struct ") + m.group(1));
            if (!lay) continue;
            const auto& mem = lay->members;
            int64_t d = 1, k = m.end();
            while (k < code.size() && d) {
                char32_t c = code.at_(k);
                d += c == U'{' ? 1 : c == U'}' ? -1 : 0;
                k += 1;
            }
            for (const py::re::Match& row : py::re::finditer(S("\\{([^{}]*)\\}"), code.slice(m.end(), k - 1))) {
                list<str> els;
                for (const str& e : split_top(row.group(1))) els.append(e.strip());
                for (int64_t c_ = 0; c_ < els.size(); ++c_) {
                    auto x = py::re::fullmatch(cast_pat, els[c_]);
                    if (x && ptrs.contains(x->group(1)) && c_ < (int64_t)mem.size() &&
                        py::re::fullmatch(S("(?:int|unsigned|short|unsigned int)"), mem[c_].second.type.strip()) &&
                        mem[c_].second.decl.replace(S(" "), S("")) == S("*") + mem[c_].first) {
                        cols.setdefault(m.group(1), py::set<int64_t>()).add(c_);
                    }
                }
            }
        }
    }
    if (cols.empty()) return;
    py::set<str> widen;
    for (const str& n : files.keys()) {
        str t = files.at(n);
        str code = blank(t);
        std::vector<std::tuple<int64_t, int64_t, str>> edits;
        for (const py::re::Match& m : py::re::finditer(table_pat, code)) {
            if (!cols.contains(m.group(1))) continue;
            int64_t d = 1, k = m.end();
            while (k < code.size() && d) {
                char32_t c = code.at_(k);
                d += c == U'{' ? 1 : c == U'}' ? -1 : 0;
                k += 1;
            }
            for (const py::re::Match& row : py::re::finditer(S("\\{([^{}]*)\\}"), code.slice(m.end(), k - 1))) {
                int64_t base_at = m.end() + row.start(1);
                int64_t pos = 0;
                int64_t c_ = 0;
                for (const str& e : split_top(row.group(1))) {
                    int64_t at = base_at + pos + (e.size() - e.lstrip().size());
                    pos += e.size() + 1;
                    str es = e.strip();
                    int64_t cc = c_++;
                    if (!cols.at(m.group(1)).contains(cc)) continue;
                    auto x = py::re::fullmatch(cast_pat, es);
                    auto y = py::re::fullmatch(S("&\\s*(\\w+)"), es);
                    auto z = py::re::match(S("\\(\\s*(?:int|unsigned|short)\\s*\\*\\s*\\)"), es);
                    if (x || (y && view_vars.contains(y->group(1)))) {
                        edits.emplace_back(at, at + es.size(), S("(intptr_t *)&") + (x ? x : y)->group(1));
                    } else if (z) {                             // ((int *)table: another thing's place - so cast)
                        edits.emplace_back(at, at + z->end(), S("(intptr_t *)"));
                    } else if (y && ints.contains(y->group(1))) {
                        widen.add(y->group(1));
                    }
                }
            }
        }
        for (int64_t i = (int64_t)edits.size() - 1; i >= 0; --i) {
            auto [a, b, nw] = edits[i];
            t = t.slice(0, a) + nw + t.slice_from(b);
        }
        files[n] = t;
    }
    for (const auto& sc : cols.items()) {
        const str& st_ = std::get<0>(sc);
        std::vector<int64_t> cs(std::get<1>(sc).begin(), std::get<1>(sc).end());
        const Layout& lay = lays.at(S("struct ") + st_);
        std::vector<str> mem;
        for (const auto& kv : lay.members) mem.push_back(kv.first);
        for (const auto& nt : files.items()) {
            const str& n = std::get<0>(nt);
            const str& t = std::get<1>(nt);
            auto sm = py::re::search(S("\\bstruct\\s+") + esc(st_) + S("\\s*\\{([^{}]*)\\}"), t);
            if (!sm) continue;
            str body = sm->group(1);
            for (int64_t c_ : cs) {
                body = py::re::sub(S("(?m)^([ \\t]*)(?:int|unsigned|short|unsigned int)\\s*\\*\\s*") + esc(mem[c_]) + S("\\s*;"),
                                   S("\\1intptr_t *") + mem[c_] + S(";\\t/* (a word's place - a pointer's in some rows) */"), body);
            }
            files[n] = t.slice(0, sm->start(1)) + body + t.slice_from(sm->end(1));
            std::sort(cs.begin(), cs.end());
            list<str> names;
            for (int64_t c_ : cs) names.append(mem[c_]);
            note(notes, "a table's int * column given pointers' places too (a word was either): intptr_t *, the ints "
                        "placed in it intptr_t",
                 n + S(" (struct ") + st_ + S(": ") + py::join(S(", "), names) + S(")"));
        }
    }
    for (const auto& nt : files.items()) {                      // (the ints: intptr_t, wherever declared)
        str t = std::get<1>(nt);
        for (const str& v : widen) {
            t = py::re::sub(S("(?m)^([ \\t]*(?:extern\\s+|static\\s+)?)(?:int|unsigned|short|unsigned int)\\s+([^;(){}]*)\\b") +
                                esc(v) + S("\\b(\\s*(?:=[^,;]*)?)\\s*([,;])"),
                            [&](const py::re::Match& m) { return _widen(m, v); }, t);
        }
        files[std::get<0>(nt)] = t;
    }
    if (!widen.empty()) {
        list<str> sw = py::sorted(widen);
        note(notes, "an int whose place a column of pointers' places is given: intptr_t", py::join(S(", "), sw));
    }
}

// ---------------------------------------------------------------- bss_ends and the small readers

str Port::bss_ends(const str& t0, const str& fname) {
    // (char *)&g - the place of the variable the old linker put after a char array: the array's end, as the
    // program means it (its bytes as they were)
    if (bss.empty()) return t0;
    str t = t0;
    std::vector<str> names;
    for (const str& k : bss.keys()) names.push_back(k);
    py::set<str> chars;
    for (const str& o : originals.values()) {
        for (const py::re::Match& m : py::re::finditer(S("\\bchar\\s+(\\w+)\\s*\\["), blank(o))) chars.add(m.group(1));
    }
    py::set<str> used;
    auto one = [&](const py::re::Match& m) -> str {
        str v = m.group(1);
        if (!bss.contains(v) || chars.contains(v)) return m.group(0);
        size_t k = std::find(names.begin(), names.end(), v) - names.begin();
        if (k == 0 || !chars.contains(names[k - 1])) return m.group(0);
        str prev = names[k - 1];
        note(notes, "the place of the variable after a char array (the old linker's order): the array's end",
             fname + S(" (&") + v + S(": ") + prev + S(" + ") + itos(bss.at(prev)) + S(")"));
        used.add(prev);
        return S("(") + prev + S(" + ") + itos(bss.at(prev)) + S(")");
    };
    t = sub_code(t, py::re::compile(S("\\(\\s*char\\s*\\*\\s*\\)\\s*&\\s*(\\w+)\\b(?!\\s*(?:\\[|->|\\.))")), one);
    for (const str& prev : py::sorted(used)) {                  // (the array declared where its end is taken)
        if (!py::re::search(S("\\b") + esc(prev) + S("\\s*\\["), blank(t))) {
            t = replace_first(t, S("#include \"port.h\""), S("#include \"port.h\"\nextern char ") + prev + S("[];"));
        }
    }
    return t;
}

const py::set<str>& Port::copied_members() {
    // the members the program copies from one to another as they are: x.val.i = y->val.i
    if (!_copied) {
        _copied = py::set<str>();
        for (const str& t : originals.values()) {
            for (const py::re::Match& m :
                 py::re::finditer(S("\\.\\s*(\\w+)\\s*=\\s*[\\w.\\->()\\[\\] ]*?\\.\\s*(\\w+)\\s*;"), blank(t))) {
                if (m.group(1) == m.group(2)) _copied->add(m.group(1));
            }
        }
    }
    return *_copied;
}

const py::set<str>& Port::ptr2_names() {
    // the members and variables the program declares char ** (a block's link, its first word)
    if (!_ptr2) {
        _ptr2 = py::set<str>();
        for (const str& t : originals.values()) {
            for (const py::re::Match& m : py::re::finditer(S("\\bchar\\s*\\*\\s*\\*\\s*(\\w+)\\s*[;,=)\\[]"), blank(t))) {
                _ptr2->add(m.group(1));
            }
        }
    }
    return *_ptr2;
}

std::optional<str> Port::pointee_of(const str& text, const str& var) {
    // the structure a variable of the text points to (struct S *v, S v[]) - or None
    str c = blank(text);
    auto m = py::re::search(S("\\b((?:struct|union)\\s+\\w+)\\s*\\*\\s*(?:\\w+\\s*,\\s*\\*?\\s*)*") + esc(var) +
                               S("\\b\\s*[;,)=\\[]"),
                           c);
    if (!m) m = py::re::search(S("\\b((?:struct|union)\\s+\\w+)\\s+") + esc(var) + S("\\s*\\["), c);
    if (!m) return std::nullopt;
    return squeeze(m->group(1));
}

// ---------------------------------------------------------------- pass_through_params

void Port::pass_through_params() {
    // a pointer parameter a function only gives on (never through it, never a member of it) that every
    // caller gives a pointer of another structure: it is that one's (the recovered source named some pointer -
    // the bytes alike, any pointer two bytes)
    if (_pass_done) return;
    _pass_done = true;
    py::dict<str, str> texts;
    for (const auto& nt : files.items()) if (std::get<0>(nt).endswith(S(".c"))) texts[std::get<0>(nt)] = std::get<1>(nt);
    for (const FnP& fn : fns.values()) {
        str body = fn->body;
        if (body.empty()) {
            str src = texts.get(fn->file ? *fn->file : S(""), S(""));
            std::optional<std::tuple<str, int64_t, int64_t, int64_t>> d_;
            if (!src.empty()) {
                for (const auto& d : function_map(src)) {
                    if (std::get<0>(d) == fn->name) { d_ = d; break; }
                }
            }
            if (d_) {
                list<str> bl = split_lines(blank(src));
                body = py::join(S("\n"), bl.slice(std::get<2>(*d_), std::get<3>(*d_) + 1));
            } else {
                body = S("");
            }
        }
        for (int64_t k = 0; k < fn->params.size(); ++k) {
            const str& pn = std::get<0>(fn->params[k]);
            const str& pt = std::get<1>(fn->params[k]);
            auto m = py::re::fullmatch(S("\\s*(?:register\\s+)?((?:struct|union)\\s+\\w+)\\s*\\*\\s*") + esc(pn) + S("\\s*"), pt);
            if (!m || body.empty()) continue;
            list<py::re::Match> uses = py::re::finditer(S("\\b") + esc(pn) + S("\\b"), body);
            bool ok = !uses.empty();
            for (const py::re::Match& u : uses) {
                if (!(py::re::search(S("[(,]\\s*(?:\\([^()]*\\)\\s*)?$"), body.slice(0, u.start())) &&
                      py::re::match(S("\\s*[,)]"), body.slice_from(u.end())))) {
                    ok = false;
                    break;
                }
            }
            if (!ok) continue;                                  // (used otherwise than given on: its own type)
            MaybeSet given;
            for (const auto& nt : texts.items()) {
                const str& t = std::get<1>(nt);
                str code = blank(t);
                for (const py::re::Match& c : py::re::finditer(S("\\b") + esc(fn->name) + S("\\s*\\("), code)) {
                    if (py::re::match(S("[\\w\\s*]*\\b") + esc(fn->name) + S("\\s*\\([^;]*\\)\\s*$"),
                                      code.slice(code.rfind(S("\n"), 0, c.start()) + 1, code.find(S("\n"), c.start())))) {
                        continue;                               // (its definition)
                    }
                    auto spo = call_args(code.slice(c.start(), c.start() + 400), fn->name, false);
                    list<Span> sp = spo ? *spo : list<Span>();
                    if (k >= sp.size()) continue;
                    str a_ = code.slice(c.start() + std::get<0>(sp[k]), c.start() + std::get<1>(sp[k])).strip();
                    if (a_.empty() || py::re::fullmatch(S("0|\\(\\s*\\w+\\s*\\*\\s*\\)\\s*0"), a_)) {
                        continue;                               // (none given; a declaration's f())
                    }
                    auto x = py::re::fullmatch(S("&?\\s*(\\w+)(?:\\s*\\[[^\\]]*\\])?"), a_);
                    given.add(x ? pointee_of(t, x->group(1)) : std::nullopt);
                }
            }
            if (given.size() == 1 && !given.none && *given.vals.begin() != m->group(1)) {
                str y = *given.vals.begin();
                param_types[std::make_tuple(fn->name, k)] = y + S(" *") + pn;
                note(mended, "a pointer parameter only given on, its callers all giving another structure's: that "
                             "one's (any pointer two bytes then)",
                     fn->name + S(" (") + pn + S(": ") + y + S(")"));
            }
        }
    }
}

// ---------------------------------------------------------------- offset_views

namespace {
// one member of the other structure's, with the places it lies at: (offset, size, name, the member)
struct YM {
    int64_t off, size;
    str name;
    Member v;
};
}  // namespace

str Port::offset_views(str t, const str& fname) {
    // struct f071f5_p { long v; int x; unsigned char c; } *p, given (struct f071f5_p *)s->type - a file's own
    // view of another structure (a struct type) by its old places: each member the other's at its place; a long
    // over two of its members, given as an argument, the two (the old callee read them off the stack so)
    bool views_cast_rule = rule(S("views_cast"));               // (one thing cast to both views: the surest evidence)

    auto build = [&](bool views_cast) -> py::dict<str, ViewEntry> {
        py::dict<str, ViewEntry> found;
        // (the old layouts of the port's texts: tags it gave anonymous ones known, headers first)
        std::vector<std::pair<str, str>> sorted_files;
        for (const auto& nt : files.items()) sorted_files.emplace_back(std::get<0>(nt), std::get<1>(nt));
        std::stable_sort(sorted_files.begin(), sorted_files.end(), [](const auto& a, const auto& b) {
            return (!a.first.endswith(S(".h"))) < (!b.first.endswith(S(".h")));
        });
        list<str> sorted_texts;
        for (const auto& nt : sorted_files) sorted_texts.append(nt.second);
        Layouts old = CTypes::_layouts(sorted_texts, 2, 4);
        py::dict<str, py::set<str>> ptr_mem;                    // (a member's name: the structures it points to)
        for (const auto& sl : old.map.items()) {
            for (const auto& kv : std::get<1>(sl).members) {
                if (kv.second.isptr && (kv.second.type.startswith(S("struct")) || kv.second.type.startswith(S("union")))) {
                    ptr_mem.setdefault(kv.first, py::set<str>()).add(squeeze(kv.second.type));
                }
            }
        }
        py::dict<str, str> local;
        for (const auto& nt : files.items()) {
            if (std::get<0>(nt).endswith(S(".c"))) {
                for (const py::re::Match& m : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{"), blank(std::get<1>(nt)))) {
                    local.setdefault(S("struct ") + m.group(1), std::get<0>(nt));
                }
            }
        }
        py::dict<str, str> exp_;                                // (#define L(s) ((struct lsym *)(s)): its uses the casts)
        for (const auto& nt : files.items()) {
            str b_ = blank(std::get<1>(nt));
            list<py::re::Match> defs_ = py::re::finditer(
                S("(?m)^\\s*#\\s*define\\s+(\\w+)\\s*\\(\\s*(\\w+)\\s*\\)\\s*\\(\\s*\\(\\s*((?:struct|union)"
                  "\\s+\\w+)\\s*\\*\\s*\\)\\s*\\(?\\s*\\2\\s*\\)?\\s*\\)\\s*$"),
                b_);
            for (const py::re::Match& mm : defs_) {
                str x_ = mm.group(3);
                b_ = py::re::sub(S("\\b") + esc(mm.group(1)) + S("\\s*\\(\\s*(\\w+)\\s*\\)"),
                                 [&](const py::re::Match& u) { return S("((") + x_ + S(" *)") + u.group(1) + S(")"); }, b_);
            }
            exp_[std::get<0>(nt)] = b_;
        }
        list<str> exps;
        for (const str& v : exp_.values()) exps.append(v);
        str alltext = py::join(S("\n"), exps);
        py::dict<str, str> heads_;
        py::set<str> weak_;
        for (const auto& nt : files.items()) {                  // (a header's structure that never has an object of
            const str& n = std::get<0>(nt);                     //  its own - no variable, no sizeof: every pointer to
            if (!n.endswith(S(".h"))) continue;                 //  it came from another's - a view too, if the evidence names one)
            for (const py::re::Match& m : py::re::finditer(S("\\bstruct\\s+(\\w+)\\s*\\{"), blank(std::get<1>(nt)))) {
                str x_ = S("struct ") + m.group(1);
                if (local.contains(x_)) continue;
                str sx = esc(m.group(1));
                str stripped = py::re::sub(S("\\bstruct\\s+") + sx + S("\\s*\\*"), S(""), alltext);
                bool own_ = py::re::search(S("\\bstruct\\s+") + sx + S("\\s+(?!\\{)\\**\\s*\\w+\\s*(?:\\[[^\\]]*\\]\\s*)?[;=,](?<!\\*)"), stripped) ||
                            py::re::search(S("sizeof\\s*\\(\\s*struct\\s+") + sx + S("\\s*\\)"), alltext);
                if (!views_cast) {
                    if (own_) continue;
                } else if (py::re::search(S("(?m)^(?:static\\s+|extern\\s+)?struct\\s+") + sx +
                                              S("\\s+(?!\\{)\\**\\s*\\w+\\s*(?:\\[[^\\]]*\\]\\s*)?[;=,](?<!\\*)"),
                                          py::re::sub(S("\\bstruct\\s+") + sx + S("\\s*\\*"), S(""), alltext)) ||
                           py::re::search(S("(?m)^[ \\t]+(?:register\\s+|static\\s+)?struct\\s+") + sx + S("\\s+\\w+\\s*\\["), alltext) ||
                           py::re::search(S("\\bstruct\\s+") + sx + S("\\s+\\w+\\s*[,)]"), alltext) ||
                           py::re::search(S("sizeof\\s*\\(\\s*struct\\s+") + sx + S("\\s*\\)"), alltext)) {
                    continue;                                   // (an object of its own at a file's top; a function's own)
                }
                if (own_) weak_.add(x_);                        // (a function's own object of it, struct targ t;: a
                heads_.setdefault(x_, n);                       //  view only where one thing is cast to both)
            }
        }
        std::vector<std::pair<str, str>> order;
        for (const auto& kv : local.items()) order.emplace_back(std::get<0>(kv), std::get<1>(kv));
        for (const auto& kv : heads_.items()) order.emplace_back(std::get<0>(kv), std::get<1>(kv));
        for (const auto& xn : order) {
            const str& x = xn.first;
            const str& n = xn.second;
            const Layout* lx = old.get(x);
            if (!lx) continue;
            py::set<str> strong_;                               // (one thing cast to both: the surest evidence)
            bool x_head = heads_.contains(x);
            // pmx: ptr_mem, or for a header's the members a single structure has
            auto pmx_get = [&](const str& k) -> py::set<str> {
                if (!ptr_mem.contains(k)) return py::set<str>();
                if (x_head && ptr_mem.at(k).size() != 1) return py::set<str>();
                return ptr_mem.at(k);
            };
            py::set<str> ys;
            str X = esc_ws(x);
            for (const py::re::Match& m : py::re::finditer(S("\\(\\s*") + X + S("\\s*\\*\\s*\\)\\s*([\\w.\\->\\[\\]]+)"), alltext)) {
                list<str> last = py::re::findall(S("\\w+"), m.group(1));
                if (!last.empty()) ys.update(pmx_get(last[-1]));
                if (views_cast && x_head && py::re::search(S("(?:->|\\.)\\s*\\w+$"), m.group(1))) {   // ((struct targ *)p->op and
                    for (const py::re::Match& o : py::re::finditer(                                   // (struct psym *)p->op: one thing, two views
                             S("\\(\\s*((?:struct|union)\\s+\\w+)\\s*\\*\\s*\\)\\s*\\(?\\s*") + esc(m.group(1)) + S("(?![\\w\\[(])"),
                             alltext)) {
                        str oy = squeeze(o.group(1));
                        if (oy != x && !heads_.contains(oy)) {
                            ys.add(oy);
                            strong_.add(oy);
                        }
                    }
                }
                if (!last.empty() && py::re::fullmatch(S("\\w+"), m.group(1))) {   // (a variable's: as it is declared)
                    std::optional<str> n_;
                    for (const auto& et : exp_.items()) {
                        if (py::re::search(S("\\(\\s*") + X + S("\\s*\\*\\s*\\)\\s*") + esc(m.group(1)) + S("\\b"), std::get<1>(et))) {
                            n_ = std::get<0>(et);
                            break;
                        }
                    }
                    std::optional<str> py_;
                    for (const auto& kp : param_types.items()) {
                        const str& fn_ = std::get<0>(std::get<0>(kp));
                        const str& pt = std::get<1>(kp);
                        if (last_star(pt) == m.group(1) && fns.contains(fn_) &&
                            (fns.at(fn_)->file ? *fns.at(fn_)->file : S("")) == (n_ ? *n_ : S("\x01none"))) {
                            py_ = py::re::sub(S("\\s*\\*.*"), S(""), pt).strip();
                        }
                    }
                    if (!py_ || py_->empty()) py_ = n_ ? pointee_of(files.at(*n_), m.group(1)) : std::nullopt;
                    if (py_ && !py_->empty()) {
                        ys = py::set<str>();
                        ys.add(*py_);
                    }
                }
            }
            for (const py::re::Match& m :
                 py::re::finditer(S("\\(\\s*") + X + S("\\s*\\*\\s*\\)\\s*\\(?\\s*[\\w.\\->\\[\\]]*?(?:->|\\.)\\s*(\\w+)\\s*\\)?"), alltext)) {
                for (const py::re::Match& am : py::re::finditer(
                         S("(?:->|\\.)\\s*") + esc(m.group(1)) + S("\\s*=\\s*([A-Za-z_]\\w*)\\s*\\("), alltext)) {   // ((struct func *)node->i2, and n->i2 = f0b6cc() -
                    auto rm = py::re::search(S("\\b((?:struct|union)\\s+\\w+)\\s*\\*\\s*") + esc(am.group(1)) + S("\\s*\\("), alltext);   // a struct isym *: what the member holds)
                    if (rm && squeeze(rm->group(1)) != x) ys.add(squeeze(rm->group(1)));
                }
            }
            // (or a variable of it given what a function of another's gives: p = f0b67a(sh), struct isym *)
            for (const auto& et : exp_.items()) {
                const str& t_ = std::get<1>(et);
                for (const str& vv : py::set<str>::from(py::re::findall(S("\\b") + X + S("\\s*\\*\\s*(\\w+)\\s*[;,=)]"), t_))) {
                    for (const py::re::Match& am : py::re::finditer(S("(?<![\\w.>])") + esc(vv) + S("\\s*=\\s*([A-Za-z_]\\w*)\\s*\\("), t_)) {
                        auto rm = py::re::search(S("\\b((?:struct|union)\\s+\\w+)\\s*\\*\\s*") + esc(am.group(1)) + S("\\s*\\("),
                                                 t_ + S("\n") + alltext);
                        if (rm && squeeze(rm->group(1)) != x) ys.add(squeeze(rm->group(1)));
                    }
                }
            }
            for (const FnP& fn : fns.values()) {                // (or what the callers give a parameter of it)
                for (int64_t k = 0; k < fn->params.size(); ++k) {
                    const str& pt = std::get<1>(fn->params[k]);
                    if (!py::re::search(X + S("\\s*\\*"), pt)) continue;
                    for (const py::re::Match& m : py::re::finditer(S("\\b") + esc(fn->name) + S("\\s*\\("), alltext)) {
                        auto spo = call_args(alltext.slice(m.start(), m.start() + 400), fn->name, false);
                        list<Span> sp = spo ? *spo : list<Span>();
                        if (k >= sp.size()) continue;
                        str a_ = alltext.slice(m.start() + std::get<0>(sp[k]), m.start() + std::get<1>(sp[k])).strip();
                        list<str> last = py::re::findall(S("\\w+"), a_);
                        if (!last.empty() && py::re::fullmatch(S("\\w+"), a_)) {   // (a variable: as declared there, or
                            str pre = alltext.slice(0, m.start());                  //  as the loop typed a parameter)
                            std::optional<str> caller;
                            list<str> heads = py::re::findall(S("(?m)^[\\w\\s*]*?\\b(\\w+)\\s*\\([^;{]*\\)\\s*$"), pre);
                            for (int64_t hi = heads.size() - 1; hi >= 0; --hi) {
                                if (fns.contains(heads[hi])) { caller = heads[hi]; break; }
                            }
                            std::optional<str> py_;
                            for (const auto& kp : param_types.items()) {
                                const str& pt2 = std::get<1>(kp);
                                if (caller && std::get<0>(std::get<0>(kp)) == *caller && last_star(pt2) == a_) {
                                    py_ = py::re::sub(S("\\s*\\*.*"), S(""), pt2).strip();
                                    break;
                                }
                            }
                            std::optional<str> cf;
                            if (caller && fns.contains(*caller)) cf = fns.at(*caller)->file;
                            if (!py_ || py_->empty()) {
                                py_ = (cf && files.contains(*cf)) ? pointee_of(files.at(*cf), a_) : std::nullopt;
                            }
                            if (py_ && !py_->empty()) {
                                ys.add(*py_);
                                continue;
                            }
                        }
                        if (!last.empty() && py::re::fullmatch(S("[\\w.\\->\\[\\]]+"), a_)) {
                            ys.update(pmx_get(last[-1]));
                            if (py::re::search(S("(?:->|\\.)\\s*") + esc(last[-1]) + S("$"), a_)) {
                                for (const py::re::Match& am : py::re::finditer(   // (f(text, node->i2), n->i2 = f0b6cc():
                                         S("(?:->|\\.)\\s*") + esc(last[-1]) + S("\\s*=\\s*([A-Za-z_]\\w*)\\s*\\("),   // a word
                                         alltext)) {                                                                    // holding a struct isym *)
                                    auto rm = py::re::search(S("\\b((?:struct|union)\\s+\\w+)\\s*\\*\\s*") + esc(am.group(1)) + S("\\s*\\("),
                                                             alltext);
                                    if (rm && squeeze(rm->group(1)) != x) ys.add(squeeze(rm->group(1)));
                                }
                            }
                        }
                    }
                }
            }
            {
                py::set<str> ys2;
                for (const str& y_ : ys) if (!local.contains(y_) && y_ != x) ys2.add(y_);
                ys = ys2;
            }
            if (ys.size() > 1 && strong_.size() == 1) ys = py::set<str>::from(strong_);   // (the other a header's: two files' own copies of one
            if (ys.size() != 1) continue;                                                  //  structure are each as it is)
            str y = *ys.begin();
            const Layout* ly = old.get(y);
            if (!ly || y == x || (weak_.contains(x) && !(strong_.size() == 1 && strong_.contains(y)))) continue;
            std::vector<YM> ymem;                               // (and the members of a structure in it: c.op)
            for (const auto& kv : ly->members) {
                const str& k = kv.first;
                const Member& v = kv.second;
                ymem.push_back({v.off, v.size, k, v});
                const Layout* sub = nullptr;
                if (!v.isptr) {
                    sub = old.get(squeeze(v.type));
                    if (!sub) {
                        str key = y + S(".") + k;
                        if (old.inner.contains(key)) sub = &std::get<1>(old.inner.at(key));
                    }
                }
                if (sub && !v.decl.contains(S("["))) {
                    for (const auto& kv2 : sub->members) {
                        ymem.push_back({v.off + kv2.second.off, kv2.second.size, k + S(".") + kv2.first, kv2.second});
                    }
                }
            }
            std::stable_sort(ymem.begin(), ymem.end(), [](const YM& a, const YM& b) {
                return a.off != b.off ? a.off < b.off : -a.size < -b.size;
            });
            if (x_head) {                                       // (a header's: only when its link to its own kind is
                py::set<int64_t> selfx, selfy;                  //  where the other's own link is - one list, two names)
                for (const auto& kv : lx->members) if (kv.second.isptr && squeeze(kv.second.type) == x) selfx.add(kv.second.off);
                for (const YM& ym : ymem) if (ym.v.isptr && squeeze(ym.v.type) == y) selfy.add(ym.off);
                if (!selfx.empty() && !selfx.issubset(selfy)) continue;
                if (selfx.empty()) {                            // (no link of its own - struct rseg over struct seg: its
                    std::vector<int64_t> xptr;                  //  pointers each on one of the other's)
                    py::set<int64_t> yptr;
                    for (const auto& kv : lx->members) if (kv.second.isptr) xptr.push_back(kv.second.off);
                    for (const YM& ym : ymem) if (ym.v.isptr) yptr.add(ym.off);
                    bool strong_is_y = strong_.size() == 1 && strong_.contains(y);
                    bool all_in = true;
                    for (int64_t o : xptr) if (!yptr.contains(o)) all_in = false;
                    if (!views_cast || (!strong_is_y && (xptr.empty() || !all_in))) continue;   // (one thing cast to both)
                }
            }
            py::dict<str, ViewMap> mapping;
            bool mapping_ok = true;
            for (const auto& kv : lx->members) {
                const str& k = kv.first;
                const Member& v = kv.second;
                int64_t lo = v.off, hi = v.off + v.size;
                std::vector<const YM*> same;                    // (one member just there: of its type the best)
                for (const YM& ym : ymem) if (ym.off == lo && ym.size == v.size) same.push_back(&ym);
                if (!same.empty()) {
                    const YM* best = nullptr;
                    for (const YM* ym : same) if (ym->v.type == v.type && ym->v.isptr == v.isptr) { best = ym; break; }
                    if (!best) for (const YM* ym : same) if (ym->v.isptr == v.isptr) { best = ym; break; }
                    if (!best) best = same[0];
                    auto ct0 = ct_small(squeeze(v.type).strip());
                    if (!v.isptr && best->v.isptr && ct0 && x_head) {   // (a number kept in the other's pointer's place - hi in
                        ViewMap vm;                                     //  left: the word there, read as a number)
                        vm.pun = true; vm.base = best->name; vm.off = 0; vm.ct = *ct0;
                        mapping[k] = vm;
                        continue;
                    }
                    ViewMap vm;
                    vm.names = list<str>({best->name});
                    mapping[k] = vm;
                    continue;
                }
                std::vector<const YM*> top, cover, inside;
                for (const YM& ym : ymem) if (!ym.name.contains(S("."))) top.push_back(&ym);
                for (const YM* ym : top) if (lo <= ym->off && ym->off < hi) cover.push_back(ym);
                auto ct_ = ct_long(squeeze(v.type).strip());
                for (const YM* ym : top) if (ym->off <= lo && hi <= ym->off + ym->size && ym->size > v.size) inside.push_back(ym);
                if (!inside.empty() && ct_ && !v.isptr && y.startswith(S("struct "))) {   // (a union's all at 0: none)
                    ViewMap vm;                                 // (a byte of a word of the other's: that byte)
                    vm.pun = true; vm.base = inside[0]->name; vm.off = lo - inside[0]->off; vm.ct = *ct_;
                    mapping[k] = vm;
                    continue;
                }
                if (cover.empty() || cover.front()->off != lo || cover.back()->off + cover.back()->size != hi) {
                    mapping_ok = false;
                    break;
                }
                bool simple = true;
                for (const YM* c : cover) if (c->v.isptr || c->v.decl.contains(S("["))) simple = false;
                if (cover.size() > 1 && v.size <= 2 && ct_ && !v.isptr && y.startswith(S("struct ")) && simple) {
                    ViewMap vm;                                 // (a word over two bytes of the other's: the word there)
                    vm.pun = true; vm.base = cover[0]->name; vm.off = 0; vm.ct = *ct_;
                    mapping[k] = vm;
                    continue;
                }
                ViewMap vm;
                for (const YM* c : cover) vm.names.append(c->name);
                mapping[k] = vm;
            }
            if (!mapping_ok) mapping = py::dict<str, ViewMap>();
            bool have = mapping_ok && !mapping.empty();
            if (have && x_head) {                               // (a header's: only a view whose link is the other's
                std::vector<str> links;                         //  under another name - u6.n for next: two structures
                for (const auto& kv : lx->members) if (kv.second.isptr && squeeze(kv.second.type) == x) links.push_back(kv.first);
                bool all_self = true;
                for (const str& k : links) {
                    bool is_self = mapping.contains(k) && !mapping.at(k).pun && mapping.at(k).names.size() == 1 &&
                                   mapping.at(k).names[0] == k;
                    if (!is_self) all_self = false;
                }
                if ((!links.empty() || !views_cast) && all_self) have = false;   // sharing a head, each its own, are not one)
                // ytop: the other's members by name (later ones override)
                std::vector<std::pair<str, const Member*>> ytop;
                auto ytop_get = [&](const str& w) -> const Member* {
                    const Member* r = nullptr;
                    for (const YM& ym : ymem) if (ym.name == w) r = &ym.v;
                    return r;
                };
                std::vector<str> on_num;
                if (have) {
                    for (const auto& kv : lx->members) {
                        if (kv.second.isptr && mapping.contains(kv.first) && !mapping.at(kv.first).pun) {
                            for (const str& w : mapping.at(kv.first).names) {
                                const Member* yt = ytop_get(w);
                                if (!(yt && yt->isptr)) on_num.push_back(w);
                            }
                        }
                    }
                }
                bool strong_is_y = strong_.size() == 1 && strong_.contains(y);
                bool all_two = true;
                for (const str& w : on_num) {
                    const Member* yt = ytop_get(w);
                    if (!(yt && yt->size == 2 && py::re::fullmatch(S("(?:unsigned\\s+)?(?:int|short|unsigned)"), yt->type.strip()))) all_two = false;
                }
                if (!on_num.empty() && strong_is_y && all_two) {
                    for (const str& w : on_num) {               // (struct lab over struct psym, its refs where psym's
                        for (const auto& ht : files.items()) {  // cls is - one thing cast to both: the number a
                            auto nw_ = widen_member(std::get<1>(ht), last_word(y), w);   // pointer's width, to hold what the other view keeps there)
                            if (nw_) files[std::get<0>(ht)] = *nw_;
                        }
                    }
                    list<str> ws;
                    for (const str& w : on_num) ws.append(w);
                    notes_view_wide.append(n + S(" (") + x + S(" over ") + y + S(": ") + py::join(S(", "), ws) + S(")"));
                } else if (!on_num.empty()) {                   // (nor one whose pointer lies on the other's number)
                    have = false;
                }
                // (a union of the other's whose members are all pointers)
                auto ptr_union = [&](const str& w) -> bool {
                    str w0 = py::split(w, S("."))[0];
                    const Member* yt = ytop_get(w0);
                    str ty = yt ? yt->type : S("");
                    if (!py::re::match(S("union\\b"), ty)) return false;
                    const Layout* sub = nullptr;
                    str key = y + S(".") + w0;
                    if (old.inner.contains(key)) sub = &std::get<1>(old.inner.at(key));
                    else sub = old.get(key);
                    if (!sub) return false;
                    for (const auto& kv2 : sub->members) if (!kv2.second.isptr) return false;
                    return true;
                };
                if (have) {
                    bool bad = false;
                    for (const auto& km : mapping.items()) {
                        const Member* lk = lx->find(std::get<0>(km));
                        const ViewMap& vm = std::get<1>(km);
                        std::vector<str> ws;
                        if (!vm.pun) for (const str& w : vm.names) ws.push_back(w);
                        else ws.push_back(vm.base);
                        for (const str& w : ws) if (lk && !lk->isptr && ptr_union(w)) bad = true;
                    }
                    if (bad) have = false;                      // (nor one with a number on the other's union of pointers:
                }                                               //  MC2's stor's st_off over ruse's r_u - two kinds, not one)
            }
            if (have) {
                ViewEntry ve;
                ve.y = y;
                ve.mapping = mapping;
                ve.n = n;
                found[x] = ve;
            }
        }
        if (views_cast) _views_weak = py::set<str>::from(weak_);
        return found;
    };

    if (!_views) {
        _views = build(false);
        if (views_cast_rule) {                                  // (the views one thing cast to both adds, or changes:
            py::dict<str, ViewEntry> plain = *_views;           //  none that reads a byte of the other's word - a
            for (const auto& xv : build(true).items()) {        //  record then taken as bytes, MASM 4.00's opitem over its node)
                const str& x = std::get<0>(xv);
                const ViewEntry& v = std::get<1>(xv);
                bool differs = !plain.contains(x) || !(plain.at(x) == v);
                bool anypun = false;
                for (const ViewMap& vm : v.mapping.values()) if (vm.pun) anypun = true;
                if (differs && anypun) {
                    view_puns_refused.append(x + S(" over ") + v.y);
                    continue;
                }
                if (differs) _views_new.add(x);
                (*_views)[x] = v;
            }
        }
    }
    py::dict<str, ViewEntry> views;
    for (const auto& xv : _views->items()) {
        const ViewEntry& v = std::get<1>(xv);
        if (v.n == fname || v.n.endswith(S(".h"))) views[std::get<0>(xv)] = v;
    }
    if (views.empty()) return t;
    for (const auto& xv : views.items()) {
        const str& x = std::get<0>(xv);
        const str& y = std::get<1>(xv).y;
        const auto& mapping = std::get<1>(xv).mapping;
        str X = esc_ws(x);
        if (_views_weak.contains(x)) {                          // (a view whose functions have objects of it)
            t = py::re::sub(S("(?m)^([ \\t]+(?:register\\s+)?)") + X + S("(\\s+\\w+\\s*;)"),
                            [&](const py::re::Match& m_) { return m_.group(1) + y + m_.group(2); }, t);   // (struct targ t; tp = &t: the room of the
        }                                                                                                   //  other's, whose members the view reads)
        str code = blank(t);
        py::set<str> vars_ = py::set<str>::from(py::re::findall(X + S("\\s*\\*\\s*(\\w+)"), code));
        list<str> macs;
        for (const py::re::Match& mm : py::re::finditer(
                 S("(?m)^\\s*#\\s*define\\s+(\\w+)\\s*\\(\\s*(\\w+)\\s*\\)\\s*\\(\\s*\\(\\s*") + X +
                     S("\\s*\\*\\s*\\)\\s*\\(?\\s*\\2\\s*\\)?\\s*\\)\\s*$"),
                 code)) {
            macs.append(mm.group(1));
        }
        std::vector<std::tuple<int64_t, int64_t, str>> edits;
        list<str> pats;
        if (!vars_.empty()) {
            list<str> vs;
            for (const str& v : py::sorted(vars_)) vs.append(esc(v));
            pats.append(S("\\b(") + py::join(S("|"), vs) + S(")\\s*->\\s*(\\w+)\\b"));
        }
        if (!macs.empty()) {                                    // (L(s)->num: the macro's cast, the other's member)
            list<str> vs;
            for (const str& v : macs) vs.append(esc(v));
            pats.append(S("\\b(?:") + py::join(S("|"), vs) + S(")\\s*\\(\\s*(\\w+)\\s*\\)\\s*->\\s*(\\w+)\\b"));
        }
        if (_views_new.contains(x)) {                           // (the cast's too, for the views the casts gave)
            pats.append(S("\\(\\s*\\(\\s*") + X +
                        S("\\s*\\*\\s*\\)\\s*([\\w.\\->\\[\\]]+?|\\((?:[^()]|\\([^()]*\\))*\\))\\s*\\)\\s*->\\s*(\\w+)\\b"));   // (((struct
        }                                                                                                                      // targ *)g_2622->sym)->kind: the cast's too)
        std::vector<std::tuple<int64_t, int64_t, str, str>> found_;
        for (const str& p_ : pats) {
            for (const py::re::Match& m : py::re::finditer(p_, code)) found_.emplace_back(m.start(), m.end(), m.group(1), m.group(2));
        }
        std::stable_sort(found_.begin(), found_.end());
        for (const auto& f : found_) {
            int64_t st0 = std::get<0>(f), en0 = std::get<1>(f);
            const str& var0 = std::get<2>(f);
            const str& mem0 = std::get<3>(f);
            if (!mapping.contains(mem0)) continue;
            const ViewMap& ms = mapping.at(mem0);
            str pre = code.slice(0, st0).rstrip();
            str post = code.slice_from(en0).lstrip();
            bool as_arg = (pre.endswith(S("(")) || pre.endswith(S(","))) && (post.startswith(S(",")) || post.startswith(S(")")));
            if (ms.pun) {                                       // (a part of a member of the other's, or a word over two)
                edits.emplace_back(st0, en0, S("(*(") + ms.ct + S(" *)((char *)&((") + y + S(" *)") + var0 + S(")->") +
                                                  ms.base + S(" + ") + itos(ms.off) + S("))"));
                continue;
            }
            if (ms.names.size() == 1) {
                edits.emplace_back(st0, en0, S("((") + y + S(" *)") + var0 + S(")->") + ms.names[0]);
            } else if (as_arg) {
                list<str> parts;
                for (const str& w : ms.names) parts.append(S("((") + y + S(" *)") + var0 + S(")->") + w);
                edits.emplace_back(st0, en0, py::join(S(", "), parts));
            }
        }
        std::sort(edits.begin(), edits.end(), std::greater<>());
        for (const auto& e : edits) t = t.slice(0, std::get<0>(e)) + std::get<2>(e) + t.slice_from(std::get<1>(e));
        if (!edits.empty()) {
            list<str> hs_;
            for (const auto& ht : files.items()) {
                if (std::get<0>(ht).endswith(S(".h")) && py::re::search(S("\\b") + esc_ws(y) + S("\\s*\\{"), blank(std::get<1>(ht)))) {
                    hs_.append(std::get<0>(ht));
                }
            }
            std::optional<str> h_;
            for (const str& h : hs_) if (std::get<0>(py::os::path::splitext(h)) == last_word(y)) { h_ = h; break; }
            if (!h_ && !hs_.empty()) h_ = hs_[0];
            if (h_) {                                           // (the other's members: its header; a set in the Python)
                list<str>& hl = need_headers.setdefault(fname, list<str>());
                if (!hl.contains(*h_)) hl.append(*h_);
            }
            note(notes, "a file's own view of another structure by its old places: its members the other's (a long over "
                        "two of them, given, the two)",
                 fname + S(" (") + x + S(" as ") + y + S(")"));
        }
    }
    return t;
}

}  // namespace codebreaker::port::pre_ansi
