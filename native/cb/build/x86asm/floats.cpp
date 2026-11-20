// cb/build/x86asm/floats.cpp -- floating-point constants; see floats.hpp. A port of asm/floats.c.
//
// The conversion works in a 192-bit mantissa (six 32-bit limbs: 112 bits for the widest format, 64 more for accuracy and 16
// for rounding). A decimal constant is turned into a binary fraction by repeated doubling of its digits, then scaled by a
// power of five with a square-and-multiply of 5^n held in the same limbs; the result is shifted into place for the target
// format and rounded. The steps are NASM's, bit for bit, since the rounding of the last place depends on them.
#include "cb/build/x86asm/floats.hpp"

#include <cstring>
#include <string>

#include "cb/build/x86asm/assembly.hpp"
#include "cb/build/x86asm/nctype.hpp"

namespace cb::build::x86asm {

// The formats; this must correspond to Floatize. Some formats have more than one enum (the halves of the 80- and 128-bit
// ones for the floatize operators), so both are listed with the offset into the byte array.
const std::array<IeeeFormat, FLOAT_ERR> kFpFormats = {{
    {1, 3, 0, 4, 0},      // FLOAT_8
    {2, 10, 0, 5, 0},     // FLOAT_16
    {2, 7, 0, 8, 0},      // FLOAT_B16
    {4, 23, 0, 8, 0},     // FLOAT_32
    {8, 52, 0, 11, 0},    // FLOAT_64
    {10, 63, 1, 15, 0},   // FLOAT_80M
    {10, 63, 1, 15, 8},   // FLOAT_80E
    {16, 112, 0, 15, 0},  // FLOAT_128L
    {16, 112, 0, 15, 8},  // FLOAT_128H
}};

namespace floats_impl {

using fp_limb = std::uint32_t;     // "a limb is like a digit but bigger"
using fp_2limb = std::uint64_t;

constexpr int LIMB_BITS = 32;
constexpr int LIMB_BYTES = LIMB_BITS / 8;
constexpr fp_limb LIMB_TOP_BIT = fp_limb{1} << (LIMB_BITS - 1);
constexpr fp_limb LIMB_MASK = ~fp_limb{0};
constexpr fp_limb LIMB_ALL_BYTES = 0x01010101;

constexpr int MANT_LIMBS = 6;      // 112 bits + 64 bits for accuracy + 16 bits for rounding
constexpr int MANT_DIGITS = 52;    // 52 digits fit in 176 bits because 10^53 > 2^176 > 10^52

using Mantissa = fp_limb[MANT_LIMBS];

// Multiply `to` by `from`, both fractions with the top bit set; the result is scaled so its top bit is set too, and the
// return value is 0 or -1, the binary exponent adjustment of that scaling.
int float_multiply(fp_limb* to, const fp_limb* from) {
    fp_2limb temp[MANT_LIMBS * 2];
    std::memset(temp, 0, sizeof temp);
    for (int i = 0; i < MANT_LIMBS; i++) {
        for (int j = 0; j < MANT_LIMBS; j++) {
            const fp_2limb n = static_cast<fp_2limb>(to[i]) * static_cast<fp_2limb>(from[j]);
            temp[i + j] += n >> LIMB_BITS;
            temp[i + j + 1] += static_cast<fp_limb>(n);
        }
    }
    for (int i = MANT_LIMBS * 2; --i;) {
        temp[i - 1] += temp[i] >> LIMB_BITS;
        temp[i] &= LIMB_MASK;
    }
    // Both top bits were set, so the product has its top bit at the first or second place: at most one shift left.
    if (temp[0] & LIMB_TOP_BIT) {
        for (int i = 0; i < MANT_LIMBS; i++) to[i] = static_cast<fp_limb>(temp[i] & LIMB_MASK);
        return 0;
    }
    for (int i = 0; i < MANT_LIMBS; i++) to[i] = static_cast<fp_limb>((temp[i] << 1) + ((temp[i + 1] & LIMB_TOP_BIT) ? 1 : 0));
    return -1;
}

// Set a bit, using *big-endian* bit numbering (0 = the most significant).
void set_bit(fp_limb* mant, std::size_t bit) { mant[bit / LIMB_BITS] |= fp_limb{1} << (~bit & (LIMB_BITS - 1)); }

// Test a single bit.
int test_bit(const fp_limb* mant, std::size_t bit) { return (mant[bit / LIMB_BITS] >> (~bit & (LIMB_BITS - 1))) & 1; }

// Is the mantissa all zero?
bool is_zero(const fp_limb* mant) {
    for (int i = 0; i < MANT_LIMBS; i++)
        if (mant[i]) return false;
    return true;
}

// Round off after the limb `i`'s bit `bit`: truncate (ROUND_ABS_DOWN) or add one at that place (ROUND_ABS_UP; the return
// value says the carry ran out of the top limb).
bool round_abs_down(fp_limb* mant, int i, fp_limb bit) {
    mant[i] &= ~(bit - 1);
    for (int j = i + 1; j < MANT_LIMBS; j++) mant[j] = 0;
    return false;
}

bool round_abs_up(fp_limb* mant, int i, fp_limb bit) {
    mant[i] = (mant[i] & ~(bit - 1)) + bit;
    for (int j = i + 1; j < MANT_LIMBS; j++) mant[j] = 0;
    while (i > 0 && !mant[i]) ++mant[--i];
    return !mant[0];
}

// The bits at and below `bit` in limb `i`, and every limb after it (ROUND_COLLECT_BITS).
fp_limb round_collect(const fp_limb* mant, int i, fp_limb bit) {
    fp_limb m = mant[i] & (2u * bit - 1u);
    for (int j = i + 1; j < MANT_LIMBS; j++) m |= mant[j];
    return m;
}

// Round a mantissa off after `bits` bits (ieee_round).
bool ieee_round(FloatRound rc, bool minus, fp_limb* mant, int bits) {
    const int i = bits / LIMB_BITS;
    const int p = bits % LIMB_BITS;
    const fp_limb bit = LIMB_TOP_BIT >> p;

    if (rc == FloatRound::Near) {
        if (mant[i] & bit) {
            mant[i] &= ~bit;
            const fp_limb m = round_collect(mant, i, bit);
            mant[i] |= bit;
            if (m) return round_abs_up(mant, i, bit);
            if (test_bit(mant, bits - 1)) return round_abs_up(mant, i, bit);
            return round_abs_down(mant, i, bit);
        }
        return round_abs_down(mant, i, bit);
    }
    if (rc == FloatRound::Zero || rc == (minus ? FloatRound::Up : FloatRound::Down)) return round_abs_down(mant, i, bit);

    // Round toward +/- infinity.
    if (round_collect(mant, i, bit)) return round_abs_up(mant, i, bit);
    return round_abs_down(mant, i, bit);
}

// Shift a mantissa to the right by `i` bits.
void ieee_shr(fp_limb* mant, int i) {
    int j = 0;
    const int sr = i % LIMB_BITS;
    const int sl = LIMB_BITS - sr;
    const int offs = i / LIMB_BITS;

    if (sr == 0) {
        if (offs)
            for (j = MANT_LIMBS - 1; j >= offs; j--) mant[j] = mant[j - offs];
    } else if (MANT_LIMBS - 1 - offs < 0) {
        j = MANT_LIMBS - 1;
    } else {
        fp_limb n = mant[MANT_LIMBS - 1 - offs] >> sr;
        for (j = MANT_LIMBS - 1; j > offs; j--) {
            const fp_limb m = mant[j - offs - 1];
            mant[j] = (m << sl) | n;
            n = m >> sr;
        }
        mant[j--] = n;
    }
    while (j >= 0) mant[j--] = 0;
}

// The kinds of value a constant can come to (enum floats).
enum class FloatKind { Zero, Denormal, Normal, Infinity, QNan, SNan };

}  // namespace

// ---- the conversions, which report through the assembly ----

namespace floats_impl {

class Converter {
public:
    explicit Converter(Assembly& as) : as_(as) {}

    // Read an exponent; INT32_MAX on error. The exponent is capped at +/-`max` so the arithmetic cannot wrap: 5000 is
    // slightly more than normal and denormal numbers need in single, double and extended precision.
    std::int32_t read_exponent(const char* string, std::int32_t max) {
        std::int32_t i = 0;
        bool neg = false;
        if (*string == '+') {
            string++;
        } else if (*string == '-') {
            neg = true;
            string++;
        }
        while (*string) {
            if (*string >= '0' && *string <= '9') {
                i = (i * 10) + (*string - '0');
                if (i > max) i = max;
            } else if (*string == '_') {
                // ignored
            } else {
                as_.nonfatal("invalid character in floating-point constant {}: '{}'", "exponent", *string);
                return INT32_MAX;
            }
            string++;
        }
        return neg ? -i : i;
    }

    // Convert a decimal constant to a mantissa in [0.5, 1) and a power of two (ieee_flconvert).
    bool flconvert(const char* string, fp_limb* mant, std::int32_t* exponent) {
        char digits[MANT_DIGITS];
        char* p = digits;
        std::int32_t tenpwr = 0;
        bool started = false, seendot = false, warned = false;

        while (*string && *string != 'E' && *string != 'e') {
            if (*string == '.') {
                if (!seendot) {
                    seendot = true;
                } else {
                    as_.nonfatal("too many periods in floating-point constant");
                    return false;
                }
            } else if (*string >= '0' && *string <= '9') {
                if (*string == '0' && !started) {
                    if (seendot) tenpwr--;
                } else {
                    started = true;
                    if (p < digits + sizeof(digits)) {
                        *p++ = static_cast<char>(*string - '0');
                    } else if (!warned) {
                        as_.warn(WARN_FLOAT_TOOLONG | ERR_PASS2, "floating-point constant significand contains more than {} digits", MANT_DIGITS);
                        warned = true;
                    }
                    if (!seendot) tenpwr++;
                }
            } else if (*string == '_') {
                // ignored
            } else {
                as_.nonfatalf(ERR_PASS2, "invalid character in floating-point constant {}: '{}'", "significand", *string);
                return false;
            }
            string++;
        }

        if (*string) {
            string++;   // eat the E
            const std::int32_t e = read_exponent(string, 5000);
            if (e == INT32_MAX) return false;
            tenpwr += e;
        }

        // The digits [digits,p) are zzzzzzz, such that the number X is X = 0.zzzzzzz * 10^tenpwr. Convert them to the binary
        // fraction by doubling the decimal fraction and collecting the carries out.
        fp_limb bit = LIMB_TOP_BIT;
        for (fp_limb* m = mant; m < mant + MANT_LIMBS; m++) *m = 0;
        fp_limb* m = mant;
        char* q = digits;
        started = false;
        std::int32_t twopwr = 0;
        while (m < mant + MANT_LIMBS) {
            fp_limb carry = 0;
            while (p > q && !p[-1]) p--;
            if (p <= q) break;
            for (char* r = p; r-- > q;) {
                std::int32_t i = 2 * *r + static_cast<std::int32_t>(carry);
                if (i >= 10) {
                    carry = 1;
                    i -= 10;
                } else {
                    carry = 0;
                }
                *r = static_cast<char>(i);
            }
            if (carry) {
                *m |= bit;
                started = true;
            }
            if (started) {
                if (bit == 1) {
                    bit = LIMB_TOP_BIT;
                    m++;
                } else {
                    bit >>= 1;
                }
            } else {
                twopwr--;
            }
        }
        twopwr += tenpwr;

        // Now `mant` holds the first fractional places of a real number which, multiplied by 2^twopwr and 5^tenpwr, gives X.
        // Multiply it by 5^tenpwr: 5^-1 = 0.2 or 5^1 = 5.0 squared up bit by bit of tenpwr.
        fp_limb mult[MANT_LIMBS];
        std::int32_t extratwos;
        if (tenpwr < 0) {   // mult = 5^-1 = 0.2
            for (fp_limb* mm = mult; mm < mult + MANT_LIMBS - 1; mm++) *mm = 0xcc * LIMB_ALL_BYTES;
            mult[MANT_LIMBS - 1] = 0xcc * LIMB_ALL_BYTES + 1;
            extratwos = -2;
            tenpwr = -tenpwr;
        } else if (tenpwr > 0) {   // mult = 5^+1 = 5.0
            mult[0] = static_cast<fp_limb>(5) << (LIMB_BITS - 3);   // 0xA000...
            for (fp_limb* mm = mult + 1; mm < mult + MANT_LIMBS; mm++) *mm = 0;
            extratwos = 3;
        } else {
            extratwos = 0;
        }
        while (tenpwr) {
            if (tenpwr & 1) twopwr += extratwos + float_multiply(mant, mult);
            extratwos = extratwos * 2 + float_multiply(mult, mult);
            tenpwr >>= 1;
            // Keep the loop terminating whatever a signed shift does.
            tenpwr &= static_cast<std::int32_t>(~std::uint32_t{0} >> 1);
        }

        // Now `mant` holds a real number in [0.5,1) that, multiplied by 2^twopwr, gives X; or zero.
        *exponent = twopwr;
        return true;
    }

    // Handle a constant with radix 2^bits and a binary exponent (ieee_flconvert_bin).
    bool flconvert_bin(const char* string, int bits, fp_limb* mant, std::int32_t* exponent) {
        static constexpr int log2tbl[16] = {-1, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3};
        fp_limb mult[MANT_LIMBS + 1];
        fp_limb* mp = nullptr;
        int ms = 0;
        std::int32_t twopwr = 0;
        bool seendot = false, seendigit = false;
        const int radix = 1 << bits;
        std::memset(mult, 0, sizeof mult);

        unsigned char c;
        while ((c = static_cast<unsigned char>(*string++)) != '\0') {
            unsigned v = 0;
            if (c == '.') {
                if (!seendot) {
                    seendot = true;
                } else {
                    as_.nonfatal("too many periods in floating-point constant");
                    return false;
                }
            } else if ((v = nasm_hexval(static_cast<char>(c))) < static_cast<unsigned>(radix)) {
                if (!seendigit && v) {
                    const int l = log2tbl[v];
                    seendigit = true;
                    mp = mult;
                    ms = (LIMB_BITS - 1) - l;
                    twopwr += l + 1 - bits;
                }
                if (seendigit) {
                    if (ms < 0) {
                        // Cast to fp_2limb as ms == -LIMB_BITS is possible.
                        *mp |= static_cast<fp_limb>(static_cast<fp_2limb>(v) >> -ms);
                        mp++;
                        if (mp > &mult[MANT_LIMBS]) mp = &mult[MANT_LIMBS];   // guard slot
                        ms += LIMB_BITS;
                    }
                    *mp |= v << ms;
                    ms -= bits;
                    if (!seendot) twopwr += bits;
                } else if (seendot) {
                    twopwr -= bits;
                }
            } else if (c == 'p' || c == 'P') {
                const std::int32_t e = read_exponent(string, 20000);
                if (e == INT32_MAX) return false;
                twopwr += e;
                break;
            } else if (c == '_') {
                // ignored
            } else {
                as_.nonfatal("floating-point constant: `{}' is invalid character", static_cast<char>(c));
                return false;
            }
        }

        if (!seendigit) {
            std::memset(mant, 0, MANT_LIMBS * sizeof(fp_limb));   // zero
            *exponent = 0;
        } else {
            std::memcpy(mant, mult, MANT_LIMBS * sizeof(fp_limb));
            *exponent = twopwr;
        }
        return true;
    }

    // A packed BCD constant: the digits from `p` back to `str`, two to a byte, then the sign byte (to_packed_bcd).
    bool to_packed_bcd(const char* str, const char* p, int s, std::uint8_t* result, const IeeeFormat& fmt) {
        int n = 0;
        int tv = -1;

        if (fmt.bytes != 10) {
            as_.nonfatal("packed BCD requires an 80-bit format");
            return false;
        }

        while (p >= str) {
            const char c = *p--;
            if (c >= '0' && c <= '9') {
                if (tv < 0) {
                    if (n == 9) as_.warn(WARN_OTHER | ERR_PASS2, "packed BCD truncated to 18 digits");
                    tv = c - '0';
                } else {
                    if (n < 9) *result++ = static_cast<std::uint8_t>(tv + ((c - '0') << 4));
                    n++;
                    tv = -1;
                }
            } else if (c == '_') {
                // ignored
            } else {
                as_.nonfatal("invalid character `{}' in packed BCD constant", c);
                return false;
            }
        }
        if (tv >= 0) {
            if (n < 9) *result++ = static_cast<std::uint8_t>(tv);
            n++;
        }
        while (n < 9) {
            *result++ = 0;
            n++;
        }
        *result = (s < 0) ? 0x80 : 0;
        return true;
    }

    Assembly& as_;
};

}  // namespace

bool Floats::float_const(std::string_view text, int s, FloatBytes& result, Floatize ffmt) const {
    using namespace floats_impl;
    const IeeeFormat& fmt = kFpFormats[static_cast<std::size_t>(ffmt)];
    Converter conv(as_);
    const std::string owned(text);
    const char* str = owned.c_str();
    const char* strend = str + owned.size();

    fp_limb mant[MANT_LIMBS] = {};
    std::int32_t exponent = 0;
    const std::int32_t expmax = 1 << (fmt.exponent - 1);
    const fp_limb one_mask = LIMB_TOP_BIT >> ((fmt.exponent + fmt.explicit_) % LIMB_BITS);
    const int one_pos = (fmt.exponent + fmt.explicit_) / LIMB_BITS;
    FloatKind type = FloatKind::QNan;
    const bool minus = s < 0;
    const int bits = fmt.bytes * 8;

    if (!str[0]) as_.assert_failed("str[0]");

    // A packed BCD constant ends in p.
    if (strend[-1] == 'P' || strend[-1] == 'p') return conv.to_packed_bcd(str, strend - 2, s, result.data(), fmt);

    if (str[0] == '_') {
        // The special tokens: __?nan?__, __?qnan?__, __?snan?__, __?infinity?__.
        switch (str[3]) {
        case 'n': case 'N': case 'q': case 'Q': type = FloatKind::QNan; break;
        case 's': case 'S': type = FloatKind::SNan; break;
        case 'i': case 'I': type = FloatKind::Infinity; break;
        default:
            as_.nonfatal("internal error: unknown FP constant token `{}'", str);
            type = FloatKind::QNan;
            break;
        }
    } else {
        bool ok = false;
        if (str[0] == '0') {
            switch (str[1]) {
            case 'x': case 'X': case 'h': case 'H':
                ok = conv.flconvert_bin(str + 2, 4, mant, &exponent);
                break;
            case 'o': case 'O': case 'q': case 'Q':
                ok = conv.flconvert_bin(str + 2, 3, mant, &exponent);
                break;
            case 'b': case 'B': case 'y': case 'Y':
                ok = conv.flconvert_bin(str + 2, 1, mant, &exponent);
                break;
            case 'd': case 'D': case 't': case 'T':
                ok = conv.flconvert(str + 2, mant, &exponent);
                break;
            case 'p': case 'P':
                return conv.to_packed_bcd(str + 2, strend - 1, s, result.data(), fmt);
            default:
                // The leading zero was just a zero.
                ok = conv.flconvert(str, mant, &exponent);
                break;
            }
        } else if (str[0] == '$') {
            ok = conv.flconvert_bin(str + 1, 4, mant, &exponent);
        } else {
            ok = conv.flconvert(str, mant, &exponent);
        }

        if (!ok) {
            type = FloatKind::QNan;
        } else if (mant[0] & LIMB_TOP_BIT) {
            // Non-zero.
            exponent--;
            if (exponent >= 2 - expmax && exponent <= expmax) {
                type = FloatKind::Normal;
            } else if (exponent > 0) {
                as_.warn(WARN_FLOAT_OVERFLOW | ERR_PASS2, "overflow in floating-point constant");
                type = FloatKind::Infinity;
            } else {
                // Underflow or denormal; the denormal code handles actual underflow.
                type = FloatKind::Denormal;
            }
        } else {
            type = FloatKind::Zero;
        }
    }

    switch (type) {
    case FloatKind::Zero:
        std::memset(mant, 0, sizeof mant);
        break;

    case FloatKind::Denormal: {
        const int shift = -(exponent + expmax - 2 - fmt.exponent) + fmt.explicit_;
        ieee_shr(mant, shift);
        ieee_round(rc_, minus, mant, bits);
        if (mant[one_pos] & one_mask) {
            // The one's position is set: it rounded up into the normal range.
            exponent = 1;
            if (!fmt.explicit_) mant[one_pos] &= ~one_mask;   // remove the explicit one
            mant[0] |= static_cast<fp_limb>(exponent) << (LIMB_BITS - 1 - fmt.exponent);
        } else if (daz_ || is_zero(mant)) {
            as_.warn(WARN_FLOAT_UNDERFLOW | ERR_PASS2, "underflow in floating-point constant");
            std::memset(mant, 0, sizeof mant);
        } else {
            as_.warn(WARN_FLOAT_DENORM | ERR_PASS2, "denormal floating-point constant");
        }
        break;
    }

    case FloatKind::Normal:
        exponent += expmax - 1;
        ieee_shr(mant, fmt.exponent + fmt.explicit_);
        ieee_round(rc_, minus, mant, bits);
        // Did we scale up by one?
        if (test_bit(mant, static_cast<std::size_t>(fmt.exponent + fmt.explicit_ - 1))) {
            ieee_shr(mant, 1);
            exponent++;
            if (exponent >= (expmax << 1) - 1) {
                as_.warn(WARN_FLOAT_OVERFLOW | ERR_PASS2, "overflow in floating-point constant");
                type = FloatKind::Infinity;
                goto overflow;
            }
        }
        if (!fmt.explicit_) mant[one_pos] &= ~one_mask;   // remove the explicit one
        mant[0] |= static_cast<fp_limb>(exponent) << (LIMB_BITS - 1 - fmt.exponent);
        break;

    case FloatKind::Infinity:
    case FloatKind::QNan:
    case FloatKind::SNan:
    overflow:
        std::memset(mant, 0, sizeof mant);
        mant[0] = ((fp_limb{1} << fmt.exponent) - 1) << (LIMB_BITS - 1 - fmt.exponent);
        if (fmt.explicit_) mant[one_pos] |= one_mask;
        if (type == FloatKind::QNan) set_bit(mant, static_cast<std::size_t>(fmt.exponent + fmt.explicit_ + 1));
        else if (type == FloatKind::SNan) set_bit(mant, static_cast<std::size_t>(fmt.exponent + fmt.explicit_ + fmt.mantissa));
        break;
    }

    mant[0] |= minus ? LIMB_TOP_BIT : 0;
    for (int i = 0; i < fmt.bytes; ++i) {
        const int src = fmt.bytes - 1 - i;   // bytes come out least significant first
        result[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(mant[src / LIMB_BYTES] >> (((LIMB_BYTES - 1) - (src % LIMB_BYTES)) * 8));
    }
    return true;
}

Floatize Floats::float_deffmt(int bytes) {
    int type;
    for (type = 0; type < FLOAT_ERR; type++) {
        if (kFpFormats[static_cast<std::size_t>(type)].bytes == bytes) break;
    }
    return static_cast<Floatize>(type);   // FLOAT_ERR if invalid
}

int Floats::float_option(std::string_view option) {
    std::string lower(option);
    for (char& c : lower) c = nasm_tolower(c);
    if (lower == "daz") {
        daz_ = true;
    } else if (lower == "nodaz") {
        daz_ = false;
    } else if (lower == "near") {
        rc_ = FloatRound::Near;
    } else if (lower == "down") {
        rc_ = FloatRound::Down;
    } else if (lower == "up") {
        rc_ = FloatRound::Up;
    } else if (lower == "zero") {
        rc_ = FloatRound::Zero;
    } else if (lower == "default") {
        rc_ = FloatRound::Near;
        daz_ = false;
    } else {
        return -1;   // unknown option
    }
    return 0;
}

}  // namespace cb::build::x86asm
