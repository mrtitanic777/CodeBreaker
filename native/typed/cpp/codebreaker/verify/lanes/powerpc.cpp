// codebreaker/verify/lanes/powerpc.cpp -- typed C++ port of codebreaker.verify.lanes.powerpc (see powerpc.hpp).

#include "codebreaker/verify/lanes/powerpc.hpp"

#include <algorithm>
#include <map>
#include <set>

#include "codebreaker/base/corpus.hpp"
#include "codebreaker/translate/l2.hpp"
#include "codebreaker/build/functions.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/shadow.hpp"
#include "codebreaker/jobs/workers.hpp"
#include "py_os.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::verify::lanes::powerpc {

namespace {

using py::str;
namespace epp = codebreaker::emulate::powerpc;
namespace V = codebreaker::verify::verdicts;
namespace c_text = codebreaker::decompile::c_text;

str S(const char* s) { return str(s); }

constexpr uint64_t M64 = 0xFFFFFFFFFFFFFFFFULL;

uint64_t U(int64_t v) { return static_cast<uint64_t>(v); }

// the bits of a 64-bit register as the text Python writes the int (0 .. 2**64-1)
str ustr(int64_t v) { return str(std::to_string(U(v))); }

int64_t fill_code(const std::optional<str>& fill) {
    if (!fill) return 0;
    if (*fill == S("pointers")) return 1;
    if (*fill == S("counts")) return 2;
    if (*fill == S("text")) return 3;
    return 0;
}
std::optional<str> fill_of(int64_t code) {
    if (code == 1) return S("pointers");
    if (code == 2) return S("counts");
    if (code == 3) return S("text");
    return std::nullopt;
}

epp::RunOptions options_of(const Vector& v, const Externs* externs, const std::optional<Span>& span, const Pages* shared, bool branches,
                           const std::optional<Span>& absent, epp::Shadow* shadow = nullptr) {
    epp::RunOptions o;
    if (externs) o.externs = *externs;
    o.span = span;
    o.stub_ret = v.stub_ret.value_or(0);
    if (shared) o.shared = *shared;
    o.fill = v.fill;
    o.branches = branches;
    o.absent = absent;
    o.shadow = shadow;
    return o;
}

py::list<epp::Region> regions_of(const Vector& v) {
    py::list<epp::Region> out;
    if (v.mem_init)
        for (const auto& r : *v.mem_init) out.append(r);
    return out;
}

py::list<uint64_t> args_of(const Vector& v) {
    py::list<uint64_t> out;
    for (int64_t a : v.args) out.append(U(a));
    return out;
}

Vector vec(const py::list<int64_t>& args, const std::optional<int64_t>& stub_ret = std::nullopt, const std::optional<str>& fill = std::nullopt) {
    Vector v;
    v.args = args;
    v.stub_ret = stub_ret;
    v.fill = fill;
    return v;
}

guided::Compare compare_of(const epp::Compare& c) {
    guided::Compare g;
    g.mn = S("cmp");
    g.size = c.size;
    g.a = static_cast<int64_t>(c.a);
    g.b = static_cast<int64_t>(c.b);
    g.at_a = c.a_from;
    g.at_b = c.b_from;
    g.has_at = true;
    return g;
}

// run(v)'s observation for the guessing explorer: {"branches", "reach", "args_at": None, "stack"}
guided::Observation observation_of(const epp::Out& out, const emulate::run::Run& r) {
    guided::Observation o;
    if (out.branches)
        for (const auto& b : *out.branches) {
            std::optional<guided::Compare> c;
            if (std::get<2>(b)) c = compare_of(*std::get<2>(b));
            o.branches.append(std::make_tuple(std::get<0>(b), std::get<1>(b), c));
        }
    for (int64_t p : r.path) o.reach.add(p);
    o.args_at = std::nullopt;
    o.stack = std::make_tuple(epp::STACK_LOW, epp::STACK_TOP);
    return o;
}

// emulate/shadow.py's ShadowPPC as the interpreter's own shadow interface (stepped before each instruction, told of each call out)
class ShadowAdapter : public epp::Shadow {
  public:
    explicit ShadowAdapter(emulate::shadow::ShadowPPC& sh) : sh_(sh) {}
    void step(epp::Machine& m, int64_t pc) override { sh_.step(m, pc); }
    void after_call(epp::Machine& m, bool known, uint64_t ret) override { sh_.after_call(m, known, static_cast<int64_t>(ret)); }

  private:
    emulate::shadow::ShadowPPC& sh_;
};

std::map<std::tuple<int64_t, int64_t>, py::list<Vector>>& directed_cache() {
    static std::map<std::tuple<int64_t, int64_t>, py::list<Vector>> c;
    return c;
}

}  // namespace

const str KIND = S("powerpc");

int64_t ARGS() { return 8; }

py::list<Vector> vectors() {
    const int64_t a = epp::ARGS_BASE;
    const int64_t n = ARGS();
    const int64_t ones = -1;   // 0xFFFFFFFFFFFFFFFF
    py::list<Vector> out;
    out.append(vec(py::list<int64_t>{0, 0, 0, 0, 0, 0, 0, 0}));
    out.append(vec(lane_common::counting(n)));
    out.append(vec(lane_common::counting(n, a, 0x1000)));
    out.append(vec(lane_common::counting(n, a, 0x1000), 1));
    out.append(vec(lane_common::counting(n, 0x100, 0x100), ones));
    out.append(vec(lane_common::alternating(n, lane_common::Pattern([a](int64_t i) { return a + 0x800 * i; }),
                                            lane_common::Pattern([](int64_t i) { return 0x20 * (i + 1); }))));
    out.append(vec(lane_common::alternating(n, lane_common::Pattern([](int64_t i) { return 0x20 * (i + 1); }),
                                            lane_common::Pattern([a](int64_t i) { return a + 0x800 * i; })), 1));
    out.append(vec(lane_common::descending(n, ones)));
    out.append(vec(py::list<int64_t>{a, 3, a + 0x4000, 0x40, a + 0x8000, 1, 0, 2}, 0x103));
    out.append(vec(lane_common::counting(n, a, 0x1000), std::nullopt, S("pointers")));
    out.append(vec(lane_common::counting(n, a, 0x1000), 1, S("counts")));
    out.append(vec(lane_common::counting(n, a, 0x1000), std::nullopt, S("text")));
    out.append(vec(py::list<int64_t>{a + 0x40, 5, a + 0x2000, 2, 1, a + 0x3000, 0, 9}, std::nullopt, S("counts")));
    out.append(vec(py::list<int64_t>{7, a + 0x100, 0x30, a + 0x5000, a + 0x200, 4, 3, 1}, 2, S("pointers")));
    return out;
}

py::list<Vector> directed(const formats::powerpc_image::Image& image, const Span& span, int64_t most) {
    auto& cache = directed_cache();
    auto it = cache.find(span);
    if (it != cache.end()) return it->second;
    std::vector<int64_t> seen;
    for (int64_t va = std::get<0>(span); va < std::get<1>(span); va += 4) {
        const int64_t w = image.word(va).value_or(0);
        if ((w >> 26) == 10 || (w >> 26) == 11) {   // cmpli, cmpi
            int64_t c = w & 0xFFFF;
            if ((w >> 26) == 11 && (c & 0x8000)) c = static_cast<int64_t>(static_cast<uint64_t>(c) | 0xFFFFFFFFFFFF0000ULL);
            if (std::find(seen.begin(), seen.end(), c) == seen.end() && c != 0 && c != 1) seen.push_back(c);
        }
    }
    const int64_t a = epp::ARGS_BASE, n = ARGS();
    py::list<Vector> out;
    for (size_t k = 0; k < seen.size() && static_cast<int64_t>(k) < most; ++k) {
        const int64_t c = seen[k];
        py::list<int64_t> same;
        for (int64_t i = 0; i < n; ++i) same.append(c);
        out.append(vec(same, c));
        out.append(vec(lane_common::alternating(n, lane_common::Pattern([a](int64_t i) { return a + 0x1000 * i; }), lane_common::Pattern(c)), c, S("counts")));
    }
    cache[span] = out;
    return out;
}

guided::Input to_guided(const Vector& v) {
    guided::Input g;
    g.args = v.args;
    g.mem_init = v.mem_init;
    g.stub_ret = v.stub_ret;
    if (v.fill) g.extra.set(S("fill"), guided::Extra(fill_code(v.fill)));
    return g;
}

Vector from_guided(const guided::Input& g) {
    Vector v;
    v.args = g.args;
    v.mem_init = g.mem_init;
    v.stub_ret = g.stub_ret;
    if (g.extra.contains(S("fill"))) v.fill = fill_of(std::get<int64_t>(g.extra.at(S("fill"))));
    return v;
}

py::list<Vector> guided_vectors(const Pages& shared, int64_t va, const Span& span, const Externs& externs, const py::list<Vector>& seeds,
                                const std::optional<Span>& absent) {
    guided::Run run = [&](const guided::Input& in) -> std::optional<guided::Observation> {
        const Vector v = from_guided(in);
        try {
            emulate::run::Run r = epp::recorded(regions_of(v), va, args_of(v), options_of(v, &externs, span, &shared, true, absent));
            const std::shared_ptr<epp::Out> out = std::static_pointer_cast<epp::Out>(r.raw);
            return observation_of(*out, r);
        } catch (const py::Exception&) {
            return std::nullopt;
        }
    };
    py::list<guided::Input> gseeds;
    for (const Vector& v : seeds) gseeds.append(to_guided(v));
    py::list<Vector> out;
    for (const guided::Input& g : lane_common::guided(run, gseeds, va & 0xFFFF, S("big"), 8)) out.append(from_guided(g));
    return out;
}

py::list<Vector> directed_and_guided(const str& module_path, const Pages& shared, int64_t va, const Span& span, const Externs& externs,
                                     const std::optional<Span>& absent, Found* found) {
    py::list<Vector> d = directed(*formats::powerpc_image::read(module_path), span);
    py::list<Vector> seeds = vectors();
    for (const Vector& v : d) seeds.append(v);
    py::list<Vector> out;
    for (const Vector& v : d) out.append(v);
    if (concolic_ready()) {
        auto [kept, got] = explore_vectors(shared, va, span, externs, seeds, absent, _writable(module_path));
        if (found) found->got = got;
        for (const Vector& v : kept) out.append(v);
        return out;
    }
    for (const Vector& v : guided_vectors(shared, va, span, externs, seeds, absent)) out.append(v);
    return out;
}

std::optional<str> _mode() { return emulate::shadow::mode(S("PowerPC:BE:64:Xenon:homed")); }

bool concolic_ready() {
    if (!(lane_common::guided_on() && lane_common::concolic_on())) return false;
    const std::optional<str> mode = _mode();
    return mode && emulate::pcode::lifter(*mode) != nullptr;
}

std::optional<Vector> _derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names) {
    Vector out = v;
    py::list<int64_t> args = v.args.copy();
    std::map<int64_t, int64_t> mem;
    if (v.mem_init)
        for (const auto& r : *v.mem_init) {
            const py::bytes& data = std::get<1>(r);
            for (int64_t k = 0; k < data.size(); ++k) mem[std::get<0>(r) + k] = data[k];
        }
    bool changed = false;
    for (const auto& kv : values.items()) {
        const str& name = std::get<0>(kv);
        const int64_t value = std::get<1>(kv);
        if (!names.contains(name)) continue;   // (names.get(name, (None, None, 0, None)): no kind matches)
        const auto& var = names.at(name);
        const str& kind = std::get<0>(var);
        const int64_t where = std::get<1>(var), was = std::get<3>(var);
        if (kind == S("arg") && where < static_cast<int64_t>(args.size())) {
            changed |= U(args[where]) != U(value);
            args[where] = value;
        } else if (kind == S("ret")) {
            changed |= U(v.stub_ret.value_or(0)) != U(value);
            out.stub_ret = value;
        } else if (kind == S("mem")) {
            auto it = mem.find(where);
            changed |= (it != mem.end() ? it->second : was) != (value & 0xFF);
            mem[where] = value & 0xFF;
        }
    }
    if (!changed) return std::nullopt;
    out.args = args;
    if (!mem.empty()) {
        const py::bytes under = v.fill ? epp::filled(*v.fill) : py::bytes();
        std::map<int64_t, std::string> pages;
        std::vector<std::pair<int64_t, std::string>> runs;
        for (const auto& kv : mem) {
            const int64_t at = kv.first;
            const int64_t lo = (at & ~(epp::PAGE - 1)) - epp::ARGS_BASE;
            if (0 <= lo && lo < under.size()) {   // (in the fill: the whole page)
                const int64_t base = at & ~(epp::PAGE - 1);
                if (!pages.count(base)) pages[base] = under.slice(lo, lo + epp::PAGE).raw();
                pages[base][static_cast<size_t>(at - base)] = static_cast<char>(kv.second);
            } else if (!runs.empty() && runs.back().first + static_cast<int64_t>(runs.back().second.size()) == at) {
                runs.back().second.push_back(static_cast<char>(kv.second));
            } else {
                runs.emplace_back(at, std::string(1, static_cast<char>(kv.second)));
            }
        }
        std::vector<std::pair<int64_t, std::string>> all;
        for (const auto& p : pages) all.emplace_back(p.first, p.second);
        for (const auto& r : runs) all.push_back(r);
        std::sort(all.begin(), all.end());
        py::list<std::tuple<int64_t, py::bytes>> mi;
        for (const auto& p : all) mi.append(std::make_tuple(p.first, py::bytes(p.second)));
        out.mem_init = mi;
    }
    return out;
}

py::list<Span> _writable(const str& module_path) {
    py::list<Span> out;
    try {
        for (const auto& s : formats::powerpc_image::read(module_path)->sections) {
            const int64_t chars = std::get<3>(s);
            if ((chars & 0x80000000LL) && !(chars & 0x20000000LL)) out.append(std::make_tuple(std::get<1>(s), std::get<1>(s) + std::get<2>(s)));
        }
    } catch (const py::Exception&) {
        return py::list<Span>();
    }
    return out;
}

std::tuple<py::list<Vector>, std::shared_ptr<concolic::Exploration<Vector>>> explore_vectors(
    const Pages& shared, int64_t va, const Span& span, const Externs& externs, const py::list<Vector>& seeds, const std::optional<Span>& absent,
    const py::list<Span>& data) {
    const std::shared_ptr<emulate::pcode::Lifter> lifter = emulate::pcode::lifter(*_mode());
    const int64_t lo = std::get<0>(span), hi = std::get<1>(span);
    const Span region(epp::ARGS_BASE, epp::ARGS_BASE + 0x10000);

    auto inputs = [&](int64_t b) {
        if (std::get<0>(region) <= b && b < std::get<1>(region)) return true;
        for (const Span& d : data)
            if (std::get<0>(d) <= b && b < std::get<1>(d)) return true;
        return false;
    };

    // observe(v, **more): (the run, its observation) or (None, None)
    auto observe = [&](const Vector& v, epp::Shadow* shadow) -> std::optional<std::tuple<emulate::run::Run, guided::Observation>> {
        try {
            emulate::run::Run r = epp::recorded(regions_of(v), va, args_of(v), options_of(v, &externs, span, &shared, true, absent, shadow));
            const std::shared_ptr<epp::Out> out = std::static_pointer_cast<epp::Out>(r.raw);
            return std::make_tuple(r, observation_of(*out, r));
        } catch (const py::Exception&) {
            return std::nullopt;
        }
    };

    py::list<std::tuple<guided::Input, guided::Observation>> seen;
    py::dict<int64_t, concolic::Trail> known;
    int64_t i = 0;
    for (const Vector& v : seeds) {
        auto o = observe(v, nullptr);
        if (o) {
            seen.append(std::make_tuple(to_guided(v), std::get<1>(*o)));
            concolic::Trail trail;
            for (const auto& b : std::get<1>(*o).branches) trail.append(concolic::Step{std::get<0>(b), std::get<1>(b)});
            known.set(i, trail);
        }
        ++i;
    }

    std::function<std::optional<concolic::Observation>(Vector&)> run = [&](Vector& v) -> std::optional<concolic::Observation> {
        emulate::shadow::ShadowPPC sh(lifter, inputs, std::optional<Span>(span));
        py::list<emulate::native_shadow::Place> places;
        for (int64_t k = 0; k < static_cast<int64_t>(v.args.size()) && k < 8; ++k) {
            emulate::native_shadow::Place p;
            p.where = py::percent(S("r%d"), 3 + k);
            p.value = v.args[k];
            p.k = std::variant<int64_t, str>(k);
            places.append(p);
        }
        sh.enter(places, 64);
        ShadowAdapter adapter(sh);
        auto r = observe(v, &adapter);
        if (!r) return std::nullopt;
        seen.append(std::make_tuple(to_guided(v), std::get<1>(*r)));
        concolic::Observation ob;
        ob.trail = sh.trail;
        for (const auto& e : sh.path) {
            concolic::PathEntry p;
            p.site = e.site;
            p.taken = e.taken;
            p.cond = e.cond;
            p.kind = e.kind;
            ob.path.append(p);
        }
        for (const auto& kv : sh.vars.items()) {
            const emulate::native_shadow::Var& var = std::get<1>(kv);
            const int64_t where = std::holds_alternative<int64_t>(var.where) ? std::get<int64_t>(var.where) : 0;
            ob.vars.set(std::get<0>(kv), std::make_tuple(var.kind, where, var.bits, var.value));
        }
        for (const str& s : sh.lost) ob.lost.add(s);
        const bool cut = std::get<0>(*r).truncated;
        if (cut) ob.lost.add(S("truncated"));
        ob.cut = cut;
        return ob;
    };
    std::function<std::optional<Vector>(const Vector&, const concolic::Values&, const concolic::Vars&)> derive =
        [](const Vector& v, const concolic::Values& values, const concolic::Vars& names) { return _derive(v, values, names); };
    auto got = std::make_shared<concolic::Exploration<Vector>>(lane_common::explored<Vector>(
        run, seeds, derive, [lo, hi](int64_t site) { return lo <= site && site < hi; }, known, SOLVER_STEPS));
    py::list<Vector> kept = got->kept.copy();
    std::map<int64_t, int64_t> one_way;
    for (const auto& e : got->edges) one_way[std::get<0>(e)] |= (1 << (std::get<1>(e) ? 1 : 0));
    bool any_single = false;
    for (const auto& kv : one_way)
        if (kv.second != 3) any_single = true;
    if (!got->complete && any_single && lane_common::GUIDED_REST && !seen.empty()) {
        guided::Run observe_only = [&](const guided::Input& in) -> std::optional<guided::Observation> {
            auto o = observe(from_guided(in), nullptr);
            if (!o) return std::nullopt;
            return std::get<1>(*o);
        };
        py::list<guided::Input> extra = guided::guided_inputs(observe_only, py::list<guided::Input>(), lane_common::GUIDED_REST, lane_common::MAX_GUIDED,
                                                              va & 0xFFFF, S("big"), 8, seen);
        for (const guided::Input& g : extra) kept.append(from_guided(g));
        got->guessed = static_cast<int64_t>(extra.size());
    }
    return {kept, got};
}

str _argument(epp::Machine& machine, int64_t v, const py::list<Span>& images) {
    const int64_t x = v & M32;
    if (epp::STACK_LOW <= x && x < epp::STACK_TOP) return S("stack");
    for (const Span& im : images)
        if (std::get<0>(im) <= x && x < std::get<1>(im)) {
            std::string raw;
            for (int64_t i = 0; i < 12; ++i) raw.push_back(static_cast<char>(machine.read(x + i, 1)));
            return S("data:") + py::bytes(raw).hex();
        }
    return lane_common::argument(x);
}

compare::Fingerprint fingerprint(const Pages& shared, int64_t entry, const Vector& v, const Externs& externs, const py::dict<py::str, int64_t>& argc,
                                 const py::list<Span>& images, const std::optional<Span>& span, bool novalue) {
    emulate::run::Run r(std::nullopt);
    try {
        r = epp::recorded(regions_of(v), entry, args_of(v), options_of(v, &externs, span, &shared, false, images[0]));
    } catch (const py::Exception& e) {
        compare::Fingerprint fp;
        fp.err = str(e.type());
        return fp;
    }
    const str ex = r.ended();
    const std::shared_ptr<epp::Out> out = std::static_pointer_cast<epp::Out>(r.raw);
    epp::Machine& m = *out->machine;
    py::list<lane_common::Called> called;
    for (const auto& c : r.calls) {
        lane_common::Called k;
        k.name = c.name;
        if (c.name && argc.contains(*c.name) && c.args) {
            py::list<int64_t> a;
            const int64_t n = argc.at(*c.name);
            for (int64_t i = 0; i < n && i < static_cast<int64_t>(c.args->size()); ++i) a.append((*c.args)[i]);
            k.args = a;
        }
        called.append(k);
    }
    py::list<str> deps = lane_common::calls(called, [&](int64_t x) { return _argument(m, x, images); });
    if (r.truncated && static_cast<int64_t>(r.calls.size()) >= epp::MAX_CALLS) deps.append(S("..."));
    const Span rebuilt = images[static_cast<int64_t>(images.size()) - 1];   // the rebuilt image, when there is one
    const int64_t lo = span ? std::get<0>(*span) : 0;
    std::optional<int64_t> ret;
    if (!(novalue || ex != S("ret"))) ret = std::get<int64_t>(r.results.at(S("r3"))) & M32;
    auto place = [&](int64_t a) -> std::optional<lane_common::Where> {
        if (images.size() > 1 && std::get<0>(rebuilt) <= a && a < std::get<1>(rebuilt)) return std::nullopt;
        return lane_common::Where(a);
    };
    py::list<int64_t> cov;
    {
        std::vector<int64_t> c;
        for (int64_t p : r.path) c.push_back(p - lo);
        std::sort(c.begin(), c.end());
        for (int64_t p : c) cov.append(p);
    }
    return lane_common::fingerprint(r, deps, ret, lane_common::left(r, place), r.kept(), cov);
}

namespace {

struct Shared {
    Pages alone, both;
    py::list<Span> images;
};

std::map<std::tuple<str, str>, Shared>& shared_cache() {
    static std::map<std::tuple<str, str>, Shared> c;
    return c;
}

// (pages of the original alone, pages of original + rebuilt, [(lo, hi)] of each image)
const Shared& memory_of(const str& module_path, const str& dll) {
    auto& cache = shared_cache();
    const auto key = std::make_tuple(module_path, dll);
    if (!cache.count(key)) {
        py::list<epp::Region> a, b, ab;
        for (const auto& r : formats::powerpc_image::regions(module_path)) {
            a.append(r);
            ab.append(r);
        }
        for (const auto& r : formats::powerpc_image::regions(dll)) {
            b.append(r);
            ab.append(r);
        }
        auto extent = [](const py::list<epp::Region>& regs) {
            int64_t lo = std::get<0>(regs[0]), hi = 0;
            for (const auto& r : regs) {
                lo = std::min(lo, std::get<0>(r));
                hi = std::max(hi, std::get<0>(r) + std::get<1>(r).size());
            }
            return Span(lo, hi);
        };
        cache.clear();   // (the one pair in use)
        Shared s;
        s.alone = epp::pages(a);
        s.both = epp::pages(ab);
        s.images.append(extent(a));
        s.images.append(extent(b));
        cache[key] = s;
    }
    return cache.at(key);
}

// repr() of the cache key's items: what an input is, sorted by key
str vector_items_repr(const Vector& v) {
    py::list<str> items;
    py::list<str> a;
    for (int64_t x : v.args) a.append(ustr(x));
    items.append(S("('args', [") + py::join(S(", "), a) + S("])"));
    if (v.fill) items.append(S("('fill', ") + py::repr(*v.fill) + S(")"));
    if (v.mem_init) items.append(S("('mem_init', ") + py::repr(*v.mem_init) + S(")"));
    if (v.stub_ret) items.append(S("('stub_ret', ") + ustr(*v.stub_ret) + S(")"));
    return S("[") + py::join(S(", "), items) + S("]");
}

str externs_repr(const Externs& d) {
    std::vector<std::pair<int64_t, str>> v;
    for (const auto& kv : d.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(v.begin(), v.end(), [](const auto& x, const auto& y) { return x.first < y.first || (x.first == y.first && x.second.u32() < y.second.u32()); });
    py::list<str> items;
    for (const auto& p : v) items.append(S("(") + py::to_str(p.first) + S(", ") + py::repr(p.second) + S(")"));
    return S("[") + py::join(S(", "), items) + S("]");
}

str argc_repr(const py::dict<str, int64_t>& d) {
    std::vector<std::pair<str, int64_t>> v;
    for (const auto& kv : d.items()) v.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(v.begin(), v.end(), [](const auto& x, const auto& y) { return x.first.u32() < y.first.u32() || (x.first == y.first && x.second < y.second); });
    py::list<str> items;
    for (const auto& p : v) items.append(S("(") + py::repr(p.first) + S(", ") + py::to_str(p.second) + S(")"));
    return S("[") + py::join(S(", "), items) + S("]");
}

}  // namespace

std::tuple<Pages, Pages, py::list<Span>> _memory(const str& module_path, const str& dll) {
    const Shared& s = memory_of(module_path, dll);
    return {s.alone, s.both, s.images};
}

reconstruct::Verdict judge(const Context& context, const Item& item) {
    const std::optional<str> sha = base::corpus::sha(context.module_path);
    const int64_t va = item.va;
    const Span span = item.span;
    const bool novalue = item.novalue;
    const int64_t rebuilt_va = item.rebuilt_va;

    auto compare_one = [&](compare::Info& info) -> std::tuple<str, str, py::list<str>> {
        const Shared& mem = memory_of(context.module_path, context.dll);
        Externs merged = context.ext_original.copy();
        for (const auto& kv : context.ext_rebuilt.items()) merged.set(std::get<0>(kv), std::get<1>(kv));
        const py::list<Span> first_image{mem.images[0]};

        compare::FingerprintOf<Vector> original = [&](const Vector& v) -> compare::Fingerprint {
            // the original's run depends on the original, and on what the build says of the calls out of it (their names, how many
            // arguments each takes): remembered across rebuilds
            std::optional<str> key;
            if (sha)
                key = compare::cache_key_repr(S("(") + py::repr(KIND) + S(", ") + py::to_str(REV) + S(", ") + py::repr(*sha) + S(", ") + py::to_str(va) + S(", (") +
                                              py::to_str(std::get<0>(span)) + S(", ") + py::to_str(std::get<1>(span)) + S("), ") + (novalue ? S("True") : S("False")) +
                                              S(", ") + vector_items_repr(v) + S(", ") + externs_repr(context.ext_original) + S(", ") + argc_repr(context.argc) + S(")"));
            return lane_common::remembered(key, [&] { return fingerprint(mem.alone, va, v, context.ext_original, context.argc, first_image, span, novalue); });
        };
        compare::FingerprintOf<Vector> rebuilt = [&](const Vector& v) {
            return fingerprint(mem.both, rebuilt_va, v, merged, context.argc, mem.images, std::nullopt, novalue);
        };
        Found found;
        auto [verdict, reason, limits] = compare::compare<Vector>(
            original, rebuilt, vectors(), py::list<Vector>(), item.insns,
            [&](const py::set<int64_t>&, const py::set<int64_t>&) {
                return directed_and_guided(context.module_path, mem.alone, va, span, context.ext_original, mem.images[0], &found);
            },
            &info);
        if (verdict != V::DIVERGENT) {
            py::list<str> more = lane_common::path_limits(found.got.get());
            py::list<str> all = limits.copy();
            for (const str& s : more) all.append(s);
            limits = all;
        }
        return {verdict, reason, limits};
    };
    return lane_common::judged(compare_one);
}

std::tuple<str, str> profile(const str& module_path) {
    (void)module_path;
    const bool ready = concolic_ready();
    py::list<str> opts;
    for (const str& o : build::functions::CFLAGS_POWERPC) opts.append(py::repr(o));
    py::list<str> vs;
    for (const Vector& v : vectors()) {
        // (a dict in the order the Python writes its keys: args, fill, stub_ret)
        py::list<str> kv;
        py::list<str> a;
        for (int64_t x : v.args) a.append(ustr(x));
        kv.append(S("'args': [") + py::join(S(", "), a) + S("]"));
        if (v.fill) kv.append(S("'fill': ") + py::repr(*v.fill));
        if (v.stub_ret) kv.append(S("'stub_ret': ") + ustr(*v.stub_ret));
        vs.append(S("{") + py::join(S(", "), kv) + S("}"));
    }
    str stamp = S("''");
    if (ready) {
        auto s = emulate::pcode::stamp(*_mode());
        stamp = S("(") + py::repr(std::get<0>(s)) + S(", ") + py::repr(std::get<1>(s)) + S(")");
    }
    const str settings = S("('powerpc', ") + py::to_str(REV) + S(", ") + py::to_str(compare::REV) + S(", [") + py::join(S(", "), opts) + S("], [") +
                         py::join(S(", "), vs) + S("], ('paths', ") + (ready ? S("True") : S("False")) + S(", ") + py::to_str(lane_common::PATH_BUDGET) + S(", ") +
                         py::to_str(lane_common::MAX_PATHS) + S(", ") + py::to_str(lane_common::GUIDED_REST) + S(", ") + py::to_str(SOLVER_STEPS) + S(", ") + stamp +
                         S("))");
    return {py::hashlib::sha1(settings.encode("utf-8")).hexdigest().slice(0, 12), settings};
}

reconstruct::Prepared prepare(const str& module_path, const str& work) {
    auto m = std::make_shared<c_text::ModulePPC>(c_text::compilable_powerpc(module_path, work));
    reconstruct::Prepared out;
    out.header = m->header();
    out.texts = m->texts();
    out.order = m->order();
    for (const auto& kv : m->by_va().items()) out.key.set(std::get<1>(kv), py::percent(S("%08x"), std::get<0>(kv)));
    py::list<str> keys;
    for (int64_t va : m->image->functions) keys.append(py::percent(S("%08x"), va));
    std::vector<str> sorted = keys.vec();
    std::sort(sorted.begin(), sorted.end(), [](const str& a, const str& b) { return a.u32() < b.u32(); });
    out.all_keys = py::list<str>(sorted);
    str all = out.header;
    for (const str& n : out.order) all += out.texts.at(n);
    out.sig = py::hashlib::sha1(all.encode("utf-8", "replace")).hexdigest();
    const std::shared_ptr<translate::l2::Made> base = m;   // (the first base: what teeth's put_texts casts the lane back to)
    out.lane = std::static_pointer_cast<void>(std::static_pointer_cast<c_text::ModulePPC>(base));
    return out;
}

reconstruct::Built build(const reconstruct::Prepared& p, const str& module_path, const str& work) {
    const std::shared_ptr<c_text::ModulePPC> m = std::static_pointer_cast<c_text::ModulePPC>(std::static_pointer_cast<translate::l2::Made>(p.lane));
    const formats::powerpc_image::Image& img = *m->image;
    int64_t top = 0;
    for (const auto& s : img.spans) top = std::max(top, std::get<0>(s) + std::get<2>(s));
    const int64_t base = (img.base + std::max<int64_t>(_OFFSET, (top - img.base + 0xFFFF) & ~int64_t(0xFFFF))) & 0xFFFF0000LL;
    const py::dict<str, str> texts = m->texts();
    build::functions::BuiltPowerPC b = build::functions::build_powerpc(m->header(), texts, m->order(), m->externs, work, base);
    const str dll = b.dll.value_or(S(""));
    const py::dict<str, int64_t>& placed = b.where;
    std::set<int64_t> startset;
    for (int64_t f : img.functions) startset.insert(f);
    for (const auto& kv : img.imports.items()) startset.insert(std::get<0>(kv));
    if (!img.run_names) throw py::TypeError("'NoneType' object is not iterable");
    for (const auto& kv : img.run_names->items()) startset.insert(std::get<0>(kv));
    std::vector<int64_t> starts(startset.begin(), startset.end());
    std::map<int64_t, int64_t> nxt;
    for (size_t i = 0; i < starts.size(); ++i) nxt[starts[i]] = i + 1 < starts.size() ? starts[i + 1] : std::get<1>(img.code);
    reconstruct::Built out;
    py::dict<str, str> dropped = b.dropped.copy();
    const py::dict<int64_t, str> by_va = m->by_va();
    for (const auto& kv : by_va.items()) {
        const int64_t va = std::get<0>(kv);
        const str& fn = std::get<1>(kv);
        if (dropped.contains(fn)) continue;
        if (!placed.contains(fn)) {
            dropped.set(fn, S("not in the build"));
            continue;
        }
        auto item = std::make_shared<Item>();
        item->key = p.key.at(fn);
        item->va = va;
        item->span = Span(va, nxt.count(va) ? nxt[va] : va + 4);
        item->insns = std::max<int64_t>((std::get<1>(item->span) - std::get<0>(item->span)) / 4, 1);
        item->rebuilt_va = placed.at(fn);
        item->novalue = m->novalue.contains(fn);
        out.items.set(fn, item);
    }
    auto ctx = std::make_shared<Context>();
    ctx->module_path = module_path;
    ctx->dll = dll;
    for (const auto& kv : m->externs.items()) ctx->ext_original.set(std::get<1>(kv), std::get<0>(kv));
    for (const auto& kv : img.imports.items())
        if (!ctx->ext_original.contains(std::get<0>(kv))) ctx->ext_original.set(std::get<0>(kv), std::get<1>(kv));
    for (const auto& kv : m->externs.items())
        if (placed.contains(std::get<0>(kv))) ctx->ext_rebuilt.set(placed.at(std::get<0>(kv)), std::get<0>(kv));
    // Calls to code the image does not hold (func_0x<address>: a partial image calling into the rest of its title) are recorded by the
    // interpreter as calls to "@<address>"; their argument counts, where every call agrees, from the module's calls (its trees), as for
    // any function outside.
    py::set<str> absent;
    const py::re::Pattern absent_name = py::re::compile(S("func_0x[0-9a-f]{8}"));
    for (const str& n : m->calls.keys())
        if (absent_name.fullmatch(n)) absent.add(n);
    ctx->argc = m->argc.copy();
    for (const auto& kv : c_text::call_counts(m->calls, absent).items())
        ctx->argc.set(S("@") + std::get<0>(kv).slice_from(static_cast<int64_t>(std::string("func_0x").size())), std::get<1>(kv));
    out.context = ctx;
    out.dropped = dropped;
    return out;
}

namespace {

namespace W = jobs::workers;

class PowerPCKind : public reconstruct::Kind {
  public:
    str KIND() const override { return powerpc::KIND; }
    std::tuple<str, str> profile(const str& module_path) const override { return powerpc::profile(module_path); }
    reconstruct::Prepared prepare(const str& module_path, const str& work) const override { return powerpc::prepare(module_path, work); }
    reconstruct::Built build(const reconstruct::Prepared& m, const str& module_path, const str& work) const override {
        return powerpc::build(m, module_path, work);
    }
    reconstruct::Verdict judge(const reconstruct::Context& context, const reconstruct::Item& item) const override {
        return powerpc::judge(*std::static_pointer_cast<Context>(context), *std::static_pointer_cast<Item>(item));
    }
    bool PER_FUNCTION() const override { return powerpc::PER_FUNCTION; }
    std::optional<std::string> encode_context(const reconstruct::Context& context) const override {
        const Context& c = *std::static_pointer_cast<Context>(context);
        W::Enc e;
        e.s(c.module_path.utf8()).s(c.dll.utf8());
        for (const Externs* d : {&c.ext_original, &c.ext_rebuilt}) {
            e.i(static_cast<int64_t>(d->size()));
            for (const auto& kv : d->items()) e.i(std::get<0>(kv)).s(std::get<1>(kv).utf8());
        }
        e.i(static_cast<int64_t>(c.argc.size()));
        for (const auto& kv : c.argc.items()) e.s(std::get<0>(kv).utf8()).i(std::get<1>(kv));
        return e.text();
    }
    reconstruct::Context decode_context(const std::string& text) const override {
        W::Dec d(text);
        auto c = std::make_shared<Context>();
        c->module_path = str(d.s());
        c->dll = str(d.s());
        for (Externs* x : {&c->ext_original, &c->ext_rebuilt}) {
            const int64_t n = d.i();
            for (int64_t i = 0; i < n; ++i) {
                const int64_t k = d.i();
                x->set(k, str(d.s()));
            }
        }
        const int64_t n = d.i();
        for (int64_t i = 0; i < n; ++i) {
            const str k(d.s());
            c->argc.set(k, d.i());
        }
        return c;
    }
    std::string encode_item(const reconstruct::Item& item) const override {
        const Item& it = *std::static_pointer_cast<Item>(item);
        return W::Enc().s(it.key.utf8()).i(it.va).i(std::get<0>(it.span)).i(std::get<1>(it.span)).i(it.insns).i(it.rebuilt_va).i(it.novalue ? 1 : 0).text();
    }
    reconstruct::Item decode_item(const std::string& text) const override {
        W::Dec d(text);
        auto it = std::make_shared<Item>();
        it->key = str(d.s());
        it->va = d.i();
        const int64_t lo = d.i(), hi = d.i();
        it->span = Span(lo, hi);
        it->insns = d.i();
        it->rebuilt_va = d.i();
        it->novalue = d.i() != 0;
        return it;
    }
};

}  // namespace

const std::shared_ptr<reconstruct::Kind>& kind() {
    static std::shared_ptr<reconstruct::Kind> k = std::make_shared<PowerPCKind>();
    return k;
}

void register_lane() { reconstruct::register_kind(KIND, kind()); }

}  // namespace codebreaker::verify::lanes::powerpc
