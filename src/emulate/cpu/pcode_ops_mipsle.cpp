// cb-unity: alone (file-local names that other files in a batch also use)
// emulate/cpu/pcode_ops_mipsle.cpp -- what the Allegrex and Emotion Engine adapters (pcode_cpu_mipsle.cpp) do themselves: every instruction the description of the closest MIPS
// does not carry the way the processor does, and every one that can fault. The FPU (the R5900's has no infinities, NaNs or denormals), coprocessors 0 and 2, the multimedia and
// 128-bit instructions, the Allegrex's extras (CLZ CLO MADD MSUB MAX MIN, EXT INS SEB SEH WSBH BITREV ROTR), the unaligned loads and stores, LL and SC, SYSCALL and BREAK and the
// traps, the machine's import stubs (the Port's intercept), the branches with their delay slots, and the faults of an access, an add or a jump that does not hold. The state is the
// processor's registers as emulate/mips_le.hpp declares them (its Cpu is the record the machines' Port reads and writes); the instruction's meaning is written here.
#include "emulate/cpu/pcode_ops_mipsle.hpp"

#include <algorithm>
#include <array>
#include <bit>
#include <cmath>
#include <cstring>
#include <format>
#include <limits>

#include "decompile/mips.hpp"

namespace cb::emulate::pcode_ops_mipsle {

namespace dm = cb::decompile::mips;
using cb::emulate::mipsle::Cpu;
using cb::emulate::mipsle::Fault;
using cb::emulate::mipsle::Model;
using cb::emulate::mipsle::Port;
using cb::emulate::mipsle::U128;

namespace {

using u32 = std::uint32_t;
using u64 = std::uint64_t;
using i32 = std::int32_t;
using i64 = std::int64_t;
using Result = std::optional<Fault>;

u64 sx32(u32 v) { return static_cast<u64>(static_cast<i64>(static_cast<i32>(v))); }
i64 s16(u32 w) { return static_cast<std::int16_t>(w & 0xFFFF); }
u32 lo32(u64 v) { return static_cast<u32>(v); }

Fault make(Fault::Kind kind, u32 at, u32 address = 0) {
    Fault f;
    f.kind = kind;
    f.pc = at;
    f.address = address;
    return f;
}

// The high 64 bits of a 64 x 64 product (unsigned), and of the signed one.
u64 mulhi_unsigned(u64 a, u64 b) {
    const u64 a_lo = a & 0xFFFFFFFF, a_hi = a >> 32, b_lo = b & 0xFFFFFFFF, b_hi = b >> 32;
    const u64 cross1 = a_hi * b_lo + ((a_lo * b_lo) >> 32);
    const u64 cross2 = a_lo * b_hi + (cross1 & 0xFFFFFFFF);
    return a_hi * b_hi + (cross1 >> 32) + (cross2 >> 32);
}

// ---- memory ------------------------------------------------------------------------------------------------------------

Result load(Cpu& c, u32 address, unsigned size, u32 at, u64& value) {
    if (address % size != 0) return make(Fault::Kind::AddressError, at, address);
    if (!c.port().read(address, size, value)) return make(Fault::Kind::BusError, at, address);
    return std::nullopt;
}

Result store(Cpu& c, u32 address, unsigned size, u32 at, u64 value) {
    if (address % size != 0) return make(Fault::Kind::AddressError, at, address);
    if (!c.port().write(address, size, value)) return make(Fault::Kind::BusError, at, address);
    return std::nullopt;
}

// The aligned 4 or 8 bytes holding `address` (for the unaligned loads and stores).
Result load_aligned(Cpu& c, u32 address, unsigned size, u32 at, u64& value) {
    if (!c.port().read(address & ~(size - 1), size, value)) return make(Fault::Kind::BusError, at, address);
    return std::nullopt;
}

Result unaligned(Cpu& c, const std::string& name, u32 w, u32 at) {
    const unsigned rt = (w >> 16) & 31;
    const u32 address = lo32(c.r[(w >> 21) & 31]) + static_cast<u32>(s16(w));
    const bool left = name == "lwl" || name == "ldl" || name == "swl" || name == "sdl";
    const bool is_load = name[0] == 'l';
    const unsigned size = name[1] == 'd' ? 8 : 4;
    u64 word = 0;
    if (const Result f = load_aligned(c, address, size, at, word)) return f;
    const unsigned k = address & (size - 1);
    const u64 all = size == 8 ? ~u64{0} : 0xFFFFFFFFull;
    const u64 reg = c.r[rt];
    u64 out;
    if (is_load) {
        if (left) {
            const unsigned shift = 8 * (size - 1 - k);
            const u64 keep = shift == 0 ? 0 : ((u64{1} << shift) - 1);
            out = ((word << shift) & all) | (reg & keep);
        } else {
            const unsigned shift = 8 * k;
            const u64 mask_keep = shift == 0 ? 0 : ~(all >> shift) & all;
            out = (word >> shift) | (reg & mask_keep);
        }
        if (size == 4) out = sx32(static_cast<u32>(out));
        if (rt != 0) c.r[rt] = out;
        return std::nullopt;
    }
    const u64 value = size == 4 ? (reg & 0xFFFFFFFFull) : reg;
    if (left) {
        const unsigned shift = 8 * (size - 1 - k);
        const u64 cleared = word & ~(all >> shift);
        out = cleared | (value >> shift);
    } else {
        const unsigned shift = 8 * k;
        const u64 keep = shift == 0 ? 0 : ((u64{1} << shift) - 1);
        out = (word & keep) | ((value << shift) & all);
    }
    if (!c.port().write(address & ~(size - 1), size, out & all)) return make(Fault::Kind::BusError, at, address);
    return std::nullopt;
}

// ---- 128-bit values and the multimedia instructions -------------------------------------------------------------------------

using Q = U128;

template <class T>
std::array<T, 16 / sizeof(T)> lanes(const Q& v) {
    std::array<T, 16 / sizeof(T)> a{};
    std::memcpy(a.data(), &v, 16);
    return a;
}
template <class T>
Q pack(const std::array<T, 16 / sizeof(T)>& a) {
    Q v;
    std::memcpy(&v, a.data(), 16);
    return v;
}

template <class T, class F>
Q lane2(const Q& a, const Q& b, F f) {
    const auto x = lanes<T>(a), y = lanes<T>(b);
    std::array<T, 16 / sizeof(T)> o{};
    for (std::size_t i = 0; i < o.size(); ++i) o[i] = f(x[i], y[i]);
    return pack<T>(o);
}
template <class T, class F>
Q lane1(const Q& a, F f) {
    const auto x = lanes<T>(a);
    std::array<T, 16 / sizeof(T)> o{};
    for (std::size_t i = 0; i < o.size(); ++i) o[i] = f(x[i]);
    return pack<T>(o);
}

template <class T>
T saturate(i64 v) {
    return static_cast<T>(std::clamp<i64>(v, std::numeric_limits<T>::min(), std::numeric_limits<T>::max()));
}

// The lanes of `a` and `b` joined: the low lanes (`upper` false) or the upper ones of the two sources, a's lane after b's (PEXT*: rt then rs).
template <class T>
Q interleave(const Q& rs, const Q& rt, bool upper) {
    const auto x = lanes<T>(rs), y = lanes<T>(rt);
    std::array<T, 16 / sizeof(T)> o{};
    const std::size_t half = o.size() / 2, base = upper ? half : 0;
    for (std::size_t i = 0; i < half; ++i) {
        o[2 * i] = y[base + i];
        o[2 * i + 1] = x[base + i];
    }
    return pack<T>(o);
}

// PPAC*: the even lanes of rt then those of rs.
template <class T>
Q pack_even(const Q& rs, const Q& rt) {
    const auto x = lanes<T>(rs), y = lanes<T>(rt);
    std::array<T, 16 / sizeof(T)> o{};
    const std::size_t half = o.size() / 2;
    for (std::size_t i = 0; i < half; ++i) {
        o[i] = y[2 * i];
        o[half + i] = x[2 * i];
    }
    return pack<T>(o);
}

// Per-64-bit-half permutations of the 16-bit and 32-bit lanes.
Q permute_halfwords(const Q& v, const std::array<unsigned, 4>& order) {
    const auto a = lanes<std::uint16_t>(v);
    std::array<std::uint16_t, 8> o{};
    for (std::size_t half = 0; half < 2; ++half)
        for (std::size_t i = 0; i < 4; ++i) o[half * 4 + i] = a[half * 4 + order[i]];
    return pack<std::uint16_t>(o);
}
Q permute_words(const Q& v, const std::array<unsigned, 4>& order) {
    const auto a = lanes<u32>(v);
    std::array<u32, 4> o{};
    for (std::size_t i = 0; i < 4; ++i) o[i] = a[order[i]];
    return pack<u32>(o);
}

Q qword_shift_right(const Q& rs, const Q& rt, unsigned bits) {   // QFSRV: (rs:rt) >> bits, the low 128 bits
    if (bits == 0) return rt;
    if (bits >= 128) {
        const unsigned s = bits - 128;
        if (s == 0) return rs;
        return {(rs.lo >> s) | (s < 64 ? rs.hi << (64 - s) : 0), s < 64 ? rs.hi >> s : 0};
    }
    Q out;
    if (bits < 64) {
        out.lo = (rt.lo >> bits) | (rt.hi << (64 - bits));
        out.hi = (rt.hi >> bits) | (rs.lo << (64 - bits));
    } else if (bits == 64) {
        out = {rt.hi, rs.lo};
    } else {
        const unsigned s = bits - 64;
        out.lo = (rt.hi >> s) | (rs.lo << (64 - s));
        out.hi = (rs.lo >> s) | (rs.hi << (64 - s));
    }
    return out;
}

// Which multimedia instruction (a word of opcode 0x1C), as the table of decompile/mips names it; true when it ran.
bool multimedia(Cpu& c, u32 w, Result& fault, u32 at) {
    const unsigned rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sa = (w >> 6) & 31, fn = w & 63;
    const Q a = c.reg128(rs), b = c.reg128(rt);
    const auto put = [&](const Q& v) {
        if (rd != 0) {
            c.r[rd] = v.lo;
            c.rh[rd] = v.hi;
        }
    };
    const auto not_run = [&] {
        const auto insn = dm::decode_word(w, at, dm::Variant::R5900);
        Fault f = make(insn ? Fault::Kind::NotRun : Fault::Kind::NoInstruction, at);
        if (insn) f.mnemonic = insn->mnemonic;
        fault = f;
    };
    switch (fn) {
        case 0x04: {   // PLZCW
            const auto count = [](u32 v) { return static_cast<u32>(std::countl_zero(static_cast<i32>(v) < 0 ? ~v : v)) - 1; };
            put({static_cast<u64>(count(lo32(a.lo))) | static_cast<u64>(count(static_cast<u32>(a.lo >> 32))) << 32, 0});
            return true;
        }
        case 0x08:   // MMI0
            switch (sa) {
                case 0x00: put(lane2<u32>(a, b, [](u32 x, u32 y) { return x + y; })); return true;
                case 0x01: put(lane2<u32>(a, b, [](u32 x, u32 y) { return x - y; })); return true;
                case 0x02: put(lane2<i32>(a, b, [](i32 x, i32 y) { return x > y ? -1 : 0; })); return true;
                case 0x03: put(lane2<i32>(a, b, [](i32 x, i32 y) { return std::max(x, y); })); return true;
                case 0x04: put(lane2<std::uint16_t>(a, b, [](std::uint16_t x, std::uint16_t y) { return static_cast<std::uint16_t>(x + y); })); return true;
                case 0x05: put(lane2<std::uint16_t>(a, b, [](std::uint16_t x, std::uint16_t y) { return static_cast<std::uint16_t>(x - y); })); return true;
                case 0x06: put(lane2<std::int16_t>(a, b, [](std::int16_t x, std::int16_t y) { return static_cast<std::int16_t>(x > y ? -1 : 0); })); return true;
                case 0x07: put(lane2<std::int16_t>(a, b, [](std::int16_t x, std::int16_t y) { return std::max(x, y); })); return true;
                case 0x08: put(lane2<std::uint8_t>(a, b, [](std::uint8_t x, std::uint8_t y) { return static_cast<std::uint8_t>(x + y); })); return true;
                case 0x09: put(lane2<std::uint8_t>(a, b, [](std::uint8_t x, std::uint8_t y) { return static_cast<std::uint8_t>(x - y); })); return true;
                case 0x0A: put(lane2<std::int8_t>(a, b, [](std::int8_t x, std::int8_t y) { return static_cast<std::int8_t>(x > y ? -1 : 0); })); return true;
                case 0x10: put(lane2<i32>(a, b, [](i32 x, i32 y) { return saturate<i32>(static_cast<i64>(x) + y); })); return true;
                case 0x11: put(lane2<i32>(a, b, [](i32 x, i32 y) { return saturate<i32>(static_cast<i64>(x) - y); })); return true;
                case 0x12: put(interleave<u32>(a, b, false)); return true;
                case 0x13: put(pack_even<u32>(a, b)); return true;
                case 0x14: put(lane2<std::int16_t>(a, b, [](std::int16_t x, std::int16_t y) { return saturate<std::int16_t>(static_cast<i64>(x) + y); })); return true;
                case 0x15: put(lane2<std::int16_t>(a, b, [](std::int16_t x, std::int16_t y) { return saturate<std::int16_t>(static_cast<i64>(x) - y); })); return true;
                case 0x16: put(interleave<std::uint16_t>(a, b, false)); return true;
                case 0x17: put(pack_even<std::uint16_t>(a, b)); return true;
                case 0x18: put(lane2<std::int8_t>(a, b, [](std::int8_t x, std::int8_t y) { return saturate<std::int8_t>(static_cast<i64>(x) + y); })); return true;
                case 0x19: put(lane2<std::int8_t>(a, b, [](std::int8_t x, std::int8_t y) { return saturate<std::int8_t>(static_cast<i64>(x) - y); })); return true;
                case 0x1A: put(interleave<std::uint8_t>(a, b, false)); return true;
                case 0x1B: put(pack_even<std::uint8_t>(a, b)); return true;
                default: break;
            }
            break;
        case 0x28:   // MMI1
            switch (sa) {
                case 0x01: put(lane1<i32>(b, [](i32 x) { return x == std::numeric_limits<i32>::min() ? std::numeric_limits<i32>::max() : (x < 0 ? -x : x); })); return true;
                case 0x02: put(lane2<u32>(a, b, [](u32 x, u32 y) { return x == y ? ~0u : 0u; })); return true;
                case 0x03: put(lane2<i32>(a, b, [](i32 x, i32 y) { return std::min(x, y); })); return true;
                case 0x04: {   // PADSBH: the low halfwords subtract, the high ones add
                    const auto x = lanes<std::int16_t>(a), y = lanes<std::int16_t>(b);
                    std::array<std::int16_t, 8> o{};
                    for (std::size_t i = 0; i < 8; ++i) o[i] = static_cast<std::int16_t>(i < 4 ? x[i] - y[i] : x[i] + y[i]);
                    put(pack<std::int16_t>(o));
                    return true;
                }
                case 0x05: put(lane1<std::int16_t>(b, [](std::int16_t x) { return static_cast<std::int16_t>(x == std::numeric_limits<std::int16_t>::min() ? 0x7FFF : (x < 0 ? -x : x)); })); return true;
                case 0x06: put(lane2<std::uint16_t>(a, b, [](std::uint16_t x, std::uint16_t y) { return static_cast<std::uint16_t>(x == y ? 0xFFFF : 0); })); return true;
                case 0x07: put(lane2<std::int16_t>(a, b, [](std::int16_t x, std::int16_t y) { return std::min(x, y); })); return true;
                case 0x0A: put(lane2<std::uint8_t>(a, b, [](std::uint8_t x, std::uint8_t y) { return static_cast<std::uint8_t>(x == y ? 0xFF : 0); })); return true;
                case 0x10: put(lane2<u32>(a, b, [](u32 x, u32 y) { return static_cast<u32>(std::min<u64>(static_cast<u64>(x) + y, 0xFFFFFFFFull)); })); return true;
                case 0x11: put(lane2<u32>(a, b, [](u32 x, u32 y) { return x > y ? x - y : 0u; })); return true;
                case 0x12: put(interleave<u32>(a, b, true)); return true;
                case 0x14: put(lane2<std::uint16_t>(a, b, [](std::uint16_t x, std::uint16_t y) { return static_cast<std::uint16_t>(std::min<u32>(static_cast<u32>(x) + y, 0xFFFF)); })); return true;
                case 0x15: put(lane2<std::uint16_t>(a, b, [](std::uint16_t x, std::uint16_t y) { return static_cast<std::uint16_t>(x > y ? x - y : 0); })); return true;
                case 0x16: put(interleave<std::uint16_t>(a, b, true)); return true;
                case 0x18: put(lane2<std::uint8_t>(a, b, [](std::uint8_t x, std::uint8_t y) { return static_cast<std::uint8_t>(std::min<u32>(static_cast<u32>(x) + y, 0xFF)); })); return true;
                case 0x19: put(lane2<std::uint8_t>(a, b, [](std::uint8_t x, std::uint8_t y) { return static_cast<std::uint8_t>(x > y ? x - y : 0); })); return true;
                case 0x1A: put(interleave<std::uint8_t>(a, b, true)); return true;
                case 0x1B: put(qword_shift_right(a, b, c.sa & 0x7F)); return true;
                default: break;
            }
            break;
        case 0x09:   // MMI2
            switch (sa) {
                case 0x02:
                case 0x03: {   // PSLLVW PSRLVW: the low word of each doubleword, by the low five bits of rs's
                    const bool left = sa == 0x02;
                    Q out;
                    const u32 w0 = lo32(b.lo), w2 = lo32(b.hi);
                    const unsigned s0 = lo32(a.lo) & 31, s2 = lo32(a.hi) & 31;
                    out.lo = sx32(left ? w0 << s0 : w0 >> s0);
                    out.hi = sx32(left ? w2 << s2 : w2 >> s2);
                    put(out);
                    return true;
                }
                case 0x08: put({c.hi[0], c.hi[1]}); return true;   // PMFHI
                case 0x09: put({c.lo[0], c.lo[1]}); return true;   // PMFLO
                case 0x0A: {   // PINTH
                    const auto x = lanes<std::uint16_t>(a), y = lanes<std::uint16_t>(b);
                    std::array<std::uint16_t, 8> o{};
                    for (std::size_t i = 0; i < 4; ++i) {
                        o[2 * i] = y[i];
                        o[2 * i + 1] = x[4 + i];
                    }
                    put(pack<std::uint16_t>(o));
                    return true;
                }
                case 0x0E: put({b.lo, a.lo}); return true;   // PCPYLD
                case 0x12: put({a.lo & b.lo, a.hi & b.hi}); return true;
                case 0x13: put({a.lo ^ b.lo, a.hi ^ b.hi}); return true;
                case 0x1A: put(permute_halfwords(b, {2, 1, 0, 3})); return true;   // PEXEH
                case 0x1B: put(permute_halfwords(b, {3, 2, 1, 0})); return true;   // PREVH
                case 0x1E: put(permute_words(b, {2, 1, 0, 3})); return true;       // PEXEW
                case 0x1F: put(permute_words(b, {1, 2, 0, 3})); return true;       // PROT3W
                default: break;
            }
            break;
        case 0x29:   // MMI3
            switch (sa) {
                case 0x03: {   // PSRAVW
                    Q out;
                    out.lo = sx32(static_cast<u32>(static_cast<i32>(lo32(b.lo)) >> (lo32(a.lo) & 31)));
                    out.hi = sx32(static_cast<u32>(static_cast<i32>(lo32(b.hi)) >> (lo32(a.hi) & 31)));
                    put(out);
                    return true;
                }
                case 0x08: c.hi = {a.lo, a.hi}; return true;   // PMTHI
                case 0x09: c.lo = {a.lo, a.hi}; return true;   // PMTLO
                case 0x0A: {   // PINTEH
                    const auto x = lanes<std::uint16_t>(a), y = lanes<std::uint16_t>(b);
                    std::array<std::uint16_t, 8> o{};
                    for (std::size_t i = 0; i < 4; ++i) {
                        o[2 * i] = y[2 * i];
                        o[2 * i + 1] = x[2 * i];
                    }
                    put(pack<std::uint16_t>(o));
                    return true;
                }
                case 0x0E: put({a.hi, b.hi}); return true;   // PCPYUD
                case 0x12: put({a.lo | b.lo, a.hi | b.hi}); return true;
                case 0x13: put({~(a.lo | b.lo), ~(a.hi | b.hi)}); return true;
                case 0x1A: put(permute_halfwords(b, {0, 2, 1, 3})); return true;   // PEXCH
                case 0x1B: {   // PCPYH
                    const auto x = lanes<std::uint16_t>(b);
                    std::array<std::uint16_t, 8> o{};
                    for (std::size_t i = 0; i < 4; ++i) {
                        o[i] = x[0];
                        o[4 + i] = x[4];
                    }
                    put(pack<std::uint16_t>(o));
                    return true;
                }
                case 0x1E: put(permute_words(b, {0, 2, 1, 3})); return true;       // PEXCW
                default: break;
            }
            break;
        case 0x34: put(lane1<std::uint16_t>(b, [sa](std::uint16_t x) { return static_cast<std::uint16_t>(x << (sa & 15)); })); return true;
        case 0x36: put(lane1<std::uint16_t>(b, [sa](std::uint16_t x) { return static_cast<std::uint16_t>(x >> (sa & 15)); })); return true;
        case 0x37: put(lane1<std::int16_t>(b, [sa](std::int16_t x) { return static_cast<std::int16_t>(x >> (sa & 15)); })); return true;
        case 0x3C: put(lane1<u32>(b, [sa](u32 x) { return x << sa; })); return true;
        case 0x3E: put(lane1<u32>(b, [sa](u32 x) { return x >> sa; })); return true;
        case 0x3F: put(lane1<i32>(b, [sa](i32 x) { return x >> sa; })); return true;
        default: break;
    }
    not_run();
    return false;
}

// ---- the FPU ------------------------------------------------------------------------------------------------------------

float as_float(u32 bits) { return std::bit_cast<float>(bits); }
u32 as_bits(float v) { return std::bit_cast<u32>(v); }

// The EE's FPU has no infinities, NaNs or denormals: an exponent of 255 is an ordinary (huge) number clamped to the largest float, a denormal is
// zero.
float ee_value(u32 bits) {
    const u32 exponent = (bits >> 23) & 0xFF;
    if (exponent == 0) return as_float(bits & 0x80000000u);
    if (exponent == 0xFF) return as_float((bits & 0x80000000u) | 0x7F7FFFFFu);
    return as_float(bits);
}
u32 ee_result(float v) {
    const u32 bits = as_bits(v);
    const u32 exponent = (bits >> 23) & 0xFF;
    if (exponent == 0) return bits & 0x80000000u;
    if (exponent == 0xFF) return (bits & 0x80000000u) | 0x7F7FFFFFu;
    return bits;
}

i32 to_word(float v, unsigned mode, bool ee) {
    if (std::isnan(v)) return std::numeric_limits<i32>::max();
    double r;
    if (ee) r = std::trunc(static_cast<double>(v));
    else
        switch (mode & 3) {
            case 0: r = std::nearbyint(static_cast<double>(v)); break;
            case 1: r = std::trunc(static_cast<double>(v)); break;
            case 2: r = std::ceil(static_cast<double>(v)); break;
            default: r = std::floor(static_cast<double>(v)); break;
        }
    if (r >= 2147483648.0) return std::numeric_limits<i32>::max();
    if (r < -2147483648.0) return std::numeric_limits<i32>::min();
    return static_cast<i32>(r);
}

void branch_to(Cpu& c, bool taken, bool likely, u32 at, u32 w) {
    if (taken) c.next_pc = at + 4 + static_cast<u32>(s16(w) * 4);
    else if (likely) {
        c.pc = c.next_pc;
        c.next_pc = c.pc + 4;
    }
}

Result fpu(Cpu& c, u32 w, u32 at) {
    const bool ee = c.model() == Model::R5900;
    const unsigned fmt = (w >> 21) & 31, rt = (w >> 16) & 31, fs = (w >> 11) & 31, fd = (w >> 6) & 31, fn = w & 63;
    const auto set = [&](unsigned reg, u64 v) {
        if (reg != 0) c.r[reg] = v;
    };
    const auto read = [&](unsigned n) { return ee ? ee_value(c.fpr[n]) : as_float(c.fpr[n]); };
    const auto write = [&](unsigned n, float v) { c.fpr[n] = ee ? ee_result(v) : as_bits(v); };
    const auto not_run = [&](u32 word) {
        const auto insn = dm::decode_word(word, at, ee ? dm::Variant::R5900 : dm::Variant::Allegrex);
        Fault f = make(insn ? Fault::Kind::NotRun : Fault::Kind::NoInstruction, at);
        if (insn) f.mnemonic = insn->mnemonic;
        return f;
    };
    switch (fmt) {
        case 0: set(rt, sx32(c.fpr[fs])); return std::nullopt;                              // MFC1
        case 2: set(rt, sx32(fs == 31 ? c.fcr31 : fs == 0 ? 0x2E00u : 0u)); return std::nullopt;   // CFC1
        case 4: c.fpr[fs] = lo32(c.r[rt]); return std::nullopt;                              // MTC1
        case 6:
            if (fs == 31) c.fcr31 = lo32(c.r[rt]);
            return std::nullopt;                                                            // CTC1
        case 8: {                                                                          // BC1F BC1T BC1FL BC1TL
            const bool cond = (c.fcr31 >> 23) & 1;
            const unsigned type = rt & 3;
            branch_to(c, (type & 1) ? cond : !cond, (type & 2) != 0, at, w);
            return std::nullopt;
        }
        default: break;
    }
    if (fmt == 20 && fn == 0x20) {   // CVT.S.W
        write(fd, static_cast<float>(static_cast<i32>(c.fpr[fs])));
        return std::nullopt;
    }
    if (fmt != 16) return not_run(w);
    const float a = read(fs), b = read(rt);
    const auto arithmetic = [&](float v) { write(fd, v); };
    switch (fn) {
        case 0x00: arithmetic(a + b); return std::nullopt;
        case 0x01: arithmetic(a - b); return std::nullopt;
        case 0x02: arithmetic(a * b); return std::nullopt;
        case 0x03:
            if (ee && b == 0) arithmetic(std::signbit(a) != std::signbit(b) ? -std::numeric_limits<float>::max() : std::numeric_limits<float>::max());
            else arithmetic(a / b);
            return std::nullopt;
        case 0x04:
            if (ee) arithmetic(std::sqrt(std::fabs(read(rt))));   // (the EE's SQRT reads FT)
            else arithmetic(std::sqrt(a));
            return std::nullopt;
        case 0x05: arithmetic(std::fabs(a)); return std::nullopt;
        case 0x06: c.fpr[fd] = c.fpr[fs]; return std::nullopt;
        case 0x07: c.fpr[fd] = c.fpr[fs] ^ 0x80000000u; return std::nullopt;
        case 0x0C: if (!ee) { c.fpr[fd] = static_cast<u32>(to_word(a, 0, false)); return std::nullopt; } break;   // ROUND.W.S
        case 0x0D: if (!ee) { c.fpr[fd] = static_cast<u32>(to_word(a, 1, false)); return std::nullopt; } break;   // TRUNC.W.S
        case 0x0E: if (!ee) { c.fpr[fd] = static_cast<u32>(to_word(a, 2, false)); return std::nullopt; } break;   // CEIL.W.S
        case 0x0F: if (!ee) { c.fpr[fd] = static_cast<u32>(to_word(a, 3, false)); return std::nullopt; } break;   // FLOOR.W.S
        case 0x24: c.fpr[fd] = static_cast<u32>(to_word(a, c.fcr31, ee)); return std::nullopt;                      // CVT.W.S
        default: break;
    }
    if (ee) {
        switch (fn) {
            case 0x16: arithmetic(b == 0 ? std::numeric_limits<float>::max() : a / std::sqrt(std::fabs(b))); return std::nullopt;   // RSQRT.S
            case 0x18: c.facc = ee_result(a + b); return std::nullopt;                                    // ADDA.S
            case 0x19: c.facc = ee_result(a - b); return std::nullopt;                                    // SUBA.S
            case 0x1A: c.facc = ee_result(a * b); return std::nullopt;                                    // MULA.S
            case 0x1C: arithmetic(ee_value(c.facc) + a * b); return std::nullopt;                         // MADD.S
            case 0x1D: arithmetic(ee_value(c.facc) - a * b); return std::nullopt;                         // MSUB.S
            case 0x1E: c.facc = ee_result(ee_value(c.facc) + a * b); return std::nullopt;                  // MADDA.S
            case 0x1F: c.facc = ee_result(ee_value(c.facc) - a * b); return std::nullopt;                  // MSUBA.S
            case 0x28: arithmetic(std::max(a, b)); return std::nullopt;                                  // MAX.S
            case 0x29: arithmetic(std::min(a, b)); return std::nullopt;                                  // MIN.S
            case 0x30: c.fcr31 &= ~(1u << 23); return std::nullopt;                                      // C.F.S
            case 0x32: c.fcr31 = (c.fcr31 & ~(1u << 23)) | (a == b ? 1u << 23 : 0); return std::nullopt;   // C.EQ.S
            case 0x34: c.fcr31 = (c.fcr31 & ~(1u << 23)) | (a < b ? 1u << 23 : 0); return std::nullopt;    // C.LT.S
            case 0x36: c.fcr31 = (c.fcr31 & ~(1u << 23)) | (a <= b ? 1u << 23 : 0); return std::nullopt;   // C.LE.S
            default: break;
        }
    } else if (fn >= 0x30) {   // the sixteen compares
        const unsigned cond = fn & 15;
        const bool unordered = std::isnan(a) || std::isnan(b);
        const bool result = ((cond & 4) != 0 && !unordered && a < b) || ((cond & 2) != 0 && !unordered && a == b) || ((cond & 1) != 0 && unordered);
        c.fcr31 = (c.fcr31 & ~(1u << 23)) | (result ? 1u << 23 : 0);
        return std::nullopt;
    }
    return not_run(w);
}


}  // namespace

// Execute the instruction at c.pc (c.next_pc follows it: a branch changes that). nullopt: it ran. A Fault: it did not finish. `fetched_word`: the instruction, when the caller has read it (the
// Port is not asked for it, nor for an intercept); else the Port is asked first for the call of an imported function, then for the word.
std::optional<Fault> step(Cpu& c, const std::optional<std::uint32_t>& fetched_word) {
    auto& r = c.r;
    auto& rh = c.rh;
    auto& hi = c.hi;
    auto& lo = c.lo;
    auto& sa = c.sa;
    auto& cop0 = c.cop0;
    auto& fpr = c.fpr;
    auto& vf = c.vf;
    auto& vi = c.vi;
    auto& pc = c.pc;
    auto& next_pc = c.next_pc;
    auto& steps = c.steps;
    Port* port_ = &c.port();
    const Model model_ = c.model();
    if (!fetched_word) {
        std::string stopped;
        switch (port_->intercept(c, stopped)) {
            case Port::Intercept::Handled: ++steps; return std::nullopt;
            case Port::Intercept::Stop: {
                Fault f = make(Fault::Kind::Syscall, pc);
                f.mnemonic = stopped;
                return f;
            }
            case Port::Intercept::None: break;
        }
    }
    const u32 at = pc;
    if (at % 4 != 0) return make(Fault::Kind::AddressError, at, at);
    u32 w = 0;
    if (fetched_word) {
        w = *fetched_word;
    } else {
        u64 fetched = 0;
        if (!port_->read(at, 4, fetched)) return make(Fault::Kind::BusError, at, at);
        w = static_cast<u32>(fetched);
    }
    const u32 saved_pc = pc, saved_next = next_pc;
    pc = next_pc;
    next_pc += 4;

    const bool ee = model_ == Model::R5900;
    const unsigned op = w >> 26, rs = (w >> 21) & 31, rt = (w >> 16) & 31, rd = (w >> 11) & 31, sh = (w >> 6) & 31, fn = w & 63;
    const u64 a = r[rs], b = r[rt];
    const i64 simm = s16(w);
    const u64 uimm = w & 0xFFFF;
    const u32 ea = lo32(a) + static_cast<u32>(simm);
    const auto set = [&](unsigned reg, u64 v) {
        if (reg != 0) r[reg] = v;
    };
    std::optional<Fault> fault;
    const auto not_run = [&] {
        const auto insn = dm::decode_word(w, at, ee ? dm::Variant::R5900 : dm::Variant::Allegrex);
        Fault f = make(insn ? Fault::Kind::NotRun : Fault::Kind::NoInstruction, at);
        if (insn) f.mnemonic = insn->mnemonic;
        fault = f;
    };
    const auto overflow = [&] { fault = make(Fault::Kind::Overflow, at); };
    const auto branch = [&](bool taken, bool likely) { branch_to(c, taken, likely, at, w); };

    switch (op) {
        case 0x00:   // SPECIAL
            switch (fn) {
                case 0x00: set(rd, sx32(lo32(b) << sh)); break;
                case 0x02:
                    if (!ee && rs == 1) set(rd, sx32(std::rotr(lo32(b), static_cast<int>(sh))));   // ROTR
                    else set(rd, sx32(lo32(b) >> sh));
                    break;
                case 0x03: set(rd, sx32(static_cast<u32>(static_cast<i32>(lo32(b)) >> sh))); break;
                case 0x04: set(rd, sx32(lo32(b) << (lo32(a) & 31))); break;
                case 0x06:
                    if (!ee && sh == 1) set(rd, sx32(std::rotr(lo32(b), static_cast<int>(lo32(a) & 31))));   // ROTRV
                    else set(rd, sx32(lo32(b) >> (lo32(a) & 31)));
                    break;
                case 0x07: set(rd, sx32(static_cast<u32>(static_cast<i32>(lo32(b)) >> (lo32(a) & 31)))); break;
                case 0x08: next_pc = lo32(a); break;
                case 0x09: next_pc = lo32(a); set(rd, sx32(at + 8)); break;
                case 0x0A: if (b == 0) set(rd, a); break;   // MOVZ
                case 0x0B: if (b != 0) set(rd, a); break;   // MOVN
                case 0x0C: {   // SYSCALL
                    std::string stopped;
                    const u32 code = (w >> 6) & 0xFFFFF;
                    if (!port_->syscall(c, code, stopped)) {
                        Fault f = make(Fault::Kind::Syscall, at);
                        f.mnemonic = stopped;
                        f.code = code;
                        fault = f;
                    }
                    break;
                }
                case 0x0D: {
                    Fault f = make(Fault::Kind::Break, at);
                    f.code = (w >> 6) & 0xFFFFF;
                    fault = f;
                    break;
                }
                case 0x0F: break;   // SYNC
                case 0x10: set(rd, hi[0]); break;
                case 0x11: hi[0] = a; break;
                case 0x12: set(rd, lo[0]); break;
                case 0x13: lo[0] = a; break;
                case 0x14: if (ee) set(rd, b << (a & 63)); else not_run(); break;                                         // DSLLV
                case 0x16:
                    if (ee) set(rd, b >> (a & 63));                                                                       // DSRLV
                    else set(rd, static_cast<u64>(std::countl_zero(lo32(a))));                                            // CLZ
                    break;
                case 0x17:
                    if (ee) set(rd, static_cast<u64>(static_cast<i64>(b) >> (a & 63)));                                  // DSRAV
                    else set(rd, static_cast<u64>(std::countl_one(lo32(a))));                                            // CLO
                    break;
                case 0x18:
                case 0x19: {   // MULT MULTU (the R5900 also writes RD)
                    const u64 product = fn == 0x18 ? static_cast<u64>(static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b))))
                                                   : static_cast<u64>(lo32(a)) * lo32(b);
                    lo[0] = sx32(static_cast<u32>(product));
                    hi[0] = sx32(static_cast<u32>(product >> 32));
                    if (ee) set(rd, lo[0]);
                    break;
                }
                case 0x1A:
                case 0x1B: {   // DIV DIVU
                    const u32 x = lo32(a), y = lo32(b);
                    if (fn == 0x1A) {
                        const i32 sx = static_cast<i32>(x), sy = static_cast<i32>(y);
                        if (sy == 0) {
                            lo[0] = sx32(sx < 0 ? 1u : 0xFFFFFFFFu);
                            hi[0] = sx32(x);
                        } else if (sx == std::numeric_limits<i32>::min() && sy == -1) {
                            lo[0] = sx32(x);
                            hi[0] = 0;
                        } else {
                            lo[0] = sx32(static_cast<u32>(sx / sy));
                            hi[0] = sx32(static_cast<u32>(sx % sy));
                        }
                    } else if (y == 0) {
                        lo[0] = sx32(0xFFFFFFFFu);
                        hi[0] = sx32(x);
                    } else {
                        lo[0] = sx32(x / y);
                        hi[0] = sx32(x % y);
                    }
                    break;
                }
                case 0x1C:
                case 0x1D:   // the Allegrex's MADD MADDU
                    if (ee) {
                        not_run();
                        break;
                    }
                    {
                        const i64 acc = static_cast<i64>(static_cast<u64>(lo32(hi[0])) << 32 | lo32(lo[0]));
                        const i64 product = fn == 0x1C ? static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b)))
                                                       : static_cast<i64>(static_cast<u64>(lo32(a)) * lo32(b));
                        const u64 sum = static_cast<u64>(acc) + static_cast<u64>(product);
                        lo[0] = sx32(static_cast<u32>(sum));
                        hi[0] = sx32(static_cast<u32>(sum >> 32));
                    }
                    break;
                case 0x20: {
                    const i32 sum = static_cast<i32>(lo32(a)) + static_cast<i32>(lo32(b));
                    if (((static_cast<i32>(lo32(a)) ^ sum) & (static_cast<i32>(lo32(b)) ^ sum)) < 0) overflow();
                    else set(rd, sx32(static_cast<u32>(sum)));
                    break;
                }
                case 0x21: set(rd, sx32(lo32(a) + lo32(b))); break;
                case 0x22: {
                    const i32 diff = static_cast<i32>(lo32(a)) - static_cast<i32>(lo32(b));
                    if (((static_cast<i32>(lo32(a)) ^ static_cast<i32>(lo32(b))) & (static_cast<i32>(lo32(a)) ^ diff)) < 0) overflow();
                    else set(rd, sx32(static_cast<u32>(diff)));
                    break;
                }
                case 0x23: set(rd, sx32(lo32(a) - lo32(b))); break;
                case 0x24: set(rd, a & b); break;
                case 0x25: set(rd, a | b); break;
                case 0x26: set(rd, a ^ b); break;
                case 0x27: set(rd, ~(a | b)); break;
                case 0x28: if (ee) set(rd, sa); else not_run(); break;                  // MFSA
                case 0x29: if (ee) sa = lo32(a) & 0xF; else not_run(); break;           // MTSA
                case 0x2A: set(rd, static_cast<i64>(a) < static_cast<i64>(b) ? 1 : 0); break;
                case 0x2B: set(rd, a < b ? 1 : 0); break;
                case 0x2C:
                    if (ee) {
                        const u64 sum = a + b;
                        if ((((a ^ sum) & (b ^ sum)) >> 63) != 0) overflow();
                        else set(rd, sum);
                    } else set(rd, static_cast<i32>(lo32(a)) > static_cast<i32>(lo32(b)) ? sx32(lo32(a)) : sx32(lo32(b)));   // MAX
                    break;
                case 0x2D:
                    if (ee) set(rd, a + b);
                    else set(rd, static_cast<i32>(lo32(a)) < static_cast<i32>(lo32(b)) ? sx32(lo32(a)) : sx32(lo32(b)));      // MIN
                    break;
                case 0x2E:
                case 0x2F:
                    if (ee) {
                        const u64 diff = a - b;
                        if (fn == 0x2E && (((a ^ b) & (a ^ diff)) >> 63) != 0) overflow();
                        else set(rd, diff);
                    } else {
                        const i64 acc = static_cast<i64>(static_cast<u64>(lo32(hi[0])) << 32 | lo32(lo[0]));
                        const i64 product = fn == 0x2E ? static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b)))
                                                       : static_cast<i64>(static_cast<u64>(lo32(a)) * lo32(b));
                        const u64 diff = static_cast<u64>(acc) - static_cast<u64>(product);
                        lo[0] = sx32(static_cast<u32>(diff));
                        hi[0] = sx32(static_cast<u32>(diff >> 32));
                    }
                    break;
                case 0x30: if (static_cast<i64>(a) >= static_cast<i64>(b)) fault = make(Fault::Kind::Trap, at); break;
                case 0x31: if (a >= b) fault = make(Fault::Kind::Trap, at); break;
                case 0x32: if (static_cast<i64>(a) < static_cast<i64>(b)) fault = make(Fault::Kind::Trap, at); break;
                case 0x33: if (a < b) fault = make(Fault::Kind::Trap, at); break;
                case 0x34: if (a == b) fault = make(Fault::Kind::Trap, at); break;
                case 0x36: if (a != b) fault = make(Fault::Kind::Trap, at); break;
                case 0x38: if (ee) set(rd, b << sh); else not_run(); break;
                case 0x3A: if (ee) set(rd, b >> sh); else not_run(); break;
                case 0x3B: if (ee) set(rd, static_cast<u64>(static_cast<i64>(b) >> sh)); else not_run(); break;
                case 0x3C: if (ee) set(rd, b << (sh + 32)); else not_run(); break;
                case 0x3E: if (ee) set(rd, b >> (sh + 32)); else not_run(); break;
                case 0x3F: if (ee) set(rd, static_cast<u64>(static_cast<i64>(b) >> (sh + 32))); else not_run(); break;
                default: not_run(); break;
            }
            break;
        case 0x01: {   // REGIMM
            const bool negative = static_cast<i64>(a) < 0;
            switch (rt) {
                case 0x00: branch(negative, false); break;
                case 0x01: branch(!negative, false); break;
                case 0x02: branch(negative, true); break;
                case 0x03: branch(!negative, true); break;
                case 0x08: if (static_cast<i64>(a) >= simm) fault = make(Fault::Kind::Trap, at); break;
                case 0x09: if (a >= static_cast<u64>(simm)) fault = make(Fault::Kind::Trap, at); break;
                case 0x0A: if (static_cast<i64>(a) < simm) fault = make(Fault::Kind::Trap, at); break;
                case 0x0B: if (a < static_cast<u64>(simm)) fault = make(Fault::Kind::Trap, at); break;
                case 0x0C: if (static_cast<i64>(a) == simm) fault = make(Fault::Kind::Trap, at); break;
                case 0x0E: if (static_cast<i64>(a) != simm) fault = make(Fault::Kind::Trap, at); break;
                case 0x10: set(31, sx32(at + 8)); branch(negative, false); break;
                case 0x11: set(31, sx32(at + 8)); branch(!negative, false); break;
                case 0x12: set(31, sx32(at + 8)); branch(negative, true); break;
                case 0x13: set(31, sx32(at + 8)); branch(!negative, true); break;
                case 0x18: if (ee) sa = ((lo32(a) ^ static_cast<u32>(simm)) & 15) * 8; else not_run(); break;                      // MTSAB
                case 0x19: if (ee) sa = ((lo32(a) ^ static_cast<u32>(simm)) & 7) * 16; else not_run(); break;                      // MTSAH
                default: not_run(); break;
            }
            break;
        }
        case 0x02: next_pc = ((at + 4) & 0xF0000000u) | ((w & 0x3FFFFFF) << 2); break;
        case 0x03: set(31, sx32(at + 8)); next_pc = ((at + 4) & 0xF0000000u) | ((w & 0x3FFFFFF) << 2); break;
        case 0x04: branch(a == b, false); break;
        case 0x05: branch(a != b, false); break;
        case 0x06: branch(static_cast<i64>(a) <= 0, false); break;
        case 0x07: branch(static_cast<i64>(a) > 0, false); break;
        case 0x08: {
            const i32 sum = static_cast<i32>(lo32(a)) + static_cast<i32>(simm);
            if (((static_cast<i32>(lo32(a)) ^ sum) & (static_cast<i32>(simm) ^ sum)) < 0) overflow();
            else set(rt, sx32(static_cast<u32>(sum)));
            break;
        }
        case 0x09: set(rt, sx32(lo32(a) + static_cast<u32>(simm))); break;
        case 0x0A: set(rt, static_cast<i64>(a) < simm ? 1 : 0); break;
        case 0x0B: set(rt, a < static_cast<u64>(simm) ? 1 : 0); break;
        case 0x0C: set(rt, a & uimm); break;
        case 0x0D: set(rt, a | uimm); break;
        case 0x0E: set(rt, a ^ uimm); break;
        case 0x0F: set(rt, sx32(static_cast<u32>(uimm) << 16)); break;
        case 0x10:   // COP0
            if (rs == 0) set(rt, sx32(cop0[rd]));
            else if (rs == 4) cop0[rd] = lo32(b);
            else if (rs == 8) {   // BC0F BC0T BC0FL BC0TL: the condition is never set
                const unsigned type = rt & 3;
                branch((type & 1) != 0, (type & 2) != 0);
            } else if (rs >= 16 && ee && fn == 0x38) cop0[12] |= 1u << 16;     // EI
            else if (rs >= 16 && ee && fn == 0x39) cop0[12] &= ~(1u << 16);    // DI
            else not_run();
            break;
        case 0x11: fault = fpu(c, w, at); break;
        case 0x12:   // COP2: VU0's register moves (the macro-mode arithmetic is not run)
            if (ee && (rs == 1)) {   // QMFC2
                set(rt, static_cast<u64>(vf[rd][1]) << 32 | vf[rd][0]);
                if (rt != 0) rh[rt] = static_cast<u64>(vf[rd][3]) << 32 | vf[rd][2];
            } else if (ee && rs == 5) {   // QMTC2
                vf[rd] = {lo32(b), static_cast<u32>(b >> 32), lo32(rh[rt]), static_cast<u32>(rh[rt] >> 32)};
            } else if (ee && rs == 2) {   // CFC2
                set(rt, sx32(vi[rd]));
            } else if (ee && rs == 6) {   // CTC2
                vi[rd] = lo32(b);
            } else not_run();
            break;
        case 0x14: branch(a == b, true); break;
        case 0x15: branch(a != b, true); break;
        case 0x16: branch(static_cast<i64>(a) <= 0, true); break;
        case 0x17: branch(static_cast<i64>(a) > 0, true); break;
        case 0x18:
        case 0x19:   // DADDI DADDIU
            if (!ee) not_run();
            else if (op == 0x19) set(rt, a + static_cast<u64>(simm));
            else {
                const u64 sum = a + static_cast<u64>(simm);
                if ((((a ^ sum) & (static_cast<u64>(simm) ^ sum)) >> 63) != 0) overflow();
                else set(rt, sum);
            }
            break;
        case 0x1A: if (ee) fault = unaligned(c, "ldl", w, at); else not_run(); break;
        case 0x1B: if (ee) fault = unaligned(c, "ldr", w, at); else not_run(); break;
        case 0x1C:   // the R5900's multimedia and second-pipeline instructions; the Allegrex's HALT and internal-register moves
            if (!ee) {
                not_run();
                break;
            }
            switch (fn) {
                case 0x00:
                case 0x01: {   // MADD MADDU (three-operand: the low word also in RD)
                    const i64 acc = static_cast<i64>(static_cast<u64>(lo32(hi[0])) << 32 | lo32(lo[0]));
                    const i64 product = fn == 0 ? static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b)))
                                                : static_cast<i64>(static_cast<u64>(lo32(a)) * lo32(b));
                    const u64 sum = static_cast<u64>(acc) + static_cast<u64>(product);
                    lo[0] = sx32(static_cast<u32>(sum));
                    hi[0] = sx32(static_cast<u32>(sum >> 32));
                    set(rd, lo[0]);
                    break;
                }
                case 0x10: set(rd, hi[1]); break;
                case 0x11: hi[1] = a; break;
                case 0x12: set(rd, lo[1]); break;
                case 0x13: lo[1] = a; break;
                case 0x18:
                case 0x19: {   // MULT1 MULTU1
                    const u64 product = fn == 0x18 ? static_cast<u64>(static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b))))
                                                   : static_cast<u64>(lo32(a)) * lo32(b);
                    lo[1] = sx32(static_cast<u32>(product));
                    hi[1] = sx32(static_cast<u32>(product >> 32));
                    set(rd, lo[1]);
                    break;
                }
                case 0x1A:
                case 0x1B: {   // DIV1 DIVU1
                    const u32 x = lo32(a), y = lo32(b);
                    if (y == 0) {
                        lo[1] = sx32(fn == 0x1A && static_cast<i32>(x) >= 0 ? 0xFFFFFFFFu : fn == 0x1A ? 1u : 0xFFFFFFFFu);
                        hi[1] = sx32(x);
                    } else if (fn == 0x1A) {
                        const i32 sx = static_cast<i32>(x), sy = static_cast<i32>(y);
                        if (sx == std::numeric_limits<i32>::min() && sy == -1) {
                            lo[1] = sx32(x);
                            hi[1] = 0;
                        } else {
                            lo[1] = sx32(static_cast<u32>(sx / sy));
                            hi[1] = sx32(static_cast<u32>(sx % sy));
                        }
                    } else {
                        lo[1] = sx32(x / y);
                        hi[1] = sx32(x % y);
                    }
                    break;
                }
                case 0x20:
                case 0x21: {   // MADD1 MADDU1
                    const i64 acc = static_cast<i64>(static_cast<u64>(lo32(hi[1])) << 32 | lo32(lo[1]));
                    const i64 product = fn == 0x20 ? static_cast<i64>(static_cast<i32>(lo32(a))) * static_cast<i64>(static_cast<i32>(lo32(b)))
                                                   : static_cast<i64>(static_cast<u64>(lo32(a)) * lo32(b));
                    const u64 sum = static_cast<u64>(acc) + static_cast<u64>(product);
                    lo[1] = sx32(static_cast<u32>(sum));
                    hi[1] = sx32(static_cast<u32>(sum >> 32));
                    set(rd, lo[1]);
                    break;
                }
                default: {
                    Result f;
                    multimedia(c, w, f, at);
                    fault = f;
                    break;
                }
            }
            break;
        case 0x1E:   // LQ
            if (ee) {
                u64 lo_half = 0, hi_half = 0;
                const u32 address = ea & ~15u;
                if (const Result f = load(c, address, 8, at, lo_half)) fault = f;
                else if (const Result g = load(c, address + 8, 8, at, hi_half)) fault = g;
                else if (rt != 0) {
                    r[rt] = lo_half;
                    rh[rt] = hi_half;
                }
            } else not_run();
            break;
        case 0x1F:
            if (ee) {   // SQ
                const u32 address = ea & ~15u;
                if (const Result f = store(c, address, 8, at, b)) fault = f;
                else fault = store(c, address + 8, 8, at, rh[rt]);
            } else {   // SPECIAL3: EXT INS and the byte and bit shuffles
                switch (fn) {
                    case 0x00: set(rt, sx32((lo32(a) >> sh) & static_cast<u32>((u64{1} << (rd + 1)) - 1))); break;   // EXT: size-1 in rd, position in sa
                    case 0x04: {                                                                                        // INS: msb in rd, lsb in sa
                        const unsigned size = rd - sh + 1;
                        const u32 mask = size == 32 ? 0xFFFFFFFFu : ((1u << size) - 1) << sh;
                        set(rt, sx32((lo32(b) & ~mask) | ((lo32(a) << sh) & mask)));
                        break;
                    }
                    case 0x20: {   // BSHFL
                        const u32 v = lo32(b);
                        switch (sh) {
                            case 0x02: set(rd, sx32(((v & 0x00FF00FFu) << 8) | ((v >> 8) & 0x00FF00FFu))); break;                       // WSBH
                            case 0x03: set(rd, sx32(((v & 0xFFu) << 24) | ((v & 0xFF00u) << 8) | ((v >> 8) & 0xFF00u) | (v >> 24))); break;                                                            // WSBW
                            case 0x10: set(rd, static_cast<u64>(static_cast<i64>(static_cast<std::int8_t>(v)))); break;                  // SEB
                            case 0x14: {                                                                                                  // BITREV
                                u32 out = 0;
                                for (unsigned i = 0; i < 32; ++i) out |= ((v >> i) & 1u) << (31 - i);
                                set(rd, sx32(out));
                                break;
                            }
                            case 0x18: set(rd, static_cast<u64>(static_cast<i64>(static_cast<std::int16_t>(v)))); break;                 // SEH
                            default: not_run(); break;
                        }
                        break;
                    }
                    default: not_run(); break;
                }
            }
            break;
        case 0x20: {
            u64 v = 0;
            if (const Result f = load(c, ea, 1, at, v)) fault = f;
            else set(rt, static_cast<u64>(static_cast<i64>(static_cast<std::int8_t>(v))));
            break;
        }
        case 0x21: {
            u64 v = 0;
            if (const Result f = load(c, ea, 2, at, v)) fault = f;
            else set(rt, static_cast<u64>(static_cast<i64>(static_cast<std::int16_t>(v))));
            break;
        }
        case 0x22: fault = unaligned(c, "lwl", w, at); break;
        case 0x23: {
            u64 v = 0;
            if (const Result f = load(c, ea, 4, at, v)) fault = f;
            else set(rt, sx32(static_cast<u32>(v)));
            break;
        }
        case 0x24: {
            u64 v = 0;
            if (const Result f = load(c, ea, 1, at, v)) fault = f;
            else set(rt, v);
            break;
        }
        case 0x25: {
            u64 v = 0;
            if (const Result f = load(c, ea, 2, at, v)) fault = f;
            else set(rt, v);
            break;
        }
        case 0x26: fault = unaligned(c, "lwr", w, at); break;
        case 0x27: {
            u64 v = 0;
            if (!ee) {
                not_run();
                break;
            }
            if (const Result f = load(c, ea, 4, at, v)) fault = f;
            else set(rt, v);
            break;
        }
        case 0x28: fault = store(c, ea, 1, at, b & 0xFF); break;
        case 0x29: fault = store(c, ea, 2, at, b & 0xFFFF); break;
        case 0x2A: fault = unaligned(c, "swl", w, at); break;
        case 0x2B: fault = store(c, ea, 4, at, b & 0xFFFFFFFFull); break;
        case 0x2C: if (ee) fault = unaligned(c, "sdl", w, at); else not_run(); break;
        case 0x2D: if (ee) fault = unaligned(c, "sdr", w, at); else not_run(); break;
        case 0x2E: fault = unaligned(c, "swr", w, at); break;
        case 0x2F: break;   // CACHE
        case 0x30: {        // LL (the Allegrex's): a load
            u64 v = 0;
            if (ee) {
                not_run();
                break;
            }
            if (const Result f = load(c, ea, 4, at, v)) fault = f;
            else set(rt, sx32(static_cast<u32>(v)));
            break;
        }
        case 0x31: {   // LWC1
            u64 v = 0;
            if (const Result f = load(c, ea, 4, at, v)) fault = f;
            else fpr[rt] = static_cast<u32>(v);
            break;
        }
        case 0x33: break;   // PREF
        case 0x36:          // LQC2
            if (ee) {
                u64 lo_half = 0, hi_half = 0;
                const u32 address = ea & ~15u;
                if (const Result f = load(c, address, 8, at, lo_half)) fault = f;
                else if (const Result g = load(c, address + 8, 8, at, hi_half)) fault = g;
                else vf[rt] = {lo32(lo_half), static_cast<u32>(lo_half >> 32), lo32(hi_half), static_cast<u32>(hi_half >> 32)};
            } else not_run();
            break;
        case 0x37: {   // LD
            u64 v = 0;
            if (!ee) {
                not_run();
                break;
            }
            if (const Result f = load(c, ea, 8, at, v)) fault = f;
            else set(rt, v);
            break;
        }
        case 0x38:   // SC (the Allegrex's): a store that always succeeds
            if (ee) {
                not_run();
                break;
            }
            fault = store(c, ea, 4, at, b & 0xFFFFFFFFull);
            if (!fault) set(rt, 1);
            break;
        case 0x39: fault = store(c, ea, 4, at, fpr[rt]); break;   // SWC1
        case 0x3E:                                              // SQC2
            if (ee) {
                const u32 address = ea & ~15u;
                if (const Result g = store(c, address, 8, at, static_cast<u64>(vf[rt][1]) << 32 | vf[rt][0])) fault = g;
                else fault = store(c, address + 8, 8, at, static_cast<u64>(vf[rt][3]) << 32 | vf[rt][2]);
            } else not_run();
            break;
        case 0x3F: if (ee) fault = store(c, ea, 8, at, b); else not_run(); break;   // SD
        default: not_run(); break;
    }
    if (fault) {
        if (fault->kind != Fault::Kind::Break && fault->kind != Fault::Kind::Syscall) {
            pc = saved_pc;
            next_pc = saved_next;
        }
        return fault;
    }
    r[0] = 0;
    rh[0] = 0;
    ++steps;
    return std::nullopt;
}

}  // namespace cb::emulate::pcode_ops_mipsle
