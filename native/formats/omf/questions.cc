// questions.cc -- the OMF reader's answers, in the shapes codebreaker/formats/omf.py gives them.
// read() comes back as the attributes of omf.Obj (but raw, the caller's own bytes); the readers
// that take an Obj (holes, code_segments, functions) are handed the attributes they read.
#include <algorithm>
#include <functional>
#include <set>

#include "../core/questions.h"
#include "../core/text.h"
#include "omf.h"

namespace cbf::omf {
namespace {

void target(Out &o, const Target &t) {
    o.begin_tuple();
    o.text(t.kind);
    if (t.what == Target::Name) o.text(t.name);
    else if (t.what == Target::Number) o.integer(t.number);
    else o.none();
    o.end_tuple();
}

void records_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Record &r : records(c.data)) {
        o.begin_tuple();
        o.integer(r.type);
        o.bytes(r.payload);
        o.end_tuple();
    }
    o.end_list();
}

void record_q(Call &c) { c.out.bytes(Bytes(record(c.int_arg(0), c.data))); }

// name(s): a str is encoded latin1, bytes taken as they are
std::string name_arg(const Value &v) { return v.kind == Value::Str ? to_latin1(v.s) : v.as_bytes().str(); }
void name_q(Call &c) { c.out.bytes(Bytes(name(name_arg(c.arg(0))))); }

void read_q(Call &c) {
    Object ob = read(c.data);
    Out &o = c.out;
    o.begin_dict();
    o.key("pubs");
    o.begin_list();
    for (const Public &p : ob.pubs) {
        o.begin_tuple();
        o.text(p.name);
        if (p.segment) o.text(*p.segment);
        else o.none();
        o.integer(p.offset);
        o.end_tuple();
    }
    o.end_list();
    o.key("ext");
    o.begin_list();
    for (auto &e : ob.ext) o.text(e);
    o.end_list();
    o.key("fix");
    o.begin_list();
    for (const Fixup &f : ob.fix) {
        o.begin_tuple();
        o.text(f.segment);
        o.integer(f.at);
        o.text(f.kind);
        target(o, f.target);
        o.integer(f.displacement);
        o.end_tuple();
    }
    o.end_list();
    o.key("groups");
    o.begin_dict();
    for (auto &g : ob.groups) {
        o.text(g.first);
        o.begin_list();
        for (auto &s : g.second) o.text(s);
        o.end_list();
    }
    o.end_dict();
    o.key("comm");
    o.begin_dict();
    for (auto &cm : ob.comm) {
        o.text(cm.first);
        o.begin_tuple();
        o.integer(cm.second.first);
        o.boolean(cm.second.second);
        o.end_tuple();
    }
    o.end_dict();
    o.key("fixloc");
    o.begin_list();
    for (const Fixup &f : ob.fix) o.integer(f.location);
    o.end_list();
    o.key("combine");
    o.begin_dict();
    for (auto &kv : ob.combine) {
        o.text(kv.first);
        o.integer(kv.second);
    }
    o.end_dict();
    o.key("start");
    if (ob.start) {
        o.begin_tuple();
        o.text(ob.start->first);
        o.integer(ob.start->second);
        o.end_tuple();
    } else {
        o.none();
    }
    o.key("seg");
    o.begin_dict();
    for (auto &kv : ob.seg) {
        o.text(kv.first);
        o.bytes(Bytes(kv.second));
    }
    o.end_dict();
    o.key("segs");
    o.begin_list();
    for (const SegDef &s : ob.segs) {
        o.begin_tuple();
        o.text(s.name);
        o.text(s.cls);
        o.integer(s.length);
        o.integer(s.align);
        o.end_tuple();
    }
    o.end_list();
    o.key("filled");
    o.begin_dict();
    for (auto &kv : ob.filled) {
        o.text(kv.first);
        o.integer(kv.second);
    }
    o.end_dict();
    o.end_dict();
}

void library_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (auto &m : library(c.data)) {
        o.begin_tuple();
        o.text(m.first);
        o.bytes(m.second);
        o.end_tuple();
    }
    o.end_list();
}

int64_t fixlen(const std::string &kind) {             // FIXLEN
    if (kind == "lo") return 1;
    if (kind == "ptr") return 4;
    return 2;                                          // off base rel, and any other kind
}

// holes(o, seg): (o.fix, seg)
void holes_q(Call &c) {
    const Value &fix = c.arg(0), &seg = c.arg(1);
    Out &o = c.out;
    o.begin_set();
    for (const Value &f : fix.items) {
        if (f.items.size() != 5) raise("ValueError", "not enough values to unpack");
        const Value &s = f.items[0];
        if (s.kind != seg.kind || s.s != seg.s) continue;
        const int64_t at = f.items[1].as_int();
        for (int64_t k = at; k < at + fixlen(f.items[2].as_str()); ++k) o.integer(k);
    }
    o.end_set();
}

bool upper_is(const std::string &s, std::initializer_list<const char *> any) {
    std::string u = s;
    for (char &ch : u) ch = char(std::toupper(uint8_t(ch)));
    for (const char *a : any)
        if (u == a) return true;
    return false;
}

// code_segments(o): (o.segs,)
void code_segments_q(Call &c) {
    Out &o = c.out;
    o.begin_list();
    for (const Value &s : c.arg(0).items) {
        const std::string &n = py_at(s.items, 0).as_str(), &cls = py_at(s.items, 1).as_str();
        const bool text = n.size() >= 5 && n.compare(n.size() - 5, 5, "_TEXT") == 0;
        if (text || upper_is(cls, {"CODE", "PROG"}) || upper_is(n, {"PROG", "_PROG", "_CODE"})) o.text(n);
    }
    o.end_list();
}

// functions(o, seg): (o.pubs, len(o.seg.get(seg, b"")), o.filled.get(seg, 0), seg)
void functions_q(Call &c) {
    const Value &pubs = c.arg(0), &seg = c.arg(3);
    std::vector<std::pair<int64_t, std::string>> ps;
    for (const Value &p : pubs.items) {
        const Value &s = p.items.at(1);
        if (s.kind == seg.kind && s.s == seg.s) ps.emplace_back(p.items.at(2).as_int(), p.items.at(0).as_str());
    }
    std::sort(ps.begin(), ps.end());
    const int64_t n = std::min(c.int_arg(1), c.int_arg(2));
    if ((!ps.empty() && ps[0].first > 0) || (ps.empty() && n)) ps.insert(ps.begin(), {0, "(module start)"});
    Out &o = c.out;
    o.begin_list();
    for (size_t k = 0; k < ps.size(); ++k) {
        o.begin_tuple();
        o.text(ps[k].second);
        o.integer(ps[k].first);
        o.integer((k + 1 < ps.size() ? ps[k + 1].first : n) - ps[k].first);
        o.end_tuple();
    }
    o.end_list();
}

void string_set(Out &o, const std::vector<std::string> &v) {
    o.begin_set();
    for (auto &s : v) o.text(s);
    o.end_set();
}

void names_of_q(Call &c) {
    Object ob = read(c.data);
    std::vector<std::string> pubs;
    for (const Public &p : ob.pubs) pubs.push_back(p.name);
    c.out.begin_tuple();
    string_set(c.out, pubs);
    string_set(c.out, ob.ext);
    c.out.end_tuple();
}

void publics_q(Call &c) { string_set(c.out, publics(c.data)); }

void externs_q(Call &c) {
    c.out.begin_list();
    for (auto &e : externs(c.data)) c.out.text(e);
    c.out.end_list();
}

// walk(data, start) -> (records up to the first failure, (exception, message) or None)
void walk_q(Call &c) {
    const Bytes d = c.data;
    Out &o = c.out;
    o.begin_tuple();
    o.begin_list();
    std::optional<PyError> err;
    try {
        for (int64_t i = c.int_arg(0, 0); i < d.size();) {
            const uint8_t t = d.at(i);
            if (t == 0) {
                ++i;
                continue;
            }
            const int64_t n = d.u16(i + 1);
            o.begin_tuple();
            o.integer(i);
            o.integer(t);
            o.bytes(d.slice(i + 3, i + 3 + n - 1));
            o.end_tuple();
            i += 3 + n;
            if (t == 0xF1) break;
        }
    } catch (const PyError &e) {
        err = e;
    }
    o.end_list();
    if (err) {
        o.begin_tuple();
        o.text(err->kind);
        o.text(err->message);
        o.end_tuple();
    } else {
        o.none();
    }
    o.end_tuple();
}

// _code(data): (code segment, its bytes, {offsets the linker fills in}, {public: (segment, offset)})
struct Code {
    std::optional<std::string> seg;
    std::string code;
    std::vector<int64_t> mask;                         // a set, in insertion order
    std::vector<std::pair<std::string, std::pair<std::string, int64_t>>> pubs;
};

int64_t width(int64_t loc) {                           // _WIDTH
    switch (loc) {
    case 0: case 4: return 1;
    case 3: case 9: case 13: return 4;
    case 11: return 6;
    default: return 2;
    }
}

Code code_of(Bytes data) {
    Object ob = read(data);
    Code r;
    for (const Public &p : ob.pubs) {
        std::pair<std::string, int64_t> v{p.segment ? *p.segment : "?", p.offset};
        auto it = std::find_if(r.pubs.begin(), r.pubs.end(), [&](auto &kv) { return kv.first == p.name; });
        if (it != r.pubs.end()) it->second = v;
        else r.pubs.emplace_back(p.name, v);
    }
    for (const SegDef &s : ob.segs) {
        std::string cu = s.cls, nu = s.name;
        for (char &ch : cu) ch = char(std::toupper(uint8_t(ch)));
        for (char &ch : nu) ch = char(std::toupper(uint8_t(ch)));
        if (cu.find("CODE") != std::string::npos || (nu.size() >= 4 && nu.compare(nu.size() - 4, 4, "TEXT") == 0)) {
            r.seg = s.name;
            break;
        }
    }
    if (!r.seg && !ob.segs.empty()) r.seg = ob.segs[0].name;
    if (!r.seg) return r;
    std::set<int64_t> seen;
    for (const Fixup &f : ob.fix)
        if (f.segment == *r.seg)
            for (int64_t k = f.at; k < f.at + width(f.location); ++k)
                if (seen.insert(k).second) r.mask.push_back(k);
    std::string bytes;
    for (auto &kv : ob.seg)
        if (kv.first == *r.seg) bytes = kv.second;
    int64_t n = 0;
    for (auto &kv : ob.filled)
        if (kv.first == *r.seg) n = kv.second;
    r.code = bytes.substr(0, size_t(std::max<int64_t>(0, std::min<int64_t>(n, int64_t(bytes.size())))));
    return r;
}

void pubs_dict(Out &o, const Code &r) {
    o.begin_dict();
    for (auto &kv : r.pubs) {
        o.text(kv.first);
        o.begin_tuple();
        o.text(kv.second.first);
        o.integer(kv.second.second);
        o.end_tuple();
    }
    o.end_dict();
}

void mask_set(Out &o, const std::vector<int64_t> &m) {
    o.begin_set();
    for (int64_t k : m) o.integer(k);
    o.end_set();
}

void code_q(Call &c) {
    Code r = code_of(c.data);
    Out &o = c.out;
    o.begin_tuple();
    if (r.seg) o.text(*r.seg);
    else o.none();
    o.bytes(Bytes(r.code));
    mask_set(o, r.mask);
    pubs_dict(o, r);
    o.end_tuple();
}

void code_segment_q(Call &c) {
    Code r = code_of(c.data);
    Out &o = c.out;
    o.begin_tuple();
    if (r.seg) o.text(*r.seg);
    else o.none();
    o.bytes(Bytes(r.code));
    mask_set(o, r.mask);
    o.end_tuple();
}

void function_bytes_q(Call &c) {
    Code r = code_of(c.data);
    Out &o = c.out;
    o.begin_dict();
    if (r.seg) {
        std::vector<std::pair<int64_t, std::string>> here;
        for (auto &kv : r.pubs)
            if (kv.second.first == *r.seg) here.emplace_back(kv.second.second, kv.first);
        std::sort(here.begin(), here.end());
        std::vector<int64_t> sorted_mask = r.mask;
        std::sort(sorted_mask.begin(), sorted_mask.end());
        std::vector<std::pair<std::string, size_t>> written;   // a name met again: its last cut wins, first place kept
        std::vector<std::pair<std::string, std::pair<std::string, std::vector<int64_t>>>> out;
        for (size_t i = 0; i < here.size(); ++i) {
            const int64_t off = here[i].first;
            const int64_t end = i + 1 < here.size() ? here[i + 1].first : int64_t(r.code.size());
            Bytes code(r.code);
            std::vector<int64_t> m;
            for (int64_t k : sorted_mask)
                if (off <= k && k < end) m.push_back(k - off);
            auto v = std::make_pair(code.slice(off, end).str(), m);
            auto it = std::find_if(out.begin(), out.end(), [&](auto &kv) { return kv.first == here[i].second; });
            if (it != out.end()) it->second = v;
            else out.emplace_back(here[i].second, v);
        }
        for (auto &kv : out) {
            o.text(kv.first);
            o.begin_tuple();
            o.bytes(Bytes(kv.second.first));
            mask_set(o, kv.second.second);
            o.end_tuple();
        }
    }
    o.end_dict();
}

void rename_externs_q(Call &c) {
    std::map<std::string, std::string> mapping;
    const Value &m = c.arg(0);
    for (size_t k = 0; k + 1 < m.items.size(); k += 2) mapping[m.items[k].as_str()] = m.items[k + 1].as_str();
    c.out.bytes(Bytes(rename_externs(c.data, mapping)));
}

std::string u16le(int64_t v) {                        // struct.pack("<H", v)
    if (v < 0 || v > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
    return std::string{char(v), char(v >> 8)};
}

std::string rec(int64_t t, const std::string &payload) { return record(t, Bytes(payload)); }

// the public names of a dict, sorted by value (ties in the dict's order)
std::vector<std::pair<std::string, int64_t>> by_value(const Value &d) {
    std::vector<std::pair<std::string, int64_t>> v;
    for (size_t k = 0; k + 1 < d.items.size(); k += 2) v.emplace_back(name_arg(d.items[k]), d.items[k + 1].as_int());
    std::stable_sort(v.begin(), v.end(), [](auto &a, auto &b) { return a.second < b.second; });
    return v;
}

// raw_object(title, code, pubs, data=b"", dpubs=None, text="_TEXT", dalign=1)
void raw_object_q(Call &c) {
    const std::string title = name_arg(c.arg(0)), code = c.arg(1).as_bytes().str();
    const std::string data = c.argc() > 3 ? c.arg(3).as_bytes().str() : "";
    const std::string text = c.argc() > 5 ? name_arg(c.arg(5)) : "_TEXT";
    const int64_t dalign = c.int_arg(6, 1);
    std::string out = rec(0x80, name(title));
    out += rec(0x96, name("") + name("DGROUP") + name(text) + name("CODE") + name("_DATA") + name("DATA"));
    out += rec(0x98, std::string(1, char(0x28)) + u16le(int64_t(code.size())) + std::string{3, 4, 1});   // byte aligned, public
    out += rec(0x98, std::string(1, char(dalign == 2 ? 0x48 : 0x28)) + u16le(int64_t(data.size())) + std::string{5, 6, 1});
    out += rec(0x9A, std::string{2, char(0xFF), 2});                                                // DGROUP: _DATA
    const std::pair<int, const std::string *> parts[] = {{1, &code}, {2, &data}};
    for (auto &sb : parts)
        for (size_t k = 0; k < sb.second->size(); k += 1000)
            out += rec(0xA0, std::string(1, char(sb.first)) + u16le(int64_t(k)) + sb.second->substr(k, 1000));
    const Value empty;
    const Value &dp = c.argc() > 4 && c.arg(4).truthy() ? c.arg(4) : empty;
    const std::tuple<int, int, const Value *> groups[] = {{0, 1, &c.arg(2)}, {1, 2, &dp}};
    for (auto &[grp, seg, ps] : groups) {
        if (!ps->truthy()) continue;
        std::string body{char(grp), char(seg)};
        for (auto &na : by_value(*ps)) body += name(na.first) + u16le(na.second) + std::string(1, '\0');
        out += rec(0x90, body);
    }
    c.out.bytes(Bytes(out + rec(0x8A, std::string(1, '\0'))));
}

// bss_object(names, start, end, title="bssvars", seg="_BSS", para=False)
void bss_object_q(Call &c) {
    const Value &names = c.arg(0);
    const int64_t start = c.int_arg(1), end = c.int_arg(2);
    const std::string title = c.argc() > 3 ? name_arg(c.arg(3)) : "bssvars";
    const std::string seg = c.argc() > 4 ? name_arg(c.arg(4)) : "_BSS";
    const bool para = c.bool_arg(5);
    std::string out = rec(0x80, name(title));
    out += rec(0x96, name("") + name("DGROUP") + name(seg) + name("BSS"));
    out += rec(0x98, std::string(1, char(para ? 0x68 : 0x48)) + u16le(end - start) + std::string{3, 4, 1});
    out += rec(0x9A, std::string{2, char(0xFF), 1});
    std::vector<std::pair<std::string, int64_t>> items;
    for (size_t k = 0; k + 1 < names.items.size(); k += 2) items.emplace_back(name_arg(names.items[k]), names.items[k + 1].as_int());
    std::sort(items.begin(), items.end(), [](auto &a, auto &b) { return a.second != b.second ? a.second < b.second : a.first < b.first; });
    size_t k = 0;
    while (k < items.size()) {
        std::string body{1, 1};
        while (k < items.size() && body.size() < 900) {
            body += name(items[k].first) + u16le(items[k].second - start) + std::string(1, '\0');
            ++k;
        }
        out += rec(0x90, body);
    }
    c.out.bytes(Bytes(out + rec(0x8A, std::string(1, '\0'))));
}

// ---- patched (a segment's bytes replaced) and rename_symbols (every external and public name) ----
std::pair<int64_t, int64_t> omf_ix(Bytes r, int64_t q) {   // an index: one byte, or two when the top bit is set
    if (r.at(q) < 0x80) return {r.at(q), q + 1};
    return {(int64_t(r.at(q) & 0x7F) << 8) | r.at(q + 1), q + 2};
}
std::pair<int64_t, int64_t> omf_idx(Bytes r, int64_t q) {  // (the same, tested on the top bit)
    if (r.at(q) & 0x80) return {(int64_t(r.at(q) & 0x7F) << 8) | r.at(q + 1), q + 2};
    return {r.at(q), q + 1};
}

// omf.patched(raw, segment, offset, value bytes) -> the module with the value written into the segment's LEDATA
void patched_q(Call &c) {
    const std::string seg = c.str_arg(0);
    const int64_t off = c.int_arg(1);
    const std::string val = c.arg(2).as_bytes().str();
    std::vector<std::string> names, segs;
    std::string out;
    for (const Record &rec : records(c.data)) {
        int64_t t = rec.type;
        std::string r = rec.payload.str();
        Bytes rb(r);
        if (t == 0x96) {
            int64_t q = 0;
            while (q < rb.size()) {
                names.push_back(latin1(rb.slice(q + 1, q + 1 + rb.at(q))));
                q += 1 + rb.at(q);
            }
        } else if (t == 0x98) {
            int64_t q = rb.at(0) >> 5 ? 1 : 4;
            auto ni = omf_ix(rb, q + 2);
            segs.push_back(py_at(names, ni.first - 1));
        } else if (t == 0xA0) {
            auto si = omf_ix(rb, 0);
            int64_t q = si.second;
            int64_t lo = rb.at(q) | (int64_t(rb.at(q + 1)) << 8);
            if (py_at(segs, si.first - 1) == seg) {
                for (size_t k = 0; k < val.size(); ++k)
                    if (lo <= off + int64_t(k) && off + int64_t(k) < lo + int64_t(r.size()) - q - 2) r[size_t(q + 2 + off + int64_t(k) - lo)] = val[k];
            }
        }
        out += record(t, Bytes(r));
    }
    c.out.bytes(Bytes(out));
}

// the walk of rename_symbols: each name passed through `fn` (latin1 text in, latin1 text out)
std::string rename_walk(Bytes data, const std::function<std::string(const std::string &)> &fn) {
    std::string out;
    auto put_name = [&](std::string &nb, Bytes nm, const std::string &rest) {
        std::string neu = to_latin1(fn(latin1(nm)));
        if (neu.size() > 255) raise("ValueError", "bytes must be in range(0, 256)");
        nb += char(neu.size()) + neu + rest;
    };
    for (const Record &rec : records(data)) {
        int64_t t = rec.type;
        std::string r = rec.payload.str();
        Bytes rb(r);
        if (t == 0x8C || t == 0xB0) {
            int64_t q = 0;
            std::string nb;
            while (q < rb.size()) {
                Bytes nm = rb.slice(q + 1, q + 1 + rb.at(q));
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
                Bytes nm = rb.slice(q + 1, q + 1 + rb.at(q));
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
        out += record(t, Bytes(r));
    }
    return out;
}

// omf.symbol_names(data) -> ([the external and public names, in the order rename_symbols meets them],
// None | (exception, message) the walk raised after them): the caller names them in that order before the
// exception is raised, as the one Python walk did
void symbol_names_q(Call &c) {
    Out &o = c.out;
    std::vector<std::string> seen;
    std::string kind, message;
    try {
        rename_walk(c.data, [&](const std::string &n) { seen.push_back(n); return n; });
    } catch (const PyError &e) {
        kind = e.kind;
        message = e.message;
    }
    o.begin_tuple();
    o.begin_list();
    for (auto &n : seen) o.text(n);
    o.end_list();
    if (kind.empty()) o.none();
    else {
        o.begin_tuple();
        o.text(kind);
        o.text(message);
        o.end_tuple();
    }
    o.end_tuple();
}

// omf.rename_symbols(data, [new names in that order]) -> the object
void rename_symbols_q(Call &c) {
    const Value &names = c.arg(0);
    size_t k = 0;
    std::string out = rename_walk(c.data, [&](const std::string &) {
        if (k >= names.items.size()) raise("IndexError", "list index out of range");
        return names.items[k++].as_str();
    });
    c.out.bytes(Bytes(out));
}

}  // namespace

CBF_QUESTIONS(omf) {
    add("omf.patched", patched_q);
    add("omf.symbol_names", symbol_names_q);
    add("omf.rename_symbols", rename_symbols_q);
    add("omf.records", records_q);
    add("omf.record", record_q);
    add("omf.name", name_q);
    add("omf.read", read_q);
    add("omf.library", library_q);
    add("omf.holes", holes_q);
    add("omf.code_segments", code_segments_q);
    add("omf.functions", functions_q);
    add("omf.names_of", names_of_q);
    add("omf.publics", publics_q);
    add("omf.externs", externs_q);
    add("omf.walk", walk_q);
    add("omf._code", code_q);
    add("omf.code_segment", code_segment_q);
    add("omf.function_bytes", function_bytes_q);
    add("omf.rename_externs", rename_externs_q);
    add("omf.raw_object", raw_object_q);
    add("omf.bss_object", bss_object_q);
}

}  // namespace cbf::omf
