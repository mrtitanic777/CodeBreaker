// codebreaker/exact/standin.cpp -- typed C++ of codebreaker.exact.standin (see standin.hpp).

#include "codebreaker/exact/standin.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <string>
#include <vector>

#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/x86.hpp"

namespace codebreaker::exact::standin {

namespace {

using py::bytes;
using py::str;
namespace omf = codebreaker::formats::omf;

// r[q] with Python's IndexError
int64_t at_(const bytes& r, int64_t q) {
    if (q < 0 || q >= r.size()) {
        throw py::IndexError("index out of range");
    }
    return static_cast<unsigned char>(r.raw()[static_cast<size_t>(q)]);
}

std::string raw_of(const bytes& b) { return b.raw(); }

bytes pack_H(int64_t v) {
    std::string s;
    if (v < 0 || v > 0xFFFF) {
        throw py::StructError("ushort format requires 0 <= number <= 65535");
    }
    s.push_back(static_cast<char>(v & 0xFF));
    s.push_back(static_cast<char>((v >> 8) & 0xFF));
    return bytes(s);
}

// struct.pack_into("<H" or "<h", buf, k, v): the buffer is a bytearray
void pack_into(std::string& buf, int64_t k, int64_t v, bool is_signed) {
    int64_t lo = is_signed ? -32768 : 0, hi = is_signed ? 32767 : 65535;
    if (v < lo || v > hi) {
        throw py::StructError(is_signed ? "short format requires -32768 <= number <= 32767" : "ushort format requires 0 <= number <= 65535");
    }
    if (k < 0 || k + 2 > static_cast<int64_t>(buf.size())) {
        throw py::StructError("pack_into requires a buffer of at least 2 bytes");
    }
    buf[static_cast<size_t>(k)] = static_cast<char>(v & 0xFF);
    buf[static_cast<size_t>(k) + 1] = static_cast<char>((v >> 8) & 0xFF);
}

int64_t unpack_H(const std::string& buf, int64_t k) { return py::unpack_from1<int64_t>("<H", bytes(buf), k); }
int64_t unpack_h(const std::string& buf, int64_t k) { return py::unpack_from1<int64_t>("<h", bytes(buf), k); }

}  // namespace

bytes _ix(int64_t n) {
    std::string s;
    if (n < 0x80) {
        s.push_back(static_cast<char>(n));
    } else {
        s.push_back(static_cast<char>(0x80 | (n >> 8)));
        s.push_back(static_cast<char>(n & 0xFF));
    }
    return bytes(s);
}

std::tuple<int64_t, int64_t> _idx(const bytes& r, int64_t q) {
    int64_t a = at_(r, q);
    if (a < 0x80) {
        return {a, q + 1};
    }
    return {((a & 0x7F) << 8) | at_(r, q + 1), q + 2};
}

py::list<Fixup> fixups(const bytes& r, const Threads& thr) {
    py::list<Fixup> out;
    int64_t q = 0;
    while (q < r.size()) {
        int64_t c = at_(r, q);
        if ((c & 0x80) == 0) {
            q += 1;
            int64_t d = (c >> 6) & 1;
            int64_t meth = (c >> 2) & 7;
            std::optional<int64_t> ix;
            if (!(d != 0 && meth >= 4)) {
                auto pr = _idx(r, q);
                ix = std::get<0>(pr);
                q = std::get<1>(pr);
            }
            thr[std::make_tuple(d, c & 3)] = std::make_tuple(meth, ix);
            continue;
        }
        Fixup f;
        f.M = (c >> 6) & 1;
        f.loc = (c >> 2) & 7;
        f.off = ((c & 3) << 8) | at_(r, q + 1);
        int64_t fd = at_(r, q + 2);
        q += 3;
        if ((fd & 0x80) != 0) {
            auto key = std::make_tuple(int64_t(1), (fd >> 4) & 3);
            if (!thr.contains(key)) {
                throw py::KeyError("(1, " + std::to_string((fd >> 4) & 3) + ")");
            }
            auto t = thr.at(key);
            f.fm = std::get<0>(t);
            f.fi = std::get<1>(t);
        } else {
            f.fm = (fd >> 4) & 7;
            f.fi = std::nullopt;
            if (f.fm < 3) {
                auto pr = _idx(r, q);
                f.fi = std::get<0>(pr);
                q = std::get<1>(pr);
            }
        }
        if ((fd & 8) != 0) {
            auto key = std::make_tuple(int64_t(0), fd & 3);
            if (!thr.contains(key)) {
                throw py::KeyError("(0, " + std::to_string(fd & 3) + ")");
            }
            auto t = thr.at(key);
            f.tm = std::get<0>(t);
            f.ti = std::get<1>(t);
        } else {
            f.tm = fd & 3;
            auto pr = _idx(r, q);
            f.ti = std::get<0>(pr);
            q = std::get<1>(pr);
        }
        f.P = (fd >> 2) & 1;
        f.disp = 0;
        if (f.P == 0) {
            f.disp = at_(r, q) | (at_(r, q + 1) << 8);
            q += 2;
        }
        out.append(f);
    }
    return out;
}

bytes fixrec(const py::list<Fixup>& fs, int64_t base) {
    std::string b;
    for (const Fixup& f : fs) {
        if (!f.at.has_value()) {
            throw py::KeyError("at");
        }
        int64_t o = *f.at - base;
        if (!(0 <= o && o < 1024)) {
            throw py::AssertionError("");
        }
        b.push_back(static_cast<char>(0x80 | (f.M << 6) | (f.loc << 2) | (o >> 8)));
        b.push_back(static_cast<char>(o & 0xFF));
        b.push_back(static_cast<char>((f.fm << 4) | (f.P << 2) | f.tm));
        if (f.fm < 3) {
            if (!f.fi.has_value()) {
                throw py::TypeError("'<' not supported between instances of 'NoneType' and 'int'");
            }
            b += raw_of(_ix(*f.fi));
        }
        if (!f.ti.has_value()) {
            throw py::TypeError("'<' not supported between instances of 'NoneType' and 'int'");
        }
        b += raw_of(_ix(*f.ti));
        if (f.P == 0) {
            b += raw_of(pack_H(f.disp));
        }
    }
    return omf::record(0x9C, bytes(b));
}

namespace {

// the object module as the model of a file: names, segments, externals, groups, fix-ups by segment
struct Model {
    omf::Obj o;
    compare::Obj co;                                      // (the object as compare.function reads it)
    py::list<str> segs, exts, grps;
    std::map<int64_t, std::vector<Fixup>> fix;
    int64_t ti = 0;
    str textname;
    std::string text;
    py::list<std::tuple<str, int64_t, int64_t>> fns;
};

compare::Obj to_compare(const omf::Obj& o) {
    compare::Obj c;
    c.seg = o.seg;
    for (const auto& f : o.fix) {
        const auto& tg = std::get<3>(f);
        str second;
        if (std::holds_alternative<str>(std::get<1>(tg))) {
            second = std::get<str>(std::get<1>(tg));
        } else if (std::holds_alternative<int64_t>(std::get<1>(tg))) {
            second = str(std::to_string(std::get<int64_t>(std::get<1>(tg))));
        }
        c.fix.append(std::make_tuple(std::get<0>(f), std::get<1>(f), std::get<2>(f), compare::Target(std::get<0>(tg), second), std::get<4>(f)));
    }
    for (const auto& p : o.pubs) {
        c.pubs.append(std::make_tuple(std::get<0>(p), std::get<1>(p).value_or(str("")), std::get<2>(p)));
    }
    return c;
}

// a name list's entry by 1-based index (names[i - 1]: Python's negative indices count from the end)
const str& name_at(const py::list<str>& names, int64_t i) {
    int64_t k = i - 1;
    if (k < 0) k += py::len(names);
    if (k < 0 || k >= py::len(names)) {
        throw py::IndexError("list index out of range");
    }
    return names[k];
}

Model make_model(const bytes& raw) {
    Model m;
    Threads thr;
    py::list<str> names;
    std::optional<std::tuple<int64_t, int64_t>> last;
    for (const auto& rec : omf::records(raw)) {
        int64_t t = std::get<0>(rec);
        const bytes& r = std::get<1>(rec);
        if (t == 0x96) {
            int64_t q = 0;
            while (q < r.size()) {
                int64_t n = at_(r, q);
                names.append(r.slice(q + 1, q + 1 + n).decode("latin1"));
                q += 1 + n;
            }
        } else if (t == 0x98) {
            int64_t q = (at_(r, 0) >> 5) != 0 ? 1 : 4;
            int64_t ni = std::get<0>(_idx(r, q + 2));
            m.segs.append(name_at(names, ni));
        } else if (t == 0x9A) {
            int64_t gi = std::get<0>(_idx(r, 0));
            m.grps.append(name_at(names, gi));
        } else if (t == 0x8C) {
            int64_t q = 0;
            while (q < r.size()) {
                int64_t n = at_(r, q);
                m.exts.append(r.slice(q + 1, q + 1 + n).decode("latin1"));
                q += 1 + n;
                q = std::get<1>(_idx(r, q));
            }
        } else if (t == 0xA0 || t == 0xA2) {
            auto pr = _idx(r, 0);
            int64_t q = std::get<1>(pr);
            last = std::make_tuple(std::get<0>(pr), at_(r, q) | (at_(r, q + 1) << 8));
        } else if (t == 0x9C) {
            for (Fixup f : fixups(r, thr)) {
                if (!last.has_value()) {
                    throw py::TypeError("'NoneType' object is not subscriptable");
                }
                f.seg = std::get<0>(*last);
                f.at = std::get<1>(*last) + f.off;
                m.fix[std::get<0>(*last)].push_back(f);
            }
        }
    }
    m.o = omf::read(raw);
    py::list<str> cs = omf::code_segments(m.o);
    if (cs.empty()) {
        throw py::IndexError("list index out of range");
    }
    m.textname = cs[0];
    int64_t idx = -1;
    for (int64_t i = 0; i < py::len(m.segs); ++i) {
        if (m.segs[i] == m.textname) {
            idx = i;
            break;
        }
    }
    if (idx < 0) {
        throw py::ValueError(m.textname.utf8() + " is not in list");
    }
    m.ti = idx + 1;
    m.text = m.o.seg.at(m.textname).raw();
    m.fns = omf::functions(m.o, m.textname);
    m.co = to_compare(m.o);
    return m;
}

int64_t index_of(const py::list<str>& xs, const str& v) {
    for (int64_t i = 0; i < py::len(xs); ++i) {
        if (xs[i] == v) return i;
    }
    throw py::ValueError(v.utf8() + " is not in list");
}

bool contains(const py::list<str>& xs, const str& v) {
    for (const str& x : xs) {
        if (x == v) return true;
    }
    return false;
}

// re.fullmatch(rb"T\d+_DATA" / rb"T\d+_TEXT", s)
bool t_number_suffix(const std::string& s, const std::string& suffix) {
    if (s.size() < 2 + suffix.size() || s[0] != 'T') return false;
    size_t i = 1;
    while (i < s.size() && s[i] >= '0' && s[i] <= '9') ++i;
    return i > 1 && s.compare(i, std::string::npos, suffix) == 0;
}

struct Frag {
    int64_t new_ = 0;       // where its code is in the new text
    int64_t at = 0;
    int64_t end = 0;
    str name;
    const Model* M = nullptr;     // null: a stand-in of the program's bytes
    char tag = 0;
    int64_t idx = 0;
};

}  // namespace

std::tuple<bytes, py::list<str>> rebuild(const Project& prj, const bytes& raw, const py::list<int64_t>& addrs, int64_t end,
                                         const std::optional<str>& title, const py::list<bytes>& alts, const compare::Decoder& decode) {
    bytes img = prj.image();
    Symbols syms = prj.symbols();
    compare::Project cp;
    cp.dgroup = prj.dgroup();
    cp.loadseg = prj.loadseg();
    cp.code_base = [&prj](int64_t a) { return prj.code_base(a); };
    cp.relocs = [&prj]() -> const py::set<int64_t>& { return prj.relocs(); };

    // the threads of the whole object, as the records leave them
    Threads thr_all;
    py::list<std::tuple<int64_t, bytes>> recs = omf::records(raw);
    for (const auto& rec : recs) {
        if (std::get<0>(rec) == 0x9C) {
            fixups(std::get<1>(rec), thr_all);
        }
    }
    Model B = make_model(raw);
    int64_t ti = B.ti;
    if (py::len(B.fns) != py::len(addrs)) {
        throw py::ValueError(py::percent("%s: %d functions compiled, %d places", title.has_value() ? py::str(*title) : str("None"),
                                         py::len(B.fns), py::len(addrs)).utf8());
    }
    // the data of a model: (the segment's name - T<n>_DATA all as "far" -, its bytes), but the code segment
    auto dat = [](const Model& m) {
        std::vector<std::tuple<str, bytes>> v;
        for (const auto& kv : m.o.seg.items()) {
            const str& s = std::get<0>(kv);
            if (s == m.textname) continue;
            std::string u = s.utf8();
            v.push_back(std::make_tuple(t_number_suffix(u, "_DATA") ? str("far") : s, std::get<1>(kv)));
        }
        return v;
    };
    std::vector<Model> As;
    for (const bytes& x : alts) {
        Model A = make_model(x);
        if (py::len(A.fns) == py::len(addrs) && dat(A) == dat(B)) {   // (only when the data is the same)
            As.push_back(std::move(A));
        }
    }
    std::string nw;
    std::vector<Frag> F;
    py::list<str> stand;
    int64_t nadds = py::len(addrs);
    for (int64_t k = 0; k < nadds; ++k) {
        int64_t at = addrs[k];
        int64_t nxt = k + 1 < nadds ? addrs[k + 1] : end;
        std::optional<std::tuple<char, const Model*, int64_t>> src;
        std::vector<std::pair<char, const Model*>> order;
        order.push_back({'B', &B});
        for (const Model& A : As) {
            order.push_back({'A', &A});
        }
        for (const auto& tm : order) {
            char tag = tm.first;
            const Model* M = tm.second;
            const str f_ = std::get<0>(M->fns[k]);
            int64_t off = std::get<1>(M->fns[k]), ln = std::get<2>(M->fns[k]);
            // (each of the file's functions at its own place: one before it may be of another length - a stand-in)
            compare::Where where = [&, M, at, off](int64_t x) -> int64_t {
                for (int64_t j = 0; j < py::len(M->fns); ++j) {
                    int64_t o_ = std::get<1>(M->fns[j]), l_ = std::get<2>(M->fns[j]);
                    if (o_ <= x && x < o_ + std::max<int64_t>(l_, 1)) {
                        return addrs[j] + x - o_;
                    }
                }
                return at - off + x;
            };
            auto br = compare::function(M->co, img, f_, off, ln, at, cp, syms.copy(), Symbols(), decode, M->textname, where);
            bool tail = k == nadds - 1 && ln < nxt - at && img.slice(at + ln, nxt).strip(bytes::zeros(1)).empty();
            if (compare::identical(std::get<0>(br), std::get<1>(br)) && (ln == nxt - at || tail)) {   // (the last: zeros after it are not
                if (tail) {                                                                            //  the module's - a segment of the
                    nxt = std::min(nxt, at + ln + ((at + ln) & 1));                                    //  next ones begins with them - but
                }                                                                                       //  its pad to an even place is)
                src = std::make_tuple(tag, M, k);
                break;
            }
        }
        Frag fd;
        fd.name = std::get<0>(B.fns[k]);
        fd.new_ = static_cast<int64_t>(nw.size());
        fd.at = at;
        fd.end = nxt;
        if (!src.has_value()) {
            nw += img.slice(at, nxt).raw();
            stand.append(fd.name);
        } else {
            fd.tag = std::get<0>(*src);
            fd.M = std::get<1>(*src);
            fd.idx = std::get<2>(*src);
            int64_t off = std::get<1>(fd.M->fns[fd.idx]), ln = std::get<2>(fd.M->fns[fd.idx]);
            nw += fd.M->text.substr(static_cast<size_t>(off), static_cast<size_t>(ln));
            if (nxt > at + ln) {   // (the module's pad to an even place, behind its last)
                nw += img.slice(at + ln, nxt).raw();
            }
        }
        F.push_back(fd);
    }

    // (the function of the model that holds the offset, and where it now is)
    auto moved = [&](const Model& M, int64_t v) -> std::optional<int64_t> {
        for (int64_t k = 0; k < py::len(M.fns); ++k) {
            int64_t off = std::get<1>(M.fns[k]), ln = std::get<2>(M.fns[k]);
            if ((off <= v && v < off + ln) || (k == py::len(M.fns) - 1 && v == off + ln)) {
                return F[static_cast<size_t>(k)].new_ + v - off;
            }
        }
        return std::nullopt;
    };
    py::list<str> extnames = B.exts.copy();
    std::vector<Fixup> tfix;
    // a stand-in's segment words (a far call's, DGROUP's): the program's relocations there are fixups again, so
    // that the linker lists them; the word itself is the linker's to fill
    std::vector<int64_t> rel;
    if (auto pk = prj.packed_relocs()) {
        for (int64_t v : *pk) rel.push_back(v);
    } else {
        for (int64_t v : prj.relocs()) rel.push_back(v);
    }
    std::sort(rel.begin(), rel.end());
    std::map<int64_t, str> byaddr;
    for (const auto& kv : syms.items()) {
        const str& n = std::get<0>(kv);
        const auto& ta = std::get<1>(kv);
        if (std::get<0>(ta) == "C" && !n.contains(str(":"))) {
            byaddr[std::get<1>(ta)] = n;
        }
    }
    for (const Frag& fd : F) {
        if (fd.M != nullptr) {
            continue;
        }
        for (int64_t s : rel) {
            if (!(fd.at <= s && s < fd.end) || s + 2 > fd.end) {
                continue;
            }
            int64_t k = fd.new_ + s - fd.at;
            int64_t w = py::unpack_from1<int64_t>("<H", img, s);
            pack_into(nw, k, 0, false);   // (the linker adds the segment to what is there)
            Fixup g;
            g.M = 1;
            g.loc = 2;
            g.fm = 5;
            g.fi = std::nullopt;
            g.P = 1;
            g.disp = 0;
            g.at = k;
            if (s >= 3 && static_cast<unsigned char>(img.raw()[static_cast<size_t>(s - 3)]) == 0x9A && s - 3 >= fd.at) {   // a far call: the function it calls
                int64_t dest = (w - prj.loadseg()) * 16 + py::unpack_from1<int64_t>("<H", img, s - 2);
                auto it = byaddr.find(dest);
                if (it == byaddr.end()) {
                    throw py::ValueError(py::percent("%s: a far call to %05x, which SYMBOLS has no name for", fd.name, dest).utf8());
                }
                const str& nm = it->second;
                if (!contains(extnames, nm)) {
                    extnames.append(nm);
                }
                g.tm = 2;
                g.ti = index_of(extnames, nm) + 1;
            } else if ((w - prj.loadseg()) * 16 == prj.dgroup() && contains(B.grps, str("DGROUP"))) {
                g.tm = 1;
                g.ti = index_of(B.grps, str("DGROUP")) + 1;
            } else {
                throw py::ValueError(py::percent("%s: a segment word %04x at %05x of no kind known", fd.name, w, s).utf8());
            }
            tfix.push_back(g);
        }
    }
    for (const Frag& fd : F) {
        if (fd.M == nullptr) {
            continue;
        }
        const Model& M = *fd.M;
        int64_t off = std::get<1>(M.fns[fd.idx]), ln = std::get<2>(M.fns[fd.idx]);
        auto fit = M.fix.find(M.ti);
        if (fit == M.fix.end()) {
            continue;
        }
        for (const Fixup& f : fit->second) {
            if (!(off <= *f.at && *f.at < off + ln)) {
                continue;
            }
            Fixup g = f;
            g.at = fd.new_ + *f.at - off;
            if (f.tm == 0 && f.ti.has_value() && *f.ti == M.ti) {   // a place of this file's own code: it has moved
                int64_t v = unpack_H(M.text, *f.at);
                std::optional<int64_t> v2 = moved(M, v);
                pack_into(nw, *g.at, (v2.has_value() ? *v2 : v) & 0xFFFF, false);
            }
            if (fd.tag != 'B') {   // (the other module numbers its names its own way)
                if (f.tm == 2) {
                    const str& nm = name_at(M.exts, *f.ti);
                    if (!contains(extnames, nm)) extnames.append(nm);
                    g.ti = index_of(extnames, nm) + 1;
                }
                if (f.fm == 2) {
                    const str& nm = name_at(M.exts, *f.fi);
                    if (!contains(extnames, nm)) extnames.append(nm);
                    g.fi = index_of(extnames, nm) + 1;
                }
            }
            tfix.push_back(g);
        }
    }
    std::set<int64_t> fixed;
    for (const Fixup& f : tfix) fixed.insert(*f.at);
    for (const Frag& fd : F) {   // calls and jumps between the file's functions that
        if (fd.M == nullptr) {   // the compiler filled itself
            continue;
        }
        const Model& M = *fd.M;
        int64_t off = std::get<1>(M.fns[fd.idx]), ln = std::get<2>(M.fns[fd.idx]);
        for (const auto& i : x86::disasm(bytes(M.text), off, off + ln, false)) {
            if ((i.mnemonic == "call" || i.mnemonic == "jmp") && i.size == 3) {
                int64_t first = static_cast<unsigned char>(M.text[static_cast<size_t>(i.address)]);
                if (first != 0xE8 && first != 0xE9) continue;
                int64_t here = fd.new_ + i.address - off;
                if (fixed.count(here + 1)) {
                    continue;
                }
                int64_t tgt = (i.address + 3 + unpack_h(M.text, i.address + 1)) & 0xFFFF;
                if (!(off <= tgt && tgt < off + ln)) {
                    std::optional<int64_t> t2 = moved(M, tgt);
                    if (t2.has_value()) {
                        pack_into(nw, here + 1, ((*t2 - (here + 3) + 0x8000) & 0xFFFF) - 0x8000, true);
                    }
                }
            }
        }
    }
    py::list<str> newext;
    for (int64_t i = py::len(B.exts); i < py::len(extnames); ++i) {
        newext.append(extnames[i]);
    }

    // where(x): how far the file's code offset x moved, or None
    auto where_moved = [&](int64_t x) -> std::optional<int64_t> {
        std::optional<int64_t> v = moved(B, x);
        return v.has_value() ? std::optional<int64_t>(*v - x) : std::nullopt;
    };

    // the records again; a file's code (and the segment words its code names, in CONST) before its data
    std::string parts[4];   // head, code, data, tail
    int cur = 0;
    auto add = [&](const bytes& b) { parts[cur] += b.raw(); };
    int64_t ci = contains(B.segs, str("CONST")) ? index_of(B.segs, str("CONST")) + 1 : 0;
    bool done_text = false, skipfix = false, extdone = false;
    int64_t lastbase = 0, segn = 0;
    for (const auto& rec : recs) {
        int64_t t = std::get<0>(rec);
        bytes r = std::get<1>(rec);
        if (t == 0x80 && title.has_value() && !title->empty()) {
            add(omf::record(0x80, omf::name(*title)));
            continue;
        }
        if (t == 0x96 && title.has_value() && !title->empty()) {   // (the far data's segment is named after the file)
            int64_t q = 0;
            std::string nb;
            while (q < r.size()) {
                int64_t n = at_(r, q);
                std::string s = r.slice(q + 1, q + 1 + n).raw();
                q += 1 + n;
                if (t_number_suffix(s, "_DATA")) {
                    s = title->encode().raw() + "_DATA";
                } else if (t_number_suffix(s, "_TEXT")) {
                    s = title->encode().raw() + "_TEXT";
                }
                nb.push_back(static_cast<char>(s.size()));
                nb += s;
            }
            add(omf::record(t, bytes(nb)));
            continue;
        }
        if (t == 0x98) {
            segn += 1;
            if (segn == ti) {
                int64_t q = (at_(r, 0) >> 5) != 0 ? 1 : 4;
                r = bytes(r.slice(0, q).raw() + pack_H(static_cast<int64_t>(nw.size())).raw() + r.slice_from(q + 2).raw());
            }
            add(omf::record(t, r));
            continue;
        }
        if ((t == 0x90 || t == 0xA0 || t == 0xA2 || t == 0x8A) && !newext.empty() && !extdone) {
            extdone = true;
            cur = 0;
            std::string nm;
            for (const str& n : newext) {
                nm += omf::name(n).raw() + std::string(1, '\0');
            }
            add(omf::record(0x8C, bytes(nm)));
        }
        if (t == 0x90) {
            auto gp = _idx(r, 0);
            auto sp = _idx(r, std::get<1>(gp));
            int64_t si = std::get<0>(sp);
            int64_t q = std::get<1>(sp);
            std::string head = r.slice(0, q).raw();
            if (si == 0) {
                head = r.slice(0, q + 2).raw();
                q += 2;
            }
            std::string body;
            while (q < r.size()) {
                int64_t n = at_(r, q);
                std::string nm = r.slice(q, q + 1 + n).raw();
                q += 1 + n;
                int64_t off = at_(r, q) | (at_(r, q + 1) << 8);
                int64_t q2 = std::get<1>(_idx(r, q + 2));
                if (si == ti) {
                    std::optional<int64_t> wm = where_moved(off);
                    off += wm.has_value() ? *wm : 0;   // (where(off) or 0)
                }
                body += nm + pack_H(off).raw() + r.slice(q + 2, q2).raw();
                q = q2;
            }
            add(omf::record(t, bytes(head + body)));
            continue;
        }
        if (t == 0xA0 || t == 0xA2) {
            auto pr = _idx(r, 0);
            int64_t si = std::get<0>(pr);
            int64_t q = std::get<1>(pr);
            lastbase = at_(r, q) | (at_(r, q + 1) << 8);
            cur = (si == ti || si == ci) ? 1 : 2;
            if (si == ti) {
                skipfix = true;
                if (!done_text) {
                    done_text = true;
                    int64_t p = 0;
                    std::vector<Fixup> fs = tfix;
                    std::stable_sort(fs.begin(), fs.end(), [](const Fixup& a, const Fixup& b) { return *a.at < *b.at; });
                    while (p < static_cast<int64_t>(nw.size())) {
                        int64_t e = std::min<int64_t>(p + 1000, static_cast<int64_t>(nw.size()));
                        for (const Fixup& f : fs) {   // (no fixup's word across two records)
                            int64_t w = f.loc == 3 ? 4 : 2;
                            if (*f.at < e && e < *f.at + w) {
                                e = *f.at;
                            }
                        }
                        add(omf::record(0xA0, bytes(_ix(ti).raw() + pack_H(p).raw() + nw.substr(static_cast<size_t>(p), static_cast<size_t>(e - p)))));
                        py::list<Fixup> here;
                        for (const Fixup& f : fs) {
                            if (p <= *f.at && *f.at < e) here.append(f);
                        }
                        if (!here.empty()) {
                            add(fixrec(here, p));
                        }
                        p = e;
                    }
                }
                continue;
            }
            skipfix = false;
            add(omf::record(t, r));
            continue;
        }
        if (t == 0x9C) {
            if (skipfix) {
                continue;
            }
            py::list<Fixup> mine = fixups(r, thr_all.copy());
            if (!mine.empty()) {
                py::list<Fixup> placed;
                for (Fixup f : mine) {
                    f.at = lastbase + f.off;
                    placed.append(f);
                }
                add(fixrec(placed, lastbase));
            }
            continue;
        }
        if (t == 0x8A || t == 0x8B) {
            cur = 3;
        } else if (t != 0x9C && t != 0xA0 && t != 0xA2) {
            cur = 0;
        }
        add(omf::record(t, r));
    }
    return {bytes(parts[0] + parts[1] + parts[2] + parts[3]), stand};
}

}  // namespace codebreaker::exact::standin
