// codebreaker/invert/early_c.cpp -- C++17 port of codebreaker/invert/early_c.py (see early_c.hpp).
//
// Build with native/invert on the include path and early_c.cc, early_c_text.cc, flow16.cc, cs.cc and
// native/l2/src/{pyre,pystr,value}.cc linked in: the model proper lives there.

#include "codebreaker/invert/early_c.hpp"

#include <algorithm>
#include <cstdio>
#include <map>
#include <set>

#include "early_c.h"   // native/invert: cbinv::ec_recover, ec_recover_cfg, ec_to_c, ...
#include "flow16.h"    // cbinv::PyError

namespace codebreaker::invert::early_c {

CallinfoHook CALLINFO;
CallregsHook CALLREGS;
SwitchHook SWITCH;

bool MARK = false;
bool FARPTR = py::os::environ_::get(py::str("MC2INV_FAR"), py::str("0")) == py::str("1");
bool DEADREGS = py::os::environ_::get(py::str("MC2INV_DEADREGS"), py::str("0")) == py::str("1");
bool UNSIGNED_CAST = py::os::environ_::get(py::str("MC2INV_UNSIGNED"), py::str("0")) == py::str("1");
bool FPCASTS = false;
std::optional<bool> RETVAL;
Last LAST;
bool REGVARS_EAGER = false;
bool FRAME_AFTER_BP = false;
bool REGPARAM_REREAD = true;

bool flow16_CMPSWITCH = false;
std::array<int64_t, 7> REGISTER_HASHES = {0, 0, 0, 0, 0, 0, 0};

std::tuple<bool, bool, bool, bool, bool, bool, bool, bool, std::optional<py::str>, bool, std::optional<py::str>> _flags() {
    return std::make_tuple(MARK, FARPTR, DEADREGS, UNSIGNED_CAST, FPCASTS, REGVARS_EAGER, FRAME_AFTER_BP, REGPARAM_REREAD,
                           py::os::environ_::get(py::str("MASM_CRET")), flow16_CMPSWITCH, py::os::environ_::get(py::str("CFG_NOCAP")));
}

namespace {

// The exception the model raised (native_invert's _RAISE table), as the Python exception type.
[[noreturn]] void raise_kind(int kind, const std::string& msg) {
    switch (kind) {
        case 1: throw py::IndexError(msg);
        case 2: throw py::ValueError(msg);
        case 4: throw py::KeyError(msg);
        case 5: throw py::AssertionError(msg);
        case 6: throw py::TypeError(msg);
        case 7: throw py::AttributeError(msg);
        default: throw py::RuntimeError(msg);   // 3 and any other kind
    }
}

// Runs a model entry point, turning the model's own error into the Python exception.
template <class F> auto guarded(F f) -> decltype(f()) {
    try {
        return f();
    } catch (const cbinv::PyError& e) {
        raise_kind(e.kind, e.msg);
    } catch (const py::Exception&) {
        throw;
    } catch (const std::exception& e) {
        throw py::RuntimeError(e.what());
    }
}

std::vector<std::string> strs(const py::list<py::str>& xs) {
    std::vector<std::string> out;
    for (const auto& x : xs) out.push_back(x.utf8());
    return out;
}
std::vector<long long> ints(const py::list<int64_t>& xs) {
    std::vector<long long> out;
    for (int64_t x : xs) out.push_back(x);
    return out;
}
py::list<py::str> to_strs(const std::vector<std::string>& v) {
    py::list<py::str> out;
    for (const auto& s : v) out.append(py::str(s));
    return out;
}

// native_invert._target: None, an int (near), or the tuple of ints (far).
CallTarget target_of(const std::string& kind, const std::string& t) {
    if (t.empty()) return std::nullopt;
    auto num = [](const std::string& s) { return py::int_from_str(py::str(s)); };
    if (kind == "far") {
        py::list<int64_t> parts;
        size_t i = 0;
        for (;;) {
            size_t c = t.find(',', i);
            parts.append(num(t.substr(i, c == std::string::npos ? c : c - i)));
            if (c == std::string::npos) break;
            i = c + 1;
        }
        return CallTarget(std::variant<int64_t, py::list<int64_t>>(parts));
    }
    return CallTarget(std::variant<int64_t, py::list<int64_t>>(num(t)));
}

// The model's hooks (status: -1 the hook raised, 0 None or falsy, 1 an answer), asked of the settings above.
cbinv::EcHooks make_hooks() {
    cbinv::EcHooks h;
    if (SWITCH) {
        SwitchHook sw = SWITCH;
        h.sw = [sw](long long addr, std::vector<long long>& out) -> int {
            try {
                auto t = sw(addr);
                if (!t) return 0;
                std::string text;
                bool first = true;
                for (int64_t x : *t) {
                    text += (first ? "" : ",") + std::to_string(x);
                    first = false;
                }
                if (text.size() + 1 > (1 << 16)) return -1;
                out.clear();
                for (int64_t x : *t) out.push_back(x);
                return 1;
            } catch (...) {
                return -1;
            }
        };
    }
    if (CALLINFO) {
        CallinfoHook ci = CALLINFO;
        h.callinfo = [ci](long long addr, const std::string& kind, const std::string& tg, long long& n) -> int {
            try {
                auto rn = ci(addr, py::str(kind), target_of(kind, tg));
                if (!rn || *rn == 0) return 0;
                if (std::to_string(*rn).size() + 1 > 256) return -1;
                n = *rn;
                return 1;
            } catch (...) {
                return -1;
            }
        };
    }
    if (CALLREGS) {
        CallregsHook cr = CALLREGS;
        h.callregs = [cr](long long addr, const std::string& kind, const std::string& tg, std::vector<std::string>& rr) -> int {
            try {
                auto r = cr(addr, py::str(kind), target_of(kind, tg));
                if (!r || r->empty()) return 0;
                std::string text;
                std::vector<std::string> names;
                for (const auto& s : *r) {
                    text += (names.empty() ? "" : ",") + s.utf8();
                    names.push_back(s.utf8());
                }
                if (text.size() + 1 > 4096) return -1;
                rr = names;
                return 1;
            } catch (...) {
                return -1;
            }
        };
    }
    return h;
}

// early_c.recover (which 0) / recover_cfg (1) through the model; LAST is updated in place, also when the model raises.
Recovered run_recover(int which, const py::bytes& code, int64_t base, const std::optional<py::dict<int64_t, py::str>>& cxmap) {
    cbinv::EcCfg C;
    auto fl = _flags();
    C.mark = std::get<0>(fl);
    C.farptr = std::get<1>(fl);
    C.deadregs = std::get<2>(fl);
    C.unsigned_cast = std::get<3>(fl);
    C.fpcasts = std::get<4>(fl);
    C.regvars_eager = std::get<5>(fl);
    C.frame_after_bp = std::get<6>(fl);
    C.regparam_reread = std::get<7>(fl);
    C.masm_cret = py::truthy(std::get<8>(fl));
    C.cmpswitch = std::get<9>(fl);
    C.nocap = py::truthy(std::get<10>(fl));
    C.retval = RETVAL ? (*RETVAL ? 1 : 0) : -1;
    for (int q = 0; q < 7; q++) C.hash[q] = REGISTER_HASHES[q];
    C.hooks = make_hooks();

    std::vector<uint8_t> raw(code.raw().begin(), code.raw().end());
    std::map<long long, std::string> cx;
    if (cxmap)
        for (const auto& kv : cxmap->items()) cx[std::get<0>(kv)] = std::get<1>(kv).utf8();

    cbinv::EcLast L;
    if (LAST.saved) {
        L.has_saved = true;
        L.saved = strs(*LAST.saved);
    }
    if (LAST.regparam) {
        L.has_regparam = true;
        for (const auto& kv : LAST.regparam->items()) L.regparam.push_back({std::get<0>(kv).utf8(), std::get<1>(kv)});
    }
    if (LAST.fptypes) {
        L.has_fptypes = true;
        for (const auto& kv : LAST.fptypes->items()) L.fptypes.push_back({std::get<0>(kv).utf8(), std::get<1>(kv).utf8()});
    }
    if (LAST.fpret) {
        L.has_fpret = true;
        L.fpret = LAST.fpret->utf8();
    }
    if (LAST.unsigned_longs) {
        L.has_ul = true;
        for (const auto& s : *LAST.unsigned_longs) L.unsigned_longs.push_back(s.utf8());
    }

    cbinv::EcResult r;
    int err = 0;
    std::string msg;
    try {
        r = which == 0 ? cbinv::ec_recover(raw, base, cx, C, L) : cbinv::ec_recover_cfg(raw, base, cx, C, L);
    } catch (const cbinv::PyError& e) {
        err = e.kind;
        msg = e.msg;
    } catch (const std::exception& e) {
        err = 3;
        msg = e.what();
    }

    // LAST is what the model left (the Python changed it in place before it raised)
    LAST = Last();
    if (L.has_saved) LAST.saved = to_strs(L.saved);
    if (L.has_regparam) {
        py::dict<py::str, int64_t> d;
        for (const auto& kv : L.regparam) d.set(py::str(kv.first), kv.second);
        LAST.regparam = d;
    }
    if (L.has_fptypes) {
        py::dict<py::str, py::str> d;
        for (const auto& kv : L.fptypes) d.set(py::str(kv.first), py::str(kv.second));
        LAST.fptypes = d;
    }
    if (L.has_fpret) LAST.fpret = py::str(L.fpret);
    if (L.has_ul) {
        py::set<py::str> s;
        for (const auto& x : L.unsigned_longs) s.add(py::str(x));
        LAST.unsigned_longs = s;
    }
    if (err) raise_kind(err, msg);

    py::list<int64_t> loc;
    for (long long x : r.loc) loc.append(x);
    return Recovered(r.fs, to_strs(r.params), to_strs(r.stmts), py::str(r.pt), loc);
}

// ---- early_c_frame (emit_local_decls and what it calls), as local_decls uses it ----

int64_t tsize(const std::string& t) { return t == "long" ? 4 : 2; }

// A copy `dest=src` is an edge src->dest; a maximal chain swaps only its endpoints.
std::vector<std::pair<int64_t, int64_t>> chain_swaps(const py::list<std::tuple<int64_t, int64_t>>& copies) {
    std::map<int64_t, int64_t> outg;
    std::set<int64_t> incoming;
    for (const auto& c : copies) {
        outg[std::get<1>(c)] = std::get<0>(c);
        incoming.insert(std::get<0>(c));
    }
    std::vector<std::pair<int64_t, int64_t>> swaps;
    std::set<int64_t> seen;
    for (const auto& c : copies) {
        int64_t src = std::get<1>(c);
        if (incoming.count(src) || seen.count(src)) continue;
        int64_t cur = src;
        seen.insert(src);
        size_t guard = 0;   // (the Python walks a cycle forever; a bounded walk ends it)
        while (outg.count(cur) && guard++ <= outg.size()) {
            cur = outg[cur];
            seen.insert(cur);
        }
        if (src != cur) swaps.push_back({src, cur});
    }
    return swaps;
}

// INVERSE: declaration order (list of offsets) to emit so the compiler's copy-chain endpoint swaps
// reproduce each var at its own offset.
std::vector<int64_t> infer_decl_order(const std::vector<int64_t>& offs, const py::list<std::tuple<int64_t, int64_t>>& copies) {
    std::map<int64_t, int64_t> pos;
    for (int64_t o : offs) pos[o] = o;
    for (const auto& ab : chain_swaps(copies))
        if (pos.count(ab.first) && pos.count(ab.second)) std::swap(pos[ab.first], pos[ab.second]);
    std::vector<int64_t> order = offs;
    std::stable_sort(order.begin(), order.end(), [&](int64_t a, int64_t b) { return pos[a] < pos[b]; });
    return order;
}

struct Decl {
    std::string ctype, name;
    int64_t count;
};

std::vector<Decl> emit_local_decls(const py::list<int64_t>& offsets, int64_t framesize, const py::set<int64_t>& longset,
                                   const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies) {
    std::set<int64_t> uniq;
    for (int64_t o : offsets) uniq.insert(o);
    std::vector<int64_t> offs(uniq.begin(), uniq.end());
    std::vector<Decl> ordered;
    if (offs.empty()) return ordered;
    bool contiguous = true;
    for (size_t k = 0; k + 1 < offs.size(); k++)
        if (offs[k + 1] - offs[k] != tsize(longset.contains(offs[k]) ? "long" : "int")) contiguous = false;
    if (py::truthy(copies) && contiguous) {
        // copy-reversal present: emit in the inferred declaration order so the compiler's
        // src<->dest offset swap lands each var at its own offset (round-trip byte-exact).
        for (int64_t o : infer_decl_order(offs, *copies))
            ordered.push_back({longset.contains(o) ? "long" : "int", "v" + std::to_string(o), 1});
        return ordered;
    }
    for (size_t k = 0; k < offs.size(); k++) {
        int64_t o = offs[k];
        std::string t = longset.contains(o) ? "long" : "int";
        int64_t sz = tsize(t);
        ordered.push_back({t, "v" + std::to_string(o), 1});
        int64_t nxt = k + 1 < offs.size() ? offs[k + 1] : framesize;
        int64_t gap = nxt - o - sz;
        if (gap > 0) ordered.push_back({"char", "_p" + std::to_string(o), gap});   // unused local(s) filler
    }
    return ordered;
}

std::string fmt_decl(const Decl& d) {
    return d.count > 1 ? d.ctype + " " + d.name + "[" + std::to_string(d.count) + "];" : d.ctype + " " + d.name + ";";
}

}  // namespace

Recovered recover(const py::bytes& code, int64_t base, const std::optional<py::dict<int64_t, py::str>>& cxmap) {
    return run_recover(0, code, base, cxmap);
}

Recovered recover_cfg(const py::bytes& code, int64_t base, const std::optional<py::dict<int64_t, py::str>>& cxmap) {
    // (the Python also hands SWITCH to flow16 here: the model's flow16 takes the hook from its own configuration)
    return run_recover(1, code, base, cxmap);
}

py::list<std::tuple<int64_t, int64_t>> detect_copies(const py::list<py::str>& stmts) {
    auto c = guarded([&] { return cbinv::ec_detect_copies(strs(stmts)); });
    py::list<std::tuple<int64_t, int64_t>> out;
    for (const auto& p : c) out.append(std::make_tuple(static_cast<int64_t>(p.first), static_cast<int64_t>(p.second)));
    return out;
}

py::list<py::str> _fold_ladders(const py::list<py::str>& lines) {
    return to_strs(guarded([&] { return cbinv::ec_fold_ladders(strs(lines), MARK); }));
}

py::str local_decls(int64_t fs, const std::optional<py::list<int64_t>>& locals, const std::optional<py::set<int64_t>>& longset,
                    const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies) {
    py::list<int64_t> offs = py::truthy(locals) ? *locals : py::list<int64_t>();
    py::set<int64_t> ls = py::truthy(longset) ? *longset : py::set<int64_t>();
    std::string out;
    for (const auto& d : emit_local_decls(offs, fs, ls, copies)) out += "    " + fmt_decl(d) + "\n";
    return py::str(out);
}

py::str to_c(const py::str& name, int64_t fs, const py::list<py::str>& params, const py::list<py::str>& stmts, const py::str& ptype,
             const std::optional<py::list<int64_t>>& locals, const std::optional<py::set<int64_t>>& longset) {
    std::vector<long long> ls;
    if (longset) {
        for (int64_t x : *longset) ls.push_back(x);
        std::sort(ls.begin(), ls.end());   // sorted(longset or ())
    }
    return py::str(guarded([&] {
        return cbinv::ec_to_c(name.utf8(), fs, strs(params), strs(stmts), ptype.utf8(), locals ? ints(*locals) : std::vector<long long>(), ls);
    }));
}

void main(const py::list<py::str>& argv) {
    py::bytes img = py::open_rb(argv[1]).read();
    int64_t off = py::int_from_str(argv[2], 16);
    int64_t ln = py::int_from_str(argv[3]);
    auto [fs, params, stmts, pt, loc] = recover(img.slice(off, off + ln));
    char name[32];
    std::snprintf(name, sizeof name, "f%05llx", static_cast<unsigned long long>(off));
    py::print(to_c(py::str(name), fs, params, stmts, pt, loc));
}

}  // namespace codebreaker::invert::early_c
