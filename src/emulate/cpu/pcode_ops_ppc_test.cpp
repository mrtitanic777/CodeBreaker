// cb-unity: alone (file-local names that other files in a batch also use)
// The vector operations of the PowerPC descriptions (engine/pcode_ops_ppc_vector.cpp), each run as an instruction of the description on the one emulator and compared with a second model
// written here in AltiVec's own terms: bytes in the order the manual numbers them (element 0 the most significant), plain integer and host float arithmetic, no lane library.
#include <array>
#include <bit>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <deque>
#include <format>
#include <functional>
#include <map>
#include <random>
#include <string>
#include <vector>

#include "core/testing.hpp"
#include "emulate/powerpc.hpp"
#include "emulate/cpu/pcode_cpu.hpp"
#include "emulate/cpu/pcode_cpu_internal.hpp"
#include "emulate/cpu/pcode_cpu_ppc.hpp"
#include "emulate/cpu/pcode_ops_ppc.hpp"

namespace cb {
namespace {

namespace po = emulate::pcode_ops_ppc;
namespace pw = cb::emulate::powerpc;
using emulate::pcode_cpu::ppc_probe;
using V = std::array<std::uint8_t, 16>;   // a vector register in AltiVec's order: byte 0 is element 0, the most significant

// ---- elements, as the manual numbers them ---------------------------------------------------------------------------------------------------------------------------------

std::uint64_t el(const V& v, unsigned ew, unsigned i) {
    std::uint64_t x = 0;
    for (unsigned k = 0; k < ew; ++k) x = (x << 8) | v[i * ew + k];
    return x;
}
void put(V& v, unsigned ew, unsigned i, std::uint64_t x) {
    for (unsigned k = 0; k < ew; ++k) v[i * ew + k] = static_cast<std::uint8_t>(x >> (8 * (ew - 1 - k)));
}
std::int64_t sx(std::uint64_t x, unsigned ew) {
    const unsigned shift = 64 - 8 * ew;
    return static_cast<std::int64_t>(x << shift) >> shift;
}
std::uint64_t cut(std::int64_t x, unsigned ew) { return ew >= 8 ? static_cast<std::uint64_t>(x) : static_cast<std::uint64_t>(x) & ((std::uint64_t{1} << (8 * ew)) - 1); }
float flt(const V& v, unsigned i) { return std::bit_cast<float>(static_cast<std::uint32_t>(el(v, 4, i))); }
void put_flt(V& v, unsigned i, float f) { put(v, 4, i, std::bit_cast<std::uint32_t>(f)); }

// ---- running an instruction ------------------------------------------------------------------------------------------------------------------------------------------------------

struct Ram {   // big-endian memory for the loads and stores
    std::map<std::uint32_t, std::uint8_t> bytes;
};

class Rig {
public:
    explicit Rig(po::Flavor flavor) : probe_(ppc_probe(flavor)) {
        probe_.emu.onRead([this](std::string_view space, std::uint64_t a, unsigned size, std::uint64_t& value) {
            if (!emulate::pcode_cpu::is_ram(space)) return false;
            value = 0;
            for (unsigned i = 0; i < size; ++i) value = (value << 8) | ram.bytes[static_cast<std::uint32_t>(a + i)];
            return true;
        });
        probe_.emu.onWrite([this](std::string_view space, std::uint64_t a, unsigned size, std::uint64_t value) {
            if (!emulate::pcode_cpu::is_ram(space)) return false;
            for (unsigned i = 0; i < size; ++i) ram.bytes[static_cast<std::uint32_t>(a + i)] = static_cast<std::uint8_t>(value >> (8 * (size - 1 - i)));
            return true;
        });
        probe_.emu.onUserOp([](decompile::sleigh::Emulator&, const std::string&, const std::vector<std::uint64_t>&, std::uint64_t&) { return false; });
    }
    ~Rig() {
        probe_.emu.onRead({});
        probe_.emu.onWrite({});
        probe_.emu.onUserOp({});
    }
    Ram ram;
    void set(unsigned reg, const V& v) {
        std::array<std::uint8_t, 16> le;
        for (unsigned i = 0; i < 16; ++i) le[i] = v[15 - i];
        probe_.emu.writeRegWide(probe_.regs.vr[reg], le.data());
    }
    V get(unsigned reg) const {
        std::array<std::uint8_t, 16> le;
        probe_.emu.readRegWide(probe_.regs.vr[reg], le.data());
        V v;
        for (unsigned i = 0; i < 16; ++i) v[i] = le[15 - i];
        return v;
    }
    void set_gpr(unsigned reg, std::uint64_t value) { probe_.emu.setReg(probe_.regs.gpr[reg], value); }
    std::uint64_t cr6() const { return probe_.emu.reg(probe_.regs.cr[6]); }
    std::uint32_t& vscr() { return probe_.vector.vscr; }
    bool step(std::uint32_t word) {   // one instruction at 0x1000: whether it ran
        const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
        probe_.emu.write(0x1000, bytes, 4);
        probe_.emu.setPc(0x1000);
        probe_.vector.word = word;
        return probe_.emu.step() == decompile::sleigh::Emulator::Status::Ok;
    }
    const po::VectorContext& context() const { return probe_.vector; }
    std::string error() const { return probe_.emu.lastError(); }
    bool decodes(const std::uint8_t (&bytes)[4]) {
        probe_.emu.write(0x1000, bytes, 4);
        return probe_.emu.length(0x1000) == 4;
    }

private:
    emulate::pcode_cpu::PpcProbe probe_;
};

constexpr unsigned kD = 5, kA = 6, kB = 7, kC = 8;
std::uint32_t vx(unsigned xo, unsigned d = kD, unsigned a = kA, unsigned b = kB) { return (4u << 26) | (d << 21) | (a << 16) | (b << 11) | xo; }
std::uint32_t va(unsigned xo6, unsigned d = kD, unsigned a = kA, unsigned b = kB, unsigned c = kC) { return (4u << 26) | (d << 21) | (a << 16) | (b << 11) | (c << 6) | xo6; }

V random_vector(std::mt19937& rng, unsigned style) {
    V v;
    for (auto& b : v) {
        switch ((rng() >> 3) % 6) {
            case 0: b = 0; break;
            case 1: b = 0xFF; break;
            case 2: b = 0x80; break;
            case 3: b = 0x7F; break;
            default: b = static_cast<std::uint8_t>(rng());
        }
    }
    if (style == 1) {   // floats of every kind: ordinary, huge, tiny, denormal, infinite, NaN
        for (unsigned i = 0; i < 4; ++i) {
            std::uint32_t bits = static_cast<std::uint32_t>(rng());
            switch (rng() % 10) {
                case 0: bits = 0x7F800000u | (rng() & 0x80000000u); break;                        // infinity
                case 1: bits = 0x7FC00000u | (rng() & 0x003FFFFFu); break;                        // a quiet NaN
                case 2: bits = 0x7F800001u | (rng() & 0x003FFFFFu); break;                        // a signaling NaN
                case 3: bits = rng() & 0x807FFFFFu; break;                                        // a denormal or zero
                case 4: bits = (rng() & 0x80000000u) | 0x3F800000u | (rng() & 0x7FFFFFu); break;  // about one
                case 5: bits = (rng() & 0x80000000u) | ((100 + rng() % 60) << 23) | (rng() & 0x7FFFFFu); break;   // large
                default: bits = (rng() & 0x80000000u) | ((110 + rng() % 35) << 23) | (rng() & 0x7FFFFFu); break;  // moderate
            }
            put(v, 4, i, bits);
        }
    }
    return v;
}

const char* keep(std::string s) {   // a name that lives as long as the test
    static std::deque<std::string> names;
    names.push_back(std::move(s));
    return names.back().c_str();
}

struct Case {
    const char* name;
    std::function<std::uint32_t()> word;                                                          // the instruction (registers kD, kA, kB, kC)
    std::function<V(const V& a, const V& b, const V& c, std::uint32_t& vscr, unsigned imm)> model;   // vD; the model may set VSCR[SAT]
    unsigned style = 0;     // 1: floats
    unsigned imm = 0;       // the immediate field (UIMM or SIMM, in the vA field)
};

bool both_nan(std::uint32_t x, std::uint32_t y) { return std::isnan(std::bit_cast<float>(x)) && std::isnan(std::bit_cast<float>(y)); }

// Whether two results agree: bit for bit, but a NaN is any NaN where `loose` (the processor's choice among several NaNs is not checked here, the library's rules are checked by its own test).
bool same_result(const V& x, const V& y, bool floats) {
    if (x == y) return true;
    if (!floats) return false;
    for (unsigned i = 0; i < 4; ++i)
        if (el(x, 4, i) != el(y, 4, i) && !both_nan(static_cast<std::uint32_t>(el(x, 4, i)), static_cast<std::uint32_t>(el(y, 4, i)))) return false;
    return true;
}

}  // namespace

// ---- the models --------------------------------------------------------------------------------------------------------------------------------------------------------------

namespace {

template <class F>
V each(const V& a, const V& b, unsigned ew, F f) {
    V r{};
    for (unsigned i = 0; i < 16 / ew; ++i) put(r, ew, i, f(el(a, ew, i), el(b, ew, i)));
    return r;
}

std::uint64_t clamp_to(std::int64_t x, std::int64_t lo, std::int64_t hi, unsigned ew, std::uint32_t& vscr) {
    if (x < lo) {
        vscr |= 1;
        x = lo;
    } else if (x > hi) {
        vscr |= 1;
        x = hi;
    }
    return cut(x, ew);
}
std::int64_t smin(unsigned ew) { return -(std::int64_t{1} << (8 * ew - 1)); }
std::int64_t smax(unsigned ew) { return (std::int64_t{1} << (8 * ew - 1)) - 1; }
std::int64_t umax(unsigned ew) { return static_cast<std::int64_t>((std::uint64_t{1} << (8 * ew)) - 1); }

std::vector<Case> integer_cases() {
    std::vector<Case> c;
    const auto lane_op = [&](const char* name, unsigned xo, unsigned ew, std::function<std::uint64_t(std::uint64_t, std::uint64_t, std::uint32_t&)> f) {
        c.push_back({name, [xo] { return vx(xo); }, [ew, f](const V& a, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 16 / ew; ++i) put(r, ew, i, f(el(a, ew, i), el(b, ew, i), vscr));
                         return r;
                     }});
    };
    struct Width {
        unsigned ew;
        const char* tag;   // b h w
    };
    const Width widths[3] = {{1, "b"}, {2, "h"}, {4, "w"}};
    // add and subtract, modulo and saturating (the xo of the byte form; the halfword form is +64, the word form +128)
    for (const Width& w : widths) {
        const unsigned ew = w.ew, k = ew == 1 ? 0 : ew == 2 ? 64 : 128;
        const auto name = [&](const std::string& s) { return keep(s); };
        if (ew == 1) lane_op(name("vaddubm"), 0, 1, [](auto a, auto b, auto&) { return cut(static_cast<std::int64_t>(a + b), 1); });
        if (ew == 2) lane_op(name("vadduhm"), 64, 2, [](auto a, auto b, auto&) { return cut(static_cast<std::int64_t>(a + b), 2); });
        if (ew == 4) lane_op(name("vadduwm"), 128, 4, [](auto a, auto b, auto&) { return cut(static_cast<std::int64_t>(a + b), 4); });
        lane_op(name(std::format("vaddu{}s", w.tag)), 512 + (ew == 1 ? 0 : ew == 2 ? 64 : 128), ew, [ew](auto a, auto b, auto& vscr) { return clamp_to(static_cast<std::int64_t>(a + b), 0, umax(ew), ew, vscr); });
        lane_op(name(std::format("vadds{}s", w.tag)), 768 + k, ew, [ew](auto a, auto b, auto& vscr) { return clamp_to(sx(a, ew) + sx(b, ew), smin(ew), smax(ew), ew, vscr); });
        lane_op(name(std::format("vsubu{}m", w.tag)), 1024 + k, ew, [ew](auto a, auto b, auto&) { return cut(static_cast<std::int64_t>(a - b), ew); });
        lane_op(name(std::format("vsubu{}s", w.tag)), 1536 + k, ew, [ew](auto a, auto b, auto& vscr) { return clamp_to(static_cast<std::int64_t>(a) - static_cast<std::int64_t>(b), 0, umax(ew), ew, vscr); });
        lane_op(name(std::format("vsubs{}s", w.tag)), 1792 + k, ew, [ew](auto a, auto b, auto& vscr) { return clamp_to(sx(a, ew) - sx(b, ew), smin(ew), smax(ew), ew, vscr); });
        lane_op(name(std::format("vavgu{}", w.tag)), 1026 + k, ew, [](auto a, auto b, auto&) { return (a + b + 1) >> 1; });
        lane_op(name(std::format("vavgs{}", w.tag)), 1282 + k, ew, [ew](auto a, auto b, auto&) { return cut((sx(a, ew) + sx(b, ew) + 1) >> 1, ew); });
        lane_op(name(std::format("vmaxu{}", w.tag)), 2 + k, ew, [](auto a, auto b, auto&) { return a > b ? a : b; });
        lane_op(name(std::format("vmaxs{}", w.tag)), 258 + k, ew, [ew](auto a, auto b, auto&) { return sx(a, ew) > sx(b, ew) ? a : b; });
        lane_op(name(std::format("vminu{}", w.tag)), 514 + k, ew, [](auto a, auto b, auto&) { return a < b ? a : b; });
        lane_op(name(std::format("vmins{}", w.tag)), 770 + k, ew, [ew](auto a, auto b, auto&) { return sx(a, ew) < sx(b, ew) ? a : b; });
        lane_op(name(std::format("vcmpequ{}", w.tag)), 6 + k, ew, [ew](auto a, auto b, auto&) { return a == b ? cut(-1, ew) : 0; });
        lane_op(name(std::format("vcmpgtu{}", w.tag)), 518 + k, ew, [ew](auto a, auto b, auto&) { return a > b ? cut(-1, ew) : 0; });
        lane_op(name(std::format("vcmpgts{}", w.tag)), 774 + k, ew, [ew](auto a, auto b, auto&) { return sx(a, ew) > sx(b, ew) ? cut(-1, ew) : 0; });
        lane_op(name(std::format("vrl{}", w.tag)), 4 + k, ew, [ew](auto a, auto b, auto&) {
            const unsigned bits = 8 * ew, n = static_cast<unsigned>(b % bits);
            return cut(static_cast<std::int64_t>(((a << n) | (a >> ((bits - n) % bits))) & (ew == 4 ? 0xFFFFFFFFull : (1ull << bits) - 1)), ew);
        });
        lane_op(name(std::format("vsl{}", w.tag)), 260 + k, ew, [ew](auto a, auto b, auto&) { return cut(static_cast<std::int64_t>(a << (b % (8 * ew))), ew); });
        lane_op(name(std::format("vsr{}", w.tag)), 516 + k, ew, [ew](auto a, auto b, auto&) { return a >> (b % (8 * ew)); });
        lane_op(name(std::format("vsra{}", w.tag)), 772 + k, ew, [ew](auto a, auto b, auto&) { return cut(sx(a, ew) >> (b % (8 * ew)), ew); });
    }
    c.push_back({"vaddcuw", [] { return vx(384); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 4, [](auto x, auto y) { return (x + y) >> 32; }); }});
    c.push_back({"vsubcuw", [] { return vx(1408); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 4, [](auto x, auto y) { return x >= y ? 1 : 0; }); }});
    c.push_back({"vand", [] { return vx(1028); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 1, [](auto x, auto y) { return x & y; }); }});
    c.push_back({"vandc", [] { return vx(1092); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 1, [](auto x, auto y) { return x & ~y & 0xFF; }); }});
    c.push_back({"vor", [] { return vx(1156); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 1, [](auto x, auto y) { return x | y; }); }});
    c.push_back({"vnor", [] { return vx(1284); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 1, [](auto x, auto y) { return ~(x | y) & 0xFF; }); }});
    c.push_back({"vxor", [] { return vx(1220); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) { return each(a, b, 1, [](auto x, auto y) { return x ^ y; }); }});
    c.push_back({"vsel", [] { return va(42); }, [](const V& a, const V& b, const V& m, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 16; ++i) r[i] = static_cast<std::uint8_t>((a[i] & ~m[i]) | (b[i] & m[i]));
                     return r;
                 }});
    c.push_back({"vperm", [] { return va(43); }, [](const V& a, const V& b, const V& m, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 16; ++i) {
                         const unsigned k = m[i] & 31;
                         r[i] = k < 16 ? a[k] : b[k - 16];
                     }
                     return r;
                 }});
    // merges
    for (const Width& w : widths) {
        const unsigned ew = w.ew, k = ew == 1 ? 0 : ew == 2 ? 64 : 128, n = 16 / ew;
        c.push_back({keep(std::format("vmrgh{}", w.tag)), [k] { return vx(12 + k); }, [ew, n](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned j = 0; j < n / 2; ++j) {
                             put(r, ew, 2 * j, el(a, ew, j));
                             put(r, ew, 2 * j + 1, el(b, ew, j));
                         }
                         return r;
                     }});
        c.push_back({keep(std::format("vmrgl{}", w.tag)), [k] { return vx(268 + k); }, [ew, n](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned j = 0; j < n / 2; ++j) {
                             put(r, ew, 2 * j, el(a, ew, n / 2 + j));
                             put(r, ew, 2 * j + 1, el(b, ew, n / 2 + j));
                         }
                         return r;
                     }});
    }
    // multiplies: even and odd elements
    for (unsigned ew : {1u, 2u}) {
        const unsigned k = ew == 1 ? 0 : 64;
        for (unsigned sign = 0; sign < 2; ++sign)
            for (unsigned even = 0; even < 2; ++even) {
                c.push_back({keep(std::format("vmul{}{}{}", even ? "e" : "o", sign ? "s" : "u", ew == 1 ? "b" : "h")), [=] { return vx((even ? 520 : 8) + (sign ? 256 : 0) + k); }, [=](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                                 V r{};
                                 for (unsigned j = 0; j < 8 / ew; ++j) {
                                     const unsigned i = 2 * j + (even ? 0 : 1);   // the element: AltiVec's even elements are 0, 2, 4 ...
                                     const std::int64_t x = sign ? sx(el(a, ew, i), ew) : static_cast<std::int64_t>(el(a, ew, i));
                                     const std::int64_t y = sign ? sx(el(b, ew, i), ew) : static_cast<std::int64_t>(el(b, ew, i));
                                     put(r, 2 * ew, j, cut(x * y, 2 * ew));
                                 }
                                 return r;
                             }});
            }
    }
    // multiply-sums (the third operand is the word to add)
    struct Sum {
        const char* name;
        unsigned xo, ew;
        bool sign_a, sign_b, sat;
    };
    static const Sum sums[] = {{"vmsumubm", 36, 1, false, false, false}, {"vmsummbm", 37, 1, true, false, false}, {"vmsumuhm", 38, 2, false, false, false}, {"vmsumuhs", 39, 2, false, false, true},
                               {"vmsumshm", 40, 2, true, true, false}, {"vmsumshs", 41, 2, true, true, true}};
    for (const Sum& s : sums)
        c.push_back({s.name, [s] { return va(s.xo); }, [s](const V& a, const V& b, const V& acc, std::uint32_t& vscr, unsigned) {
                         V r{};
                         for (unsigned j = 0; j < 4; ++j) {
                             const bool any_signed = s.sign_a || s.sign_b;
                             std::int64_t sum = any_signed ? sx(el(acc, 4, j), 4) : static_cast<std::int64_t>(el(acc, 4, j));
                             for (unsigned k = 0; k < 4 / s.ew; ++k) {
                                 const unsigned i = j * (4 / s.ew) + k;
                                 const std::int64_t x = s.sign_a ? sx(el(a, s.ew, i), s.ew) : static_cast<std::int64_t>(el(a, s.ew, i));
                                 const std::int64_t y = s.sign_b ? sx(el(b, s.ew, i), s.ew) : static_cast<std::int64_t>(el(b, s.ew, i));
                                 sum += x * y;
                             }
                             if (s.sat) put(r, 4, j, any_signed ? clamp_to(sum, smin(4), smax(4), 4, vscr) : clamp_to(sum, 0, umax(4), 4, vscr));
                             else put(r, 4, j, cut(sum, 4));
                         }
                         return r;
                     }});
    c.push_back({"vmladduhm", [] { return va(34); }, [](const V& a, const V& b, const V& d, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 8; ++i) put(r, 2, i, cut(static_cast<std::int64_t>(el(a, 2, i) * el(b, 2, i) + el(d, 2, i)), 2));
                     return r;
                 }});
    for (unsigned round = 0; round < 2; ++round)
        c.push_back({round ? "vmhraddshs" : "vmhaddshs", [round] { return va(32 + round); }, [round](const V& a, const V& b, const V& d, std::uint32_t& vscr, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 8; ++i) {
                             const std::int64_t p = sx(el(a, 2, i), 2) * sx(el(b, 2, i), 2);
                             put(r, 2, i, clamp_to(((p + (round ? 0x4000 : 0)) >> 15) + sx(el(d, 2, i), 2), smin(2), smax(2), 2, vscr));
                         }
                         return r;
                     }});
    // sums across
    for (const auto& s : {std::tuple<const char*, unsigned, unsigned, bool>{"vsum4ubs", 1544, 1, false}, {"vsum4sbs", 1800, 1, true}, {"vsum4shs", 1608, 2, true}})
        c.push_back({std::get<0>(s), [s] { return vx(std::get<1>(s)); }, [s](const V& a, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                         const unsigned ew = std::get<2>(s);
                         const bool sign = std::get<3>(s);
                         V r{};
                         for (unsigned j = 0; j < 4; ++j) {
                             std::int64_t sum = sign ? sx(el(b, 4, j), 4) : static_cast<std::int64_t>(el(b, 4, j));
                             for (unsigned k = 0; k < 4 / ew; ++k) sum += sign ? sx(el(a, ew, j * (4 / ew) + k), ew) : static_cast<std::int64_t>(el(a, ew, j * (4 / ew) + k));
                             put(r, 4, j, sign ? clamp_to(sum, smin(4), smax(4), 4, vscr) : clamp_to(sum, 0, umax(4), 4, vscr));
                         }
                         return r;
                     }});
    c.push_back({"vsum2sws", [] { return vx(1672); }, [](const V& a, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                     V r{};
                     put(r, 4, 1, clamp_to(sx(el(a, 4, 0), 4) + sx(el(a, 4, 1), 4) + sx(el(b, 4, 1), 4), smin(4), smax(4), 4, vscr));
                     put(r, 4, 3, clamp_to(sx(el(a, 4, 2), 4) + sx(el(a, 4, 3), 4) + sx(el(b, 4, 3), 4), smin(4), smax(4), 4, vscr));
                     return r;
                 }});
    c.push_back({"vsumsws", [] { return vx(1928); }, [](const V& a, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                     V r{};
                     put(r, 4, 3, clamp_to(sx(el(a, 4, 0), 4) + sx(el(a, 4, 1), 4) + sx(el(a, 4, 2), 4) + sx(el(a, 4, 3), 4) + sx(el(b, 4, 3), 4), smin(4), smax(4), 4, vscr));
                     return r;
                 }});
    // packs: the elements of vA then those of vB
    struct Pack {
        const char* name;
        unsigned xo, ew;
        int mode;   // 0 modulo, 1 unsigned to unsigned saturate, 2 signed to signed, 3 signed to unsigned
    };
    static const Pack packs[] = {{"vpkuhum", 14, 2, 0}, {"vpkuwum", 78, 4, 0}, {"vpkuhus", 142, 2, 1}, {"vpkuwus", 206, 4, 1}, {"vpkshss", 398, 2, 2},
                                 {"vpkswss", 462, 4, 2}, {"vpkshus", 270, 2, 3}, {"vpkswus", 334, 4, 3}};
    for (const Pack& p : packs)
        c.push_back({p.name, [p] { return vx(p.xo); }, [p](const V& a, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                         V r{};
                         const unsigned n = 16 / p.ew, oe = p.ew / 2;
                         for (unsigned i = 0; i < 2 * n; ++i) {
                             const V& src = i < n ? a : b;
                             const std::uint64_t raw = el(src, p.ew, i % n);
                             std::uint64_t out;
                             switch (p.mode) {
                                 case 0: out = cut(static_cast<std::int64_t>(raw), oe); break;
                                 case 1: out = clamp_to(static_cast<std::int64_t>(raw), 0, umax(oe), oe, vscr); break;
                                 case 2: out = clamp_to(sx(raw, p.ew), smin(oe), smax(oe), oe, vscr); break;
                                 default: out = clamp_to(sx(raw, p.ew), 0, umax(oe), oe, vscr); break;
                             }
                             put(r, oe, i, out);
                         }
                         return r;
                     }});
    // unpacks
    for (const auto& u : {std::tuple<const char*, unsigned, unsigned, bool>{"vupkhsb", 526, 1, true}, {"vupklsb", 654, 1, false}, {"vupkhsh", 590, 2, true}, {"vupklsh", 718, 2, false}})
        c.push_back({std::get<0>(u), [u] { return vx(std::get<1>(u), kD, 0, kB); }, [u](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                         const unsigned ew = std::get<2>(u);
                         const bool high = std::get<3>(u);
                         V r{};
                         for (unsigned i = 0; i < 8 / ew; ++i) put(r, 2 * ew, i, cut(sx(el(b, ew, (high ? 0 : 8 / ew) + i), ew), 2 * ew));
                         return r;
                     }});
    c.push_back({"vpkpx", [] { return vx(782); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 8; ++i) {
                         const std::uint32_t w = static_cast<std::uint32_t>(el(i < 4 ? a : b, 4, i % 4));
                         put(r, 2, i, ((w >> 9) & 0x8000u) | ((w >> 9) & 0x7C00u) | ((w >> 6) & 0x03E0u) | ((w >> 3) & 0x001Fu));   // the low bit of the top byte, then the five high bits of each colour byte
                     }
                     return r;
                 }});
    for (unsigned high = 0; high < 2; ++high)
        c.push_back({high ? "vupkhpx" : "vupklpx", [high] { return vx(high ? 846 : 974, kD, 0, kB); }, [high](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 4; ++i) {
                             const std::uint32_t h = static_cast<std::uint32_t>(el(b, 2, (high ? 0 : 4) + i));
                             put(r, 4, i, ((h & 0x8000u) ? 0xFF000000u : 0) | (((h >> 10) & 31) << 16) | (((h >> 5) & 31) << 8) | (h & 31));
                         }
                         return r;
                     }});
    // splats
    for (const Width& w : widths) {
        const unsigned ew = w.ew;
        c.push_back({keep(std::format("vsplt{}", w.tag)), [ew] { return vx((ew == 1 ? 524 : ew == 2 ? 588 : 652), kD, 3 % (16 / ew), kB); }, [ew](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 16 / ew; ++i) put(r, ew, i, el(b, ew, 3 % (16 / ew)));
                         return r;
                     }});
        c.push_back({keep(std::format("vspltis{}", w.tag)), [ew] { return vx((ew == 1 ? 780 : ew == 2 ? 844 : 908), kD, 0x1B, 0); }, [ew](const V&, const V&, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 16 / ew; ++i) put(r, ew, i, cut(-5, ew));   // 0x1B is the five-bit -5
                         return r;
                     }});
    }
    // the whole-vector shifts: by bits (vsl, vsr: the low three bits of vB's last byte) and by octets (vslo, vsro: bits 3 to 6 of it)
    const auto whole_shift = [&](const char* name, unsigned xo, bool left, bool octets) {
        c.push_back({name, [xo] { return vx(xo); }, [left, octets](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         if (octets) {
                             const unsigned n = (b[15] >> 3) & 15;
                             for (unsigned i = 0; i < 16; ++i) {
                                 const int from = left ? static_cast<int>(i + n) : static_cast<int>(i) - static_cast<int>(n);
                                 r[i] = (from >= 0 && from < 16) ? a[from] : 0;
                             }
                         } else {
                             const unsigned n = b[15] & 7;
                             for (unsigned i = 0; i < 16; ++i) {
                                 const unsigned hi = left ? a[i] : (i > 0 ? a[i - 1] : 0), lo = left ? (i < 15 ? a[i + 1] : 0) : a[i];
                                 r[i] = left ? static_cast<std::uint8_t>(((hi << n) | (lo >> (8 - n))) & 0xFF) : static_cast<std::uint8_t>(((hi << (8 - n)) | (lo >> n)) & 0xFF);
                             }
                         }
                         return r;
                     }});
    };
    whole_shift("vsl", 452, true, false);
    whole_shift("vsr", 708, false, false);
    whole_shift("vslo", 1036, true, true);
    whole_shift("vsro", 1100, false, true);
    return c;
}

// The floating point models: the host's single precision (and fused multiply-add) where an operation is exact, a tolerance for the estimates.
std::vector<Case> float_cases() {
    std::vector<Case> c;
    const auto lane_float = [&](const char* name, unsigned xo, std::function<float(float, float)> f) {
        c.push_back({name, [xo] { return vx(xo); }, [f](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 4; ++i) put_flt(r, i, f(flt(a, i), flt(b, i)));
                         return r;
                     }, 1});
    };
    lane_float("vaddfp", 10, [](float x, float y) { return x + y; });
    lane_float("vsubfp", 74, [](float x, float y) { return x - y; });
    lane_float("vmaxfp", 1034, [](float x, float y) { return std::isnan(x) ? x : std::isnan(y) ? y : (x == 0 && y == 0) ? (std::signbit(x) ? y : x) : x > y ? x : y; });
    lane_float("vminfp", 1098, [](float x, float y) { return std::isnan(x) ? x : std::isnan(y) ? y : (x == 0 && y == 0) ? (std::signbit(x) ? x : y) : x < y ? x : y; });
    const auto mask_float = [&](const char* name, unsigned xo, std::function<bool(float, float)> f) {
        c.push_back({name, [xo] { return vx(xo); }, [f](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 4; ++i) put(r, 4, i, f(flt(a, i), flt(b, i)) ? 0xFFFFFFFFu : 0u);
                         return r;
                     }, 1});
    };
    mask_float("vcmpeqfp", 198, [](float x, float y) { return x == y; });
    mask_float("vcmpgefp", 454, [](float x, float y) { return x >= y; });
    mask_float("vcmpgtfp", 710, [](float x, float y) { return x > y; });
    c.push_back({"vcmpbfp", [] { return vx(966); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 4; ++i) {
                         const float x = flt(a, i), y = flt(b, i);
                         put(r, 4, i, (std::isnan(x) || std::isnan(y)) ? 0xC0000000u : ((x > y ? 0x80000000u : 0u) | (x < -y ? 0x40000000u : 0u)));
                     }
                     return r;
                 }, 1});
    c.push_back({"vmaddfp", [] { return va(46); }, [](const V& a, const V& b, const V& d, std::uint32_t&, unsigned) {   // vmaddfp vD,vA,vC,vB: vA * vC + vB (the third operand of the VA form is vC)
                     V r{};
                     for (unsigned i = 0; i < 4; ++i) put_flt(r, i, std::fmaf(flt(a, i), flt(d, i), flt(b, i)));
                     return r;
                 }, 1});
    c.push_back({"vnmsubfp", [] { return va(47); }, [](const V& a, const V& b, const V& d, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 4; ++i) put_flt(r, i, -std::fmaf(flt(a, i), flt(d, i), -flt(b, i)));
                     return r;
                 }, 1});
    const auto unary = [&](const char* name, unsigned xo, std::function<float(float)> f) {
        c.push_back({name, [xo] { return vx(xo, kD, 0, kB); }, [f](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 4; ++i) put_flt(r, i, f(flt(b, i)));
                         return r;
                     }, 1});
    };
    unary("vrfin", 522, [](float x) { return std::nearbyintf(x); });
    unary("vrfiz", 586, [](float x) { return std::truncf(x); });
    unary("vrfip", 650, [](float x) { return std::ceilf(x); });
    unary("vrfim", 714, [](float x) { return std::floorf(x); });
    // conversions with the scale in the vA field
    c.push_back({"vcfsx", [] { return vx(842, kD, 7, kB); }, [](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 4; ++i) put_flt(r, i, std::ldexp(static_cast<float>(static_cast<std::int32_t>(el(b, 4, i))), -7));
                     return r;
                 }});
    c.push_back({"vcfux", [] { return vx(778, kD, 31, kB); }, [](const V&, const V& b, const V&, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 4; ++i) put_flt(r, i, std::ldexp(static_cast<float>(static_cast<std::uint32_t>(el(b, 4, i))), -31));
                     return r;
                 }});
    for (const bool is_signed : {true, false})
        c.push_back({is_signed ? "vctsxs" : "vctuxs", [is_signed] { return vx(is_signed ? 970 : 906, kD, 5, kB); }, [is_signed](const V&, const V& b, const V&, std::uint32_t& vscr, unsigned) {
                         V r{};
                         for (unsigned i = 0; i < 4; ++i) {
                             const float x = flt(b, i);
                             std::uint64_t out;
                             if (std::isnan(x)) {
                                 out = 0;
                                 vscr |= 1;
                             } else {
                                 const double scaled = std::trunc(static_cast<double>(x) * 32.0);   // (exact in a double)
                                 const double lo = is_signed ? -2147483648.0 : 0.0, hi = is_signed ? 2147483647.0 : 4294967295.0;
                                 if (scaled < lo || scaled > hi) vscr |= 1;
                                 out = cut(static_cast<std::int64_t>(std::clamp(scaled, lo, hi)), 4);
                             }
                             put(r, 4, i, out);
                         }
                         return r;
                     }, 1});
    return c;
}

}  // namespace

namespace {

struct Check {
    unsigned runs = 0, wrong = 0;
    std::string first;
};

// Run every case many times on random operands (floats chosen from the usual and the unusual) and say which differ from the model.
void run_cases(po::Flavor flavor, const std::vector<Case>& cases, unsigned rounds, std::map<std::string, Check>& results) {
    Rig rig(flavor);
    std::mt19937 rng(0xA17157C);
    for (const Case& c : cases) {
        Check& r = results[c.name];
        for (unsigned n = 0; n < rounds; ++n) {
            const V a = random_vector(rng, c.style), b = random_vector(rng, c.style), d = random_vector(rng, c.style);
            rig.set(kA, a);
            rig.set(kB, b);
            rig.set(kC, d);
            rig.set(kD, V{});
            const std::uint32_t before = static_cast<std::uint32_t>(rng()) & 0x00010000u;   // VSCR[NJ] is held clear in the model: Java mode
            rig.vscr() = 0;
            (void)before;
            const std::uint32_t word = c.word();
            std::uint32_t model_vscr = 0;
            const V expected = c.model(a, b, d, model_vscr, c.imm);
            const bool ran = rig.step(word);
            const V got = rig.get(kD);
            ++r.runs;
            const bool floats = c.style == 1;
            const bool same = ran && same_result(got, expected, floats) && ((rig.context().vscr & 1) == (model_vscr & 1));
            if (!same && ++r.wrong == 1) {
                std::string text = ran ? "" : "did not run (" + rig.error() + "); ";
                for (unsigned i = 0; i < 16; ++i) text += std::format("{:02X}", expected[i]);
                text += " expected, ";
                for (unsigned i = 0; i < 16; ++i) text += std::format("{:02X}", got[i]);
                text += std::format(" got; sat {}/{}; a=", model_vscr & 1, rig.context().vscr & 1);
                for (unsigned i = 0; i < 16; ++i) text += std::format("{:02X}", a[i]);
                text += " b=";
                for (unsigned i = 0; i < 16; ++i) text += std::format("{:02X}", b[i]);
                text += " c=";
                for (unsigned i = 0; i < 16; ++i) text += std::format("{:02X}", d[i]);
                r.first = text;
            }
        }
    }
}

}  // namespace

CB_TEST("emulate.pcode_ops_ppc.altivec_integer_and_float_operations_match_a_second_model", [] {
    std::map<std::string, Check> results;
    run_cases(po::Flavor::AltiVec, integer_cases(), 200, results);
    run_cases(po::Flavor::AltiVec, float_cases(), 400, results);
    unsigned ops = 0, wrong = 0;
    for (const auto& [name, r] : results) {
        ++ops;
        if (r.wrong != 0) {
            ++wrong;
            std::printf("  %s: %u of %u differ; first: %s\n", name.c_str(), r.wrong, r.runs, r.first.c_str());
        }
    }
    std::printf("pcode_ops_ppc altivec: %u operations checked against the second model, %u differ\n", ops, wrong);
    CB_CHECK(ops > 120);
    CB_CHECK_EQ(wrong, 0u);
});

// A survey of what each description decodes of opcode 4 (every VX extended opcode and VA opcode), printed for the report.
CB_TEST("emulate.pcode_ops_ppc.survey_of_the_decodable_vector_opcodes", [] {
    for (po::Flavor flavor : {po::Flavor::AltiVec, po::Flavor::Xenon}) {
        Rig rig(flavor);
        std::string vxs, vas;
        for (unsigned xo = 0; xo < 2048; ++xo) {
            bool ok = false;
            for (unsigned fields = 0; fields < 4 && !ok; ++fields) {
                const std::uint32_t word = vx(xo, kD, (fields & 1) ? kA : 0, (fields & 2) ? kB : 0);
                const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
                rig.set_gpr(1, 0);
                ok = rig.decodes(bytes);
            }
            if (ok) vxs += std::format("{} ", xo);
        }
        for (unsigned xo = 0; xo < 64; ++xo) {
            const std::uint32_t word = va(xo);
            const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8), static_cast<std::uint8_t>(word)};
            if (rig.decodes(bytes)) vas += std::format("{} ", xo);
        }
        std::printf("decodable VX xo (%s): %s\nVA xo: %s\n", flavor == po::Flavor::AltiVec ? "altivec" : "xenon", vxs.c_str(), vas.c_str());
    }
});

namespace {

// vsldoi (p-code in the description) with the byte count 5, and the operations that read or write the vector status register.
std::vector<Case> shift_double_case() {
    std::vector<Case> c;
    c.push_back({"vsldoi", [] { return va(44, kD, kA, kB, 5); }, [](const V& a, const V& b, const V&, std::uint32_t&, unsigned) {
                     V r{};
                     for (unsigned i = 0; i < 16; ++i) r[i] = i + 5 < 16 ? a[i + 5] : b[i + 5 - 16];
                     return r;
                 }});
    return c;
}

// The estimates: not an exact answer (the processor leaves the low bits to the implementation: the manual allows one part in 4096), so a tolerance and the special values.
bool within(float got, double exact, double tolerance) {
    if (std::isnan(exact)) return std::isnan(got);
    if (std::isinf(exact) || std::fabs(exact) > 3.4028234e38) return std::isinf(got) && (got > 0) == (exact > 0);   // (too big for a single: infinity)
    if (std::fabs(exact) < 1.2e-38) return true;                                                    // (a denormal or zero: not checked)
    return std::fabs(got - exact) <= tolerance * std::fabs(exact) + 1e-30;
}

}  // namespace

CB_TEST("emulate.pcode_ops_ppc.altivec_shifts_across_two_registers_and_the_status_register", [] {
    std::map<std::string, Check> results;
    run_cases(po::Flavor::AltiVec, shift_double_case(), 200, results);
    if (results["vsldoi"].wrong != 0) std::printf("  vsldoi: %s\n", results["vsldoi"].first.c_str());
    CB_CHECK_EQ(results["vsldoi"].wrong, 0u);
    // mtvscr keeps NJ and SAT; mfvscr reads them back in the last word
    Rig rig(po::Flavor::AltiVec);
    V v{};
    put(v, 4, 3, 0xFFFFFFFFu);
    rig.set(kB, v);
    CB_CHECK(rig.step(vx(1604, 0, 0, kB)));   // mtvscr vB (XOP_1_10 = 802)
    CB_CHECK_EQ(rig.context().vscr, 0x00010001u);
    CB_CHECK(rig.step(vx(1540, kD, 0, 0)));   // mfvscr vD (XOP_1_10 = 770)
    const V got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 3), 0x00010001u);
    CB_CHECK_EQ(el(got, 4, 0), 0u);
});

CB_TEST("emulate.pcode_ops_ppc.altivec_estimates_are_within_what_the_manual_allows", [] {
    std::mt19937 rng(99);
    Rig rig(po::Flavor::AltiVec);
    struct Estimate {
        const char* name;
        unsigned xo;
        double (*exact)(double);
        double tolerance;
    };
    static const Estimate estimates[] = {{"vrefp", 266, [](double x) { return 1.0 / x; }, 1.0 / 4096},
                                         {"vrsqrtefp", 330, [](double x) { return x < 0 ? std::nan("") : 1.0 / std::sqrt(x); }, 1.0 / 4096},
                                         {"vexptefp", 394, [](double x) { return std::exp2(x); }, 1.0 / 1024},
                                         {"vlogefp", 458, [](double x) { return x < 0 ? std::nan("") : std::log2(x); }, 1.0 / 1024}};
    unsigned wrong = 0;
    for (const Estimate& e : estimates)
        for (unsigned n = 0; n < 300; ++n) {
            const V b = random_vector(rng, 1);
            rig.set(kB, b);
            rig.vscr() = 0;
            CB_CHECK(rig.step(vx(e.xo, kD, 0, kB)));
            const V got = rig.get(kD);
            for (unsigned i = 0; i < 4; ++i) {
                const float x = flt(b, i);
                if (e.xo == 458 && std::fabs(e.exact(x)) < 1e-3) continue;   // (a logarithm near zero: the tolerance is relative to a value that vanishes)
                if (!within(flt(got, i), e.exact(x), e.tolerance) && ++wrong <= 5) std::printf("  %s(%g) = %g, exact %g\n", e.name, x, flt(got, i), e.exact(x));
            }
        }
    CB_CHECK_EQ(wrong, 0u);
});

// Non-Java mode (VSCR[NJ]): a denormal input is a zero of its sign and a denormal result is a zero; in Java mode they are what IEEE says. The estimates, vcmpbfp and the VMX128 dot products too.
CB_TEST("emulate.pcode_ops_ppc.non_java_mode_flushes_denormals_in_the_estimates_the_bounds_compare_and_the_dot_products", [] {
    const float denormal = std::bit_cast<float>(0x00000100u);   // 2^-141
    const float negative_denormal = -denormal;
    struct Row {
        const char* name;
        unsigned xo;
        float in;
        float java, non_java;
    };
    const Row rows[] = {
        {"vrsqrtefp", 330, denormal, std::numeric_limits<float>::quiet_NaN() /* (approximately 2^70.5: only checked finite) */, std::numeric_limits<float>::infinity()},
        {"vlogefp", 458, denormal, -141.0f, -std::numeric_limits<float>::infinity()},
        {"vexptefp", 394, -140.0f, std::exp2(-140.0f), 0.0f},   // a denormal result
        {"vrefp", 266, negative_denormal, -std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity()},
    };
    Rig rig(po::Flavor::AltiVec);
    for (const Row& r : rows)
        for (unsigned non_java = 0; non_java < 2; ++non_java) {
            V b{};
            for (unsigned i = 0; i < 4; ++i) put_flt(b, i, r.in);
            rig.set(kB, b);
            rig.vscr() = non_java ? 0x10000u : 0u;
            CB_CHECK(rig.step(vx(r.xo, kD, 0, kB)));
            const float got = flt(rig.get(kD), 0);
            if (non_java) {
                CB_CHECK_EQ(std::bit_cast<std::uint32_t>(got), std::bit_cast<std::uint32_t>(r.non_java));
            } else if (std::isnan(r.java)) {
                CB_CHECK(std::isfinite(got) && got > 1e20f);
            } else if (std::string(r.name) == "vexptefp") {
                CB_CHECK(got > 0 && std::fpclassify(got) == FP_SUBNORMAL);   // (the host's value of 2^-140, or a neighbor)
            } else if (std::string(r.name) == "vlogefp") {
                CB_CHECK(std::fabs(got - r.java) < 0.2f);
            } else {
                CB_CHECK_EQ(std::bit_cast<std::uint32_t>(got), std::bit_cast<std::uint32_t>(r.java));
            }
        }
    {   // vcmpbfp: a = 2^-141, b = 0: a > b in Java mode (bit 0 set), a zero in non-Java mode
        V a{}, b{};
        for (unsigned i = 0; i < 4; ++i) put_flt(a, i, denormal), put_flt(b, i, 0.0f);
        rig.set(kA, a);
        rig.set(kB, b);
        rig.vscr() = 0;
        CB_CHECK(rig.step(vx(966, kD, kA, kB)));   // vcmpbfp
        CB_CHECK_EQ(el(rig.get(kD), 4, 0), 0x80000000u);
        rig.vscr() = 0x10000u;
        CB_CHECK(rig.step(vx(966, kD, kA, kB)));
        CB_CHECK_EQ(el(rig.get(kD), 4, 0), 0u);
    }
    {   // vmsum3fp128: the products of denormals vanish in non-Java mode
        Rig x360(po::Flavor::Xenon);
        V a{}, b{};
        for (unsigned i = 0; i < 4; ++i) put_flt(a, i, std::bit_cast<float>(0x00400000u)), put_flt(b, i, 0.5f);   // 2^-127 * 2^-1: denormal products
        x360.set(kA, a);
        x360.set(kB, b);
        const auto word = (5u << 26) | (kD << 21) | (kA << 16) | (kB << 11) | (1u << 4) | (6u << 6);
        x360.vscr() = 0;
        CB_CHECK(x360.step(word));
        CB_CHECK(flt(x360.get(kD), 0) > 0.0f);
        x360.vscr() = 0x10000u;
        CB_CHECK(x360.step(word));
        CB_CHECK_EQ(std::bit_cast<std::uint32_t>(flt(x360.get(kD), 0)), 0u);
    }
});

CB_TEST("emulate.pcode_ops_ppc.altivec_loads_and_stores_that_are_not_p_code", [] {
    Rig rig(po::Flavor::AltiVec);
    for (unsigned i = 0; i < 64; ++i) rig.ram.bytes[0x2000 + i] = static_cast<std::uint8_t>(0xA0 + i);
    rig.set_gpr(3, 0x2000);
    for (unsigned offset : {0u, 1u, 5u, 15u}) {
        rig.set_gpr(4, offset + 16);
        const std::uint32_t base = 0x2000 + 16 + offset;
        const auto xform = [&](unsigned xo, unsigned d) { return (31u << 26) | (d << 21) | (3u << 16) | (4u << 11) | (xo << 1); };
        CB_CHECK(rig.step(xform(6, kD)));   // lvsl
        V want{};
        for (unsigned i = 0; i < 16; ++i) want[i] = static_cast<std::uint8_t>((base & 15) + i);
        CB_CHECK(rig.get(kD) == want);
        CB_CHECK(rig.step(xform(38, kD)));   // lvsr
        for (unsigned i = 0; i < 16; ++i) want[i] = static_cast<std::uint8_t>(16 - (base & 15) + i);
        CB_CHECK(rig.get(kD) == want);
        CB_CHECK(rig.step(xform(359, kD)));   // lvxl
        for (unsigned i = 0; i < 16; ++i) want[i] = static_cast<std::uint8_t>(0xA0 + 16 + i);
        CB_CHECK(rig.get(kD) == want);
        CB_CHECK(rig.step(xform(39, kD)));   // lvehx: the aligned halfword goes to its own element
        V half{};
        const std::uint32_t aligned = base & ~1u;
        put(half, 2, (aligned & 15) / 2, (std::uint64_t{0xA0 + (aligned - 0x2000)} << 8) | (0xA0 + (aligned - 0x2000) + 1));
        CB_CHECK(rig.get(kD) == half);
        CB_CHECK(rig.step(xform(71, kD)));   // lvewx
        V word{};
        const std::uint32_t quad = base & ~3u;
        std::uint64_t value = 0;
        for (unsigned i = 0; i < 4; ++i) value = (value << 8) | (0xA0 + (quad - 0x2000) + i);
        put(word, 4, (quad & 15) / 4, value);
        CB_CHECK(rig.get(kD) == word);
        // the stores of one element: what the vector holds in the element the address selects
        V src{};
        for (unsigned i = 0; i < 16; ++i) src[i] = static_cast<std::uint8_t>(0x10 + i);
        rig.set(kB, src);
        for (unsigned bytes : {1u, 2u, 4u}) {
            if (bytes == 2 && (base & 1)) continue;   // (the description stores a misaligned halfword where it is given: the adapter aligns it, see stvehx_aligns_its_address...)
            rig.ram.bytes.clear();
            const unsigned xo = bytes == 1 ? 135 : bytes == 2 ? 167 : 199;
            CB_CHECK(rig.step((31u << 26) | (kB << 21) | (3u << 16) | (4u << 11) | (xo << 1)));   // stvebx, stvehx, stvewx
            const std::uint32_t at = base & ~(bytes - 1);
            for (unsigned i = 0; i < bytes; ++i) CB_CHECK_EQ(static_cast<unsigned>(rig.ram.bytes[at + i]), static_cast<unsigned>(src[(at & 15) / bytes * bytes + i]));
            CB_CHECK_EQ(rig.ram.bytes.size(), static_cast<std::size_t>(bytes));
            for (unsigned i = 0; i < 64; ++i) rig.ram.bytes[0x2000 + i] = static_cast<std::uint8_t>(0xA0 + i);   // (the loads above read again)
        }
    }
});

// The record forms of the vector compares set CR6 (the adapter does it: the description's AltiVec forms do not): all true, all false, mixed.
CB_TEST("emulate.pcode_ops_ppc.vector_compares_with_the_record_bit_set_cr6", [] {
    constexpr std::uint32_t kCode = 0x10000000;
    struct Program {
        std::vector<std::uint32_t> words;
        unsigned expect;   // CR6
    };
    const std::uint32_t mfcr_r3 = (31u << 26) | (3u << 21) | (19u << 1), blr = 0x4E800020;
    const std::vector<Program> programs = {
        {{vx(780, 1, 0x1F, 0), vx(6 + 1024, 3, 1, 1), mfcr_r3, blr}, 8},                                 // vspltisb v1,-1; vcmpequb. v3,v1,v1: every byte equal
        {{vx(780, 1, 0x1F, 0), vx(1220, 2, 2, 2), vx(6 + 1024, 3, 2, 1), mfcr_r3, blr}, 2},             // v2 zero against v1 all ones: no byte equal
        {{vx(780, 1, 0x1F, 0), vx(1220, 2, 2, 2), vx(780, 4, 0x01, 0), vx(1220, 5, 5, 5), vx(1028, 6, 1, 1), vx(6 + 1024, 3, 4, 6), mfcr_r3, blr}, 2},   // v4 = 1s, v6 = -1s: none equal
        {{vx(780, 1, 0x1F, 0), vx(1220, 2, 2, 2), vx(1156, 3, 1, 2), vx(966 + 1024, 4, 2, 1), mfcr_r3, blr}, 0},   // vcmpbfp.: v2 = 0 against v1 (NaN bytes): not all within bounds
    };
    for (const Program& p : programs) {
        pw::Region code;
        code.address = kCode;
        for (std::uint32_t w : p.words)
            for (unsigned k = 0; k < 4; ++k) code.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
        const std::array<pw::Region, 1> regions = {code};
        pw::RunOptions options;
        options.max_steps = 100;
        const pw::RunResult r = emulate::pcode_cpu::run(regions, kCode, {}, options);
        CB_CHECK_EQ(r.exit_text(), std::string("ret"));
        CB_CHECK_EQ((r.ret >> 4) & 0xF, static_cast<std::uint64_t>(p.expect));
    }
});


// VMX128 (the Xbox 360's Xenon description): the floating point forms whose operands come from vD as well, the dot products, and the left and right loads and stores.
// No second source exists for the formulas of vmaddfp128 and vmaddcfp128 beyond the manual's: they are the ones the Xenia emulator uses (vD = vA vB + vD, and vD = vA vD + vB).
CB_TEST("emulate.pcode_ops_ppc.vmx128_floating_point_and_memory_operations", [] {
    std::mt19937 rng(0x128);
    Rig rig(po::Flavor::Xenon);
    const auto x128 = [](unsigned n) { return (5u << 26) | (kD << 21) | (kA << 16) | (kB << 11) | (1u << 4) | (n << 6); };
    unsigned wrong = 0;
    for (unsigned round = 0; round < 400; ++round) {
        const V a = random_vector(rng, 1), b = random_vector(rng, 1), d = random_vector(rng, 1);
        rig.set(kA, a);
        rig.set(kB, b);
        for (unsigned form = 0; form < 8; ++form) {
            rig.set(kD, d);
            rig.vscr() = 0;
            CB_CHECK(rig.step(x128(form)));
            const V got = rig.get(kD);
            V want{};
            for (unsigned i = 0; i < 4; ++i) {
                const float x = flt(a, i), y = flt(b, i), z = flt(d, i);
                switch (form) {
                    case 0: put_flt(want, i, x + y); break;                    // vaddfp128
                    case 1: put_flt(want, i, x - y); break;                    // vsubfp128
                    case 2: put_flt(want, i, x * y); break;                    // vmulfp128
                    case 3: put_flt(want, i, std::fmaf(x, y, z)); break;       // vmaddfp128: vA * vB + vD
                    case 4: put_flt(want, i, std::fmaf(x, z, y)); break;       // vmaddcfp128: vA * vD + vB
                    case 5: put_flt(want, i, -std::fmaf(x, z, -y)); break;     // vnmsubfp128: -(vA * vD - vB)
                    default: break;
                }
            }
            if (form >= 6) {   // the dot products: every element holds the sum (to within rounding: the order of the additions is the implementation's)
                const unsigned terms = form == 6 ? 3 : 4;
                double sum = 0;
                bool special = false;
                for (unsigned i = 0; i < terms; ++i) {
                    const double p = static_cast<double>(flt(a, i)) * flt(b, i);
                    special = special || std::isnan(p) || std::isinf(p) || std::fabs(p) > 1e30;
                    sum += p;
                }
                for (unsigned i = 0; i < 4 && !special; ++i)
                    if (std::fabs(static_cast<double>(flt(got, i)) - sum) > 1e-5 * (std::fabs(sum) + 1e-30) + 1e-38) ++wrong;
                continue;
            }
            if (!same_result(got, want, true) && ++wrong <= 5) std::printf("  vmx128 form %u differs\n", form);
        }
    }
    CB_CHECK_EQ(wrong, 0u);
    // lvlx / lvrx / stvlx / stvrx: the bytes from the address to the end of its quadword (or from the start of it to the address)
    for (unsigned i = 0; i < 64; ++i) rig.ram.bytes[0x2000 + i] = static_cast<std::uint8_t>(0xA0 + i);
    rig.set_gpr(3, 0x2000);
    for (unsigned offset : {0u, 1u, 7u, 15u}) {
        rig.set_gpr(4, 16 + offset);
        const std::uint32_t at = 0x2010 + offset;
        const unsigned eb = at & 15;
        const auto xform = [&](unsigned xo, unsigned d) { return (31u << 26) | (d << 21) | (3u << 16) | (4u << 11) | (xo << 1); };
        V left{}, right{};
        for (unsigned i = 0; i + eb < 16; ++i) left[i] = static_cast<std::uint8_t>(0xA0 + 16 + eb + i);
        for (unsigned i = 0; i < eb; ++i) right[16 - eb + i] = static_cast<std::uint8_t>(0xA0 + 16 + i);
        CB_CHECK(rig.step(xform(519, kD)));   // lvlx
        CB_CHECK(rig.get(kD) == left);
        CB_CHECK(rig.step(xform(551, kD)));   // lvrx
        CB_CHECK(rig.get(kD) == right);
        V src{};
        for (unsigned i = 0; i < 16; ++i) src[i] = static_cast<std::uint8_t>(0x30 + i);
        rig.set(kB, src);
        rig.ram.bytes.clear();
        CB_CHECK(rig.step(xform(647, kB)));   // stvlx: the first 16 - eb bytes of vS at the address
        CB_CHECK_EQ(rig.ram.bytes.size(), static_cast<std::size_t>(16 - eb));
        for (unsigned i = 0; i + eb < 16; ++i) CB_CHECK_EQ(static_cast<unsigned>(rig.ram.bytes[at + i]), static_cast<unsigned>(src[i]));
        rig.ram.bytes.clear();
        CB_CHECK(rig.step(xform(679, kB)));   // stvrx: the last eb bytes of vS at the start of the quadword
        CB_CHECK_EQ(rig.ram.bytes.size(), static_cast<std::size_t>(eb));
        for (unsigned i = 0; i < eb; ++i) CB_CHECK_EQ(static_cast<unsigned>(rig.ram.bytes[0x2010 + i]), static_cast<unsigned>(src[16 - eb + i]));
        for (unsigned i = 0; i < 64; ++i) rig.ram.bytes[0x2000 + i] = static_cast<std::uint8_t>(0xA0 + i);
    }
});

// vcmpequw128 with the record bit sets CR6 through the description's own operation.
CB_TEST("emulate.pcode_ops_ppc.vmx128_compare_with_the_record_bit_sets_cr6", [] {
    Rig rig(po::Flavor::Xenon);
    V ones, zeros{};
    ones.fill(0xFF);
    const auto cmp = [&](unsigned record) { return (6u << 26) | (kD << 21) | (kA << 16) | (kB << 11) | (4u << 7) | (record << 6); };   // vcmpequw128
    rig.set(kA, ones);
    rig.set(kB, ones);
    CB_CHECK(rig.step(cmp(1)));
    CB_CHECK_EQ(rig.cr6(), 8u);
    rig.set(kB, zeros);
    CB_CHECK(rig.step(cmp(1)));
    CB_CHECK_EQ(rig.cr6(), 2u);
    V mixed = ones;
    mixed[0] = 0;
    rig.set(kB, mixed);
    CB_CHECK(rig.step(cmp(1)));
    CB_CHECK_EQ(rig.cr6(), 0u);
});

// The names the descriptions give and the tables serve: what is bound, and the operations of the vector families that are not (the report's list).
CB_TEST("emulate.pcode_ops_ppc.every_table_name_is_an_operation_of_its_description", [] {
    for (po::Flavor flavor : {po::Flavor::AltiVec, po::Flavor::Xenon}) {
        const emulate::pcode_cpu::PpcProbe probe = ppc_probe(flavor);
        std::printf("pcode_ops_ppc %s: %zu operations bound, %zu of the description's not served, %zu table names the description lacks\n", flavor == po::Flavor::AltiVec ? "altivec" : "xenon",
                    probe.binding.bound, probe.binding.unbound.size(), probe.binding.unknown.size());
        std::string vector_names;
        for (const std::string& name : probe.binding.unbound)
            if (name.starts_with("vector") || name.starts_with("loadVector") || name.starts_with("storeVector")) vector_names += name + " ";
        std::printf("  unserved vector names: %s\n", vector_names.c_str());
        for (const std::string& name : probe.binding.unknown) std::printf("  unknown: %s\n", name.c_str());
        CB_CHECK(probe.binding.unknown.empty());
        CB_CHECK(probe.binding.bound > 100);
    }
});


// vupkd3d128, vpkd3d128 and vrlimi128, from the formats of the XDK's "New Instructions in VMX128": values worked out here in doubles (3.0 + x 2^-22 for the signed and the packed
// unsigned input, 1.0 + x 2^-23 for an unpacked unsigned field), and the mask insert of the packed words.
CB_TEST("emulate.pcode_ops_ppc.vmx128_d3d_pack_unpack_and_rotate_insert", [] {
    Rig rig(po::Flavor::Xenon);
    const auto unpack = [](unsigned type) { return (6u << 26) | (kD << 21) | ((type << 2) << 16) | (kB << 11) | (127u << 4); };
    const auto pack = [](unsigned type, unsigned select, unsigned shift) { return (6u << 26) | (kD << 21) | (select << 16) | (type << 18) | (kB << 11) | (shift << 6) | (1u << 4) | (6u << 8); };
    const auto f32 = [](double x) { return static_cast<std::uint32_t>(std::bit_cast<std::uint32_t>(static_cast<float>(x))); };
    const double u22 = std::ldexp(1.0, -22), u23 = std::ldexp(1.0, -23);
    // D3DCOLOR: W X Y Z = 0x80 0xFF 0x40 0x20 in the w word; unpacked: x = X, y = Y, z = Z, w = W as 1.0 + n 2^-23
    V b{};
    put(b, 4, 3, 0x80FF4020u);
    rig.set(kB, b);
    CB_CHECK(rig.step(unpack(0)));
    V got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), f32(1.0 + 0xFF * u23));
    CB_CHECK_EQ(el(got, 4, 1), f32(1.0 + 0x40 * u23));
    CB_CHECK_EQ(el(got, 4, 2), f32(1.0 + 0x20 * u23));
    CB_CHECK_EQ(el(got, 4, 3), f32(1.0 + 0x80 * u23));
    // NORMSHORT2: x = high half, y = low half (signed: 3.0 + s 2^-22), z = 0.0, w = 3.0; the most negative value is a NaN
    put(b, 4, 3, 0x7FFF8001u);
    rig.set(kB, b);
    CB_CHECK(rig.step(unpack(1)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), f32(3.0 + 32767 * u22));
    CB_CHECK_EQ(el(got, 4, 1), f32(3.0 - 32767 * u22));
    CB_CHECK_EQ(el(got, 4, 2), 0u);
    CB_CHECK_EQ(el(got, 4, 3), f32(3.0));
    put(b, 4, 3, 0x80000000u);
    rig.set(kB, b);
    CB_CHECK(rig.step(unpack(1)));
    CB_CHECK(std::isnan(flt(rig.get(kD), 0)));
    // NORMPACKED32: W (2 bits, unsigned) Z Y X (10 bits, signed), X in the low bits
    put(b, 4, 3, (2u << 30) | (static_cast<std::uint32_t>(-3 & 0x3FF) << 20) | (5u << 10) | 0x1FFu);
    rig.set(kB, b);
    CB_CHECK(rig.step(unpack(2)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), f32(3.0 + 511 * u22));
    CB_CHECK_EQ(el(got, 4, 1), f32(3.0 + 5 * u22));
    CB_CHECK_EQ(el(got, 4, 2), f32(3.0 - 3 * u22));
    CB_CHECK_EQ(el(got, 4, 3), f32(1.0 + 2 * u23));
    // FLOAT16_4: x y in the z word, z w in the w word (half floats: 1.0 = 0x3C00, -2.5 = 0xC100, 131008 = 0x7FFF, a denormal 0x0001 is zero)
    put(b, 4, 2, 0x3C00C100u);
    put(b, 4, 3, 0x7FFF0001u);
    rig.set(kB, b);
    CB_CHECK(rig.step(unpack(5)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), f32(1.0));
    CB_CHECK_EQ(el(got, 4, 1), f32(-2.5));
    CB_CHECK_EQ(el(got, 4, 2), f32(131008.0));
    CB_CHECK_EQ(el(got, 4, 3), 0u);
    // packing: D3DCOLOR from floats in 3.0 + n 2^-22 (W X Y Z = 0x80 0xFF 0x40 0x20), inserted at the w word (VPACK_32, shift 0) into what vD held
    V src{}, dest{};
    put_flt(src, 3, static_cast<float>(3.0 + 0x80 * u22));
    put_flt(src, 0, static_cast<float>(3.0 + 0xFF * u22));
    put_flt(src, 1, static_cast<float>(3.0 + 0x40 * u22));
    put_flt(src, 2, static_cast<float>(3.0 + 0x20 * u22));
    for (unsigned i = 0; i < 16; ++i) dest[i] = static_cast<std::uint8_t>(0x11 * (i % 15 + 1));
    rig.set(kB, src);
    rig.set(kD, dest);
    CB_CHECK(rig.step(pack(0, 1, 0)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 3), 0x80FF4020u);
    for (unsigned i = 0; i < 3; ++i) CB_CHECK_EQ(el(got, 4, i), el(dest, 4, i));
    rig.set(kD, dest);
    CB_CHECK(rig.step(pack(0, 1, 2)));   // shift 2: the y word
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 1), 0x80FF4020u);
    CB_CHECK_EQ(el(got, 4, 3), el(dest, 4, 3));
    // values past the range are clamped (NORMSHORT2: 3.5 is far past the largest, 3.0 - 2^-22 is -1)
    V clamp{};
    put_flt(clamp, 0, 3.5f);
    put_flt(clamp, 1, static_cast<float>(3.0 - u22));
    rig.set(kB, clamp);
    rig.set(kD, V{});
    CB_CHECK(rig.step(pack(1, 1, 0)));
    CB_CHECK_EQ(el(rig.get(kD), 4, 3), 0x7FFFFFFFu);
    // the 64-bit mask selects: FLOAT16_4 with VPACK_64LO and shift 0 fills z and w; shift 3 inserts the low word alone into x
    V halves{};
    put_flt(halves, 0, 1.0f);
    put_flt(halves, 1, -2.5f);
    put_flt(halves, 2, 131008.0f);
    put_flt(halves, 3, 1.0e-9f);   // (too small for a half float: zero)
    rig.set(kB, halves);
    rig.set(kD, dest);
    CB_CHECK(rig.step(pack(5, 2, 0)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 2), 0x3C00C100u);
    CB_CHECK_EQ(el(got, 4, 3), 0x7FFF0000u);
    CB_CHECK_EQ(el(got, 4, 0), el(dest, 4, 0));
    rig.set(kD, dest);
    CB_CHECK(rig.step(pack(5, 2, 3)));
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), 0x7FFF0000u);
    CB_CHECK_EQ(el(got, 4, 1), el(dest, 4, 1));
    rig.set(kD, dest);
    CB_CHECK(rig.step(pack(5, 3, 3)));   // VPACK_64HI shift 3: the high word alone, at w
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 3), 0x3C00C100u);
    CB_CHECK_EQ(el(got, 4, 0), el(dest, 4, 0));
    // vrlimi128: vB rotated left by the shift words (one moves x to w), the words the mask names (8 is x) inserted into vD
    V words{}, old{};
    for (unsigned i = 0; i < 4; ++i) {
        put(words, 4, i, 0xA0 + i);
        put(old, 4, i, 0xB0 + i);
    }
    const auto rlimi = [](unsigned mask, unsigned shift) { return (6u << 26) | (kD << 21) | (mask << 16) | (kB << 11) | (shift << 6) | (1u << 4) | (7u << 8); };
    rig.set(kB, words);
    rig.set(kD, old);
    CB_CHECK(rig.step(rlimi(1, 1)));   // the mask names w; x rotated into w
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 3), 0xA0u);
    for (unsigned i = 0; i < 3; ++i) CB_CHECK_EQ(el(got, 4, i), el(old, 4, i));
    rig.set(kD, old);
    CB_CHECK(rig.step(rlimi(0xA, 2)));   // x and z from a rotation by two: x gets z, z gets x's old neighbour two on
    got = rig.get(kD);
    CB_CHECK_EQ(el(got, 4, 0), 0xA2u);
    CB_CHECK_EQ(el(got, 4, 1), el(old, 4, 1) );
    CB_CHECK_EQ(el(got, 4, 2), 0xA0u);
    CB_CHECK_EQ(el(got, 4, 3), el(old, 4, 3));
});

// stvehx and lvsl as the manual has them, in a run: the address is aligned to the halfword (the description stores where it is given), and a zero RA field is the number zero.
CB_TEST("emulate.pcode_ops_ppc.stvehx_aligns_its_address_and_lvsl_takes_a_zero_ra_as_zero", [] {
    constexpr std::uint32_t kCode = 0x10000000;
    const std::vector<std::uint32_t> program = {
        vx(844, 1, 5, 0),                                                           // vspltish v1,5
        (31u << 26) | (1u << 21) | (3u << 16) | (4u << 11) | (167u << 1),           // stvehx v1,r3,r4
        (31u << 26) | (2u << 21) | (0u << 16) | (4u << 11) | (6u << 1),             // lvsl v2,0,r4 (r0 is not read: the address is r4 alone)
        (31u << 26) | (1u << 21) | (0u << 16) | (5u << 11) | (167u << 1),           // stvehx v1,0,r5: an element of v1 at r5 alone (r0 = r3's value is not added)
        0x4E800020};
    pw::Region code;
    code.address = kCode;
    for (std::uint32_t w : program)
        for (unsigned k = 0; k < 4; ++k) code.data.push_back(static_cast<std::uint8_t>(w >> (24 - 8 * k)));
    pw::Region data;
    data.address = pw::kArgsBase;
    data.data = Bytes(0x1000);
    const std::array<pw::Region, 2> regions = {code, data};
    const std::array<std::uint64_t, 3> args = {pw::kArgsBase + 0x100, 1, pw::kArgsBase + 0x200};   // r3, r4, r5
    pw::RunOptions options;
    options.max_steps = 20;
    const pw::RunResult r = emulate::pcode_cpu::run(regions, kCode, args, options);
    CB_CHECK_EQ(r.exit_text(), std::string("ret"));
    bool aligned_store = false, second_store = false;
    for (const pw::ByteWrite& w : r.writes) {
        if (w.address == pw::kArgsBase + 0x101 && w.after == 5) aligned_store = true;   // (0x100 + 1 aligned down is 0x100: the halfword 0x0005 puts its 5 at 0x101)
        if (w.address == pw::kArgsBase + 0x201 && w.after == 5) second_store = true;
    }
    CB_CHECK(aligned_store);
    CB_CHECK(second_store);
    CB_CHECK_EQ(r.writes.size(), static_cast<std::size_t>(2));
});


}  // namespace cb
