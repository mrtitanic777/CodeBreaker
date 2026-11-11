// cb/verify/guided.cpp -- see guided.hpp.

#include "cb/verify/guided.hpp"

#include <algorithm>
#include <array>
#include <deque>
#include <set>

#include "cb/core/error.hpp"

namespace cb::verify::guided {

namespace {

constexpr std::size_t kMaxPlaces = 6;          // places tried for one operand
constexpr std::uint64_t kArgSlotsScanned = 32; // words from the first argument that count as "an argument"
constexpr int kFixedTries = 12;                // fixed changes tried per round when nothing else opened a branch
constexpr std::size_t kSitesPerJump = 3;       // inputs that reached a one-sided jump, each tried
constexpr std::size_t kMemHitsPerBlock = 2;    // places in one memory block that hold a value

// The low `size` bytes (size 1..8).
constexpr std::uint64_t mask_of(unsigned size) {
    return size >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * size)) - 1;
}

// SplitMix64: a tiny generator with a fixed definition, so a run is reproducible everywhere.
class Rng {
public:
    explicit Rng(std::uint64_t seed) : state_(seed) {}
    std::uint64_t next() {
        std::uint64_t z = (state_ += 0x9E3779B97F4A7C15ull);
        z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
        z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
        return z ^ (z >> 31);
    }
    std::size_t below(std::size_t n) { return static_cast<std::size_t>(next() % n); } // n > 0; the bias is negligible
    double unit() { return static_cast<double>(next() >> 11) * (1.0 / 9007199254740992.0); } // [0, 1)
private:
    std::uint64_t state_;
};

// `value` as `size` bytes in the machine's byte order.
Bytes encode(std::uint64_t value, unsigned size, ByteOrder order) {
    Bytes out(size);
    for (unsigned i = 0; i < size; ++i) {
        const unsigned shift = 8 * (order == ByteOrder::Little ? i : size - 1 - i);
        out[i] = static_cast<std::uint8_t>(value >> shift);
    }
    return out;
}

std::set<std::pair<std::uint64_t, bool>> edges_of(const Observation& obs) {
    std::set<std::pair<std::uint64_t, bool>> out;
    for (const Branch& b : obs.branches)
        out.emplace(b.address, b.taken);
    return out;
}

} // namespace

const std::array<std::uint64_t, 12> kInteresting = {0,    1,     2,      0xFFFFFFFF, 0x7FFFFFFF, 0x80000000,
                                                    0xFF, 0x100, 0xFFFF, 0x10000,    0x40,       0x8000};

CompareOp parse_compare_op(std::string_view m) {
    if (m == "test")
        return CompareOp::Test;
    if (m == "and")
        return CompareOp::And;
    if (m == "or")
        return CompareOp::Or;
    if (m == "dec")
        return CompareOp::Dec;
    if (m == "inc")
        return CompareOp::Inc;
    if (m == "add")
        return CompareOp::Add;
    if (m == "xor")
        return CompareOp::Xor;
    return CompareOp::Other;
}

std::vector<Flip> flips(const Compare& cmp) {
    const std::uint64_t mask = mask_of(cmp.size);
    const std::uint64_t a = cmp.a, b = cmp.b;
    switch (cmp.op) {
    case CompareOp::Test:
    case CompareOp::And:
    case CompareOp::Or:
        if (a == b)
            return {{0, a, a == 0 ? 1u : 0u}}; // a zero test: zero or not
        break;
    default:
        break;
    }
    if (cmp.op == CompareOp::Test || cmp.op == CompareOp::And) // a flag test: the bits, set or clear
        return {{0, a, (a | b) & mask}, {0, a, (a & ~b) & mask}, {1, b, (b | a) & mask}};
    switch (cmp.op) {
    case CompareOp::Dec:
    case CompareOp::Inc:
    case CompareOp::Add:
    case CompareOp::Xor:
    case CompareOp::Or:
        return {{0, a, (cmp.op != CompareOp::Inc ? b : std::uint64_t{0} - b) & mask}, {0, a, 0}};
    default:
        break;
    }
    std::vector<Flip> out;
    for (int k = 0; k < 2; ++k) {
        const std::uint64_t seen = k == 0 ? a : b, other = k == 0 ? b : a;
        for (std::uint64_t want : {other, other + 1, other - 1}) {
            want &= mask;
            if (want != seen)
                out.push_back({k, seen, want});
        }
    }
    return out;
}

std::vector<Place> places(const Input& v, const Observation& obs, const Compare& cmp, int operand, std::uint64_t value,
                          unsigned size, const Machine& machine) {
    std::vector<Place> found;
    const std::optional<std::uint64_t> at = operand == 0 ? cmp.a_at : cmp.b_at;
    const std::uint64_t w = machine.word;
    if (at) {
        if (obs.args_at && *obs.args_at <= *at && *at < *obs.args_at + w * kArgSlotsScanned &&
            (*at - *obs.args_at) % w + size <= w) {
            const std::uint64_t byte = (*at - *obs.args_at) % w;
            found.push_back({PlaceKind::Arg, (*at - *obs.args_at) / w,
                             machine.order == ByteOrder::Little ? byte : w - size - byte});
        } else if (!(obs.stack_low <= *at && *at < obs.stack_high)) { // (not the function's own frame)
            found.push_back({PlaceKind::Addr, *at, 0});
        }
    }
    const std::uint64_t mask = mask_of(size);
    for (std::size_t i = 0; i < v.args.size(); ++i)
        if ((v.args[i] & mask) == value)
            found.push_back({PlaceKind::Arg, i, 0});
    if (v.stub_ret && (*v.stub_ret & mask) == value)
        found.push_back({PlaceKind::StubRet, 0, 0});
    if (value != 0 && (size == 1 || size == 2 || size == 4)) {
        const Bytes needle = encode(value, size, machine.order);
        const std::vector<MemInit> none;
        const std::vector<MemInit>& blocks = v.mem_init ? *v.mem_init : none;
        for (std::size_t j = 0; j < blocks.size(); ++j) {
            const Bytes& data = blocks[j].data;
            std::size_t hits = 0;
            auto pos = std::search(data.begin(), data.end(), needle.begin(), needle.end());
            while (pos != data.end() && hits < kMemHitsPerBlock) {
                found.push_back({PlaceKind::Mem, j, static_cast<std::uint64_t>(pos - data.begin())});
                ++hits;
                pos = std::search(pos + 1, data.end(), needle.begin(), needle.end());
            }
        }
    }
    std::vector<Place> out; // the first six, each once
    for (const Place& f : found)
        if (std::find(out.begin(), out.end(), f) == out.end())
            out.push_back(f);
    if (out.size() > kMaxPlaces)
        out.resize(kMaxPlaces);
    return out;
}

Input put(const Input& v, const Place& place, std::uint64_t value, unsigned size, const Machine& machine) {
    Input out = v;
    const std::uint64_t mask = mask_of(size);
    const std::uint64_t wmask = mask_of(machine.word);
    switch (place.kind) {
    case PlaceKind::Arg: {
        CB_ASSERT(place.offset < 8, "a byte offset inside an argument word");
        if (out.args.size() <= place.index)
            out.args.resize(static_cast<std::size_t>(place.index) + 1, 0);
        const unsigned shift = static_cast<unsigned>(8 * place.offset);
        std::uint64_t& word = out.args[static_cast<std::size_t>(place.index)];
        word = (word & ~(mask << shift) & wmask) | ((value & mask) << shift);
        break;
    }
    case PlaceKind::StubRet:
        out.stub_ret = (out.stub_ret.value_or(0) & ~mask & wmask) | (value & mask);
        break;
    case PlaceKind::Addr:
        if (!out.mem_init)
            out.mem_init.emplace();
        out.mem_init->push_back({place.index, encode(value, size, machine.order)});
        break;
    case PlaceKind::Mem: {
        CB_ASSERT(out.mem_init && place.index < out.mem_init->size(),
                  "a place in a memory block the input does not set");
        Bytes& data = (*out.mem_init)[static_cast<std::size_t>(place.index)].data;
        CB_ASSERT(place.offset <= data.size(), "a place past the end of its memory block");
        const Bytes piece = encode(value, size, machine.order);
        // `places` only finds values that lie wholly inside the block; written wider than what is left, the block grows
        const std::size_t end = static_cast<std::size_t>(place.offset) + piece.size();
        if (end > data.size())
            data.resize(end, 0);
        std::copy(piece.begin(), piece.end(), data.begin() + static_cast<std::ptrdiff_t>(place.offset));
        break;
    }
    }
    return out;
}

std::vector<Input> guided_inputs(const Run& run, std::span<const Input> seeds, const Options& options) {
    Rng rng(options.seed);
    const Machine& machine = options.machine;
    const std::uint64_t wmask = mask_of(machine.word);

    std::set<std::pair<std::uint64_t, bool>> seen_edges;
    std::unordered_set<std::uint64_t> seen_reach;
    std::deque<Seen> pool; // seeds and kept inputs; a deque so references stay valid as inputs are added

    auto learn = [&](const Observation& o) {
        for (const auto& e : edges_of(o))
            seen_edges.insert(e);
        seen_reach.insert(o.reach.begin(), o.reach.end());
    };
    for (const Seen& s : options.observed) {
        learn(s.observation);
        pool.push_back(s);
    }
    for (const Input& v : seeds) {
        std::optional<Observation> o = run(v);
        if (!o)
            continue;
        learn(*o);
        pool.push_back({v, std::move(*o)});
    }
    if (pool.empty())
        return {};

    std::vector<Input> kept;
    std::set<Input> tried;
    int spent = 0;
    const auto budget_left = [&] { return spent < options.budget; };
    const auto room = [&] { return kept.size() < static_cast<std::size_t>(options.keep); };

    // Run `v`; keep it if the original now takes a direction or runs an instruction no input did before.
    auto attempt = [&](const Input& v) {
        if (!budget_left() || !tried.insert(v).second)
            return false;
        ++spent;
        std::optional<Observation> o = run(v);
        if (!o)
            return false;
        bool fresh = false;
        for (const auto& e : edges_of(*o))
            fresh |= seen_edges.insert(e).second;
        for (std::uint64_t a : o->reach)
            fresh |= seen_reach.insert(a).second;
        if (!fresh)
            return false;
        kept.push_back(v);
        pool.push_back({v, std::move(*o)});
        return true;
    };

    // A jump seen going one way only, with the inputs (and comparisons) that reached it.
    struct Site {
        const Seen* seen;
        const Compare* compare;
    };
    auto frontier = [&] {
        std::map<std::uint64_t, std::set<bool>> taken;
        for (const auto& [a, t] : seen_edges)
            taken[a].insert(t);
        std::map<std::uint64_t, std::vector<Site>> out; // by address: the order the jumps are tried in
        for (const Seen& s : pool)
            for (const Branch& b : s.observation.branches)
                if (taken[b.address].size() == 1)
                    out[b.address].push_back({&s, b.compare ? &*b.compare : nullptr});
        return out;
    };

    bool progress = true;
    while (progress && budget_left() && room()) {
        progress = false;
        for (const auto& [address, sites] : frontier()) {
            (void)address;
            if (!budget_left() || !room())
                break;
            // the first few inputs that reached this jump, each with what it compared
            for (std::size_t s = 0; s < std::min(sites.size(), kSitesPerJump); ++s) {
                const Seen& at = *sites[s].seen;
                const Compare* c = sites[s].compare;
                if (!c)
                    continue;
                const Input& v = at.input;
                bool done = false;
                for (const Flip& f : flips(*c)) {
                    std::vector<Place> where = places(v, at.observation, *c, f.operand, f.seen, c->size, machine);
                    if (where.empty()) // not in the input as it is: try each argument
                        for (std::size_t i = 0; i < v.args.size(); ++i)
                            where.push_back({PlaceKind::Arg, i, 0});
                    for (const Place& place : where) {
                        if (attempt(put(v, place, f.wanted, c->size, machine))) {
                            progress = done = true;
                            break;
                        }
                    }
                    if (done || !budget_left())
                        break;
                }
                if (done)
                    break;
            }
        }
        if (!progress && budget_left()) {
            // nothing above opened anything: fixed changes to inputs that are known to run
            for (int n = 0; n < std::min(kFixedTries, options.budget - spent); ++n) {
                Input w = pool[rng.below(pool.size())].input;
                const double r = rng.unit();
                if (r < 0.25) {
                    const std::array<std::uint64_t, 4> returns = {0, 1, wmask, 2};
                    w.stub_ret = returns[rng.below(returns.size())];
                } else if (!w.args.empty()) {
                    std::uint64_t& arg = w.args[rng.below(w.args.size())];
                    arg = r < 0.75 ? kInteresting[rng.below(kInteresting.size())]
                                   : arg ^ (std::uint64_t{1} << rng.below(8 * machine.word));
                }
                if (attempt(w))
                    progress = true;
                if (!room())
                    break;
            }
        }
    }
    return kept;
}

} // namespace cb::verify::guided
