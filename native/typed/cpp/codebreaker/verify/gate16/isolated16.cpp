// codebreaker/verify/gate16/isolated16.cpp -- C++17 port of codebreaker.verify.gate16.isolated16 (see isolated16.hpp).
//
// isolated16.py -- each function of a rebuilt 16-bit module run beside the original's, on its own.  Both are entered the way
// the function is called (far or near), on the same arguments, with the original's data segment on both sides.  What one
// function calls -- another of the module's functions, a Windows function, an interrupt -- is not run: it is recorded (who,
// with which argument words) and answered at once with the same made-up result on both sides (emulate/windows16, `callees`).
//
// Notes on the port:
//   * check() is a generator in the Python; it returns the list of what it would have yielded.  The worker processes it
//     spawns (`python -c ... _part`) are threads here (jobs::pool::threads).
//   * What the Python pickles (the original's fingerprints, the guided inputs, the tables) is kept in a small binary form of
//     this file's own, under cache kinds ending in "_cpp"; the keys carry the revision constants below where the Python's
//     carry the text of the sources.
//   * A recorded call's Context has no "data" field in the typed emulator: a replayed call's data segment image is not given.

#include "codebreaker/verify/gate16/isolated16.hpp"

#include <algorithm>
#include <filesystem>
#include <map>
#include <set>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/base/parallel.hpp"
#include "codebreaker/decompile/decompiler.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/jobs/pool.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "codebreaker/symbols/api_arguments.hpp"
#include "codebreaker/verify/lane_common.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::gate16::isolated16 {

namespace V = verdicts;
namespace w16 = emulate::windows16;
namespace lane = lanes::win16;
using lane::Vector;

namespace {

// bump when what the original's side of a run is made of changes (the Python keys on the sources of the checker, the
// emulator, the NE reader and the lane)
const char* const REVISION = "isolated16-cpp-1";

bool code_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

// ---- the original's fingerprint: what a run did, in a form two builds can be compared on -------------------------------
using Result = std::variant<std::monostate, int64_t, py::str, std::tuple<py::str, int64_t>>;
using Pairs = py::list<std::tuple<int64_t, int64_t>>;
using Kept = std::tuple<bool, bool, bool>;

struct Fp {
    py::str exit;
    py::list<py::str> calls;
    Pairs data;                               // sorted (offset, byte) of what it left in the data segment
    Pairs far;                                // ... and through far pointers
    Result result;                            // (absent: None)
    std::optional<int64_t> pop;
    std::optional<Kept> kept;
    concolic::Trail trail;                    // the path of the function's own jumps (the original's side only)
};

py::str result_text(const Result& r) {
    if (std::holds_alternative<std::monostate>(r)) return py::str("None");
    if (std::holds_alternative<int64_t>(r)) return py::to_str(std::get<int64_t>(r));
    if (std::holds_alternative<py::str>(r)) return std::get<py::str>(r);
    const auto& t = std::get<std::tuple<py::str, int64_t>>(r);
    return py::percent("(%s, %d)", py::repr(std::get<0>(t)), std::get<1>(t));
}

// ---- a small binary form for the caches ---------------------------------------------------------------------------------
struct Out {
    std::string s;
    void u(uint64_t v) {
        while (v >= 0x80) {
            s.push_back(static_cast<char>((v & 0x7F) | 0x80));
            v >>= 7;
        }
        s.push_back(static_cast<char>(v));
    }
    void i(int64_t v) { u((static_cast<uint64_t>(v) << 1) ^ static_cast<uint64_t>(v >> 63)); }
    void b(bool v) { s.push_back(v ? 1 : 0); }
    void str(const py::str& v) {
        std::string t = v.utf8();
        u(t.size());
        s += t;
    }
    void bytes(const py::bytes& v) {
        u(static_cast<uint64_t>(v.size()));
        s += v.raw();
    }
};

struct Bad {};

struct In {
    const std::string& s;
    size_t p = 0;
    explicit In(const std::string& text) : s(text) {}
    uint64_t u() {
        uint64_t v = 0;
        int shift = 0;
        while (true) {
            if (p >= s.size() || shift > 63) throw Bad();
            uint8_t c = static_cast<uint8_t>(s[p++]);
            v |= static_cast<uint64_t>(c & 0x7F) << shift;
            if (!(c & 0x80)) break;
            shift += 7;
        }
        return v;
    }
    int64_t i() {
        uint64_t v = u();
        return static_cast<int64_t>((v >> 1) ^ (~(v & 1) + 1));
    }
    bool b() {
        if (p >= s.size()) throw Bad();
        return s[p++] != 0;
    }
    std::string raw() {
        uint64_t n = u();
        if (p + n > s.size()) throw Bad();
        std::string t = s.substr(p, static_cast<size_t>(n));
        p += static_cast<size_t>(n);
        return t;
    }
    py::str str() { return py::str(raw()); }
    py::bytes bytes() { return py::bytes(raw()); }
};

void put_pairs(Out& o, const Pairs& p) {
    o.u(static_cast<uint64_t>(p.size()));
    for (const auto& e : p) {
        o.i(std::get<0>(e));
        o.i(std::get<1>(e));
    }
}
Pairs get_pairs(In& in) {
    Pairs out;
    for (uint64_t n = in.u(); n; --n) {
        int64_t a = in.i(), b = in.i();
        out.append(std::make_tuple(a, b));
    }
    return out;
}

std::string pack(const Fp& f) {
    Out o;
    o.str(f.exit);
    o.u(static_cast<uint64_t>(f.calls.size()));
    for (const py::str& c : f.calls) o.str(c);
    put_pairs(o, f.data);
    put_pairs(o, f.far);
    o.u(f.result.index());
    if (std::holds_alternative<int64_t>(f.result)) o.i(std::get<int64_t>(f.result));
    else if (std::holds_alternative<py::str>(f.result)) o.str(std::get<py::str>(f.result));
    else if (std::holds_alternative<std::tuple<py::str, int64_t>>(f.result)) {
        o.str(std::get<0>(std::get<std::tuple<py::str, int64_t>>(f.result)));
        o.i(std::get<1>(std::get<std::tuple<py::str, int64_t>>(f.result)));
    }
    o.b(f.pop.has_value());
    if (f.pop) o.i(*f.pop);
    o.b(f.kept.has_value());
    if (f.kept) {
        o.b(std::get<0>(*f.kept));
        o.b(std::get<1>(*f.kept));
        o.b(std::get<2>(*f.kept));
    }
    o.u(static_cast<uint64_t>(f.trail.size()));
    for (const auto& t : f.trail) {
        o.i(std::get<0>(t));
        o.b(std::get<1>(t));
    }
    return o.s;
}

Fp unpack(const std::string& s) {
    In in(s);
    Fp f;
    f.exit = in.str();
    for (uint64_t n = in.u(); n; --n) f.calls.append(in.str());
    f.data = get_pairs(in);
    f.far = get_pairs(in);
    switch (in.u()) {
        case 0: break;
        case 1: f.result = in.i(); break;
        case 2: f.result = in.str(); break;
        case 3: {
            py::str a = in.str();
            int64_t b = in.i();
            f.result = std::make_tuple(a, b);
            break;
        }
        default: throw Bad();
    }
    if (in.b()) f.pop = in.i();
    if (in.b()) {
        bool a = in.b(), b = in.b(), c = in.b();
        f.kept = std::make_tuple(a, b, c);
    }
    for (uint64_t n = in.u(); n; --n) {
        int64_t site = in.i();
        bool taken = in.b();
        f.trail.append(std::make_tuple(site, taken));
    }
    return f;
}

void put_vector(Out& o, const Vector& v) {
    o.u(static_cast<uint64_t>(v.args.size()));
    for (int64_t a : v.args) o.i(a);
    o.b(v.mem.has_value());
    if (v.mem) {
        o.u(static_cast<uint64_t>(v.mem->size()));
        for (const auto& e : *v.mem) {
            const w16::Where& w = std::get<0>(e);
            o.b(std::holds_alternative<int64_t>(w));
            if (std::holds_alternative<int64_t>(w)) {
                o.i(std::get<int64_t>(w));
            } else {
                o.str(std::get<0>(std::get<1>(w)));
                o.i(std::get<1>(std::get<1>(w)));
            }
            o.bytes(std::get<1>(e));
        }
    }
    o.b(v.stub_ret.has_value());
    if (v.stub_ret) o.i(*v.stub_ret);
    o.b(v.guided);
    o.b(v.noisy);
}

Vector get_vector(In& in) {
    Vector v;
    for (uint64_t n = in.u(); n; --n) v.args.append(in.i());
    if (in.b()) {
        w16::MemInit mem;
        for (uint64_t n = in.u(); n; --n) {
            w16::Where w;
            if (in.b()) {
                w = in.i();
            } else {
                py::str k = in.str();
                int64_t at = in.i();
                w = std::make_tuple(k, at);
            }
            mem.append(std::make_tuple(w, in.bytes()));
        }
        v.mem = mem;
    }
    if (in.b()) v.stub_ret = in.i();
    v.guided = in.b();
    v.noisy = in.b();
    return v;
}

// Python's `repr((list(args), mem, stub_ret))` stand-in: one text per input, the same for the same input
py::str same_key(const Vector& v) {
    Out o;
    put_vector(o, v);
    return py::hashlib::sha1(py::bytes(o.s)).hexdigest();
}

// ---- the build's functions by place -------------------------------------------------------------------------------------
py::dict<py::str, core16::SegOff> _built_places(const py::str& folder) {
    // {name: (segment, offset)} of the functions in a build, from its map.
    py::dict<py::str, core16::SegOff> out;
    static const py::re::Pattern MAP = py::re::compile(
        py::str("^([0-9a-f]{4}):([0-9a-f]{4,8})[+*]?\\s+_?([A-Za-z_]\\w*?)(?:@\\d+)?\\s*$"));
    py::file f = py::open(py::os::path::join(folder, py::str("m.map")), py::str("r"), std::optional<py::str>(py::str("utf-8")),
                          std::optional<py::str>(py::str("replace")));
    py::str text = f.read_text();
    for (const py::str& line : py::split(text, py::str("\n"))) {
        std::optional<py::re::Match> m = MAP.match(line.strip());
        if (m && !out.contains(m->group(3)))
            out.set(m->group(3), std::make_tuple(py::int_parse(m->group(1), 16), py::int_parse(m->group(2), 16)));
    }
    return out;
}

std::tuple<int64_t, int64_t> gdt_span() { return {w16::GDT_ADDR, w16::GDT_ADDR + w16::_N_GDT * 8}; }

py::str word(int64_t w, const py::dict<py::str, int64_t>& entry, const std::optional<int64_t>& nxt,
             const py::dict<core16::SegOff, py::str>& places) {
    // An argument word as what it is, where two builds legitimately differ in its bits.
    if (!places.empty() && nxt) {
        // (the offset half of a far pointer to one of the module's own functions -- a timer's or an enumeration's callback --
        // is that function: it sits elsewhere in a rebuild.)
        int64_t seg = ((*nxt) >> 3) - w16::_MODULE_SLOT + 1;
        auto got = places.get(std::make_tuple(seg, w));
        if (got) return py::str("&") + *got;
    }
    std::optional<int64_t> ds = entry.get(py::str("DS")), ss = entry.get(py::str("SS"));
    if (ds && w == *ds) return py::str("DS");
    if (ss && w == *ss) return py::str("SS");
    if (nxt == ss || (0xE000 <= w && w < 0xFFF0)) return py::str("stack");   // a place in the stack (frames differ)
    return py::percent("%x", w);
}

py::str input_text(const Vector& v) {
    // An input as a verdict names it: its first argument words, and for a branch-guided one what else it sets.
    py::list<py::str> first;
    for (int64_t i = 0; i < std::min<int64_t>(4, v.args.size()); ++i) first.append(py::percent("%x", v.args[i]));
    py::str out = py::str("args ") + py::join(py::str(" "), first);
    if (v.guided) {
        py::list<py::str> extra;
        if (v.mem) {
            for (int64_t i = 0; i < std::min<int64_t>(2, v.mem->size()); ++i) {
                const auto& e = (*v.mem)[i];
                const w16::Where& w = std::get<0>(e);
                if (std::holds_alternative<int64_t>(w)) extra.append(py::percent("%x=%s", std::get<int64_t>(w), std::get<1>(e).hex()));
                else extra.append(py::percent("DS:%04x=%s", std::get<1>(std::get<1>(w)), std::get<1>(e).hex()));
            }
        }
        if (v.stub_ret) extra.append(py::percent("windows returns %x", *v.stub_ret));
        py::str tail;
        for (const py::str& e : extra) tail = tail + py::str(", ") + e;
        out = py::str("guided ") + out + tail;
    }
    return out;
}

Fp fingerprint_of(const w16::Trace16& r, bool wide, int64_t data_len, const py::dict<core16::SegOff, py::str>& places) {
    w16::Regs entry = r.entry_regs ? *r.entry_regs : w16::Regs();
    py::str ex = r.exit ? *r.exit : py::str("None");
    if (ex.contains(py::str("exception")) || ex.contains(py::str("fault"))) ex = py::str("fault");
    py::list<py::str> calls;
    for (const w16::CalleeCall& c : r.calls_out) {
        py::list<py::str> parts;
        for (int64_t i = 0; i < c.args.size(); ++i)
            parts.append(word(c.args[i], entry, i + 1 < c.args.size() ? std::optional<int64_t>(c.args[i + 1]) : std::nullopt, places));
        calls.append(py::percent("%s(%s)", c.name, py::join(py::str(","), parts)));
    }
    for (const w16::Hit& h : r.imports_hit) calls.append(py::str("api ") + lane::_call(h, py::list<int64_t>(), entry));
    std::optional<int64_t> ds = r.layout.ds_base;
    int64_t stack = r.layout.stack_base;
    std::map<int64_t, int64_t> state, before;
    // (and what it leaves anywhere else but its stack: a caller's buffer reached through a far pointer, a block it was
    // given -- the same place on both sides, the module's own segments sitting at the same bases.)
    std::map<int64_t, int64_t> far, far_before;
    auto [gdt_lo, gdt_hi] = gdt_span();
    for (const auto& w : r.mem_writes) {
        int64_t a = static_cast<int64_t>(w.addr), n = static_cast<int64_t>(w.size), val = static_cast<int64_t>(w.value);
        std::map<int64_t, int64_t>* into;
        std::map<int64_t, int64_t>* at;
        int64_t base;
        if (ds && *ds <= a && a < *ds + data_len) {
            into = &state;
            at = &before;
            base = *ds;
        } else if (stack <= a && a < stack + 0x10000) {
            continue;   // (its frame, its arguments)
        } else if (gdt_lo <= a && a < gdt_hi) {
            // (the processor marking a descriptor accessed as a selector is loaded is not the function's doing)
            continue;
        } else {
            into = &far;
            at = &far_before;
            base = 0;
        }
        for (int64_t i = 0; i < std::min<int64_t>(n, 4); ++i) {
            int64_t k = a - base + i;
            at->emplace(k, (static_cast<int64_t>(w.old) >> (8 * i)) & 0xFF);
            (*into)[k] = (val >> (8 * i)) & 0xFF;
        }
    }
    Fp out;
    std::vector<std::pair<int64_t, int64_t>> kept_state;
    for (const auto& kv : state) {
        auto b = before.find(kv.first);
        if (b == before.end() || kv.second != b->second) kept_state.push_back(kv);
    }
    // (through a far pointer: what is written, not whether it changed what was there)
    for (const auto& kv : kept_state) out.data.append(std::make_tuple(kv.first, kv.second));
    for (const auto& kv : far) out.far.append(std::make_tuple(kv.first, kv.second));
    w16::Regs regs = r.exit_regs ? *r.exit_regs : w16::Regs();
    out.exit = ex;
    out.calls = calls;
    if (ex == py::str("ret")) {
        std::optional<int64_t> res = wide ? r.ret32 : r.ret;
        if (res) out.result = *res;
        if (r.unsure) return out;   // a callee removing it is not known how many bytes: the stack after it says nothing
        std::optional<int64_t> eds = entry.get(py::str("DS"));
        if (res && eds && (*res & 0xFFFF) == *eds && !wide) out.result = py::str("DS");
        if (std::holds_alternative<int64_t>(out.result) && (std::get<int64_t>(out.result) & 0xFFFF) == w16::ENTRY_AX && !wide) {
            out.result = py::str("none");   // (AX as it came in: no result set)
        } else if (std::holds_alternative<int64_t>(out.result) && (std::get<int64_t>(out.result) & 0xFF00) == (w16::ENTRY_AX & 0xFF00) && !wide) {
            out.result = std::make_tuple(py::str("byte"), std::get<int64_t>(out.result) & 0xFF);   // (AL set, AH the caller's: a byte result)
        }
        out.pop = r.sp_delta;
        out.kept = std::make_tuple(regs.get(py::str("SI")) == entry.get(py::str("SI")), regs.get(py::str("DI")) == entry.get(py::str("DI")),
                                   regs.get(py::str("DS")) == entry.get(py::str("DS")));
    }
    return out;
}

// the entries of `x` from `i`, up to two, as Python prints a list of them
py::str two_from(const py::list<py::str>& x, int64_t i) {
    py::list<py::str> part;
    for (int64_t k = i; k < std::min<int64_t>(x.size(), i + 2); ++k) part.append(x[k]);
    return py::repr(part);
}

py::str kept_text(const std::optional<Kept>& k) {
    if (!k) return py::str("None");
    auto b = [](bool v) { return py::str(v ? "True" : "False"); };
    return py::str("(") + b(std::get<0>(*k)) + py::str(", ") + b(std::get<1>(*k)) + py::str(", ") + b(std::get<2>(*k)) + py::str(")");
}

std::optional<py::str> _differ(const Fp& a, const Fp& b) {
    // The first way two fingerprints differ, in words, or None.
    if (a.exit == py::str("truncated") || b.exit == py::str("truncated")) {
        // (a run stopped at the instruction cap says nothing past where it stopped: What both did, the calls up to the
        // shorter run's last, is what compares)
        int64_t n = std::min(a.calls.size(), b.calls.size());
        for (int64_t i = 0; i < n; ++i) {
            if (a.calls[i] != b.calls[i]) {
                return py::percent("calls: original %s, ours %s", two_from(a.calls, i), two_from(b.calls, i));
            }
        }
        return std::nullopt;
    }
    auto pairs_diff = [](const char* what, const Pairs& x, const Pairs& y) -> std::optional<py::str> {
        std::map<int64_t, int64_t> dx, dy;
        for (const auto& e : x) dx[std::get<0>(e)] = std::get<1>(e);
        for (const auto& e : y) dy[std::get<0>(e)] = std::get<1>(e);
        if (x == y) return std::nullopt;
        std::set<int64_t> keys;
        for (const auto& kv : dx) keys.insert(kv.first);
        for (const auto& kv : dy) keys.insert(kv.first);
        std::vector<int64_t> at;
        for (int64_t o : keys) {
            auto p = dx.find(o), q = dy.find(o);
            bool same = (p == dx.end()) == (q == dy.end()) && (p == dx.end() || p->second == q->second);
            if (!same) at.push_back(o);
            if (at.size() == 4) break;
        }
        py::list<py::str> xs, ys;
        for (int64_t o : at) {
            auto p = dx.find(o), q = dy.find(o);
            xs.append(py::percent("%04x=%s", o, p == dx.end() ? py::str("None") : py::to_str(p->second)));
            ys.append(py::percent("%04x=%s", o, q == dy.end() ? py::str("None") : py::to_str(q->second)));
        }
        return py::percent("%s: original %s, ours %s", py::str(what), py::repr(xs), py::repr(ys));
    };
    auto opt_text = [](const std::optional<int64_t>& v) { return v ? py::to_str(*v) : py::str("None"); };
    if (a.exit != b.exit) return py::percent("ends: original %s, ours %s", a.exit, b.exit);
    if (!(a.calls == b.calls)) {
        int64_t n = std::min(a.calls.size(), b.calls.size()), i = n;
        for (int64_t k = 0; k < n; ++k)
            if (a.calls[k] != b.calls[k]) {
                i = k;
                break;
            }
        return py::percent("calls: original %s, ours %s", two_from(a.calls, i), two_from(b.calls, i));
    }
    if (auto d = pairs_diff("leaves in the data segment", a.data, b.data)) return d;
    if (auto d = pairs_diff("leaves through a far pointer", a.far, b.far)) return d;
    // result
    bool skip = std::holds_alternative<py::str>(a.result) && std::get<py::str>(a.result) == py::str("none");
    if (!skip && std::holds_alternative<std::tuple<py::str, int64_t>>(a.result) &&
        std::get<0>(std::get<std::tuple<py::str, int64_t>>(a.result)) == py::str("byte")) {
        int64_t want = std::get<1>(std::get<std::tuple<py::str, int64_t>>(a.result));
        // (the original returns a byte: AL is all)
        bool low_known = false;
        int64_t low = 0;
        if (std::holds_alternative<std::tuple<py::str, int64_t>>(b.result)) {
            low = std::get<1>(std::get<std::tuple<py::str, int64_t>>(b.result));
            low_known = true;
        } else if (std::holds_alternative<int64_t>(b.result)) {
            low = std::get<int64_t>(b.result) & 0xFF;
            low_known = true;
        }
        if (low_known && low == want) skip = true;   // (the original sets none: nothing to match)
    }
    if (!skip && !(a.result == b.result))
        return py::percent("returns: original %s, ours %s", result_text(a.result), result_text(b.result));
    if (a.pop != b.pop) return py::percent("removes argument bytes: original %s, ours %s", opt_text(a.pop), opt_text(b.pop));
    if (a.kept != b.kept) return py::percent("keeps SI, DI, DS: original %s, ours %s", kept_text(a.kept), kept_text(b.kept));
    return std::nullopt;
}

// ---- the decompiled module's tables, kept ---------------------------------------------------------------------------------
std::string pack_tables(const Tables& t) {
    Out o;
    o.u(static_cast<uint64_t>(t.funcs.size()));
    for (const auto& kv : t.funcs.items()) {
        o.str(std::get<0>(kv));
        o.i(std::get<0>(std::get<1>(kv)));
        o.b(std::get<1>(std::get<1>(kv)));
    }
    o.u(static_cast<uint64_t>(t.wide.size()));
    for (const py::str& w : t.wide) o.str(w);
    o.u(static_cast<uint64_t>(t.place.size()));
    for (const auto& kv : t.place.items()) {
        o.str(std::get<0>(kv));
        o.i(std::get<0>(std::get<1>(kv)));
        o.i(std::get<1>(std::get<1>(kv)));
    }
    return o.s;
}

Tables unpack_tables(const std::string& s) {
    In in(s);
    Tables t;
    for (uint64_t n = in.u(); n; --n) {
        py::str name = in.str();
        int64_t pop = in.i();
        bool far = in.b();
        t.funcs.set(name, std::make_tuple(pop, far));
    }
    for (uint64_t n = in.u(); n; --n) t.wide.append(in.str());
    for (uint64_t n = in.u(); n; --n) {
        py::str name = in.str();
        int64_t a = in.i(), b = in.i();
        t.place.set(name, std::make_tuple(a, b));
    }
    return t;
}

}  // namespace

py::dict<py::str, std::tuple<int64_t, bool>> _functions(const decompile::c_text::Module16& m) {
    // {name: (bytes of arguments it removes, far)} from the description's F lines.
    py::dict<py::str, std::tuple<int64_t, bool>> out;
    py::file f = py::open(m.description->path, py::str("r"), std::optional<py::str>(py::str("utf-8")));
    for (const py::str& line : f.readlines_text()) {   // (universal newlines)
        py::list<py::str> p = py::split(line.rstrip(py::str("\n")), py::str("\t"));
        if (p.size() > 4 && p[0] == py::str("F")) {
            py::str pop = p[3];
            py::str digits = pop.lstrip(py::str("-"));
            bool numeric = !digits.empty() && digits.isdigit();
            out.set(p[2], std::make_tuple(numeric ? py::int_parse(pop) : int64_t(-1), p[4].contains(py::str("far"))));
        }
    }
    return out;
}

Tables _tables(const py::str& original) {
    // What check() needs of the decompiled module: {funcs, place, wide} (made once, and kept: decompiling the whole module
    // again was 85% of a gate run).  Kept by the original and all that makes the decompiled text: the decompiler, the
    // passes over its output, what is known of Windows' functions.
    std::vector<base::cache::Part> parts;
    {
        py::file f = py::open(original, py::str("rb"));
        parts.push_back(f.read());
    }
    parts.push_back(base::cache::tool(decompile::decompiler::_exe()));
    parts.push_back(decompile::decompiler::_language_stamp());
    parts.push_back(symbols::api_arguments::sizes16());
    parts.push_back(py::str(REVISION));
    parts.push_back(py::repr(std::get<0>(decompile::c_text::generation())));
    py::str k = base::cache::key(parts);
    if (auto got = base::cache::get(py::str("gate16_tables_cpp"), k)) {
        try {
            return unpack_tables(got->raw());
        } catch (const Bad&) {
        }
    }
    decompile::c_text::Module16 m = decompile::c_text::compilable16(original);
    Tables out;
    out.funcs = _functions(m);
    std::vector<py::str> wide(m.wide.begin(), m.wide.end());
    std::sort(wide.begin(), wide.end(), code_less);
    out.wide = py::list<py::str>(wide);
    for (const auto& kv : m.by_va().items()) {
        auto w = m.where.get(std::get<0>(kv));
        if (w && *w) out.place.set(std::get<1>(kv), **w);
    }
    base::cache::put(py::str("gate16_tables_cpp"), k, py::bytes(pack_tables(out)));
    return out;
}

namespace {

// What the original's side of a run depends on besides the function and the input: the original, how its callees and
// Windows' functions are answered, the checker and emulator as written.
py::str original_key(const py::str& original, const std::optional<w16::Callees>& callees, const std::optional<std::tuple<int64_t, int64_t>>& data_end,
                     bool chain) {
    std::vector<base::cache::Part> parts;
    {
        py::file f = py::open(original, py::str("rb"));
        parts.push_back(f.read());
    }
    parts.push_back(symbols::api_arguments::sizes16());
    // repr(sorted((callees or {}).items()))
    std::vector<std::tuple<core16::SegOff, std::tuple<py::str, int64_t, bool>>> items;
    if (callees) for (const auto& kv : callees->items()) items.push_back(kv);
    std::sort(items.begin(), items.end(), [](const auto& a, const auto& b) { return std::get<0>(a) < std::get<0>(b); });
    py::str text = py::str("[");
    for (size_t i = 0; i < items.size(); ++i) {
        const auto& at = std::get<0>(items[i]);
        const auto& c = std::get<1>(items[i]);
        text = text + (i ? py::str(", ") : py::str("")) +
               py::percent("((%d, %d), (%s, %d, %s))", std::get<0>(at), std::get<1>(at), py::repr(std::get<0>(c)), std::get<1>(c),
                           py::str(std::get<2>(c) ? "True" : "False"));
    }
    text = text + py::str("]");
    parts.push_back(text);
    parts.push_back(data_end ? py::percent("(%d, %d)", std::get<0>(*data_end), std::get<1>(*data_end)) : py::str("None"));
    parts.push_back(chain);
    parts.push_back(py::str(REVISION));
    parts.push_back(int64_t(lane::REV));
    return base::cache::key(parts);
}

}  // namespace

py::list<Vector> guided_inputs(const py::str& original, const py::str& original_key_, const py::str& name, int64_t oseg, int64_t ooff, bool far_,
                               const std::tuple<int64_t, int64_t>& span, const py::list<Vector>& seeds,
                               const std::optional<w16::Callees>& callees, const std::optional<std::tuple<int64_t, int64_t>>& data_end,
                               int64_t insn_cap, const std::optional<py::dict<int64_t, concolic::Trail>>& known, ExploredSummary* explored) {
    // Branch-guided inputs: arguments, words of the data segment and what an unmodelled Windows call returns, chosen to take
    // paths of the original no other input took -- the original run as this checker runs it, its callees answered.  Found
    // path by path where the explorer can follow the run (lane::explore_vectors), else guessed (guided_vectors).  Kept,
    // keyed on the original's side (original_key) and the explorer's limits; none when CB_NO_GUIDED is set.
    if (!lane_common::guided_on()) return py::list<Vector>();
    bool ready = lane::concolic_ready();
    std::vector<base::cache::Part> parts;
    parts.push_back(original_key_);
    parts.push_back(name);
    parts.push_back(oseg);
    parts.push_back(ooff);
    parts.push_back(far_);
    parts.push_back(py::percent("(%d, %d)", std::get<0>(span), std::get<1>(span)));
    {
        Out o;
        for (const Vector& s : seeds) put_vector(o, s);
        parts.push_back(py::hashlib::sha1(py::bytes(o.s)).hexdigest());
    }
    parts.push_back(lane_common::GUIDED_BUDGET);
    parts.push_back(lane_common::MAX_GUIDED);
    parts.push_back(lane_common::PATH_BUDGET);
    parts.push_back(lane_common::MAX_PATHS);
    parts.push_back(ready);
    if (ready) {
        auto stamp = emulate::pcode::stamp();
        parts.push_back(std::get<0>(stamp));
        parts.push_back(std::get<1>(stamp));
    } else {
        parts.push_back(py::str(""));
    }
    parts.push_back(py::str(REVISION));
    py::str k = base::cache::key(parts);
    if (auto got = base::cache::get(py::str("gate16_guided_cpp"), k)) {
        try {
            In in(got->raw());
            py::list<Vector> out;
            for (uint64_t n = in.u(); n; --n) out.append(get_vector(in));
            ExploredSummary summary;
            if (in.b()) summary.paths = in.i();
            summary.all = in.b();
            summary.spent = in.i();
            if (explored) {
                explored->paths = summary.paths;
                explored->all = summary.all;
                explored->spent = summary.spent;
            }
            return out;
        } catch (const Bad&) {
        }
    }
    w16::ProbeOptions probe;
    probe.callees = callees;
    probe.data_end = data_end;
    probe.insn_cap = insn_cap;
    probe.ports = false;
    lane::Explored how = lane::explore_vectors(original, oseg, ooff, !far_, span, seeds, known, probe);
    py::list<Vector> out;
    for (const Vector& g : how.inputs) {
        Vector v = g;
        v.guided = true;
        out.append(v);
    }
    ExploredSummary summary;
    if (how.exploration) {
        summary.paths = how.exploration->paths;
        summary.all = how.exploration->complete;
        summary.spent = how.exploration->runs;
    }
    Out o;
    o.u(static_cast<uint64_t>(out.size()));
    for (const Vector& v : out) put_vector(o, v);
    o.b(summary.paths.has_value());
    if (summary.paths) o.i(*summary.paths);
    o.b(summary.all);
    o.i(summary.spent);
    base::cache::put(py::str("gate16_guided_cpp"), k, py::bytes(o.s));
    if (explored) {
        explored->paths = summary.paths;
        explored->all = summary.all;
        explored->spent = summary.spent;
        std::map<py::str, w16::Trace16> made;
        for (const auto& e : how.made) made[same_key(std::get<0>(e))] = std::get<1>(e);
        for (int64_t i = 0; i < out.size() && i < how.inputs.size(); ++i) {
            auto it = made.find(same_key(how.inputs[i]));
            if (it != made.end()) explored->runs.set(i, it->second);
        }
    }
    return out;
}

namespace {

// a recorded call's context for one side: the module's own code its far pointers reach (a callback in the recorded memory)
// sent to that side's function of the same name
std::shared_ptr<w16::Context> side(const std::shared_ptr<w16::Context>& context, const py::dict<py::str, core16::SegOff>& where,
                                    const py::dict<py::str, core16::SegOff>& place, const py::list<py::str>& named) {
    if (!context || context->own.empty()) return context;
    auto out = std::make_shared<w16::Context>(*context);
    py::dict<core16::SegOff, std::tuple<int64_t, int64_t, py::str>> redirect;
    for (const py::str& n : named) redirect.set(place.at(n), std::make_tuple(std::get<0>(where.at(n)), std::get<1>(where.at(n)), n));
    out->redirect = redirect;
    return out;
}

// the key of a recorded call: the name it was given, else a digest of what it holds
py::str context_key(const std::shared_ptr<w16::Context>& c) {
    if (!c) return py::str("");
    if (c->key) return *c->key;
    Out o;
    o.bytes(c->stack);
    o.i(c->sp);
    o.i(c->ss);
    o.i(c->ds ? *c->ds : -1);
    o.i(c->es ? *c->es : -1);
    for (const auto& kv : c->regs.items()) {
        o.str(std::get<0>(kv));
        o.i(std::get<1>(kv));
    }
    if (c->snapshot) o.bytes(*c->snapshot);
    return py::hashlib::sha1(py::bytes(o.s)).hexdigest();
}

// ---- worker processes (jobs/workers): a share's inputs and answers travel in files ----------------------------------------------------
std::string tables_text(const Tables& t) {
    namespace W = jobs::workers;
    std::string out;
    for (const auto& kv : t.funcs.items())
        out += "F " + W::hex(std::get<0>(kv).utf8()) + " " + std::to_string(std::get<0>(std::get<1>(kv))) + " " + (std::get<1>(std::get<1>(kv)) ? "1" : "0") + "\n";
    for (const py::str& w : t.wide) out += "W " + W::hex(w.utf8()) + "\n";
    for (const auto& kv : t.place.items())
        out += "P " + W::hex(std::get<0>(kv).utf8()) + " " + std::to_string(std::get<0>(std::get<1>(kv))) + " " + std::to_string(std::get<1>(std::get<1>(kv))) + "\n";
    return out;
}
Tables tables_from(const std::string& text) {
    namespace W = jobs::workers;
    Tables t;
    size_t at = 0;
    while (at < text.size()) {
        size_t nl = text.find('\n', at);
        if (nl == std::string::npos) nl = text.size();
        const std::string line = text.substr(at, nl - at);
        at = nl + 1;
        if (line.size() < 3) continue;
        W::Dec d(line.substr(2));
        if (line[0] == 'F') {
            const py::str n(d.s());
            const int64_t a = d.i();
            const bool far_ = d.i() != 0;
            t.funcs.set(n, std::make_tuple(a, far_));
        } else if (line[0] == 'W') {
            t.wide.append(py::str(d.s()));
        } else if (line[0] == 'P') {
            const py::str n(d.s());
            const int64_t seg = d.i();
            const int64_t off = d.i();
            t.place.set(n, core16::SegOff(seg, off));
        }
    }
    return t;
}
}  // namespace

py::list<Checked> check(const py::str& original, const py::str& folder, const py::list<py::str>& names_in,
                        const std::optional<py::str>& image_in, const std::optional<py::list<Vector>>& inputs,
                        const std::optional<int64_t>& workers_in, const std::optional<Tables>& tables_in, bool chain) {
    // Each named function (one the build has as C) beside the original's.  Yields (name, verdict, why): equivalent,
    // divergent, or inconclusive (not run) -- the judge's words.  Many at once: shared out over threads.
    py::list<py::str> names = names_in.copy();
    // (as many as the machine's budget, base/parallel workers() -- the setting low_load holds it to four.  CB_GATE_WORKERS
    // for another number)
    int64_t workers = workers_in ? *workers_in : 0;
    if (!workers) {
        std::optional<py::str> env = py::os::environ_::get(py::str("CB_GATE_WORKERS"));
        int64_t asked = env && !env->empty() ? py::int_parse(*env) : 0;
        // The Python ran each share in a process of its own. Unicorn and the tracer keep state per process (the code buffer,
        // the callback slots), so shares on threads corrupt each other: one share at a time unless CB_GATE_WORKERS asks.
        // (each share is a process of this program when it can start them, else the caller's own, one at a time)
        workers = asked ? asked : (jobs::workers::enabled() ? jobs::workers::count() : 1);
    }
    workers = std::max<int64_t>(1, workers);
    Tables tables = tables_in ? *tables_in : _tables(original);
    if (workers > 1 && names.size() > 2 * workers && !inputs && !chain) {
        // Shares of a few functions each, `workers` running at once, the next started as one ends: one function can take a
        // minute and another a second, and equal shares made at the start had the others idle while the slowest went on.
        int64_t count = std::min<int64_t>(names.size(), workers * 4);
        py::list<py::list<py::str>> shares;
        for (int64_t k = 0; k < count; ++k) {
            py::list<py::str> share;
            for (int64_t i = k; i < names.size(); i += count) share.append(names[i]);
            shares.append(share);
        }
        auto one = [&](const py::list<py::str>& share) -> py::list<Checked> {
            if (jobs::workers::enabled()) {
                // a worker process: the share and the tables in files, the answer in one
                namespace W = jobs::workers;
                const std::string dir = W::scratch_folder();
                std::string nm;
                for (const py::str& n : share) nm += W::hex(n.utf8()) + "\n";
                W::write_file(dir + "\\names.txt", nm);
                W::write_file(dir + "\\tables.txt", tables_text(tables));
                const int64_t rc = W::run({"gate", W::hex(original.utf8()), W::hex(folder.utf8()), image_in ? W::hex(image_in->utf8()) : std::string("-"),
                                           dir + "\\tables.txt", dir + "\\names.txt", dir + "\\answer.txt"});
                py::list<Checked> res;
                std::set<py::str, bool (*)(const py::str&, const py::str&)> got(code_less);
                const std::string answer = W::read_file(dir + "\\answer.txt");
                size_t at = 0;
                while (at < answer.size()) {
                    size_t nl = answer.find('\n', at);
                    if (nl == std::string::npos) nl = answer.size();
                    if (nl > at) {
                        W::Dec d(answer.substr(at, nl - at));
                        Checked c;
                        c.name = py::str(d.s());
                        c.verdict = py::str(d.s());
                        c.why = py::str(d.s());
                        got.insert(c.name);
                        res.append(c);
                    }
                    at = nl + 1;
                }
                std::error_code ec;
                std::filesystem::remove_all(dir, ec);
                for (const py::str& n : share)
                    if (!got.count(n))
                        res.append(Checked{n, py::str(V::INCONCLUSIVE), py::str("the checker failed: the worker process gave no answer (exit ") + py::to_str(rc) + py::str(")")});
                return res;
            }
            py::list<Checked> items;
            std::set<py::str, bool (*)(const py::str&, const py::str&)> got(code_less);
            py::str why;
            try {
                for (const Checked& c : check(original, folder, share, image_in, std::nullopt, 1, tables, false)) {
                    got.insert(c.name);
                    items.append(c);
                }
            } catch (const py::Exception& e) {
                why = py::percent("%s: %s", py::str(e.type()), py::str(e.what()));
            } catch (const std::exception& e) {
                why = py::str(e.what());
            }
            if (why.empty()) why = py::str("the worker gave no answer");
            py::list<Checked> res = items;
            for (const py::str& n : share) {
                if (!got.count(n)) res.append(Checked{n, py::str(V::INCONCLUSIVE), py::str("the checker failed: ") + why.slice(0, 200)});
            }
            return res;
        };
        py::list<Checked> out;
        for (const py::list<Checked>& items : jobs::pool::threads(one, shares, workers)) out.extend(items);
        return out;
    }
    py::list<Checked> out;
    py::dict<py::str, std::tuple<int64_t, bool>> funcs = tables.funcs;
    py::dict<py::str, core16::SegOff> place = tables.place;
    py::set<py::str> wide_set = py::set<py::str>::from(tables.wide);
    py::dict<py::str, core16::SegOff> ours = _built_places(folder);
    py::str image;
    if (image_in) {
        image = *image_in;
    } else {
        bool found = false;
        py::str want = py::os::path::basename(original).lower();
        for (const py::str& f : py::os::listdir(folder)) {
            if (f.lower() == want) {
                image = py::os::path::join(folder, f);
                found = true;
                break;
            }
        }
        if (!found) throw py::RuntimeError("generator raised StopIteration");
    }
    formats::ne::NEFile nef(original);
    int64_t auto_ = nef.h.autodata_seg;
    std::optional<formats::ne::Segment> seg;   // (a driver may have none)
    for (const formats::ne::Segment& s : nef.segments())
        if (s.idx == auto_) {
            seg = s;
            break;
        }
    py::bytes data = seg && seg->file_off ? w16::relocated_data(original) : py::bytes();   // (fixed up, as loaded)
    // (and the scattered inputs once more over a data segment whose zeros are a pattern, the same on both sides: much of it
    // is zeros at load, and a C function reading the global beside the one it should read the zero beside a zero)
    std::string noisy_bytes;
    for (int64_t i = 0; i < data.size(); ++i) {
        int64_t b = data[i];
        noisy_bytes.push_back(static_cast<char>(b ? b : ((((static_cast<uint64_t>(i) * 0x9E3779B1ULL) >> 13) & 0xFF) | 1)));
    }
    py::bytes noisy(noisy_bytes);
    py::list<Vector> noisy_runs;
    if (!data.empty()) {
        py::list<Vector> all = lane::vectors();
        for (int64_t i = std::max<int64_t>(0, all.size() - 4); i < all.size(); ++i) {
            Vector v = all[i];
            v.noisy = true;
            noisy_runs.append(v);
        }
    }
    // (where the original's data ends and its segment ends: past them both sides hold the same)
    std::optional<std::tuple<int64_t, int64_t>> data_end;
    if (seg) {
        int64_t top = std::max(seg->length, seg->minalloc);
        data_end = std::make_tuple(seg->length, top ? top : int64_t(0x10000));
    }
    // (functions both sides know by name: a data symbol -- the build's codebreaker_data at DS:0 -- or a name only one side's
    // map has would make "0, DS" a pointer on one side alone)
    py::list<py::str> named;
    for (const py::str& n : funcs)
        if (place.contains(n) && ours.contains(n)) named.append(n);
    py::dict<core16::SegOff, py::str> places_orig, places_ours;
    for (const py::str& n : named) {
        places_orig.set(place.at(n), n);
        places_ours.set(ours.at(n), n);
    }
    std::optional<w16::Callees> callees_orig, callees_ours;
    {
        w16::Callees co, cu;
        for (const auto& kv : funcs.items()) {
            const py::str& n = std::get<0>(kv);
            if (place.contains(n)) co.set(place.at(n), std::make_tuple(n, std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv))));
            if (ours.contains(n)) cu.set(ours.at(n), std::make_tuple(n, std::get<0>(std::get<1>(kv)), std::get<1>(std::get<1>(kv))));
        }
        callees_orig = co;
        callees_ours = cu;
    }
    int64_t insn_cap = chain ? 200000 : 20000;
    if (chain) {
        // (chained: the module's other functions run for real on both sides -- what one function leaves for another,
        // through the stack, registers or memory, is then seen)
        callees_orig.reset();
        callees_ours.reset();
    }
    // The original's side of a run is the same whatever build it is compared with: kept, keyed on all it depends on, so a
    // module gated again after a change to the decompiler runs only its own side.
    py::str okey = original_key(original, callees_orig, data_end, chain);

    auto original_run = [&](const py::str& name, int64_t oseg, int64_t ooff, bool far_, bool wide, const Vector& v,
                            const std::optional<py::bytes>& dimg, const std::tuple<int64_t, int64_t>& span,
                            const std::optional<w16::Trace16>& ran) -> Fp {
        // The original's fingerprint on `v`, kept; with the path its own jumps took ("trail", lane::trail: the path
        // explorer's seeds).  `ran`: the record of that very run, when the explorer made it already.
        std::vector<base::cache::Part> parts;
        parts.push_back(okey);
        parts.push_back(name);
        parts.push_back(oseg);
        parts.push_back(ooff);
        parts.push_back(far_);
        parts.push_back(wide);
        parts.push_back(py::percent("(%d, %d)", std::get<0>(span), std::get<1>(span)));
        parts.push_back(v.args);
        parts.push_back(v.noisy);
        parts.push_back(context_key(v.context));   // (a recorded call by the name it was given -- its memory is 64 MB)
        {
            Out o;
            put_vector(o, v);
            parts.push_back(py::hashlib::sha1(py::bytes(o.s)).hexdigest());   // (the memory and what Windows returns)
        }
        py::str k = base::cache::key(parts);
        if (auto got = base::cache::get(py::str("gate16_original_cpp"), k)) {
            try {
                return unpack(got->raw());
            } catch (const Bad&) {
            }
        }
        w16::Trace16 a;
        if (ran) {
            a = *ran;
        } else {
            w16::ProbeOptions o;
            o.seg = oseg;
            o.offset = ooff;
            o.args = v.args;
            o.near_ = !far_;
            o.callees = callees_orig;
            o.insn_cap = insn_cap;
            o.data_end = data_end;
            o.data_image = dimg;
            o.context = side(v.context, place, place, named);
            o.mem_init = v.mem;
            o.stub_ret = v.stub_ret;
            a = w16::probe(original, o);
        }
        Fp fa = fingerprint_of(a, wide, data.size(), places_orig);
        py::list<int64_t> path;
        for (uint64_t p : a.path) path.append(static_cast<int64_t>(p));
        fa.trail = lane::trail(original, oseg, span, path);
        base::cache::put(py::str("gate16_original_cpp"), k, py::bytes(pack(fa)));
        return fa;
    };

    for (const py::str& name : names) {
        if (!place.contains(name) || !ours.contains(name) || !funcs.contains(name)) {
            out.append(Checked{name, py::str(V::INCONCLUSIVE), py::str("not found on both sides")});
            continue;
        }
        bool far_ = std::get<1>(funcs.at(name));
        bool wide = wide_set.contains(name);
        int64_t oseg = std::get<0>(place.at(name)), ooff = std::get<1>(place.at(name));
        int64_t bseg = std::get<0>(ours.at(name)), boff = std::get<1>(ours.at(name));
        std::optional<py::str> why;
        // (and inputs aimed at what its own code tests: the constants it compares with)
        int64_t later = 0x10000;
        bool any_later = false;
        for (const auto& pl : place.values()) {
            if (std::get<0>(pl) == oseg && std::get<1>(pl) > ooff) {
                later = any_later ? std::min(later, std::get<1>(pl)) : std::get<1>(pl);
                any_later = true;
            }
        }
        std::tuple<int64_t, int64_t> span = std::make_tuple(ooff, any_later ? later : int64_t(0x10000));
        py::list<Vector> aimed = lane::directed(original, oseg, span, 12);
        // (and the branch-guided ones the judge runs: a branch no fixed or aimed input takes -- a global tested before
        // anything is done, a Windows call's result -- hid the path behind it.  Looked for only once the others all agree: a
        // function found to differ needs no more.)  (The seeds' paths are known from their runs as fixed inputs: of seeds
        // that went the same way only one is run again with the explorer's shadow; the inputs it finds it has run.)
        py::list<Vector> seeds = lane::vectors() + aimed;
        py::dict<int64_t, concolic::Trail> trails;
        ExploredSummary explored;
        bool explored_made = false;

        // the inputs in order: the given ones, or the fixed ones and then (computed when the fixed ones all passed) the guided
        bool use_given = inputs && !inputs->empty();
        py::list<Vector> fixed = use_given ? inputs->copy() : seeds + noisy_runs;
        size_t total_fixed = static_cast<size_t>(fixed.size());
        py::list<Vector> guided_list;
        bool guided_done = use_given;
        bool broke = false;
        for (size_t idx = 0;; ++idx) {
            if (idx == total_fixed) {
                if (guided_done) break;
                guided_list = guided_inputs(original, okey, name, oseg, ooff, far_, span, seeds, callees_orig, data_end, insn_cap, trails, &explored);
                explored_made = true;
                guided_done = true;
                fixed = fixed + guided_list;
                total_fixed = static_cast<size_t>(fixed.size());
                if (idx == total_fixed) break;
            }
            const Vector& v = fixed[static_cast<int64_t>(idx)];
            Fp fa, fb;
            try {
                // (a call recorded in the running system: its data segment as it was then, both sides)
                std::optional<py::bytes> dimg = v.noisy ? std::optional<py::bytes>(noisy) : std::nullopt;
                std::optional<w16::Trace16> ran;
                if (v.guided && explored_made) {
                    int64_t at = static_cast<int64_t>(idx) - static_cast<int64_t>(seeds.size() + noisy_runs.size());
                    auto got = explored.runs.get(at);
                    if (got) ran = *got;
                }
                fa = original_run(name, oseg, ooff, far_, wide, v, dimg, span, ran);
                if (!use_given && !v.guided && !v.noisy && idx < static_cast<size_t>(seeds.size())) trails.set(static_cast<int64_t>(idx), fa.trail);
                w16::ProbeOptions o;
                o.seg = bseg;
                o.offset = boff;
                o.args = v.args;
                o.near_ = !far_;
                o.callees = callees_ours;
                o.insn_cap = insn_cap;
                o.data_image = (dimg && !dimg->empty()) ? dimg : std::optional<py::bytes>(data);
                o.data_end = data_end;
                o.context = side(v.context, ours, place, named);
                o.mem_init = v.mem;
                o.stub_ret = v.stub_ret;
                w16::Trace16 b = w16::probe(image, o);
                fb = fingerprint_of(b, wide, data.size(), places_ours);
            } catch (const py::Exception& e) {   // (the emulator itself fell over)
                if (!why) why = py::percent("the emulator: %s", py::str(e.type()));
                continue;
            } catch (const std::exception&) {
                if (!why) why = py::str("the emulator: Exception");
                continue;
            }
            if (fa.exit == py::str("fault") && fb.exit != py::str("fault")) {
                // An input the original faults on (a selector it loads at once, `lds si, p`, that no caller would pass) is
                // no behavior to match: its C, loading a selector only to use it, went on.  What the original did before it
                // faulted must still be done.
                bool same = fb.calls.size() >= fa.calls.size();
                if (same)
                    for (int64_t i = 0; i < fa.calls.size(); ++i)
                        if (fb.calls[i] != fa.calls[i]) {
                            same = false;
                            break;
                        }
                if (!same) {
                    out.append(Checked{name, py::str(V::DIVERGENT),
                                       py::percent("%s: calls: original %s, ours %s", input_text(v), two_from(fa.calls, 0), two_from(fb.calls, 0))});
                    broke = true;
                    break;
                }
                continue;
            }
            std::optional<py::str> d = _differ(fa, fb);
            if (d) {
                out.append(Checked{name, py::str(V::DIVERGENT), py::percent("%s: %s", input_text(v), *d)});
                broke = true;
                break;
            }
        }
        if (!broke) {
            // (and, when it passes, how much of the original that was: the paths of its own jumps the runs took -- all there
            // are, when the explorer found no more)
            py::str seen = explored.paths ? py::percent("paths %d%s", *explored.paths, explored.all ? py::str(" (all)") : py::str("")) : py::str("");
            out.append(Checked{name, py::str(why ? V::INCONCLUSIVE : V::EQUIVALENT), why ? *why : seen});
        }
    }
    return out;
}

// A worker process's share: args = original, folder, image or "-", tables file, names file, answer file (all text hex where it is not a path).
int worker_main(const std::vector<std::string>& args) {
    namespace W = jobs::workers;
    if (args.size() < 6) return 2;
    const py::str original(W::unhex(args[0])), folder(W::unhex(args[1]));
    const std::string image = W::unhex(args[2]);
    const Tables tables = tables_from(W::read_file(args[3]));
    py::list<py::str> names;
    {
        const std::string text = W::read_file(args[4]);
        size_t at = 0;
        while (at < text.size()) {
            size_t nl = text.find('\n', at);
            if (nl == std::string::npos) nl = text.size();
            if (nl > at) names.append(py::str(W::unhex(text.substr(at, nl - at))));
            at = nl + 1;
        }
    }
    const py::list<Checked> rows = check(original, folder, names, args[2] == "-" ? std::optional<py::str>() : std::optional<py::str>(py::str(image)), std::nullopt, 1, tables, false);
    std::string out;
    for (const Checked& c : rows) out += W::Enc().s(c.name.utf8()).s(c.verdict.utf8()).s(c.why.utf8()).text() + "\n";
    W::write_file(args[5], out);
    return 0;
}
}  // namespace codebreaker::verify::gate16::isolated16
