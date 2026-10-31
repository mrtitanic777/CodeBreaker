// big.cc -- Python's unbounded int (see big.hpp).
#include "big.hpp"

#include <algorithm>
#include <intrin.h>
#include <stdexcept>

namespace pyengine {

namespace {
using Limbs = std::vector<uint32_t>;

void trim(Limbs& m) {
    while (!m.empty() && m.back() == 0) m.pop_back();
}
int mcmp(const Limbs& a, const Limbs& b) {
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (size_t i = a.size(); i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
Limbs madd(const Limbs& a, const Limbs& b) {
    Limbs r(std::max(a.size(), b.size()) + 1, 0);
    uint64_t c = 0;
    for (size_t i = 0; i < r.size(); i++) {
        uint64_t s = c;
        if (i < a.size()) s += a[i];
        if (i < b.size()) s += b[i];
        r[i] = (uint32_t)s;
        c = s >> 32;
    }
    trim(r);
    return r;
}
Limbs msub(const Limbs& a, const Limbs& b) {   // a >= b
    Limbs r(a.size(), 0);
    int64_t br = 0;
    for (size_t i = 0; i < a.size(); i++) {
        int64_t s = (int64_t)a[i] - br - (i < b.size() ? (int64_t)b[i] : 0);
        br = s < 0;
        if (s < 0) s += (int64_t)1 << 32;
        r[i] = (uint32_t)s;
    }
    trim(r);
    return r;
}
Limbs mmul(const Limbs& a, const Limbs& b) {
    if (a.empty() || b.empty()) return {};
    Limbs r(a.size() + b.size(), 0);
    for (size_t i = 0; i < a.size(); i++) {
        uint64_t c = 0;
        for (size_t j = 0; j < b.size(); j++) {
            uint64_t t = (uint64_t)a[i] * b[j] + r[i + j] + c;
            r[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        size_t k = i + b.size();
        while (c) {
            uint64_t t = (uint64_t)r[k] + c;
            r[k++] = (uint32_t)t;
            c = t >> 32;
        }
    }
    trim(r);
    return r;
}
int mbits(const Limbs& a) {
    if (a.empty()) return 0;
    int n = (int)(a.size() - 1) * 32;
    uint32_t top = a.back();
    while (top) {
        n++;
        top >>= 1;
    }
    return n;
}
Limbs mshl(const Limbs& a, int n) {
    if (a.empty()) return {};
    int w = n / 32, b = n % 32;
    Limbs r(a.size() + w + 1, 0);
    for (size_t i = 0; i < a.size(); i++) {
        uint64_t x = (uint64_t)a[i] << b;
        r[i + w] |= (uint32_t)x;
        r[i + w + 1] |= (uint32_t)(x >> 32);
    }
    trim(r);
    return r;
}
Limbs mshr(const Limbs& a, int n) {
    int w = n / 32, b = n % 32;
    if ((size_t)w >= a.size()) return {};
    Limbs r(a.size() - w, 0);
    for (size_t i = 0; i < r.size(); i++) {
        uint64_t x = a[i + w];
        if (i + w + 1 < a.size()) x |= (uint64_t)a[i + w + 1] << 32;
        r[i] = (uint32_t)(x >> b);
    }
    trim(r);
    return r;
}
bool mtestbit(const Limbs& a, int n) {
    size_t w = n / 32;
    return w < a.size() && ((a[w] >> (n % 32)) & 1);
}
// binary long division of magnitudes: q = a / b, r = a % b
void mdivmod(const Limbs& a, const Limbs& b, Limbs& q, Limbs& r) {
    if (b.empty()) throw std::runtime_error("division by zero");
    if (mcmp(a, b) < 0) {
        q.clear();
        r = a;
        return;
    }
    if (b.size() == 1) {
        uint64_t d = b[0], rem = 0;
        q.assign(a.size(), 0);
        for (size_t i = a.size(); i-- > 0;) {
            uint64_t cur = (rem << 32) | a[i];
            q[i] = (uint32_t)(cur / d);
            rem = cur % d;
        }
        trim(q);
        r.clear();
        if (rem) r.push_back((uint32_t)rem);
        return;
    }
    int nb = mbits(a);
    q.assign(a.size(), 0);
    r.clear();
    for (int i = nb - 1; i >= 0; i--) {
        r = mshl(r, 1);
        if (mtestbit(a, i)) {
            if (r.empty()) r.push_back(1);
            else r[0] |= 1;
        }
        if (mcmp(r, b) >= 0) {
            r = msub(r, b);
            q[i / 32] |= 1u << (i % 32);
        }
    }
    trim(q);
}
bool add_ovf(int64_t a, int64_t b, int64_t& r) {
    if ((b > 0 && a > INT64_MAX - b) || (b < 0 && a < INT64_MIN - b)) return true;
    r = a + b;
    return false;
}
bool sub_ovf(int64_t a, int64_t b, int64_t& r) {
    if ((b < 0 && a > INT64_MAX + b) || (b > 0 && a < INT64_MIN + b)) return true;
    r = a - b;
    return false;
}
bool mul_ovf(int64_t a, int64_t b, int64_t& r) {
    if (a == 0 || b == 0) {
        r = 0;
        return false;
    }
    int64_t hi;
    int64_t lo = _mul128(a, b, &hi);
    if ((hi == 0 && lo >= 0) || (hi == -1 && lo < 0)) {
        r = lo;
        return false;
    }
    return true;
}
Limbs of_u64(uint64_t v) {
    Limbs m;
    while (v) {
        m.push_back((uint32_t)v);
        v >>= 32;
    }
    return m;
}
}  // namespace

Big Big::from_mag(bool neg, std::vector<uint32_t> mag) {
    trim(mag);
    Big r;
    if (mag.empty()) return r;
    if (mag.size() <= 2) {
        uint64_t u = mag[0] | (mag.size() > 1 ? (uint64_t)mag[1] << 32 : 0);
        if (!neg && u <= (uint64_t)INT64_MAX) {
            r.v_ = (int64_t)u;
            return r;
        }
        if (neg && u <= (uint64_t)INT64_MAX + 1) {
            r.v_ = (int64_t)(0 - u);
            return r;
        }
    }
    r.small_ = false;
    r.neg_ = neg;
    r.mag_ = std::move(mag);
    return r;
}

void Big::to_mag(bool& neg, std::vector<uint32_t>& mag) const {
    if (!small_) {
        neg = neg_;
        mag = mag_;
        return;
    }
    neg = v_ < 0;
    uint64_t u = neg ? 0 - (uint64_t)v_ : (uint64_t)v_;
    mag = of_u64(u);
}

Big Big::from_decimal(const std::string& digits) {
    bool neg = false;
    size_t i = 0;
    if (!digits.empty() && digits[0] == '-') {
        neg = true;
        i = 1;
    }
    Limbs m;
    Limbs ten = {10};
    for (; i < digits.size(); i++) {
        char c = digits[i];
        if (c < '0' || c > '9') continue;
        m = mmul(m, ten);
        m = madd(m, of_u64((uint64_t)(c - '0')));
    }
    return from_mag(neg, m);
}

Big Big::pow2(int n) { return shl(Big(1), n); }

int Big::sign() const {
    if (small_) return v_ < 0 ? -1 : (v_ > 0 ? 1 : 0);
    return neg_ ? -1 : 1;
}

int Big::bit_length() const {
    bool n;
    Limbs m;
    to_mag(n, m);
    return mbits(m);
}

std::string Big::str() const {
    if (small_) return std::to_string(v_);
    Limbs m = mag_, q, r;
    Limbs bil = {1000000000u};
    std::vector<uint32_t> chunks;
    while (!m.empty()) {
        mdivmod(m, bil, q, r);
        chunks.push_back(r.empty() ? 0 : r[0]);
        m = q;
    }
    std::string s = neg_ ? "-" : "";
    s += std::to_string(chunks.back());
    for (size_t i = chunks.size() - 1; i-- > 0;) {
        std::string c = std::to_string(chunks[i]);
        s += std::string(9 - c.size(), '0') + c;
    }
    return s;
}

static Big addsub(const Big& a, const Big& b, bool sub, bool an, const Limbs& am, bool bn, const Limbs& bm);

Big operator+(const Big& a, const Big& b) {
    if (a.small_ && b.small_) {
        int64_t r;
        if (!add_ovf(a.v_, b.v_, r)) return Big(r);
    }
    bool an, bn;
    Limbs am, bm;
    a.to_mag(an, am);
    b.to_mag(bn, bm);
    return addsub(a, b, false, an, am, bn, bm);
}
Big operator-(const Big& a, const Big& b) {
    if (a.small_ && b.small_) {
        int64_t r;
        if (!sub_ovf(a.v_, b.v_, r)) return Big(r);
    }
    bool an, bn;
    Limbs am, bm;
    a.to_mag(an, am);
    b.to_mag(bn, bm);
    return addsub(a, b, true, an, am, bn, bm);
}
static Big addsub(const Big&, const Big&, bool sub, bool an, const Limbs& am, bool bn, const Limbs& bm) {
    if (sub) bn = !bn;
    if (an == bn) return Big::from_mag(an, madd(am, bm));
    int c = mcmp(am, bm);
    if (c == 0) return Big(0);
    if (c > 0) return Big::from_mag(an, msub(am, bm));
    return Big::from_mag(bn, msub(bm, am));
}

Big operator*(const Big& a, const Big& b) {
    if (a.small_ && b.small_) {
        int64_t r;
        if (!mul_ovf(a.v_, b.v_, r)) return Big(r);
    }
    bool an, bn;
    Limbs am, bm;
    a.to_mag(an, am);
    b.to_mag(bn, bm);
    return Big::from_mag(an != bn, mmul(am, bm));
}

Big Big::operator-() const { return Big(0) - *this; }

static void floordivmod(const Big& a, const Big& b, Big* q, Big* r) {
    if (b.sign() == 0) throw std::runtime_error("integer division by zero");
    if (a.small() && b.small() && !(a.i64() == INT64_MIN && b.i64() == -1)) {
        int64_t x = a.i64(), y = b.i64();
        int64_t qq = x / y, rr = x % y;
        if (rr != 0 && ((rr < 0) != (y < 0))) {
            qq -= 1;
            rr += y;
        }
        if (q) *q = Big(qq);
        if (r) *r = Big(rr);
        return;
    }
    // truncated on magnitudes, then floored
    Big aa = a.abs(), bb = b.abs();
    Big tq, tr;
    Big::divmod_mag(aa, bb, tq, tr);
    bool neg = (a.sign() < 0) != (b.sign() < 0);
    if (neg) tq = -tq;
    if (a.sign() < 0) tr = -tr;
    if (tr.sign() != 0 && ((tr.sign() < 0) != (b.sign() < 0))) {
        tq = tq - Big(1);
        tr = tr + b;
    }
    if (q) *q = tq;
    if (r) *r = tr;
}
Big floordiv(const Big& a, const Big& b) {
    Big q;
    floordivmod(a, b, &q, nullptr);
    return q;
}
Big floormod(const Big& a, const Big& b) {
    Big r;
    floordivmod(a, b, nullptr, &r);
    return r;
}

Big shl(const Big& a, int n) {
    if (n <= 0 || a.sign() == 0) return a;
    if (a.small_ && n < 62) {
        int64_t lim = (int64_t)1 << (62 - n);
        if (a.v_ < lim && a.v_ > -lim) return Big(a.v_ * ((int64_t)1 << n));
    }
    bool an;
    Limbs am;
    a.to_mag(an, am);
    return Big::from_mag(an, mshl(am, n));
}
Big shr(const Big& a, int n) {
    if (n <= 0) return a;
    if (a.small_) {
        if (n >= 63) return Big(a.v_ < 0 ? -1 : 0);
        return Big(a.v_ >> n);   // arithmetic shift floors (MSVC, C++20)
    }
    if (!a.neg_) return Big::from_mag(false, mshr(a.mag_, n));
    // floor for negatives: -((|a| - 1) >> n) - 1
    Big m = a.abs() - Big(1);
    bool mn;
    Limbs mm;
    m.to_mag(mn, mm);
    return -Big::from_mag(false, mshr(mm, n)) - Big(1);
}
Big ipow(const Big& a, int n) {
    Big r(1), base = a;
    while (n > 0) {
        if (n & 1) r = r * base;
        n >>= 1;
        if (n) base = base * base;
    }
    return r;
}

void Big::divmod_mag(const Big& a, const Big& b, Big& q, Big& r) {
    bool an, bn;
    Limbs am, bm, qm, rem;
    a.to_mag(an, am);
    b.to_mag(bn, bm);
    mdivmod(am, bm, qm, rem);
    q = from_mag(false, qm);
    r = from_mag(false, rem);
}

int cmp(const Big& a, const Big& b) {
    if (a.small_ && b.small_) return a.v_ < b.v_ ? -1 : (a.v_ > b.v_ ? 1 : 0);
    int sa = a.sign(), sb = b.sign();
    if (sa != sb) return sa < sb ? -1 : 1;
    bool an, bn;
    Limbs am, bm;
    a.to_mag(an, am);
    b.to_mag(bn, bm);
    int c = mcmp(am, bm);
    return sa < 0 ? -c : c;
}

size_t Big::hash() const {
    if (small_) return std::hash<int64_t>()(v_);
    size_t h = neg_ ? 7 : 3;
    for (uint32_t x : mag_) h = h * 1000003u ^ x;
    return h;
}

}  // namespace pyengine
