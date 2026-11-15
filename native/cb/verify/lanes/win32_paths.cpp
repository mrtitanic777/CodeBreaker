// cb-unity: alone
// cb/verify/lanes/win32_paths.cpp -- how the 32-bit lane finds inputs for one function: those aimed at what its code tests (directed), those that
// take branches no earlier input took, guessed from the comparisons it makes (verify/guided) or solved for path by path (verify/concolic, over
// the symbolic shadow of the original's run) (see win32.hpp).

#include <algorithm>
#include <cstring>
#include <tuple>

#include "cb/core/fs.hpp"
#include "cb/emulate/native_shadow.hpp"
#include "cb/verify/lanes/win32_internal.hpp"
#include "cb/verify/stage_timer.hpp"

namespace cb::verify::lanes::win32 {

namespace emu = emulate::emulator;

namespace {

constexpr std::uint32_t kLow32 = 0xFFFFFFFFu;

Bytes dword_le(std::uint32_t v) { return Bytes{static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v >> 16), static_cast<std::uint8_t>(v >> 24)}; }

Vector vector_of(const std::vector<std::uint64_t>& args, std::vector<guided::MemInit> memory, std::optional<std::int64_t> stub_ret = std::nullopt,
                 bool with_memory = true) {
    Vector v;
    for (const std::uint64_t a : args) v.args.push_back(static_cast<std::int64_t>(a));
    if (with_memory) v.mem_init = std::move(memory);
    v.stub_ret = stub_ret;
    return v;
}

std::vector<guided::MemInit> joined(std::vector<guided::MemInit> a, const std::vector<guided::MemInit>& b) {
    a.insert(a.end(), b.begin(), b.end());
    return a;
}

// What a run did, for the question "did this input make the original do something no earlier one did?".
using Did = std::tuple<std::string, std::optional<std::int64_t>, std::vector<std::string>, std::optional<std::string>>;
Did did(const compare::Fingerprint& f) { return {f.exit, f.ret, f.deps, f.writes}; }

}  // namespace

std::vector<Vector> directed_vectors(const Context& context, std::uint64_t func, const std::set<std::int64_t>& reached, const Shape& shape) {
    const Context::Harvest& harvest = context.harvest(func);
    const std::vector<guided::MemInit>& filled = context.data_filled();
    if (harvest.constants.empty() && harvest.globals.empty() && filled.empty()) return {};
    const std::size_t n = Win32Lane::kArity;
    const std::vector<guided::MemInit> provisioned = Win32Lane::provisioned_memory();
    std::vector<std::uint64_t> object;
    for (std::size_t i = 0; i < n; ++i) object.push_back(Win32Lane::kObject + 0x40 * (i % 32));
    const std::vector<std::uint64_t> ints = lane_common::counting(n);

    // the globals the function reads, set to non-zero values
    std::vector<std::vector<guided::MemInit>> global_sets;
    if (!harvest.globals.empty()) {
        for (const std::uint32_t value : {1u, Win32Lane::kSafe, Win32Lane::kObject + 0x800}) {
            std::vector<guided::MemInit> g;
            for (const std::uint32_t a : harvest.globals) g.push_back(guided::MemInit{a, dword_le(value)});
            global_sets.push_back(std::move(g));
        }
        std::vector<guided::MemInit> each;   // each global a value of its own (see Context::data_filled)
        for (std::size_t i = 0; i < harvest.globals.size(); ++i)
            each.push_back(guided::MemInit{harvest.globals[i], dword_le(Win32Lane::kSafe + 4 * static_cast<std::uint32_t>(i))});
        global_sets.push_back(std::move(each));
    }

    std::vector<Vector> candidates;
    if (!filled.empty()) {
        candidates.push_back(vector_of(object, joined(provisioned, filled), 1));
        candidates.push_back(vector_of(ints, joined(provisioned, filled)));
    }
    for (const auto& g : global_sets) {
        candidates.push_back(vector_of(object, joined(provisioned, g), 1));
        candidates.push_back(vector_of(ints, joined(provisioned, g)));
    }
    // (Every position is tried, also past the arguments the function removes on return: a function does sometimes read further up the
    // stack, and one difference was only found so.)
    for (const std::uint32_t k : harvest.constants) {
        for (std::size_t i = 0; i < n; ++i) {
            std::vector<std::uint64_t> a = object;
            a[i] = k;
            candidates.push_back(vector_of(a, provisioned, 1));
            if (!global_sets.empty()) candidates.push_back(vector_of(a, joined(provisioned, global_sets[1]), 1));
        }
        candidates.push_back(vector_of(std::vector<std::uint64_t>(n, k), {}, 1, false));
        // ...and into every field of the objects the arguments point at (`if (p->flags & 0x40)`).
        Bytes everywhere;
        for (int i = 0; i < 0x400; ++i) {
            const Bytes w = dword_le(k);
            everywhere.insert(everywhere.end(), w.begin(), w.end());
        }
        const std::vector<guided::MemInit> fill = {{Win32Lane::kObject, everywhere}, {Win32Lane::kSafe, Bytes(16, 0xC3)}};
        candidates.push_back(vector_of(object, fill, 1));
        candidates.push_back(vector_of(object, fill));
    }
    // "New code" is counted over everything that ran, callees included: an input that drives a helper down a different path exercises the
    // caller's use of it even at 100% own coverage.
    if (shape) {   // (a member function: the first value goes in ECX)
        for (Vector& v : candidates) v = shape(v);
    }
    std::vector<Vector> keep;
    std::set<std::int64_t> seen = reached;
    for (const Vector& v : candidates) {
        const compare::Fingerprint f = fingerprint(context, context.original, func, v);
        if (f.err) continue;
        bool fresh = false;
        for (const std::int64_t a : f.reach) fresh |= seen.insert(a).second;
        if (fresh) {
            keep.push_back(v);
            if (keep.size() >= Win32Lane::kMaxDirected) return keep;
        }
    }
    // Then inputs that make the original DO something no earlier input made it do, though no new code runs: the values either side of each
    // constant it compares with. `x < 100` and `x < 101` run the same instructions on every input but 100; only that input tells them apart,
    // and no count of instructions reached would ever ask for it.
    std::set<Did> done;
    std::vector<Vector> known = Win32Lane::vectors();
    for (Vector& v : Win32Lane::confirm_vectors()) known.push_back(std::move(v));
    for (Vector v : known) {
        if (shape) v = shape(v);
        const compare::Fingerprint f = fingerprint(context, context.original, func, v);
        if (!f.err) done.insert(did(f));
    }
    for (const Vector& v : keep) {
        const compare::Fingerprint f = fingerprint(context, context.original, func, v);
        if (!f.err) done.insert(did(f));
    }
    std::vector<Vector> edges;
    for (const std::uint32_t k : harvest.constants) {
        for (const std::uint32_t near : {(k - 1) & kLow32, (k + 1) & kLow32, k}) {
            for (std::size_t i = 0; i < n; ++i) {
                std::vector<std::uint64_t> a = ints;
                a[i] = near;
                edges.push_back(vector_of(a, {}, std::nullopt, false));
                a = object;
                a[i] = near;
                edges.push_back(vector_of(a, provisioned, 1));
            }
        }
    }
    for (Vector& v : edges) {
        if (shape) v = shape(v);
        candidates.push_back(std::move(v));
    }
    for (const Vector& v : candidates) {
        const compare::Fingerprint f = fingerprint(context, context.original, func, v);
        if (f.err || !done.insert(did(f)).second) continue;
        keep.push_back(v);
        if (keep.size() >= Win32Lane::kMaxDirected) break;
    }
    return keep;
}

std::vector<Vector> guided_vectors(const Context& context, std::uint64_t func, const std::vector<Vector>& seeds, const Shape& shape) {
    const guided::Run run = [&](const guided::Input& in) -> std::optional<guided::Observation> {
        const Vector v = Win32Lane::from_guided(in);
        return branch_run(context, func, shape ? shape(v) : v);
    };
    std::vector<guided::Input> seed_inputs;
    for (const Vector& v : seeds) seed_inputs.push_back(Win32Lane::to_guided(v));
    std::vector<Vector> out;
    for (const guided::Input& g : lane_common::guided(run, seed_inputs, func & 0xFFFF)) out.push_back(Win32Lane::from_guided(g));
    return out;
}

std::pair<std::vector<Vector>, std::shared_ptr<concolic::Stats>> explore_vectors(const Context& context, std::uint64_t func, std::vector<Vector> seeds,
                                                                                const Shape& shape) {
    std::uint64_t lo = 0, hi = 0;
    try {
        auto lease = context.tracers.acquire();
        const emu::ResolvedFunction f = lease.get().resolve(fs::to_text(context.original), emu::FuncSelector{func});
        lo = f.va();
        hi = f.end_va();
    } catch (const Error&) {   // a function the tracer cannot find: guess, as without a lifter
        return {guided_vectors(context, func, seeds, shape), nullptr};
    }
    // The inputs of the symbolic shadow: the scratch window the pointer arguments point into, and the module's writable data.
    std::vector<std::pair<std::uint64_t, std::uint64_t>> input_memory{{emu::kScratchBase, emu::kScratchBase + emu::kScratchSize}};
    for (const auto& range : context.writable(context.original)) input_memory.push_back(range);

    // The seeds are run first as the guessing explorer runs them (their branches logged): of those that took the same path only one is run again
    // with its shadow.
    std::vector<guided::Seen> seen;
    std::map<std::size_t, concolic::Trail> known;
    for (std::size_t i = 0; i < seeds.size(); ++i) {
        const auto o = branch_run(context, func, shape ? shape(seeds[i]) : seeds[i]);
        if (!o) continue;
        concolic::Trail trail;
        for (const guided::Branch& b : o->branches) trail.push_back({static_cast<std::int64_t>(b.address), b.taken});
        known[i] = std::move(trail);
        seen.push_back({Win32Lane::to_guided(seeds[i]), *o});
    }

    std::vector<std::shared_ptr<const emulate::native_shadow::Run>> kept_runs;   // the conditions' nodes live in these until the end
    const std::function<std::optional<concolic::Observation>(Vector&)> run = [&](Vector& v) -> std::optional<concolic::Observation> {
        const Vector w = shape ? shape(v) : v;
        emulate::native_shadow::Shadow shadow(emulate::native_shadow::Setup::for_lifter(emulate::native_shadow::Kind::Flat32, *context.lifter, input_memory,
                                                                                        std::make_pair(lo, hi)));
        const std::uint64_t esp = emu::kStackTop - 4 * w.args.size() - 4;
        std::vector<emulate::native_shadow::Place> places;
        for (std::size_t k = 0; k < w.args.size(); ++k) {
            emulate::native_shadow::Place p;
            p.where = esp + 4 + 4 * k;
            p.value = static_cast<std::uint64_t>(w.args[k]) & kLow32;
            p.input = static_cast<std::int64_t>(k);
            places.push_back(std::move(p));
        }
        for (const auto& [reg, value] : {std::pair<const char*, std::optional<std::uint32_t>>{"ecx", w.ecx}, {"edx", w.edx}}) {
            if (!value) continue;
            emulate::native_shadow::Place p;
            p.where = std::string(reg == std::string_view("ecx") ? "ECX" : "EDX");
            p.value = *value;
            p.input = std::string(reg);
            places.push_back(std::move(p));
        }
        shadow.enter(places, 32);
        const std::optional<emulate::Run> r = observed_run(context, func, w, &shadow);
        if (!r) return std::nullopt;
        guided::Observation o = observation_of(*r);
        o.args_at = esp + 4;
        seen.push_back({Win32Lane::to_guided(v), std::move(o)});
        const std::shared_ptr<const emulate::native_shadow::Run> made = shadow.run();
        kept_runs.push_back(made);
        concolic::Observation ob;
        for (const emulate::native_shadow::Step& s : made->trail) ob.trail.push_back({static_cast<std::int64_t>(s.site), s.taken});
        for (const emulate::native_shadow::PathEntry& e : made->path) {
            concolic::PathEntry p;
            p.site = static_cast<std::int64_t>(e.site);
            p.taken = e.taken;
            p.cond = e.condition;
            p.kind = e.kind == emulate::native_shadow::PathKind::Jump ? concolic::Kind::Jump
                     : e.kind == emulate::native_shadow::PathKind::Inner ? concolic::Kind::Inner
                                                                       : concolic::Kind::Pin;
            ob.path.push_back(p);
        }
        for (const auto& [var_name, var] : made->vars) {
            using Place = concolic::InputVar::Place;
            const std::int64_t value = static_cast<std::int64_t>(var.value);
            if (var.kind == emulate::native_shadow::VarKind::Arg && std::holds_alternative<std::uint64_t>(var.where)) {
                ob.vars[var_name] = {Place::Arg, static_cast<std::int64_t>(std::get<std::uint64_t>(var.where)), var.bits, value};
            } else if (var.kind == emulate::native_shadow::VarKind::Reg && std::holds_alternative<std::string>(var.where)) {
                const std::string& reg = std::get<std::string>(var.where);
                const bool is_ecx = reg == "ecx" || reg == "ECX";
                const bool is_edx = reg == "edx" || reg == "EDX";
                if (is_ecx || is_edx) ob.vars[var_name] = {Place::Reg, is_ecx ? 1 : 2, var.bits, value};
            } else if (var.kind == emulate::native_shadow::VarKind::Ret) {
                ob.vars[var_name] = {Place::Ret, 0, var.bits, value};   // what the imports the emulator answers return
            } else if (var.kind == emulate::native_shadow::VarKind::Mem && std::holds_alternative<std::uint64_t>(var.where)) {
                ob.vars[var_name] = {Place::Mem, static_cast<std::int64_t>(std::get<std::uint64_t>(var.where)), var.bits, value};
            }
        }
        ob.lost = made->lost;
        ob.cut = r->truncated;
        if (r->truncated) ob.lost.insert("truncated");
        return ob;
    };
    const std::function<std::optional<Vector>(const Vector&, const concolic::Values&, const concolic::Vars&)> derived =
        [&](const Vector& v, const concolic::Values& values, const concolic::Vars& names) { return Win32Lane::derive(v, values, names, shape); };
    auto got = std::make_shared<concolic::Exploration<Vector>>(lane_common::explored<Vector>(
        run, std::span<Vector>(seeds), derived, [lo, hi](std::int64_t site) { return static_cast<std::uint64_t>(site) >= lo && static_cast<std::uint64_t>(site) < hi; },
        known));
    std::vector<Vector> kept = got->kept;
    // Where the paths found are not all there are and a jump of the function's own was still seen going one way only, what is left of the
    // guessing explorer's budget goes to it, seeded with every run made.
    std::map<std::int64_t, int> ways;
    for (const concolic::Step& e : got->edges) ways[e.site] |= 1 << static_cast<int>(e.taken);
    const bool one_way = std::ranges::any_of(ways, [](const auto& w) { return w.second != 3; });
    if (!got->complete && one_way && lane_common::kGuidedRest > 0 && !seen.empty()) {
        const guided::Run observe_only = [&](const guided::Input& in) -> std::optional<guided::Observation> {
            const Vector v = Win32Lane::from_guided(in);
            return branch_run(context, func, shape ? shape(v) : v);
        };
        guided::Options options;
        options.budget = lane_common::kGuidedRest;
        options.keep = lane_common::kMaxGuided;
        options.seed = func & 0xFFFF;
        options.observed = seen;
        const std::vector<guided::Input> extra = guided::guided_inputs(observe_only, {}, options);
        for (const guided::Input& g : extra) kept.push_back(Win32Lane::from_guided(g));
        got->guessed = static_cast<int>(extra.size());
    }
    return {std::move(kept), std::static_pointer_cast<concolic::Stats>(got)};
}

std::vector<Vector> directed_and_guided(const Context& context, std::uint64_t func, const std::set<std::int64_t>& reached, const Shape& shape,
                                        std::shared_ptr<concolic::Stats>* found) {
    std::vector<Vector> d;
    {
        const StageTimer timer("judge.directed");
        d = directed_vectors(context, func, reached, shape);
    }
    std::vector<Vector> seeds = Win32Lane::vectors();
    for (Vector& v : Win32Lane::confirm_vectors()) seeds.push_back(std::move(v));
    seeds.insert(seeds.end(), d.begin(), d.end());
    if (lane_common::guided_on() && lane_common::concolic_on() && context.lifter != nullptr) {
        const StageTimer timer("judge.explore (concolic)");
        auto [kept, got] = explore_vectors(context, func, std::move(seeds), shape);
        if (found != nullptr) *found = got;
        d.insert(d.end(), kept.begin(), kept.end());
        return d;
    }
    const StageTimer timer("judge.guided");
    const std::vector<Vector> guessed = guided_vectors(context, func, seeds, shape);
    d.insert(d.end(), guessed.begin(), guessed.end());
    return d;
}

}  // namespace cb::verify::lanes::win32
