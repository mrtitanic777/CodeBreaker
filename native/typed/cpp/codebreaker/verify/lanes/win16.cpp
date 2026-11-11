// codebreaker/verify/lanes/win16.cpp -- C++17 port of codebreaker.verify.lanes.win16 (see win16.hpp).
//
// win16.py -- what is particular to reconstructing a 16-bit Windows module.  The module is decompiled and made compilable
// by decompile/c_text, every function is compiled and a 16-bit DLL linked by build/functions, and each function is run
// beside the original in the 16-bit emulator (emulate/windows16).

#include "codebreaker/verify/lanes/win16.hpp"

#include <algorithm>
#include <map>
#include <mutex>
#include <set>

#include "codebreaker/base/cache.hpp"
#include "codebreaker/base/corpus.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/decompile/describe.hpp"
#include "codebreaker/emulate/emulator.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/shadow.hpp"
#include "codebreaker/translate/backends/c.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

// ---- the fingerprint in the cache: the base fingerprint's dict plus the lane's "trail" (a tuple of (site, taken)) ----
namespace codebreaker::base::cache::pickle {

template <> struct Codec<verify::lanes::win16::Fingerprint16> {
    using F = verify::lanes::win16::Fingerprint16;
    static void put_tuple(std::string& out, size_t n, const std::function<void(size_t)>& item) {
        if (n == 0) {
            put_op(out, ')');
            return;
        }
        put_mark(out);
        for (size_t i = 0; i < n; ++i) item(i);
        put_op(out, 't');
    }
    static void dump(std::string& out, const F& v) {
        put_op(out, '}');
        put_mark(out);
        auto key = [&](const char* k) { put_str(out, py::str(k)); };
        key("ret");
        Codec<std::optional<int64_t>>::dump(out, v.ret);
        key("exit");
        put_str(out, v.exit);
        key("truncated");
        Codec<bool>::dump(out, v.truncated);
        key("deps");
        put_tuple(out, v.deps.vec().size(), [&](size_t i) { put_str(out, v.deps.vec()[i]); });
        key("writes");
        Codec<std::optional<py::str>>::dump(out, v.writes);
        key("pop");
        Codec<std::optional<int64_t>>::dump(out, v.pop);
        key("saved");
        Codec<std::optional<bool>>::dump(out, v.saved);
        key("cov");
        put_tuple(out, v.cov.vec().size(), [&](size_t i) { put_int(out, v.cov.vec()[i]); });
        key("reach");
        put_tuple(out, v.reach.vec().size(), [&](size_t i) { put_int(out, v.reach.vec()[i]); });
        if (v.steps) {
            key("steps");
            put_int(out, *v.steps);
        }
        if (v.err) {
            key("err");
            put_str(out, *v.err);
        }
        key("trail");
        put_tuple(out, v.trail.vec().size(), [&](size_t i) {
            const auto& step = v.trail.vec()[i];
            put_tuple(out, 2, [&](size_t k) {
                if (k == 0) put_int(out, std::get<0>(step));
                else Codec<bool>::dump(out, std::get<1>(step));
            });
        });
        put_op(out, 'u');
    }
    static bool strings(const Node& n, py::list<py::str>& out) {
        if (n.kind != Node::Tuple && n.kind != Node::List) return false;
        py::list<py::str> r;
        for (const Node& c : n.items) {
            if (c.kind != Node::Str) return false;
            r.append(c.s);
        }
        out = r;
        return true;
    }
    static bool ints(const Node& n, py::list<int64_t>& out) {
        if (n.kind != Node::Tuple && n.kind != Node::List) return false;
        py::list<int64_t> r;
        for (const Node& c : n.items) {
            if (c.kind != Node::Int) return false;
            r.append(c.i);
        }
        out = r;
        return true;
    }
    static bool steps(const Node& n, py::list<std::tuple<int64_t, bool>>& out) {
        if (n.kind != Node::Tuple && n.kind != Node::List) return false;
        py::list<std::tuple<int64_t, bool>> r;
        for (const Node& e : n.items) {
            if ((e.kind != Node::Tuple && e.kind != Node::List) || e.items.size() != 2) return false;
            if (e.items[0].kind != Node::Int || (e.items[1].kind != Node::Bool && e.items[1].kind != Node::Int)) return false;
            r.append(std::make_tuple(e.items[0].i, e.items[1].kind == Node::Bool ? e.items[1].b : e.items[1].i != 0));
        }
        out = r;
        return true;
    }
    static bool load(const Node& n, F& v) {
        if (n.kind != Node::Dict || n.items.size() % 2 != 0) return false;
        F out;
        for (size_t i = 0; i + 1 < n.items.size(); i += 2) {
            const Node& k = n.items[i];
            const Node& x = n.items[i + 1];
            if (k.kind != Node::Str) return false;
            const std::string name = k.s.utf8();
            bool ok = false;
            if (name == "ret") ok = Codec<std::optional<int64_t>>::load(x, out.ret);
            else if (name == "exit") ok = Codec<py::str>::load(x, out.exit);
            else if (name == "truncated") ok = Codec<bool>::load(x, out.truncated);
            else if (name == "deps") ok = strings(x, out.deps);
            else if (name == "writes") ok = Codec<std::optional<py::str>>::load(x, out.writes);
            else if (name == "pop") ok = Codec<std::optional<int64_t>>::load(x, out.pop);
            else if (name == "saved") ok = Codec<std::optional<bool>>::load(x, out.saved);
            else if (name == "cov") ok = ints(x, out.cov);
            else if (name == "reach") ok = ints(x, out.reach);
            else if (name == "steps") ok = Codec<std::optional<int64_t>>::load(x, out.steps);
            else if (name == "err") ok = Codec<std::optional<py::str>>::load(x, out.err);
            else if (name == "trail") ok = steps(x, out.trail);
            if (!ok) return false;   // a field this port does not know: the run is made again
        }
        v = std::move(out);
        return true;
    }
};

}  // namespace codebreaker::base::cache::pickle

namespace codebreaker::verify::lanes::win16 {

namespace V = verdicts;
namespace w16 = emulate::windows16;
namespace dsc = decompile::describe;
using compare::Fingerprint;

const py::str KIND = py::str("ne");

const platforms::win16::Platform& P() { return platforms::win16::PLATFORM(); }
const verdicts::NotRun& NOT_RUN() { return V::NOT_RUN(); }

namespace {

int64_t arg_words() { return P().convention.arguments; }

std::string hexs(int64_t v) { return py::percent("%x", v).utf8(); }

bool code_less(const py::str& a, const py::str& b) { return a.u32() < b.u32(); }

// ---- random.Random(seed).randrange(lo, hi): CPython's Mersenne Twister, seeded by an integer ----
class Random {
    uint32_t mt_[624];
    int idx_ = 625;
    void init_genrand(uint32_t s) {
        mt_[0] = s;
        for (int i = 1; i < 624; ++i) mt_[i] = 1812433253U * (mt_[i - 1] ^ (mt_[i - 1] >> 30)) + static_cast<uint32_t>(i);
        idx_ = 624;
    }
    uint32_t next() {
        if (idx_ >= 624) {
            for (int k = 0; k < 624; ++k) {
                uint32_t y = (mt_[k] & 0x80000000U) | (mt_[(k + 1) % 624] & 0x7fffffffU);
                mt_[k] = mt_[(k + 397) % 624] ^ (y >> 1) ^ ((y & 1U) ? 0x9908b0dfU : 0U);
            }
            idx_ = 0;
        }
        uint32_t y = mt_[idx_++];
        y ^= y >> 11;
        y ^= (y << 7) & 0x9d2c5680U;
        y ^= (y << 15) & 0xefc60000U;
        y ^= y >> 18;
        return y;
    }

  public:
    explicit Random(uint64_t seed) {   // random.seed(int): init_by_array over the 32-bit chunks of the number
        std::vector<uint32_t> key;
        do {
            key.push_back(static_cast<uint32_t>(seed & 0xFFFFFFFFU));
            seed >>= 32;
        } while (seed);
        init_genrand(19650218U);
        size_t i = 1, j = 0;
        for (size_t k = std::max<size_t>(624, key.size()); k; --k) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525U)) + key[j] + static_cast<uint32_t>(j);
            ++i;
            ++j;
            if (i >= 624) {
                mt_[0] = mt_[623];
                i = 1;
            }
            if (j >= key.size()) j = 0;
        }
        for (size_t k = 623; k; --k) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941U)) - static_cast<uint32_t>(i);
            ++i;
            if (i >= 624) {
                mt_[0] = mt_[623];
                i = 1;
            }
        }
        mt_[0] = 0x80000000U;
    }
    int64_t randrange(int64_t lo, int64_t hi) {
        int64_t width = hi - lo;
        int k = 0;
        for (int64_t w = width; w; w >>= 1) ++k;
        int64_t r = next() >> (32 - k);
        while (r >= width) r = next() >> (32 - k);
        return lo + r;
    }
};

Vector vec(const py::list<int64_t>& args) {
    Vector v;
    v.args = args;
    return v;
}

// Python's repr of the parts of a cache key
py::str rtuple(const std::vector<py::str>& parts) {
    if (parts.size() == 1) return py::str("(") + parts[0] + py::str(",)");
    return py::str("(") + py::join(py::str(", "), py::list<py::str>(parts)) + py::str(")");
}
py::str rlist(const std::vector<py::str>& parts) { return py::str("[") + py::join(py::str(", "), py::list<py::str>(parts)) + py::str("]"); }
py::str rbool(bool b) { return py::str(b ? "True" : "False"); }
py::str rints(const py::list<int64_t>& xs) {
    std::vector<py::str> p;
    for (int64_t x : xs) p.push_back(py::repr(x));
    return rtuple(p);
}
py::str rspan(const std::tuple<int64_t, int64_t>& s) { return rtuple({py::repr(std::get<0>(s)), py::repr(std::get<1>(s))}); }
py::str rmem(const std::optional<MemInit>& mem) {
    if (!mem) return py::str("None");
    std::vector<py::str> p;
    for (const auto& e : *mem) {
        const emulate::windows16::Where& w = std::get<0>(e);
        py::str where = std::holds_alternative<int64_t>(w)
                            ? py::repr(std::get<int64_t>(w))
                            : rtuple({py::repr(std::get<0>(std::get<1>(w))), py::repr(std::get<1>(std::get<1>(w)))});
        p.push_back(rtuple({where, py::repr(std::get<1>(e))}));
    }
    if (p.empty()) return py::str("()");
    return rtuple(p);
}
py::str rframe(const Frame& f) {
    std::vector<py::str> p;
    for (const auto& e : f) p.push_back(rtuple({py::repr(std::get<0>(e)), py::repr(std::get<1>(e))}));
    return rtuple(p);
}

}  // namespace

// ---- inputs -------------------------------------------------------------------------------------------------------
namespace {

py::list<Vector> scattered(int64_t n, int64_t sel) {
    // Four more, every word a different one spread over the segment (the same on every run): with the same word in each
    // argument, or small ones pointing at the quiet start of the data segment, a function reading the wrong argument
    // or the wrong field read what it should have -- planted that way, 17 of 40 such bugs went unseen.
    Random rnd(0x16);
    py::list<Vector> out;
    for (int64_t k = 0; k < 4; ++k) {
        py::list<int64_t> words;
        for (int64_t i = 0; i < n; ++i) words.append(rnd.randrange(0x0100, 0x7F00) & ~int64_t(1));
        if (k % 2) {   // (and as far pointers: offset, selector)
            py::list<int64_t> w2;
            for (int64_t i = 0; i < n; ++i) w2.append(i % 2 ? sel : words[i]);
            words = w2;
        }
        out.append(vec(words));
    }
    return out;
}

}  // namespace

py::list<Vector> vectors() {
    // The inputs every function is run on: eight words of arguments each.  Plain numbers alone go nowhere in 16-bit code --
    // most functions take far pointers, and the first use of a pointer whose selector word is 3 or 0x400 is a protection
    // fault, on both sides, before anything has been compared.  So half the inputs are (offset, selector) pairs naming a
    // real, writable segment, in both orders.
    int64_t n = arg_words(), sel = w16::SAFE_ARG;
    py::list<Vector> out;
    out.append(vec(py::list<int64_t>(std::vector<int64_t>(static_cast<size_t>(n), 0))));
    out.append(vec(py::list<int64_t>(std::vector<int64_t>(static_cast<size_t>(n), 0x0400))));
    out.append(vec(py::list<int64_t>(std::vector<int64_t>(static_cast<size_t>(n), 0x0080))));
    out.append(vec(lane_common::counting(n)));
    out.append(vec(lane_common::counting(n, 0x100, 0x100)));
    out.append(vec(lane_common::descending(n, 0xFFFF)));
    out.append(vec(lane_common::alternating(n, lane_common::Pattern(std::function<int64_t(int64_t)>([](int64_t i) { return 0x20 * (i + 1); })),
                                            lane_common::Pattern(sel))));   // offset, selector, ...
    out.append(vec(lane_common::alternating(n, lane_common::Pattern(sel),
                                            lane_common::Pattern(std::function<int64_t(int64_t)>([](int64_t i) { return 0x20 * (i + 1); })))));   // selector, offset, ...
    out.append(vec(py::list<int64_t>(std::vector<int64_t>(static_cast<size_t>(n), sel))));
    out.append(vec(py::list<int64_t>{1, 0x40, sel, 2, 0x80, sel, 3, 0x100}));
    for (const Vector& v : scattered(n, sel)) out.append(v);
    return out;
}

py::str _call(const Hit& hit, const py::list<int64_t>& inputs, const py::dict<py::str, int64_t>& entry,
              const std::optional<py::dict<int64_t, py::str>>& segs) {
    // One Windows call as text (lane_common.call): its name, and its arguments when their size is known.  An argument is
    // written as what it IS rather than its bit pattern wherever the two builds would legitimately differ: the data or
    // stack selector, an address in the stack, one of the function's own inputs, the selector of one of the module's code
    // segments (`segs`), and a far pointer into one of them as what it points at (hit.pointed).
    py::str name = hit.api ? *hit.api : py::str("?");
    if (!hit.arg_bytes) return lane_common::call(name);
    int64_t size = *hit.arg_bytes;
    py::list<int64_t> all = hit.args_stack ? *hit.args_stack : py::list<int64_t>();
    int64_t take = py::floordiv(size, 2);
    // (words[:size // 2]: a negative count takes from the end less)
    int64_t count = take >= 0 ? std::min(take, all.size()) : std::max<int64_t>(0, all.size() + take);
    std::vector<int64_t> words(all.vec().begin(), all.vec().begin() + count);
    std::optional<int64_t> ss = entry.get(py::str("SS")), ds = entry.get(py::str("DS"));
    py::list<py::str> out;
    for (size_t i = 0; i < words.size(); ++i) {
        int64_t w = words[i];
        std::optional<int64_t> nxt = i + 1 < words.size() ? std::optional<int64_t>(words[i + 1]) : std::nullopt;
        std::optional<py::bytes> pointed = hit.pointed.get(static_cast<int64_t>(i));
        if (ds && w == *ds) {
            out.append(py::str("DS"));
        } else if (segs && segs->contains(w & ~int64_t(3))) {
            out.append(segs->at(w & ~int64_t(3)));
        } else if (segs && nxt && segs->contains(*nxt & ~int64_t(3)) && pointed) {
            out.append(py::str("&") + pointed->hex());   // (a far pointer into a code segment: what is there)
        } else if (ss && w == *ss) {
            out.append(py::str("SS"));
        } else if (nxt == ss) {
            out.append(py::str("stack"));   // offset part of a far pointer into the stack
        } else {
            out.append(lane_common::argument(w, inputs, 0xFF));
        }
    }
    return lane_common::call(name, out);
}

// ---- directed inputs ----------------------------------------------------------------------------------------------
namespace {

std::mutex g_cache_mutex;
std::map<std::tuple<py::str, int64_t, int64_t, int64_t>, py::list<Vector>>& directed_cache() {
    static std::map<std::tuple<py::str, int64_t, int64_t, int64_t>, py::list<Vector>> c;
    return c;
}

}  // namespace

py::list<Vector> directed(const py::str& module_path, int64_t seg, const std::tuple<int64_t, int64_t>& span, int64_t most) {
    // Inputs aimed at what the function's own code tests: the constants it compares its values with.  A branch on
    // `cmp ax, 0x102` is never taken by inputs chosen without looking, and everything behind it stays unexercised; giving
    // the constant in each argument position (alone, and as the offset of a valid far pointer) reaches it.
    // (`most` is not part of the Python's key either: the first caller's answer is kept)
    auto key = std::make_tuple(module_path, seg, std::get<0>(span), std::get<1>(span));
    {
        std::lock_guard<std::mutex> g(g_cache_mutex);
        auto it = directed_cache().find(key);
        if (it != directed_cache().end()) return it->second.copy();
    }
    formats::ne::NEFile nef(module_path);
    formats::ne::Segment s = nef.segments()[seg - 1];
    py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
    std::shared_ptr<const dsc::Decoder> md = dsc::decoder16(true);
    std::vector<int64_t> seen;
    auto has = [&](int64_t c) { return std::find(seen.begin(), seen.end(), c) != seen.end(); };
    // (a switch written as a chain of subtractions -- `dec ax / dec ax / sub ax, 4 / sub ax, 9 / sub ax, 72h`, each case's
    // `je` after its step: a window procedure's messages 1, 2, 6, 0Fh, 81h -- tests the running total, not each step's
    // number: the totals are the cases)
    std::map<int64_t, int64_t> total;
    for (const dsc::Insn& ins : md->disasm(code.slice(std::get<0>(span), std::get<1>(span)), std::get<0>(span))) {
        const auto& ops = ins.operands;
        std::optional<int64_t> reg;
        if (!ops.empty() && ops[0].type == decompile::inline_asm::X86_OP_REG) reg = ops[0].reg;
        const std::string& mn = ins.mn;
        if (reg && (mn == "dec" || mn == "inc" || mn == "sub" || mn == "add") &&
            (mn == "dec" || mn == "inc" || (ops.size() > 1 && ops[1].type == decompile::inline_asm::X86_OP_IMM))) {
            int64_t step = (mn == "dec" || mn == "inc") ? 1 : ops[1].imm;
            step = (mn == "dec" || mn == "sub") ? step : -step;
            int64_t c = (total.count(*reg) ? total[*reg] : 0) + step;
            total[*reg] = c & 0xFFFF;
            c = total[*reg];
            if (!has(c) && c != 0 && c != 0xFFFF) seen.push_back(c);
            continue;
        }
        if (reg && mn != "cmp" && mn != "test" && mn != "or") total.erase(*reg);   // (the register set anew: a chain ends)
        if (mn != "cmp" && mn != "test" && mn != "and") continue;
        for (const auto& op : ops) {
            if (op.type == decompile::inline_asm::X86_OP_IMM) {
                int64_t c = op.imm & 0xFFFF;
                if (!has(c) && c != 0 && c != 0xFFFF) seen.push_back(c);
            }
        }
    }
    int64_t n = arg_words(), sel = w16::SAFE_ARG;
    py::list<Vector> out;
    size_t upto = std::min<size_t>(seen.size(), static_cast<size_t>(std::max<int64_t>(most / 2, 24)));
    for (size_t i = 0; i < upto; ++i) {
        int64_t c = seen[i];
        out.append(vec(py::list<int64_t>(std::vector<int64_t>(static_cast<size_t>(n), c))));
        out.append(vec(lane_common::alternating(n, lane_common::Pattern(c), lane_common::Pattern(sel))));
    }
    std::lock_guard<std::mutex> g(g_cache_mutex);
    directed_cache()[key] = out;
    return out.copy();
}

// ---- a caller-frame helper ----------------------------------------------------------------------------------------
std::optional<Frame> frame_slots(const py::str& text, const py::str& name) {
    // [(BP less, bytes)] of a caller-frame helper's parameters, in their order, from its C; or None
    int64_t at = text.find(py::str("{"));
    py::str head = at < 0 ? text : text.slice(0, at);
    std::optional<py::re::Match> m = py::re::search(py::str("\\b") + py::re::escape(name) + py::str("\\(((?:frame_[0-9a-f]+,?)+)\\)"), head);
    if (!m) return std::nullopt;
    py::dict<py::str, py::str> types;
    for (const auto& e : py::re::compile(py::str("(\\w+) __far \\*frame_([0-9a-f]+);")).findall_groups<py::str, py::str>(head).vec())
        types.set(std::get<1>(e), std::get<0>(e));
    Frame out;
    for (const py::str& p : py::split(m->group(1), py::str(","))) {
        py::str less = p.slice_from(py::str("frame_").size());
        if (!types.contains(less)) return std::nullopt;
        py::str ty = types.at(less);
        out.append(std::make_tuple(py::int_parse(less, 16), translate::backends::c::SIZEOF.get(ty, int64_t(2))));
    }
    return out;
}

namespace {

py::list<int64_t> frame_arguments(const Frame& frame) {
    // The words a frame helper's C is entered with: a far pointer into the stack segment to each place, the last
    // parameter deepest (its callers push them so)
    py::list<int64_t> words;
    for (int64_t i = frame.size() - 1; i >= 0; --i) {
        words.append(w16::STACK_SEL);
        words.append((CALLER_BP - std::get<0>(frame[i])) & 0xFFFF);
    }
    return words;
}

}  // namespace

// ---- fingerprints -------------------------------------------------------------------------------------------------
py::list<py::str> _effects(const emulate::run::Run& r, const std::function<py::str(const Hit&)>& call) {
    // The run's effects outside itself, in the order it had them: each Windows call (`call(hit)`) and each access to a
    // device (emulate/ports16) -- `out(3fb,83)`, `in(3fd)`.  What a function did to the UART is what it is for: a rebuild
    // writing another divisor, or reading the line status of another port, did something else.
    std::vector<std::tuple<int64_t, int64_t, int64_t, py::str>> out;
    std::shared_ptr<w16::Trace16> raw = std::static_pointer_cast<w16::Trace16>(r.raw);
    for (int64_t k = 0; k < r.calls.size(); ++k) {
        const Hit& h = raw->imports_hit[k];
        out.emplace_back(h.n ? *h.n : 0, 0, k, call(h));
    }
    int64_t k = 0;
    for (const emulate::run::IO& e : r.io) {
        py::str text = e.kind == py::str("...")  ? py::str("...")
                       : e.kind == py::str("in") ? py::percent("in(%x)", e.port)
                                                 : py::percent("out(%x,%x)", e.port, e.value);
        out.emplace_back(e.step ? *e.step : 0, 1, k++, text);
    }
    std::sort(out.begin(), out.end(), [](const auto& a, const auto& b) {
        return std::tie(std::get<0>(a), std::get<1>(a), std::get<2>(a)) < std::tie(std::get<0>(b), std::get<1>(b), std::get<2>(b));
    });
    py::list<py::str> res;
    for (const auto& t : out) res.append(std::get<3>(t));
    return res;
}

namespace {

std::mutex g_jump_mutex;
struct Jump {
    py::bytes code;
    std::map<int64_t, std::optional<std::tuple<int64_t, int64_t>>> known;
};
std::map<std::pair<py::str, int64_t>, std::shared_ptr<Jump>>& jumps_cache() {
    static std::map<std::pair<py::str, int64_t>, std::shared_ptr<Jump>> c;
    return c;
}

}  // namespace

concolic::Trail trail(const py::str& module_path, int64_t seg, const std::tuple<int64_t, int64_t>& span, const py::list<int64_t>& path) {
    // [(site, taken)] -- the function's own conditional jumps in the order a run took them, from the run's instructions
    // (linear addresses): the path it went, as the explorer names one (concolic), for a run made without a shadow.
    std::shared_ptr<Jump> jump;
    {
        std::lock_guard<std::mutex> g(g_jump_mutex);
        auto& slot = jumps_cache()[{module_path, seg}];
        if (!slot) {
            formats::ne::NEFile nef(module_path);
            formats::ne::Segment s = nef.segments()[seg - 1];
            slot = std::make_shared<Jump>();
            slot->code = nef.data.slice(s.file_off, s.file_off + s.length);
        }
        jump = slot;
    }
    int64_t base = w16::segment_base(seg);
    int64_t lo = base + std::get<0>(span), hi = base + std::get<1>(span);
    concolic::Trail out;
    std::shared_ptr<const dsc::Decoder> md;
    for (int64_t i = 0; i + 1 < path.size(); ++i) {
        int64_t a = path[i];
        if (!(lo <= a && a < hi)) continue;
        int64_t off = a - base;
        std::optional<std::tuple<int64_t, int64_t>> j;
        bool have;
        {
            std::lock_guard<std::mutex> g(g_jump_mutex);
            have = jump->known.count(off) != 0;
            if (have) j = jump->known[off];
        }
        if (!have) {
            if (!md) md = dsc::decoder16();
            py::list<dsc::Insn> ins = md->disasm(jump->code.slice(off, off + 8), off);
            if (!ins.empty()) {
                const dsc::Insn& in = ins[0];
                const std::string& mn = in.mn;
                if (((mn.rfind("j", 0) == 0 && mn != "jmp") || mn.rfind("loop", 0) == 0) && in.os.rfind("0x", 0) == 0)
                    j = std::make_tuple(py::int_parse(py::str(in.os), 16), off + in.size);
            }
            std::lock_guard<std::mutex> g(g_jump_mutex);
            jump->known[off] = j;
        }
        if (j && std::get<0>(*j) != std::get<1>(*j)) {
            int64_t target = path[i + 1] - base;
            if (target == std::get<0>(*j) || target == std::get<1>(*j)) out.append(std::make_tuple(a, target == std::get<0>(*j)));
        }
    }
    return out;
}

namespace {

// Python's `lane_common.remembered` for the lane's own fingerprint (the cache file's one pickle holds the trail too)
Fingerprint16 remembered16(const std::optional<py::str>& key, const std::function<Fingerprint16()>& run,
                           const std::function<bool(const Fingerprint16&)>& keep) {
    if (!key || !lane_common::fp_cache_on()) return run();
    if (!base::cache::GENERATION_REPRS.contains(py::str("fingerprints")))
        base::cache::GENERATION_REPRS.set(py::str("fingerprints"), []() -> std::optional<py::str> { return py::repr(compare::REV); });
    std::optional<Fingerprint16> fp = base::cache::value<Fingerprint16>(py::str("fingerprints"), *key);
    if (!fp) {
        fp = run();
        if (!fp->err && (!keep || keep(*fp))) base::cache::keep<Fingerprint16>(py::str("fingerprints"), *key, *fp);
    }
    return *fp;
}

}  // namespace

Fingerprint16 fingerprint(const py::str& path, int64_t seg, int64_t off, bool near_, const std::optional<bool>& wide, const Vector& v,
                          const std::optional<py::bytes>& data_image, const std::optional<std::tuple<int64_t, int64_t>>& span,
                          const py::list<std::tuple<int64_t, int64_t>>& own, const std::optional<Frame>& frame,
                          const std::optional<emulate::run::Run>& ran, const std::optional<py::dict<int64_t, py::str>>& segs,
                          const std::optional<int64_t>& steps) {
    // What one run of one function did, in a form two builds can be compared on.  `own`: ranges of the data segment that
    // are the build's own (_own_data), whose writes are left out.  `frame`: a caller-frame helper's places (frame_slots),
    // whose writes are compared.  `ran`: the record of this very run, made already (by the path explorer).  With `span`,
    // also the path the function's own jumps took ("trail").  `segs`: the module's code segment selectors as _call names
    // them.  `steps`: the instructions a run may take, when not the emulator's own limit.
    std::optional<MemInit> mem = v.mem;
    std::optional<int64_t> bp;
    if (frame) {   // (a caller-frame helper: see CALLER_BP)
        bp = CALLER_BP;
        MemInit all = mem ? mem->copy() : MemInit();
        for (const auto& e : *frame) {
            int64_t less = std::get<0>(e), size = std::get<1>(e);
            all.append(std::make_tuple(emulate::windows16::Where(w16::STACK_BASE + ((CALLER_BP - less) & 0xFFFF)),
                                       py::bytes(std::string(static_cast<size_t>(size), static_cast<char>(FRAME_FILL)))));
        }
        mem = all;
    }
    emulate::run::Run r(std::nullopt);
    try {
        if (ran) {
            r = *ran;
        } else {
            ProbeOptions o;
            o.seg = seg;
            o.offset = off;
            o.args = v.args;
            o.near_ = near_;
            o.data_image = data_image;
            o.stub_ret = v.stub_ret;
            o.mem_init = mem;
            o.bp = bp;
            o.ports = true;
            o.insn_cap = steps;
            r = w16::recorded(path, o);
        }
    } catch (const py::ImportError&) {
        throw;
    } catch (const py::Exception& e) {
        Fingerprint16 fp;
        fp.err = py::str(e.type());
        return fp;
    } catch (const std::exception&) {
        Fingerprint16 fp;
        fp.err = py::str("Exception");
        return fp;
    }
    std::shared_ptr<w16::Trace16> raw = std::static_pointer_cast<w16::Trace16>(r.raw);
    py::str ex = r.ended();
    std::optional<int64_t> ds = raw->layout.ds_base;
    py::dict<int64_t, std::tuple<py::str, int64_t>> places;   // a frame helper's places in the stack: byte -> its own
    if (frame) {
        for (const auto& e : *frame) {
            for (int64_t i = 0; i < std::get<1>(e); ++i)
                places.set(w16::STACK_BASE + ((CALLER_BP - std::get<0>(e) + i) & 0xFFFF), std::make_tuple(py::str("F"), -std::get<0>(e) + i));
        }
    }
    std::function<std::optional<lane_common::Where>(int64_t)> place = [&](int64_t a) -> std::optional<lane_common::Where> {
        if (!places.empty()) {
            bool any = false;
            for (int64_t i = 0; i < 4; ++i) any = any || places.contains(a + i);
            if (any) {
                lane_common::ByteNamer namer = [places, a](int64_t i) -> std::optional<lane_common::Place> {
                    auto got = places.get(a + i);
                    if (!got) return std::nullopt;
                    return lane_common::Place(*got);
                };
                return lane_common::Where(namer);
            }
        }
        if (ds && *ds <= a && a < *ds + 0x10000) {
            for (const auto& range : own)
                if (std::get<0>(range) <= a - *ds && a - *ds < std::get<1>(range)) return std::nullopt;
            return lane_common::Where(std::make_tuple(py::str("D"), a - *ds));
        }
        if (w16::HEAP_BASE <= a && a < w16::HEAP_BASE + 64 * w16::HEAP_STRIDE)
            return lane_common::Where(std::make_tuple(py::str("H"), a - w16::HEAP_BASE));
        return std::nullopt;   // the stack, or somewhere wild
    };
    // a byte left holding what it held on entry is not an effect (one compiler clears a flag with a word write, another
    // with a byte write)
    std::optional<py::str> writes = lane_common::left(r, place, nullptr, 4, false);
    emulate::windows16::Regs entry = raw->entry_regs ? *raw->entry_regs : emulate::windows16::Regs();
    int64_t base = raw->code_base ? *raw->code_base : 0;
    int64_t lo = span ? std::get<0>(*span) : off, hi = span ? std::get<1>(*span) : off;
    std::set<int64_t> covered;
    for (int64_t a : r.path)
        if (lo <= a - base && a - base < hi) covered.insert(a - base - lo);
    py::list<int64_t> cov(std::vector<int64_t>(covered.begin(), covered.end()));
    std::optional<int64_t> ret;
    if (ex == py::str("ret") && wide) {   // None: the function returns nothing
        const char* name = *wide ? "DX:AX" : "AX";
        int64_t value = 0;
        auto got = r.results.get(py::str(name));
        if (got && std::holds_alternative<int64_t>(*got)) value = std::get<int64_t>(*got);
        ret = value;
        // An exported function starts `push ds / pop ax`; one that sets no result returns with that still in AX.  Its own
        // data selector is a leftover, not a result.
        std::optional<int64_t> eds = entry.get(py::str("DS"));
        int64_t cmp = (eds && *eds != 0) ? *eds : -1;
        if ((value & 0xFFFF) == cmp && !*wide) ret = LEFTOVER;
    }
    py::list<py::str> deps = _effects(r, [&](const Hit& h) { return _call(h, v.args, entry, segs); });
    Fingerprint base_fp = lane_common::fingerprint(r, deps, ret, writes, r.kept(P().convention.preserved), cov);
    Fingerprint16 fp;
    static_cast<Fingerprint&>(fp) = base_fp;
    if (span) fp.trail = trail(path, seg, *span, r.path);
    return fp;
}

// ---- the judge ----------------------------------------------------------------------------------------------------
namespace {

int64_t steps_limit() { return emulate::emulator::_MAX_INSNS; }

}  // namespace

py::dict<int64_t, py::str> _named(const py::list<int64_t>& original_segments, const std::optional<py::dict<int64_t, int64_t>>& order) {
    // {selector: "seg<n>"} for a build's code segments, n the original's number of the segment each holds: the original's
    // own (`order` None), or a rebuild's through `order` (its segment -> the original's).
    py::dict<int64_t, py::str> out;
    if (order) {
        for (const auto& kv : order->items())
            out.set(w16::_sel(w16::_MODULE_SLOT + std::get<0>(kv) - 1), py::percent("seg%d", std::get<1>(kv)));
    } else {
        for (int64_t n : original_segments) out.set(w16::_sel(w16::_MODULE_SLOT + n - 1), py::percent("seg%d", n));
    }
    return out;
}

std::tuple<py::str, py::str, py::list<py::str>, py::str> _judge(const JudgeContext& context, const JudgeItem& item) {
    // Worker.  item = (key, seg, off, near, wide, span, insns, rebuilt seg, rebuilt off[, frame]): `frame` a caller-frame
    // helper's places (frame_slots), else None.
    const py::str& module_path = context.module_path;
    const py::str& dll = context.dll;
    const py::bytes& data_image = context.data_image;
    const auto& own = context.own;
    const auto& order = context.order;   // rebuilt segment -> the original's
    int64_t seg = item.seg, off = item.off, bseg = item.bseg, boff = item.boff;
    bool near_ = item.near_;
    const std::optional<bool>& wide = item.wide;
    const std::tuple<int64_t, int64_t>& span = item.span;
    int64_t insns = item.insns;
    const std::optional<Frame>& frame = item.frame;
    std::optional<Fingerprint16> last;
    std::map<py::str, Fingerprint16, bool (*)(const py::str&, const py::str&)> runs(code_less);
    std::shared_ptr<concolic::ExplorationStats> paths;

    py::list<int64_t> order_values;
    for (int64_t x : order.values()) order_values.append(x);
    py::dict<int64_t, py::str> orig_names = _named(order_values);
    py::dict<int64_t, py::str> rebuilt_names = _named(py::list<int64_t>(), order);

    std::vector<std::pair<int64_t, int64_t>> sorted_order;
    for (const auto& kv : order.items()) sorted_order.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(sorted_order.begin(), sorted_order.end());

    auto original = [&](const Vector& v, const std::optional<emulate::run::Run>& ran = std::nullopt) -> Fingerprint16 {
        // the original's run depends only on the original: remembered across rebuilds (a run cut off at the step limit is
        // not), and within this judgement (the path explorer's runs too) (and on what of the rebuild it is compared with:
        // the places in the data segment the rebuild keeps for itself, left out on both sides, and which code segment of
        // the rebuild holds which of the original's)
        std::vector<py::str> parts;
        std::optional<py::str> sha = base::corpus::sha(module_path);
        parts.push_back(py::repr(py::str("ne")));
        parts.push_back(py::repr(REV));
        parts.push_back(sha ? py::repr(*sha) : py::str("None"));
        parts.push_back(py::repr(seg));
        parts.push_back(py::repr(off));
        parts.push_back(rbool(near_));
        parts.push_back(wide ? rbool(*wide) : py::str("None"));
        parts.push_back(rints(v.args));
        parts.push_back(rspan(span));
        parts.push_back(rmem(v.mem));
        parts.push_back(v.stub_ret ? py::repr(*v.stub_ret) : py::str("None"));
        {
            std::vector<py::str> o;
            for (const auto& r : own) o.push_back(rspan(r));
            parts.push_back(o.empty() ? py::str("()") : rtuple(o));
        }
        {
            std::vector<py::str> o;
            for (const auto& kv : sorted_order) o.push_back(rtuple({py::repr(kv.first), py::repr(kv.second)}));
            parts.push_back(rlist(o));
        }
        if (frame) {
            parts.push_back(py::repr(CALLER_BP));
            parts.push_back(py::repr(FRAME_FILL));
            parts.push_back(rframe(*frame));
        }
        py::str k = compare::cache_key_repr(rtuple(parts));
        auto have = runs.find(k);
        if (have != runs.end()) {
            last = have->second;
            return have->second;
        }
        Fingerprint16 fp = remembered16(
            k,
            [&] {
                return fingerprint(module_path, seg, off, near_, wide, v, std::nullopt, span, own, frame, ran, orig_names);
            },
            [](const Fingerprint16& f) { return !f.truncated; });
        runs.emplace(k, fp);
        last = fp;
        return fp;
    };

    auto rebuilt = [&](const Vector& in) -> Fingerprint16 {
        Vector v = in;
        if (frame) v.args = frame_arguments(*frame);   // (pointers to the places, not the input's words)
        Fingerprint16 fp = fingerprint(dll, bseg, boff, near_, wide, v, std::optional<py::bytes>(data_image), std::nullopt, own, frame,
                                       std::nullopt, rebuilt_names);
        if (fp.exit == py::str("truncated") && last && last->exit == py::str("ret")) {
            // (the original returned and its C was still going: C takes more instructions than the code it was made from.
            // Given RETRY times the steps; one still going then does not end)
            fp = fingerprint(dll, bseg, boff, near_, wide, v, std::optional<py::bytes>(data_image), std::nullopt, own, frame, std::nullopt,
                             rebuilt_names, RETRY * steps_limit());
        }
        if (last && last->ret && *last->ret == LEFTOVER && !fp.err && fp.ret) fp.ret = LEFTOVER;   // the original returned no value here
        return fp;
    };

    auto aimed = [&](const py::set<int64_t>&, const py::set<int64_t>&) -> py::list<Vector> {
        // the directed inputs, then those found path by path (or guessed) seeded with every input -- of the fixed ones the
        // paths they took are known: one per path is run again
        py::list<Vector> d = directed(module_path, seg, span);
        py::list<Vector> seeds = vectors() + d;
        py::dict<int64_t, concolic::Trail> known;
        py::list<Vector> fixed = vectors();
        for (int64_t i = 0; i < fixed.size(); ++i) {
            Fingerprint16 fp = original(fixed[i]);
            if (!fp.err) known.set(i, fp.trail);
        }
        Explored got = explore_vectors(module_path, seg, off, near_, span, seeds, known);
        if (!frame) {   // (the explorer ran the original as the judge does)
            for (const auto& e : got.made) original(std::get<0>(e), w16::record(std::get<1>(e)));
        }
        paths = got.exploration;
        return d + got.inputs;
    };

    auto judge = [&](compare::Info& info) -> std::tuple<py::str, py::str, py::list<py::str>> {
        compare::FingerprintOf<Vector> fo = [&](const Vector& v) -> Fingerprint { return original(v); };
        compare::FingerprintOf<Vector> fr = [&](const Vector& v) -> Fingerprint { return rebuilt(v); };
        auto [verdict, reason, limits] =
            compare::compare<Vector>(fo, fr, vectors(), py::list<Vector>(), insns, aimed, &info);
        if (verdict != V::DIVERGENT) {
            limits = limits + lane_common::path_limits(paths.get());
        }
        return {verdict, reason, limits};
    };
    return lane_common::judged(judge);
}

// ---- branch-guided inputs (verify/guided, through lane_common.guided) -----------------------------------------------
// The explorer reads an input's arguments as words rising up the stack from `args_at`; the tracer pushes them in Pascal
// order, the first deepest.  So to the explorer an input's words are the other way round, and its memory is linear
// addresses of the original's run -- in the data segment ("D", offset there in whichever build), or the scratch segment
// pointers go to (at one place in every build); a place elsewhere is not something an input sets, and is left out.
guided::Input _to_explorer(const Vector& v, const std::optional<int64_t>& ds) {
    guided::Input g;
    py::list<int64_t> args;
    for (int64_t i = v.args.size() - 1; i >= 0; --i) args.append(v.args[i]);
    g.args = args;
    if (v.mem && !v.mem->empty()) {
        py::list<std::tuple<int64_t, py::bytes>> mem;
        for (const auto& e : *v.mem) {
            const emulate::windows16::Where& w = std::get<0>(e);
            int64_t at;
            if (std::holds_alternative<int64_t>(w)) {
                at = std::get<int64_t>(w);
            } else {
                if (!ds) throw py::TypeError("unsupported operand type(s) for +: 'NoneType' and 'int'");
                at = *ds + std::get<1>(std::get<1>(w));
            }
            mem.append(std::make_tuple(at, std::get<1>(e)));
        }
        g.mem_init = mem;
    }
    if (v.stub_ret) g.stub_ret = v.stub_ret;
    return g;
}

Vector _from_explorer(const guided::Input& g, const std::optional<int64_t>& ds) {
    Vector v;
    py::list<int64_t> args;
    for (int64_t i = g.args.size() - 1; i >= 0; --i) args.append(g.args[i] & 0xFFFF);
    v.args = args;
    MemInit mem;
    if (g.mem_init) {
        for (const auto& e : *g.mem_init) {
            int64_t at = std::get<0>(e);
            if (ds && *ds <= at && at < *ds + 0x10000)
                mem.append(std::make_tuple(emulate::windows16::Where(std::make_tuple(py::str("D"), at - *ds)), std::get<1>(e)));
            else if (w16::SCRATCH_BASE <= at && at < w16::SCRATCH_BASE + 0x10000)
                mem.append(std::make_tuple(emulate::windows16::Where(at), std::get<1>(e)));
        }
    }
    if (!mem.empty()) v.mem = mem;
    if (g.stub_ret) v.stub_ret = *g.stub_ret & 0xFFFFFFFFLL;
    return v;
}

namespace {

// the options the lane's runs are made with: `probe` as given, else the judge's own (ports on); the call sets the rest
ProbeOptions run_options(const std::optional<ProbeOptions>& probe, int64_t seg, int64_t off, bool near_, const Vector& v,
                         const std::tuple<int64_t, int64_t>& span) {
    ProbeOptions o;
    if (probe) o = *probe; else o.ports = true;
    o.seg = seg;
    o.offset = off;
    o.args = v.args;
    o.near_ = near_;
    o.stub_ret = v.stub_ret;
    o.mem_init = v.mem;
    o.branches = span;
    return o;
}

// a run as the explorer reads it
guided::Observation observed(const w16::Trace16& r, const Vector& v) {
    guided::Observation o;
    if (r.branches) {
        for (const auto& b : *r.branches) {
            std::optional<guided::Compare> cmp;
            if (std::get<2>(b)) {
                const emulate::native_trace::Compare& c = *std::get<2>(b);
                guided::Compare g;
                g.mn = std::get<0>(c);
                g.size = static_cast<int64_t>(std::get<1>(c));
                g.a = static_cast<int64_t>(std::get<2>(c));
                g.b = static_cast<int64_t>(std::get<3>(c));
                if (std::get<4>(c)) g.at_a = static_cast<int64_t>(*std::get<4>(c));
                if (std::get<5>(c)) g.at_b = static_cast<int64_t>(*std::get<5>(c));
                g.has_at = true;
                cmp = g;
            }
            o.branches.append(std::make_tuple(static_cast<int64_t>(std::get<0>(b)), std::get<1>(b), cmp));
        }
    }
    for (uint64_t a : r.path) o.reach.add(static_cast<int64_t>(a));
    o.args_at = w16::STACK_BASE + w16::FAR_RET - 2 * v.args.size();
    o.stack = std::make_tuple(w16::STACK_BASE, w16::STACK_BASE + 0x10000);
    return o;
}

}  // namespace

std::optional<guided::Observation> _branch_run(const py::str& module_path, int64_t seg, int64_t off, bool near_,
                                               const std::tuple<int64_t, int64_t>& span, const Vector& v,
                                               const std::optional<ProbeOptions>& probe) {
    // The original run on `v` with its branches logged, as the explorer reads it, or None when the run is not an
    // observation.  `probe`: more of probe's options, for every run.
    try {
        w16::Trace16 r = w16::probe(module_path, run_options(probe, seg, off, near_, v, span));
        return observed(r, v);
    } catch (const py::ImportError&) {
        throw;
    } catch (const std::exception&) {
        return std::nullopt;
    }
}

py::list<Vector> guided_vectors(const py::str& module_path, int64_t seg, int64_t off, bool near_, const std::tuple<int64_t, int64_t>& span,
                                const py::list<Vector>& seeds, const std::optional<ProbeOptions>& probe) {
    // Inputs that take branches of the original no seed took (verify/guided).  `probe`: how the original is run, when not as
    // the judge runs it -- the build's gate (verify/gate16/isolated16) answers the module's own callees and fills the data
    // segment's tail, and explores so.
    int64_t auto_ = formats::ne::NEFile(module_path).h.autodata_seg;
    std::optional<int64_t> ds = auto_ ? std::optional<int64_t>(w16::segment_base(auto_)) : std::nullopt;
    py::list<guided::Input> explorer_seeds;
    for (const Vector& v : seeds) explorer_seeds.append(_to_explorer(v, ds));
    py::list<guided::Input> kept = lane_common::guided(
        [&](const guided::Input& g) { return _branch_run(module_path, seg, off, near_, span, _from_explorer(g, ds), probe); },
        explorer_seeds, off, py::str("little"), 2);
    py::list<Vector> out;
    for (const guided::Input& g : kept) out.append(_from_explorer(g, ds));
    return out;
}

// Path by path (verify/concolic): the original run with its symbolic shadow (emulate/shadow) -- its arguments, the words
// of its data segment and of the scratch segment pointers go to, and what an unmodelled Windows call returns are the
// inputs -- and inputs solved for to take each path its conditions allow.  Where the lifter is not there or CB_NO_CONCOLIC
// is set, the inputs are guessed as before (guided_vectors).
bool concolic_ready() {
    return lane_common::guided_on() && lane_common::concolic_on() && emulate::pcode::lifter() != nullptr;
}

std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names, const std::optional<int64_t>& ds) {
    // Input `v` with the named inputs (shadow.vars) set to `values`: an argument word, what an unmodelled call returns,
    // bytes of the data segment or of the scratch segment.
    Vector out = v;
    std::vector<int64_t> args(v.args.vec());
    std::map<int64_t, int64_t> mem;
    if (v.mem) {
        for (const auto& e : *v.mem) {
            const emulate::windows16::Where& w = std::get<0>(e);
            const py::bytes& data = std::get<1>(e);
            int64_t at;
            if (std::holds_alternative<int64_t>(w)) {
                at = std::get<int64_t>(w);
            } else {
                if (!ds) continue;
                at = *ds + std::get<1>(std::get<1>(w));
            }
            for (int64_t k = 0; k < data.size(); ++k) mem[at + k] = data[k];
        }
    }
    bool changed = false;
    for (const auto& kv : values.items()) {
        const py::str& name = std::get<0>(kv);
        int64_t value = std::get<1>(kv);
        auto got = names.get(name);
        std::string kind = got ? std::get<0>(*got).utf8() : std::string();
        int64_t where = got ? std::get<1>(*got) : 0;
        int64_t was = got ? std::get<3>(*got) : 0;
        if (kind == "arg" && where < static_cast<int64_t>(args.size())) {
            changed = changed || args[static_cast<size_t>(where)] != (value & 0xFFFF);
            args[static_cast<size_t>(where)] = value & 0xFFFF;
        } else if (kind == "ret") {
            changed = changed || (v.stub_ret ? *v.stub_ret : 0) != value;
            out.stub_ret = value & 0xFFFFFFFFLL;
        } else if (kind == "mem") {
            auto m = mem.find(where);
            changed = changed || (m != mem.end() ? m->second : was) != (value & 0xFF);
            mem[where] = value & 0xFF;
        }
    }
    if (!changed) return std::nullopt;
    out.args = py::list<int64_t>(args);
    std::vector<std::pair<int64_t, std::vector<int64_t>>> runs;
    for (const auto& kv : mem) {
        if (!runs.empty() && runs.back().first + static_cast<int64_t>(runs.back().second.size()) == kv.first) runs.back().second.push_back(kv.second);
        else runs.push_back({kv.first, {kv.second}});
    }
    MemInit entries;
    for (const auto& r : runs) {
        int64_t at = r.first;
        py::bytes data = py::bytes::from_list(r.second);
        if (ds && *ds <= at && at < *ds + 0x10000)
            entries.append(std::make_tuple(emulate::windows16::Where(std::make_tuple(py::str("D"), at - *ds)), data));
        else if (w16::SCRATCH_BASE <= at && at < w16::SCRATCH_BASE + 0x10000)
            entries.append(std::make_tuple(emulate::windows16::Where(at), data));
    }
    if (!entries.empty()) out.mem = entries; else out.mem.reset();
    return out;
}

namespace {

concolic::Observation to_observation(emulate::native_shadow::Shadow& sh) {
    concolic::Observation o;
    o.trail = sh.trail();
    for (const auto& e : sh.path()) {
        concolic::PathEntry p;
        p.site = e.site;
        p.taken = e.taken;
        p.cond = e.cond;
        p.kind = e.kind;
        o.path.append(p);
    }
    for (const auto& kv : sh.vars().items()) {
        const emulate::native_shadow::Var& v = std::get<1>(kv);
        int64_t where = std::holds_alternative<int64_t>(v.where) ? std::get<int64_t>(v.where) : 0;
        o.vars.set(std::get<0>(kv), std::make_tuple(v.kind, where, v.bits, v.value));
    }
    return o;
}

}  // namespace

Explored explore_vectors(const py::str& module_path, int64_t seg, int64_t off, bool near_, const std::tuple<int64_t, int64_t>& span,
                         const py::list<Vector>& seeds, const std::optional<py::dict<int64_t, concolic::Trail>>& known,
                         const std::optional<ProbeOptions>& probe) {
    // (inputs, exploration, [(input, record)]): inputs taking paths of the original no seed took, found path by path
    // (verify/concolic; lane_common.explored), how far the exploration got (its .paths, .complete), and every run it made,
    // input and record (a judge running the original as `probe` does need not run those again).  `known`: {seed index:
    // trail} of seeds already run.  Falls back to guided_vectors (exploration None) where it cannot.
    //
    // Where the paths found are not all there are and a jump of the function's own was still seen going one way only, what
    // is left of the guessing explorer's budget (lane_common.GUIDED_REST) goes to it, seeded with the runs made: a
    // condition over what the shadow does not follow -- the made-up answer of a callee not run, a Windows call modelled
    // from its arguments -- is no question for the solver, and guessing still reaches some of them.
    Explored result;
    if (!concolic_ready()) {
        result.inputs = guided_vectors(module_path, seg, off, near_, span, seeds, probe);
        return result;
    }
    int64_t auto_ = formats::ne::NEFile(module_path).h.autodata_seg;
    std::optional<int64_t> ds = auto_ ? std::optional<int64_t>(w16::segment_base(auto_)) : std::nullopt;
    int64_t base = w16::segment_base(seg);
    int64_t lo = base + std::get<0>(span), hi = base + std::get<1>(span);
    int64_t scratch = w16::SCRATCH_BASE;
    std::shared_ptr<emulate::pcode::Lifter> lifter = emulate::pcode::lifter();
    py::list<std::tuple<Vector, ProbeRecord>> made;   // (input, record) of every run
    emulate::shadow::Inputs inputs;
    if (ds) inputs.append(std::make_tuple(*ds, *ds + 0x10000));
    inputs.append(std::make_tuple(scratch, scratch + 0x10000));

    std::function<std::optional<concolic::Observation>(Vector&)> run = [&](Vector& v) -> std::optional<concolic::Observation> {
        std::shared_ptr<emulate::native_shadow::Shadow> sh = emulate::shadow::Shadow16(lifter, w16::selector_base, inputs, std::make_tuple(lo, hi));
        ProbeOptions o = run_options(probe, seg, off, near_, v, span);
        o.shadow = sh.get();
        ProbeRecord r;
        try {
            r = w16::probe(module_path, o);
        } catch (const py::ImportError&) {
            throw;
        } catch (const std::exception&) {
            return std::nullopt;
        }
        concolic::Observation ob = to_observation(*sh);
        for (const py::str& s : sh->lost()) ob.lost.add(s);
        bool cut = r.truncated || (r.exit && *r.exit == py::str("truncated"));
        if (cut) ob.lost.add(py::str("truncated"));
        ob.cut = cut;
        made.append(std::make_tuple(v, r));
        return ob;
    };
    std::function<std::optional<Vector>(const Vector&, const concolic::Values&, const concolic::Vars&)> derive =
        [&](const Vector& v, const concolic::Values& values, const concolic::Vars& names) { return _derive(v, values, names, ds); };
    auto got = std::make_shared<concolic::Exploration<Vector>>(lane_common::explored<Vector>(
        run, seeds, derive, [lo, hi](int64_t site) { return lo <= site && site < hi; },
        known ? *known : py::dict<int64_t, concolic::Trail>()));
    py::list<Vector> kept = got->kept.copy();
    std::map<int64_t, int64_t> one_way;
    for (const auto& edge : got->edges) one_way[std::get<0>(edge)] |= int64_t(1) << int(std::get<1>(edge));
    bool partial = false;
    for (const auto& kv : one_way) partial = partial || kv.second != 3;
    if (!got->complete && partial && lane_common::GUIDED_REST && !made.empty()) {
        guided::Run plain = [&](const guided::Input& g) -> std::optional<guided::Observation> {
            Vector v = _from_explorer(g, ds);
            ProbeRecord r;
            try {
                r = w16::probe(module_path, run_options(probe, seg, off, near_, v, span));
            } catch (const py::ImportError&) {
                throw;
            } catch (const std::exception&) {
                return std::nullopt;
            }
            made.append(std::make_tuple(v, r));
            return observed(r, v);
        };
        py::list<std::tuple<guided::Input, guided::Observation>> seen;
        for (const auto& e : made) seen.append(std::make_tuple(_to_explorer(std::get<0>(e), ds), observed(std::get<1>(e), std::get<0>(e))));
        py::list<guided::Input> extra = guided::guided_inputs(plain, py::list<guided::Input>(), lane_common::GUIDED_REST,
                                                              lane_common::MAX_GUIDED, off, py::str("little"), 2, seen);
        for (const guided::Input& g : extra) kept.append(_from_explorer(g, ds));
        got->guessed = extra.size();
        result.guessed = extra.size();
    }
    result.inputs = kept;
    result.exploration = got;
    result.made = made;
    return result;
}

// ---- the module ---------------------------------------------------------------------------------------------------
py::dict<int64_t, std::tuple<std::tuple<int64_t, int64_t>, int64_t>> _spans(const formats::ne::NEFile& nef,
                                                                           const py::dict<int64_t, core16::SegOff>& where) {
    // {address: ((start, end), instruction count)} -- each function runs to the next one in its segment (or the
    // segment's end).
    std::shared_ptr<const dsc::Decoder> md = dsc::decoder16();
    std::map<int64_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : nef.segments()) segs[s.idx] = s;
    std::vector<int64_t> seg_order;
    std::map<int64_t, std::vector<std::pair<int64_t, int64_t>>> by_seg;
    for (const auto& kv : where.items()) {
        int64_t sg = std::get<0>(std::get<1>(kv));
        if (!by_seg.count(sg)) seg_order.push_back(sg);
        by_seg[sg].emplace_back(std::get<1>(std::get<1>(kv)), std::get<0>(kv));
    }
    py::dict<int64_t, std::tuple<std::tuple<int64_t, int64_t>, int64_t>> out;
    for (int64_t sg : seg_order) {
        auto& lst = by_seg[sg];
        std::sort(lst.begin(), lst.end());
        const formats::ne::Segment& s = segs.at(sg);
        py::bytes code = nef.data.slice(s.file_off, s.file_off + s.length);
        for (size_t i = 0; i < lst.size(); ++i) {
            int64_t off = lst[i].first, va = lst[i].second;
            int64_t end = i + 1 < lst.size() ? lst[i + 1].first : code.size();
            int64_t n = md->disasm(code.slice(off, end), off).size();
            out.set(va, std::make_tuple(std::make_tuple(off, end), std::max<int64_t>(n, 1)));
        }
    }
    return out;
}

std::tuple<py::str, py::str> profile(const std::optional<py::str>& module_path) {
    // (digest, settings) of everything other than the source that a verdict depends on.
    (void)module_path;
    bool ready = concolic_ready();
    std::vector<py::str> options;
    for (const py::str& f : build::functions::CFLAGS16) options.push_back(py::repr(f));   // P.compiler.options()
    std::vector<py::str> vs;
    for (const Vector& v : vectors()) vs.push_back(py::str("{'args': ") + py::repr(v.args) + py::str("}"));
    py::str stamp = py::str("''");
    if (ready) {
        auto s = emulate::pcode::stamp();
        stamp = rtuple({py::repr(std::get<0>(s)), py::repr(std::get<1>(s))});
    }
    py::str settings = rtuple(
        {py::repr(py::str("ne")), py::repr(REV), py::repr(compare::REV), rlist(options), rlist(vs),
         rtuple({py::repr(py::str("guided")), py::repr(lane_common::GUIDED_BUDGET), py::repr(lane_common::MAX_GUIDED)}),
         rtuple({py::repr(py::str("paths")), rbool(ready), py::repr(lane_common::PATH_BUDGET), py::repr(lane_common::MAX_PATHS), stamp})});
    return {py::hashlib::sha1(settings.encode("utf-8")).hexdigest().slice(0, 12), settings};
}

py::str key(int64_t seg, int64_t off) { return py::percent("%d:%04x", seg, off); }

Prepared prepare(const py::str& module_path, const py::str& work) {
    return prepared_from(decompile::c_text::compilable16(module_path, work));
}

Prepared prepared_from(const decompile::c_text::Module16& made) {
    Prepared m;
    static_cast<decompile::c_text::Module16&>(m) = made;
    py::dict<int64_t, py::str> by_va = m.by_va();
    for (const auto& kv : by_va.items()) m.key.set(std::get<1>(kv), key(std::get<0>(*m.where.at(std::get<0>(kv))), std::get<1>(*m.where.at(std::get<0>(kv)))));
    std::vector<py::str> all(m.key.values());
    std::sort(all.begin(), all.end(), code_less);
    m.all_keys = py::list<py::str>(all);
    py::dict<py::str, py::str> texts = m.texts();
    py::str joined;
    for (const py::str& n : m.order()) joined = joined + texts.at(n);
    m.sig = py::hashlib::sha1((m.header() + joined).encode("utf-8", "replace")).hexdigest();
    return m;
}

namespace {

py::dict<int64_t, py::bytes> code_images_of(const Prepared& m, const formats::ne::NEFile& nef) {
    std::optional<py::str> env = py::os::environ_::get(py::str("CB_CODE_DATA"));
    py::set<int64_t> want;
    py::dict<py::str, py::str> texts = m.texts();
    if (env && *env == py::str("0")) {
        for (const auto& kv : m.by_va().items()) {
            const std::optional<core16::SegOff>& w = m.where.at(std::get<0>(kv));
            auto t = texts.get(std::get<1>(kv));
            if (t && t->contains(py::str("codebreaker_cs()"))) want.add(std::get<0>(*w));
        }
    } else {
        for (const auto& kv : m.code_reads.items())
            if (!std::get<1>(kv).left.empty()) want.add(std::get<0>(kv));
    }
    std::map<int64_t, formats::ne::Segment> segs;
    for (const formats::ne::Segment& s : nef.segments()) segs[s.idx] = s;
    std::vector<int64_t> sorted(want.begin(), want.end());
    std::sort(sorted.begin(), sorted.end());
    py::dict<int64_t, py::bytes> out;
    for (int64_t sg : sorted)
        if (segs.count(sg) && segs[sg].file_off) out.set(sg, nef.data.slice(segs[sg].file_off, segs[sg].file_off + segs[sg].length));
    return out;
}

}  // namespace

py::dict<int64_t, py::bytes> _code_images(const Prepared& m, const formats::ne::NEFile& nef) { return code_images_of(m, nef); }

namespace {
constexpr int64_t INT_REGISTERS = 20;   // sizeof(struct codebreaker_registers): eight words, CF, ZF, a word
}

py::list<std::tuple<int64_t, int64_t>> _own_data(const py::str& work) {
    // The offsets in the rebuild's data segment of what the build itself keeps there and the original does not: the
    // registers an interrupt helper hands its routine (codebreaker_int, c_text.interrupt_routines).  Writing them is the
    // build's bookkeeping, not the function's effect.
    py::str text;
    try {
        py::file f = py::open(py::os::path::join(work, py::str("m.map")), py::str("r"), std::optional<py::str>(py::str("utf-8")),
                              std::optional<py::str>(py::str("replace")));
        text = f.read_text();
    } catch (const py::OSError&) {
        return py::list<std::tuple<int64_t, int64_t>>();
    }
    std::optional<py::re::Match> m = py::re::search(py::str("(?m)^[0-9A-Fa-f]{4}:([0-9A-Fa-f]{4,8})[+*]?\\s+_codebreaker_int\\s*$"), text);
    py::list<std::tuple<int64_t, int64_t>> out;
    if (m) {
        int64_t at = py::int_parse(m->group(1), 16);
        out.append(std::make_tuple(at, at + INT_REGISTERS));
    }
    return out;
}

Built build(const Prepared& m, const py::str& module_path, const py::str& work) {
    // -> (context for judge, {name: item}, {name: why it is not in the build})
    formats::ne::NEFile nef(module_path);
    py::dict<int64_t, py::str> by_va = m.by_va();
    py::dict<py::str, int64_t> code_segment;
    for (const auto& kv : by_va.items()) code_segment.set(std::get<1>(kv), std::get<0>(*m.where.at(std::get<0>(kv))));
    py::set<int64_t> interrupts;
    for (const auto& i : m.interrupts) interrupts.add(i.vector);
    py::dict<py::str, py::str> texts = m.texts();
    build::functions::Built16 b = build::functions::build16(m.header(), texts, m.order(), m.imports, work, m.segments, std::nullopt,
                                                            code_segment, interrupts, m.code_data, _code_images(m, nef));
    int64_t auto_ = nef.h.autodata_seg;
    py::bytes data_image;
    if (auto_) {
        formats::ne::Segment s = nef.segments()[auto_ - 1];
        data_image = s.file_off ? nef.data.slice(s.file_off, s.file_off + s.length) : py::bytes();
    }
    py::dict<int64_t, core16::SegOff> where;
    for (const auto& kv : by_va.items()) where.set(std::get<0>(kv), *m.where.at(std::get<0>(kv)));
    auto spans = _spans(nef, where);
    Built out;
    py::dict<py::str, py::str> dropped = b.dropped.copy();
    py::dict<py::str, JudgeItem>& items = out.items;
    for (const auto& kv : by_va.items()) {
        int64_t va = std::get<0>(kv);
        const py::str& fn = std::get<1>(kv);
        if (dropped.contains(fn)) continue;
        if (!b.where.contains(fn)) {
            dropped.set(fn, py::str("not in the build"));
            continue;
        }
        core16::SegOff at = *m.where.at(va);
        auto sp = spans.at(va);
        JudgeItem it;
        it.key = m.key.at(fn);
        it.seg = std::get<0>(at);
        it.off = std::get<1>(at);
        it.near_ = m.near.contains(fn);
        it.wide = m.novalue.contains(fn) ? std::optional<bool>() : std::optional<bool>(m.wide.contains(fn));
        it.span = std::get<0>(sp);
        it.insns = std::get<1>(sp);
        it.bseg = std::get<0>(b.where.at(fn));
        it.boff = std::get<1>(b.where.at(fn));
        auto t = texts.get(fn);
        it.frame = frame_slots(t ? *t : py::str(""), fn);   // (a caller-frame helper: _judge)
        items.set(fn, it);
    }
    out.dropped = dropped;
    py::dict<int64_t, int64_t> order;   // rebuilt code segment -> the original's
    for (const JudgeItem& it : items.values()) {
        if (!order.contains(it.bseg)) order.set(it.bseg, py::int_parse(py::split(it.key, py::str(":"))[0]));
    }
    out.context.module_path = module_path;
    out.context.dll = b.dll ? *b.dll : py::str("");
    out.context.data_image = data_image;
    out.context.own = _own_data(work);
    out.context.order = order;
    return out;
}

// ---- the lane as a reconstruct::Kind -------------------------------------------------------------------------------
namespace {

class Win16Kind : public reconstruct::Kind {
  public:
    py::str KIND() const override { return win16::KIND; }
    bool PER_FUNCTION() const override { return true; }
    std::tuple<py::str, py::str> profile(const py::str& module_path) const override {
        return win16::profile(std::optional<py::str>(module_path));
    }
    reconstruct::Prepared prepare(const py::str& module_path, const py::str& work) const override {
        return win16::as_prepared(std::make_shared<win16::Prepared>(win16::prepare(module_path, work)));
    }
    reconstruct::Built build(const reconstruct::Prepared& m, const py::str& module_path, const py::str& work) const override {
        std::shared_ptr<win16::Prepared> mine = std::static_pointer_cast<win16::Prepared>(m.lane);
        win16::Built b = win16::build(*mine, module_path, work);
        reconstruct::Built out;
        out.context = std::make_shared<JudgeContext>(b.context);
        for (const auto& kv : b.items.items()) out.items.set(std::get<0>(kv), std::make_shared<JudgeItem>(std::get<1>(kv)));
        out.dropped = b.dropped;
        return out;
    }
    reconstruct::Verdict judge(const reconstruct::Context& context, const reconstruct::Item& item) const override {
        return win16::_judge(*std::static_pointer_cast<JudgeContext>(context), *std::static_pointer_cast<JudgeItem>(item));
    }
};

}  // namespace

reconstruct::Prepared as_prepared(const std::shared_ptr<Prepared>& m) {
    reconstruct::Prepared out;
    out.header = m->header();
    out.texts = m->texts();
    out.order = m->order();
    out.key = m->key;
    out.all_keys = m->all_keys;
    out.sig = m->sig;
    out.lane = m;
    return out;
}

const std::shared_ptr<reconstruct::Kind>& kind() {
    static std::shared_ptr<reconstruct::Kind> k = std::make_shared<Win16Kind>();
    return k;
}

void register_lane() { reconstruct::register_kind(KIND, kind()); }

}  // namespace codebreaker::verify::lanes::win16
