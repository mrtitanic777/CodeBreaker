// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_cpu_a64_fp.cpp -- see pcode_cpu_a64_fp.hpp.
#include "emulate/cpu/pcode_cpu_a64_fp.hpp"

#include <bit>
#include <cmath>
#include <cstring>
#include <limits>

#include "emulate/cpu/pcode_vector.hpp"

namespace cb::emulate::pcode_cpu::a64fp {
namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i64 = std::int64_t;

unsigned bits_of(u32 w, unsigned pos, unsigned width) { return (w >> pos) & (width >= 32 ? 0xFFFFFFFFu : ((1u << width) - 1)); }

double to_double(u64 bits, unsigned ftype) {
    if (ftype == 0) return std::bit_cast<float>(static_cast<u32>(bits));
    return std::bit_cast<double>(bits);
}

u64 from_double(double d, unsigned ftype) {
    if (ftype == 0) return std::bit_cast<u32>(static_cast<float>(d));
    return std::bit_cast<u64>(d);
}

float half_to_float(std::uint16_t h) {
    const unsigned sign = h >> 15, exp = (h >> 10) & 31, frac = h & 0x3FF;
    float v;
    if (exp == 0) v = std::ldexp(static_cast<float>(frac), -24);
    else if (exp == 31) v = frac ? std::numeric_limits<float>::quiet_NaN() : std::numeric_limits<float>::infinity();
    else v = std::ldexp(static_cast<float>(frac | 0x400), static_cast<int>(exp) - 25);
    return sign ? -v : v;
}

std::uint16_t float_to_half(double d) {
    if (std::isnan(d)) return 0x7E00;
    const unsigned sign = std::signbit(d) ? 0x8000u : 0u;
    d = std::fabs(d);
    if (std::isinf(d) || d >= 65520.0) return static_cast<std::uint16_t>(sign | 0x7C00);
    if (d < std::ldexp(1.0, -14)) {   // subnormal: a multiple of 2^-24
        const double q = std::nearbyint(std::ldexp(d, 24));
        return static_cast<std::uint16_t>(sign | static_cast<unsigned>(q));
    }
    int e;
    const double m = std::frexp(d, &e);   // d = m * 2^e, m in [0.5, 1)
    double frac = std::nearbyint(std::ldexp(m, 11));   // 11 significant bits, in [1024, 2048]
    if (frac >= 2048.0) {
        frac /= 2;
        ++e;
    }
    const int exp = e + 14;
    if (exp >= 31) return static_cast<std::uint16_t>(sign | 0x7C00);
    return static_cast<std::uint16_t>(sign | (static_cast<unsigned>(exp) << 10) | (static_cast<unsigned>(frac) & 0x3FF));
}

double round_mode(double d, unsigned mode) {
    switch (mode) {
        case 0: return std::nearbyint(d);   // to nearest, ties to even
        case 1: return std::ceil(d);
        case 2: return std::floor(d);
        case 3: return std::trunc(d);
        default: return std::round(d);      // to nearest, ties away
    }
}

u64 mask_of(bool is64) { return is64 ? ~0ull : 0xFFFFFFFFull; }

u64 sx(u64 v, unsigned width) {
    if (width >= 64) return v;
    const u64 m = 1ull << (width - 1);
    return ((v & ((1ull << width) - 1)) ^ m) - m;
}

// A floating-point value to an integer of `bits` bits, saturating; NaN is zero.
u64 to_integer(double d, unsigned mode, bool is_signed, unsigned bits_) {
    if (std::isnan(d)) return 0;
    const double r = round_mode(d, mode);
    if (is_signed) {
        const double lim = std::ldexp(1.0, static_cast<int>(bits_) - 1);
        if (r >= lim) return bits_ == 64 ? 0x7FFFFFFFFFFFFFFFull : 0x7FFFFFFFull;
        if (r < -lim) return bits_ == 64 ? 0x8000000000000000ull : 0x80000000ull;
        return static_cast<u64>(static_cast<i64>(r)) & mask_of(bits_ == 64);
    }
    const double lim = std::ldexp(1.0, static_cast<int>(bits_));
    if (r >= lim) return mask_of(bits_ == 64);
    if (r <= 0) return 0;
    return static_cast<u64>(r);
}

}  // namespace

bool is_nan_result(unsigned ftype, u64 value) {
    return ftype == 0 ? (value & 0x7F800000u) == 0x7F800000u && (value & 0x7FFFFFu) != 0
         : ftype == 1 ? (value & 0x7FF0000000000000ull) == 0x7FF0000000000000ull && (value & 0xFFFFFFFFFFFFFull) != 0
                      : (value & 0x7C00u) == 0x7C00u && (value & 0x3FFu) != 0;
}

u64 scalar_result(u32 w, u64 n, u64 m, u64 a) {
    const unsigned ftype = bits_of(w, 22, 2);
    // three sources (bit 24 is set)
    if (bits_of(w, 24, 5) == 0x1F) {
    const bool o1 = bits_of(w, 21, 1), o0 = bits_of(w, 15, 1);
    if (ftype == 0) {
        const float x = std::bit_cast<float>(static_cast<u32>(n)), y = std::bit_cast<float>(static_cast<u32>(m)), z = std::bit_cast<float>(static_cast<u32>(a));
        const float r = !o1 && !o0 ? std::fma(x, y, z) : !o1 && o0 ? std::fma(-x, y, z) : o1 && !o0 ? std::fma(-x, y, -z) : std::fma(x, y, -z);
        return std::bit_cast<u32>(r);
    }
    const double x = std::bit_cast<double>(n), y = std::bit_cast<double>(m), z = std::bit_cast<double>(a);
    const double r = !o1 && !o0 ? std::fma(x, y, z) : !o1 && o0 ? std::fma(-x, y, z) : o1 && !o0 ? std::fma(-x, y, -z) : std::fma(x, y, -z);
    return std::bit_cast<u64>(r);
    }
    // one source
    if (bits_of(w, 24, 8) == 0x1E && bits_of(w, 10, 5) == 0x10) {
        const unsigned opcode = bits_of(w, 15, 6);
        if (ftype == 3) {
            const std::uint16_t h = static_cast<std::uint16_t>(n);
            if (opcode == 0) return h;
            if (opcode == 1) return static_cast<u64>(h & 0x7FFF);
            if (opcode == 2) return static_cast<u64>(h ^ 0x8000);
            const double d = half_to_float(h);
            return from_double(d, opcode == 4 ? 0 : 1);
        }
        const u64 raw = ftype == 0 ? (n & 0xFFFFFFFFull) : n;
        const double d = to_double(raw, ftype);
        switch (opcode) {
            case 0: return raw;
            case 1: return ftype == 0 ? (raw & 0x7FFFFFFFull) : (raw & 0x7FFFFFFFFFFFFFFFull);
            case 2: return ftype == 0 ? (raw ^ 0x80000000ull) : (raw ^ 0x8000000000000000ull);
            case 3: return from_double(ftype == 0 ? static_cast<double>(std::sqrt(static_cast<float>(d))) : std::sqrt(d), ftype);
            case 4: return from_double(d, 0);
            case 5: return from_double(d, 1);
            case 7: return float_to_half(d);
            default: {
                const unsigned mode = opcode == 8 ? 0 : opcode == 9 ? 1 : opcode == 10 ? 2 : opcode == 11 ? 3 : opcode == 12 ? 4 : 0;
                return from_double(round_mode(d, mode), ftype);
            }
        }
    }
    // two sources (bits 11:10 == 10)
    if (bits_of(w, 10, 2) == 2) {
        const unsigned opcode = bits_of(w, 12, 4);
        const double x = to_double(n, ftype), y = to_double(m, ftype);
        double r;
        switch (opcode) {
            case 0: r = x * y; break;
            case 1: r = x / y; break;
            case 2: r = x + y; break;
            case 3: r = x - y; break;
            case 4: r = std::isnan(x) || std::isnan(y) ? std::numeric_limits<double>::quiet_NaN() : std::fmax(x, y); break;
            case 5: r = std::isnan(x) || std::isnan(y) ? std::numeric_limits<double>::quiet_NaN() : std::fmin(x, y); break;
            case 6: r = std::fmax(x, y); break;   // (maxnm and minnm: a quiet NaN operand loses)
            case 7: r = std::fmin(x, y); break;
            default: r = -(x * y); break;
        }
        if (ftype == 0) {
            // single precision: compute in single so rounding is single's
            const float fa = static_cast<float>(x), fb = static_cast<float>(y);
            float fr;
            switch (opcode) {
                case 0: fr = fa * fb; break;
                case 1: fr = fa / fb; break;
                case 2: fr = fa + fb; break;
                case 3: fr = fa - fb; break;
                case 8: fr = -(fa * fb); break;
                default: fr = static_cast<float>(r); break;
            }
            return std::bit_cast<u32>(fr);
        }
        return std::bit_cast<u64>(r);
    }
    return 0;
}

bool int_conversion(u32 w, u64 x, u64 v, u64& out) {
    const unsigned ftype = bits_of(w, 22, 2), rmode = bits_of(w, 19, 2), opcode = bits_of(w, 16, 3);
    const bool sf = bits_of(w, 31, 1) != 0;
    if (opcode == 2 || opcode == 3) {   // scvtf, ucvtf
        const u64 value = x & mask_of(sf);
        double d;
        float f;
        if (opcode == 2) {
            const i64 s = static_cast<i64>(sx(value, sf ? 64 : 32));
            d = static_cast<double>(s);
            f = static_cast<float>(s);
        } else {
            d = static_cast<double>(value);
            f = static_cast<float>(value);
        }
        out = ftype == 0 ? static_cast<u64>(std::bit_cast<u32>(f)) : std::bit_cast<u64>(d);
        return false;
    }
    const double d = to_double(v, ftype);
    const bool is_signed = !(opcode & 1);
    unsigned mode = rmode == 0 ? 0 : rmode == 1 ? 1 : rmode == 2 ? 2 : 3;
    if (opcode >= 4) mode = 4;
    out = to_integer(d, mode, is_signed, sf ? 64 : 32) & mask_of(sf);
    return true;
}

bool fixed_conversion(u32 w, u64 x, u64 v, u64& out) {
    const unsigned ftype = bits_of(w, 22, 2), opcode = bits_of(w, 16, 3), fbits = 64 - bits_of(w, 10, 6);
    const bool sf = bits_of(w, 31, 1) != 0;
    if (opcode == 2 || opcode == 3) {
        const u64 value = x & mask_of(sf);
        const double s = opcode == 2 ? static_cast<double>(static_cast<i64>(sx(value, sf ? 64 : 32))) : static_cast<double>(value);
        const double d = std::ldexp(s, -static_cast<int>(fbits));
        out = from_double(d, ftype);
        return false;
    }
    const double d = std::ldexp(to_double(v, ftype), static_cast<int>(fbits));
    out = to_integer(d, 3, opcode == 0, sf ? 64 : 32) & mask_of(sf);
    return true;
}

bool round_convert(u32 w, std::string_view mnemonic, bool scalar, const std::uint8_t* n, std::uint8_t* out, std::size_t& bytes) {
    namespace vec = cb::emulate::vec;
    const unsigned ew = bits_of(w, 22, 1) != 0 ? 8 : 4;
    const bool q = bits_of(w, 30, 1) != 0;
    if (scalar) bytes = ew;
    else bytes = q ? 16 : 8;
    if (!scalar && ew == 8 && !q) return false;
    vec::FpEnv env;
    env.nan = vec::NanRule::X86;   // (the lane's NaNs are the host's)
    unsigned flags = 0;
    if (mnemonic.starts_with("frint") && mnemonic.size() == 6) {
        switch (mnemonic[5]) {
            case 'n': env.round = vec::Round::NearestEven; break;
            case 'm': env.round = vec::Round::TowardNegative; break;
            case 'p': env.round = vec::Round::TowardPositive; break;
            case 'z': env.round = vec::Round::TowardZero; break;
            case 'a': env.round = vec::Round::NearestAway; break;
            case 'x': case 'i': env.round = vec::Round::NearestEven; break;
            default: return false;
        }
        vec::fp1(out, n, bytes, ew, vec::Fp1::RoundToInt, env, flags);
        return true;
    }
    if (mnemonic.starts_with("fcvt") && mnemonic.size() == 6 && (mnemonic[5] == 's' || mnemonic[5] == 'u')) {
        switch (mnemonic[4]) {
            case 'n': env.round = vec::Round::NearestEven; break;
            case 'm': env.round = vec::Round::TowardNegative; break;
            case 'p': env.round = vec::Round::TowardPositive; break;
            case 'z': env.round = vec::Round::TowardZero; break;
            case 'a': env.round = vec::Round::NearestAway; break;
            default: return false;
        }
        vec::floatToInt(out, n, bytes, ew, ew, mnemonic[5] == 's' ? vec::Sign::Signed : vec::Sign::Unsigned, 0, vec::IntOverflow::Saturate, env, flags);
        return true;
    }
    return false;
}

bool fixed_convert(u32 w, std::string_view mnemonic, bool scalar, const std::uint8_t* n, std::uint8_t* out, std::size_t& bytes) {
    namespace vec = cb::emulate::vec;
    const unsigned immh = bits_of(w, 19, 4), immhb = bits_of(w, 16, 7);
    const unsigned ew = (immh & 8) != 0 ? 8 : (immh & 4) != 0 ? 4 : 0;
    if (ew == 0) return false;
    const bool q = bits_of(w, 30, 1) != 0;
    bytes = scalar ? ew : q ? 16 : 8;
    if (!scalar && ew == 8 && !q) return false;
    vec::FpEnv env;
    env.nan = vec::NanRule::X86;
    env.round = vec::Round::TowardZero;
    unsigned flags = 0;
    vec::floatToInt(out, n, bytes, ew, ew, mnemonic == "fcvtzs" ? vec::Sign::Signed : vec::Sign::Unsigned, 16 * ew - immhb, vec::IntOverflow::Saturate, env, flags);
    return true;
}

bool fused_lanes(u32 w, std::string_view mnemonic, int form, const std::uint8_t* n, const std::uint8_t* m, const std::uint8_t* d, std::uint8_t* out, std::size_t& bytes) {
    const unsigned ew = bits_of(w, 22, 1) != 0 ? 8 : 4;
    const bool q = bits_of(w, 30, 1) != 0, subtract = mnemonic == "fmls";
    bytes = form == 2 ? ew : q ? 16 : 8;
    if (form == 0 && ew == 8 && !q) return false;
    unsigned index = 0;
    const unsigned rm = bits_of(w, 16, 5);
    (void)rm;
    if (form != 0) index = ew == 4 ? (bits_of(w, 11, 1) << 1) | bits_of(w, 21, 1) : bits_of(w, 11, 1);
    for (std::size_t i = 0; i < bytes; i += ew) {
        const std::size_t at = form == 0 ? i : static_cast<std::size_t>(index) * ew;
        if (ew == 4) {
            float x, y, acc;
            std::memcpy(&x, n + (form == 2 ? 0 : i), 4);
            std::memcpy(&y, m + at, 4);
            std::memcpy(&acc, d + (form == 2 ? 0 : i), 4);
            const float r = std::fma(subtract ? -x : x, y, acc);
            std::memcpy(out + i, &r, 4);
        } else {
            double x, y, acc;
            std::memcpy(&x, n + (form == 2 ? 0 : i), 8);
            std::memcpy(&y, m + at, 8);
            std::memcpy(&acc, d + (form == 2 ? 0 : i), 8);
            const double r = std::fma(subtract ? -x : x, y, acc);
            std::memcpy(out + i, &r, 8);
        }
    }
    return true;
}

bool any_nan_lane(const std::uint8_t* v, std::size_t bytes, unsigned ew) {
    for (std::size_t i = 0; i + ew <= bytes; i += ew) {
        u64 lane = 0;
        std::memcpy(&lane, v + i, ew);
        if (is_nan_result(ew == 4 ? 0 : 1, lane)) return true;
    }
    return false;
}

bool host_lanes(u32 w, std::string_view mnemonic, int form, const std::uint8_t* n, const std::uint8_t* m, std::uint8_t* out, std::size_t& bytes) {
    const unsigned ew = bits_of(w, 22, 1) != 0 ? 8 : 4;
    const bool q = bits_of(w, 30, 1) != 0;
    bytes = form >= 2 ? ew : q ? 16 : 8;
    if (form == 0 && ew == 8 && !q) return false;
    unsigned index = 0;
    if (form == 1 || form == 3) index = ew == 4 ? (bits_of(w, 11, 1) << 1) | bits_of(w, 21, 1) : bits_of(w, 11, 1);
    int op;
    if (mnemonic == "fadd") op = 0;
    else if (mnemonic == "fsub") op = 1;
    else if (mnemonic == "fmul") op = 2;
    else if (mnemonic == "fdiv") op = 3;
    else if (mnemonic == "fabd") op = 4;
    else return false;
    for (std::size_t i = 0; i < bytes; i += ew) {
        const std::size_t at = form == 1 || form == 3 ? static_cast<std::size_t>(index) * ew : i;
        if (ew == 4) {
            float x, y;
            std::memcpy(&x, n + i, 4);
            std::memcpy(&y, m + at, 4);
            const float r = op == 0 ? x + y : op == 1 ? x - y : op == 2 ? x * y : op == 3 ? x / y : std::fabs(x - y);
            std::memcpy(out + i, &r, 4);
        } else {
            double x, y;
            std::memcpy(&x, n + i, 8);
            std::memcpy(&y, m + at, 8);
            const double r = op == 0 ? x + y : op == 1 ? x - y : op == 2 ? x * y : op == 3 ? x / y : std::fabs(x - y);
            std::memcpy(out + i, &r, 8);
        }
    }
    return true;
}

}  // namespace cb::emulate::pcode_cpu::a64fp
