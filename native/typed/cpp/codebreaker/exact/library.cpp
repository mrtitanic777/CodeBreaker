// codebreaker/exact/library.cpp -- typed C++ port of codebreaker.exact.library (see library.hpp).

#include "codebreaker/exact/library.hpp"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <set>

#include "codebreaker/exact/toolchain.hpp"
#include "codebreaker/exact/x86.hpp"
#include "py_os.hpp"

namespace codebreaker::exact::library {

using py::str;
using l2::Val;

namespace {

str S(const char* s) { return str(s); }

bool has_nonzero(const py::bytes& b) {   // bytes.strip(bytes(1)) is not empty
    for (unsigned char c : b.raw())
        if (c != 0) return true;
    return false;
}

// b[lo:hi] (Python's slice)
py::bytes slice(const py::bytes& b, int64_t lo, int64_t hi) { return b.slice(lo, hi); }

// a dict of (kind, address) the Python's `x in d and d[x][0] == "C"` reads
bool kind_is(const Refs& r, const str& n, const char* kind) { return r.contains(n) && std::get<0>(r.at(n)) == kind; }

}  // namespace

int64_t W(const py::bytes& b, int64_t k) {
    if (k < 0) k += b.size();   // (struct.unpack_from counts a negative offset from the end)
    if (k < 0 || k + 2 > b.size()) throw py::StructError("unpack_from requires a buffer of at least " + std::to_string(k + 2) + " bytes");
    return b.byte_(k) | (b.byte_(k + 1) << 8);
}

// ---------------------------------------------------------------------------------------------- Member
Member::Member(const str& tag_, const str& name_, const py::bytes& raw_, int64_t rank_) : tag(tag_), name(name_), raw(raw_), rank(rank_) {
    o = omf::read(raw);
    for (const str& s : omf::code_segments(o)) {
        if (o.filled.contains(s) && o.filled.at(s) != 0)
            code.append(CodeSeg(s, o.seg.at(s).slice(0, o.filled.at(s)), omf::holes(o, s)));
    }
    py::bytes d = o.seg.contains(S("_DATA")) ? o.seg.at(S("_DATA")) : py::bytes();
    d = d.slice(0, o.filled.contains(S("_DATA")) ? o.filled.at(S("_DATA")) : 0);
    if (!d.empty()) data = DataSeg(d, omf::holes(o, S("_DATA")));
    for (const auto& sg : o.segs) segments.append(std::optional<str>(std::get<0>(sg)));
    for (const auto& p : o.pubs) {
        const std::optional<str>& s = std::get<1>(p);
        if (!segments.contains(s)) segments.append(s);
    }
    py::list<Val> frags;
    for (const auto& p : o.pubs) {
        const std::optional<str>& s = std::get<1>(p);
        l2::Dict origin;
        origin.set(S("module"), Val(label()));
        int64_t at = segments.index(s);
        origin.set(S("segment"), Val(at));
        origin.set(S("offset"), Val(std::get<2>(p)));
        frags.append(Val(l2::fragment(std::nullopt, std::optional<str>(std::get<0>(p)), std::nullopt, std::optional<l2::Dict>(origin),
                                      std::optional<str>(S("library")), s)));
    }
    l2 = l2::module(frags);
    // {public name: (its segment, its offset)}, read from the member's L2 Module
    const Val::List& us = std::get<Val::List>(l2.at(S("units")).v);
    for (const Val& uv : us) {
        const l2::Dict& u = std::get<l2::Dict>(uv.v);
        const l2::Dict& org = std::get<l2::Dict>(u.at(S("origin")).v);
        pubs.set(std::get<str>(u.at(S("name")).v),
                 Pub(segments[std::get<int64_t>(org.at(S("segment")).v)], std::get<int64_t>(org.at(S("offset")).v)));
    }
}

str Member::label() const { return (py::truthy(tag) ? tag + S(":") : S("")) + name; }

std::tuple<int64_t, int64_t> Member::seglen(const str& seg) const {
    for (const auto& sd : o.segs) {
        if (std::get<0>(sd) == seg) {
            int64_t al = std::get<3>(sd);
            int64_t a = al == 1 ? 1 : al == 2 ? 2 : al == 3 ? 16 : al == 4 ? 256 : 1;
            return {std::get<2>(sd), a};
        }
    }
    return {0, 1};
}

// ---------------------------------------------------------------------------------------------- Placed, DataRaw
Placed::Placed(const MemberP& m_, const std::optional<int64_t>& code_, const std::optional<int64_t>& data_, const str& why_)
    : m(m_), code(code_), data(data_), why(why_) {}

std::optional<int64_t> Placed::end() const {
    if (!code) return std::nullopt;
    int64_t n = 0;
    if (!m->code.empty()) n = std::get<1>(m->code[0]).size();   // sum(len(b) for s, b, h in self.m.code[:1])
    return *code + n;
}

DataRaw::DataRaw(const MemberP& m_, int64_t lo, int64_t hi, const py::dict<str, int64_t>& pubs_, const str& why_)
    : m(m_), name(m_ ? m_->name : S("")), raw(m_ ? m_->raw : py::bytes()), data(lo), end(hi), pubs(pubs_), why(why_) {}

std::tuple<int64_t, int64_t> DataRaw::seglen(const str& seg) const {
    if (seg == "_DATA") return {end - data, 2};
    return {0, 1};
}

// ---------------------------------------------------------------------------------------------- the candidates
py::list<std::tuple<str, str>> candidates(const Project& prj) {
    py::list<std::tuple<str, str>> out;
    str model = prj.model();
    for (const LibRef& x : prj.profile_libraries(model)) {
        std::optional<str> kit = prj.profile_kit();
        if (!kit) throw py::KeyError("kit");
        out.append(std::make_tuple(x.tag, toolchain::kit_path(kit, x.path)));
    }
    for (const LibRef& x : prj.project_libraries()) out.append(std::make_tuple(x.tag, toolchain::kit_path(std::nullopt, x.path, prj.dir())));
    return out;
}

py::list<MemberP> modules(const Project& prj) {
    py::list<MemberP> out;
    int64_t rank = 0;
    for (const auto& c : candidates(prj)) {
        const str& tag = std::get<0>(c);
        const str& path = std::get<1>(c);
        auto f = py::open_rb(path);
        py::bytes data = f.read();
        f.close();
        for (const auto& nm : omf::library(data)) {
            try {
                out.append(std::make_shared<Member>(tag, std::get<0>(nm), std::get<1>(nm), rank));
            } catch (const std::exception&) {   // (a record this reader does not know: not a candidate)
            }
        }
        ++rank;
    }
    return out;
}

py::list<int64_t> find(const py::bytes& img, const py::bytes& d, const py::set<int64_t>& holes, int64_t lo, int64_t hi) {
    std::vector<std::tuple<int64_t, int64_t>> runs;
    int64_t k = 0;
    int64_t n = d.size();
    while (k < n) {
        if (holes.contains(k)) {
            ++k;
            continue;
        }
        int64_t e = k;
        while (e < n && !holes.contains(e)) ++e;
        runs.emplace_back(e - k, k);
        k = e;
    }
    if (runs.empty()) return py::list<int64_t>();
    auto best = *std::max_element(runs.begin(), runs.end());   // max(runs): the longest, the later of equals
    int64_t ln = std::get<0>(best), at = std::get<1>(best);
    py::bytes key = d.slice(at, at + ln);
    py::list<int64_t> out;
    int64_t p = img.find(key, lo, std::optional<int64_t>(hi));
    while (p >= 0) {
        int64_t s = p - at;
        if (s >= lo && s + n <= hi) {
            bool all = true;
            for (int64_t x = 0; x < n; ++x) {
                if (holes.contains(x)) continue;
                if (img[s + x] != d[x]) {
                    all = false;
                    break;
                }
            }
            if (all) out.append(s);
        }
        p = img.find(key, p + 1, std::optional<int64_t>(hi));
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- locate
MapP locate(const Project& prj, bool) {
    if (!prj._library) prj._library = _locate(prj);
    return prj._library;
}

MapP _locate(const Project& prj) {
    py::bytes img = prj.image();
    int64_t dg = prj.dgroup();
    modules::Symbols syms = prj.symbols();
    py::dict<int64_t, str> code_at;
    for (const auto& kv : syms.items()) {
        const str& n = std::get<0>(kv);
        const auto& ta = std::get<1>(kv);
        if (std::get<0>(ta) == "C" && !n.contains(S(":"))) code_at.set(std::get<1>(ta), n);
    }
    py::list<MemberP> mods = modules(prj);
    py::dict<int64_t, py::list<MemberP>> hits;                  // code place -> [modules]
    py::list<std::tuple<MemberP, py::list<int64_t>>> multi;
    std::map<const Member*, py::list<int64_t>> dhits;
    for (const MemberP& m : mods) {
        if (!m->code.empty()) {
            const CodeSeg& c0 = m->code[0];
            const py::bytes& b = std::get<1>(c0);
            const py::set<int64_t>& h = std::get<2>(c0);
            if (b.size() - h.size() < 3) continue;
            py::list<int64_t> ps = find(img, b, h, 0, dg);
            if (ps.size() == 1) {
                if (!hits.contains(ps[0])) hits.set(ps[0], py::list<MemberP>());
                hits.at(ps[0]).append(m);
            } else if (!ps.empty()) {
                multi.append(std::make_tuple(m, ps));
            }
        }
        if (m->data && std::get<0>(*m->data).size() - std::get<1>(*m->data).size() >= 4) {
            py::list<int64_t> r;
            for (int64_t p : find(img, std::get<0>(*m->data), std::get<1>(*m->data), dg, img.size())) r.append(p - dg);
            dhits[m.get()] = r;
        }
    }

    auto callers_name = [&](const MemberP& m, int64_t at) {
        const str& s0 = std::get<0>(m->code[0]);
        for (const auto& kv : m->pubs.items()) {
            const auto& sp = std::get<1>(kv);
            if (!(std::get<0>(sp) && *std::get<0>(sp) == s0)) continue;
            if (code_at.contains(at + std::get<1>(sp)) && code_at.at(at + std::get<1>(sp)) == std::get<0>(kv)) return true;
        }
        return false;
    };

    py::dict<int64_t, PlacedP> placed;
    for (const auto& kv : hits.items()) {
        int64_t at = std::get<0>(kv);
        std::vector<MemberP> ms = std::get<1>(kv).vec();
        std::vector<std::tuple<bool, int64_t>> keys;
        for (const MemberP& m : ms) keys.emplace_back(!callers_name(m, at), m->rank);
        std::vector<size_t> idx(ms.size());
        for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
        std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return keys[a] < keys[b]; });
        py::list<MemberP> sorted_ms;
        for (size_t i : idx) sorted_ms.append(ms[i]);
        py::list<int64_t> dp = dhits.count(sorted_ms[0].get()) ? dhits[sorted_ms[0].get()] : py::list<int64_t>();
        PlacedP p = std::make_shared<Placed>(sorted_ms[0], std::optional<int64_t>(at), dp.size() == 1 ? std::optional<int64_t>(dp[0]) : std::nullopt);
        p->also = sorted_ms.slice(1, sorted_ms.size());
        placed.set(at, p);
    }
    for (const auto& mp_ : multi) {   // (a module whose bytes stand in several places)
        const MemberP& m = std::get<0>(mp_);
        py::list<int64_t> free_, named;
        for (int64_t a : std::get<1>(mp_))
            if (!placed.contains(a)) free_.append(a);
        for (int64_t a : free_)
            if (callers_name(m, a)) named.append(a);
        if (named.size() == 1 || free_.size() == 1) {
            int64_t a = (!named.empty() ? named : free_)[0];
            placed.set(a, std::make_shared<Placed>(m, std::optional<int64_t>(a), std::nullopt,
                                                   py::percent(S("(by %s)"), !named.empty() ? S("its name in SYMBOLS") : S("the one place left"))));
        }
    }
    auto placed_values = [&]() {
        py::list<PlacedP> v;
        for (const auto& kv : placed.items()) v.append(std::get<1>(kv));
        return v;
    };
    Refs refs = _refs(prj, placed_values());
    // a module too short to be told by its bytes (a jump to another routine): placed by its names' places
    for (const MemberP& m : mods) {
        if (m->code.empty()) continue;
        bool in_placed = false;
        for (const auto& kv : placed.items()) {
            const PlacedP& p = std::get<1>(kv);
            if (p->m == m) in_placed = true;
            for (const MemberP& x : p->also)
                if (x == m) in_placed = true;
        }
        if (in_placed) continue;
        const str& s0 = std::get<0>(m->code[0]);
        const py::bytes& b = std::get<1>(m->code[0]);
        const py::set<int64_t>& h = std::get<2>(m->code[0]);
        bool in_multi = false;
        for (const auto& x : multi)
            if (std::get<0>(x) == m) in_multi = true;
        if (b.size() - h.size() >= 3 && !in_multi) continue;
        for (const auto& kv : m->pubs.items()) {
            const str& n = std::get<0>(kv);
            const auto& sp = std::get<1>(kv);
            int64_t off = std::get<1>(sp);
            std::optional<str> source;
            if (syms.contains(n) && std::get<0>(syms.at(n)) == "C") source = S("SYMBOLS");
            else if (kind_is(refs, n, "C")) source = S("a call of the library's");
            if (!(std::get<0>(sp) && *std::get<0>(sp) == s0) || !source) continue;
            int64_t a = (*source == "SYMBOLS" ? std::get<1>(syms.at(n)) : std::get<1>(refs.at(n))) - off;
            if (placed.contains(a) || a + b.size() > img.size()) continue;
            bool ok = true;
            for (int64_t x = 0; x < b.size(); ++x) {
                if (h.contains(x)) continue;
                if (img[a + x] != b[x]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                placed.set(a, std::make_shared<Placed>(m, std::optional<int64_t>(a), std::nullopt, py::percent(S("(by %s, which %s names)"), n, *source)));
                break;
            }
        }
    }
    py::list<PlacedP> coded;
    {
        std::vector<int64_t> keys = placed.keys();
        std::sort(keys.begin(), keys.end());
        for (int64_t a : keys) coded.append(placed.at(a));
    }
    // the library part: the run of modules at the end of the code
    int64_t gap = prj.library_gap();
    std::optional<int64_t> start;
    if (auto ls = prj.library_start()) {
        if (std::holds_alternative<py::str>(*ls)) start = py::int_parse(std::get<py::str>(*ls), 0);
        else start = std::get<int64_t>(*ls);
    }
    if (!start && !coded.empty()) {
        int64_t k = coded.size() - 1;
        while (k > 0 && *coded[k]->code - *coded[k - 1]->end() <= gap) k -= 1;
        start = coded[k]->code;
    }
    py::list<PlacedP> lone;
    for (const PlacedP& p : coded)
        if (start && *p->code < *start) lone.append(p);   // (a library module among the program's own)
    py::dict<str, int64_t> lib_pubs;
    for (const PlacedP& p : coded) {
        if (start && *p->code >= *start) {
            py::list<MemberP> ms{p->m};
            ms.extend(p->also);
            for (const MemberP& m : ms)
                for (const auto& kv : m->pubs.items())
                    if (!lib_pubs.contains(std::get<0>(kv))) lib_pubs.set(std::get<0>(kv), *p->code + std::get<1>(std::get<1>(kv)));
        }
    }
    // a lone module counts when what it calls is in the library part (a wrapper of the program's own that has
    // a library routine's bytes calls the program's code: P2's f05a85, fscanf's bytes)
    auto calls_library = [&](const PlacedP& p) {
        py::list<PlacedP> one{p};
        for (const auto& kv : _refs(prj, one).items()) {
            const str& n = std::get<0>(kv);
            const auto& ta = std::get<1>(kv);
            if (std::get<0>(ta) == "C" && (std::get<1>(ta) < *start || (lib_pubs.contains(n) && lib_pubs.at(n) != std::get<1>(ta)))) return false;
        }
        return true;
    };
    {
        py::list<PlacedP> keep;
        for (const PlacedP& p : lone)
            if (calls_library(p)) keep.append(p);
        lone = keep;
    }
    {
        py::list<PlacedP> keep;
        for (const PlacedP& p : coded)
            if (!start || *p->code >= *start) keep.append(p);
        coded = keep;
    }
    // every module of the same bytes at a place is one of its group (atoi and atol: one jump to __catox) ...
    std::set<const Member*> inplace;
    for (const PlacedP& p : coded) {
        inplace.insert(p->m.get());
        for (const MemberP& m : p->also) inplace.insert(m.get());
    }
    for (const PlacedP& p : coded) {
        const py::bytes& b = std::get<1>(p->m->code[0]);
        const py::set<int64_t>& h = std::get<2>(p->m->code[0]);
        for (const MemberP& m : mods) {
            if (inplace.count(m.get()) || m->code.empty() || std::get<1>(m->code[0]).size() != b.size()) continue;
            const py::bytes& mb = std::get<1>(m->code[0]);
            const py::set<int64_t>& mh = std::get<2>(m->code[0]);
            if (!(mh == h)) continue;
            bool ok = true;
            for (int64_t k = 0; k < b.size(); ++k) {
                if (h.contains(k)) continue;
                if (mb[k] != b[k]) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                p->also.append(m);
                inplace.insert(m.get());
            }
        }
    }
    // ... and of a group the one taken is the one whose calls go where the modules they name are
    for (const PlacedP& p : coded) {
        if (p->also.empty()) continue;
        auto agrees = [&](const MemberP& m) {
            int64_t n = 0;
            py::list<PlacedP> one{std::make_shared<Placed>(m, p->code)};
            for (const auto& kv : _refs(prj, one).items()) {
                const str& nm = std::get<0>(kv);
                const auto& ta = std::get<1>(kv);
                if (std::get<0>(ta) == "C" && lib_pubs.contains(nm)) n += lib_pubs.at(nm) == std::get<1>(ta) ? 1 : -1;
            }
            return n;
        };
        py::list<MemberP> group{p->m};
        group.extend(p->also);
        MemberP best;
        std::tuple<int64_t, bool> best_key;
        for (const MemberP& m : group) {
            std::tuple<int64_t, bool> key(agrees(m), m == p->m);
            if (!best || key > best_key) {
                best = m;
                best_key = key;
            }
        }
        if (best != p->m) {
            py::list<MemberP> rest;
            for (const MemberP& m : group)
                if (m != best) rest.append(m);
            p->also = rest;
            p->m = best;
            p->why = (py::truthy(p->why) ? p->why + S(" ") : S("")) + S("(of the same bytes, the one whose calls go where their names are)");
        }
    }
    refs = _refs(prj, coded);
    py::set<const Member*> taken;
    for (const PlacedP& p : coded) {
        taken.add(p->m.get());
        for (const MemberP& m : p->also) taken.add(m.get());
    }
    // modules of data only (by their _DATA, or another data segment's bytes: the 8087 fixups' CDATA)
    py::list<PlacedP> donly, first;
    for (const MemberP& m : mods) {
        if (!m->code.empty() || taken.contains(m.get())) continue;
        std::optional<std::tuple<int64_t, str>> got;
        py::list<int64_t> dp = dhits.count(m.get()) ? dhits[m.get()] : py::list<int64_t>();
        if (dp.size() == 1) {
            got = std::make_tuple(dp[0], S(""));
        } else {
            for (const auto& kv : m->pubs.items()) {
                const str& n = std::get<0>(kv);
                const auto& sp = std::get<1>(kv);
                if (!(std::get<0>(sp) && *std::get<0>(sp) == "_DATA")) continue;
                int64_t off = std::get<1>(sp);
                std::optional<int64_t> a;
                if (syms.contains(n) && std::get<0>(syms.at(n)) == "D") a = std::get<1>(syms.at(n));
                else if (kind_is(refs, n, "D")) a = std::get<1>(refs.at(n));
                if (!a) continue;
                int64_t base = *a - off;
                bool ok = syms.contains(n);
                if (!ok && m->data) {
                    ok = true;
                    const py::bytes& d0 = std::get<0>(*m->data);
                    for (int64_t x = 0; x < d0.size(); ++x) {
                        if (std::get<1>(*m->data).contains(x)) continue;
                        if (!(dg + base + x < img.size() && img[dg + base + x] == d0[x])) {
                            ok = false;
                            break;
                        }
                    }
                }
                if (ok) {
                    got = std::make_tuple(base, py::percent(S("(by %s, %04x%s)"), n, *a, syms.contains(n) ? S(" in SYMBOLS") : S(": a word of the code")));
                    break;
                }
            }
        }
        if (got) {
            PlacedP same;
            for (const PlacedP& d : donly)
                if (d->data == std::get<0>(*got) && d->m->seglen(S("_DATA")) == m->seglen(S("_DATA"))) {
                    same = d;
                    break;
                }
            if (same) same->also.append(m);   // (the same data in another library: one module)
            else donly.append(std::make_shared<Placed>(m, std::nullopt, std::optional<int64_t>(std::get<0>(*got)), std::get<1>(*got)));
            taken.add(m.get());
            continue;
        }
        struct Other {
            str s;
            py::bytes d;
            py::set<int64_t> h;
        };
        std::vector<Other> other;
        for (const auto& sd : m->o.segs) {
            const str& s = std::get<0>(sd);
            if (s != "_DATA" && m->o.filled.contains(s) && m->o.filled.at(s) != 0)
                other.push_back(Other{s, m->o.seg.at(s).slice(0, m->o.filled.at(s)), omf::holes(m->o, s)});
        }
        bool all_one = true, any_big = false;
        for (const Other& x : other) {
            if (x.d.size() - x.h.size() >= 4) {
                any_big = true;
                if (find(img, x.d, x.h, dg, img.size()).size() != 1) all_one = false;
            }
        }
        if (!other.empty() && all_one && any_big) {
            py::list<str> names;
            for (const Other& x : other) names.append(x.s);
            first.append(std::make_shared<Placed>(m, std::nullopt, std::nullopt, py::percent(S("(no code: its %s found in the data)"), py::join(S(", "), names))));
            taken.add(m.get());
        }
    }
    // a module its bytes cannot tell (none, or zeros the linker fills) is taken
    //  - when a placed module names one of its publics and nothing placed (or the kit's objects linked first)
    //    defines it: the 8087 fixups' FIxRQQ, which the emulator names
    //  - when it names a placed module's public that nothing else placed names (it is what took that module):
    //    __fltused's module, which names fakcvt's routines
    py::set<str> defined = _first_object_names(prj);
    py::dict<str, py::list<str>> named;
    {
        py::list<PlacedP> all;
        all.extend(coded);
        all.extend(donly);
        all.extend(first);
        for (const PlacedP& p : all) {
            py::list<MemberP> ms{p->m};
            ms.extend(p->also);
            for (const MemberP& m : ms)
                for (const str& n : m->pubs) defined.add(n);
            for (const str& e : p->m->o.ext) {   // (of a group of the same bytes, the one taken)
                if (!named.contains(e)) named.set(e, py::list<str>());
                named.at(e).append(p->m->label());
            }
        }
    }
    auto byteless = [&](const MemberP& m) {
        if (!m->code.empty()) return false;
        for (const str& s : m->o.seg) {
            int64_t n = m->o.filled.contains(s) ? m->o.filled.at(s) : 0;
            if (has_nonzero(m->o.seg.at(s).slice(0, n))) return false;
        }
        return true;
    };
    for (const MemberP& m : mods) {
        if (!byteless(m) || taken.contains(m.get()) || m->pubs.empty()) continue;
        std::vector<str> keys = m->pubs.keys();
        std::sort(keys.begin(), keys.end());
        py::list<str> why;
        for (const str& n : keys)
            if (named.contains(n) && !defined.contains(n)) why.append(n);
        if (!why.empty()) {
            first.append(std::make_shared<Placed>(m, std::nullopt, std::nullopt,
                                                  py::percent(S("(its bytes tell nothing: %s, which %s names)"), why[0], named.at(why[0])[0])));
            taken.add(m.get());
            for (const str& n : m->pubs) defined.add(n);
        }
    }
    {
        py::list<PlacedP> cd;
        cd.extend(coded);
        cd.extend(donly);
        for (const PlacedP& p : cd) {
            py::set<str> own;
            own.add(p->m->label());
            for (const MemberP& m : p->also) own.add(m->label());
            py::list<str> alone;
            py::list<MemberP> ms{p->m};
            ms.extend(p->also);
            for (const MemberP& m : ms)
                for (const str& n : m->pubs) {
                    bool others = false;
                    if (named.contains(n))
                        for (const str& who : named.at(n))
                            if (!own.contains(who)) others = true;
                    if (!others) alone.append(n);
                }
            for (const MemberP& m : mods) {
                if (!byteless(m) || taken.contains(m.get()) || m->pubs.empty()) continue;
                py::list<str> took;
                for (const str& e : m->o.ext)
                    if (alone.contains(e) && !m->pubs.contains(e)) took.append(e);
                if (!took.empty()) {
                    first.append(std::make_shared<Placed>(
                        m, std::nullopt, std::nullopt,
                        py::percent(S("(its bytes tell nothing: it took %s, for %s, which no other module names)"), p->m->label(), took[0])));
                    taken.add(m.get());
                }
            }
        }
    }
    {
        std::vector<PlacedP> fv = first.vec();
        std::vector<size_t> idx(fv.size());
        std::vector<std::tuple<int64_t, int64_t>> keys;
        for (const PlacedP& p : fv) {
            int64_t at = 0;
            for (int64_t i = 0; i < mods.size(); ++i)
                if (mods[i] == p->m) {
                    at = i;
                    break;
                }
            keys.emplace_back(p->m->rank, at);
        }
        for (size_t i = 0; i < idx.size(); ++i) idx[i] = i;
        std::stable_sort(idx.begin(), idx.end(), [&](size_t a, size_t b) { return keys[a] < keys[b]; });
        py::list<PlacedP> sorted_first;
        for (size_t i : idx) sorted_first.append(fv[i]);
        first = sorted_first;
    }
    // names the placed modules need that nothing placed defines, and the library modules that would: another
    // version of those is in the program (a stand-in), or the original link left them unresolved too
    for (const PlacedP& p : first)
        for (const str& n : p->m->pubs) defined.add(n);
    py::dict<str, std::tuple<str, py::list<str>>> missing;
    {
        std::vector<str> names = named.keys();
        std::sort(names.begin(), names.end());
        for (const str& n : names) {
            if (!defined.contains(n) && !n.startswith(std::make_tuple(S("FI"), S("FJ")))) {
                std::vector<str> labels;
                for (const MemberP& m : mods)
                    if (m->pubs.contains(n)) labels.push_back(m->label());
                std::sort(labels.begin(), labels.end());
                labels.erase(std::unique(labels.begin(), labels.end()), labels.end());
                missing.set(n, std::make_tuple(named.at(n)[0], py::list<str>(labels)));
            }
        }
    }
    // stand-ins for the gaps: not zeros (a pad, a segment of zeros), nor a placed module's far data
    std::vector<std::tuple<int64_t, int64_t>> far;
    for (const PlacedP& p : coded) {
        for (const auto& sd : p->m->o.segs) {
            const str& s = std::get<0>(sd);
            const str& cls = std::get<1>(sd);
            int64_t ln = std::get<2>(sd);
            py::bytes d = (p->m->o.seg.contains(s) ? p->m->o.seg.at(s) : py::bytes()).slice(0, p->m->o.filled.contains(s) ? p->m->o.filled.at(s) : 0);
            bool is_code = false;
            for (const CodeSeg& c : p->m->code)
                if (std::get<0>(c) == s) is_code = true;
            str up = cls.upper();
            if (!is_code && !(up == "DATA" || up == "BSS" || up == "STACK" || up == "CONST") && has_nonzero(d)) {
                py::list<int64_t> ps = find(img, d, omf::holes(p->m->o, s), *start, dg);
                if (ps.size() == 1) far.emplace_back(ps[0], ps[0] + ln);
            }
        }
    }
    py::list<Raw> raws;
    if (!coded.empty()) {
        int64_t end = prj.code_end() != 0 ? prj.code_end() : _code_end(img, dg);
        if (!far.empty()) {   // (the classes after CODE - far data - begin there)
            int64_t m = std::get<0>(far[0]);
            for (const auto& f : far) m = std::min(m, std::get<0>(f));
            end = std::min(end, m);
        }
        py::list<int64_t> later;
        for (int64_t a : modules::starts(prj))
            if (a > *start) later.append(a);
        if (!later.empty()) {   // (the program's own modules after the library: a code segment of their own, placed later by the linker)
            int64_t m = later[0];
            for (int64_t a : later) m = std::min(m, a);
            end = std::min(end, m);
        }
        py::list<str> segs;
        for (const PlacedP& p : coded) segs.append(std::get<0>(p->m->code[0]));
        str main_seg = segs[0];
        {
            int64_t best = -1;
            for (const str& s : segs) {
                int64_t c = segs.count(s);
                if (c > best) {
                    best = c;
                    main_seg = s;
                }
            }
        }
        if (segs[-1] != main_seg) end = std::min(end, *coded[-1]->end());   // (the last is a code segment of its own, after the main one: what follows is of the classes after CODE)
        py::set<str> pubs_all;
        for (const PlacedP& q : coded) {
            py::list<MemberP> ms{q->m};
            ms.extend(q->also);
            for (const MemberP& m : ms)
                for (const str& n : m->pubs) pubs_all.add(n);
        }
        int64_t pos = *start;
        std::vector<std::tuple<int64_t, int64_t>> far_sorted = far;
        std::sort(far_sorted.begin(), far_sorted.end());
        for (int64_t pi = 0; pi <= coded.size(); ++pi) {
            PlacedP p = pi < coded.size() ? coded[pi] : nullptr;
            int64_t nxt = p ? *p->code : end;
            if (nxt > pos) {
                int64_t lo = pos;
                for (const auto& ab : far_sorted) {
                    int64_t a = std::get<0>(ab), b = std::get<1>(ab);
                    if ((a <= lo && lo < b) || (lo < a && a < nxt && !has_nonzero(slice(img, lo, a)))) lo = std::max(lo, b);
                }
                int64_t hi = nxt;
                while (hi - lo > 1 && img[hi - 1] == 0) hi -= 1;   // (zeros before the next module: pads, not the stand-in's)
                if ((hi & 1) && hi < nxt && (lo & 1) == 0) hi += 1;   // (but a module's code of its own is of even length)
                if (has_nonzero(slice(img, lo, nxt))) {
                    pos = lo;
                    Refs known = refs.copy();
                    for (const auto& kv : syms.items())
                        if (!std::get<0>(kv).contains(S(":"))) known.set(std::get<0>(kv), std::get<1>(kv));
                    std::vector<std::tuple<int64_t, str>> pubs;
                    for (const auto& kv : known.items()) {
                        const auto& ta = std::get<1>(kv);
                        int64_t a = std::get<1>(ta);
                        if (std::get<0>(ta) == "C" && pos <= a && a < hi && !pubs_all.contains(std::get<0>(kv))) pubs.emplace_back(a - pos, std::get<0>(kv));
                    }
                    std::sort(pubs.begin(), pubs.end());
                    raws.append(Raw(pos, hi, py::list<std::tuple<int64_t, str>>(pubs)));
                }
            }
            if (p) pos = std::max(pos, *p->end());
        }
    }
    auto mp = std::make_shared<Map>();
    mp->start = start;
    mp->coded = coded;
    mp->donly = donly;
    mp->first = first;
    mp->raws = raws;
    mp->refs = refs;
    mp->missing = missing;
    mp->lone = lone;
    {
        py::list<PlacedP> cd;
        cd.extend(coded);
        cd.extend(donly);
        mp->allrefs = _refs(prj, cd);
    }
    mp->objects = _objects(prj, img, dg);
    mp->dataraws = _data_standins(prj, mods, taken, missing, mp->allrefs, coded, donly);
    for (const DataRawP& d : mp->dataraws)
        for (const str& n : d->pubs) missing.pop(n, std::make_tuple(S(""), py::list<str>()));
    mp->rawdata = _raw_data(prj, mods, raws, img.slice_from(dg));
    for (const Raw& r : raws)
        for (const auto& on : std::get<2>(r)) missing.pop(std::get<1>(on), std::make_tuple(S(""), py::list<str>()));
    mp->missing = missing;
    py::list<Block> blocks;
    for (const PlacedP& p : donly) blocks.append(Block(p));
    for (const DataRawP& d : mp->dataraws) blocks.append(Block(d));
    for (const PlacedP& p : first) mp->order.append(Row(std::nullopt, S("first"), Item(p)));
    mp->order.extend(_order(coded, blocks, raws, mp->rawdata));
    for (const DataRawP& d : mp->dataraws) {   // (the modules after the stand-in, before the next known data, have their _DATA there too: it ends before them)
        if (!d->by_next) continue;
        int64_t k = -1;
        for (int64_t i = 0; i < mp->order.size(); ++i) {
            const Item& it = std::get<2>(mp->order[i]);
            if (std::holds_alternative<DataRawP>(it) && std::get<DataRawP>(it) == d) {
                k = i;
                break;
            }
        }
        if (k < 0) throw py::ValueError("is not in list");
        int64_t room = 0;
        for (int64_t i = k + 1; i < mp->order.size(); ++i) {
            const Row& row = mp->order[i];
            const Item& it = std::get<2>(row);
            bool has_data = false;
            if (auto p = std::get_if<PlacedP>(&it)) has_data = (*p)->data.has_value();
            else if (std::holds_alternative<DataRawP>(it)) has_data = true;
            if (has_data) break;
            if (std::get<1>(row) == "mod") {
                auto [ln, al] = std::get<PlacedP>(it)->m->seglen(S("_DATA"));
                room += ln + (ln != 0 && al == 2 ? (room & 1) : 0);
            }
        }
        d->end -= room;
        py::dict<str, int64_t> keep;
        for (const auto& kv : d->pubs.items())
            if (std::get<1>(kv) < d->end - d->data) keep.set(std::get<0>(kv), std::get<1>(kv));
        d->pubs = keep;
    }
    return mp;
}

// ---------------------------------------------------------------------------------------------- data stand-ins
py::list<DataRawP> _data_standins(const Project& prj, const py::list<MemberP>& mods, py::set<const Member*>& taken,
                                  py::dict<str, std::tuple<str, py::list<str>>>& missing, const Refs& refs, const py::list<PlacedP>& coded,
                                  const py::list<PlacedP>& donly) {
    py::list<DataRawP> out;
    std::vector<int64_t> starts;
    {
        py::list<PlacedP> cd;
        cd.extend(coded);
        cd.extend(donly);
        std::set<int64_t> ss;
        for (const PlacedP& p : cd)
            if (p->data) ss.insert(*p->data);
        starts.assign(ss.begin(), ss.end());
    }
    for (const MemberP& m : mods) {
        if (!m->code.empty() || taken.contains(m.get()) || !m->data) continue;
        bool any_missing = false;
        for (const str& n : m->pubs)
            if (missing.contains(n)) any_missing = true;
        if (!any_missing) continue;
        std::vector<std::tuple<int64_t, str>> kit;
        for (const auto& kv : m->pubs.items()) {
            const auto& sp = std::get<1>(kv);
            if (std::get<0>(sp) && *std::get<0>(sp) == "_DATA") kit.emplace_back(std::get<1>(sp), std::get<0>(kv));
        }
        std::sort(kit.begin(), kit.end());
        std::vector<std::tuple<int64_t, str, int64_t>> placed;
        for (const auto& on : kit) {
            const str& n = std::get<1>(on);
            if (kind_is(refs, n, "D")) placed.emplace_back(std::get<0>(on), n, std::get<1>(refs.at(n)));
        }
        if (placed.empty()) continue;
        int64_t lo = std::get<2>(placed[0]) - std::get<0>(placed[0]);
        int64_t span = std::get<0>(m->seglen(S("_DATA")));   // (a public placed far off is not in this version's module)
        {
            std::vector<std::tuple<int64_t, str, int64_t>> keep;
            for (const auto& t : placed)
                if (lo <= std::get<2>(t) && std::get<2>(t) < lo + span + 16) keep.push_back(t);
            placed = keep;
        }
        int64_t moved = 1 << 16;
        for (const auto& t : placed)
            if (std::get<2>(t) - lo != std::get<0>(t)) moved = std::min(moved, std::get<0>(t));
        std::set<int64_t> shifts;
        for (const auto& t : placed)
            if (std::get<0>(t) >= moved) shifts.insert(std::get<2>(t) - lo - std::get<0>(t));
        int64_t hi = 1 << 16;
        for (int64_t s : starts)
            if (s > lo) hi = std::min(hi, s);
        bool by_next = true;
        if (shifts.size() <= 1) {   // (what moved, moved alike: the version's length is the kit's and that much)
            int64_t own = lo + std::get<0>(m->seglen(S("_DATA"))) + (shifts.empty() ? 0 : *shifts.begin());
            if (own <= hi) {
                hi = own;
                by_next = false;
            }
        }
        py::dict<str, int64_t> pubs;
        for (const auto& t : placed)
            if (lo <= std::get<2>(t) && std::get<2>(t) < hi) pubs.set(std::get<1>(t), std::get<2>(t) - lo);
        for (const auto& on : kit) {   // (a public the words place elsewhere is not this one's)
            if (std::get<0>(on) < moved && !pubs.contains(std::get<1>(on)) && !refs.contains(std::get<1>(on))) pubs.set(std::get<1>(on), std::get<0>(on));
        }
        str why = py::percent(S("(another version of %s: its data as the program has it%s)"), m->label(),
                              moved < (1 << 16) ? py::percent(S(", the publics from +%x on moved"), moved) : S(""));
        auto d = std::make_shared<DataRaw>(m, lo, hi, pubs, why);
        d->by_next = by_next;
        out.append(d);
        taken.add(m.get());
    }
    // a needed name the words place where no module's data is (another version keeps it in a module of its own):
    // a stand-in of its own, as long as the kit's module gives it (the link's drift map tells if it is more)
    std::vector<std::tuple<int64_t, int64_t>> covered;
    py::set<str> have;
    for (const DataRawP& d : out) {
        covered.emplace_back(d->data, d->end);
        for (const str& n : d->pubs) have.add(n);
    }
    py::dict<str, int64_t> sizes = prj.data_sizes();
    {
        std::vector<str> names = missing.keys();
        std::sort(names.begin(), names.end());
        for (const str& n : names) {
            if (have.contains(n) || !kind_is(refs, n, "D")) continue;
            int64_t a = std::get<1>(refs.at(n));
            bool cov = false;
            for (const auto& c : covered)
                if (std::get<0>(c) <= a && a < std::get<1>(c)) cov = true;
            if (cov) continue;
            MemberP m;
            for (const MemberP& x : mods)
                if (x->pubs.contains(n) && std::get<0>(x->pubs.at(n)) && *std::get<0>(x->pubs.at(n)) == "_DATA") {
                    m = x;
                    break;
                }
            if (!m) continue;
            std::vector<int64_t> offs;
            for (const auto& kv : m->pubs.items()) {
                const auto& sp = std::get<1>(kv);
                if (std::get<0>(sp) && *std::get<0>(sp) == "_DATA") offs.push_back(std::get<1>(sp));
            }
            std::sort(offs.begin(), offs.end());
            int64_t off = std::get<1>(m->pubs.at(n));
            int64_t size;
            if (sizes.contains(n)) {
                size = sizes.at(n);
            } else {
                int64_t mn = std::get<0>(m->seglen(S("_DATA")));
                std::vector<int64_t> cand;
                for (int64_t o : offs)
                    if (o > off) cand.push_back(o);
                cand.push_back(mn);
                size = *std::min_element(cand.begin(), cand.end()) - off;
            }
            py::dict<str, int64_t> pb;
            pb.set(n, 0);
            auto d = std::make_shared<DataRaw>(m, a, a + size, pb,
                                               py::percent(S("(%s, which %s defines in the kit: here in a module of its own; %d bytes%s)"), n, m->label(), size,
                                                           sizes.contains(n) ? S("") : S(", a guess")));
            d->by_next = false;
            d->m = nullptr;   // d.m = _Named(n.lstrip("_"), m)
            d->name = n.lstrip(S("_"));
            d->raw = m->raw;
            out.append(d);
        }
    }
    // zeros of the data no module accounts for, which the link's fit found (project "data_pads": {DS: length})
    {
        std::vector<std::tuple<str, int64_t>> pads;
        for (const auto& kv : prj.data_pads().items()) pads.emplace_back(std::get<0>(kv), std::get<1>(kv));
        std::sort(pads.begin(), pads.end());
        for (const auto& pl : pads) {
            int64_t a = py::int_from_str(std::get<0>(pl), 16);
            auto d = std::make_shared<DataRaw>(nullptr, a, a + std::get<1>(pl), py::dict<str, int64_t>(),
                                               S("(zeros no module of the kit accounts for: a pad the link's fit found)"));
            d->by_next = false;
            d->name = py::percent(S("pad%04x"), a);
            out.append(d);
        }
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- objects
py::list<std::tuple<int64_t, int64_t, str>> _objects(const Project& prj, const py::bytes& img, int64_t dg) {
    py::list<std::tuple<int64_t, int64_t, str>> out;
    for (const str& x : prj.profile_objects(prj.model())) {
        std::optional<str> kit = prj.profile_kit();
        if (!kit) throw py::KeyError("kit");
        std::shared_ptr<Member> m;
        try {
            auto f = py::open_rb(toolchain::kit_path(kit, x));
            py::bytes raw = f.read();
            f.close();
            int64_t k = x.rfind(S("/"));
            m = std::make_shared<Member>(S(""), k < 0 ? x : x.slice_from(k + 1), raw, -1);
        } catch (const py::OSError&) {
            continue;
        }
        if (!m->code.empty() && std::get<1>(m->code[0]).size() - std::get<2>(m->code[0]).size() >= 3) {
            py::list<int64_t> ps = find(img, std::get<1>(m->code[0]), std::get<2>(m->code[0]), 0, dg);
            if (ps.size() == 1) out.append(std::make_tuple(ps[0], ps[0] + std::get<1>(m->code[0]).size(), m->label()));
        }
    }
    return out;
}

py::set<str> _first_object_names(const Project& prj) {
    py::set<str> out;
    for (const str& x : prj.first_objects()) {
        std::optional<str> kit = prj.profile_kit();
        if (!kit) throw py::KeyError("kit");
        auto f = py::open_rb(toolchain::kit_path(kit, x));
        py::bytes raw = f.read();
        f.close();
        omf::Obj o = omf::read(raw);
        for (const auto& p : o.pubs) out.add(std::get<0>(p));
    }
    return out;
}

Refs _refs(const Project& prj, const py::list<PlacedP>& placed) {
    py::bytes img = prj.image();
    Refs refs;
    auto setdefault = [&](const str& k, const std::tuple<str, int64_t>& v) {
        if (!refs.contains(k)) refs.set(k, v);
    };
    for (const PlacedP& p : placed) {
        if (p->code) {
            const str& s0 = std::get<0>(p->m->code[0]);
            const py::bytes& b = p->m->o.seg.at(s0);
            for (const auto& fx : p->m->o.fix) {
                const str& seg = std::get<0>(fx);
                int64_t fo = std::get<1>(fx);
                const str& kind = std::get<2>(fx);
                const omf::Target& tgt = std::get<3>(fx);
                int64_t disp = std::get<4>(fx);
                if (seg != s0 || std::get<0>(tgt) != "ext" || fo + 2 > b.size() || *p->code + fo + 2 > img.size()) continue;
                const str& tname = std::get<py::str>(std::get<1>(tgt));
                int64_t mine = (W(b, fo) + disp) & 0xFFFF;
                int64_t theirs = W(img, *p->code + fo);
                if (kind == "rel") {
                    int64_t base = prj.code_base(*p->code);
                    setdefault(tname, std::make_tuple(S("C"), base + ((*p->code + fo + 2 + theirs - base) & 0xFFFF) - mine));
                } else if (kind == "off") {
                    setdefault(tname, std::make_tuple(S("D"), (theirs - mine) & 0xFFFF));
                }
            }
        }
        if (p->data) {
            py::bytes b = p->m->o.seg.contains(S("_DATA")) ? p->m->o.seg.at(S("_DATA")) : py::bytes();
            for (const auto& fx : p->m->o.fix) {
                const str& seg = std::get<0>(fx);
                int64_t fo = std::get<1>(fx);
                const str& kind = std::get<2>(fx);
                const omf::Target& tgt = std::get<3>(fx);
                int64_t disp = std::get<4>(fx);
                int64_t at = prj.dgroup() + *p->data + fo;
                if (seg == "_DATA" && std::get<0>(tgt) == "ext" && kind == "off" && fo + 2 <= b.size() && at + 2 <= img.size())
                    setdefault(std::get<py::str>(std::get<1>(tgt)), std::make_tuple(S("D"), (W(img, at) - ((W(b, fo) + disp) & 0xFFFF)) & 0xFFFF));
            }
        }
    }
    return refs;
}

int64_t _code_end(const py::bytes& img, int64_t dg) {
    int64_t e = dg;
    while (e > dg - 16 && e > 0 && img[e - 1] == 0) e -= 1;
    return e;
}

// ---------------------------------------------------------------------------------------------- raw data
py::dict<int64_t, RawData> _raw_data(const Project& prj, const py::list<MemberP>& mods, const py::list<Raw>& raws, const py::bytes& ds) {
    py::dict<int64_t, RawData> out;
    for (const Raw& r : raws) {
        int64_t lo = std::get<0>(r);
        for (const auto& on : std::get<2>(r)) {
            const str& n = std::get<1>(on);
            for (const MemberP& m : mods) {
                if (!m->pubs.contains(n) || !m->data) continue;
                const py::bytes& kd = std::get<0>(*m->data);
                const py::set<int64_t>& kh = std::get<1>(*m->data);
                std::optional<int64_t> r0;
                for (int64_t k = 0; k < kd.size(); ++k)
                    if (kd[k] != 0 && !kh.contains(k)) {
                        r0 = k;
                        break;
                    }
                if (!r0) continue;
                int64_t r1 = *r0;
                while (r1 < kd.size() && kd[r1] != 0 && !kh.contains(r1) && r1 - *r0 < 12) r1 += 1;
                if (r1 - *r0 < 4) continue;
                std::optional<std::tuple<int64_t, int64_t>> best;
                py::bytes key = kd.slice(*r0, r1);
                int64_t p = ds.find(key);
                while (p >= 0) {
                    int64_t base = p - *r0;
                    int64_t k = 0;
                    while (base >= 0 && k < kd.size() && base + k < ds.size() && (kh.contains(k) || ds[base + k] == kd[k])) k += 1;
                    if (base >= 0 && (!best || k > std::get<1>(*best))) best = std::make_tuple(base, k);
                    p = ds.find(key, p + 1);
                }
                if (!best || std::get<1>(*best) < r1) continue;
                int64_t base = std::get<0>(*best), k = std::get<1>(*best);
                while (k > r1 && kd[k - 1] == 0) k -= 1;
                k += k & 1;
                py::dict<str, int64_t> dp;
                for (const auto& kv : m->pubs.items()) {
                    const auto& sp = std::get<1>(kv);
                    if (std::get<0>(sp) && *std::get<0>(sp) == "_DATA" && std::get<1>(sp) < k) dp.set(std::get<0>(kv), std::get<1>(sp));
                }
                out.set(lo, RawData(base, base + k, dp, py::percent(S("the data of %s as far as the program has it"), m->label())));
                break;
            }
            if (out.contains(lo)) break;
        }
    }
    // a stand-in no kit module's data fits: the data its own instructions address, where no placed module's is
    py::bytes img = prj.image();
    static const py::re::Pattern rx = py::re::compile(S("\\[0x([0-9a-f]{3,4})\\]"));
    for (const Raw& r : raws) {
        int64_t lo = std::get<0>(r), hi = std::get<1>(r);
        if (out.contains(lo)) continue;
        std::set<int64_t> hits;
        for (const x86::Insn& i : x86::disasm(img, lo, hi))
            for (const str& v : rx.findall(i.op_str)) hits.insert(py::int_from_str(v, 16));
        modules::Symbols known = prj.symbols();
        std::vector<int64_t> hv;
        for (int64_t h : hits) {
            bool is_known = false;
            for (const auto& kv : known.items()) {
                const auto& ta = std::get<1>(kv);
                if (!std::get<0>(kv).contains(S(":")) && std::get<0>(ta) == "D" && std::get<1>(ta) == h) is_known = true;
            }
            if (!is_known) hv.push_back(h);
        }
        std::sort(hv.begin(), hv.end());
        std::vector<std::array<int64_t, 2>> runs;
        for (int64_t h : hv) {
            if (!runs.empty() && h - runs.back()[1] <= 2) runs.back()[1] = h + 2;
            else runs.push_back({h, h + 2});
        }
        std::vector<std::array<int64_t, 2>> small;
        for (const auto& rr : runs)
            if (rr[1] - rr[0] <= 16) small.push_back(rr);
        if (small.size() == 1) out.set(lo, RawData(small[0][0], small[0][1], py::dict<str, int64_t>(), S("its own data, which its instructions address")));
    }
    return out;
}

// ---------------------------------------------------------------------------------------------- the order
namespace {
int64_t block_data(const Block& b) {
    if (auto p = std::get_if<PlacedP>(&b)) return *(*p)->data;
    return std::get<DataRawP>(b)->data;
}
std::tuple<int64_t, int64_t> block_seglen(const Block& b) {
    if (auto p = std::get_if<PlacedP>(&b)) return (*p)->m->seglen(S("_DATA"));
    return std::get<DataRawP>(b)->seglen(S("_DATA"));
}
}  // namespace

py::list<Row> _order(const py::list<PlacedP>& coded, const py::list<Block>& donly, const py::list<Raw>& raws, const py::dict<int64_t, RawData>& rawdata) {
    std::vector<Row> rows;
    for (const PlacedP& p : coded) rows.emplace_back(p->code, S("mod"), Item(p));
    for (const Raw& r : raws) rows.emplace_back(std::get<0>(r), S("raw"), Item(r));
    std::stable_sort(rows.begin(), rows.end(), [](const Row& a, const Row& b) { return *std::get<0>(a) < *std::get<0>(b); });
    std::vector<Block> blocks = donly.vec();
    std::stable_sort(blocks.begin(), blocks.end(), [](const Block& a, const Block& b) { return block_data(a) < block_data(b); });
    if (blocks.empty()) return py::list<Row>(rows);

    auto lay = [&](int64_t s) {
        std::vector<Row> out;
        int64_t score = 0;
        int64_t pos = s;
        size_t bi = 0;
        for (const Row& r : rows) {
            int64_t ln, al;
            if (std::get<1>(r) == "mod") {
                std::tie(ln, al) = std::get<PlacedP>(std::get<2>(r))->m->seglen(S("_DATA"));
            } else {
                int64_t at = *std::get<0>(r);
                if (rawdata.contains(at)) {
                    const RawData& rd = rawdata.at(at);
                    ln = std::get<1>(rd) - std::get<0>(rd);
                    al = 2;
                } else {
                    ln = 0;
                    al = 1;
                }
            }
            while (bi < blocks.size() && block_data(blocks[bi]) <= pos + (al == 2 ? (pos & 1) : 0)) {
                out.emplace_back(std::nullopt, S("data"), std::visit([](const auto& x) { return Item(x); }, blocks[bi]));
                pos = std::max(pos, block_data(blocks[bi]) + std::get<0>(block_seglen(blocks[bi])));
                bi += 1;
            }
            if (ln != 0 && al == 2 && (pos & 1)) pos += 1;
            if (std::get<1>(r) == "mod") {
                const PlacedP& p = std::get<PlacedP>(std::get<2>(r));
                if (p->data && ln != 0) score += (*p->data == pos) ? 1 : 0;
            }
            out.push_back(r);
            pos += ln;
        }
        for (size_t k = bi; k < blocks.size(); ++k)
            out.emplace_back(std::nullopt, S("data"), std::visit([](const auto& x) { return Item(x); }, blocks[k]));
        return std::make_pair(score, out);
    };
    int64_t kmin = 0, kmax = 0;
    bool any = false;
    auto note = [&](int64_t v) {
        if (!any) {
            kmin = kmax = v;
            any = true;
        } else {
            kmin = std::min(kmin, v);
            kmax = std::max(kmax, v);
        }
    };
    for (const Row& r : rows)
        if (std::get<1>(r) == "mod" && std::get<PlacedP>(std::get<2>(r))->data) note(*std::get<PlacedP>(std::get<2>(r))->data);
    for (const Block& b : blocks) note(block_data(b));
    std::optional<std::pair<int64_t, std::vector<Row>>> best;
    for (int64_t s = std::max<int64_t>(0, kmin - 0x800); s < kmax + 1; ++s) {
        auto res = lay(s);
        if (!best || res.first > best->first) best = res;
    }
    return py::list<Row>(best->second);
}

// ---------------------------------------------------------------------------------------------- the listing
namespace {
struct ByLabel {
    bool operator()(const MemberP& a, const MemberP& b) const { return a->label() < b->label(); }
};
}  // namespace

py::list<str> lines(const Map& mp, const str& title) {
    py::list<str> out;
    out.append(py::truthy(title) ? S("# ") + title : S("# the library's part of the program, in the order the linker took its modules"));
    out.append(S("# (codebreaker exact library): where each module's code (or data) stands in the program."));
    for (const Row& row : mp.order) {
        const str& kind = std::get<1>(row);
        const Item& x = std::get<2>(row);
        if (kind == "raw") {
            const Raw& r = std::get<Raw>(x);
            int64_t lo = std::get<0>(r), hi = std::get<1>(r);
            const auto& pubs = std::get<2>(r);
            str nm = !pubs.empty() ? std::get<1>(pubs[0]).lstrip(S("_$")) : py::percent(S("lib%05x"), lo);
            std::optional<RawData> rd;
            if (mp.rawdata.contains(lo)) rd = mp.rawdata.at(lo);
            str dtxt;
            if (rd) {
                std::vector<std::tuple<str, int64_t>> items;
                for (const auto& kv : std::get<2>(*rd).items()) items.emplace_back(std::get<0>(kv), std::get<1>(kv));
                std::stable_sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<1>(a) < std::get<1>(b); });
                str dp;
                for (const auto& kv : items) dp += py::percent(S(" dpub=%s@%x"), std::get<0>(kv), std::get<1>(kv));
                dtxt = py::percent(S(" data=%04x-%04x"), std::get<0>(*rd), std::get<1>(*rd)) + dp;
            }
            str ps;
            for (const auto& on : pubs) ps += py::percent(S(" pub=%s@%x"), std::get<1>(on), std::get<0>(on));
            out.append(py::percent(S("RAW %s code=%05x-%05x%s%s  # (no library module has these bytes%s)"), nm, lo, hi, ps, dtxt,
                                   rd ? S("; ") + std::get<3>(*rd) : S("")));
            continue;
        }
        if (auto d = std::get_if<DataRawP>(&x)) {
            std::vector<std::tuple<int64_t, str>> items;
            for (const auto& kv : (*d)->pubs.items()) items.emplace_back(std::get<1>(kv), std::get<0>(kv));
            std::sort(items.begin(), items.end());
            str dp;
            for (const auto& kv : items) dp += py::percent(S(" dpub=%s@%x"), std::get<1>(kv), std::get<0>(kv));
            out.append(py::percent(S("RAW %s data=%04x-%04x%s  # %s"), (*d)->name, (*d)->data, (*d)->end, dp, (*d)->why));
            continue;
        }
        const PlacedP& p = std::get<PlacedP>(x);
        py::list<str> note;
        if (p->code) note.append(py::percent(S("code %05x (%d bytes)"), *p->code, *p->end() - *p->code));
        if (p->data) note.append(py::percent(S("data %04x"), *p->data));
        if (py::truthy(p->why)) note.append(p->why);
        std::vector<MemberP> group{p->m};
        for (const MemberP& m : p->also) group.push_back(m);
        std::stable_sort(group.begin(), group.end(), ByLabel());   // (the same bytes: the build takes the one it needs)
        for (const MemberP& m : group) {
            if (m == p->m) {
                out.append(py::percent(S("%-16s # %s"), m->label(), py::join(S("  "), note)));
            } else {
                out.append(py::percent(S("%-16s # %s  (the same bytes as %s)"), m->label(),
                                       p->code ? py::percent(S("code %05x"), *p->code) : py::percent(S("data %04x"), *p->data), p->m->label()));
            }
        }
    }
    return out;
}

int64_t main(const Project& prj, const py::list<str>& argv, const std::function<void(const str&)>& out) {
    auto print = [&](const str& s) {
        if (out) out(s + S("\n"));
        else std::cout << (s + S("\n")).utf8();
    };
    MapP mp = locate(prj, argv.contains(S("-v")));
    str txt = py::join(S("\n"), lines(*mp)) + S("\n");
    print(txt);
    for (const PlacedP& p : mp->lone)
        print(py::percent(S("   among the program's own code: %s at %05x (%d bytes)"), p->m->label(), *p->code, *p->end() - *p->code));
    for (const auto& o : mp->objects)
        print(py::percent(S("   the kit's object %s at %05x (%d bytes)"), std::get<2>(o), std::get<0>(o), std::get<1>(o) - std::get<0>(o)));
    int64_t stand = 0;
    for (const Raw& r : mp->raws) stand += std::get<1>(r) - std::get<0>(r);
    print(py::percent(S("library part from %05x: %d modules with code, %d of data only, %d stand-ins (%d bytes)"), mp->start ? *mp->start : 0,
                      mp->coded.size(), mp->donly.size(), mp->raws.size(), stand));
    {
        std::vector<str> names = mp->missing.keys();
        std::sort(names.begin(), names.end());
        for (const str& n : names) {
            const auto& wb = mp->missing.at(n);
            if (!std::get<1>(wb).empty())
                print(py::percent(S("   needed, not in the program: %s (named by %s; defined by %s) - another version there, or an object"), n,
                                  std::get<0>(wb), py::join(S(", "), std::get<1>(wb))));
        }
        py::list<str> own;
        for (const str& n : names)
            if (std::get<1>(mp->missing.at(n)).empty()) own.append(n);
        if (!own.empty()) print(S("   the program's or the linker's (no library module defines them): ") + py::join(S(", "), own));
    }
    if (argv.contains(S("--write"))) {
        auto f = py::open(prj.path(S("CRTLIB")), S("w"), std::nullopt, std::nullopt, std::optional<str>(S("\n")));
        f.write(txt);
        f.close();
    }
    return 0;
}

}  // namespace codebreaker::exact::library
