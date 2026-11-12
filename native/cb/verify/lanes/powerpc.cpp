#include "cb/verify/lanes/powerpc.hpp"

#include <algorithm>
#include <format>
#include <iterator>

#include "cb/core/crypto.hpp"
#include "cb/core/error.hpp"
#include "cb/core/text.hpp"
#include "cb/emulate/shadow.hpp"

namespace cb::verify::lanes {
namespace {

namespace epp = emulate::powerpc;
namespace stdfs = std::filesystem;
using Lane = PowerPcLane;
using Vector = Lane::Vector;
using Span = Lane::Span;

constexpr std::uint64_t kLow32 = 0xFFFFFFFF;

std::uint32_t low32(std::uint64_t v) { return static_cast<std::uint32_t>(v); }
std::uint64_t unsigned_of(std::int64_t v) { return static_cast<std::uint64_t>(v); }
std::int64_t signed_of(std::uint64_t v) { return static_cast<std::int64_t>(v); }

// The fill as the guided search carries it in an input's `extra`: 1 pointers, 2 counts, 3 text.
std::int64_t fill_code(const std::optional<epp::Fill>& fill) {
    if (!fill) return 0;
    switch (*fill) {
    case epp::Fill::Pointers: return 1;
    case epp::Fill::Counts: return 2;
    case epp::Fill::Text: return 3;
    case epp::Fill::None: return 0;
    }
    return 0;
}
std::optional<epp::Fill> fill_of(std::int64_t code) {
    switch (code) {
    case 1: return epp::Fill::Pointers;
    case 2: return epp::Fill::Counts;
    case 3: return epp::Fill::Text;
    default: return std::nullopt;
    }
}

Vector make_vector(const std::vector<std::uint64_t>& args, std::optional<std::uint64_t> stub_ret = std::nullopt,
                   std::optional<epp::Fill> fill = std::nullopt) {
    Vector v;
    for (const std::uint64_t a : args) v.args.push_back(signed_of(a));
    if (stub_ret) v.stub_ret = signed_of(*stub_ret);
    v.fill = fill;
    return v;
}

std::vector<std::uint64_t> arguments_of(const Vector& v) {
    std::vector<std::uint64_t> out;
    for (const std::int64_t a : v.args) out.push_back(unsigned_of(a));
    return out;
}

std::span<const epp::Region> regions_of(const Vector& v) {
    return v.mem_init ? std::span<const epp::Region>(*v.mem_init) : std::span<const epp::Region>();
}

epp::RunOptions options_of(const Vector& v, const epp::ExternTable* externs, const std::optional<Span>& span, const epp::PageImage& shared, bool branches,
                           const std::optional<Span>& absent, epp::Shadow* shadow = nullptr) {
    epp::RunOptions o;
    o.externs = externs;
    o.span = span;
    o.stub_ret = unsigned_of(v.stub_ret.value_or(0));
    o.shared = shared;
    o.fill = v.fill.value_or(epp::Fill::None);
    o.branches = branches;
    o.absent = absent;
    o.shadow = shadow;
    return o;
}

// run(v)'s observation for the guessing explorer: its branches with what each compared, what ran, and where the stack is.
guided::Observation observation_of(const epp::RunResult& r) {
    guided::Observation o;
    if (r.branches) {
        for (const epp::BranchEvent& b : *r.branches) {
            guided::Branch branch{b.address, b.taken, std::nullopt};
            if (b.compare) {
                guided::Compare c;
                c.op = guided::CompareOp::Other;
                c.size = b.compare->size;
                c.a = b.compare->a;
                c.b = b.compare->b;
                c.a_at = b.compare->a_from;
                c.b_at = b.compare->b_from;
                branch.compare = c;
            }
            o.branches.push_back(branch);
        }
    }
    o.reach.insert(r.path.begin(), r.path.end());
    o.stack_low = epp::kStackLow;
    o.stack_high = epp::kStackTop;
    return o;
}

// One run of the original on `v` with its branches logged, or nothing when it could not be run at all (one run of many: the others go on).
std::optional<epp::RunResult> observed_run(const Lane::Context& context, std::uint32_t va, Span span, const Vector& v, epp::Shadow* shadow) {
    try {
        return epp::run(regions_of(v), va, arguments_of(v), options_of(v, &context.ext_original, span, context.memory.alone, true, context.memory.images[0], shadow));
    } catch (const Error&) {
        return std::nullopt;
    }
}

// The 32-bit big-endian word of the image at `va`.
std::optional<std::uint32_t> word_in(std::span<const epp::Region> regions, std::uint32_t va) {
    for (const epp::Region& r : regions) {
        if (va >= r.address && static_cast<std::uint64_t>(va) + 4 <= static_cast<std::uint64_t>(r.address) + r.data.size()) {
            const std::size_t at = va - r.address;
            return (static_cast<std::uint32_t>(r.data[at]) << 24) | (static_cast<std::uint32_t>(r.data[at + 1]) << 16) |
                   (static_cast<std::uint32_t>(r.data[at + 2]) << 8) | r.data[at + 3];
        }
    }
    return std::nullopt;
}

std::string hex8(std::uint32_t va) { return std::format("{:08x}", va); }

concolic::PathEntry path_entry_of(const emulate::native_shadow::PathEntry& e) {
    concolic::PathEntry p;
    p.site = signed_of(e.site);
    p.taken = e.taken;
    p.cond = e.condition;
    switch (e.kind) {
    case emulate::native_shadow::PathKind::Jump: p.kind = concolic::Kind::Jump; break;
    case emulate::native_shadow::PathKind::Inner: p.kind = concolic::Kind::Inner; break;
    case emulate::native_shadow::PathKind::Pin: p.kind = concolic::Kind::Pin; break;
    }
    return p;
}

}  // namespace

const std::string& PowerPcLane::name() const {
    static const std::string kName = "powerpc";
    return kName;
}

reconstruct::KindTraits PowerPcLane::traits() {
    reconstruct::KindTraits t;
    t.per_function = true;   // a verdict holds while the function and what it reaches are unchanged
    return t;
}

// ---- the inputs ----------------------------------------------------------------------------------------------------------------

std::vector<Vector> PowerPcLane::vectors() {
    const std::uint64_t a = epp::kArgsBase;
    const std::size_t n = kArguments;
    constexpr std::uint64_t ones = ~std::uint64_t{0};
    using lane_common::alternating;
    using lane_common::counting;
    using lane_common::descending;
    const auto from_base = [a](std::size_t i) { return a + 0x800 * i; };
    const auto small = [](std::size_t i) { return std::uint64_t{0x20} * (i + 1); };
    return {
        make_vector({0, 0, 0, 0, 0, 0, 0, 0}),
        make_vector(counting(n)),
        make_vector(counting(n, a, 0x1000)),
        make_vector(counting(n, a, 0x1000), 1),
        make_vector(counting(n, 0x100, 0x100), ones),
        make_vector(alternating(n, lane_common::Pattern(from_base), lane_common::Pattern(small))),
        make_vector(alternating(n, lane_common::Pattern(small), lane_common::Pattern(from_base)), 1),
        make_vector(descending(n, ones)),
        make_vector({a, 3, a + 0x4000, 0x40, a + 0x8000, 1, 0, 2}, 0x103),
        make_vector(counting(n, a, 0x1000), std::nullopt, epp::Fill::Pointers),
        make_vector(counting(n, a, 0x1000), 1, epp::Fill::Counts),
        make_vector(counting(n, a, 0x1000), std::nullopt, epp::Fill::Text),
        make_vector({a + 0x40, 5, a + 0x2000, 2, 1, a + 0x3000, 0, 9}, std::nullopt, epp::Fill::Counts),
        make_vector({7, a + 0x100, 0x30, a + 0x5000, a + 0x200, 4, 3, 1}, 2, epp::Fill::Pointers),
    };
}

std::vector<Vector> PowerPcLane::directed(const std::function<std::optional<std::uint32_t>(std::uint32_t)>& word, Span span, std::size_t most) {
    std::vector<std::uint64_t> seen;   // the constants, each once, in the order the code first has them
    for (std::uint64_t va = span.lo; va < span.hi; va += 4) {
        const std::uint32_t w = word(static_cast<std::uint32_t>(va)).value_or(0);
        const std::uint32_t opcode = w >> 26;
        if (opcode != 10 && opcode != 11) continue;   // cmpli, cmpi
        std::uint64_t c = w & 0xFFFF;
        if (opcode == 11 && (c & 0x8000)) c |= 0xFFFFFFFFFFFF0000ULL;   // (cmpi's is signed)
        if (c != 0 && c != 1 && std::ranges::find(seen, c) == seen.end()) seen.push_back(c);
    }
    const std::uint64_t a = epp::kArgsBase;
    std::vector<Vector> out;
    for (std::size_t k = 0; k < seen.size() && k < most; ++k) {
        const std::uint64_t c = seen[k];
        out.push_back(make_vector(std::vector<std::uint64_t>(kArguments, c), c));
        out.push_back(make_vector(lane_common::alternating(kArguments, lane_common::Pattern([a](std::size_t i) { return a + 0x1000 * i; }), lane_common::Pattern(c)), c,
                                  epp::Fill::Counts));
    }
    return out;
}

guided::Input PowerPcLane::to_guided(const Vector& v) {
    guided::Input g;
    g.args = arguments_of(v);
    if (v.mem_init) {
        g.mem_init.emplace();
        for (const Region& r : *v.mem_init) g.mem_init->push_back({r.address, r.data});
    }
    if (v.stub_ret) g.stub_ret = unsigned_of(*v.stub_ret);
    if (v.fill) g.extra["fill"] = fill_code(v.fill);
    return g;
}

Vector PowerPcLane::from_guided(const guided::Input& g) {
    Vector v;
    for (const std::uint64_t a : g.args) v.args.push_back(signed_of(a));
    if (g.mem_init) {
        v.mem_init.emplace();
        for (const guided::MemInit& m : *g.mem_init) v.mem_init->push_back({static_cast<std::uint32_t>(m.address), m.data});
    }
    if (g.stub_ret) v.stub_ret = signed_of(*g.stub_ret);
    if (const auto fill = g.extra.find("fill"); fill != g.extra.end()) v.fill = fill_of(std::get<std::int64_t>(fill->second));
    return v;
}

std::optional<Vector> PowerPcLane::derive(const Vector& v, const concolic::Values& values, const concolic::Vars& names) {
    Vector out = v;
    std::vector<std::int64_t> args = v.args;
    std::map<std::uint32_t, std::uint8_t> memory;   // the bytes the input sets, by address
    if (v.mem_init) {
        for (const Region& r : *v.mem_init) {
            for (std::size_t k = 0; k < r.data.size(); ++k) memory[r.address + static_cast<std::uint32_t>(k)] = r.data[k];
        }
    }
    bool changed = false;
    for (const auto& [name, value] : values) {
        const auto known = names.find(name);
        if (known == names.end()) continue;
        const concolic::InputVar& var = known->second;
        if (var.place == concolic::InputVar::Place::Arg && var.where >= 0 && static_cast<std::size_t>(var.where) < args.size()) {
            const std::size_t at = static_cast<std::size_t>(var.where);
            changed |= unsigned_of(args[at]) != value;
            args[at] = signed_of(value);
        } else if (var.place == concolic::InputVar::Place::Mem) {
            const std::uint32_t address = static_cast<std::uint32_t>(var.where);
            const auto set = memory.find(address);
            changed |= (set != memory.end() ? set->second : static_cast<std::uint8_t>(var.value)) != static_cast<std::uint8_t>(value & 0xFF);
            memory[address] = static_cast<std::uint8_t>(value & 0xFF);
        }
    }
    if (!changed) return std::nullopt;
    out.args = args;
    if (!memory.empty()) {
        const Bytes under = v.fill ? epp::filled(*v.fill) : Bytes();
        std::map<std::uint32_t, Bytes> pages;   // the pages of the fill the bytes fall in, whole
        std::vector<Region> runs;               // bytes elsewhere, each run of neighbours one region
        for (const auto& [at, byte] : memory) {
            const std::uint32_t base = at & ~(epp::kPageSize - 1);
            const std::int64_t lo = static_cast<std::int64_t>(base) - static_cast<std::int64_t>(epp::kArgsBase);
            if (0 <= lo && static_cast<std::size_t>(lo) < under.size()) {   // (in the fill: the whole page)
                if (!pages.contains(base)) pages[base] = Bytes(under.begin() + lo, under.begin() + std::min<std::size_t>(lo + epp::kPageSize, under.size()));
                pages[base].resize(epp::kPageSize);
                pages[base][at - base] = byte;
            } else if (!runs.empty() && runs.back().address + runs.back().data.size() == at) {
                runs.back().data.push_back(byte);
            } else {
                runs.push_back({at, Bytes{byte}});
            }
        }
        std::vector<Region> all;
        for (auto& [base, data] : pages) all.push_back({base, std::move(data)});
        for (Region& r : runs) all.push_back(std::move(r));
        std::ranges::sort(all, {}, &Region::address);
        out.mem_init = std::move(all);
    }
    return out;
}

// ---- what a run showed -----------------------------------------------------------------------------------------------------------

PowerPcLane::Memory PowerPcLane::memory_of(std::span<const Region> original, std::span<const Region> rebuilt) {
    const auto extent = [](std::span<const Region> regions) {
        Span s{0, 0};
        if (regions.empty()) return s;
        s.lo = regions.front().address;
        for (const Region& r : regions) {
            s.lo = std::min(s.lo, r.address);
            s.hi = std::max<std::uint32_t>(s.hi, r.address + static_cast<std::uint32_t>(r.data.size()));
        }
        return s;
    };
    Memory m;
    std::vector<Region> both(original.begin(), original.end());
    both.insert(both.end(), rebuilt.begin(), rebuilt.end());
    m.alone = epp::PageImage::of(original);
    m.both = epp::PageImage::of(both);
    m.images = {extent(original)};
    if (!rebuilt.empty()) m.images.push_back(extent(rebuilt));
    return m;
}

std::string PowerPcLane::argument(epp::Machine& machine, std::uint64_t value, std::span<const Span> images) {
    const std::uint32_t x = low32(value);
    if (epp::kStackLow <= x && x < epp::kStackTop) return "stack";
    for (const Span& image : images) {
        if (image.contains(x)) {
            Bytes data;
            for (std::uint32_t i = 0; i < 12; ++i) data.push_back(static_cast<std::uint8_t>(machine.read(x + i, 1)));
            return "data:" + crypto::to_hex(data);
        }
    }
    return lane_common::argument(static_cast<std::int64_t>(x));
}

compare::Fingerprint PowerPcLane::fingerprint(const epp::PageImage& shared, std::uint32_t entry, const Vector& v, const epp::ExternTable& externs,
                                              const std::map<std::string, std::int64_t>& argc, std::span<const Span> images,
                                              const std::optional<Span>& span, bool novalue) {
    epp::RunResult r;
    try {
        r = epp::run(regions_of(v), entry, arguments_of(v), options_of(v, &externs, span, shared, false, images.front()));
    } catch (const Error& e) {   // (this input is not an observation: the comparer goes on to the next)
        compare::Fingerprint fp;
        fp.err = e.what();
        return fp;
    }
    const std::string ex = r.exit_text();
    const emulate::Run record = epp::record(r);
    std::vector<lane_common::Called> called;
    for (const epp::Call& c : r.calls) {
        lane_common::Called k;
        k.name = c.name;
        if (const auto known = argc.find(c.name); known != argc.end()) {   // (how many arguments it takes, where every call agrees)
            std::vector<std::int64_t> a;
            for (std::int64_t i = 0; i < known->second && i < static_cast<std::int64_t>(c.args.size()); ++i) a.push_back(signed_of(c.args[static_cast<std::size_t>(i)]));
            k.args = std::move(a);
        }
        called.push_back(std::move(k));
    }
    epp::Machine& machine = *r.machine;
    std::vector<std::string> deps = lane_common::calls(called, [&](std::int64_t x) { return argument(machine, unsigned_of(x), images); });
    if (r.truncated && r.calls.size() >= epp::kMaxCalls) deps.push_back("...");
    const Span rebuilt = images.back();   // the rebuilt image, when there is one
    lane_common::ChangedOptions changed;
    changed.place = [&](std::uint64_t a) -> lane_common::Where {
        if (images.size() > 1 && rebuilt.contains(static_cast<std::uint32_t>(a))) return lane_common::Skip{};
        return lane_common::Place(a);
    };
    lane_common::FingerprintParts parts;
    if (ex == "ret" && !novalue) parts.ret = signed_of(r.ret & kLow32);
    parts.writes = lane_common::left(record, changed);
    parts.saved = record.kept();
    const std::int64_t lo = span ? span->lo : 0;
    for (const std::uint32_t p : r.path) parts.cov.push_back(static_cast<std::int64_t>(p) - lo);
    std::ranges::sort(parts.cov);
    return lane_common::fingerprint(record, std::move(deps), std::move(parts));
}

// ---- finding inputs ----------------------------------------------------------------------------------------------------------------

std::pair<std::vector<Vector>, std::shared_ptr<concolic::Stats>> PowerPcLane::explore(const Context& context, std::uint32_t va, Span span,
                                                                                    std::span<const Vector> seeds_in) {
    const Span region{epp::kArgsBase, epp::kArgsBase + 0x10000};
    const auto is_input = [&](std::uint64_t b) {
        if (region.contains(static_cast<std::uint32_t>(b))) return true;
        return std::ranges::any_of(context.writable, [&](const Span& d) { return d.contains(static_cast<std::uint32_t>(b)); });
    };

    std::vector<Vector> seeds(seeds_in.begin(), seeds_in.end());
    std::vector<guided::Seen> seen;                               // what the seeds did, for the guessing that may follow
    std::map<std::size_t, concolic::Trail> known;                 // the paths of the seeds, run beforehand without a shadow
    for (std::size_t i = 0; i < seeds.size(); ++i) {
        const auto r = observed_run(context, va, span, seeds[i], nullptr);
        if (!r) continue;
        guided::Observation o = observation_of(*r);
        concolic::Trail trail;
        for (const guided::Branch& b : o.branches) trail.push_back({signed_of(b.address), b.taken});
        known[i] = std::move(trail);
        seen.push_back({to_guided(seeds[i]), std::move(o)});
    }

    std::vector<std::shared_ptr<const emulate::native_shadow::Run>> kept_runs;   // the conditions' nodes live in these until the end
    const std::function<std::optional<concolic::Observation>(Vector&)> run = [&](Vector& v) -> std::optional<concolic::Observation> {
        emulate::shadow::PowerPcShadow shadow(*context.lifter, is_input);
        std::vector<emulate::native_shadow::Place> places;
        for (std::size_t k = 0; k < v.args.size() && k < kArguments; ++k) {
            emulate::native_shadow::Place p;
            p.where = std::format("r{}", 3 + k);
            p.value = unsigned_of(v.args[k]);
            p.input = static_cast<std::int64_t>(k);
            places.push_back(std::move(p));
        }
        shadow.enter(places, 64);
        const auto r = observed_run(context, va, span, v, &shadow);
        if (!r) return std::nullopt;
        seen.push_back({to_guided(v), observation_of(*r)});
        const emulate::native_shadow::Run& made = shadow.run();
        kept_runs.push_back(shadow.shared_run());
        concolic::Observation ob;
        for (const emulate::native_shadow::Step& s : made.trail) ob.trail.push_back({signed_of(s.site), s.taken});
        for (const emulate::native_shadow::PathEntry& e : made.path) ob.path.push_back(path_entry_of(e));
        for (const auto& [var_name, var] : made.vars) {
            // (only the inputs the explorer may set: an argument, bytes of memory; what a call out returned is the environment's)
            if (var.kind == emulate::native_shadow::VarKind::Arg && std::holds_alternative<std::uint64_t>(var.where)) {
                ob.vars[var_name] = {concolic::InputVar::Place::Arg, signed_of(std::get<std::uint64_t>(var.where)), var.bits, signed_of(var.value)};
            } else if (var.kind == emulate::native_shadow::VarKind::Mem && std::holds_alternative<std::uint64_t>(var.where)) {
                ob.vars[var_name] = {concolic::InputVar::Place::Mem, signed_of(std::get<std::uint64_t>(var.where)), var.bits, signed_of(var.value)};
            }
        }
        ob.lost = made.lost;
        ob.cut = r->truncated;
        if (r->truncated) ob.lost.insert("truncated");
        return ob;
    };
    const std::function<std::optional<Vector>(const Vector&, const concolic::Values&, const concolic::Vars&)> derived = &PowerPcLane::derive;
    auto got = std::make_shared<concolic::Exploration<Vector>>(lane_common::explored<Vector>(
        run, std::span<Vector>(seeds), derived, [lo = span.lo, hi = span.hi](std::int64_t site) { return lo <= site && site < hi; }, known, kSolverSteps));
    std::vector<Vector> kept = got->kept;
    // Where a jump of the original has gone one way only on every input the explorer made, and the paths are not all there is, guess.
    std::map<std::int64_t, int> ways;
    for (const concolic::Step& e : got->edges) ways[e.site] |= e.taken ? 2 : 1;
    const bool one_way = std::ranges::any_of(ways, [](const auto& w) { return w.second != 3; });
    if (!got->complete && one_way && lane_common::kGuidedRest > 0 && !seen.empty()) {
        const guided::Run observe_only = [&](const guided::Input& in) -> std::optional<guided::Observation> {
            const auto r = observed_run(context, va, span, from_guided(in), nullptr);
            if (!r) return std::nullopt;
            return observation_of(*r);
        };
        guided::Options options;
        options.budget = lane_common::kGuidedRest;
        options.keep = lane_common::kMaxGuided;
        options.seed = va & 0xFFFF;
        options.machine = {guided::ByteOrder::Big, 8};
        options.observed = seen;
        const std::vector<guided::Input> extra = guided::guided_inputs(observe_only, {}, options);
        for (const guided::Input& g : extra) kept.push_back(from_guided(g));
        got->guessed = static_cast<int>(extra.size());
    }
    return {std::move(kept), std::static_pointer_cast<concolic::Stats>(got)};
}

std::vector<Vector> PowerPcLane::directed_and_guided(const Context& context, std::uint32_t va, Span span, std::shared_ptr<concolic::Stats>* explored) {
    std::vector<Vector> d;
    {
        const std::lock_guard<std::mutex> lock(context.lock);
        auto& cache = context.directed_by_span;
        const auto key = std::make_pair(span.lo, span.hi);
        auto it = cache.find(key);
        if (it == cache.end()) {
            it = cache.emplace(key, directed([&](std::uint32_t a) { return word_in(context.original_regions, a); }, span)).first;
        }
        d = it->second;
    }
    std::vector<Vector> seeds = vectors();
    seeds.insert(seeds.end(), d.begin(), d.end());
    std::vector<Vector> out = d;
    if (lane_common::guided_on() && lane_common::concolic_on() && context.lifter != nullptr) {
        auto [kept, got] = explore(context, va, span, seeds);
        if (explored) *explored = got;
        out.insert(out.end(), kept.begin(), kept.end());
        return out;
    }
    if (!lane_common::guided_on()) return out;
    const guided::Run run = [&](const guided::Input& in) -> std::optional<guided::Observation> {
        const auto r = observed_run(context, va, span, from_guided(in), nullptr);
        if (!r) return std::nullopt;
        return observation_of(*r);
    };
    std::vector<guided::Input> gseeds;
    for (const Vector& v : seeds) gseeds.push_back(to_guided(v));
    for (const guided::Input& g : lane_common::guided(run, gseeds, va & 0xFFFF, {guided::ByteOrder::Big, 8})) out.push_back(from_guided(g));
    return out;
}

// ---- one function ------------------------------------------------------------------------------------------------------------------

namespace {

// What an input is, as text, for a fingerprint's key: its arguments, fill, memory and stub return.
std::string vector_text(const Vector& v) {
    std::string out = "args";
    for (const std::int64_t a : v.args) out += std::format(" {}", unsigned_of(a));
    if (v.fill) out += std::format(" fill {}", fill_code(v.fill));
    if (v.mem_init) {
        for (const epp::Region& r : *v.mem_init) out += std::format(" mem {:x}:{}", r.address, crypto::to_hex(r.data));
    }
    if (v.stub_ret) out += std::format(" ret {}", unsigned_of(*v.stub_ret));
    return out;
}

std::string externs_text(const epp::ExternTable& table) {
    const std::map<std::uint32_t, std::string> sorted(table.begin(), table.end());
    std::string out;
    for (const auto& [address, name] : sorted) out += std::format(" {:x}={}", address, name);
    return out;
}

std::string argc_text(const std::map<std::string, std::int64_t>& argc) {
    std::string out;
    for (const auto& [name, n] : argc) out += std::format(" {}={}", name, n);
    return out;
}

}  // namespace

Judgement PowerPcLane::judge_function(const Context& context, const Item& item) {
    const lane_common::Judgement made = lane_common::judged([&]() -> lane_common::Judgement {
        const Memory& memory = context.memory;
        epp::ExternTable merged = context.ext_original;
        for (const auto& [address, name] : context.ext_rebuilt) merged[address] = name;
        const std::span<const Span> first_image = std::span<const Span>(memory.images).first(1);

        const std::function<compare::Fingerprint(const Vector&)> original = [&](const Vector& v) {
            // the original's run depends on the original, and on what the build says of the calls out of it (their names, how many
            // arguments each takes): remembered across rebuilds
            std::optional<std::string> key;
            if (context.sha) {
                key = compare::cache_key_text(std::format("powerpc {} {} {:x} {:x}-{:x} {} | {} |{} |{}", kRev, *context.sha, item.va, item.span.lo, item.span.hi,
                                                           item.novalue, vector_text(v), externs_text(context.ext_original), argc_text(context.argc)));
            }
            return lane_common::remembered(context.fingerprints, key, [&] {
                return fingerprint(memory.alone, item.va, v, context.ext_original, context.argc, first_image, item.span, item.novalue);
            });
        };
        const std::function<compare::Fingerprint(const Vector&)> rebuilt = [&](const Vector& v) {
            return fingerprint(memory.both, item.rebuilt_va, v, merged, context.argc, memory.images, std::nullopt, item.novalue);
        };
        std::shared_ptr<concolic::Stats> explored;
        const std::vector<Vector> fixed = vectors();
        const compare::Outcome outcome = compare::compare<Vector>(
            original, rebuilt, fixed, {}, {.insns = item.insns},
            [&](const std::set<std::int64_t>&, const std::set<std::int64_t>&) { return directed_and_guided(context, item.va, item.span, &explored); });
        std::vector<std::string> limits = outcome.limit_codes();
        if (outcome.verdict != Verdict::Divergent) {
            const std::vector<std::string> paths = lane_common::path_limits(explored.get());
            limits.insert(limits.end(), paths.begin(), paths.end());
        }
        return {outcome.verdict, outcome.reason, limits, outcome.detail};
    });
    return {made.verdict, made.reason, made.limits, made.detail};
}

// ---- the kind ------------------------------------------------------------------------------------------------------------------------

Profile PowerPcLane::profile(const stdfs::path&) const {
    const bool paths = lane_common::guided_on() && lane_common::concolic_on() && services_.lifters != nullptr &&
                       services_.lifters->get(emulate::pcode::Mode::PpcBe64) != nullptr;
    std::string settings = std::format("powerpc rev {} compare {} options", kRev, compare::kRevision);
    for (const std::string& o : build::functions::cflags_powerpc()) settings += " " + o;
    settings += " vectors";
    for (const Vector& v : vectors()) settings += " [" + vector_text(v) + "]";
    settings += std::format(" paths {} {} {} {} {}", paths, lane_common::kPathBudget, lane_common::kMaxPaths, lane_common::kGuidedRest, kSolverSteps);
    return {crypto::to_hex(crypto::sha1(as_bytes(settings))).substr(0, 12), settings};
}

std::unique_ptr<Prepared> PowerPcLane::prepare(const stdfs::path& module_path, const stdfs::path& work) const {
    auto p = std::make_unique<PreparedPowerPc>();
    p->module = decompile::c_text::compilable_powerpc(services_.decompiler, module_path, work);
    const translate::l2::Made& made = p->module.made;
    p->header = made.header();
    p->order = made.order();
    const auto texts = made.texts();
    std::string all = p->header;
    for (const auto& [name, text] : texts) p->texts[name] = text;
    for (const std::string& name : p->order) {
        if (const auto text = p->texts.find(name); text != p->texts.end()) all += text->second;
    }
    for (const auto& [va, name] : made.by_va()) {
        const std::string key = hex8(static_cast<std::uint32_t>(va));
        p->key[name] = key;
        p->all_keys.push_back(key);
    }
    std::ranges::sort(p->all_keys);
    p->sig = crypto::to_hex(crypto::sha1(as_bytes(all)));
    return p;
}

Built PowerPcLane::build(const Prepared& prepared, const stdfs::path& module_path, const stdfs::path& work) const {
    const auto& p = static_cast<const PreparedPowerPc&>(prepared);
    const decompile::c_text::ModulePPC& m = p.module;
    const Image original = services_.read_image(module_path);
    std::uint64_t top = 0;
    for (const auto& s : m.image.spans) top = std::max(top, s.va + s.length);
    const std::uint64_t span_of_image = top > original.base ? top - original.base : 0;
    const std::uint64_t base = (original.base + std::max<std::uint64_t>(kRebuiltOffset, (span_of_image + 0xFFFF) & ~std::uint64_t{0xFFFF})) & 0xFFFF0000ULL;

    build::functions::Texts texts;
    for (const auto& [name, text] : m.made.texts()) texts[name] = text;
    std::vector<std::string> externs;
    for (const auto& entry : m.externs) externs.push_back(entry.first);
    const build::functions::BuiltPowerPc b = services_.builder.build_powerpc(services_.tools, m.made.header(), texts, p.order, externs, work, base);

    // where each function ends: at the start of the next thing the image has (a function, an import stub, an inlined run), or its code's end
    std::set<std::uint64_t> starts;
    for (const auto& [va, name] : m.made.by_va()) starts.insert(va);
    for (const auto& entry : m.image.imports) starts.insert(entry.first);
    for (const auto& entry : m.image.inlined) starts.insert(entry.first);
    auto ctx = std::make_shared<Context>();
    std::vector<Region> rebuilt_regions;
    if (b.dll) rebuilt_regions = services_.read_image(*b.dll).regions;
    ctx->memory = memory_of(original.regions, rebuilt_regions);
    ctx->original_regions = original.regions;
    ctx->writable = original.writable;
    ctx->sha = services_.content_sha ? services_.content_sha(module_path) : std::nullopt;
    ctx->fingerprints = services_.fingerprints;
    ctx->lifter = services_.lifters ? services_.lifters->get(emulate::pcode::Mode::PpcBe64) : nullptr;
    for (const auto& [name, address] : m.externs) ctx->ext_original[static_cast<std::uint32_t>(address)] = name;
    for (const auto& [address, name] : m.image.imports) ctx->ext_original.try_emplace(static_cast<std::uint32_t>(address), name);
    for (const auto& [name, address] : m.externs) {
        if (const auto placed = b.where.find(name); placed != b.where.end()) ctx->ext_rebuilt[static_cast<std::uint32_t>(placed->second)] = name;
    }
    // Calls to code the image does not hold (func_0x<address>: a partial image calling into the rest of its title) are recorded by the
    // interpreter as calls to "@<address>"; their argument counts, where every call agrees, from the module's calls, as for any function outside.
    std::set<std::string> absent;
    for (const auto& [callee, counts] : m.calls) {
        const bool absent_name = callee.size() == 15 && callee.starts_with("func_0x") &&
                                 std::ranges::all_of(std::string_view(callee).substr(7), [](char c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f'); });
        if (absent_name) absent.insert(callee);
    }
    ctx->argc = m.argc;
    for (const auto& [callee, count] : decompile::c_text::call_counts(m.calls, absent)) ctx->argc["@" + callee.substr(7)] = count;

    Built built;
    std::map<std::string, Item> items;
    for (const auto& [reason_name, reason] : b.dropped) built.missing[reason_name] = reason;
    for (const auto& [va, name] : m.made.by_va()) {
        if (built.missing.contains(name)) continue;
        const auto placed = b.where.find(name);
        if (placed == b.where.end()) {
            built.missing[name] = "not in the build";
            continue;
        }
        Item item;
        item.key = p.key.at(name);
        item.va = static_cast<std::uint32_t>(va);
        const auto next = starts.upper_bound(va);
        item.span = {item.va, static_cast<std::uint32_t>(next != starts.end() ? *next : original.code.hi)};
        item.insns = std::max<std::int64_t>((static_cast<std::int64_t>(item.span.hi) - item.span.lo) / 4, 1);
        item.rebuilt_va = static_cast<std::uint32_t>(placed->second);
        item.novalue = m.novalue.contains(name);
        built.items.push_back(name);
        items[name] = item;
    }
    built.judge = [ctx, items = std::move(items)](const std::string& name) { return judge_function(*ctx, items.at(name)); };
    return built;
}

}  // namespace cb::verify::lanes
