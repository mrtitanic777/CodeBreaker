// codebreaker/formats/omf.cpp -- typed C++ of codebreaker.formats.omf (see omf.hpp).
//
// The readers are native/formats/omf (omf.cc, with core/bytes.cc and core/text.cc): linked, and called
// directly with typed results.  The question handlers of native/formats/omf/questions.cc (which shaped
// their answers as pickle) are the typed functions below.

#include "codebreaker/formats/omf.hpp"

#include <algorithm>
#include <cstdio>
#include <set>

#include "codebreaker/exact/aout.hpp"
#include "../../../../formats/core/bytes.h"
#include "../../../../formats/core/text.h"
#include "../../../../formats/omf/omf.h"

namespace codebreaker::formats::omf {

namespace {

namespace cb = ::cbf;
namespace cbo = ::cbf::omf;

// ---- the exception an answer carried: native._exception(kind, message)
[[noreturn]] void raise_py(const cb::PyError& e) {
    const std::string& k = e.kind;
    const std::string& m = e.message;
    if (k == "struct.error") throw py::StructError(m);
    if (k == "KeyError") {
        std::string t = m;                                    // message.strip("'"), then str(KeyError(t)) is its repr
        size_t a = t.find_first_not_of('\'');
        size_t b = t.find_last_not_of('\'');
        t = a == std::string::npos ? std::string() : t.substr(a, b - a + 1);
        throw py::KeyError(py::repr(py::str(t)));
    }
    if (k == "UnicodeDecodeError") throw py::UnicodeDecodeError("'ascii' codec can't decode bytes in position 0-0: " + m);
    if (k == "UnicodeEncodeError") throw py::UnicodeEncodeError("'latin-1' codec can't encode characters in position 0-0: " + m);
    if (k == "ValueError") throw py::ValueError(m);
    if (k == "IndexError") throw py::IndexError(m);
    if (k == "TypeError") throw py::TypeError(m);
    if (k == "MemoryError") throw py::MemoryError(m);
    if (k == "ZeroDivisionError") throw py::ZeroDivisionError(m);
    if (k == "OverflowError") throw py::OverflowError(m);
    if (k == "AttributeError") throw py::AttributeError(m);
    if (k == "OSError") throw py::OSError(m);
    if (k == "AssertionError") throw py::AssertionError(m);
    throw py::RuntimeError(m);
}

// runs a call into the C++ reader, its exceptions made the Python's
template <class F>
auto native(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cb::PyError& e) {
        raise_py(e);
    } catch (const cb::Fallback& f) {
        throw Fallback(f.reason);
    }
}

cb::Bytes B(const py::bytes& b) { return cb::Bytes(b.raw()); }
py::str S(const std::string& utf8) { return py::str(utf8); }
std::string U(const py::str& s) { return s.utf8(); }
py::bytes PB(const std::string& s) { return py::bytes(s); }
py::bytes PB(cb::Bytes b) { return py::bytes(b.str()); }

Target target_of(const cbo::Target& t) {
    if (t.what == cbo::Target::Name) return Target(S(t.kind), S(t.name));
    if (t.what == cbo::Target::Number) return Target(S(t.kind), t.number);
    return Target(S(t.kind), std::monostate{});
}

Obj obj_of(const cbo::Object& ob, const py::bytes& raw) {
    Obj o;
    for (const cbo::Public& p : ob.pubs)
        o.pubs.append(Public(S(p.name), p.segment ? std::optional<py::str>(S(*p.segment)) : std::nullopt, p.offset));
    for (const auto& e : ob.ext) o.ext.append(S(e));
    for (const cbo::Fixup& f : ob.fix) {
        o.fix.append(Fixup(S(f.segment), f.at, S(f.kind), target_of(f.target), f.displacement));
        o.fixloc.append(f.location);
    }
    for (const auto& g : ob.groups) {
        py::list<py::str> mem;
        for (const auto& s : g.second) mem.append(S(s));
        o.groups.set(S(g.first), mem);
    }
    for (const auto& c : ob.comm) o.comm.set(S(c.first), std::make_tuple(c.second.first, c.second.second));
    for (const auto& kv : ob.combine) o.combine.set(S(kv.first), kv.second);
    if (ob.start) o.start = std::make_tuple(S(ob.start->first), ob.start->second);
    for (const auto& kv : ob.seg) o.seg.set(S(kv.first), PB(kv.second));
    for (const cbo::SegDef& s : ob.segs) o.segs.append(SegDef(S(s.name), S(s.cls), s.length, s.align));
    for (const auto& kv : ob.filled) o.filled.set(S(kv.first), kv.second);
    o.raw = raw;
    return o;
}

// an a.out object, as the fields omf.read's Obj has
Obj obj_of(const exact::aout::Obj& a) {
    Obj o;
    o.seg = a.seg;
    o.filled = a.filled;
    o.segs = a.segs;
    o.groups = a.groups;
    for (const auto& s : a.syms) o.syms.append(s);
    for (const auto& p : a.pubs) o.pubs.append(Public(std::get<0>(p), std::get<1>(p), std::get<2>(p)));
    o.ext = a.ext;
    o.comm = a.comm;
    for (const auto& f : a.fix) {
        const auto& t = std::get<3>(f);
        o.fix.append(Fixup(std::get<0>(f), std::get<1>(f), std::get<2>(f), Target(std::get<0>(t), std::get<1>(t)), std::get<4>(f)));
    }
    o.raw = a.raw;
    o.from_aout = true;
    return o;
}

// the C++ reader on `data` (an a.out object is the caller's: Fallback)
cbo::Object read_native(const py::bytes& data) {
    return native([&] { return cbo::read(B(data)); });
}

std::string upper_ascii(std::string s) {
    for (char& c : s) c = char(std::toupper(uint8_t(c)));
    return s;
}

bool ends_with(const std::string& s, const char* tail) {
    size_t n = std::char_traits<char>::length(tail);
    return s.size() >= n && s.compare(s.size() - n, n, tail) == 0;
}

// "%x" with a minimum width, zero padded
std::string hexw(int64_t v, int width, bool upper = false) {
    char b[40];
    std::snprintf(b, sizeof b, upper ? "%0*llX" : "%0*llx", width, static_cast<unsigned long long>(v));
    return b;
}

// "%-Ns" over code points
std::string pad_right(const py::str& s, size_t width) {
    std::string out = s.utf8();
    size_t n = s.u32().size();
    if (n < width) out.append(width - n, ' ');
    return out;
}
std::string pad_left_int(int64_t v, size_t width) {
    std::string out = std::to_string(v);
    if (out.size() < width) out.insert(0, width - out.size(), ' ');
    return out;
}

// ---- the code of an object, cut at its publics (omf._code and the questions that share it)
struct Code {
    std::optional<std::string> seg;
    std::string code;
    std::vector<int64_t> mask;                         // a set, in insertion order
    std::vector<std::pair<std::string, std::pair<std::string, int64_t>>> pubs;
};

int64_t width_of(int64_t loc) {                        // _WIDTH
    switch (loc) {
        case 0: case 4: return 1;
        case 3: case 9: case 13: return 4;
        case 11: return 6;
        default: return 2;
    }
}

Code code_of(const py::bytes& data) {
    cbo::Object ob = read_native(data);
    Code r;
    for (const cbo::Public& p : ob.pubs) {
        std::pair<std::string, int64_t> v{p.segment ? *p.segment : "?", p.offset};
        auto it = std::find_if(r.pubs.begin(), r.pubs.end(), [&](auto& kv) { return kv.first == p.name; });
        if (it != r.pubs.end()) it->second = v;
        else r.pubs.emplace_back(p.name, v);
    }
    for (const cbo::SegDef& s : ob.segs) {
        std::string cu = upper_ascii(s.cls), nu = upper_ascii(s.name);
        if (cu.find("CODE") != std::string::npos || ends_with(nu, "TEXT")) {
            r.seg = s.name;
            break;
        }
    }
    if (!r.seg && !ob.segs.empty()) r.seg = ob.segs[0].name;
    if (!r.seg) return r;
    std::set<int64_t> seen;
    for (const cbo::Fixup& f : ob.fix)
        if (f.segment == *r.seg)
            for (int64_t k = f.at; k < f.at + width_of(f.location); ++k)
                if (seen.insert(k).second) r.mask.push_back(k);
    std::string bytes;
    for (auto& kv : ob.seg)
        if (kv.first == *r.seg) bytes = kv.second;
    int64_t n = 0;
    for (auto& kv : ob.filled)
        if (kv.first == *r.seg) n = kv.second;
    r.code = bytes.substr(0, size_t(std::max<int64_t>(0, std::min<int64_t>(n, int64_t(bytes.size())))));
    return r;
}

py::set<int64_t> mask_set(const std::vector<int64_t>& m) {
    py::set<int64_t> s;
    for (int64_t k : m) s.add(k);
    return s;
}

// ---- patched / rename_symbols (the OMF records walked)
std::pair<int64_t, int64_t> omf_ix(cb::Bytes r, int64_t q) {   // an index: one byte, or two when the top bit is set
    if (r.at(q) < 0x80) return {r.at(q), q + 1};
    return {(int64_t(r.at(q) & 0x7F) << 8) | r.at(q + 1), q + 2};
}
std::pair<int64_t, int64_t> omf_idx(cb::Bytes r, int64_t q) {  // (the same, tested on the top bit)
    if (r.at(q) & 0x80) return {(int64_t(r.at(q) & 0x7F) << 8) | r.at(q + 1), q + 2};
    return {r.at(q), q + 1};
}

// the walk of rename_symbols: each name passed through `fn` (latin1 text in, latin1 text out)
std::string rename_walk(cb::Bytes data, const std::function<std::string(const std::string&)>& fn) {
    std::string out;
    auto put_name = [&](std::string& nb, cb::Bytes nm, const std::string& rest) {
        std::string neu = cb::to_latin1(fn(cb::latin1(nm)));
        if (neu.size() > 255) cb::raise("ValueError", "bytes must be in range(0, 256)");
        nb += char(neu.size()) + neu + rest;
    };
    for (const cbo::Record& rec : cbo::records(data)) {
        int64_t t = rec.type;
        std::string r = rec.payload.str();
        cb::Bytes rb(r);
        if (t == 0x8C || t == 0xB0) {
            int64_t q = 0;
            std::string nb;
            while (q < rb.size()) {
                cb::Bytes nm = rb.slice(q + 1, q + 1 + rb.at(q));
                q += 1 + rb.at(q);
                std::string rest;
                if (t == 0x8C) {
                    rest = rb.slice(q, q + 1).str();
                    q += 1;
                } else {
                    int64_t st = q;
                    q += 1;
                    int64_t kind = rb.at(q);
                    q += 1;
                    for (int k = 0; k < (kind == 0x61 ? 2 : 1); ++k) {
                        int64_t b0 = rb.at(q);
                        q += 1 + (b0 == 0x81 ? 2 : b0 == 0x84 ? 3 : b0 == 0x88 ? 4 : 0);
                    }
                    rest = rb.slice(st, q).str();
                }
                put_name(nb, nm, rest);
            }
            r = nb;
        } else if (t == 0x90 || t == 0x91) {
            int64_t q = 0;
            q = omf_idx(rb, q).second;
            auto s = omf_idx(rb, q);
            q = s.second;
            if (s.first == 0) q += 2;
            std::string nb = rb.slice(0, q).str();
            int64_t w = t == 0x91 ? 4 : 2;
            while (q < rb.size()) {
                cb::Bytes nm = rb.slice(q + 1, q + 1 + rb.at(q));
                q += 1 + rb.at(q);
                std::string rest = rb.slice(q, q + w).str();
                q += w;
                int64_t q2 = omf_idx(rb, q).second;
                rest += rb.slice(q, q2).str();
                q = q2;
                put_name(nb, nm, rest);
            }
            r = nb;
        }
        out += cbo::record(t, cb::Bytes(r));
    }
    return out;
}

std::string u16le(int64_t v) {                        // struct.pack("<H", v)
    if (v < 0 || v > 0xFFFF) cb::raise("struct.error", "'H' format requires 0 <= number <= 65535");
    return std::string{char(v), char(v >> 8)};
}

std::string rec(int64_t t, const std::string& payload) { return cbo::record(t, cb::Bytes(payload)); }

// the public names of a dict, sorted by value (ties in the dict's order)
std::vector<std::pair<std::string, int64_t>> by_value(const py::dict<py::str, int64_t>& d) {
    std::vector<std::pair<std::string, int64_t>> v;
    for (const auto& kv : d.items()) v.emplace_back(cb::to_latin1(U(std::get<0>(kv))), std::get<1>(kv));
    std::stable_sort(v.begin(), v.end(), [](auto& a, auto& b) { return a.second < b.second; });
    return v;
}

std::string nm(const std::string& latin1_bytes) { return cbo::name(latin1_bytes); }

}  // namespace

// ------------------------------------------------------------------------------------------ constants
const py::dict<int64_t, int64_t> _WIDTH = {{0, 1}, {1, 2}, {2, 2}, {3, 4}, {4, 1}, {5, 2}, {9, 4}, {11, 6}, {13, 4}};

const py::dict<int64_t, py::str> _KIND = {
    {0x80, py::str("THEADR")}, {0x82, py::str("LHEADR")}, {0x88, py::str("COMENT")}, {0x8A, py::str("MODEND")},
    {0x8B, py::str("MODEND")}, {0x8C, py::str("EXTDEF")}, {0x8E, py::str("TYPDEF")}, {0x90, py::str("PUBDEF")},
    {0x91, py::str("PUBDEF")}, {0x94, py::str("LINNUM")}, {0x95, py::str("LINNUM")}, {0x96, py::str("LNAMES")},
    {0x98, py::str("SEGDEF")}, {0x99, py::str("SEGDEF")}, {0x9A, py::str("GRPDEF")}, {0x9C, py::str("FIXUPP")},
    {0x9D, py::str("FIXUPP")}, {0xA0, py::str("LEDATA")}, {0xA1, py::str("LEDATA")}, {0xA2, py::str("LIDATA")},
    {0xA3, py::str("LIDATA")}, {0xB0, py::str("COMDEF")}, {0xF0, py::str("LIBHDR")}, {0xF1, py::str("LIBEND")}};

// ------------------------------------------------------------------------------------------ the reader
py::list<std::tuple<int64_t, py::bytes>> records(const py::bytes& data) {
    return native([&] {
        py::list<std::tuple<int64_t, py::bytes>> out;
        for (const cbo::Record& r : cbo::records(B(data))) out.append(std::make_tuple(int64_t(r.type), PB(r.payload)));
        return out;
    });
}

py::bytes record(int64_t t, const py::bytes& payload) {
    return native([&] { return PB(cbo::record(t, B(payload))); });
}

py::bytes name(const py::str& s) {
    return native([&] { return PB(cbo::name(cb::to_latin1(U(s)))); });
}

py::bytes name(const py::bytes& s) {
    return native([&] { return PB(cbo::name(s.raw())); });
}

py::bytes patched(const py::bytes& raw, const py::str& spec) {
    auto m = py::re::fullmatch(py::str(R"((\w+)\+([0-9a-fA-F]+)=([0-9a-fA-F]+))"), spec);
    if (!m) throw py::AttributeError("'NoneType' object has no attribute 'group'");
    py::str seg_name = m->group(1);
    int64_t off = py::int_parse(m->group(2), 16);
    py::str hex = m->group(3);
    if (hex.u32().size() % 2) throw py::ValueError("fromhex() arg must contain an even number of hexadecimal digits");
    py::bytes val = py::bytes::fromhex(hex);
    return native([&] {
        const std::string seg = U(seg_name);
        const std::string v = val.raw();
        std::vector<std::string> names, segs;
        std::string out;
        for (const cbo::Record& rec : cbo::records(B(raw))) {
            int64_t t = rec.type;
            std::string r = rec.payload.str();
            cb::Bytes rb(r);
            if (t == 0x96) {
                int64_t q = 0;
                while (q < rb.size()) {
                    names.push_back(cb::latin1(rb.slice(q + 1, q + 1 + rb.at(q))));
                    q += 1 + rb.at(q);
                }
            } else if (t == 0x98) {
                int64_t q = rb.at(0) >> 5 ? 1 : 4;
                auto ni = omf_ix(rb, q + 2);
                segs.push_back(cb::py_at(names, ni.first - 1));
            } else if (t == 0xA0) {
                auto si = omf_ix(rb, 0);
                int64_t q = si.second;
                int64_t lo = rb.at(q) | (int64_t(rb.at(q + 1)) << 8);
                if (cb::py_at(segs, si.first - 1) == seg) {
                    for (size_t k = 0; k < v.size(); ++k)
                        if (lo <= off + int64_t(k) && off + int64_t(k) < lo + int64_t(r.size()) - q - 2)
                            r[size_t(q + 2 + off + int64_t(k) - lo)] = v[k];
                }
            }
            out += cbo::record(t, cb::Bytes(r));
        }
        return PB(out);
    });
}

Obj read(const py::bytes& data) {
    if (exact::aout::is_aout(data)) return obj_of(exact::aout::read(data));          // (a Version 7 a.out: XENIX's compiler)
    return obj_of(read_native(data), data);
}

py::list<std::tuple<py::str, py::bytes>> library(const py::bytes& data) {
    return native([&] {
        py::list<std::tuple<py::str, py::bytes>> out;
        for (auto& m : cbo::library(B(data))) out.append(std::make_tuple(S(m.first), PB(m.second)));
        return out;
    });
}

py::set<int64_t> holes(const Obj& o, const py::str& seg) {
    py::set<int64_t> out;
    for (const Fixup& f : o.fix) {
        if (!(std::get<0>(f) == seg)) continue;
        const int64_t at = std::get<1>(f);
        const py::str& kind = std::get<2>(f);
        const int64_t n = kind == py::str("lo") ? 1 : kind == py::str("ptr") ? 4 : 2;           // (off base rel, and any other kind: 2)
        for (int64_t k = at; k < at + n; ++k) out.add(k);
    }
    return out;
}

py::list<py::str> code_segments(const Obj& o) {
    py::list<py::str> out;
    for (const SegDef& s : o.segs) {
        const std::string n = U(std::get<0>(s));
        const std::string cls = upper_ascii(U(std::get<1>(s)));
        const std::string nu = upper_ascii(n);
        const bool text = n.size() >= 5 && ends_with(n, "_TEXT");
        if (text || cls == "CODE" || cls == "PROG" || nu == "PROG" || nu == "_PROG" || nu == "_CODE") out.append(std::get<0>(s));
    }
    return out;
}

py::list<std::tuple<py::str, int64_t, int64_t>> functions(const Obj& o, const py::str& seg) {
    std::vector<std::pair<int64_t, py::str>> ps;
    for (const Public& p : o.pubs)
        if (std::get<1>(p) && *std::get<1>(p) == seg) ps.emplace_back(std::get<2>(p), std::get<0>(p));
    std::sort(ps.begin(), ps.end(), [](const auto& a, const auto& b) {
        if (a.first != b.first) return a.first < b.first;
        return a.second.u32() < b.second.u32();
    });
    const int64_t len = o.seg.contains(seg) ? o.seg.at(seg).size() : 0;
    const int64_t filled = o.filled.get(seg, 0);
    const int64_t n = std::min(len, filled);
    if ((!ps.empty() && ps[0].first > 0) || (ps.empty() && n)) ps.insert(ps.begin(), {0, py::str("(module start)")});
    py::list<std::tuple<py::str, int64_t, int64_t>> out;
    for (size_t k = 0; k < ps.size(); ++k)
        out.append(std::make_tuple(ps[k].second, ps[k].first, (k + 1 < ps.size() ? ps[k + 1].first : n) - ps[k].first));
    return out;
}

py::bytes rename_externs(const py::bytes& data, const py::dict<py::str, py::str>& mapping) {
    return native([&] {
        std::map<std::string, std::string> m;
        for (const auto& kv : mapping.items()) m[U(std::get<0>(kv))] = U(std::get<1>(kv));
        return PB(cbo::rename_externs(B(data), m));
    });
}

py::bytes rename_symbols(const py::bytes& data, const std::function<py::str(const py::str&)>& fn) {
    // the names the walk meets, and the failure it raised after them
    std::vector<std::string> seen;
    std::optional<cb::PyError> failed;
    try {
        rename_walk(B(data), [&](const std::string& n) { seen.push_back(n); return n; });
    } catch (const cb::PyError& e) {
        failed = e;
    } catch (const cb::Fallback& f) {
        throw Fallback(f.reason);
    }
    py::list<py::str> renamed;
    for (const std::string& n : seen) {
        py::str neu = fn(S(n));                              // (fn, the encoding and the length are checked in the order
        if (neu.encode("latin1").size() > 255)               //  the single walk met them; a damaged record raises after)
            throw py::ValueError("bytes must be in range(0, 256)");
        renamed.append(neu);
    }
    if (failed) raise_py(*failed);
    return native([&] {
        size_t k = 0;
        return PB(rename_walk(B(data), [&](const std::string&) {
            if (k >= size_t(renamed.size())) cb::raise("IndexError", "list index out of range");
            return U(renamed[int64_t(k++)]);
        }));
    });
}

py::bytes raw_object(const py::str& title, const py::bytes& code_b, const py::dict<py::str, int64_t>& pubs, const py::bytes& data_b,
                     const py::dict<py::str, int64_t>& dpubs, const py::str& text_s, int64_t dalign) {
    return native([&] {
        const std::string tl = cb::to_latin1(U(title)), code = code_b.raw(), data = data_b.raw();
        const std::string text = cb::to_latin1(U(text_s));
        std::string out = rec(0x80, nm(tl));
        out += rec(0x96, nm("") + nm("DGROUP") + nm(text) + nm("CODE") + nm("_DATA") + nm("DATA"));
        out += rec(0x98, std::string(1, char(0x28)) + u16le(int64_t(code.size())) + std::string{3, 4, 1});   // byte aligned, public
        out += rec(0x98, std::string(1, char(dalign == 2 ? 0x48 : 0x28)) + u16le(int64_t(data.size())) + std::string{5, 6, 1});
        out += rec(0x9A, std::string{2, char(0xFF), 2});                                                      // DGROUP: _DATA
        const std::pair<int, const std::string*> parts[] = {{1, &code}, {2, &data}};
        for (auto& sb : parts)
            for (size_t k = 0; k < sb.second->size(); k += 1000)
                out += rec(0xA0, std::string(1, char(sb.first)) + u16le(int64_t(k)) + sb.second->substr(k, 1000));
        const std::tuple<int, int, const py::dict<py::str, int64_t>*> groups[] = {{0, 1, &pubs}, {1, 2, &dpubs}};
        for (auto& [grp, seg, ps] : groups) {
            if (ps->empty()) continue;
            std::string body{char(grp), char(seg)};
            for (auto& na : by_value(*ps)) body += nm(na.first) + u16le(na.second) + std::string(1, '\0');
            out += rec(0x90, body);
        }
        return PB(out + rec(0x8A, std::string(1, '\0')));
    });
}

py::bytes bss_object(const py::dict<py::str, int64_t>& names, int64_t start, int64_t end, const py::str& title_s, const py::str& seg_s,
                     bool para) {
    return native([&] {
        const std::string title = cb::to_latin1(U(title_s));
        const std::string seg = cb::to_latin1(U(seg_s));
        std::string out = rec(0x80, nm(title));
        out += rec(0x96, nm("") + nm("DGROUP") + nm(seg) + nm("BSS"));
        out += rec(0x98, std::string(1, char(para ? 0x68 : 0x48)) + u16le(end - start) + std::string{3, 4, 1});
        out += rec(0x9A, std::string{2, char(0xFF), 1});
        std::vector<std::pair<std::string, int64_t>> items;
        for (const auto& kv : names.items()) items.emplace_back(cb::to_latin1(U(std::get<0>(kv))), std::get<1>(kv));
        std::sort(items.begin(), items.end(), [](auto& a, auto& b) { return a.second != b.second ? a.second < b.second : a.first < b.first; });
        size_t k = 0;
        while (k < items.size()) {
            std::string body{1, 1};
            while (k < items.size() && body.size() < 900) {
                body += nm(items[k].first) + u16le(items[k].second - start) + std::string(1, '\0');
                ++k;
            }
            out += rec(0x90, body);
        }
        return PB(out + rec(0x8A, std::string(1, '\0')));
    });
}

std::tuple<py::set<py::str>, py::set<py::str>> names_of(const py::bytes& data) {
    cbo::Object ob = read_native(data);
    py::set<py::str> pubs, ext;
    for (const cbo::Public& p : ob.pubs) pubs.add(S(p.name));
    for (const auto& e : ob.ext) ext.add(S(e));
    return std::make_tuple(pubs, ext);
}

py::set<py::str> publics(const py::bytes& data) {
    return native([&] {
        py::set<py::str> out;
        for (auto& s : cbo::publics(B(data))) out.add(S(s));
        return out;
    });
}

py::list<py::str> externs(const py::bytes& data) {
    return native([&] {
        py::list<py::str> out;
        for (auto& s : cbo::externs(B(data))) out.append(S(s));
        return out;
    });
}

std::optional<std::tuple<int64_t, int64_t, py::bytes>> Walk::next() {
    if (pos_ < items_.size()) return items_[pos_++];
    if (error_) {                                           // (raised once, as the generator's failure)
        auto e = *error_;
        error_.reset();
        raise_py(cb::PyError{U(std::get<0>(e)), U(std::get<1>(e))});
    }
    return std::nullopt;
}

Walk walk(const py::bytes& data, int64_t start) {
    Walk w;
    const cb::Bytes d = B(data);
    try {
        for (int64_t i = start; i < d.size();) {
            const uint8_t t = d.at(i);
            if (t == 0) {
                ++i;
                continue;
            }
            const int64_t n = d.u16(i + 1);
            w.items_.append(std::make_tuple(i, int64_t(t), PB(d.slice(i + 3, i + 3 + n - 1))));
            i += 3 + n;
            if (t == 0xF1) break;
        }
    } catch (const cb::PyError& e) {
        w.error_ = std::make_tuple(S(e.kind), S(e.message));
    }
    return w;
}

std::tuple<int64_t, int64_t> _idx(const py::bytes& r, int64_t q) {
    return native([&] {
        auto v = omf_idx(B(r), q);
        return std::make_tuple(v.first, v.second);
    });
}

std::tuple<int64_t, int64_t> index(const py::bytes& r, int64_t q) { return _idx(r, q); }

std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>, py::dict<py::str, std::tuple<py::str, int64_t>>> _code(const py::bytes& data) {
    Code r = code_of(data);
    py::dict<py::str, std::tuple<py::str, int64_t>> pubs;
    for (auto& kv : r.pubs) pubs.set(S(kv.first), std::make_tuple(S(kv.second.first), kv.second.second));
    return std::make_tuple(r.seg ? std::optional<py::str>(S(*r.seg)) : std::nullopt, PB(r.code), mask_set(r.mask), pubs);
}

std::tuple<std::optional<py::str>, py::bytes, py::set<int64_t>> code_segment(const py::bytes& data) {
    Code r = code_of(data);
    return std::make_tuple(r.seg ? std::optional<py::str>(S(*r.seg)) : std::nullopt, PB(r.code), mask_set(r.mask));
}

py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> function_bytes(const py::bytes& data) {
    Code r = code_of(data);
    py::dict<py::str, std::tuple<py::bytes, py::set<int64_t>>> out;
    if (!r.seg) return out;
    std::vector<std::pair<int64_t, std::string>> here;
    for (auto& kv : r.pubs)
        if (kv.second.first == *r.seg) here.emplace_back(kv.second.second, kv.first);
    std::sort(here.begin(), here.end());
    std::vector<int64_t> sorted_mask = r.mask;
    std::sort(sorted_mask.begin(), sorted_mask.end());
    for (size_t i = 0; i < here.size(); ++i) {                 // (a name met again: its last cut wins, first place kept)
        const int64_t off = here[i].first;
        const int64_t end = i + 1 < here.size() ? here[i + 1].first : int64_t(r.code.size());
        cb::Bytes code(r.code);
        std::vector<int64_t> m;
        for (int64_t k : sorted_mask)
            if (off <= k && k < end) m.push_back(k - off);
        out.set(S(here[i].second), std::make_tuple(PB(code.slice(off, end)), mask_set(m)));
    }
    return out;
}

// ------------------------------------------------------------------------------------------ the listing
int64_t main(const py::list<py::str>& argv) {
    if (argv.empty()) {
        py::print(py::str("python -m codebreaker.formats.omf <file.obj | file.lib> [--raw]\n"
                          "each module's segments, publics and externals; --raw every record (type, length)"));
        return 2;
    }
    py::bytes data;
    {
        py::file f = py::open_rb(argv[0]);
        data = f.read(-1);
        f.close();
    }
    bool raw = false;
    for (const py::str& a : argv)
        if (a == py::str("--raw")) raw = true;
    if (raw) {
        Walk w = walk(data);
        while (auto r = w.next()) {
            const int64_t off = std::get<0>(*r), t = std::get<1>(*r);
            const py::bytes& body = std::get<2>(*r);
            py::str kind = _KIND.contains(t) ? _KIND.at(t) : py::str("?" + hexw(t, 2, true));
            py::print(py::str("@" + hexw(off, 6) + " " + pad_right(kind, 8) + " len=" + std::to_string(body.size() + 1)));
        }
        return 0;
    }
    for (const auto& m : library(data)) {
        const py::str& nm_ = std::get<0>(m);
        const py::bytes& module = std::get<1>(m);
        Obj o = read(module);
        py::print(py::str("== " + U(nm_) + " (" + std::to_string(module.size()) + " bytes)"));
        for (const SegDef& s : o.segs)
            py::print(py::str("  segment " + pad_right(std::get<0>(s), 12) + " class=" + pad_right(std::get<1>(s), 8) + " " +
                              pad_left_int(std::get<2>(s), 6) + " bytes"));
        for (const Public& p : o.pubs)
            py::print(py::str("  public  " + pad_right(std::get<0>(p), 24) + " " + (std::get<1>(p) ? U(*std::get<1>(p)) : "-") + ":" +
                              hexw(std::get<2>(p), 4)));
        for (const py::str& e : o.ext) py::print(py::str("  extern  " + U(e)));
    }
    return 0;
}

}  // namespace codebreaker::formats::omf
