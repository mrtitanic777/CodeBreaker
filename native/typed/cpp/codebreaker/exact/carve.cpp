// codebreaker/exact/carve.cpp -- typed C++ of codebreaker.exact.carve (see carve.hpp).

#include "codebreaker/exact/carve.hpp"

#include <algorithm>

#include "codebreaker/exact/x86.hpp"

namespace codebreaker::exact::carve {

namespace {

using py::bytes;
using py::str;
namespace re = py::re;

int64_t rel(const bytes& img, int64_t k) {
    int64_t v = py::unpack_from1<int64_t>("<H", img, k);
    return v >= 0x8000 ? v - 0x10000 : v;      // struct "<h"
}

bool is_hexdigit(char32_t c) { return (c >= U'0' && c <= U'9') || (c >= U'a' && c <= U'f') || (c >= U'A' && c <= U'F'); }

// near calls' targets, from the code disassembled from each known start (until a new one is not found)
void calls_(const bytes& img, py::set<int64_t>& starts, const std::function<bool(int64_t)>& inside, project::Project& prj,
            py::dict<int64_t, str>& why, int64_t hi) {
    const bool emu = project::profile_parts(*prj.toolchain()).carve.emulator_ints;
    py::set<int64_t> done;
    while (true) {
        py::list<int64_t> todo = py::sorted(starts - done);
        if (todo.empty()) return;
        py::list<int64_t> allst = py::sorted(starts);
        for (int64_t a : todo) {
            done.add(a);
            int64_t nxt = hi;
            for (int64_t x : allst) {
                if (x > a && x < nxt) nxt = x;
            }
            for (const x86::Insn& i : x86::body(img, a, nxt, emu, prj.code_base(a))) {
                if (i.mnemonic == "call" && i.size == 3 && i.bytes[0] == 0xE8) {
                    int64_t base = prj.code_base(i.address);
                    int64_t t = base + ((i.address + 3 + rel(img, i.address + 1) - base) & 0xFFFF);
                    if (inside(t) && !starts.contains(t)) {
                        starts.add(t);
                        why.setdefault(t, py::percent(str("called from %05x"), i.address));
                    }
                }
            }
        }
    }
}

}  // namespace

std::tuple<int64_t, int64_t, py::list<std::tuple<int64_t, int64_t>>> region(project::Project& prj, const library::MapP& mp0) {
    library::MapP mp = mp0 ? mp0 : library::locate(*project::as_library(prj));
    int64_t hi = mp->start ? *mp->start : prj.dgroup;
    int64_t lo = project::num(prj.cfg.code_start);
    py::list<std::tuple<int64_t, int64_t>> skip;
    for (const auto* grp : {&mp->coded, &mp->lone}) {
        for (const library::PlacedP& p : *grp) {
            if (p->code && *p->code < hi) skip.append(std::make_tuple(*p->code, *p->end()));
        }
    }
    for (const auto& o : mp->objects) skip.append(std::make_tuple(std::get<0>(o), std::get<1>(o)));
    return std::make_tuple(lo, hi, py::sorted(skip));
}

str _hexrx(const str& rx) {
    std::u32string out;
    std::u32string s;
    for (char32_t c : rx.u32()) {
        if (c != U' ') s += c;
    }
    size_t k = 0;
    while (k < s.size()) {
        char32_t c = s[k];
        if (s.compare(k, 2, U"..") == 0) {
            out += U'.';
            k += 2;
        } else if (is_hexdigit(c) && k + 1 < s.size() && is_hexdigit(s[k + 1])) {
            int64_t v = py::int_parse(str(s.substr(k, 2)), 16);
            out += re::escape(str(std::u32string(1, static_cast<char32_t>(v)))).u32();
            k += 2;
        } else {
            out += c;
            k += 1;
        }
    }
    return str(out);
}

py::list<std::tuple<int64_t, int64_t, str>> functions(project::Project& prj, bool /*verbose*/) {
    const bytes& img = prj.image;
    library::MapP mp = library::locate(*project::as_library(prj));
    auto reg = region(prj, mp);
    const int64_t lo = std::get<0>(reg), hi = std::get<1>(reg);
    const py::list<std::tuple<int64_t, int64_t>> skip = std::get<2>(reg);
    const project::CarveSpec& cv = project::profile_parts(*prj.toolchain()).carve;
    py::dict<str, int64_t> names;
    for (const library::PlacedP& p : mp->coded) {
        py::list<library::MemberP> ms;
        ms.append(p->m);
        for (const auto& a : p->also) ms.append(a);
        for (const library::MemberP& m : ms) {
            for (const auto& kv : m->pubs.items()) {
                const library::Pub& pub = std::get<1>(kv);
                if (std::get<0>(pub) && *std::get<0>(pub) == std::get<0>(m->code[0])) names.setdefault(std::get<0>(kv), *p->code + std::get<1>(pub));
            }
        }
    }
    for (const auto& kv : prj.symbols().items()) {
        if (std::get<0>(std::get<1>(kv)) == "C") names.setdefault(std::get<0>(kv), std::get<1>(std::get<1>(kv)));
    }
    auto inside = [&](int64_t a) {
        if (!(lo <= a && a < hi)) return false;
        for (const auto& xy : skip) {
            if (std::get<0>(xy) <= a && a < std::get<1>(xy)) return false;
        }
        return true;
    };
    py::set<int64_t> starts;
    py::dict<int64_t, str> why;
    std::optional<int64_t> target;
    if (cv.prologue_call && !cv.prologue_call->empty() && names.contains(*cv.prologue_call)) target = names.at(*cv.prologue_call);
    // (the image from lo to hi, as latin-1 text: a byte a character)
    str text = img.slice(lo, hi).decode("latin1");
    for (const str& rx : cv.prologues) {
        re::Pattern r(_hexrx(rx), re::S);
        for (const re::Match& m : r.finditer(text)) {
            int64_t a = lo + m.start();
            if (!inside(a)) continue;
            if (r.groups() > 0 && m.group_opt(1) && target) {
                int64_t k = lo + m.start(1);
                if (((k + 2 + rel(img, k)) & 0xFFFF) != (*target & 0xFFFF)) continue;
            }
            starts.add(a);
            why.setdefault(a, str("prologue"));
        }
    }
    if (cv.frames) {
        bytes head = img.slice(0, hi);
        const bytes pat(std::string("\x55\x8b\xec", 3));
        int64_t pos = 0;
        while (true) {
            int64_t a = head.find(pat, pos);
            if (a < 0) break;
            pos = a + 3;
            if (a < lo || !inside(a)) continue;
            if (a == lo || img[a - 1] == 0xC3 || img[a - 1] == 0xCB || img[a - 1] == 0x00 || img[a - 1] == 0x90 ||
                (a >= 3 && (img[a - 3] == 0xC2 || img[a - 3] == 0xCA))) {
                starts.add(a);
                why.setdefault(a, str("frame after a return"));
            }
        }
    }
    for (const project::Num& a : prj.cfg.starts_add) starts.add(project::num(a));
    if (cv.calls) calls_(img, starts, inside, prj, why, hi);
    for (const project::Num& a : prj.cfg.starts_drop) starts.discard(project::num(a));
    py::list<int64_t> st = py::sorted(starts);
    py::set<int64_t> endset;
    for (int64_t i = 1; i < st.size(); ++i) endset.add(st[i]);
    endset.add(hi);
    for (const auto& xy : skip) {
        if (std::get<0>(xy) > lo) endset.add(std::get<0>(xy));
    }
    py::list<std::tuple<int64_t, int64_t, str>> out;
    for (int64_t a : st) {
        std::optional<int64_t> e;
        for (int64_t x : endset) {
            if (x > a && (!e || x < *e)) e = x;
        }
        if (!e) throw py::ValueError("min() arg is an empty sequence");
        int64_t z = *e;
        while (z > a + 1 && img[z - 1] == 0) --z;
        if (z < *e && (img[z - 1] == 0xC3 || img[z - 1] == 0xCB || (z - a >= 3 && (img[z - 3] == 0xC2 || img[z - 3] == 0xCA)))) e = z;   // (zeros after the return: a pad)
        out.append(std::make_tuple(a, *e - a, why.get(a, str(""))));
    }
    return out;
}

int64_t main(project::Project& prj, const py::list<str>& argv, std::ostream& out) {
    auto fs = functions(prj, argv.contains(str("-v")));
    int64_t total = 0;
    for (const auto& f : fs) {
        if (argv.contains(str("-v"))) out << py::percent(str("%05x %5d  %s"), std::get<0>(f), std::get<1>(f), std::get<2>(f)).utf8() << "\n";
        total += std::get<1>(f);
    }
    out << py::percent(str("%d functions of the program's own, %d bytes"), py::len(fs), total).utf8() << "\n";
    if (argv.contains(str("--write"))) {
        py::file f = py::open(prj.path(str("FUNCS")), str("w"), std::nullopt, std::nullopt, str("\n"));
        f.write(str("# where the program's own functions begin, and their lengths (codebreaker exact carve)\n"));
        for (const auto& x : fs) f.write(py::percent(str("%05x %d  %s\n"), std::get<0>(x), std::get<1>(x), std::get<2>(x)));
        f.close();
    }
    return 0;
}

}  // namespace codebreaker::exact::carve
