// cb/emulate/powerpc.cpp -- see powerpc.hpp.
#include "cb/emulate/powerpc.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>
#include <mutex>

#include <intrin.h>
#include <stdlib.h>

namespace cb::emulate::powerpc {

namespace {

constexpr std::uint64_t kM64 = ~std::uint64_t{0};
constexpr std::uint64_t kM32 = 0xFFFFFFFFull;
constexpr std::uint64_t kQuietNan = 0x7FF8000000000000ull;   // Python's math.nan

constexpr std::int64_t s64(std::uint64_t v) { return static_cast<std::int64_t>(v); }
constexpr std::int64_t s32(std::uint64_t v) { return static_cast<std::int32_t>(static_cast<std::uint32_t>(v)); }

double nan_value() { return to_double(kQuietNan); }

}  // namespace

std::string_view to_text(StopReason reason) {
    switch (reason) {
        case StopReason::Alignment: return "alignment";
        case StopReason::Invalid: return "invalid";
        case StopReason::Memory: return "memory";
        case StopReason::Syscall: return "syscall";
        case StopReason::Trap: return "trap";
        case StopReason::Unsupported: return "unsupported";
    }
    return "unsupported";
}

Fault::Fault(StopReason reason) : EmulationError(std::string("powerpc: ") + std::string(to_text(reason))), reason_(reason) {}

// ---- arithmetic helpers --------------------------------------------------------------------------------------------

std::uint64_t mask(unsigned mb, unsigned me) {
    const std::uint64_t from_mb = mb == 0 ? kM64 : (std::uint64_t{1} << (64 - mb)) - 1;   // bits mb..63 (the low 64 - mb bits)
    const std::uint64_t below_me = (std::uint64_t{1} << (63 - me)) - 1;                   // bits me+1..63
    return mb <= me ? (from_mb & ~below_me) : (from_mb | ~below_me);
}

std::uint64_t rotl64(std::uint64_t v, unsigned n) {
    n &= 63;
    return n ? std::rotl(v, static_cast<int>(n)) : v;
}

std::uint64_t rotl32(std::uint64_t v, unsigned n) {
    const std::uint64_t r = std::rotl(static_cast<std::uint32_t>(v), static_cast<int>(n & 31));
    return r | (r << 32);
}

std::uint64_t sign_extend(std::uint64_t v, unsigned bits) {
    if (bits >= 64) return v;
    v &= (std::uint64_t{1} << bits) - 1;
    return (v & (std::uint64_t{1} << (bits - 1))) ? v - (std::uint64_t{1} << bits) : v;
}

double to_double(std::uint64_t bits) { return std::bit_cast<double>(bits); }
std::uint64_t to_bits(double x) { return std::bit_cast<std::uint64_t>(x); }

double round_to_single(double x) { return static_cast<double>(static_cast<float>(x)); }

namespace {

// A double as a single's four bytes, big-endian, as a 32-bit word.
std::uint32_t single_word(double x) { return std::bit_cast<std::uint32_t>(static_cast<float>(x)); }
double single_from_word(std::uint32_t w) { return static_cast<double>(std::bit_cast<float>(w)); }

// A floating value as the integer an fctiw / fctid makes of it: round to nearest (even) or toward zero, saturating at
// the destination's ends (NaN gives the most negative).
std::int64_t to_integer(double x, unsigned bits, bool toward_zero) {
    const std::int64_t lo = bits == 64 ? std::numeric_limits<std::int64_t>::min() : -(std::int64_t{1} << (bits - 1));
    const std::int64_t hi = bits == 64 ? std::numeric_limits<std::int64_t>::max() : (std::int64_t{1} << (bits - 1)) - 1;
    if (std::isnan(x)) return lo;
    if (x >= static_cast<double>(hi)) return hi;
    if (x <= static_cast<double>(lo)) return lo;
    return static_cast<std::int64_t>(toward_zero ? std::trunc(x) : std::nearbyint(x));
}

// (the sum mod 2**64, the carry out)
std::pair<std::uint64_t, bool> add_with_carry(std::uint64_t a, std::uint64_t b, unsigned carry) {
    unsigned long long sum = 0;
    const unsigned char out = _addcarry_u64(static_cast<unsigned char>(carry), a, b, &sum);
    return {sum, out != 0};
}

// The low `size` bytes of v (2 or 4) in the other order.
std::uint64_t swap_bytes(std::uint64_t v, unsigned size) {
    return size == 4 ? _byteswap_ulong(static_cast<unsigned long>(v)) : _byteswap_ushort(static_cast<unsigned short>(v));
}

// ---- the load / store / logic tables of the opcode groups ----------------------------------------------------------

struct LoadSpec {
    unsigned size;
    bool sign;
    bool update;
};
struct StoreSpec {
    unsigned size;
    bool update;
};
struct IndexedLoad {
    unsigned size;
    bool sign, update, swap;
};
struct IndexedStore {
    unsigned size;
    bool update, swap;
};

std::optional<LoadSpec> load_spec(unsigned op) {
    switch (op) {
        case 32: return LoadSpec{4, false, false};   // lwz
        case 33: return LoadSpec{4, false, true};    // lwzu
        case 34: return LoadSpec{1, false, false};   // lbz
        case 35: return LoadSpec{1, false, true};    // lbzu
        case 40: return LoadSpec{2, false, false};   // lhz
        case 41: return LoadSpec{2, false, true};    // lhzu
        case 42: return LoadSpec{2, true, false};    // lha
        case 43: return LoadSpec{2, true, true};     // lhau
        default: return std::nullopt;
    }
}

std::optional<StoreSpec> store_spec(unsigned op) {
    switch (op) {
        case 36: return StoreSpec{4, false};   // stw
        case 37: return StoreSpec{4, true};    // stwu
        case 38: return StoreSpec{1, false};   // stb
        case 39: return StoreSpec{1, true};    // stbu
        case 44: return StoreSpec{2, false};   // sth
        case 45: return StoreSpec{2, true};    // sthu
        default: return std::nullopt;
    }
}

// indexed loads of opcode 31 by extended opcode: xo -> (size, signed, update, byte-reversed)
std::optional<IndexedLoad> indexed_load(unsigned xo) {
    switch (xo) {
        case 23: return IndexedLoad{4, false, false, false};    // lwzx
        case 55: return IndexedLoad{4, false, true, false};     // lwzux
        case 87: return IndexedLoad{1, false, false, false};    // lbzx
        case 119: return IndexedLoad{1, false, true, false};    // lbzux
        case 279: return IndexedLoad{2, false, false, false};   // lhzx
        case 311: return IndexedLoad{2, false, true, false};    // lhzux
        case 343: return IndexedLoad{2, true, false, false};    // lhax
        case 375: return IndexedLoad{2, true, true, false};     // lhaux
        case 341: return IndexedLoad{4, true, false, false};    // lwax
        case 373: return IndexedLoad{4, true, true, false};     // lwaux
        case 21: return IndexedLoad{8, false, false, false};    // ldx
        case 53: return IndexedLoad{8, false, true, false};     // ldux
        case 534: return IndexedLoad{4, false, false, true};    // lwbrx
        case 790: return IndexedLoad{2, false, false, true};    // lhbrx
        case 20: return IndexedLoad{4, false, false, false};    // lwarx
        case 84: return IndexedLoad{8, false, false, false};    // ldarx
        default: return std::nullopt;
    }
}

std::optional<IndexedStore> indexed_store(unsigned xo) {
    switch (xo) {
        case 151: return IndexedStore{4, false, false};   // stwx
        case 183: return IndexedStore{4, true, false};    // stwux
        case 215: return IndexedStore{1, false, false};   // stbx
        case 247: return IndexedStore{1, true, false};    // stbux
        case 407: return IndexedStore{2, false, false};   // sthx
        case 439: return IndexedStore{2, true, false};    // sthux
        case 149: return IndexedStore{8, false, false};   // stdx
        case 181: return IndexedStore{8, true, false};    // stdux
        case 662: return IndexedStore{4, false, true};    // stwbrx
        case 918: return IndexedStore{2, false, true};    // sthbrx
        default: return std::nullopt;
    }
}

// The logical operations of opcode 31: a result for (rs, rb), or none for another xo.
std::optional<std::uint64_t> logical(unsigned xo, std::uint64_t a, std::uint64_t b) {
    switch (xo) {
        case 28: return a & b;      // and
        case 60: return a & ~b;     // andc
        case 444: return a | b;     // or
        case 412: return a | ~b;    // orc
        case 316: return a ^ b;     // xor
        case 124: return ~(a | b);  // nor
        case 476: return ~(a & b);  // nand
        case 284: return ~(a ^ b);  // eqv
        default: return std::nullopt;
    }
}

// The condition-register logic of opcode 19: a bit for (a, b), or none for another xo.
std::optional<unsigned> cr_logic(unsigned xo, unsigned a, unsigned b) {
    switch (xo) {
        case 257: return (a & b) & 1;       // crand
        case 449: return (a | b) & 1;       // cror
        case 193: return (a ^ b) & 1;       // crxor
        case 225: return ~(a & b) & 1;      // crnand
        case 33: return ~(a | b) & 1;       // crnor
        case 289: return ~(a ^ b) & 1;      // creqv
        case 129: return (a & ~b) & 1;      // crandc
        case 417: return (a | ~b) & 1;      // crorc
        default: return std::nullopt;
    }
}

bool trap_taken(unsigned to, std::int64_t a, std::int64_t b, std::uint64_t ua, std::uint64_t ub) {
    return ((to & 16) && a < b) || ((to & 8) && a > b) || ((to & 4) && a == b) || ((to & 2) && ua < ub) || ((to & 1) && ua > ub);
}

// Does a conditional branch with these BO and BI fields go? (decrements CTR when BO says so)
bool branch_taken(Machine& m, unsigned bo, unsigned bi) {
    bool ok = true;
    if (!(bo & 4)) {
        m.ctr -= 1;
        ok = (m.ctr != 0) != ((bo & 2) != 0);
    }
    if (ok && !(bo & 16)) ok = m.cr_bit(bi) == (((bo >> 3) & 1) != 0);
    return ok;
}

// ---- the instruction groups -----------------------------------------------------------------------------------------

void memory_op(Machine& m, unsigned op, std::uint32_t w, unsigned rt, unsigned ra, std::uint64_t simm) {
    auto& r = m.r;
    if (op == 58 || op == 62) simm = sign_extend(w & 0xFFFC, 16);
    const std::uint64_t base = ra ? r[ra] : 0;
    const std::uint32_t ea = static_cast<std::uint32_t>(base + simm);
    if (const auto load = load_spec(op)) {
        const std::uint64_t v = m.read(ea, load->size);
        r[rt] = load->sign ? sign_extend(v, 8 * load->size) : v;
        if (load->update) r[ra] = base + simm;
    } else if (const auto store = store_spec(op)) {
        m.write(ea, store->size, r[rt]);
        if (store->update) r[ra] = base + simm;
    } else if (op == 58) {
        const unsigned kind = w & 3;
        if (kind == 2) {   // lwa
            r[rt] = sign_extend(m.read(ea, 4), 32);
        } else {           // ld, ldu
            r[rt] = m.read(ea, 8);
            if (kind == 1) r[ra] = base + simm;
        }
    } else if (op == 62) {   // std, stdu
        m.write(ea, 8, r[rt]);
        if ((w & 3) == 1) r[ra] = base + simm;
    } else if (op == 46) {   // lmw
        for (unsigned i = rt; i < 32; ++i) r[i] = m.read(static_cast<std::uint32_t>(ea + 4ull * (i - rt)), 4);
    } else if (op == 47) {   // stmw
        for (unsigned i = rt; i < 32; ++i) m.write(static_cast<std::uint32_t>(ea + 4ull * (i - rt)), 4, r[i]);
    } else if (op == 48 || op == 49) {   // lfs, lfsu
        m.f[rt] = to_bits(single_from_word(static_cast<std::uint32_t>(m.read(ea, 4))));
        if (op == 49) r[ra] = base + simm;
    } else if (op == 50 || op == 51) {   // lfd, lfdu
        m.f[rt] = m.read(ea, 8);
        if (op == 51) r[ra] = base + simm;
    } else if (op == 52 || op == 53) {   // stfs, stfsu
        m.write(ea, 4, single_word(to_double(m.f[rt])));
        if (op == 53) r[ra] = base + simm;
    } else if (op == 54 || op == 55) {   // stfd, stfdu
        m.write(ea, 8, m.f[rt]);
        if (op == 55) r[ra] = base + simm;
    } else {
        throw Fault(StopReason::Unsupported);
    }
}

void record_cr0(Machine& m, std::uint64_t v) {   // CR0 from a result, as a signed 64-bit number
    const std::int64_t s = s64(v);
    m.set_cr(0, s < 0 ? 8 : s > 0 ? 4 : 2);
}

void compare_signed(Machine& m, unsigned field, std::int64_t a, std::int64_t b) { m.set_cr(field, a < b ? 8 : a > b ? 4 : 2); }
void compare_unsigned(Machine& m, unsigned field, std::uint64_t a, std::uint64_t b) { m.set_cr(field, a < b ? 8 : a > b ? 4 : 2); }

void arithmetic(Machine& m, unsigned xo, unsigned rt, unsigned ra, unsigned rb, bool rc) {
    auto& r = m.r;
    const std::uint64_t a = r[ra], b = r[rb];
    std::uint64_t v = 0;
    auto carried = [&](std::uint64_t x, std::uint64_t y, unsigned cin) {
        const auto [sum, out] = add_with_carry(x, y, cin);
        m.ca = out;
        return sum;
    };
    switch (xo) {
        case 266: v = a + b; break;                           // add
        case 40: v = b - a; break;                            // subf
        case 10: v = carried(a, b, 0); break;                 // addc
        case 8: v = carried(~a, b, 1); break;                 // subfc
        case 138: v = carried(a, b, m.ca); break;             // adde
        case 136: v = carried(~a, b, m.ca); break;            // subfe
        case 202: v = carried(a, 0, m.ca); break;             // addze
        case 200: v = carried(~a, 0, m.ca); break;            // subfze
        case 234: v = carried(a, kM64, m.ca); break;          // addme
        case 232: v = carried(~a, kM64, m.ca); break;         // subfme
        case 104: v = 0 - a; break;                           // neg
        case 235: v = static_cast<std::uint64_t>(s32(a) * s32(b)); break;                          // mullw
        case 75: v = static_cast<std::uint64_t>((s32(a) * s32(b)) >> 32) & kM32; break;           // mulhw
        case 11: v = ((a & kM32) * (b & kM32)) >> 32; break;                                       // mulhwu
        case 233: v = static_cast<std::uint64_t>(s64(a) * s64(b)); break;                          // mulld (the low 64 bits)
        case 73: v = static_cast<std::uint64_t>(__mulh(s64(a), s64(b))); break;                    // mulhd
        case 9: v = __umulh(a, b); break;                                                          // mulhdu
        case 491: {                                                                                // divw
            const std::int64_t x = s32(a), y = s32(b);
            v = (y == 0 || (x == -0x80000000ll && y == -1)) ? 0 : static_cast<std::uint64_t>(x / y) & kM32;
            break;
        }
        case 459: v = (b & kM32) == 0 ? 0 : (a & kM32) / (b & kM32); break;                        // divwu
        case 489: {                                                                                // divd
            const std::int64_t x = s64(a), y = s64(b);
            v = (y == 0 || (x == std::numeric_limits<std::int64_t>::min() && y == -1)) ? 0 : static_cast<std::uint64_t>(x / y);
            break;
        }
        case 457: v = b == 0 ? 0 : a / b; break;                                                   // divdu
        default: throw Fault(StopReason::Unsupported);
    }
    r[rt] = v;
    if (rc) record_cr0(m, v);
}

void float_other(Machine& m, std::uint32_t w, unsigned xo, unsigned rt, unsigned ra, unsigned rb) {
    auto& f = m.f;
    const double b = to_double(f[rb]);
    switch (xo) {
        case 72: f[rt] = f[rb]; break;                                         // fmr
        case 40: f[rt] = f[rb] ^ (std::uint64_t{1} << 63); break;              // fneg
        case 264: f[rt] = f[rb] & ~(std::uint64_t{1} << 63); break;            // fabs
        case 136: f[rt] = f[rb] | (std::uint64_t{1} << 63); break;             // fnabs
        case 12: f[rt] = to_bits(round_to_single(b)); break;                   // frsp
        case 0:
        case 32: {                                                             // fcmpu, fcmpo
            const double a = to_double(f[ra]);
            m.set_cr(rt >> 2, (std::isnan(a) || std::isnan(b)) ? 1 : a < b ? 8 : a > b ? 4 : 2);
            break;
        }
        case 14:
        case 15: f[rt] = static_cast<std::uint64_t>(to_integer(b, 32, xo == 15)) & kM32; break;     // fctiw, fctiwz
        case 814:
        case 815: f[rt] = static_cast<std::uint64_t>(to_integer(b, 64, xo == 815)); break;          // fctid, fctidz
        case 846: f[rt] = to_bits(static_cast<double>(s64(f[rb]))); break;                          // fcfid
        case 583: f[rt] = 0; break;                                                                 // mffs: no status is kept
        case 711:
        case 134:
        case 70:
        case 38:
        case 64: break;                                                                             // status register moves
        default: throw Fault(StopReason::Unsupported);
    }
    (void)w;
}

void floating(Machine& m, unsigned op, std::uint32_t w, unsigned rt, unsigned ra, unsigned rb) {
    auto& f = m.f;
    const unsigned rc5 = (w >> 6) & 31;
    const unsigned xo5 = (w >> 1) & 31;
    const unsigned xo = (w >> 1) & 0x3FF;
    const bool single = op == 59;
    const double a = to_double(f[ra]), b = to_double(f[rb]), c = to_double(f[rc5]);
    double v = 0;
    switch (xo5) {
        case 21: v = a + b; break;
        case 20: v = a - b; break;
        case 25: v = a * c; break;
        case 18:
            v = b != 0 ? a / b : (a == 0 || std::isnan(a)) ? nan_value() : std::copysign(std::numeric_limits<double>::infinity(), a) * std::copysign(1.0, b);
            break;
        case 29: v = a * c + b; break;
        case 28: v = a * c - b; break;
        case 31: v = -(a * c + b); break;
        case 30: v = -(a * c - b); break;
        case 22: v = b >= 0 ? std::sqrt(b) : nan_value(); break;
        case 24: v = b != 0 ? 1.0 / b : std::copysign(std::numeric_limits<double>::infinity(), b); break;
        case 26: v = b > 0 ? 1.0 / std::sqrt(b) : (b == 0 ? std::numeric_limits<double>::infinity() : nan_value()); break;
        default:
            if (xo5 == 23 && !single) {   // fsel
                f[rt] = a >= 0.0 ? f[rc5] : f[rb];
                return;
            }
            if (single) throw Fault(StopReason::Unsupported);
            float_other(m, w, xo, rt, ra, rb);
            return;
    }
    f[rt] = to_bits(single ? round_to_single(v) : v);
}

void extended(Machine& m, std::uint32_t w, unsigned rt, unsigned ra, unsigned rb) {
    auto& r = m.r;
    const unsigned xo = (w >> 1) & 0x3FF;
    const bool rc = w & 1;
    const std::uint64_t base = ra ? r[ra] : 0;
    if (const auto load = indexed_load(xo)) {
        const std::uint32_t ea = static_cast<std::uint32_t>(base + r[rb]);
        std::uint64_t v = m.read(ea, load->size);
        if (load->swap) v = swap_bytes(v, load->size);
        r[rt] = load->sign ? sign_extend(v, 8 * load->size) : v;
        if (load->update) r[ra] = base + r[rb];
        if (xo == 20 || xo == 84) m.reserve = ea;   // lwarx, ldarx: a reservation
        return;
    }
    if (const auto store = indexed_store(xo)) {
        const std::uint32_t ea = static_cast<std::uint32_t>(base + r[rb]);
        std::uint64_t v = store->size == 8 ? r[rt] : r[rt] & ((std::uint64_t{1} << (8 * store->size)) - 1);
        if (store->swap) v = swap_bytes(v, store->size);
        m.write(ea, store->size, v);
        if (store->update) r[ra] = base + r[rb];
        return;
    }
    if (const auto v = logical(xo, r[rt], r[rb])) {
        r[ra] = *v;
        if (rc) record_cr0(m, r[ra]);
        return;
    }
    if (xo == 150 || xo == 214) {   // stwcx., stdcx.: stored while the reservation lwarx/ldarx made holds (one processor: it does, until this)
        if (m.reserve) {
            m.write(static_cast<std::uint32_t>(base + r[rb]), xo == 150 ? 4 : 8, r[rt]);
            m.set_cr(0, 2);
        } else {
            m.set_cr(0, 0);
        }
        m.reserve.reset();
        return;
    }
    if (xo == 0) {   // cmp
        if (rt & 1) {
            compare_signed(m, rt >> 2, s64(r[ra]), s64(r[rb]));
        } else {
            compare_signed(m, rt >> 2, s32(r[ra]), s32(r[rb]));
        }
        return;
    }
    if (xo == 32) {   // cmpl
        if (rt & 1) {
            compare_unsigned(m, rt >> 2, r[ra], r[rb]);
        } else {
            compare_unsigned(m, rt >> 2, r[ra] & kM32, r[rb] & kM32);
        }
        return;
    }
    if (xo == 339) {   // mfspr
        const unsigned spr = ((w >> 16) & 31) | (((w >> 11) & 31) << 5);
        r[rt] = spr == 8 ? m.lr : spr == 9 ? m.ctr : spr == 1 ? std::uint64_t{m.ca} << 29 : 0;   // time base and the like read as zero
        return;
    }
    if (xo == 467) {   // mtspr
        const unsigned spr = ((w >> 16) & 31) | (((w >> 11) & 31) << 5);
        if (spr == 8) {
            m.lr = static_cast<std::uint32_t>(r[rt]);
        } else if (spr == 9) {
            m.ctr = r[rt];
        } else if (spr == 1) {
            m.ca = ((r[rt] >> 29) & 1) != 0;
        }
        return;
    }
    if (xo == 19) {   // mfcr
        r[rt] = m.cr;
        return;
    }
    if (xo == 144) {   // mtcrf
        const unsigned fxm = (w >> 12) & 0xFF;
        for (unsigned i = 0; i < 8; ++i) {
            if (fxm & (0x80u >> i)) m.set_cr(i, static_cast<unsigned>((r[rt] >> (28 - 4 * i)) & 0xF));
        }
        return;
    }
    if (xo == 371 || xo == 83) {   // mftb, mfmsr
        r[rt] = 0;
        return;
    }
    if (xo == 278 || xo == 246 || xo == 86 || xo == 54 || xo == 598 || xo == 982 || xo == 854) return;   // cache hints, sync, eieio
    if (xo == 1014) {   // dcbz: the 32-byte block is cleared
        const std::uint32_t ea = static_cast<std::uint32_t>(base + r[rb]) & ~31u;
        for (unsigned i = 0; i < 32; i += 8) m.write(ea + i, 8, 0);
        return;
    }
    if (xo == 535 || xo == 567 || xo == 599 || xo == 631 || xo == 663 || xo == 695 || xo == 727 || xo == 759 || xo == 983) {
        const std::uint32_t ea = static_cast<std::uint32_t>(base + r[rb]);
        if (xo == 535 || xo == 567) {   // lfsx, lfsux
            m.f[rt] = to_bits(single_from_word(static_cast<std::uint32_t>(m.read(ea, 4))));
        } else if (xo == 599 || xo == 631) {   // lfdx, lfdux
            m.f[rt] = m.read(ea, 8);
        } else if (xo == 663 || xo == 695) {   // stfsx, stfsux
            m.write(ea, 4, single_word(to_double(m.f[rt])));
        } else if (xo == 727 || xo == 759) {   // stfdx, stfdux
            m.write(ea, 8, m.f[rt]);
        } else {   // stfiwx
            m.write(ea, 4, m.f[rt] & kM32);
        }
        if (xo == 567 || xo == 631 || xo == 695 || xo == 759) r[ra] = base + r[rb];
        return;
    }
    if (xo == 4 || xo == 68) {   // tw, td
        const bool wide = xo == 68;
        const std::int64_t a = wide ? s64(r[ra]) : s32(r[ra]), b = wide ? s64(r[rb]) : s32(r[rb]);
        const std::uint64_t ua = wide ? r[ra] : r[ra] & kM32, ub = wide ? r[rb] : r[rb] & kM32;
        if (trap_taken(rt, a, b, ua, ub)) throw Fault(StopReason::Trap);
        return;
    }
    // shifts and the rest
    std::uint64_t v = 0;
    if (xo == 24) {   // slw
        const unsigned n = r[rb] & 63;
        v = n < 32 ? (r[rt] << n) & kM32 : 0;
    } else if (xo == 536) {   // srw
        const unsigned n = r[rb] & 63;
        v = n < 32 ? (r[rt] & kM32) >> n : 0;
    } else if (xo == 792 || xo == 824) {   // sraw, srawi
        const unsigned n = xo == 792 ? static_cast<unsigned>(r[rb] & 63) : rb;
        const std::int64_t s = s32(r[rt]);
        if (n > 31) {
            v = s < 0 ? kM64 : 0;
            m.ca = s < 0;
        } else {
            v = static_cast<std::uint64_t>(s >> n);
            m.ca = s < 0 && (r[rt] & ((std::uint64_t{1} << n) - 1)) != 0;
        }
    } else if (xo == 27) {   // sld
        const unsigned n = r[rb] & 127;
        v = n < 64 ? r[rt] << n : 0;
    } else if (xo == 539) {   // srd
        const unsigned n = r[rb] & 127;
        v = n < 64 ? r[rt] >> n : 0;
    } else if (xo == 794 || (xo >> 1) == 413) {   // srad, sradi
        const unsigned n = xo == 794 ? static_cast<unsigned>(r[rb] & 127) : (rb | (((w >> 1) & 1) << 5));
        const std::int64_t s = s64(r[rt]);
        if (n > 63) {
            v = s < 0 ? kM64 : 0;
            m.ca = s < 0;
        } else {
            v = static_cast<std::uint64_t>(s >> n);
            m.ca = s < 0 && (r[rt] & ((std::uint64_t{1} << n) - 1)) != 0;
        }
    } else if (xo == 26) {   // cntlzw
        v = static_cast<std::uint64_t>(std::countl_zero(static_cast<std::uint32_t>(r[rt])));
    } else if (xo == 58) {   // cntlzd
        v = static_cast<std::uint64_t>(std::countl_zero(r[rt]));
    } else if (xo == 954) {   // extsb
        v = sign_extend(r[rt], 8);
    } else if (xo == 922) {   // extsh
        v = sign_extend(r[rt], 16);
    } else if (xo == 986) {   // extsw
        v = sign_extend(r[rt], 32);
    } else {
        arithmetic(m, xo & 0x1FF, rt, ra, rb, rc);
        return;
    }
    r[ra] = v;
    if (rc) record_cr0(m, v);
}

}  // namespace

// ---- Machine -------------------------------------------------------------------------------------------------------

PageImage PageImage::of(std::span<const Region> regions) {
    auto pages = std::make_shared<std::unordered_map<std::uint32_t, Page>>();
    for (const Region& region : regions) {
        std::size_t pos = 0;
        while (pos < region.data.size()) {
            const std::uint32_t a = static_cast<std::uint32_t>(region.address + pos);
            const std::size_t off = a & (kPageSize - 1);
            const std::size_t n = std::min<std::size_t>(kPageSize - off, region.data.size() - pos);
            auto [it, inserted] = pages->try_emplace(a >> 12);
            if (inserted) it->second.fill(0);
            std::memcpy(it->second.data() + off, region.data.data() + pos, n);
            pos += n;
        }
    }
    PageImage image;
    image.pages_ = std::move(pages);
    return image;
}

const Page* PageImage::find(std::uint32_t page_number) const {
    if (!pages_) return nullptr;
    const auto it = pages_->find(page_number);
    return it == pages_->end() ? nullptr : &it->second;
}

Machine::Machine(std::span<const Region> regions, PageImage shared) : shared_(std::move(shared)) {
    for (const Region& region : regions) load(region.address, region.data);
}

void Machine::set_under(PageImage under) {
    under_ = std::move(under);
    ro_key_ = ~0u;
}

void Machine::load(std::uint32_t va, ByteView data) {
    std::size_t pos = 0;
    while (pos < data.size()) {
        const std::uint32_t a = static_cast<std::uint32_t>(va + pos);
        Page& page = page_for_write(a >> 12);
        const std::size_t off = a & (kPageSize - 1);
        const std::size_t n = std::min<std::size_t>(kPageSize - off, data.size() - pos);
        std::memcpy(page.data() + off, data.data() + pos, n);
        pos += n;
    }
}

Page& Machine::page_for_write(std::uint32_t key) {
    if (rw_key_ == key) return *rw_page_;
    auto it = pages_.find(key);
    if (it == pages_.end()) {
        if (pages_.size() >= kMaxPages) throw Fault(StopReason::Memory);
        const Page* base = shared_.find(key);
        if (base == nullptr) base = under_.find(key);
        auto page = std::make_unique<Page>();
        if (base != nullptr) {
            *page = *base;
        } else {
            page->fill(0);
        }
        it = pages_.emplace(key, std::move(page)).first;
        ro_key_ = ~0u;   // a page that was read from the shared image is shadowed now
    }
    rw_key_ = key;
    rw_page_ = it->second.get();
    return *rw_page_;
}

const Page& Machine::page_for_read(std::uint32_t key) {
    if (rw_key_ == key) return *rw_page_;
    if (ro_key_ == key) return *ro_page_;
    const auto it = pages_.find(key);
    if (it != pages_.end()) {
        rw_key_ = key;
        rw_page_ = it->second.get();
        return *rw_page_;
    }
    const Page* base = shared_.find(key);
    if (base == nullptr) base = under_.find(key);
    if (base != nullptr) {   // read, not written: not copied
        ro_key_ = key;
        ro_page_ = base;
        return *base;
    }
    return page_for_write(key);   // memory nobody gave: a zero page, created by being touched
}

const Page* Machine::peek(std::uint32_t address) const {
    const std::uint32_t key = address >> 12;
    if (const auto it = pages_.find(key); it != pages_.end()) return it->second.get();
    if (const Page* p = shared_.find(key)) return p;
    return under_.find(key);
}

const Page* Machine::find_written(std::uint32_t page_number) const {
    const auto it = pages_.find(page_number);
    return it == pages_.end() ? nullptr : it->second.get();
}

const Page* Machine::find_unwritten(std::uint32_t page_number) const {
    if (const Page* p = shared_.find(page_number)) return p;
    return under_.find(page_number);
}

std::uint64_t Machine::read(std::uint32_t address, unsigned size) {
    const unsigned off = address & (kPageSize - 1);
    std::uint64_t v = 0;
    if (off + size <= kPageSize) {
        const Page& page = page_for_read(address >> 12);
        for (unsigned i = 0; i < size; ++i) v = v << 8 | page[off + i];
        return v;
    }
    for (unsigned i = 0; i < size; ++i) {   // across a page boundary: each byte finds its own page
        const std::uint32_t x = address + i;
        v = v << 8 | page_for_read(x >> 12)[x & (kPageSize - 1)];
    }
    return v;
}

void Machine::write(std::uint32_t address, unsigned size, std::uint64_t value) {
    const bool track = !(kStackLow <= address && address < kStackTop);   // what the stack holds is not the function's output
    for (unsigned i = 0; i < size; ++i) {
        const std::uint32_t x = address + i;
        const std::uint8_t byte = static_cast<std::uint8_t>(value >> (8 * (size - 1 - i)));
        Page& page = page_for_write(x >> 12);
        std::uint8_t& slot = page[x & (kPageSize - 1)];
        if (track) {
            if (auto* old = writes_.find(x)) {
                old->second = byte;
            } else {
                writes_.insert_or_assign(x, std::pair<std::uint8_t, std::uint8_t>{slot, byte});
            }
        }
        slot = byte;
    }
}

void Machine::set_cr(unsigned field, unsigned bits) {
    const unsigned shift = 28 - 4 * field;
    cr = (cr & ~(0xFu << shift)) | (bits << shift);
}

// ---- fills ---------------------------------------------------------------------------------------------------------

std::optional<Fill> fill_from_text(std::string_view name) {
    if (name == "pointers") return Fill::Pointers;
    if (name == "counts") return Fill::Counts;
    if (name == "text") return Fill::Text;
    return std::nullopt;
}

Bytes filled(Fill kind, std::size_t size) {
    Bytes out;
    out.reserve(size);
    switch (kind) {
        case Fill::Pointers:
            for (std::size_t a = 0; a < size; ++a) {   // every word holds the address 0x100 past itself
                const std::uint32_t word = kArgsBase + static_cast<std::uint32_t>(a & ~std::size_t{3}) + 0x100;
                out.push_back(static_cast<std::uint8_t>(word >> (8 * (3 - (a & 3)))));
            }
            break;
        case Fill::Counts:
            for (std::size_t a = 0; a < size; ++a) {   // words 1, 2, 3, ...
                const std::uint32_t word = static_cast<std::uint32_t>(a / 4 + 1);
                out.push_back(static_cast<std::uint8_t>(word >> (8 * (3 - (a & 3)))));
            }
            break;
        case Fill::Text:
            for (std::size_t a = 0; a < size; ++a) out.push_back(a % 24 == 23 ? 0 : static_cast<std::uint8_t>(0x41 + a % 24));
            break;
        case Fill::None: break;
    }
    return out;
}

const PageImage& fill_image(Fill kind) {
    static std::mutex mutex;
    static PageImage images[4];   // by Fill, built when first asked for
    static bool built[4] = {false, false, false, false};
    const std::lock_guard<std::mutex> lock(mutex);
    const std::size_t i = static_cast<std::size_t>(kind);
    if (!built[i]) {
        if (kind != Fill::None) {
            const Region region{kArgsBase, filled(kind)};
            images[i] = PageImage::of(std::span<const Region>(&region, 1));
        }
        built[i] = true;
    }
    return images[i];
}

// ---- one instruction -----------------------------------------------------------------------------------------------

void step(Machine& m, std::uint32_t w) {
    const unsigned op = w >> 26;
    const unsigned rt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
    auto& r = m.r;
    std::uint32_t next = m.pc + 4;
    const std::uint64_t simm = sign_extend(w & 0xFFFF, 16);
    const std::uint64_t uimm = w & 0xFFFF;
    auto branch_to = [&](std::uint64_t displacement, bool absolute) {
        next = static_cast<std::uint32_t>(absolute ? displacement : m.pc + displacement);
    };

    if (op == 14) {   // addi
        r[rt] = (ra ? r[ra] : 0) + simm;
    } else if (op == 15) {   // addis
        r[rt] = (ra ? r[ra] : 0) + sign_extend(uimm << 16, 32);
    } else if ((op >= 32 && op <= 55) || op == 58 || op == 62) {
        memory_op(m, op, w, rt, ra, simm);
    } else if (op == 18) {   // b
        const std::uint64_t d = sign_extend(w & 0x03FFFFFC, 26);
        if (w & 1) m.lr = next;
        branch_to(d, (w & 2) != 0);
    } else if (op == 16) {   // bc
        if (branch_taken(m, rt, ra)) {
            const std::uint64_t d = sign_extend(w & 0xFFFC, 16);
            if (w & 1) m.lr = next;
            branch_to(d, (w & 2) != 0);
        } else if (w & 1) {
            m.lr = next;
        }
    } else if (op == 19) {
        const unsigned xo = (w >> 1) & 0x3FF;
        if (xo == 16) {   // bclr
            const std::uint32_t target = m.lr;
            if (branch_taken(m, rt, ra)) {
                if (w & 1) m.lr = next;
                next = target & 0xFFFFFFFCu;
            } else if (w & 1) {
                m.lr = next;
            }
        } else if (xo == 528) {   // bcctr
            if (branch_taken(m, rt | 4, ra)) {
                if (w & 1) m.lr = next;
                next = static_cast<std::uint32_t>(m.ctr) & 0xFFFFFFFCu;
            } else if (w & 1) {
                m.lr = next;
            }
        } else if (xo == 0) {   // mcrf
            m.set_cr(rt >> 2, (m.cr >> (28 - 4 * (ra >> 2))) & 0xF);
        } else if (const auto bit = cr_logic(xo, m.cr_bit(ra), m.cr_bit(rb))) {
            m.cr = (m.cr & ~(1u << (31 - rt))) | (*bit << (31 - rt));
        } else if (xo == 150) {   // isync
        } else {
            throw Fault(StopReason::Unsupported);
        }
    } else if (op == 31) {
        extended(m, w, rt, ra, rb);
    } else if (op == 24) {
        r[ra] = r[rt] | uimm;
    } else if (op == 25) {
        r[ra] = r[rt] | (uimm << 16);
    } else if (op == 26) {
        r[ra] = r[rt] ^ uimm;
    } else if (op == 27) {
        r[ra] = r[rt] ^ (uimm << 16);
    } else if (op == 28) {
        r[ra] = r[rt] & uimm;
        record_cr0(m, r[ra]);
    } else if (op == 29) {
        r[ra] = r[rt] & (uimm << 16);
        record_cr0(m, r[ra]);
    } else if (op == 11) {   // cmpi
        if (rt & 1) {
            compare_signed(m, rt >> 2, s64(r[ra]), s64(simm));
        } else {
            compare_signed(m, rt >> 2, s32(r[ra]), s64(simm));
        }
    } else if (op == 10) {   // cmpli
        compare_unsigned(m, rt >> 2, (rt & 1) ? r[ra] : r[ra] & kM32, uimm);
    } else if (op == 12 || op == 13) {   // addic, addic.
        const auto [sum, out] = add_with_carry(r[ra], simm, 0);
        r[rt] = sum;
        m.ca = out;
        if (op == 13) record_cr0(m, r[rt]);
    } else if (op == 8) {   // subfic
        const auto [sum, out] = add_with_carry(~r[ra], simm, 1);
        r[rt] = sum;
        m.ca = out;
    } else if (op == 7) {   // mulli
        r[rt] = static_cast<std::uint64_t>(s64(r[ra]) * s64(simm));
    } else if (op == 21) {   // rlwinm
        const unsigned mb = (w >> 6) & 31, me = (w >> 1) & 31;
        r[ra] = rotl32(r[rt], rb) & mask(mb + 32, me + 32);
        if (w & 1) record_cr0(m, r[ra]);
    } else if (op == 20) {   // rlwimi
        const unsigned mb = (w >> 6) & 31, me = (w >> 1) & 31;
        const std::uint64_t k = mask(mb + 32, me + 32);
        r[ra] = (rotl32(r[rt], rb) & k) | (r[ra] & ~k);
        if (w & 1) record_cr0(m, r[ra]);
    } else if (op == 23) {   // rlwnm
        const unsigned mb = (w >> 6) & 31, me = (w >> 1) & 31;
        r[ra] = rotl32(r[rt], static_cast<unsigned>(r[rb] & 31)) & mask(mb + 32, me + 32);
        if (w & 1) record_cr0(m, r[ra]);
    } else if (op == 30) {   // the 64-bit rotates
        const unsigned sh = rb | (((w >> 1) & 1) << 5);
        const unsigned mbe = ((w >> 6) & 31) | (((w >> 5) & 1) << 5);
        const unsigned kind = (w >> 2) & 7;
        std::uint64_t v = 0;
        if (kind == 0) {   // rldicl
            v = rotl64(r[rt], sh) & mask(mbe, 63);
        } else if (kind == 1) {   // rldicr
            v = rotl64(r[rt], sh) & mask(0, mbe);
        } else if (kind == 2) {   // rldic
            v = rotl64(r[rt], sh) & mask(mbe, 63 - sh);
        } else if (kind == 3) {   // rldimi
            const std::uint64_t k = mask(mbe, 63 - sh);
            v = (rotl64(r[rt], sh) & k) | (r[ra] & ~k);
        } else {
            const unsigned n = r[rb] & 63;
            if (((w >> 1) & 0xF) == 8) {   // rldcl
                v = rotl64(r[rt], n) & mask(mbe, 63);
            } else if (((w >> 1) & 0xF) == 9) {   // rldcr
                v = rotl64(r[rt], n) & mask(0, mbe);
            } else {
                throw Fault(StopReason::Unsupported);
            }
        }
        r[ra] = v;
        if (w & 1) record_cr0(m, v);
    } else if (op == 59 || op == 63) {
        floating(m, op, w, rt, ra, rb);
    } else if (op == 3 || op == 2) {   // twi, tdi: a trap is taken only if its test holds
        const bool wide = op == 2;
        const std::int64_t a = wide ? s64(r[ra]) : s32(r[ra]), b = s64(simm);
        const std::uint64_t ua = wide ? r[ra] : r[ra] & kM32, ub = wide ? simm : simm & kM32;
        if (trap_taken(rt, a, b, ua, ub)) throw Fault(StopReason::Trap);
    } else if (op == 17) {   // sc
        throw Fault(StopReason::Syscall);
    } else {
        throw Fault(StopReason::Unsupported);
    }
    m.pc = next;
}

// ---- the branch log --------------------------------------------------------------------------------------------------

namespace {

// The branch log of run(branches=true), for an input generator (verify/guided): each conditional branch -- (address, taken,
// compare) -- where compare is what the last compare into the condition field it tests compared, or none when that field
// was last set some other way. A register's "where" is the memory it was last loaded from, if nothing has written it since.
class BranchLog {
public:
    static constexpr std::size_t kCap = 4096;
    std::vector<BranchEvent> events;

    void step(Machine& m, std::uint32_t pc, std::uint32_t w) {
        const unsigned op = w >> 26;
        const unsigned rt = (w >> 21) & 31, ra = (w >> 16) & 31, rb = (w >> 11) & 31;
        const unsigned xo = (w >> 1) & 0x3FF;
        auto& r = m.r;
        std::optional<std::pair<unsigned, Compare>> compare;   // (field, what it compared)
        std::optional<unsigned> branch;                        // the field a branch tests
        std::optional<Loaded> load;
        unsigned load_reg = 0;
        if (op == 10 || op == 11 || (op == 31 && (xo == 0 || xo == 32))) {
            const unsigned size = (rt & 1) ? 8 : 4;
            const std::uint64_t mask = size == 8 ? kM64 : kM32;
            Compare c;
            c.size = size;
            c.a = r[ra] & mask;
            c.a_from = where(ra, size);
            if (op == 11) {
                c.b = sign_extend(w & 0xFFFF, 16) & mask;
            } else if (op == 10) {
                c.b = w & 0xFFFF;
            } else {
                c.b = r[rb] & mask;
                c.b_from = where(rb, size);
            }
            compare = std::pair<unsigned, Compare>{rt >> 2, c};
        } else if ((op == 16 || (op == 19 && xo == 16)) && (rt & 0x14) != 0x14) {
            if (!(rt & 16)) branch = ra >> 2;   // (the field it tests; none: the counter alone)
        } else if (load_spec(op) || (op == 58 && ((w & 3) == 0 || (w & 3) == 2))) {
            const unsigned size = load_spec(op) ? load_spec(op)->size : ((w & 3) == 0 ? 8 : 4);
            const std::uint64_t d = sign_extend(w & (op == 58 ? 0xFFFC : 0xFFFF), 16);
            load = Loaded{static_cast<std::uint32_t>((ra ? r[ra] : 0) + d), size};
            load_reg = rt;
        }
        const std::uint32_t cr = m.cr;
        step_one(m, w);
        if (compare) {
            field_[compare->first] = compare->second;
        } else if (cr != m.cr) {   // a field set by something else
            for (unsigned f = 0; f < 8; ++f) {
                if (((cr ^ m.cr) >> (28 - 4 * f)) & 0xF) field_[f].reset();
            }
        }
        if (op == 16 || (op == 19 && xo == 16)) {
            if ((rt & 0x14) != 0x14 && events.size() < kCap) {
                events.push_back({pc, m.pc != pc + 4, branch ? field_[*branch] : std::nullopt});
            }
        } else if (!compare) {
            if (!(store_spec(op) && !store_spec(op)->update)) loaded_[rt].reset();   // (a plain store writes no register)
            loaded_[ra].reset();
            if (load && load_reg != ra) loaded_[load_reg] = load;
        }
    }

private:
    struct Loaded {
        std::uint32_t address;
        unsigned size;
    };
    static void step_one(Machine& m, std::uint32_t w) { powerpc::step(m, w); }

    std::optional<std::uint32_t> where(unsigned reg, unsigned size) const {
        const auto& at = loaded_[reg];
        if (!at || at->size < size) return std::nullopt;
        return static_cast<std::uint32_t>(at->address + (at->size > size ? at->size - size : 0));
    }

    std::array<std::optional<Compare>, 8> field_{};
    std::array<std::optional<Loaded>, 32> loaded_{};
};

}  // namespace

// ---- running a function --------------------------------------------------------------------------------------------

std::string RunResult::exit_text() const {
    switch (exit) {
        case Exit::None: return "none";
        case Exit::Ret: return "ret";
        case Exit::Exception: return std::format("exception:{}", stopped ? to_text(*stopped) : std::string_view("unsupported"));
    }
    return "none";
}

RunResult run(std::span<const Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const RunOptions& options) {
    auto machine = std::make_shared<Machine>(regions, options.shared);
    Machine& m = *machine;
    if (options.fill != Fill::None) m.set_under(fill_image(options.fill));
    for (std::size_t i = 0; i < args.size() && i < 8; ++i) m.r[3 + i] = args[i];
    for (std::size_t i = 0; i < options.fargs.size() && i < 13; ++i) m.f[1 + i] = to_bits(options.fargs[i]);
    for (unsigned i = 14; i < 32; ++i) m.r[i] = kSavedRegisterBase + i;   // values a function must hand back unchanged
    m.r[1] = kEntryStackPointer;
    m.r[13] = 0x7E000000;   // per-thread data pointer of these targets
    m.r[2] = 0;
    m.lr = kReturnAddress;
    m.pc = entry;
    const auto saved_at_entry = [&] {
        std::array<std::uint64_t, 19> s;
        for (unsigned i = 14; i < 32; ++i) s[i - 14] = m.r[i];
        s[18] = m.r[1];
        return s;
    };
    const auto entry_saved = saved_at_entry();

    RunResult out;
    const AddressRange span = options.span.value_or(AddressRange{0, 0});
    std::optional<BranchLog> log;
    if (options.branches) log.emplace();
    // the addresses of the span that ran, in order of first arrival
    std::vector<bool> seen;
    if (span.hi > span.lo) seen.assign((span.hi - span.lo) / 4 + 1, false);
    std::optional<std::uint32_t> last, linked;   // the last instruction; where a call from it went
    std::uint64_t count = 0;

    auto call_out = [&](std::string name) {   // a function outside what was given: record the call, return through lr
        Call call;
        call.name = std::move(name);
        for (unsigned i = 0; i < 8; ++i) call.args[i] = m.r[3 + i];
        out.calls.push_back(std::move(call));
        m.r[3] = options.stub_ret;
        m.pc = m.lr;
        if (options.shadow != nullptr) options.shadow->after_call(m, true, options.stub_ret);
    };

    try {
        for (;;) {
            const std::uint32_t pc = m.pc;
            if (pc == kReturnAddress) {
                out.exit = Exit::Ret;
                break;
            }
            if (options.externs != nullptr) {
                if (const auto it = options.externs->find(pc); it != options.externs->end()) {
                    if (out.calls.size() >= kMaxCalls) {
                        out.truncated = true;
                        break;
                    }
                    call_out(it->second);
                    continue;
                }
            }
            if (count >= options.max_steps) {
                out.truncated = true;
                break;
            }
            ++count;
            if (span.contains(pc)) {
                const std::size_t slot = (pc - span.lo) / 4;
                if ((pc & 3) == 0 && !seen[slot]) {
                    seen[slot] = true;
                    out.path.push_back(pc);
                } else if ((pc & 3) != 0 && std::find(out.path.begin(), out.path.end(), pc) == out.path.end()) {
                    out.path.push_back(pc);   // (an unaligned address: it fails the next check, but it ran)
                }
            }
            if (pc & 3) throw Fault(StopReason::Alignment);
            const std::uint32_t w = static_cast<std::uint32_t>(m.read(pc, 4));
            if (w == 0) {
                if (options.absent && options.absent->contains(pc) && linked == pc && last && m.lr == *last + 4) {
                    // Called, and nothing is there: code outside what was given (a partial image calls into the rest of its
                    // title). A call out, like one to `externs`.
                    if (out.calls.size() >= kMaxCalls) {
                        out.truncated = true;
                        break;
                    }
                    call_out(std::format("@{:08x}", pc));
                    continue;
                }
                throw Fault(StopReason::Invalid);
            }
            last = pc;
            linked.reset();
            if (options.shadow != nullptr) options.shadow->step(m, pc);
            if (log && span.contains(pc)) {
                log->step(m, pc, w);
            } else {
                step(m, w);
            }
            const unsigned op = w >> 26;
            if ((w & 1) && (op == 16 || op == 18 || op == 19)) linked = m.pc;   // a branch that links: where it went
        }
    } catch (const Fault& fault) {   // the guest stopped: that is how the run ends
        out.exit = Exit::Exception;
        out.stopped = fault.reason();
    }
    out.ret = m.r[3];
    out.fret = m.f[1];
    out.steps = count;
    out.pc = m.pc;
    for (const auto& [address, bytes] : m.writes()) {
        if (bytes.first != bytes.second) out.writes.push_back({address, bytes.first, bytes.second});
    }
    out.saved = saved_at_entry() == entry_saved;
    out.machine = std::move(machine);
    if (log) out.branches = std::move(log->events);
    return out;
}

cb::emulate::Run record(const RunResult& result) {
    cb::emulate::Run run;
    run.exit = result.exit_text();
    run.results["r3"] = result.ret;
    run.results["f1"] = result.fret;
    for (const ByteWrite& w : result.writes) {
        cb::emulate::MemoryWrite write;
        write.address = w.address;
        write.size = 1;
        write.value = w.after;
        write.old = w.before;
        run.writes.push_back(write);
    }
    for (const Call& c : result.calls) {
        cb::emulate::Call call;
        call.name = c.name;
        call.args = std::vector<std::int64_t>();
        for (std::uint64_t a : c.args) call.args->push_back(static_cast<std::int64_t>(a));
        run.calls.push_back(std::move(call));
    }
    if (result.machine) {
        const Machine& m = *result.machine;
        cb::emulate::Preserved preserved;
        for (unsigned i = 14; i < 32; ++i) preserved[std::format("r{}", i)] = {kSavedRegisterBase + i, m.r[i]};
        preserved["r1"] = {std::uint64_t{kEntryStackPointer}, m.r[1]};
        run.preserved = std::move(preserved);
    }
    for (std::uint32_t a : result.path) run.path.push_back(a);
    run.steps = result.steps;
    run.truncated = result.truncated;
    run.endian = cb::emulate::Endian::Big;
    run.raw = result;
    return run;
}

cb::emulate::Run recorded(std::span<const Region> regions, std::uint32_t entry, std::span<const std::uint64_t> args, const RunOptions& options) {
    return record(run(regions, entry, args, options));
}

}  // namespace cb::emulate::powerpc
