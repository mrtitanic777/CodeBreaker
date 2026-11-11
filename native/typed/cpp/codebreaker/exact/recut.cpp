// codebreaker/exact/recut.cpp -- typed C++ port of codebreaker.exact.recut.

#include "codebreaker/exact/recut.hpp"

#include <algorithm>
#include <map>
#include <set>
#include <tuple>
#include <utility>
#include <vector>

#include "codebreaker/exact/omf.hpp"
#include "codebreaker/exact/standin.hpp"

namespace codebreaker::exact::recut {

using standin::Fixup;

namespace {

// bytes(struct.pack("<H", v)): struct.error when v does not fit an unsigned short
py::bytes pack_h(int64_t v) {
    if (v < 0 || v > 65535) throw py::StructError("ushort format requires 0 <= number <= 65535");
    std::string s;
    s.push_back(static_cast<char>(v & 0xFF));
    s.push_back(static_cast<char>((v >> 8) & 0xFF));
    return py::bytes(s);
}

// the word of a segment-or-offset fixup that holds the relocation: `at`, +2 for a pointer (loc 3)
int64_t relw(const Fixup& f) { return *f.at + (f.loc == 3 ? 2 : 0); }

}  // namespace

py::list<int64_t> order(const Project& prj) {
    const std::optional<py::list<int64_t>> packed = prj.packed_relocs();
    if (packed) return py::list<int64_t>(*packed);   // list(prj.packed.relocs)
    const py::list<int64_t> h = prj.header();
    if (h.empty()) return py::list<int64_t>();
    const int64_t at = h[12];
    const py::bytes file = prj.file();
    const py::bytes table = file.slice(at, at + 4 * h[3]);
    // struct.iter_unpack("<HH", table): s * 16 + o for each (o, s)
    if (table.size() % 4 != 0) throw py::StructError("iterative unpacking requires a buffer of a multiple of 4 bytes");
    py::list<int64_t> out;
    for (int64_t i = 0; i < table.size(); i += 4) {
        const int64_t o = table[i] | (table[i + 1] << 8);
        const int64_t s = table[i + 2] | (table[i + 3] << 8);
        out.append(s * 16 + o);
    }
    return out;
}

py::bytes recut(const Project& prj, const py::bytes& raw) {
    const compare::Symbols syms = prj.symbols();
    const omf::Obj o = omf::read(raw);
    const py::list<std::tuple<int64_t, py::bytes>> recs = omf::records(raw);
    std::vector<py::str> names;
    std::vector<py::str> segs;
    standin::Threads thr;
    std::optional<std::tuple<int64_t, int64_t>> last;
    std::map<int64_t, py::list<Fixup>> fix;
    for (const auto& rec : recs) {
        const int64_t t = std::get<0>(rec);
        const py::bytes& r = std::get<1>(rec);
        if (t == 0x96) {
            int64_t q = 0;
            while (q < r.size()) {
                names.push_back(r.slice(q + 1, q + 1 + r[q]).decode("latin1"));
                q += 1 + r[q];
            }
        } else if (t == 0x98) {
            const int64_t q = (r[0] >> 5) ? 1 : 4;
            const int64_t ni = std::get<0>(standin::_idx(r, q + 2));
            const int64_t k = ni - 1 < 0 ? ni - 1 + static_cast<int64_t>(names.size()) : ni - 1;   // names[ni - 1]
            if (k < 0 || k >= static_cast<int64_t>(names.size())) throw py::IndexError("list index out of range");
            segs.push_back(names[static_cast<size_t>(k)]);
        } else if (t == 0xA0 || t == 0xA2) {
            const auto [si, q] = standin::_idx(r, 0);
            last = std::make_tuple(si, r[q] | (r[q + 1] << 8));
        } else if (t == 0x9C) {
            for (Fixup f : standin::fixups(r, thr)) {
                if (!last) throw py::TypeError("'NoneType' object is not subscriptable");
                f.at = std::get<1>(*last) + f.off;
                fix[std::get<0>(*last)].append(f);
            }
        }
    }
    const py::list<int64_t> orig = order(prj);
    struct Plan {
        py::bytes code;
        std::vector<std::pair<int64_t, int64_t>> bounds;
        py::list<Fixup> fl;
        std::map<int64_t, int64_t> rank;
    };
    std::map<int64_t, Plan> plan;
    const py::list<py::str> code_segments = omf::code_segments(o);
    for (size_t idx = 0; idx < segs.size(); ++idx) {
        const int64_t si = static_cast<int64_t>(idx) + 1;
        const py::str& sn = segs[idx];
        bool is_code = false;
        for (const py::str& c : code_segments) {
            if (c == sn) is_code = true;
        }
        const py::bytes segbytes = o.seg.get(sn, py::bytes());
        if (!is_code || segbytes.size() == 0) continue;
        std::vector<std::pair<int64_t, py::str>> pubs;
        for (const auto& p : o.pubs) {
            const py::str& nm = std::get<0>(p);
            const std::optional<py::str>& sg = std::get<1>(p);
            if (sg && *sg == sn && syms.contains(nm)) {
                const py::str& kind = std::get<0>(syms.at(nm));
                if (kind == py::str("C") || kind == py::str("?")) pubs.emplace_back(std::get<2>(p), nm);
            }
        }
        std::sort(pubs.begin(), pubs.end(), [](const std::pair<int64_t, py::str>& a, const std::pair<int64_t, py::str>& b) {
            if (a.first != b.first) return a.first < b.first;
            return a.second.u32() < b.second.u32();
        });
        if (pubs.empty()) continue;
        const int64_t base = std::get<1>(syms.at(pubs[0].second)) - pubs[0].first;
        const int64_t filled = o.filled.get(sn, segbytes.size());
        const py::bytes code = segbytes.slice(0, filled);
        const int64_t n = code.size();
        const py::list<Fixup> fl = fix.count(si) ? fix[si] : py::list<Fixup>();
        std::map<int64_t, Fixup> rel;                       // {relw(f): f for f in fl if f.loc in (2, 3)}
        for (const Fixup& f : fl) {
            if (f.loc == 2 || f.loc == 3) rel[relw(f)] = f;
        }
        std::vector<int64_t> seq;                           // [x - base for x in orig if base <= x < base + n and x - base in rel]
        for (int64_t x : orig) {
            if (base <= x && x < base + n && rel.count(x - base)) seq.push_back(x - base);
        }
        std::vector<int64_t> sorted_seq = seq;
        std::sort(sorted_seq.begin(), sorted_seq.end());
        std::vector<int64_t> sorted_rel;                    // (sorted(rel): its keys)
        for (const auto& kv : rel) sorted_rel.push_back(kv.first);
        if (sorted_seq != sorted_rel) return raw;           // (not the program's relocations: left as it is)
        std::vector<std::vector<int64_t>> runs;
        for (int64_t x : seq) {
            if (!runs.empty() && x < runs.back().back()) runs.back().push_back(x);
            else runs.push_back({x});
        }
        std::vector<std::pair<int64_t, int64_t>> spans;
        for (const Fixup& f : fl) spans.emplace_back(*f.at, *f.at + (f.loc == 3 ? 4 : 2));
        auto ok = [&](int64_t c) {                          // (a cut splits no fixup's word)
            for (const auto& s : spans) {
                if (s.first < c && c < s.second) return false;
            }
            return true;
        };
        std::vector<int64_t> cuts{0};
        for (size_t k = 1; k < runs.size(); ++k) {
            const int64_t lo = *std::max_element(runs[k - 1].begin(), runs[k - 1].end()) + 2;
            const int64_t mn = *std::min_element(runs[k].begin(), runs[k].end());
            const int64_t hi = mn - (rel.at(mn).loc == 3 ? 2 : 0);
            std::optional<int64_t> c;
            for (int64_t v = hi; v >= lo; --v) {
                if (ok(v)) {
                    c = v;
                    break;
                }
            }
            if (!c) return raw;
            cuts.push_back(*c);
        }
        cuts.push_back(n);
        std::vector<std::pair<int64_t, int64_t>> bounds;
        for (size_t k = 0; k + 1 < cuts.size(); ++k) {
            int64_t a = cuts[k];
            const int64_t b = cuts[k + 1];
            while (b - a > 1024) {                          // (no record over a kilobyte)
                std::optional<int64_t> c;
                for (int64_t v = a + 1024; v > a; --v) {
                    if (ok(v)) {
                        c = v;
                        break;
                    }
                }
                if (!c) throw py::StopIteration();          // next(...) of nothing
                bounds.emplace_back(a, *c);
                a = *c;
            }
            bounds.emplace_back(a, b);
        }
        std::map<int64_t, int64_t> rank;
        for (size_t k = 0; k < seq.size(); ++k) rank[seq[k]] = static_cast<int64_t>(k);
        plan[si] = Plan{code, bounds, fl, rank};
    }
    if (plan.empty()) return raw;
    std::string out;
    std::set<int64_t> done;
    std::optional<int64_t> last_seg;
    for (const auto& rec : recs) {
        const int64_t t = std::get<0>(rec);
        const py::bytes& r = std::get<1>(rec);
        if (t == 0xA0 || t == 0xA2) {
            const int64_t si = std::get<0>(standin::_idx(r, 0));
            last_seg = si;
            if (plan.count(si)) {
                if (!done.count(si)) {
                    done.insert(si);
                    const Plan& p = plan.at(si);
                    for (const auto& ab : p.bounds) {
                        const int64_t a = ab.first, b = ab.second;
                        out += (omf::record(0xA0, standin::_ix(si) + pack_h(a) + p.code.slice(a, b))).raw();
                        std::vector<Fixup> relf, other;
                        for (const Fixup& f : p.fl) {       // here = [f for f in fl if a <= f.at < b]
                            if (a <= *f.at && *f.at < b) {
                                if (f.loc == 2 || f.loc == 3) relf.push_back(f);
                                else other.push_back(f);
                            }
                        }
                        std::stable_sort(relf.begin(), relf.end(), [&](const Fixup& x, const Fixup& y) {
                            return p.rank.at(relw(x)) < p.rank.at(relw(y));
                        });
                        std::vector<Fixup> both = relf;
                        both.insert(both.end(), other.begin(), other.end());
                        for (size_t k = 0; k < both.size(); k += 100) {    // (a FIXUPP record of no more than a kilobyte or so)
                            py::list<Fixup> chunk;
                            for (size_t j = k; j < std::min(both.size(), k + 100); ++j) chunk.append(both[j]);
                            out += standin::fixrec(chunk, a).raw();
                        }
                    }
                }
                continue;
            }
        }
        if (t == 0x9C && last_seg && plan.count(*last_seg)) continue;
        out += omf::record(t, r).raw();
    }
    return py::bytes(out);
}

}  // namespace codebreaker::exact::recut
