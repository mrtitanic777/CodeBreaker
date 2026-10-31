// omf.cc -- the OMF reader (omf.h).
#include "omf.h"

#include <algorithm>
#include <functional>
#include <set>

#include "../core/text.h"

namespace cbf::omf {

namespace {
// _idx: an OMF index at q -> (value, the offset past it)
std::pair<int64_t, int64_t> idx(Bytes r, int64_t q) {
    const uint8_t b = r.at(q);
    if (b & 0x80) return {((b & 0x7F) << 8) | r.at(q + 1), q + 2};
    return {b, q + 1};
}

uint64_t le_bytes(Bytes r, int64_t a, int64_t b) { return r.slice(a, b).from_le(); }   // int.from_bytes(r[a:b], "little")

// a dict kept in insertion order: d[k] = v keeps k's first place
template <class V>
void put(std::vector<std::pair<std::string, V>> &d, const std::string &k, V v) {
    for (auto &kv : d)
        if (kv.first == k) {
            kv.second = std::move(v);
            return;
        }
    d.emplace_back(k, std::move(v));
}

// bytearray b[off:off+len(d)] = d, b first grown to reach
void lay(std::string &b, int64_t off, const std::string &d) {
    if (off + int64_t(d.size()) > int64_t(b.size())) b.append(size_t(off + int64_t(d.size()) - int64_t(b.size())), '\0');
    b.replace(size_t(off), d.size(), d);
}

constexpr int64_t kBig = int64_t(1) << 28;   // an expansion past this is the Python reader's to fail on
}  // namespace

bool is_aout(Bytes data) {
    if (data.size() < 16) return false;
    const uint16_t m = data.u16(0);
    return m == 0x107 || m == 0x108 || m == 0x109;
}

std::vector<Record> records(Bytes data) {
    std::vector<Record> out;
    int64_t p = 0;
    while (p + 3 <= data.size()) {
        const uint8_t t = data.at(p);
        const int64_t n = data.at(p + 1) | (data.at(p + 2) << 8);
        out.push_back({t, data.slice(p + 3, p + 3 + n - 1)});
        p += 3 + n;
        if (t == 0x8A || t == 0x8B) break;
    }
    return out;
}

std::string record(int64_t t, Bytes payload) {
    if (t < 0 || t > 255) raise("ValueError", "bytes must be in range(0, 256)");
    const int64_t n = payload.size() + 1;
    if (n > 0xFFFF) raise("struct.error", "'H' format requires 0 <= number <= 65535");
    std::string b(1, char(t));
    b += char(n);
    b += char(n >> 8);
    b.append(reinterpret_cast<const char *>(payload.data()), size_t(payload.size()));
    int sum = 0;
    for (char c : b) sum += uint8_t(c);
    b += char((-sum) & 0xFF);
    return b;
}

std::string name(const std::string &s) {
    if (s.size() > 255) raise("ValueError", "bytes must be in range(0, 256)");
    return std::string(1, char(s.size())) + s;
}

Object read(Bytes data) {
    if (is_aout(data)) fallback("a Version 7 a.out object (exact/aout.py)");
    Object o;
    std::vector<std::string> names, segs;
    std::vector<std::pair<int64_t, std::string>> buf;      // {segment index: its bytes}, in the order made
    std::vector<std::pair<int64_t, int64_t>> filled;
    std::map<std::pair<int, int>, std::pair<std::optional<int64_t>, std::optional<int64_t>>> thr;
    std::optional<std::pair<int64_t, int64_t>> last;
    auto buffer = [&buf](int64_t si) -> std::string & {
        for (auto &kv : buf)
            if (kv.first == si) return kv.second;
        raise("KeyError", dec(si));
    };
    auto note_filled = [&filled](int64_t si, int64_t end) {
        for (auto &kv : filled)
            if (kv.first == si) {
                kv.second = std::max(kv.second, end);
                return;
            }
        filled.emplace_back(si, std::max<int64_t>(0, end));
    };
    for (const Record &rec : records(data)) {
        const uint8_t t = rec.type;
        const Bytes r = rec.payload;
        if (t == 0x96) {                                          // LNAMES
            for (int64_t q = 0; q < r.size(); q += 1 + r.at(q)) names.push_back(latin1(r.slice(q + 1, q + 1 + r.at(q))));
        } else if (t == 0x98 || t == 0x99) {                      // SEGDEF
            const int64_t align = r.at(0) >> 5;
            int64_t q = align ? 1 : 4;
            const int w = t == 0x99 ? 4 : 2;
            int64_t ln = int64_t(le_bytes(r, q, q + w));
            if (r.at(0) & 2) ln = t == 0x98 ? 0x10000 : 0;      // (a 4 GB 32-bit segment: its data say how much)
            auto [ni, q2] = idx(r, q + w);
            auto [ci, q3] = idx(r, q2);
            (void)q3;
            const std::string nm = py_at(names, ni - 1);
            segs.push_back(nm);
            o.segs.push_back({nm, ci ? py_at(names, ci - 1) : "", ln, align});
            put(o.combine, nm, int64_t((r.at(0) >> 2) & 7));       // (5: a stack segment)
            const int64_t si = int64_t(segs.size());
            if (std::none_of(buf.begin(), buf.end(), [si](auto &kv) { return kv.first == si; }))
                buf.emplace_back(si, std::string(size_t(std::max<int64_t>(0, std::min<int64_t>(ln, int64_t(1) << 24))), '\0'));
        } else if ((t == 0x8A || t == 0x8B) && !r.empty() && (r.at(0) & 0x40)) {   // MODEND with the start
            const uint8_t fd = r.at(1);
            int64_t q = 2;
            if (!(fd & 0x80) && ((fd >> 4) & 7) < 3) q = idx(r, q).second;
            if (!(fd & 8)) {
                auto [ti, q2] = idx(r, q);
                q = q2;
                const int64_t disp = !(fd & 4) ? (r.at(q) | (r.at(q + 1) << 8)) : 0;
                if ((fd & 3) == 0 && ti) o.start = std::make_pair(py_at(segs, ti - 1), disp);
            }
        } else if (t == 0x9A) {                                   // GRPDEF
            auto [gi, q] = idx(r, 0);
            std::vector<std::string> mem;
            while (q < r.size()) {
                auto [si, q2] = idx(r, q + 1);
                q = q2;
                mem.push_back(py_at(segs, si - 1));
            }
            put(o.groups, py_at(names, gi - 1), mem);
        } else if (t == 0x8C || t == 0xB0 || t == 0xB4 || t == 0xB5 || t == 0xB8) {   // EXTDEF COMDEF LEXTDEF LCOMDEF
            int64_t q = 0;
            while (q < r.size()) {
                const std::string nm = latin1(r.slice(q + 1, q + 1 + r.at(q)));
                o.ext.push_back(nm);
                q += 1 + r.at(q);
                q = idx(r, q).second;
                if (t == 0xB0 || t == 0xB8) {                     // a communal variable: its kind and size follow
                    const bool far = r.at(q) == 0x61;
                    q += 1;
                    std::vector<int64_t> vals;
                    for (int k = 0; k < (far ? 2 : 1); ++k) {
                        const uint8_t c = r.at(q);
                        const int n = c == 0x81 ? 2 : c == 0x84 ? 3 : c == 0x88 ? 4 : 0;
                        vals.push_back(n ? int64_t(le_bytes(r, q + 1, q + 1 + n)) : c);
                        q += 1 + n;
                    }
                    put(o.comm, nm, std::make_pair(far ? vals[0] * vals[1] : vals[0], far));
                }
            }
        } else if (t == 0xBC) {                                   // CEXTDEF: named by a name index
            int64_t q = 0;
            while (q < r.size()) {
                auto [ni, q2] = idx(r, q);
                q = idx(r, q2).second;
                o.ext.push_back(0 < ni && ni <= int64_t(names.size()) ? names[size_t(ni - 1)] : "?");
            }
        } else if (t == 0x90 || t == 0x91) {                      // PUBDEF
            const int w = t == 0x91 ? 4 : 2;
            auto [gi, q1] = idx(r, 0);
            (void)gi;
            auto [si, q] = idx(r, q1);
            if (si == 0) q += 2;
            while (q < r.size()) {
                const std::string nm = latin1(r.slice(q + 1, q + 1 + r.at(q)));
                q += 1 + r.at(q);
                const int64_t off = int64_t(le_bytes(r, q, q + w));
                q = idx(r, q + w).second;
                o.pubs.push_back({nm, si ? std::optional<std::string>(py_at(segs, si - 1)) : std::nullopt, off});
            }
        } else if (t == 0xA0 || t == 0xA1) {                      // LEDATA
            const int w = t == 0xA1 ? 4 : 2;
            auto [si, q] = idx(r, 0);
            const int64_t off = int64_t(le_bytes(r, q, q + w));
            const std::string d = r.slice(q + w).str();
            std::string &b = buffer(si);
            if (off + int64_t(d.size()) > kBig) fallback("a segment past memory");
            lay(b, off, d);
            last = std::make_pair(si, off);
            note_filled(si, off + int64_t(d.size()));
        } else if (t == 0xA2 || t == 0xA3) {                      // LIDATA: repeated blocks
            const int w = t == 0xA3 ? 4 : 2;
            auto [si, q] = idx(r, 0);
            const int64_t off = int64_t(le_bytes(r, q, q + w));
            q += w;
            std::function<std::string(int64_t &)> block = [&](int64_t &q) -> std::string {
                const int64_t rep = int64_t(le_bytes(r, q, q + w));
                const int64_t cnt = r.at(q + w) | (r.at(q + w + 1) << 8);
                q += w + 2;
                std::string s;
                if (cnt == 0) {
                    s = r.slice(q + 1, q + 1 + r.at(q)).str();
                    q = q + 1 + r.at(q);
                } else {
                    for (int64_t k = 0; k < cnt; ++k) s += block(q);
                }
                if (rep > 0 && int64_t(s.size()) * rep > kBig) fallback("a repeat past memory");
                std::string out;
                for (int64_t k = 0; k < rep; ++k) out += s;
                return out;
            };
            std::string out;
            while (q < r.size()) out += block(q);
            std::string &b = buffer(si);
            if (off + int64_t(out.size()) > kBig) fallback("a segment past memory");
            lay(b, off, out);
            last = std::make_pair(si, off);
            note_filled(si, off + int64_t(out.size()));
        } else if (t == 0x9C || t == 0x9D) {                      // FIXUPP
            const int w = t == 0x9D ? 4 : 2;
            int64_t q = 0;
            while (q < r.size()) {
                const uint8_t c = r.at(q);
                if (!(c & 0x80)) {                                // a thread
                    q += 1;
                    const int d = (c >> 6) & 1, meth = (c >> 2) & 7;
                    std::optional<int64_t> ix;
                    if (!(d && meth >= 4)) {
                        auto [v, q2] = idx(r, q);
                        ix = v;
                        q = q2;
                    }
                    thr[{d, c & 3}] = {meth, ix};
                    continue;
                }
                const int segrel = (c >> 6) & 1, loc = (c >> 2) & 7;
                const int64_t off = ((c & 3) << 8) | r.at(q + 1);
                const uint8_t fd = r.at(q + 2);
                q += 3;
                if (fd & 0x80) {
                    // the frame from a thread: not kept
                } else if (((fd >> 4) & 7) < 3) {
                    q = idx(r, q).second;
                }
                std::optional<int64_t> tm, ti;
                if (fd & 8) {
                    auto it = thr.find({0, fd & 3});
                    if (it != thr.end()) tm = it->second.first, ti = it->second.second;
                } else {
                    tm = fd & 3;
                    auto [v, q2] = idx(r, q);
                    ti = v;
                    q = q2;
                }
                int64_t disp = 0;
                if (!(fd & 4)) {
                    disp = int64_t(le_bytes(r, q, q + w));
                    q += w;
                }
                Target tgt;
                if (!tm) {
                    tgt.kind = "?";
                } else {
                    if (!ti) raise("TypeError", "unsupported operand type(s) for -: 'NoneType' and 'int'");
                    if ((*tm & 3) == 0) {
                        tgt.kind = "seg", tgt.what = Target::Name, tgt.name = py_at(segs, *ti - 1);
                    } else if ((*tm & 3) == 1) {
                        tgt.kind = "grp";
                        if (*ti && *ti - 1 < int64_t(names.size())) tgt.what = Target::Name, tgt.name = py_at(names, *ti - 1);
                        else tgt.what = Target::Number, tgt.number = *ti;
                    } else {
                        tgt.kind = "ext", tgt.what = Target::Name, tgt.name = py_at(o.ext, *ti - 1);
                    }
                }
                std::string kind = loc == 1 || loc == 5 ? "off" : loc == 2 ? "base" : loc == 3 ? "ptr" : loc == 0 ? "lo" : "?" + dec(loc);
                if (!segrel) kind = "rel";
                if (!last) continue;                              // a fix-up before any data: nothing to place it in
                o.fix.push_back({py_at(segs, last->first - 1), last->second + off, kind, tgt, disp, (c >> 2) & 0x0F});
            }
        } else if (t == 0xB2 || t == 0xB3) {                      // BAKPAT: values added in once the segment is written
            const int w = t == 0xB3 ? 4 : 2;
            auto [si, q] = idx(r, 0);
            const uint8_t sz = r.at(q);
            const int size = sz == 0 ? 1 : sz == 1 ? 2 : sz == 2 ? 4 : 2;
            q += 1;
            std::string &b = buffer(si);
            while (q + 2 * w <= r.size()) {
                const int64_t at = int64_t(le_bytes(r, q, q + w)), val = int64_t(le_bytes(r, q + w, q + 2 * w));
                q += 2 * w;
                if (at + size <= int64_t(b.size())) {
                    uint64_t cur = Bytes(reinterpret_cast<const uint8_t *>(b.data()) + at, size_t(size)).from_le();
                    const uint64_t v = (cur + uint64_t(val)) & (size == 4 ? 0xFFFFFFFFull : (1ull << (8 * size)) - 1);
                    for (int k = 0; k < size; ++k) b[size_t(at + k)] = char(v >> (8 * k));
                }
            }
        }
    }
    for (auto &kv : buf) put(o.seg, py_at(segs, kv.first - 1), kv.second);
    for (auto &kv : filled) put(o.filled, py_at(segs, kv.first - 1), kv.second);
    return o;
}

std::vector<std::pair<std::string, Bytes>> library(Bytes data) {
    std::vector<std::pair<std::string, Bytes>> out;
    if (data.at(0) != 0xF0) {
        out.emplace_back("(object)", data);
        return out;
    }
    const int64_t page = (data.at(1) | (data.at(2) << 8)) + 3;
    const int64_t dict_off = data.u32(3);
    std::map<std::string, int64_t> seen;
    int64_t p = page;
    while (p < dict_off) {
        if (data.at(p) != 0x80) {
            p += (p % page) ? page - p % page : page;
            continue;
        }
        int64_t q = p;
        while (q + 3 <= data.size()) {
            const uint8_t t = data.at(q);
            q += 3 + (data.at(q + 1) | (data.at(q + 2) << 8));
            if (t == 0x8A || t == 0x8B) break;
        }
        const std::string nm = latin1(data.slice(p + 4, p + 4 + data.at(p + 3)));
        const int64_t n = ++seen[nm];
        out.emplace_back(nm + (n > 1 ? "#" + dec(n) : ""), data.slice(p, q));
        p = q;
        if (p % page) p += page - p % page;
    }
    return out;
}

std::vector<std::string> publics(Bytes data) {
    std::vector<std::string> out;
    std::set<std::string> seen;
    auto add = [&](std::string s) {
        if (seen.insert(s).second) out.push_back(std::move(s));
    };
    const int64_t n = data.size();
    int64_t i = 0;
    while (i + 3 <= n) {
        const uint8_t rectype = data.at(i);
        const int64_t length = data.at(i + 1) | (data.at(i + 2) << 8);
        const int64_t body = i + 3, end = i + 3 + length;
        if (end > n) break;
        if (rectype == 0x90 || rectype == 0x91) {
            int64_t p = body;
            p = idx(data, p).second;
            auto [seg, p2] = idx(data, p);
            p = p2;
            if (seg == 0) p += 2;                                 // base frame present when segment index == 0
            const int64_t offsz = rectype == 0x90 ? 2 : 4;
            while (p < end - 1) {                                 // end-1 is the record checksum
                const int64_t ln = data.at(p);
                p += 1;
                if (p + ln > end) break;
                add(latin1(data.slice(p, p + ln)));
                p += ln + offsz;
                p = idx(data, p).second;
            }
        }
        i = end;
    }
    return out;
}

std::vector<std::string> externs(Bytes data) {
    std::vector<std::string> out;
    const int64_t n = data.size();
    int64_t i = 0;
    while (i + 3 <= n) {
        const uint8_t rectype = data.at(i);
        const int64_t end = i + 3 + (data.at(i + 1) | (data.at(i + 2) << 8));
        if (end > n) break;
        if (rectype == 0x8C) {
            int64_t q = i + 3;
            while (q < end - 1) {                                 // end-1 is the record checksum
                const int64_t ln = data.at(q);
                out.push_back(latin1(data.slice(q + 1, q + 1 + ln)));
                q += 1 + ln;
                q += (q < end && (data.at(q) & 0x80)) ? 2 : 1;      // type index
            }
        }
        i = end;
    }
    return out;
}

std::string rename_externs(Bytes data, const std::map<std::string, std::string> &mapping) {
    std::string out;
    for (const Record &rec : records(data)) {
        std::string r = rec.payload.str();
        if (rec.type == 0x8C) {
            const Bytes p = rec.payload;
            std::string nb;
            int64_t q = 0;
            while (q < p.size()) {
                std::string nm = latin1(p.slice(q + 1, q + 1 + p.at(q)));
                q += 1 + p.at(q);
                const std::string ty = p.slice(q, q + 1).str();
                q += 1;
                auto it = mapping.find(nm);
                const std::string enc = to_latin1(it != mapping.end() ? it->second : nm);
                if (enc.size() > 255) raise("ValueError", "bytes must be in range(0, 256)");
                nb += char(enc.size());
                nb += enc + ty;
            }
            r = nb;
        }
        out += record(rec.type, Bytes(r));
    }
    return out;
}

}  // namespace cbf::omf
