#include "cb/translate/frontends/python/wide.hpp"

#include <intrin.h>

#include <algorithm>
#include <limits>

namespace cb::translate::frontends::python {

namespace {

using Mag = std::array<std::uint64_t, 4>;
constexpr std::uint64_t kAllOnes = std::numeric_limits<std::uint64_t>::max();

bool is_zero(const Mag& a) { return (a[0] | a[1] | a[2] | a[3]) == 0; }

int compare_mag(const Mag& a, const Mag& b) {
    for (int i = 3; i >= 0; --i) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

unsigned bit_length_mag(const Mag& a) {
    for (int i = 3; i >= 0; --i) {
        unsigned long highest = 0;
        if (_BitScanReverse64(&highest, a[i]) != 0) return static_cast<unsigned>(i) * 64 + static_cast<unsigned>(highest) + 1;
    }
    return 0;
}

Mag add_mag(const Mag& a, const Mag& b, bool& overflow) {
    Mag r{};
    unsigned char carry = 0;
    for (int i = 0; i < 4; ++i) carry = _addcarry_u64(carry, a[i], b[i], &r[i]);
    overflow = carry != 0;
    return r;
}

// a - b, a >= b
Mag sub_mag(const Mag& a, const Mag& b) {
    Mag r{};
    unsigned char borrow = 0;
    for (int i = 0; i < 4; ++i) borrow = _subborrow_u64(borrow, a[i], b[i], &r[i]);
    return r;
}

Mag mul_mag(const Mag& a, const Mag& b, bool& overflow) {
    std::array<std::uint64_t, 8> r{};
    for (int i = 0; i < 4; ++i) {
        std::uint64_t carry = 0;
        for (int j = 0; j < 4; ++j) {
            std::uint64_t high = 0;
            const std::uint64_t low = _umul128(a[i], b[j], &high);
            std::uint64_t sum = 0;
            const unsigned char c1 = _addcarry_u64(0, r[i + j], low, &sum);
            std::uint64_t with_carry = 0;
            const unsigned char c2 = _addcarry_u64(0, sum, carry, &with_carry);
            r[i + j] = with_carry;
            carry = high + c1 + c2;   // (high is at most 2**64 - 2, so this cannot wrap)
        }
        r[i + 4] = carry;
    }
    overflow = (r[4] | r[5] | r[6] | r[7]) != 0;
    return {r[0], r[1], r[2], r[3]};
}

Mag shift_left_mag(const Mag& a, unsigned n, bool& overflow) {
    overflow = !is_zero(a) && bit_length_mag(a) + n > Wide::kBits;
    if (overflow || n >= Wide::kBits) return {};
    const unsigned limbs = n / 64, bits = n % 64;
    Mag r{};
    for (unsigned i = limbs; i < 4; ++i) {
        r[i] = a[i - limbs] << bits;
        if (bits != 0 && i > limbs) r[i] |= a[i - limbs - 1] >> (64 - bits);
    }
    return r;
}

Mag shift_right_mag(const Mag& a, unsigned n) {
    if (n >= Wide::kBits) return {};
    const unsigned limbs = n / 64, bits = n % 64;
    Mag r{};
    for (unsigned i = 0; i + limbs < 4; ++i) {
        r[i] = a[i + limbs] >> bits;
        if (bits != 0 && i + limbs + 1 < 4) r[i] |= a[i + limbs + 1] << (64 - bits);
    }
    return r;
}

// q = a / b, r = a % b on magnitudes (b != 0)
void divmod_mag(const Mag& a, const Mag& b, Mag& q, Mag& r) {
    q = {};
    r = {};
    if (compare_mag(a, b) < 0) {
        r = a;
        return;
    }
    if ((a[1] | a[2] | a[3] | b[1] | b[2] | b[3]) == 0) {
        q[0] = a[0] / b[0];
        r[0] = a[0] % b[0];
        return;
    }
    for (int bit = static_cast<int>(bit_length_mag(a)) - 1; bit >= 0; --bit) {   // schoolbook, a bit at a time
        bool overflow = false;
        r = shift_left_mag(r, 1, overflow);   // (r < b <= a, so one more bit always fits)
        r[0] |= (a[bit / 64] >> (bit % 64)) & 1;
        if (compare_mag(r, b) >= 0) {
            r = sub_mag(r, b);
            q[bit / 64] |= std::uint64_t{1} << (bit % 64);
        }
    }
}

}  // namespace

Wide::Wide(bool negative, const Mag& mag) : negative_(negative), mag_(mag) {
    if (zero()) negative_ = false;
}

Wide::Wide(std::int64_t v) : negative_(v < 0), mag_{v < 0 ? 0 - static_cast<std::uint64_t>(v) : static_cast<std::uint64_t>(v), 0, 0, 0} {}

Wide Wide::from_u64(std::uint64_t v) { return Wide(false, Mag{v, 0, 0, 0}); }

Wide Wide::saturated(bool negative) { return Wide(negative, Mag{kAllOnes, kAllOnes, kAllOnes, kAllOnes}); }

Wide Wide::pow2(unsigned n) {
    if (n >= kBits) return saturated(false);
    Mag m{};
    m[n / 64] = std::uint64_t{1} << (n % 64);
    return Wide(false, m);
}

std::optional<Wide> Wide::from_decimal(std::string_view text) {
    bool negative = false;
    if (!text.empty() && text.front() == '-') {
        negative = true;
        text.remove_prefix(1);
    }
    if (text.empty()) return std::nullopt;
    Mag m{};
    bool saturate = false;
    for (const char c : text) {
        if (c < '0' || c > '9') return std::nullopt;
        if (saturate) continue;
        bool overflow = false;
        m = mul_mag(m, Mag{10, 0, 0, 0}, overflow);
        bool carried = false;
        m = add_mag(m, Mag{static_cast<std::uint64_t>(c - '0'), 0, 0, 0}, carried);
        saturate = overflow || carried;
    }
    return saturate ? saturated(negative) : Wide(negative, m);
}

unsigned Wide::bit_length() const { return bit_length_mag(mag_); }

std::optional<std::int64_t> Wide::to_i64() const {
    if ((mag_[1] | mag_[2] | mag_[3]) != 0) return std::nullopt;
    constexpr std::uint64_t kMinMagnitude = std::uint64_t{1} << 63;
    if (!negative_) return mag_[0] < kMinMagnitude ? std::optional<std::int64_t>(static_cast<std::int64_t>(mag_[0])) : std::nullopt;
    return mag_[0] <= kMinMagnitude ? std::optional<std::int64_t>(static_cast<std::int64_t>(0 - mag_[0])) : std::nullopt;
}

std::optional<std::uint64_t> Wide::to_u64() const {
    if (negative_ || (mag_[1] | mag_[2] | mag_[3]) != 0) return std::nullopt;
    return mag_[0];
}

std::string Wide::to_string() const {
    if (zero()) return "0";
    constexpr std::uint64_t kChunk = 10'000'000'000'000'000'000ull;   // 10**19, the most that fits a limb
    std::string digits;
    Mag m = mag_;
    while (!is_zero(m)) {
        Mag q{}, r{};
        divmod_mag(m, Mag{kChunk, 0, 0, 0}, q, r);
        std::uint64_t rest = r[0];
        for (int i = 0; i < 19; ++i) {
            digits.push_back(static_cast<char>('0' + rest % 10));
            rest /= 10;
        }
        m = q;
    }
    while (digits.size() > 1 && digits.back() == '0') digits.pop_back();
    if (negative_) digits.push_back('-');
    std::ranges::reverse(digits);
    return digits;
}

std::size_t Wide::hash() const {
    std::size_t h = negative_ ? 0x9E3779B97F4A7C15ull : 0;
    for (const std::uint64_t limb : mag_) h = (h ^ limb) * 0x100000001B3ull + 0x7F4A7C15u;
    return h;
}

Wide Wide::abs() const { return Wide(false, mag_); }
Wide Wide::operator-() const { return Wide(!negative_, mag_); }

Wide operator+(const Wide& a, const Wide& b) {
    if (a.negative_ == b.negative_) {
        bool overflow = false;
        const Mag sum = add_mag(a.mag_, b.mag_, overflow);
        return overflow ? Wide::saturated(a.negative_) : Wide(a.negative_, sum);
    }
    const int order = compare_mag(a.mag_, b.mag_);
    if (order == 0) return Wide();
    return order > 0 ? Wide(a.negative_, sub_mag(a.mag_, b.mag_)) : Wide(b.negative_, sub_mag(b.mag_, a.mag_));
}

Wide operator-(const Wide& a, const Wide& b) { return a + (-b); }

Wide operator*(const Wide& a, const Wide& b) {
    bool overflow = false;
    const Mag product = mul_mag(a.mag_, b.mag_, overflow);
    const bool negative = a.negative_ != b.negative_;
    return overflow ? Wide::saturated(negative) : Wide(negative, product);
}

Wide floor_div(const Wide& a, const Wide& b) {
    Mag q{}, r{};
    divmod_mag(a.mag_, b.mag_, q, r);
    const bool negative = a.negative_ != b.negative_;
    if (negative && !is_zero(r)) {   // the quotient rounds away from zero
        bool overflow = false;
        q = add_mag(q, Mag{1, 0, 0, 0}, overflow);
    }
    return Wide(negative, q);
}

Wide shifted_left(const Wide& a, unsigned n) {
    bool overflow = false;
    const Mag m = shift_left_mag(a.mag_, n, overflow);
    return overflow ? Wide::saturated(a.negative_) : Wide(a.negative_, m);
}

Wide shifted_right(const Wide& a, unsigned n) {
    if (!a.negative_) return Wide(false, shift_right_mag(a.mag_, n));
    // floor(-m / 2**n) = -(((m - 1) >> n) + 1)
    bool overflow = false;
    const Mag below = shift_right_mag(sub_mag(a.mag_, Mag{1, 0, 0, 0}), n);
    return Wide(true, add_mag(below, Mag{1, 0, 0, 0}, overflow));
}

Wide raised_to(const Wide& a, unsigned n) {
    Wide result(1), base = a;
    while (n != 0) {
        if ((n & 1) != 0) result = result * base;
        n >>= 1;
        if (n != 0) base = base * base;
    }
    return result;
}

std::strong_ordering operator<=>(const Wide& a, const Wide& b) {
    if (a.negative_ != b.negative_) return a.negative_ ? std::strong_ordering::less : std::strong_ordering::greater;
    const int order = compare_mag(a.mag_, b.mag_);
    if (order == 0) return std::strong_ordering::equal;
    const bool a_smaller_magnitude = order < 0;
    return (a_smaller_magnitude != a.negative_) ? std::strong_ordering::less : std::strong_ordering::greater;
}

}  // namespace cb::translate::frontends::python
