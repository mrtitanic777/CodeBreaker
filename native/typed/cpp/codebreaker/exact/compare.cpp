// codebreaker/exact/compare.cpp -- typed C++ from codebreaker.exact.compare (see compare.hpp).

#include "codebreaker/exact/compare.hpp"

#include <algorithm>

namespace codebreaker::exact::compare {

namespace {

using py::str;
using py::bytes;

// W(b, k): the unsigned little-endian word at b[k] (struct.error past the end)
int64_t W(const bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<H", b, k); }
// S(b, k): the signed one
int64_t S(const bytes& b, int64_t k) { return py::unpack_from1<int64_t>("<h", b, k); }

}  // namespace

std::tuple<py::list<int64_t>, py::list<str>> function(const Obj& o, const bytes& img, const str& name, int64_t off, int64_t ln,
                                                      int64_t at, const Project& prj, const Symbols& syms, const Symbols& learn,
                                                      const Decoder& decode, const str& text, const std::optional<Where>& where_in) {
    const bytes& code = o.seg.at(text);
    py::set<int64_t> skip;
    py::list<str> notes;
    int64_t dg = prj.dgroup;
    int64_t ls = prj.loadseg;
    bool placed = where_in.has_value();                     // (each function of the file at a place of its own)
    Where where = where_in ? *where_in : Where([&](int64_t x) { return at - off + x; });
    int64_t base = prj.code_base(at);
    Cache cache;

    auto bind = [&](const str& nm, const str& t, int64_t a) {
        if (!syms.contains(nm)) {
            syms[nm] = std::make_tuple(t, a);
            learn[nm] = std::make_tuple(t, a);
        } else if (syms.at(nm) != std::make_tuple(t, a) &&
                   !(std::get<0>(syms.at(nm)) == str("?") && std::get<1>(syms.at(nm)) == a)) {   // ("?": a place, kind unsaid)
            notes.append(py::percent(str("%s is %s %04x in SYMBOLS, here %s %04x"), nm, std::get<0>(syms.at(nm)),
                                     std::get<1>(syms.at(nm)), t, a));
        }
    };

    // the image's segment:offset at +k, as an image address
    auto far = [&](int64_t k) { return (W(img, at + k + 2) - ls) * 16 + W(img, at + k); };

    const str S_ext("ext"), S_seg("seg"), S_C("C"), S_D("D"), S_S("S");

    for (const auto& f : o.fix) {
        const str& seg = std::get<0>(f);
        int64_t fo = std::get<1>(f);
        const str& kind = std::get<2>(f);
        const Target& tgt = std::get<3>(f);
        int64_t disp = std::get<4>(f);
        if (seg != text || !(off <= fo && fo < off + ln)) continue;
        int64_t k = fo - off;
        skip.add(k);
        skip.add(k + 1);
        int64_t mine = (W(code, fo) + disp) & 0xFFFF;
        int64_t theirs = W(img, at + k);
        if (kind == str("rel")) {
            int64_t dest = base + ((at + k + 2 + theirs - base) & 0xFFFF);
            if (std::get<0>(tgt) == S_ext) {
                bind(std::get<1>(tgt), S_C, dest - mine);
            } else if (dest != where(mine)) {
                notes.append(py::percent(str("+%x: calls %05x, this file's code would be at %05x"), k, dest, where(mine)));
            }
        } else if (kind == str("ptr")) {                    // a far call or a far address: offset and segment
            skip.add(k + 2);
            skip.add(k + 3);
            int64_t dest = far(k);
            if (std::get<0>(tgt) == S_ext) {
                bind(std::get<1>(tgt), S_C, (dest - mine) & 0xFFFFF);
            } else if (std::get<1>(tgt) == text) {
                if (dest != where(mine))
                    notes.append(py::percent(str("+%x: a far place of this file's code %05x, expected %05x"), k, dest, where(mine)));
            } else if (o.seg.contains(std::get<1>(tgt)) && std::get<1>(tgt).endswith(str("_TEXT"))) {   // another code segment of the file
                std::vector<std::tuple<int64_t, str>> fs;
                for (const auto& p : o.pubs) {
                    if (std::get<1>(p) == std::get<1>(tgt) && std::get<2>(p) <= mine) fs.emplace_back(std::get<2>(p), std::get<0>(p));
                }
                std::sort(fs.begin(), fs.end(), [](const auto& a, const auto& b) {
                    if (std::get<0>(a) != std::get<0>(b)) return std::get<0>(a) < std::get<0>(b);
                    return std::get<1>(a) < std::get<1>(b);
                });
                std::optional<int64_t> exp;
                if (!fs.empty() && syms.contains(std::get<1>(fs.back())))
                    exp = std::get<1>(syms.at(std::get<1>(fs.back()))) + mine - std::get<0>(fs.back());
                if (!exp || dest != *exp) {
                    // (`exp and "%05x" % exp`: None stays None, 0 stays 0)
                    str shown = (exp && *exp != 0) ? py::percent(str("%05x"), *exp) : (exp ? str("0") : str("None"));
                    notes.append(py::percent(str("+%x: far place %05x in %s, expected %s"), k, dest, std::get<1>(tgt), shown));
                }
            } else {
                notes.append(py::percent(str("+%x: far fixup to %s"), k, py::repr(tgt)));
            }
        } else if (kind == str("base")) {                   // a segment word alone
            if (tgt == Target(S_seg, text)) {
                int64_t sb = (theirs - ls) * 16;
                if (!(sb <= at && at < sb + 0x10000))
                    notes.append(py::percent(str("+%x: segment %04x is not this code's"), k, theirs));
            } else if (std::get<0>(tgt) != S_ext) {         // (an external's: checked with the offset beside it)
                if (!prj.relocs().empty() && !prj.relocs().contains(at + k)) {   // (another segment of the file's: a word the
                    notes.append(py::percent(str("+%x: segment of %s, not a word the loader sets"), k, py::repr(tgt)));   // loader sets)
                } else {
                    bind(py::percent(str("%s@seg"), std::get<1>(tgt)), S_S, theirs);
                }
            }
        } else if (kind == str("off")) {
            if (std::get<0>(tgt) == S_ext) {
                py::list<int64_t> segw;
                for (const auto& g : o.fix) {
                    if (std::get<0>(g) == text && std::get<2>(g) == str("base") && std::get<3>(g) == tgt && off <= std::get<1>(g) &&
                        std::get<1>(g) < off + ln)
                        segw.append(W(img, at + std::get<1>(g) - off));
                }
                if (!segw.empty()) {                        // a function's address in two halves
                    bind(std::get<1>(tgt), S_C, (segw[0] - ls) * 16 + ((theirs - mine) & 0xFFFF));
                } else if (syms.contains(std::get<1>(tgt)) && std::get<0>(syms.at(std::get<1>(tgt))) == S_C) {
                    int64_t a = std::get<1>(syms.at(std::get<1>(tgt)));
                    if (((a - prj.code_base(a)) & 0xFFFF) != ((theirs - mine) & 0xFFFF)) {
                        notes.append(py::percent(str("+%x: %s is at %05x, here offset %04x"), k, std::get<1>(tgt), a,
                                                 (theirs - mine) & 0xFFFF));
                    }
                } else {
                    bind(std::get<1>(tgt), syms.contains(std::get<1>(tgt)) ? std::get<0>(syms.at(std::get<1>(tgt))) : S_D,
                         (theirs - mine) & 0xFFFF);
                }
            } else if (std::get<1>(tgt) == text) {          // a place of this file's code (a switch's table)
                if (((where(mine) - base) & 0xFFFF) != theirs)
                    notes.append(py::percent(str("+%x: the place %04x, this file's code would be at %04x"), k, theirs,
                                             (where(mine) - base) & 0xFFFF));
            } else {                                        // the file's own data: what the word points at must be there
                std::vector<const Fixup*> fs;
                for (const auto& g : o.fix) {
                    if (std::get<0>(g) == std::get<1>(tgt) && std::get<1>(g) == mine && std::get<2>(g) == str("base") &&
                        std::get<0>(std::get<3>(g)) == S_ext)
                        fs.push_back(&g);
                }
                if (!fs.empty()) {                          // the segment of a far item, kept among the constants
                    str segname = py::percent(str("%s@seg"), std::get<1>(std::get<3>(*fs[0])));
                    if (prj.relocs().empty()) {             // (an image without its relocation table: EXEPACK's)
                        bind(segname, S_S, W(img, dg + theirs));
                    } else if (!prj.relocs().contains(dg + theirs)) {
                        notes.append(py::percent(str("+%x: DS:%04x is not a segment the loader sets"), k, theirs));
                    } else {
                        bind(segname, S_S, W(img, dg + theirs));
                    }
                    continue;
                }
                bytes d = o.seg.get(std::get<1>(tgt), bytes());
                int64_t e = mine;
                while (e < d.size() && d[e] != 0) e++;
                bytes s = (std::get<1>(tgt) == str("CONST") || std::get<1>(tgt) == str("_DATA")) ? d.slice(mine, e + 1) : bytes();
                py::set<int64_t> holes;
                for (const auto& g : o.fix) {
                    if (std::get<0>(g) != std::get<1>(tgt)) continue;
                    int64_t width = std::get<2>(g) == str("ptr") ? 4 : 2;
                    for (int64_t x = 0; x < width; x++) holes.add(std::get<1>(g) + x);
                }
                bool differs = false;
                if (s.size() != 0) {
                    differs = dg + theirs + s.size() > img.size();
                    for (int64_t x = 0; !differs && x < s.size(); x++) {
                        if (!holes.contains(mine + x) && img[dg + theirs + x] != s[x]) differs = true;
                    }
                }
                if (s.size() != 0 && differs) {
                    notes.append(py::percent(str("+%x: %s+%x (%r) is not at DS:%04x (%r)"), k, std::get<1>(tgt), mine,
                                             s.slice(0, 24), theirs,
                                             bytes(img.slice(dg + theirs, dg + theirs + std::min<int64_t>(s.size(), 24)))));
                } else if (s.size() > 2 && (mine == 0 || d[mine - 1] == 0) && theirs != 0 && 0x20 <= img[dg + theirs - 1] &&
                           img[dg + theirs - 1] < 0x7F) {
                    notes.append(py::percent(str("? +%x: %s+%x (%r) is the end of another string at DS:%04x - a number, not a string?"),
                                             k, std::get<1>(tgt), mine, s.slice(0, 24), theirs));
                }
                // (through a register the word may be an array's origin - its start less its first index - which
                // can be another item's place: such a word is held only to the others of its kind)
                bind(py::percent(str("%s:%s+%x%s"), name, std::get<1>(tgt), mine, _indexed(code, off, ln, fo, cache, decode) ? "[]" : ""),
                     S_D, theirs);
            }
        } else {
            notes.append(py::percent(str("+%x: fixup %s to %s"), k, kind, py::repr(tgt)));
        }
    }
    if (placed && where(off) == at) _calls_within(code, img, off, ln, at, base, where, skip, decode);
    py::list<int64_t> bad;
    for (int64_t k = 0; k < ln; k++) {
        if (!skip.contains(k) && (at + k >= img.size() || img[at + k] != code[off + k])) bad.append(k);
    }
    std::optional<str> shift = _shift(code, img, off, at, bad);
    if (shift) notes.append(*shift);
    return std::make_tuple(bad, notes);
}

// the instruction holding the word at fo addresses memory through a base or index register
bool _indexed(const bytes& code, int64_t off, int64_t ln, int64_t fo, Cache& cache, const Decoder& decode) {
    if (!cache.ins) {
        py::list<std::tuple<int64_t, int64_t, str>> ins;
        for (const auto& i : decode(code.slice(off, off + ln), off)) ins.append(std::make_tuple(i.address, i.size, i.op_str));
        cache.ins = ins;
    }
    for (const auto& [a, sz, ops] : *cache.ins) {
        if (a <= fo && fo < a + sz) {
            int64_t m = ops.find(str("["));
            if (m < 0) return false;
            str inside = ops.slice(m, ops.find(str("]"), m));
            for (const char* r : {"bx", "bp", "si", "di"})
                if (inside.contains(str(r))) return true;
            return false;
        }
    }
    return false;
}

// bytes that differ alone (an immediate) and each by the same amount: a constant or an enumeration with
// members more or fewer in the program's source than in this one
std::optional<str> _shift(const bytes& code, const bytes& img, int64_t off, int64_t at, const py::list<int64_t>& bad) {
    py::list<int64_t> lone;
    for (int64_t k : bad) {
        if (!bad.contains(k - 1) && !bad.contains(k + 1) && at + k < img.size()) lone.append(k);
    }
    py::set<int64_t> ds;
    for (int64_t k : lone) ds.add((img[at + k] - code[off + k]) & 0xFF);
    if (!lone.empty() && ds.size() == 1 && (lone.size() >= 2 || lone == bad)) {
        int64_t d = *ds.begin();   // (the only member: set.pop())
        py::list<str> shown;
        for (int64_t k : lone.slice(0, 6)) shown.append(py::percent(str("+%x"), k));
        return py::percent(str("? %d value%s %s%d in the program (%s) - a constant, or an enumeration with members more or fewer before them"),
                           lone.size(), lone.size() > 1 ? "s each" : "", d < 0x80 ? "+" : "-", d < 0x80 ? d : 0x100 - d,
                           py::join(str(", "), shown));
    }
    return std::nullopt;
}

// near calls and jumps from the function to another of its file have no fixup (the compiler wrote them):
// with the file's functions at places of their own, each must reach its callee's place
void _calls_within(const bytes& code, const bytes& img, int64_t off, int64_t ln, int64_t at, int64_t base, const Where& where,
                   const py::set<int64_t>& skip, const Decoder& decode) {
    for (const auto& i : decode(code.slice(off, off + ln), off)) {
        if ((i.bytes[0] == 0xE8 || i.bytes[0] == 0xE9) && i.size == 3 && !skip.contains(i.address - off + 1)) {
            int64_t k = i.address - off + 1;
            int64_t mine = (i.address + 3 + S(i.bytes, 1)) & 0xFFFF;
            if (off <= mine && mine < off + ln) continue;
            if (at + k + 2 <= img.size() && base + ((at + k + 2 + S(img, at + k) - base) & 0xFFFF) == where(mine)) {
                skip.add(k);
                skip.add(k + 1);
            }
        }
    }
    // (and a call the disassembly does not see - data after a call, as MS-Pascal's entry helper has it, puts it
    // out of step: an E8 on both sides whose targets are the same place)
    for (int64_t k = 1; k < ln - 1; k++) {
        if (skip.contains(k) || code[off + k - 1] != 0xE8 || at + k + 1 >= img.size() || img[at + k - 1] != 0xE8) continue;
        int64_t mine = (off + k + 2 + S(code, off + k)) & 0xFFFF;
        if (off <= mine && mine < off + ln) continue;
        int64_t w = where(mine);
        if (w >= 0 && base + ((at + k + 2 + S(img, at + k) - base) & 0xFFFF) == w) {
            skip.add(k);
            skip.add(k + 1);
        }
    }
}

bool identical(const py::list<int64_t>& bad, const py::list<str>& notes) {
    if (!bad.empty()) return false;
    for (const auto& n : notes)
        if (!n.startswith(str("?"))) return false;
    return true;
}

}  // namespace codebreaker::exact::compare
