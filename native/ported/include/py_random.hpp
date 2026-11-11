// py_random.hpp -- Python's random module: CPython's Mersenne Twister and the methods built on it, exact for a seed
// (the same sequence as `random.Random(seed)` in CPython 3.14).
//
//   py::random::Random rng(seed)          seed: int (any sign), or a str / bytes (version 2: sha512 appended)
//   rng.random()  getrandbits(k)  randbytes(n)  randrange(...)  randint  choice  choices  shuffle  sample  uniform
//   py::random::seed(x), py::random::random() ... the module-level functions on one shared instance
//
// getrandbits(k) returns an int64: k above 63 is an OverflowError (use getrandbits_bytes for more); the ints are
// int64, so randrange's arguments are.
#pragma once

#include <cmath>
#include <random>
#include <vector>

#include "py.hpp"

namespace py {
namespace random {

class Random {
  public:
    Random() { seed(); }
    explicit Random(int64_t s) { seed(s); }
    explicit Random(const str& s) { seed(s); }
    explicit Random(const bytes& b) { seed(b); }

    // seed(): from the system's entropy
    void seed() {
        std::random_device rd;
        std::vector<uint32_t> key;
        for (int i = 0; i < 16; i++) key.push_back(rd());
        init_by_array_(key);
    }
    // seed(int): init_by_array over the 32-bit words of abs(seed), least significant first
    void seed(int64_t s) {
        uint64_t a = s < 0 ? 0 - static_cast<uint64_t>(s) : static_cast<uint64_t>(s);
        std::vector<uint32_t> key;
        while (a) {
            key.push_back(static_cast<uint32_t>(a & 0xFFFFFFFFu));
            a >>= 32;
        }
        if (key.empty()) key.push_back(0);
        init_by_array_(key);
    }
    // seed(bytes): version 2: int.from_bytes(a + sha512(a).digest(), 'big')
    void seed(const bytes& b) {
        std::string all = b.raw() + hashlib::sha512(b).digest().raw();
        size_t top = 0;
        while (top < all.size() && all[top] == 0) top++;   // (leading zero bytes are not part of the number)
        std::string n = all.substr(top);
        std::vector<uint32_t> key;
        for (size_t end = n.size(); end > 0;) {
            size_t start = end >= 4 ? end - 4 : 0;
            uint32_t w = 0;
            for (size_t i = start; i < end; i++) w = (w << 8) | static_cast<unsigned char>(n[i]);
            key.push_back(w);
            end = start;
        }
        if (key.empty()) key.push_back(0);
        init_by_array_(key);
    }
    void seed(const str& s) { seed(s.encode("utf-8")); }

    uint32_t genrand_uint32() {
        static const uint32_t mag01[2] = {0u, 0x9908b0dfu};
        uint32_t y;
        if (mti_ >= N) {
            int kk;
            for (kk = 0; kk < N - M; kk++) {
                y = (mt_[kk] & 0x80000000u) | (mt_[kk + 1] & 0x7fffffffu);
                mt_[kk] = mt_[kk + M] ^ (y >> 1) ^ mag01[y & 1u];
            }
            for (; kk < N - 1; kk++) {
                y = (mt_[kk] & 0x80000000u) | (mt_[kk + 1] & 0x7fffffffu);
                mt_[kk] = mt_[kk + (M - N)] ^ (y >> 1) ^ mag01[y & 1u];
            }
            y = (mt_[N - 1] & 0x80000000u) | (mt_[0] & 0x7fffffffu);
            mt_[N - 1] = mt_[M - 1] ^ (y >> 1) ^ mag01[y & 1u];
            mti_ = 0;
        }
        y = mt_[mti_++];
        y ^= (y >> 11);
        y ^= (y << 7) & 0x9d2c5680u;
        y ^= (y << 15) & 0xefc60000u;
        y ^= (y >> 18);
        return y;
    }

    double random() {
        uint32_t a = genrand_uint32() >> 5, b = genrand_uint32() >> 6;
        return (a * 67108864.0 + b) * (1.0 / 9007199254740992.0);
    }

    int64_t getrandbits(int64_t k) {
        if (k < 0) throw ValueError("Cannot convert negative int");
        if (k > 63) throw OverflowError("getrandbits: more than 63 bits does not fit an int64");
        if (k == 0) return 0;
        if (k <= 32) return static_cast<int64_t>(genrand_uint32() >> (32 - k));
        uint64_t out = 0;
        int shift = 0;
        int64_t left = k;
        while (left > 0) {
            uint32_t r = genrand_uint32();
            if (left < 32) r >>= (32 - left);
            out |= static_cast<uint64_t>(r) << shift;
            shift += 32;
            left -= 32;
        }
        return static_cast<int64_t>(out);
    }
    // getrandbits(k) as the little-endian bytes of the number (ceil(k/8) of them): any k
    bytes getrandbits_bytes(int64_t k) {
        if (k < 0) throw ValueError("Cannot convert negative int");
        std::string out;
        int64_t left = k;
        while (left > 0) {
            uint32_t r = genrand_uint32();
            if (left < 32) r >>= (32 - left);
            for (int i = 0; i < 4; i++) out.push_back(static_cast<char>((r >> (8 * i)) & 0xFF));
            left -= 32;
        }
        out.resize(static_cast<size_t>((k + 7) / 8));
        return bytes(out);
    }
    // randbytes(n): getrandbits(n * 8).to_bytes(n, 'little')
    bytes randbytes(int64_t n) {
        if (n < 0) throw ValueError("Cannot convert negative int");
        return getrandbits_bytes(n * 8);
    }

    // _randbelow_with_getrandbits(n)
    int64_t _randbelow(int64_t n) {
        if (n <= 0) return 0;
        int64_t k = 0;
        for (int64_t t = n; t; t >>= 1) k++;   // n.bit_length()
        int64_t r = getrandbits(k);
        while (r >= n) r = getrandbits(k);
        return r;
    }

    // randrange(stop) / randrange(start, stop[, step])
    int64_t randrange(int64_t stop) {
        if (stop > 0) return _randbelow(stop);
        throw ValueError("empty range for randrange()");
    }
    int64_t randrange(int64_t start, int64_t stop, int64_t step = 1) {
        int64_t width = stop - start;
        int64_t n;
        if (step == 1) {
            if (width > 0) return start + _randbelow(width);
            throw ValueError("empty range in randrange(" + std::to_string(start) + ", " + std::to_string(stop) + ")");
        }
        if (step > 0) n = floordiv(width + step - 1, step);
        else if (step < 0) n = floordiv(width + step + 1, step);
        else throw ValueError("zero step for randrange()");
        if (n <= 0)
            throw ValueError("empty range in randrange(" + std::to_string(start) + ", " + std::to_string(stop) + ", " + std::to_string(step) + ")");
        return start + step * _randbelow(n);
    }
    int64_t randint(int64_t a, int64_t b) {
        if (b < a) throw ValueError("empty range in randint(" + std::to_string(a) + ", " + std::to_string(b) + ")");
        return randrange(a, b + 1);
    }

    double uniform(double a, double b) { return a + (b - a) * random(); }

    template <class T> T choice(const list<T>& seq) {
        if (seq.empty()) throw IndexError("Cannot choose from an empty sequence");
        return seq[_randbelow(seq.size())];
    }
    template <class T> void shuffle(list<T>& x) {
        for (int64_t i = x.size() - 1; i >= 1; i--) {
            int64_t j = _randbelow(i + 1);
            T t = x[i];
            x[i] = x[j];
            x[j] = t;
        }
    }
    template <class T> list<T> sample(const list<T>& population, int64_t k) {
        int64_t n = population.size();
        if (!(0 <= k && k <= n)) throw ValueError("Sample larger than population or is negative");
        std::vector<T> result;
        int64_t setsize = 21;   // size of a small set minus size of an empty list
        if (k > 5) setsize += static_cast<int64_t>(std::pow(4.0, std::ceil(std::log(static_cast<double>(k * 3)) / std::log(4.0))));
        if (n <= setsize) {
            std::vector<T> pool = population.vec();
            for (int64_t i = 0; i < k; i++) {
                int64_t j = _randbelow(n - i);
                result.push_back(pool[static_cast<size_t>(j)]);
                pool[static_cast<size_t>(j)] = pool[static_cast<size_t>(n - i - 1)];
            }
        } else {
            std::vector<char> selected(static_cast<size_t>(n), 0);
            for (int64_t i = 0; i < k; i++) {
                int64_t j = _randbelow(n);
                while (selected[static_cast<size_t>(j)]) j = _randbelow(n);
                selected[static_cast<size_t>(j)] = 1;
                result.push_back(population[j]);
            }
        }
        list<T> out;
        for (const T& v : result) out.append(v);
        return out;
    }
    // choices(population, k=k): with replacement, equal weights
    template <class T> list<T> choices(const list<T>& population, int64_t k = 1) {
        int64_t n = population.size();
        list<T> out;
        for (int64_t i = 0; i < k; i++) {
            int64_t j = static_cast<int64_t>(std::floor(random() * static_cast<double>(n)));
            if (j >= n) throw IndexError("list index out of range");   // (population[0] of an empty list)
            out.append(population[j]);
        }
        return out;
    }
    // choices(population, weights=weights, k=k)
    template <class T> list<T> choices(const list<T>& population, const list<double>& weights, int64_t k = 1) {
        int64_t n = population.size();
        if (n != weights.size()) throw ValueError("The number of weights does not match the population");
        std::vector<double> cum;
        double acc = 0;
        for (double w : weights) cum.push_back(acc += w);
        double total = cum.empty() ? 0.0 : cum.back();
        if (!(total > 0.0)) throw ValueError("Total of weights must be greater than zero");
        if (!std::isfinite(total)) throw ValueError("Total of weights must be finite");
        int64_t hi = n - 1;
        list<T> out;
        for (int64_t i = 0; i < k; i++) {
            double x = random() * total;
            int64_t lo = 0, h = hi;   // bisect(cum_weights, x, 0, hi)
            while (lo < h) {
                int64_t mid = (lo + h) / 2;
                if (x < cum[static_cast<size_t>(mid)]) h = mid;
                else lo = mid + 1;
            }
            out.append(population[lo]);
        }
        return out;
    }

  private:
    static constexpr int N = 624, M = 397;
    uint32_t mt_[N];
    int mti_ = N + 1;

    void init_genrand_(uint32_t s) {
        mt_[0] = s;
        for (mti_ = 1; mti_ < N; mti_++) mt_[mti_] = 1812433253u * (mt_[mti_ - 1] ^ (mt_[mti_ - 1] >> 30)) + static_cast<uint32_t>(mti_);
    }
    void init_by_array_(const std::vector<uint32_t>& key) {
        init_genrand_(19650218u);
        size_t i = 1, j = 0;
        for (size_t k = N > key.size() ? N : key.size(); k; k--) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1664525u)) + key[j] + static_cast<uint32_t>(j);
            i++;
            j++;
            if (i >= N) {
                mt_[0] = mt_[N - 1];
                i = 1;
            }
            if (j >= key.size()) j = 0;
        }
        for (size_t k = N - 1; k; k--) {
            mt_[i] = (mt_[i] ^ ((mt_[i - 1] ^ (mt_[i - 1] >> 30)) * 1566083941u)) - static_cast<uint32_t>(i);
            i++;
            if (i >= N) {
                mt_[0] = mt_[N - 1];
                i = 1;
            }
        }
        mt_[0] = 0x80000000u;
        mti_ = N;
    }
};

// the module-level functions (random.random() ...): one shared generator
inline Random& instance() {
    static Random r;
    return r;
}
inline void seed() { instance().seed(); }
inline void seed(int64_t s) { instance().seed(s); }
inline double random() { return instance().random(); }
inline int64_t getrandbits(int64_t k) { return instance().getrandbits(k); }
inline int64_t randrange(int64_t stop) { return instance().randrange(stop); }
inline int64_t randrange(int64_t start, int64_t stop, int64_t step = 1) { return instance().randrange(start, stop, step); }
inline int64_t randint(int64_t a, int64_t b) { return instance().randint(a, b); }
inline double uniform(double a, double b) { return instance().uniform(a, b); }
template <class T> T choice(const list<T>& s) { return instance().choice(s); }
template <class T> void shuffle(list<T>& x) { instance().shuffle(x); }
template <class T> list<T> sample(const list<T>& p, int64_t k) { return instance().sample(p, k); }

}  // namespace random
}  // namespace py
