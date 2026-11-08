// codebreaker/verify/guided.cpp -- typed C++ port of codebreaker.verify.guided (see the header).

#include "codebreaker/verify/guided.hpp"

#include <algorithm>
#include <map>
#include <string>
#include <vector>

namespace codebreaker::verify::guided {

namespace {

// Python's random.Random (the Mersenne Twister, seeded from an int as CPython does: init_by_array on
// the 32-bit words of the seed), with the three calls guided_inputs makes: random(), randrange(n)
// and choice(seq). The typed runtime has no random module yet; this is the local stand-in.
class Random {
  public:
    explicit Random(int64_t seed) {
        uint64_t mag = seed < 0 ? 0 - static_cast<uint64_t>(seed) : static_cast<uint64_t>(seed);
        std::vector<uint32_t> key;
        if (mag == 0) key.push_back(0);
        while (mag) {
            key.push_back(static_cast<uint32_t>(mag & 0xFFFFFFFFu));
            mag >>= 32;
        }
        init_by_array_(key);
    }

    // random.random(): a float in [0, 1) of 53 random bits
    double random() {
        uint32_t a = genrand_() >> 5, b = genrand_() >> 6;
        return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
    }

    // random.randrange(n) for n > 0
    int64_t randrange(int64_t n) {
        if (n <= 0) throw py::ValueError("empty range for randrange()");
        return randbelow_(n);
    }

    // random.choice(seq)
    template <class T> T choice(const py::list<T>& seq) {
        if (seq.size() == 0) throw py::IndexError("Cannot choose from an empty sequence");
        return seq[randbelow_(seq.size())];
    }

  private:
    static constexpr int N = 624, M = 397;
    uint32_t mt_[N];
    int idx_ = N + 1;

    void init_genrand_(uint32_t s) {
        mt_[0] = s;
        for (int i = 1; i < N; i++) mt_[i] = 1812433253u * (mt_[i - 1] ^ (mt_[i - 1] >> 30)) + static_cast<uint32_t>(i);
        idx_ = N;
    }
    void init_by_array_(const std::vector<uint32_t>& key) {
        init_genrand_(19650218u);
        int i = 1, j = 0;
        int len = static_cast<int>(key.size());
        for (int k = std::max(N, len); k; k--) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525u)) + key[j] + static_cast<uint32_t>(j);
            i++;
            j++;
            if (i >= N) { mt_[0] = mt_[N - 1]; i = 1; }
            if (j >= len) j = 0;
        }
        for (int k = N - 1; k; k--) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941u)) - static_cast<uint32_t>(i);
            i++;
            if (i >= N) { mt_[0] = mt_[N - 1]; i = 1; }
        }
        mt_[0] = 0x80000000u;
        idx_ = N;
    }
    uint32_t genrand_() {
        if (idx_ >= N) {
            static const uint32_t mag01[2] = {0u, 0x9908b0dfu};
            int kk;
            uint32_t y;
            for (kk = 0; kk < N - M; kk++) {
                y = (mt_[kk] & 0x80000000u) | (mt_[kk + 1] & 0x7fffffffu);
                mt_[kk] = mt_[kk + M] ^ (y >> 1) ^ mag01[y & 1];
            }
            for (; kk < N - 1; kk++) {
                y = (mt_[kk] & 0x80000000u) | (mt_[kk + 1] & 0x7fffffffu);
                mt_[kk] = mt_[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 1];
            }
            y = (mt_[N - 1] & 0x80000000u) | (mt_[0] & 0x7fffffffu);
            mt_[N - 1] = mt_[M - 1] ^ (y >> 1) ^ mag01[y & 1];
            idx_ = 0;
        }
        uint32_t y = mt_[idx_++];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680u;
        y ^= (y << 15) & 0xefc60000u;
        y ^= (y >> 18);
        return y;
    }
    // Random._randbelow_with_getrandbits (n < 2**32 here)
    int64_t randbelow_(int64_t n) {
        int64_t k = py::bit_length(n);
        int64_t r = genrand_() >> (32 - k);
        while (r >= n) r = genrand_() >> (32 - k);
        return r;
    }
};

uint64_t mask_of_(int64_t size) { return size >= 8 ? ~0ULL : (size <= 0 ? 0ULL : (1ULL << (8 * size)) - 1); }
int64_t wrap_(uint64_t v) { return static_cast<int64_t>(v); }

// int.to_bytes(size, order), with the ValueError for a byteorder that is neither
py::bytes to_bytes_(int64_t value, int64_t size, const py::str& order) {
    if (order != py::str("little") && order != py::str("big")) throw py::ValueError("byteorder must be either 'little' or 'big'");
    return py::to_bytes(value, size, order.utf8().c_str());
}

// the key the Python dedupes tried inputs by: repr(sorted(v.items()))
py::str _key(const Input& v) {
    std::string s = "args=";
    for (int64_t x : v.args) s += std::to_string(x) + ",";
    if (v.mem_init) {
        s += ";mem_init=";
        for (const auto& m : *v.mem_init) {
            s += std::to_string(std::get<0>(m)) + ":" + std::to_string(std::get<1>(m).raw().size()) + "#";
            s += std::get<1>(m).raw();
            s += "|";
        }
    }
    if (v.stub_ret) s += ";stub_ret=" + std::to_string(*v.stub_ret);
    std::vector<std::pair<py::str, Extra>> ex;
    for (const auto& kv : v.extra.items()) ex.emplace_back(std::get<0>(kv), std::get<1>(kv));
    std::sort(ex.begin(), ex.end(), [](const auto& a, const auto& b) { return a.first.u32() < b.first.u32(); });
    for (const auto& kv : ex) {
        s += ";" + kv.first.utf8() + "=";
        if (std::holds_alternative<int64_t>(kv.second)) s += std::to_string(std::get<int64_t>(kv.second));
        else
            for (int64_t x : std::get<py::list<int64_t>>(kv.second)) s += std::to_string(x) + ",";
    }
    return py::str(s);
}

}  // namespace

const py::list<int64_t> INTERESTING = {0, 1, 2, 0xFFFFFFFF, 0x7FFFFFFF, 0x80000000, 0xFF, 0x100, 0xFFFF, 0x10000, 0x40, 0x8000};

py::set<std::tuple<int64_t, bool>> _edges(const Observation& obs) {
    py::set<std::tuple<int64_t, bool>> out;
    for (const auto& br : obs.branches) out.add({std::get<0>(br), std::get<1>(br)});
    return out;
}

Input _copy(const Input& v) {
    Input out = v;
    out.args = v.args.copy();
    if (v.mem_init) out.mem_init = v.mem_init->copy();
    return out;
}

py::list<std::tuple<int64_t, int64_t, int64_t>> _flips(const Compare& cmp) {
    const py::str& mn = cmp.mn;
    int64_t size = cmp.size, a = cmp.a, b = cmp.b;
    uint64_t mask = mask_of_(size);
    py::list<std::tuple<int64_t, int64_t, int64_t>> out;
    bool is_test_and_or = mn == py::str("test") || mn == py::str("and") || mn == py::str("or");
    if (is_test_and_or && a == b) {                                    // a zero test: zero or not
        out.append({0, a, a == 0 ? 1 : 0});
        return out;
    }
    if (mn == py::str("test") || mn == py::str("and")) {                // a flag test: the bits, set or clear
        out.append({0, a, wrap_((static_cast<uint64_t>(a) | static_cast<uint64_t>(b)) & mask)});
        out.append({0, a, wrap_((static_cast<uint64_t>(a) & ~static_cast<uint64_t>(b)) & mask)});
        out.append({1, b, wrap_((static_cast<uint64_t>(b) | static_cast<uint64_t>(a)) & mask)});
        return out;
    }
    if (mn == py::str("dec") || mn == py::str("inc") || mn == py::str("add") || mn == py::str("xor") || mn == py::str("or")) {
        int64_t first = mn != py::str("inc") ? b : -b;
        out.append({0, a, wrap_(static_cast<uint64_t>(first) & mask)});
        out.append({0, a, 0});
        return out;
    }
    const int64_t seen_other[2][2] = {{a, b}, {b, a}};
    for (int64_t k = 0; k < 2; k++) {
        int64_t seen = seen_other[k][0], other = seen_other[k][1];
        const int64_t wants[3] = {other, other + 1, other - 1};
        for (int64_t want0 : wants) {
            int64_t want = wrap_(static_cast<uint64_t>(want0) & mask);
            if (want != seen) out.append({k, seen, want});
        }
    }
    return out;
}

_Machine::_Machine(const py::str& byteorder, int64_t word_) : order(byteorder), word(word_) {
    wmask = mask_of_(word_);
}

py::list<Place> _places(const Input& v, const Observation& obs, const Compare& cmp, int64_t k, int64_t value,
                        int64_t size, const _Machine& mach) {
    py::list<Place> found;
    std::optional<int64_t> at = cmp.has_at ? (k == 0 ? cmp.at_a : cmp.at_b) : std::nullopt;
    const std::optional<int64_t>& args_at = obs.args_at;
    std::tuple<int64_t, int64_t> stack = obs.stack ? *obs.stack : std::tuple<int64_t, int64_t>(0, 0);
    if (at) {
        int64_t w = mach.word;
        if (args_at && *args_at <= *at && *at < *args_at + w * 32 && py::mod(*at - *args_at, w) + size <= w) {
            int64_t byte = py::mod(*at - *args_at, w);
            found.append({PlaceKind::arg, py::floordiv(*at - *args_at, w),
                          mach.order == py::str("little") ? byte : w - size - byte});
        } else if (!(std::get<0>(stack) <= *at && *at < std::get<1>(stack))) {   // (not the function's own frame)
            found.append({PlaceKind::addr, *at, 0});
        }
    }
    uint64_t mask = mask_of_(size);
    int64_t i = 0;
    for (int64_t x : v.args) {
        if ((static_cast<uint64_t>(x) & mask) == static_cast<uint64_t>(value)) found.append({PlaceKind::arg, i, 0});
        i++;
    }
    if (v.stub_ret && (static_cast<uint64_t>(*v.stub_ret) & mask) == static_cast<uint64_t>(value)) {
        found.append({PlaceKind::stub_ret, 0, 0});
    }
    if (value && (size == 1 || size == 2 || size == 4)) {
        py::bytes needle = to_bytes_(value, size, mach.order);
        if (v.mem_init) {
            int64_t j = 0;
            for (const auto& block : *v.mem_init) {
                const py::bytes& data = std::get<1>(block);
                int64_t pos = data.find(needle);
                int64_t hits = 0;
                while (pos >= 0 && hits < 2) {
                    found.append({PlaceKind::mem, j, pos});
                    hits++;
                    pos = data.find(needle, pos + 1);
                }
                j++;
            }
        }
    }
    py::list<Place> out;
    for (const Place& f : found) {
        if (!out.contains(f)) out.append(f);
    }
    return out.slice(0, 6);
}

Input _put(const Input& v, const Place& place, int64_t value, int64_t size, const _Machine& mach) {
    Input out = _copy(v);
    uint64_t mask = mask_of_(size);
    uint64_t val = static_cast<uint64_t>(value);
    switch (place.kind) {
        case PlaceKind::arg: {
            int64_t i = place.a, byte = place.b;
            while (out.args.size() <= i) out.args.append(0);
            uint64_t shift = static_cast<uint64_t>(8 * byte);
            uint64_t cur = static_cast<uint64_t>(out.args[i]);
            out.args[i] = wrap_((cur & ~(mask << shift) & mach.wmask) | ((val & mask) << shift));
            break;
        }
        case PlaceKind::stub_ret:
            out.stub_ret = wrap_((static_cast<uint64_t>(out.stub_ret ? *out.stub_ret : 0) & ~mask & mach.wmask) | (val & mask));
            break;
        case PlaceKind::addr: {
            py::list<std::tuple<int64_t, py::bytes>> mi = out.mem_init ? *out.mem_init : py::list<std::tuple<int64_t, py::bytes>>();
            mi = mi.copy();
            mi.append({place.a, to_bytes_(wrap_(val & mask), size, mach.order)});
            out.mem_init = mi;
            break;
        }
        case PlaceKind::mem: {
            int64_t j = place.a, at = place.b;
            auto [addr, data] = (*out.mem_init)[j];
            std::string d = data.raw();                                  // a bytearray
            std::string repl = to_bytes_(wrap_(val & mask), size, mach.order).raw();
            size_t lo = static_cast<size_t>(std::min<int64_t>(at, static_cast<int64_t>(d.size())));
            size_t hi = static_cast<size_t>(std::min<int64_t>(at + size, static_cast<int64_t>(d.size())));
            d.replace(lo, hi - lo, repl);                                // data[at:at + size] = ...
            (*out.mem_init)[j] = {addr, py::bytes(d)};
            break;
        }
    }
    return out;
}

py::list<Input> guided_inputs(const Run& run, const py::list<Input>& seeds, int64_t budget, int64_t keep, int64_t seed,
                              const py::str& byteorder, int64_t word,
                              const py::list<std::tuple<Input, Observation>>& observed) {
    Random rng(seed);
    _Machine mach(byteorder, word);
    py::set<std::tuple<int64_t, bool>> seen_edges;
    py::set<int64_t> seen_reach;
    py::list<std::tuple<Input, Observation>> pool;            // (input, its observation) -- seeds and kept inputs
    for (const auto& vo : observed) {
        const Observation& o = std::get<1>(vo);
        seen_edges.update(_edges(o));
        seen_reach.update(o.reach);
        pool.append(vo);
    }
    for (const Input& v : seeds) {
        std::optional<Observation> o = run(v);
        if (!o) continue;
        seen_edges.update(_edges(*o));
        seen_reach.update(o->reach);
        pool.append({v, *o});
    }
    if (pool.size() == 0) return py::list<Input>();
    py::list<Input> kept;
    py::set<py::str> tried;
    int64_t spent = 0;

    auto attempt = [&](const Input& v) -> bool {
        py::str k = _key(v);
        if (tried.contains(k) || spent >= budget) return false;
        tried.add(k);
        spent += 1;
        std::optional<Observation> o = run(v);
        if (!o) return false;
        py::set<std::tuple<int64_t, bool>> new_e;
        for (const auto& e : _edges(*o)) {
            if (!seen_edges.contains(e)) new_e.add(e);
        }
        py::set<int64_t> new_r;
        for (int64_t a : o->reach) {
            if (!seen_reach.contains(a)) new_r.add(a);
        }
        if (new_e.empty() && new_r.empty()) return false;
        seen_edges.update(new_e);
        seen_reach.update(new_r);
        kept.append(v);
        pool.append({v, *o});
        return true;
    };

    bool progress = true;
    while (progress && spent < budget && kept.size() < keep) {
        progress = false;
        // frontier(): jumps seen going one way only, each with the inputs (and comparisons) that reached it
        std::map<int64_t, py::set<bool>> taken;
        for (const auto& e : seen_edges) taken[std::get<0>(e)].add(std::get<1>(e));
        std::map<int64_t, std::vector<std::tuple<int64_t, std::optional<Compare>>>> sites_of;   // jump -> (pool index, compare)
        for (int64_t pi = 0; pi < pool.size(); pi++) {
            const Observation& o = std::get<1>(pool[pi]);
            for (const auto& br : o.branches) {
                auto it = taken.find(std::get<0>(br));
                if (it != taken.end() && it->second.size() == 1) {
                    sites_of[std::get<0>(br)].emplace_back(pi, std::get<2>(br));
                }
            }
        }
        for (const auto& kv : sites_of) {                         // sorted by jump address
            if (spent >= budget || kept.size() >= keep) break;
            // the first few inputs that reached this jump, each with what it compared
            for (size_t si = 0; si < kv.second.size() && si < 3; si++) {
                Input v = std::get<0>(pool[std::get<0>(kv.second[si])]);
                Observation o = std::get<1>(pool[std::get<0>(kv.second[si])]);
                const std::optional<Compare>& c = std::get<1>(kv.second[si]);
                if (!c) continue;
                int64_t size = c->size;
                bool done = false;
                for (const auto& flip : _flips(*c)) {
                    int64_t k = std::get<0>(flip), was = std::get<1>(flip), want = std::get<2>(flip);
                    py::list<Place> places = _places(v, o, *c, k, was, size, mach);
                    if (places.size() == 0) {                       // not in the input as it is: try each argument
                        for (int64_t i = 0; i < v.args.size(); i++) places.append({PlaceKind::arg, i, 0});
                    }
                    for (const Place& place : places) {
                        if (attempt(_put(v, place, want, size, mach))) {
                            progress = done = true;
                            break;
                        }
                    }
                    if (done || spent >= budget) break;
                }
                if (done) break;
            }
        }
        if (!progress && spent < budget) {
            // nothing above opened anything: fixed changes to inputs that are known to run
            const int64_t tries = std::min<int64_t>(12, budget - spent);      // (range() is evaluated once)
            for (int64_t n = 0; n < tries; n++) {
                Input w = _copy(std::get<0>(pool[rng.randrange(pool.size())]));
                double r = rng.random();
                if (r < 0.25) {
                    w.stub_ret = rng.choice(py::list<int64_t>{0, 1, wrap_(mach.wmask), 2});
                } else if (w.args.size() != 0) {
                    int64_t i = rng.randrange(w.args.size());
                    w.args[i] = r < 0.75 ? rng.choice(INTERESTING)
                                         : (w.args[i] ^ (int64_t(1) << rng.randrange(8 * word)));
                }
                if (attempt(w)) progress = true;
                if (kept.size() >= keep) break;
            }
        }
    }
    return kept;
}

}  // namespace codebreaker::verify::guided
