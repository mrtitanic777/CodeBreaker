// codebreaker/exact/habits/newcomp.cpp -- C++ from codebreaker.exact.habits.newcomp.
//
// Where the Python keeps a register renaming as a dict whose values may be None until setmap filters them,
// the C++ keeps the unfiltered one as an OMap (str -> optional<str>) and setmap filters it into the Map mp.

#include "codebreaker/exact/habits/newcomp.hpp"

#include <algorithm>
#include <map>
#include <set>

namespace codebreaker::exact::habits::newcomp {

namespace {

using Map = py::dict<py::str, py::str>;
using OMap = py::dict<py::str, std::optional<py::str>>;
using WalkItem = cmpp::WalkItem;
using Walk = py::list<WalkItem>;
using Jump = std::tuple<int64_t, int64_t>;

int64_t off(const WalkItem& x) { return std::get<0>(x); }
int64_t siz(const WalkItem& x) { return std::get<1>(x); }
const py::str& txt(const WalkItem& x) { return std::get<2>(x); }
const py::str& mnem(const WalkItem& x) { return std::get<3>(x); }

// int(text, 0): a literal with its own base prefix; a leading zero is not allowed before more digits.
int64_t int0(const py::str& s) {
    const std::string t = s.strip().utf8();
    size_t i = (!t.empty() && (t[0] == '+' || t[0] == '-')) ? 1 : 0;
    if (t.size() > i + 1 && t[i] == '0' && ((t[i + 1] >= '0' && t[i + 1] <= '9') || t[i + 1] == '_')) {
        for (size_t j = i; j < t.size(); ++j) {
            if (t[j] != '0' && t[j] != '_') throw py::ValueError("invalid literal for int() with base 0: " + py::repr(s).utf8());
        }
    }
    return py::int_from_str(s, 0);
}

py::re::Pattern pat(const std::string& p) { return py::re::Pattern(py::str(p), 0); }
std::optional<py::re::Match> fullmatch(const std::string& p, const py::str& s) { return pat(p).fullmatch(s); }
std::optional<py::re::Match> rematch(const std::string& p, const py::str& s) { return pat(p).match(s); }
std::optional<py::re::Match> research(const std::string& p, const py::str& s) { return pat(p).search(s); }

// struct.unpack_from("<h", b, at)[0] and "<H"
int64_t unpack_h(const py::bytes& b, int64_t at) { return py::unpack_from1<int64_t>("<h", b, at); }
int64_t unpack_H(const py::bytes& b, int64_t at) { return py::unpack_from1<int64_t>("<H", b, at); }

py::str hex04(int64_t v) { return py::percent(py::str("%04x"), v); }

// ENDS = (0xE8, 0x9A, 0xE9, 0xEB, 0xC2, 0xC3, 0xCA, 0xCB)
bool in_ends(int64_t c) { return c == 0xE8 || c == 0x9A || c == 0xE9 || c == 0xEB || c == 0xC2 || c == 0xC3 || c == 0xCA || c == 0xCB; }
bool is_jcc(int64_t c) { return 0x70 <= c && c <= 0x7F; }

OMap omap(const Map& m) {
    OMap o;
    for (const auto& kv : m.items()) o.set(std::get<0>(kv), std::get<1>(kv));
    return o;
}

// sorted(mp.items())
std::vector<std::tuple<py::str, py::str>> sorted_items(const Map& m) {
    auto v = m.items();
    std::sort(v.begin(), v.end());
    return v;
}

// ", ".join("%s for %s" % (k.upper(), v.upper()) for k, v in sorted(mp.items()))
py::str renaming_text(const Map& m) {
    py::str names;
    bool first = true;
    for (const auto& kv : sorted_items(m)) {
        names += py::str(first ? "" : ", ") + py::percent(py::str("%s for %s"), std::get<0>(kv).upper(), std::get<1>(kv).upper());
        first = false;
    }
    return names;
}

// the registers in the patterns (R16 of the Python)
const std::string R16S = "ax|cx|dx|bx|si|di";

// repr() of a pair of strings
py::str show_pair(const py::str& a, const py::str& b) { return py::str("(") + py::repr(a) + py::str(", ") + py::repr(b) + py::str(")"); }

// the jump tables (CASE statements) of the compiled code: each is the words a linker fills in behind a jmp cs:[bx+table]
py::list<int64_t> table_lengths(const py::bytes& got, const py::set<int64_t>& fx) {
    py::list<int64_t> tabs;
    int64_t k = got.find(TABJMP);
    while (k >= 0) {
        int64_t n = 0;
        while (fx.contains(k + 5 + 2 * n) && fx.contains(k + 6 + 2 * n)) n += 1;
        tabs.append(2 * n);
        k = got.find(TABJMP, k + 5);
    }
    return tabs;
}

// the instructions of a procedure, with the jump tables stepped over (they are words, not instructions)
Walk walk_with_tables(const py::bytes& b, const py::list<int64_t>& tabs) {
    Walk out;
    int64_t p = 0, t = 0;
    while (true) {
        int64_t k = b.find(TABJMP, p);
        if (k < 0 || t >= py::len(tabs)) {
            for (const WalkItem& x : cmpp::walk(b.slice_from(p), 0, p == 0)) out.append(WalkItem(p + off(x), siz(x), txt(x), mnem(x)));
            return out;
        }
        for (const WalkItem& x : cmpp::walk(b.slice(p, k + 5), 0, p == 0)) out.append(WalkItem(p + off(x), siz(x), txt(x), mnem(x)));
        out.append(WalkItem(k + 5, tabs[t], py::percent(py::str("table of %d"), tabs[t] / 2), py::str("table")));
        p = k + 5 + tabs[t];
        t += 1;
    }
}

// the lines of a stretch for boolx: a jump by its target, a variable's address by the order they come in
py::list<boolx::Instruction> cond_lines(const Walk& L, const py::bytes& b, int64_t i0, int64_t i1) {
    py::list<boolx::Instruction> out;
    py::dict<py::str, int64_t> glob;   // a variable's address is the linker's: named by the order they come in
    for (const WalkItem& it : L.slice(i0, i1)) {
        int64_t o = off(it), n = siz(it);
        py::str x = txt(it);
        auto r = rel(b.slice(o, o + n));
        if (r && b[o] != 0xE8) {
            x = py::percent(py::str("%s 0x%x"), mnem(it), o + n + std::get<1>(*r));
        } else {
            x = pat("\\[(0x[0-9a-f]+|\\d+)\\]").sub_fn(
                [&](const py::re::Match& mm) {
                    py::str key = mm.group(1);
                    if (!glob.contains(key)) glob.set(key, py::len(glob));
                    return py::percent(py::str("[g%d]"), glob.at(key));
                },
                x);
        }
        out.append(boolx::Instruction(o, x));
    }
    return out;
}

// the registers SI and DI hold at index i: the last "mov R, word ptr [..]" before it, or their start values
py::dict<py::str, py::str> cond_bases(const Walk& L, int64_t i) {
    py::dict<py::str, py::str> d;
    d.set(py::str("si"), py::str("si0"));
    d.set(py::str("di"), py::str("di0"));
    for (const char* r : {"si", "di"}) {
        for (int64_t j = i - 1; j > std::max<int64_t>(-1, i - 40); --j) {
            auto m = fullmatch(std::string("mov ") + r + ", word ptr \\[([^\\]]+)\\]", txt(L[j]));
            if (m) { d.set(py::str(r), m->group(1)); break; }
            if (rematch(std::string("(mov|xchg) ") + r + "\\b", txt(L[j])) || mnem(L[j]) == "call" || mnem(L[j]) == "lcall") break;
        }
    }
    return d;
}

std::optional<py::str> fam_part(const Family& f, int64_t k) {
    return k == 0 ? std::get<0>(f) : k == 1 ? std::get<1>(f) : std::get<2>(f);
}

}  // namespace

const py::bytes DISPOSE = py::bytes::fromhex(py::str("9a5002a20c"));   // call far 0CA2:0250, the heap's routine that frees a block

const py::re::Pattern& NUM() {
    static const py::re::Pattern p(py::str("0x[0-9a-f]+|\\b\\d+\\b"), 0);
    return p;
}

const py::dict<py::str, Family> FAM = {
    {py::str("ax"), Family(py::str("ax"), py::str("al"), py::str("ah"))},
    {py::str("cx"), Family(py::str("cx"), py::str("cl"), py::str("ch"))},
    {py::str("dx"), Family(py::str("dx"), py::str("dl"), py::str("dh"))},
    {py::str("bx"), Family(py::str("bx"), py::str("bl"), py::str("bh"))},
    {py::str("si"), Family(py::str("si"), std::nullopt, std::nullopt)},
    {py::str("di"), Family(py::str("di"), std::nullopt, std::nullopt)},
};

namespace {
// PART = {p: (f, k) for f, ps in FAM.items() for k, p in enumerate(ps) if p}
py::dict<py::str, std::tuple<py::str, int64_t>> make_part() {
    py::dict<py::str, std::tuple<py::str, int64_t>> out;
    for (const auto& fk : FAM.items()) {
        for (int64_t k = 0; k < 3; ++k) {
            std::optional<py::str> p = fam_part(std::get<1>(fk), k);
            if (p) out.set(*p, std::make_tuple(std::get<0>(fk), k));
        }
    }
    return out;
}
}  // namespace

const py::dict<py::str, std::tuple<py::str, int64_t>> PART = make_part();

const py::re::Pattern& REG() {
    static const py::re::Pattern p = [] {
        py::str alt;
        bool first = true;
        for (const py::str& k : PART.keys()) {
            alt += py::str(first ? "" : "|") + k;
            first = false;
        }
        return py::re::Pattern(py::str("\\b(") + alt + py::str(")\\b"), 0);
    }();
    return p;
}

const py::str R16 = py::str("ax|cx|dx|bx|si|di");
const py::bytes TABJMP = py::bytes::fromhex(py::str("2effa7"));   // jmp word ptr cs:[bx + table]
bool DEBUG = false;
py::list<int64_t> GWALK;                                          // ... and where the compiled code's instructions begin
py::list<std::tuple<int64_t, int64_t>> AMAP;                      // the alignment of the last pair found equivalent

py::str mask(const py::str& t) { return NUM().sub(py::str("N"), t).replace(py::str("- N"), py::str("+ N")); }

// a relative jump or call -> (length of opcode, displacement) or None
std::optional<std::tuple<int64_t, int64_t>> rel(const py::bytes& b) {
    if (py::len(b) == 2 && ((0x70 <= b[0] && b[0] <= 0x7F) || b[0] == 0xEB || b[0] == 0xE2 || b[0] == 0xE3))
        return std::make_tuple<int64_t, int64_t>(1, b[1] > 127 ? b[1] - 256 : b[1]);
    if (py::len(b) == 3 && (b[0] == 0xE8 || b[0] == 0xE9)) return std::make_tuple<int64_t, int64_t>(1, unpack_h(b, 1));
    return std::nullopt;
}

// the original's instruction with the registers renamed; None if it cannot be written (SI and DI have no halves)
std::optional<py::str> rename(const py::str& t, const py::dict<py::str, py::str>& mp) {
    bool bad = false;
    py::str r = REG().sub_fn(
        [&](const py::re::Match& m) -> py::str {
            const auto& fk = PART.at(m.group(1));
            const py::str& f = std::get<0>(fk);
            int64_t k = std::get<1>(fk);
            std::optional<py::str> p = fam_part(FAM.at(mp.get(f, f)), k);
            if (!p) { bad = true; return m.group(1); }
            return *p;
        },
        t);
    if (bad) return std::nullopt;
    return r;
}

std::tuple<bool, py::list<py::str>> equivalent(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    using Ret = std::tuple<bool, py::list<py::str>>;
    auto fail = [](const py::str& why) { return Ret(false, py::list<py::str>{why}); };
    // the jump tables of CASE statements are words, not instructions: in the compiled code every one is
    // an address the linker fills in, so their length is known; the original's lie at the same jumps
    const py::list<int64_t> tabs = table_lengths(got, fx);
    Walk W = walk_with_tables(want, tabs);
    Walk G = walk_with_tables(got, tabs);
    int64_t wi = 0, gi = 0;
    py::list<std::tuple<int64_t, int64_t>> amap;
    py::list<Jump> jumps;
    py::list<py::str> notes;
    Map mp;
    int64_t since = 0;
    py::list<int64_t> fors;
    int64_t reload = -1;
    py::dict<int64_t, int64_t> temps, rtemps;
    std::optional<std::tuple<py::str, int64_t>> count;
    auto fixed = [&](int64_t og, int64_t sg) {
        for (int64_t k = 0; k < sg; ++k)
            if (fx.contains(og + k)) return true;
        return false;
    };
    auto setmap = [&](const OMap& nw, int64_t ow) {
        Map nm;
        for (const auto& kv : nw.items())
            if (std::get<1>(kv) && std::get<0>(kv) != *std::get<1>(kv)) nm.set(std::get<0>(kv), *std::get<1>(kv));
        if (mp.empty() && !nm.empty()) since = ow;
        if (!mp.empty() && nm.empty())
            notes.append(py::percent(py::str("registers used otherwise (%s) from +%04x"), renaming_text(mp), since));
        mp = nm;
    };
    std::optional<std::tuple<int64_t, int64_t>> sync;

    // the last resort: from the last call or jump to the next, do the two compute the same (tools/symx.py)?
    auto stretch = [&]() -> bool {
        if (!sync || count) return false;
        auto end = [&](const Walk& L, const py::bytes& b, int64_t i) -> std::optional<int64_t> {
            while (i < py::len(L)) {
                int64_t c = b[off(L[i])];
                if (mnem(L[i]) == "table") return i;
                if (0x70 <= c && c <= 0x7F && b[off(L[i]) + 1] == 1 && i != 0 && fullmatch("mov \\w\\w, 0", txt(L[i - 1]))) { i += 1; continue; }
                if (in_ends(c) || (0x70 <= c && c <= 0x7F)) return i;
                i += 1;
            }
            return std::nullopt;
        };
        const int64_t s0 = std::get<0>(*sync), s1 = std::get<1>(*sync);
        std::optional<int64_t> ew = end(W, want, s0);
        std::optional<int64_t> eg = end(G, got, s1);
        if (!ew || !eg || *ew < wi || *eg < gi || want[off(W[*ew])] != got[off(G[*eg])] || mnem(W[*ew]) == "table") return false;
        int64_t c = want[off(W[*ew])];
        py::list<symx::Ins> iw, ig;
        for (const WalkItem& x : W.slice(s0, *ew)) iw.append(symx::Ins(txt(x), want.slice(off(x), off(x) + siz(x))));
        for (const WalkItem& x : G.slice(s1, *eg)) ig.append(symx::Ins(txt(x), got.slice(off(x), off(x) + siz(x))));
        py::dict<int64_t, py::set<int64_t>> fx_;
        {
            int64_t k = 0;
            for (const WalkItem& x : G.slice(s1, *eg)) {
                if (fixed(off(x), siz(x))) {
                    py::set<int64_t> s;
                    for (int64_t j = 0; j < siz(x) - 1; ++j)
                        if (fx.contains(off(x) + j) && fx.contains(off(x) + j + 1)) s.add(unpack_H(got, off(x) + j));
                    fx_.set(k, s);
                }
                k += 1;
            }
        }
        auto res = symx::same(iw, ig, fx_, !(c == 0xE8 || c == 0x9A), 0x70 <= c && c <= 0x7F);
        if (!std::get<0>(res)) {
            if (DEBUG) py::print(py::percent(py::str("symx +%04x..+%04x: %s"), off(W[s0]), off(W[*ew]), std::get<1>(res)));
            return false;
        }
        {
            py::list<std::tuple<int64_t, int64_t>> keep;
            for (const auto& x : amap)
                if (std::get<0>(x) <= off(W[s0])) keep.append(x);
            amap.vec() = keep.vec();
        }
        notes.append(py::percent(py::str("the same values, by other registers and temporaries, from +%04x to +%04x"), off(W[s0]), off(W[*ew])));
        wi = *ew;
        gi = *eg;
        mp = Map();
        return true;
    };

    // a condition the compiled code works out in full and the original leaves early (tools/boolx.py)
    auto cond = [&]() -> bool {
        int64_t ws = wi, gs = gi;
        while (ws != 0 && gs != 0 && txt(W[ws - 1]) == txt(G[gs - 1]) && rematch("(cmp|test) ", txt(G[gs - 1]))) {
            ws -= 1;
            gs -= 1;
        }
        std::optional<int64_t> eg;
        for (int64_t j = gi; j < std::min<int64_t>(py::len(G) - 1, gi + 60); ++j) {
            const py::str& m = mnem(G[j]);
            if (m == "call" || m == "lcall" || m == "ret" || m == "retf" || m == "table") return false;
            if (fullmatch("shr (ax|cx|dx), 1", txt(G[j])) && (mnem(G[j + 1]) == "jae" || mnem(G[j + 1]) == "jb")) { eg = j + 2; break; }
        }
        if (!eg || *eg + 1 >= py::len(G)) {
            if (DEBUG) py::print(py::percent(py::str("cond +%04x: no end in the compiled code"), off(W[wi])));
            return false;
        }
        const py::str n0 = mask(txt(G[*eg])), n1 = mask(txt(G[*eg + 1]));
        std::optional<int64_t> ew;
        for (int64_t j = wi + 1; j < std::min<int64_t>(py::len(W) - 1, wi + 60); ++j)
            if (mask(txt(W[j])) == n0 && mask(txt(W[j + 1])) == n1) { ew = j; break; }
        if (!ew) {
            if (DEBUG) py::print(py::percent(py::str("cond +%04x: the original does not come to %s"), off(W[wi]), show_pair(n0, n1)));
            return false;
        }
        boolx::SameResult r = boolx::same(cond_lines(W, want, ws, *ew), off(W[ws]), off(W[*ew]), cond_lines(G, got, gs, *eg), off(G[gs]),
                                          off(G[*eg]), cond_bases(W, ws), cond_bases(G, gs));
        if (!r.same) {
            if (DEBUG) py::print(py::percent(py::str("boolx +%04x: %s"), off(W[ws]), r.why));
            return false;
        }
        if (r.elses) jumps.append(*r.elses);
        notes.append(py::percent(py::str("a condition worked out in full where the original jumps, from +%04x to +%04x"), off(W[ws]), off(W[*ew])));
        {
            py::list<std::tuple<int64_t, int64_t>> keep;
            for (const auto& x : amap)
                if (std::get<0>(x) <= off(W[ws])) keep.append(x);
            amap.vec() = keep.vec();
        }
        wi = *ew;
        gi = *eg;
        return true;
    };

    while (wi < py::len(W) && gi < py::len(G)) {
        const int64_t ow = off(W[wi]), sw = siz(W[wi]);
        const py::str tw = txt(W[wi]);
        const py::str mw_ = mnem(W[wi]);   // (the Python's `_`)
        const int64_t og = off(G[gi]), sg = siz(G[gi]);
        const py::str tg = txt(G[gi]);
        const py::bytes bw = want.slice(ow, ow + sw), bg = got.slice(og, og + sg);
        amap.append(std::make_tuple(ow, og));
        if (wi != 0 && gi != 0) {   // behind a call the registers hold nothing; behind a jump the same, if they did before it
            int64_t pw = want[off(W[wi - 1])], pg = got[off(G[gi - 1])];
            if (pw == pg && (pw == 0xE8 || pw == 0x9A || (mp.empty() && (in_ends(pw) || is_jcc(pw)))) && mnem(W[wi - 1]) != "table") sync = std::make_tuple(wi, gi);
        }
        const bool fxg = fixed(og, sg);
        bool same = sw == sg;
        if (same)
            for (int64_t k = 0; k < sw; ++k)
                if (!(bw[k] == bg[k] || fx.contains(og + k))) { same = false; break; }
        auto eq = [&](const std::optional<py::str>& a, const py::str& b) { return a.has_value() && (*a == b || (fxg && mask(*a) == mask(b))); };
        auto rw = rel(bw), rg = rel(bg);
        if (rw && rg && bw[0] == bg[0] && !fxg) {   // a jump or a call: where it goes is checked at the end (a call's, by build.py)
            if (bw[0] != 0xE8) jumps.append(Jump(ow + sw + std::get<1>(*rw), og + sg + std::get<1>(*rg)));
            else setmap(OMap(), ow);
            wi += 1;
            gi += 1;
            continue;
        }
        if (!same && tw != tg && mp.empty() && cond()) continue;
        // FOR with constant bounds: the later compiler still tests them before the loop (xor dx,dx / shr dx,1 / jb past the loop)
        if (!same && tw == "xor dx, dx" && wi + 2 < py::len(W) && txt(W[wi + 1]) == "shr dx, 1" && want[off(W[wi + 2])] == 0x72) {
            const WalkItem j3 = W[wi + 2];
            int64_t d = want[off(j3) + 1];
            int64_t tgt = off(j3) + 2 + (d > 127 ? d - 256 : d);
            if (tgt > off(j3)) {
                fors.append(tgt);
                OMap nw;
                for (const auto& kv : mp.items())
                    if (!(std::get<0>(kv) == "dx" || std::get<1>(kv) == "dx")) nw.set(std::get<0>(kv), std::get<1>(kv));
                setmap(nw, ow);
                notes.append(py::percent(py::str("FOR entry test on constants at +%04x"), ow));
                wi += 3;
                continue;
            }
        }
        if (tw == tg && tw.startswith("table of")) { wi += 1; gi += 1; continue; }   // (build.py compares where the entries point)
        // CASE: the later compiler takes the least value by itself (sub bx,lo / ja / jmp to its case) before the jump through the table
        if (bw == py::bytes::fromhex(py::str("7703")) && tg == "shl bx, 1" && wi != 0 && txt(W[wi - 1]).startswith("sub bx, ") && wi + 2 < py::len(W) &&
            want[off(W[wi + 1])] == 0xE9 && txt(W[wi + 2]) == "shl bx, 1") {
            const WalkItem j3 = W[wi + 1];
            fors.append(off(j3) + 3 + unpack_h(want, off(j3) + 1));
            notes.append(py::percent(py::str("CASE: least value taken first at +%04x"), ow));
            wi += 2;
            continue;
        }
        if (py::len(bw) == 2 && bw[0] == 0x76 && tg == "shl bx, 1" && wi != 0 && txt(W[wi - 1]).startswith("sub bx, ") && wi + 1 < py::len(W) && txt(W[wi + 1]) == "shl bx, 1") {
            fors.append(ow + 2 + std::get<1>(*rw));
            notes.append(py::percent(py::str("CASE: least value taken first at +%04x"), ow));
            wi += 1;
            continue;   // ... or jbe case-lo, when it is near
        }
        // after a DISPOSE through the library DI is lost: the original loads the WITH pointer again, the in-line code still has it
        if (reload == wi && !same && tw.startswith("mov di, word ptr [bp") && wi + 1 < py::len(W) && txt(W[wi + 1]) == tg) {
            notes.append(py::percent(py::str("WITH pointer loaded again after DISPOSE at +%04x"), ow));
            wi += 1;
            continue;
        }
        // the frame: the two compilers do not need the same number of temporaries, so the frame's size may differ,
        // and a temporary may lie elsewhere in it - but one of the original's is always the same one of the compiled
        if (mw_ == "hdr" && mnem(G[gi]) == "hdr" && py::len(bw) == 2 && bw != bg) {   // (MS-Pascal 3.x: the size alone)
            notes.append(py::percent(py::str("frame of %d bytes, %d as compiled here"), bw[0] | bw[1] << 8, bg[0] | bg[1] << 8));
            wi += 1;
            gi += 1;
            continue;
        }
        if (mw_ == "hdr" && mnem(G[gi]) == "hdr" && bw[0] == bg[0] && bw != bg) {
            notes.append(py::percent(py::str("frame of %d bytes, %d in the 1981 code"), bw[1] | bw[2] << 8, bg[1] | bg[2] << 8));
            wi += 1;
            gi += 1;
            continue;
        }
        auto tempeq = [&](const std::optional<py::str>& x, const py::str& y) -> bool {
            auto mw = research("\\[bp - (0x[0-9a-f]+|\\d+)\\]", x ? *x : py::str(""));
            auto mg = research("\\[bp - (0x[0-9a-f]+|\\d+)\\]", y);
            if (mw && mg && x->slice(0, mw->start()) + x->slice_from(mw->end()) == y.slice(0, mg->start()) + y.slice_from(mg->end())) {
                int64_t a = int0(mw->group(1)), b = int0(mg->group(1));
                bool all = true;
                for (const auto& kv : temps.items())
                    if (std::get<0>(kv) != a && !((std::get<0>(kv) < a) == (std::get<1>(kv) < b))) { all = false; break; }
                if (a != b && temps.get(a, b) == b && rtemps.get(b, a) == a && all) {
                    temps.set(a, b);
                    rtemps.set(b, a);
                    return true;   // (in the same order: a parameter is not taken for a local)
                }
            }
            return false;
        };
        if (!same && tempeq(!mp.empty() ? rename(tw, mp) : std::optional<py::str>(tw), tg)) { wi += 1; gi += 1; continue; }
        // a shift count: the compiler of 1981 loads it into CX before the value to be shifted, the later one after
        // it (the value is in AX by then). Between the two places neither may touch CX.
        auto mc = fullmatch("mov cx, (0x[0-9a-f]+|\\d+)", tg);
        if (!count && mc && tw != tg && !same && !tw.endswith(py::str(", ") + mc->group(1))) {
            bool shifted = false;
            for (const WalkItem& g : G.slice(gi + 1, gi + 8))
                if (research("\\b(shl|shr|sal|sar) \\w+, cl", txt(g))) { shifted = true; break; }
            if (shifted) {
                count = std::make_tuple(mc->group(1), ow);
                gi += 1;
                continue;
            }
        }
        if (count) {
            if (tw == py::str("mov cx, ") + std::get<0>(*count) && tg != tw) {
                notes.append(py::percent(py::str("shift count loaded after the value at +%04x"), std::get<1>(*count)));
                count.reset();
                wi += 1;
                continue;
            }
            if (research("\\b(cx|cl|ch)\\b", tw))
                return fail(py::percent(py::str("CX used before the shift count is loaded, at +%04x: original %s, compiled %s"), ow, tw, tg));
        }
        // before MUL the compiler of 1981 saves DX in a temporary it never reads again; the later one does not
        {
            auto m = fullmatch("mov word ptr (\\[bp - \\w+\\]), dx", tg);
            if (m && tw != tg && gi + 1 < py::len(G) && txt(G[gi + 1]).startswith("mul ")) {
                int64_t uses = 0;
                for (const WalkItem& g : G)
                    if (txt(g).contains(m->group(1))) uses += 1;
                if (uses == 1) {
                    notes.append(py::percent(py::str("DX not saved before MUL at +%04x"), ow));
                    gi += 1;
                    continue;
                }
            }
        }
        if (mp.empty() && same) { wi += 1; gi += 1; continue; }
        auto xw = fullmatch("xchg (" + R16S + "), (" + R16S + ")", tw);
        auto xg = fullmatch("xchg (" + R16S + "), (" + R16S + ")", tg);
        auto lw = fullmatch("mov (" + R16S + "), (.+)", tw);
        auto lg = fullmatch("mov (" + R16S + "), (.+)", tg);
        if (!mp.empty()) {
            // what the compiled code has for the original's register x; None: nothing
            auto old = [&](const py::str& x) -> std::optional<py::str> {
                if (mp.contains(x)) return mp.at(x);
                for (const py::str& v : mp.values())
                    if (v == x) return std::nullopt;
                return x;
            };
            if (same && tw.startswith("lcall")) { setmap(OMap(), ow); wi += 1; gi += 1; continue; }
            if (eq(rename(tw, mp), tg)) { wi += 1; gi += 1; continue; }   // the same instruction, the registers renamed
            auto m = fullmatch("xor (ah|ch|dh|bh), \\1", tw);   // the high byte cleared: SI and DI have none
            if (m && tg == py::percent(py::str("and %s, 0xff"), mp.get(m->group(1)[0] + py::str("x"), py::str("?")))) { wi += 1; gi += 1; continue; }
            m = fullmatch("mov (" + R16S + "), ax", tw);   // the value put aside, AX wanted for the next term
            if (m && tw != tg && wi + 1 < py::len(W) && txt(W[wi + 1]).startswith("mov ax, ") && old(py::str("ax"))) {
                OMap nw = omap(mp);
                nw.set(m->group(1), old(py::str("ax")));
                setmap(nw, ow);
                wi += 1;
                continue;
            }
            auto ma = fullmatch("add (" + R16S + "), (" + R16S + ")", tw);
            auto mb = fullmatch("add (" + R16S + "), (" + R16S + ")", tg);
            if (ma && mb && old(ma->group(1)) == mb->group(2) && old(ma->group(2)) == mb->group(1)) {   // a sum, the terms the other way round
                OMap nw;
                for (const auto& kv : mp.items()) {
                    const py::str &k = std::get<0>(kv), &v = std::get<1>(kv);
                    if (!(k == ma->group(1) || k == ma->group(2)) && !(v == mb->group(1) || v == mb->group(2))) nw.set(k, v);
                }
                nw.set(ma->group(1), mb->group(1));
                setmap(nw, ow);
                wi += 1;
                gi += 1;
                continue;
            }
            m = fullmatch("mov (" + R16S + "), (0x[0-9a-f]+|\\d+)", tw);   // a constant loaded before a store the compiled code must do first,
            if (m && wi + 1 < py::len(W) && gi + 1 < py::len(G) && txt(G[gi + 1]) == tw &&
                (eq(rename(txt(W[wi + 1]), mp), tg) || tempeq(rename(txt(W[wi + 1]), mp), tg)) &&
                rematch("mov (byte|word) ptr \\[bp", tg)) {   // because it has the value in the register the constant goes to
                OMap nw;
                for (const auto& kv : mp.items())
                    if (std::get<1>(kv) != m->group(1) && std::get<0>(kv) != m->group(1)) nw.set(std::get<0>(kv), std::get<1>(kv));
                setmap(nw, ow);
                wi += 2;
                gi += 2;
                continue;
            }
            if (xw) {   // an exchange the compiled code has not: the values change places in the original only
                py::str a = xw->group(1), b = xw->group(2);
                OMap nw = omap(mp);
                std::optional<py::str> oa = old(a), ob = old(b);
                nw.set(a, ob);
                nw.set(b, oa);
                setmap(nw, ow);
                wi += 1;
                continue;
            }
            if (xg) {   // ... or one only the compiled code has
                py::str a = xg->group(1), b = xg->group(2);
                OMap nw;
                for (const py::str& k : FAM.keys()) {
                    std::optional<py::str> o = old(k);
                    // {a: b, b: a}.get(old(k), old(k))
                    if (o && *o == a) nw.set(k, b);
                    else if (o && *o == b) nw.set(k, a);
                    else nw.set(k, o);
                }
                setmap(nw, ow);
                gi += 1;
                continue;
            }
            if (lw && lg && eq(rename(lw->group(2), mp), lg->group(2))) {   // the same value loaded, into different registers
                OMap nw;
                for (const auto& kv : mp.items())
                    if (std::get<1>(kv) != lg->group(1)) nw.set(std::get<0>(kv), std::get<1>(kv));
                nw.set(lw->group(1), lg->group(1));
                setmap(nw, ow);
                wi += 1;
                gi += 1;
                continue;
            }
            auto m3 = fullmatch("(?:pop|(?:xor|sub) (" + R16S + "),|lea|mov) (" + R16S + ")(?:, (.+))?", tw);
            if (same && m3) {   // a register set anew by both
                std::optional<py::str> g1 = m3->group_opt(1), g3 = m3->group_opt(3);
                py::str r = m3->group(2);
                if ((!g1 || *g1 == r) && (g1.has_value() || !g3 || eq(rename(*g3, mp), *g3))) {
                    OMap nw;
                    for (const auto& kv : mp.items())
                        if (std::get<0>(kv) != r && std::get<1>(kv) != r) nw.set(std::get<0>(kv), std::get<1>(kv));
                    setmap(nw, ow);
                    wi += 1;
                    gi += 1;
                    continue;
                }
            }
            if (stretch()) continue;
            return fail(py::percent(py::str("with %s, at +%04x: original %s, compiled %s"), renaming_text(mp), ow, tw, tg));
        }
        // a value loaded into AX where the compiled code loads it into another register
        if (lw && lg && lw->group(1) != lg->group(1) && tw.contains(py::str("ptr")) && eq(lw->group(2), lg->group(2)) &&
            (lw->group(1) == "ax" || lg->group(1) == "ax" ||
             ((lw->group(1) == "cx" || lw->group(1) == "dx" || lw->group(1) == "bx") && (lg->group(1) == "cx" || lg->group(1) == "dx" || lg->group(1) == "bx")))) {
            OMap nw;
            nw.set(lw->group(1), lg->group(1));
            setmap(nw, ow);
            wi += 1;
            gi += 1;
            continue;
        }
        // a value moved out of AX, which the next computation wants (the compiled code leaves it there and computes elsewhere)
        {
            auto m = fullmatch("mov (cx|dx|bx|si|di), ax", tw);
            if (m && tw != tg && wi + 1 < py::len(W) && txt(W[wi + 1]).startswith("mov ax, ")) {
                OMap nw;
                nw.set(m->group(1), py::str("ax"));
                setmap(nw, ow);
                wi += 1;
                continue;
            }
        }
        // ... or the compiled code moves the value out of AX to work on it (an index), which the original does in AX
        if (xg && (xg->group(1) == "ax" || xg->group(2) == "ax") && tw != tg && !same) {
            std::vector<py::str> rs;
            for (const py::str& x : {xg->group(1), xg->group(2)})
                if (x != "ax") rs.push_back(x);
            if (rs.empty()) throw py::IndexError("list index out of range");
            OMap nw;
            nw.set(py::str("ax"), rs[0]);
            nw.set(rs[0], py::str("ax"));
            setmap(nw, ow);
            gi += 1;
            continue;
        }
        // a register's value moved into AX to be worked on there
        if (xw && (xw->group(1) == "ax" || xw->group(2) == "ax")) {
            std::vector<py::str> rs;
            for (const py::str& x : {xw->group(1), xw->group(2)})
                if (x != "ax") rs.push_back(x);
            if (rs.empty()) throw py::IndexError("list index out of range");
            OMap nw;
            nw.set(py::str("ax"), rs[0]);
            nw.set(rs[0], py::str("ax"));
            setmap(nw, ow);
            wi += 1;
            continue;
        }
        // a pointer still in DX: the later compiler takes it from there (mov di,dx), the one of 1981 loads the variable again -
        // with the very instructions that loaded DX a moment ago, and no call between
        if (tw == "mov di, dx" && mp.empty()) {
            bool done = false;
            for (int64_t k : {1, 2, 3}) {
                if (gi + k > py::len(G) || !txt(G[gi + k - 1]).startswith("mov di, ")) continue;
                py::str x = txt(G[gi + k - 1]).slice_from(8);
                for (int64_t j = gi - 1; j > std::max<int64_t>(gi - 10, k - 2); --j) {
                    py::list<py::str> a, b;
                    for (const WalkItem& g : G.slice(gi, gi + k - 1)) a.append(txt(g));
                    for (const WalkItem& g : G.slice(j - (k - 1), j)) b.append(txt(g));
                    bool anycall = false;
                    for (const WalkItem& g : G.slice(j, gi))
                        if (txt(g).contains(py::str("call"))) { anycall = true; break; }
                    if (txt(G[j]) == py::str("mov dx, ") + x && a == b && !anycall) {
                        notes.append(py::percent(py::str("pointer taken from DX at +%04x"), ow));
                        wi += 1;
                        gi += k;
                        done = true;
                        break;
                    }
                }
                if (done) break;
            }
            if (done) continue;
        }
        // ... and the other way: the compiler of 1981 still has an outer procedure's frame in SI and uses it; the later one
        // loads it again, into DI, with the instructions that loaded SI
        if (tw.startswith("mov di, word ptr [bp") && mp.empty() && research("\\bsi\\b", tg)) {
            bool done = false;
            for (int64_t k : {1, 2}) {
                if (wi + k >= py::len(W)) break;
                py::list<py::str> loads;
                for (const WalkItem& w : W.slice(wi, wi + k)) loads.append(pat("\\bdi\\b").sub(py::str("si"), txt(w)));
                bool all_mov = true;
                for (const py::str& l : loads)
                    if (!l.startswith("mov si, ")) { all_mov = false; break; }
                if (pat("\\bdi\\b").sub(py::str("si"), txt(W[wi + k])) != tg || !all_mov) continue;
                for (int64_t j = gi - k; j > std::max<int64_t>(gi - 14, -1); --j) {
                    py::list<py::str> seg;
                    for (const WalkItem& g : G.slice(j, j + k)) seg.append(txt(g));
                    bool bad = false;
                    for (const WalkItem& g : G.slice(j + k, gi))
                        if (txt(g).contains(py::str("call")) || rematch("(mov|lea|pop|xchg) si,", txt(g))) { bad = true; break; }
                    if (seg == loads && !bad) {
                        notes.append(py::percent(py::str("outer frame loaded again at +%04x"), ow));
                        wi += k + 1;
                        gi += 1;
                        done = true;
                        break;
                    }
                }
                if (done) break;
            }
            if (done) continue;
        }
        // an outer procedure's variable passed by reference right after it was assigned: the compiler of 1981 pushes the
        // variable itself there (push [di-n], its error: seen with a call put between, which makes it right); the later one
        // loads the frame again and pushes the address
        {
            auto m = fullmatch("push word ptr \\[di ([-+] \\w+)\\]", tg);
            if (m && tw == "mov di, word ptr [bp]" && wi + 2 < py::len(W) && txt(W[wi + 1]) == py::str("lea dx, [di ") + m->group(1) + py::str("]") &&
                txt(W[wi + 2]) == "push dx" && gi != 0 &&
                fullmatch("mov (byte|word) ptr \\[di " + py::re::escape(m->group(1)).utf8() + "\\], \\w+", txt(G[gi - 1]))) {
                notes.append(py::percent(py::str("address of the variable pushed, where the 1981 compiler pushes its value (an error of that compiler), at +%04x"), ow));
                wi += 3;
                gi += 1;
                continue;
            }
        }
        // DISPOSE
        if (gi + 2 < py::len(G) && txt(G[gi + 1]) == "mov si, word ptr [si]" && txt(G[gi + 2]) == "or byte ptr [si - 2], 1") {
            std::optional<int64_t> k;
            if (tg.startswith("lea si, ") && eq(tw, py::str("lea dx, ") + tg.slice_from(8)) && wi + 2 < py::len(W) && txt(W[wi + 1]) == "push dx") k = 2;
            else if (tg.startswith("mov si, word ptr ") && eq(tw, py::str("push word ptr ") + tg.slice_from(17))) k = 1;
            else if (tg == "mov si, di" && tw == "mov dx, di" && wi + 2 < py::len(W) && txt(W[wi + 1]) == "push di") k = 2;
            else if (!tg.contains(py::str("ptr")) && tg.startswith("mov si, ") && tg.slice(8, 9).isdigit() && eq(tw, py::str("mov dx, ") + tg.slice_from(8)) &&
                     wi + 2 < py::len(W) && txt(W[wi + 1]) == "push dx")
                k = 2;   // a variable by its address
            if (k && wi + *k < py::len(W) && want.slice(off(W[wi + *k]), off(W[wi + *k]) + 5) == DISPOSE) {
                notes.append(py::percent(py::str("DISPOSE through the library at +%04x"), ow));
                wi += *k + 1;
                gi += 3;
                reload = wi;
                continue;
            }
        }
        // DISPOSE of a variable of an outer procedure: mov di,[bp] / lea dx,[di-n] / push dx against mov si,[bp] / lea si,[si-n]
        if (gi + 3 < py::len(G) && wi + 3 < py::len(W) && txt(G[gi + 2]) == "mov si, word ptr [si]" && txt(G[gi + 3]) == "or byte ptr [si - 2], 1") {
            auto m1 = fullmatch("mov si, (word ptr \\[.+\\])", tg);
            auto m2 = fullmatch("lea si, \\[si ([-+] \\w+)\\]", txt(G[gi + 1]));
            if (m1 && m2 && tw == py::str("mov di, ") + m1->group(1) && txt(W[wi + 1]) == py::str("lea dx, [di ") + m2->group(1) + py::str("]") &&
                txt(W[wi + 2]) == "push dx" && want.slice(off(W[wi + 3]), off(W[wi + 3]) + 5) == DISPOSE) {
                notes.append(py::percent(py::str("DISPOSE through the library at +%04x"), ow));
                wi += 4;
                gi += 4;
                reload = wi;
                continue;
            }
        }
        // a conditional jump one of the two had to make long
        if (rw && 0x70 <= bw[0] && bw[0] <= 0x7F && bg[0] == (bw[0] ^ 1) && py::len(bg) == 2 && bg[1] == 3 && gi + 1 < py::len(G) && got[off(G[gi + 1])] == 0xE9) {
            const WalkItem g2 = G[gi + 1];
            jumps.append(Jump(ow + sw + std::get<1>(*rw), off(g2) + 3 + unpack_h(got, off(g2) + 1)));
            notes.append(py::percent(py::str("long jump at +%04x"), ow));
            wi += 1;
            gi += 2;
            continue;
        }
        if (rg && 0x70 <= bg[0] && bg[0] <= 0x7F && bw[0] == (bg[0] ^ 1) && py::len(bw) == 2 && bw[1] == 3 && wi + 1 < py::len(W) && want[off(W[wi + 1])] == 0xE9) {
            const WalkItem w2 = W[wi + 1];
            jumps.append(Jump(off(w2) + 3 + unpack_h(want, off(w2) + 1), og + sg + std::get<1>(*rg)));
            notes.append(py::percent(py::str("long jump at +%04x"), ow));
            wi += 2;
            gi += 1;
            continue;
        }
        if (stretch()) continue;
        return fail(py::percent(py::str("no pattern at +%04x: original %s, compiled %s"), ow, tw, tg));
    }
    if (count) return fail(py::percent(py::str("a shift count the original never loads, from +%04x"), std::get<1>(*count)));
    if (wi != py::len(W) || gi != py::len(G))
        return fail(py::percent(py::str("the two do not end together: %d of %d, %d of %d instructions"), wi, py::len(W), gi, py::len(G)));
    setmap(OMap(), py::len(want));
    std::map<int64_t, int64_t> at;   // dict(amap): the last of equal keys wins
    for (const auto& x : amap) at[std::get<0>(x)] = std::get<1>(x);
    at[py::len(want)] = py::len(got);
    for (int64_t t : fors)
        if (!at.count(t)) return fail(py::percent(py::str("the FOR entry test's jump to +%04x goes into an instruction"), t));
    for (const Jump& j : jumps) {
        int64_t tw_ = std::get<0>(j), tg_ = std::get<1>(j);
        if (0 <= tw_ && tw_ <= py::len(want)) {
            auto it = at.find(tw_);
            if (it == at.end() || it->second != tg_)
                return fail(py::percent(py::str("a jump to +%04x goes to +%04x in the compiled code, not +%s"), tw_, tg_,
                                        it != at.end() ? hex04(it->second) : py::str("?")));
        }
    }
    if (!temps.empty()) {
        auto items = temps.items();
        std::sort(items.begin(), items.end());
        py::str s;
        bool first = true;
        for (const auto& kv : items) {
            s += py::str(first ? "" : ", ") + py::percent(py::str("-%X for -%X"), std::get<0>(kv), std::get<1>(kv));
            first = false;
        }
        notes.append(py::percent(py::str("temporaries elsewhere in the frame (%s)"), s));
    }
    {
        // AMAP = sorted(set(amap)); GWALK[:] = [g[0] for g in G]
        std::set<std::tuple<int64_t, int64_t>> uniq(amap.vec().begin(), amap.vec().end());
        AMAP = py::list<std::tuple<int64_t, int64_t>>();
        for (const auto& x : uniq) AMAP.append(x);
        GWALK.clear();
        for (const WalkItem& g : G) GWALK.append(off(g));
    }
    if (!notes.empty()) return Ret(true, notes);
    return fail(py::str("differs only in jump displacements?"));
}

std::tuple<bool, py::list<py::str>> equivalent3(const py::bytes& want, const py::bytes& got, const py::set<int64_t>& fx) {
    using Ret = std::tuple<bool, py::list<py::str>>;
    auto fail = [](const py::str& why) { return Ret(false, py::list<py::str>{why}); };
    const py::bytes E8 = py::bytes::fromhex(py::str("e8"));
    if (!(want.slice(0, 1) == got.slice(0, 1) && got.slice(0, 1) == E8) ||
        !(((got[4] | got[5] << 8) - (want[3] | want[4] << 8)) == 0 || ((got[4] | got[5] << 8) - (want[3] | want[4] << 8)) == 4))
        return fail(py::str("the entry is not call helper / dw frame"));
    Walk W, G;
    for (const WalkItem& x : cmpp::walk(want.slice_from(5), 0, false)) W.append(WalkItem(5 + off(x), siz(x), txt(x), mnem(x)));
    for (const WalkItem& x : cmpp::walk(got.slice_from(6), 0, false)) G.append(WalkItem(6 + off(x), siz(x), txt(x), mnem(x)));
    py::list<py::str> notes{py::str("entry helper of MS-Pascal 3.0x")};
    py::list<std::tuple<int64_t, int64_t>> amap{std::make_tuple<int64_t, int64_t>(0, 0)};
    py::list<Jump> jumps;
    int64_t wi = 0, gi = 0;
    auto local = [](const py::str& t) {
        return pat("\\[bp - (0x[0-9a-f]+|\\d+)\\]").sub_fn(
            [](const py::re::Match& m) { return py::percent(py::str("[bp - 0x%x]"), int0(m.group(1)) + 4); }, t);
    };
    auto hexed = [](const py::str& t) {
        return pat("\\[bp - (\\d+)\\]").sub_fn(
            [](const py::re::Match& m) { return py::percent(py::str("[bp - 0x%x]"), py::int_from_str(m.group(1), 10)); }, t);
    };
    // (the Python's cond() is defined here too, but never called)
    while (wi < py::len(W) && gi < py::len(G)) {
        const int64_t ow = off(W[wi]), sw = siz(W[wi]);
        const py::str tw = txt(W[wi]);
        const int64_t og = off(G[gi]), sg = siz(G[gi]);
        const py::str tg = txt(G[gi]);
        const py::bytes bw = want.slice(ow, ow + sw), bg = got.slice(og, og + sg);
        amap.append(std::make_tuple(ow, og));
        bool fxg = false;
        for (int64_t k = 0; k < sg; ++k)
            if (fx.contains(og + k)) { fxg = true; break; }
        bool same = sw == sg;
        if (same)
            for (int64_t k = 0; k < sw; ++k)
                if (!(bw[k] == bg[k] || fx.contains(og + k))) { same = false; break; }
        auto eq = [&](const py::str& a, const py::str& b) { return a == b || (fxg && mask(a) == mask(b)); };
        auto rw = rel(bw), rg = rel(bg);
        if (rw && rg && bw[0] == bg[0] && !fxg) {
            if (bw[0] != 0xE8) jumps.append(Jump(ow + sw + std::get<1>(*rw), og + sg + std::get<1>(*rg)));
            wi += 1;
            gi += 1;
            continue;
        }
        if (same) { wi += 1; gi += 1; continue; }
        if (local(tw) == hexed(tg)) { wi += 1; gi += 1; continue; }
        if (tw == "mov sp, bp" && tg == "lea sp, [bp - 4]") { wi += 1; gi += 1; continue; }
        auto m = fullmatch("add di, (\\w+)", tw);
        if (m && wi + 1 < py::len(W) && fxg && mask(txt(W[wi + 1]).replace(py::str("[di]"), py::str("[di + N]"))) == mask(tg)) {
            notes.append(py::percent(py::str("indexed store at +%04x"), ow));
            wi += 2;
            gi += 1;
            continue;
        }
        if (rematch("mov dx, word ptr \\[", tw) && eq(tw.replace(py::str("mov dx,"), py::str("mov ax,")), tg)) {
            notes.append(py::percent(py::str("byte moved through DX at +%04x"), ow));
            wi += 1;
            gi += 1;
            continue;
        }
        if (rematch("mov byte ptr \\[.*\\], dl$", tw) && eq(tw.slice(0, -2) + py::str("al"), tg)) { wi += 1; gi += 1; continue; }
        if (tw.startswith("mov dx, ") && wi + 2 < py::len(W) && txt(W[wi + 1]) == "push ds" && txt(W[wi + 2]) == "push dx" && gi + 4 < py::len(G) &&
            mask(tg) == "mov bx, N") {
            py::list<py::str> tail;
            for (const WalkItem& g : G.slice(gi + 1, gi + 5)) tail.append(txt(g));
            if (tail == py::list<py::str>{py::str("mov dx, ds"), py::str("mov es, dx"), py::str("push es"), py::str("push bx")}) {
                notes.append(py::percent(py::str("far address pushed at +%04x"), ow));
                wi += 3;
                gi += 5;
                continue;
            }
        }
        if (tw.startswith("or al, byte ptr [bp") && local(tw).replace(py::str("or al, byte ptr"), py::str("or ax, word ptr")) == hexed(tg)) { wi += 1; gi += 1; continue; }
        return fail(py::percent(py::str("no pattern at +%04x: original %s, compiled %s"), ow, tw, tg));
    }
    if (wi != py::len(W) || gi != py::len(G)) return fail(py::str("the two do not end together"));
    std::map<int64_t, int64_t> at;
    for (const auto& x : amap) at[std::get<0>(x)] = std::get<1>(x);
    at[py::len(want)] = py::len(got);
    for (const Jump& j : jumps) {
        auto it = at.find(std::get<0>(j));
        if (it == at.end() || it->second != std::get<1>(j))
            return fail(py::percent(py::str("a jump to +%04x goes to +%04x in the compiled code"), std::get<0>(j), std::get<1>(j)));
    }
    {
        std::set<std::tuple<int64_t, int64_t>> uniq(amap.vec().begin(), amap.vec().end());
        AMAP = py::list<std::tuple<int64_t, int64_t>>();
        for (const auto& x : uniq) AMAP.append(x);
        GWALK.clear();
        for (const WalkItem& g : G) GWALK.append(off(g));
    }
    return Ret(true, notes);
}

}  // namespace codebreaker::exact::habits::newcomp
