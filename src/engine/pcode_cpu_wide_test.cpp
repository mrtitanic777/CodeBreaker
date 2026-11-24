// cb-unity: alone (file-local names that other files in a batch also use)
// The one emulator's wide path (decompile/engine/pcode_wide.hh, pcode_wide_float.hh, pcode_emulator.cc): p-code operations on varnodes wider than 8 bytes. Each operation is
// checked against an independent computation, at sizes 1 to 8 against the description's behaviour table (the core's OpBehavior, what the narrow path runs) and at sizes 9 to 256
// against a bit-serial computation written here; the x87 extended float against exact cases, the host's doubles and a 128-bit significand model; then real instructions of the
// descriptions (x86-64's 128-bit multiply and divide, SSE moves and logic, x87; PowerPC's AltiVec loads and logic, a big-endian space) and the memory hooks' view of a wide access.
#include <algorithm>
#include <array>
#include <bit>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <filesystem>
#include <memory>
#include <optional>
#include <random>
#include <string>
#include <vector>

#include "base/config.hpp"
#include "core/fs.hpp"
#include "core/testing.hpp"
#include "decompile/engine/core/cpp/opcodes.hh"
#include "decompile/engine/pcode_emulator.hh"
#include "engine/pcode_cpu_internal.hpp"

namespace cb {
namespace {

namespace gh = ghidra;
namespace sl = decompile::sleigh;
using Bytes = std::vector<std::uint8_t>;
using Emu = sl::Emulator;

// ---- a bit-serial reference: a value is a vector of bits, least significant first. Nothing here shares a line with the emulator's byte arithmetic.
using Bits = std::vector<int>;

Bits to_bits(const Bytes& v, std::size_t n) {
    Bits b(8 * n, 0);
    for (std::size_t i = 0; i < 8 * n && i / 8 < v.size(); ++i) b[i] = (v[i / 8] >> (i % 8)) & 1;
    return b;
}
Bytes from_bits(const Bits& b) {
    Bytes v((b.size() + 7) / 8, 0);
    for (std::size_t i = 0; i < b.size(); ++i)
        if (b[i]) v[i / 8] = static_cast<std::uint8_t>(v[i / 8] | (1u << (i % 8)));
    return v;
}
Bits resized(const Bits& a, std::size_t bits, bool sign_extend) {
    Bits r(bits, 0);
    for (std::size_t i = 0; i < bits; ++i) r[i] = i < a.size() ? a[i] : (sign_extend && !a.empty() ? a.back() : 0);
    return r;
}
Bits bit_add(const Bits& a, const Bits& b, int carry_in, int* carry_out = nullptr) {
    Bits r(a.size());
    int carry = carry_in;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const int s = a[i] + b[i] + carry;
        r[i] = s & 1;
        carry = s >> 1;
    }
    if (carry_out) *carry_out = carry;
    return r;
}
Bits bit_not(const Bits& a) {
    Bits r = a;
    for (int& x : r) x ^= 1;
    return r;
}
Bits bit_neg(const Bits& a) { return bit_add(bit_not(a), Bits(a.size(), 0), 1); }
Bits bit_sub(const Bits& a, const Bits& b) { return bit_add(a, bit_not(b), 1); }
int bit_cmp_unsigned(const Bits& a, const Bits& b) {
    for (std::size_t i = a.size(); i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
int bit_cmp_signed(const Bits& a, const Bits& b) {
    Bits x = a, y = b;
    if (!x.empty()) {
        x.back() ^= 1;   // (flip the sign bits: signed order becomes unsigned order)
        y.back() ^= 1;
    }
    return bit_cmp_unsigned(x, y);
}
Bits bit_mul(const Bits& a, const Bits& b) {
    Bits r(a.size(), 0);
    for (std::size_t i = 0; i < b.size(); ++i) {
        if (!b[i]) continue;
        Bits shifted(a.size(), 0);
        for (std::size_t j = 0; j + i < a.size(); ++j) shifted[j + i] = a[j];
        r = bit_add(r, shifted, 0);
    }
    return r;
}
void bit_divmod(const Bits& a, const Bits& b, Bits& q, Bits& r) {
    q.assign(a.size(), 0);
    r.assign(a.size(), 0);
    for (std::size_t i = a.size(); i-- > 0;) {
        for (std::size_t j = r.size() - 1; j > 0; --j) r[j] = r[j - 1];
        r[0] = a[i];
        if (bit_cmp_unsigned(r, b) >= 0) {
            r = bit_sub(r, b);
            q[i] = 1;
        }
    }
}
std::uint64_t count_of(const Bytes& c, std::size_t cn) {   // a count: all ones when any bit above 64 is set
    std::uint64_t v = 0;
    for (std::size_t i = 0; i < cn; ++i) {
        if (i < 8) v |= std::uint64_t{c[i]} << (8 * i);
        else if (c[i]) return ~std::uint64_t{0};
    }
    return v;
}

// The meaning of an operation, one bit at a time. `a` has `an` bytes, `c` has `cn`, the result `rn`. Returns false for division by zero.
bool reference(unsigned op, std::size_t rn, const Bytes& a, std::size_t an, const Bytes& c, std::size_t cn, Bytes& out) {
    const std::size_t rbits = 8 * rn, abits = 8 * an;
    const Bits A = to_bits(a, an), C = to_bits(c, cn);
    Bits r;
    switch (op) {
        case gh::CPUI_COPY: case gh::CPUI_INT_ZEXT: r = resized(A, rbits, false); break;
        case gh::CPUI_INT_SEXT: r = resized(A, rbits, true); break;
        case gh::CPUI_PIECE: {   // a high, c low
            r.assign(rbits, 0);
            for (std::size_t i = 0; i < C.size() && i < rbits; ++i) r[i] = C[i];
            for (std::size_t i = 0; i < A.size() && C.size() + i < rbits; ++i) r[C.size() + i] = A[i];
            break;
        }
        case gh::CPUI_SUBPIECE: {
            const std::uint64_t off = 8 * count_of(c, cn);
            r.assign(rbits, 0);
            for (std::size_t i = 0; i < rbits; ++i)
                if (off < abits && off + i < abits) r[i] = A[static_cast<std::size_t>(off) + i];
            break;
        }
        case gh::CPUI_INT_AND: case gh::CPUI_INT_OR: case gh::CPUI_INT_XOR: {
            const Bits x = resized(A, rbits, false), y = resized(C, rbits, false);
            r.assign(rbits, 0);
            for (std::size_t i = 0; i < rbits; ++i) r[i] = op == gh::CPUI_INT_AND ? (x[i] & y[i]) : op == gh::CPUI_INT_OR ? (x[i] | y[i]) : (x[i] ^ y[i]);
            break;
        }
        case gh::CPUI_INT_ADD: r = bit_add(resized(A, rbits, false), resized(C, rbits, false), 0); break;
        case gh::CPUI_INT_SUB: r = bit_sub(resized(A, rbits, false), resized(C, rbits, false)); break;
        case gh::CPUI_INT_MULT: r = bit_mul(resized(A, rbits, false), resized(C, rbits, false)); break;
        case gh::CPUI_INT_NEGATE: r = bit_not(resized(A, rbits, false)); break;
        case gh::CPUI_INT_2COMP: r = bit_neg(resized(A, rbits, false)); break;
        case gh::CPUI_INT_LEFT: case gh::CPUI_INT_RIGHT: case gh::CPUI_INT_SRIGHT: {
            const Bits x = resized(A, rbits, op == gh::CPUI_INT_SRIGHT);
            const std::uint64_t k = count_of(c, cn);
            const int fill = op == gh::CPUI_INT_SRIGHT ? x.back() : 0;
            r.assign(rbits, op == gh::CPUI_INT_LEFT ? 0 : fill);
            if (k < rbits)
                for (std::size_t i = 0; i < rbits; ++i) {
                    if (op == gh::CPUI_INT_LEFT) r[i] = i >= k ? x[i - static_cast<std::size_t>(k)] : 0;
                    else r[i] = i + k < rbits ? x[i + static_cast<std::size_t>(k)] : fill;
                }
            break;
        }
        case gh::CPUI_INT_EQUAL: case gh::CPUI_INT_NOTEQUAL: case gh::CPUI_INT_LESS: case gh::CPUI_INT_LESSEQUAL: case gh::CPUI_INT_SLESS: case gh::CPUI_INT_SLESSEQUAL: {
            const Bits y = resized(C, abits, false);
            const int u = bit_cmp_unsigned(A, y), s = bit_cmp_signed(A, y);
            const bool yes = op == gh::CPUI_INT_EQUAL ? u == 0 : op == gh::CPUI_INT_NOTEQUAL ? u != 0 : op == gh::CPUI_INT_LESS ? u < 0 : op == gh::CPUI_INT_LESSEQUAL ? u <= 0
                           : op == gh::CPUI_INT_SLESS ? s < 0 : s <= 0;
            r.assign(rbits, 0);
            r[0] = yes ? 1 : 0;
            break;
        }
        case gh::CPUI_INT_CARRY: {
            int carry = 0;
            bit_add(A, resized(C, abits, false), 0, &carry);
            r.assign(rbits, 0);
            r[0] = carry;
            break;
        }
        case gh::CPUI_INT_SCARRY: case gh::CPUI_INT_SBORROW: {   // signed overflow: the result does not fit n bits, seen in n + 1 bits
            const Bits x = resized(A, abits + 1, true), y = resized(C, abits + 1, true);
            const Bits wide = op == gh::CPUI_INT_SCARRY ? bit_add(x, y, 0) : bit_sub(x, y);
            r.assign(rbits, 0);
            r[0] = wide[abits] != wide[abits - 1] ? 1 : 0;
            break;
        }
        case gh::CPUI_INT_DIV: case gh::CPUI_INT_REM: case gh::CPUI_INT_SDIV: case gh::CPUI_INT_SREM: {
            const std::size_t w = std::max(an, cn) * 8;
            const bool sgn = op == gh::CPUI_INT_SDIV || op == gh::CPUI_INT_SREM;
            Bits x = resized(A, w, sgn), y = resized(C, w, sgn);
            if (std::all_of(y.begin(), y.end(), [](int v) { return v == 0; })) return false;
            const bool nx = sgn && x.back(), ny = sgn && y.back();
            if (nx) x = bit_neg(x);
            if (ny) y = bit_neg(y);
            Bits q, rem;
            bit_divmod(x, y, q, rem);
            const bool quotient = op == gh::CPUI_INT_DIV || op == gh::CPUI_INT_SDIV;
            Bits pick = quotient ? q : rem;
            if (quotient ? nx != ny : nx) pick = bit_neg(pick);
            r = resized(pick, rbits, false);
            break;
        }
        case gh::CPUI_POPCOUNT: {
            std::uint64_t n = 0;
            for (int bit : A) n += bit;
            r = resized(to_bits(Bytes{static_cast<std::uint8_t>(n), static_cast<std::uint8_t>(n >> 8)}, 2), rbits, false);
            break;
        }
        case gh::CPUI_LZCOUNT: {
            std::uint64_t n = 0;
            for (std::size_t i = A.size(); i-- > 0 && !A[i];) ++n;
            r = resized(to_bits(Bytes{static_cast<std::uint8_t>(n), static_cast<std::uint8_t>(n >> 8)}, 2), rbits, false);
            break;
        }
        default: return false;
    }
    out = from_bits(r);
    out.resize(rn);
    return true;
}

// The emulator's wide evaluation.
bool wide_eval(unsigned op, std::size_t rn, const Bytes& a, std::size_t an, const Bytes& c, std::size_t cn, Bytes& out, bool& by_zero) {
    out.assign(rn, 0xEE);   // (poisoned: an operation must write every byte)
    const bool ok = Emu::evaluateWideOp(op, out.data(), rn, a.data(), an, c.data(), cn, by_zero);
    return ok;
}

Bytes random_value(std::mt19937_64& rng, std::size_t n) {
    Bytes v(n, 0);
    switch (rng() % 8) {
        case 0: break;                                   // zero
        case 1: v[0] = 1; break;                         // one
        case 2: std::fill(v.begin(), v.end(), std::uint8_t{0xFF}); break;
        case 3: v[n - 1] = 0x80; break;                  // the smallest signed
        case 4: std::fill(v.begin(), v.end(), std::uint8_t{0xFF}); v[n - 1] = 0x7F; break;   // the largest signed
        case 5:                                          // sparse
            for (std::size_t i = 0; i < n; ++i) v[i] = (rng() & 3) == 0 ? static_cast<std::uint8_t>(rng()) : 0;
            break;
        default:
            for (std::size_t i = 0; i < n; ++i) v[i] = static_cast<std::uint8_t>(rng());
            break;
    }
    return v;
}

constexpr std::array<unsigned, 29> kIntegerOps = {
    gh::CPUI_COPY, gh::CPUI_INT_ZEXT, gh::CPUI_INT_SEXT, gh::CPUI_INT_ADD, gh::CPUI_INT_SUB, gh::CPUI_INT_MULT, gh::CPUI_INT_AND, gh::CPUI_INT_OR, gh::CPUI_INT_XOR,
    gh::CPUI_INT_EQUAL, gh::CPUI_INT_NOTEQUAL, gh::CPUI_INT_LESS, gh::CPUI_INT_LESSEQUAL, gh::CPUI_INT_SLESS, gh::CPUI_INT_SLESSEQUAL, gh::CPUI_INT_LEFT, gh::CPUI_INT_RIGHT,
    gh::CPUI_INT_SRIGHT, gh::CPUI_SUBPIECE, gh::CPUI_PIECE, gh::CPUI_INT_CARRY, gh::CPUI_INT_SCARRY, gh::CPUI_INT_SBORROW, gh::CPUI_INT_NEGATE, gh::CPUI_INT_2COMP,
    gh::CPUI_INT_DIV, gh::CPUI_INT_REM, gh::CPUI_INT_SDIV, gh::CPUI_INT_SREM};

bool unary_op(unsigned op) {
    return op == gh::CPUI_COPY || op == gh::CPUI_INT_ZEXT || op == gh::CPUI_INT_SEXT || op == gh::CPUI_INT_NEGATE || op == gh::CPUI_INT_2COMP || op == gh::CPUI_POPCOUNT ||
           op == gh::CPUI_LZCOUNT;
}
bool compare_op(unsigned op) {
    return op == gh::CPUI_INT_EQUAL || op == gh::CPUI_INT_NOTEQUAL || op == gh::CPUI_INT_LESS || op == gh::CPUI_INT_LESSEQUAL || op == gh::CPUI_INT_SLESS ||
           op == gh::CPUI_INT_SLESSEQUAL || op == gh::CPUI_INT_CARRY || op == gh::CPUI_INT_SCARRY || op == gh::CPUI_INT_SBORROW;
}
bool shift_op(unsigned op) { return op == gh::CPUI_INT_LEFT || op == gh::CPUI_INT_RIGHT || op == gh::CPUI_INT_SRIGHT; }

std::uint64_t as_u64(const Bytes& v) {
    std::uint64_t r = 0;
    for (std::size_t i = 0; i < v.size() && i < 8; ++i) r |= std::uint64_t{v[i]} << (8 * i);
    return r;
}
Bytes from_u64(std::uint64_t v, std::size_t n) {
    Bytes r(n, 0);
    for (std::size_t i = 0; i < n && i < 8; ++i) r[i] = static_cast<std::uint8_t>(v >> (8 * i));
    return r;
}
std::string hex(const Bytes& v) {
    std::string s;
    char buf[4];
    for (std::size_t i = v.size(); i-- > 0;) {
        std::snprintf(buf, sizeof buf, "%02x", v[i]);
        s += buf;
    }
    return s;
}

}  // namespace

// Sizes 1 to 8: the wide path against the behaviour table, so it is the same operation as the one the narrow path (and the decompiler) runs.
CB_TEST("engine.pcode_cpu.wide_integer_operations_at_8_bytes_and_under_match_the_behaviour_table", [] {
    Emu& emu = engine::pcode_cpu::emulator_named("6502.sla");
    std::mt19937_64 rng(20261123);
    unsigned checked = 0, different = 0;
    const auto mask = [](std::size_t n) { return n >= 8 ? ~std::uint64_t{0} : (std::uint64_t{1} << (8 * n)) - 1; };
    for (const unsigned op : kIntegerOps) {
        for (std::size_t n = 1; n <= 8; ++n)
            for (std::size_t m = 1; m <= 8; ++m) {   // n: the output's size, m: the first input's
                const bool same = !(op == gh::CPUI_INT_ZEXT || op == gh::CPUI_INT_SEXT || op == gh::CPUI_SUBPIECE || op == gh::CPUI_PIECE || compare_op(op));
                if (same && n != m) continue;
                if ((op == gh::CPUI_INT_ZEXT || op == gh::CPUI_INT_SEXT) && n < m) continue;
                if (op == gh::CPUI_SUBPIECE && n > m) continue;
                if (op == gh::CPUI_PIECE && n + m > 8) continue;
                if (compare_op(op) && n != 1) continue;
                for (int k = 0; k < 600; ++k) {
                    const Bytes a = random_value(rng, m);
                    const std::size_t cn = op == gh::CPUI_PIECE ? n : shift_op(op) ? 1 : op == gh::CPUI_SUBPIECE ? 1 : m;
                    Bytes c = random_value(rng, cn);
                    if (shift_op(op)) c[0] = static_cast<std::uint8_t>(rng() % 80);
                    if (op == gh::CPUI_SUBPIECE) c[0] = static_cast<std::uint8_t>(rng() % (m + 2));
                    const std::size_t out_size = op == gh::CPUI_PIECE ? n + m : n;
                    const std::uint64_t av = as_u64(a), cv = as_u64(c);
                    if ((op == gh::CPUI_INT_DIV || op == gh::CPUI_INT_REM || op == gh::CPUI_INT_SDIV || op == gh::CPUI_INT_SREM) && cv == 0) continue;
                    if ((op == gh::CPUI_INT_SDIV || op == gh::CPUI_INT_SREM) && m == 8 && av == 0x8000000000000000ull && cv == ~std::uint64_t{0}) continue;   // (the host's own overflow)
                    std::uint64_t table = 0;
                    const int in_size = static_cast<int>(m);
                    if (!emu.evaluateOp(op, static_cast<int>(out_size), in_size, av, cv, table, false)) {
                        CB_CHECK(false);
                        continue;
                    }
                    table &= mask(out_size);
                    Bytes got;
                    bool by_zero = false;
                    CB_CHECK(wide_eval(op, out_size, a, m, c, cn, got, by_zero));
                    CB_CHECK(!by_zero);
                    ++checked;
                    if (as_u64(got) != table && ++different <= 10)
                        std::printf("  op %u out %zu in %zu: a=%llx c=%llx: wide %llx, table %llx\n", op, out_size, m, static_cast<unsigned long long>(av), static_cast<unsigned long long>(cv),
                                    static_cast<unsigned long long>(as_u64(got)), static_cast<unsigned long long>(table));
                }
            }
    }
    std::printf("wide ops at 1-8 bytes: %u evaluations, %u differ from the table\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

// Sizes 9 to 256 (and 1 to 8 too): the wide path against the bit-serial reference above, both byte orders being no concern of the values' (they are in value order).
CB_TEST("engine.pcode_cpu.wide_integer_operations_match_a_bit_serial_computation", [] {
    std::mt19937_64 rng(20261124);
    const std::array<std::size_t, 16> sizes = {9, 10, 12, 15, 16, 17, 24, 31, 32, 33, 40, 48, 63, 64, 128, 256};
    unsigned checked = 0, different = 0, zero_divisions = 0;
    for (const unsigned op : kIntegerOps) {
        for (const std::size_t n : sizes) {
            // (the reference is slow at the big sizes: fewer cases there)
            const int cases = n <= 64 ? 120 : 6;
            for (int k = 0; k < cases; ++k) {
                std::size_t an = n, cn = n, rn = n;
                if (op == gh::CPUI_INT_ZEXT || op == gh::CPUI_INT_SEXT) an = 1 + rng() % (n - 1), rn = n;
                if (op == gh::CPUI_SUBPIECE) rn = 1 + rng() % n;
                if (op == gh::CPUI_PIECE) {
                    an = 1 + rng() % (n - 1);
                    cn = n - an;
                }
                if (compare_op(op)) rn = 1;
                const Bytes a = random_value(rng, an);
                Bytes c = random_value(rng, cn);
                if (shift_op(op)) {
                    cn = 1 + rng() % 3;
                    c = from_u64(rng() % (8 * n + 40), cn);
                    if (rng() % 16 == 0 && cn >= 3) c[2] = 1;   // (a count over 64k: everything shifts out)
                }
                if (op == gh::CPUI_SUBPIECE) {
                    cn = 1 + rng() % 2;
                    c = from_u64(rng() % (an + 3), cn);
                }
                if (compare_op(op)) {
                    an = n;
                    cn = n;
                }
                Bytes want, got;
                const bool ref_ok = reference(op, rn, a, an, c, cn, want);
                bool by_zero = false;
                const bool ok = wide_eval(op, rn, a, an, c, cn, got, by_zero);
                if (!ref_ok) {   // (a division by zero: the emulator says so and leaves the output alone)
                    ++zero_divisions;
                    CB_CHECK(ok);
                    CB_CHECK(by_zero);
                    continue;
                }
                CB_CHECK(ok);
                CB_CHECK(!by_zero);
                ++checked;
                if (got != want && ++different <= 10)
                    std::printf("  op %u out %zu in %zu/%zu: a=%s c=%s: wide %s, reference %s\n", op, rn, an, cn, hex(a).c_str(), hex(c).c_str(), hex(got).c_str(), hex(want).c_str());
            }
        }
    }
    // POPCOUNT and LZCOUNT take one input and give a small count
    for (const unsigned op : {static_cast<unsigned>(gh::CPUI_POPCOUNT), static_cast<unsigned>(gh::CPUI_LZCOUNT)})
        for (const std::size_t n : sizes)
            for (int k = 0; k < 40; ++k) {
                const Bytes a = random_value(rng, n);
                Bytes want, got;
                CB_CHECK(reference(op, 2, a, n, {}, 0, want));
                bool by_zero = false;
                CB_CHECK(wide_eval(op, 2, a, n, {}, 0, got, by_zero));
                ++checked;
                if (got != want && ++different <= 10) std::printf("  op %u size %zu: a=%s: wide %s, reference %s\n", op, n, hex(a).c_str(), hex(got).c_str(), hex(want).c_str());
            }
    std::printf("wide ops at 9-256 bytes: %u evaluations (%u divisions by zero), %u differ from the bit-serial reference\n", checked, zero_divisions, different);
    CB_CHECK_EQ(different, 0u);
});

// 16, 24 and 32 bytes through the table by 64-bit limbs: a second reference that works on a different split of the bytes than a bit at a time.
CB_TEST("engine.pcode_cpu.wide_arithmetic_matches_the_behaviour_table_by_64_bit_limbs", [] {
    Emu& emu = engine::pcode_cpu::emulator_named("6502.sla");
    std::mt19937_64 rng(20261125);
    const auto table = [&](unsigned op, std::uint64_t a, std::uint64_t c) {
        std::uint64_t r = 0;
        CB_CHECK(emu.evaluateOp(op, 8, 8, a, c, r, false));
        return r;
    };
    unsigned checked = 0, different = 0;
    for (const std::size_t n : {16u, 24u, 32u})
        for (int k = 0; k < 400; ++k) {
            const std::size_t limbs = n / 8;
            const Bytes a = random_value(rng, n), c = random_value(rng, n);
            std::vector<std::uint64_t> la(limbs), lc(limbs);
            for (std::size_t i = 0; i < limbs; ++i) {
                la[i] = as_u64(Bytes(a.begin() + 8 * i, a.begin() + 8 * i + 8));
                lc[i] = as_u64(Bytes(c.begin() + 8 * i, c.begin() + 8 * i + 8));
            }
            const auto join = [&](const std::vector<std::uint64_t>& l) {
                Bytes r;
                for (const std::uint64_t v : l)
                    for (int i = 0; i < 8; ++i) r.push_back(static_cast<std::uint8_t>(v >> (8 * i)));
                return r;
            };
            const auto check = [&](unsigned op, const std::vector<std::uint64_t>& want_limbs, std::size_t rn) {
                Bytes got;
                bool by_zero = false;
                CB_CHECK(wide_eval(op, rn, a, n, c, n, got, by_zero));
                Bytes want = join(want_limbs);
                want.resize(rn);
                ++checked;
                if (got != want && ++different <= 10) std::printf("  limbs: op %u size %zu: a=%s c=%s: wide %s, limbs %s\n", op, n, hex(a).c_str(), hex(c).c_str(), hex(got).c_str(), hex(want).c_str());
            };
            std::vector<std::uint64_t> r(limbs);
            // add and subtract: the carry or borrow of each limb from the table's own CARRY and SBORROW-free formula
            std::uint64_t carry = 0;
            for (std::size_t i = 0; i < limbs; ++i) {
                const std::uint64_t s = table(gh::CPUI_INT_ADD, la[i], lc[i]);
                const std::uint64_t c1 = table(gh::CPUI_INT_CARRY, la[i], lc[i]);
                const std::uint64_t t = table(gh::CPUI_INT_ADD, s, carry);
                const std::uint64_t c2 = table(gh::CPUI_INT_CARRY, s, carry);
                r[i] = t;
                carry = c1 | c2;
            }
            check(gh::CPUI_INT_ADD, r, n);
            std::uint64_t borrow = 0;
            for (std::size_t i = 0; i < limbs; ++i) {
                const std::uint64_t d = table(gh::CPUI_INT_SUB, la[i], lc[i]);
                const std::uint64_t b1 = table(gh::CPUI_INT_LESS, la[i], lc[i]);
                const std::uint64_t t = table(gh::CPUI_INT_SUB, d, borrow);
                const std::uint64_t b2 = table(gh::CPUI_INT_LESS, d, borrow);
                r[i] = t;
                borrow = b1 | b2;
            }
            check(gh::CPUI_INT_SUB, r, n);
            for (const unsigned op : {static_cast<unsigned>(gh::CPUI_INT_AND), static_cast<unsigned>(gh::CPUI_INT_OR), static_cast<unsigned>(gh::CPUI_INT_XOR)}) {
                for (std::size_t i = 0; i < limbs; ++i) r[i] = table(op, la[i], lc[i]);
                check(op, r, n);
            }
            // unsigned less: the highest limb that differs decides
            std::uint64_t less = 0, equal = 1;
            for (std::size_t i = limbs; i-- > 0;) {
                if (table(gh::CPUI_INT_NOTEQUAL, la[i], lc[i])) {
                    less = table(gh::CPUI_INT_LESS, la[i], lc[i]);
                    equal = 0;
                    break;
                }
            }
            check(gh::CPUI_INT_LESS, {less}, 1);
            check(gh::CPUI_INT_EQUAL, {equal}, 1);
            // the low limb of a product: the table's multiply of the low limbs
            for (std::size_t i = 0; i < limbs; ++i) r[i] = i == 0 ? table(gh::CPUI_INT_MULT, la[0], lc[0]) : 0;
            Bytes got;
            bool by_zero = false;
            CB_CHECK(wide_eval(gh::CPUI_INT_MULT, 8, a, n, c, n, got, by_zero));   // (a product narrowed to the low limb)
            ++checked;
            if (as_u64(got) != r[0]) ++different;
            // left and right shift by whole limbs and part of one
            const unsigned shift = static_cast<unsigned>(rng() % 64);
            const std::size_t limb_shift = rng() % limbs;
            const unsigned total = static_cast<unsigned>(64 * limb_shift + shift);
            std::vector<std::uint64_t> shifted(limbs, 0);
            for (std::size_t i = 0; i + limb_shift < limbs; ++i) {   // right
                std::uint64_t v = table(gh::CPUI_INT_RIGHT, la[i + limb_shift], shift);
                if (shift != 0 && i + limb_shift + 1 < limbs) v |= table(gh::CPUI_INT_LEFT, la[i + limb_shift + 1], 64 - shift);
                shifted[i] = v;
            }
            Bytes count = from_u64(total, 2);
            Bytes got_right;
            CB_CHECK(wide_eval(gh::CPUI_INT_RIGHT, n, a, n, count, 2, got_right, by_zero));
            ++checked;
            if (got_right != join(shifted) && ++different <= 10) std::printf("  limbs: right shift by %u of %s: %s, limbs %s\n", total, hex(a).c_str(), hex(got_right).c_str(), hex(join(shifted)).c_str());
            std::vector<std::uint64_t> lefted(limbs, 0);
            for (std::size_t i = limb_shift; i < limbs; ++i) {
                std::uint64_t v = table(gh::CPUI_INT_LEFT, la[i - limb_shift], shift);
                if (shift != 0 && i - limb_shift >= 1) v |= table(gh::CPUI_INT_RIGHT, la[i - limb_shift - 1], 64 - shift);
                lefted[i] = v;
            }
            Bytes got_left;
            CB_CHECK(wide_eval(gh::CPUI_INT_LEFT, n, a, n, count, 2, got_left, by_zero));
            ++checked;
            if (got_left != join(lefted) && ++different <= 10) std::printf("  limbs: left shift by %u of %s: %s, limbs %s\n", total, hex(a).c_str(), hex(got_left).c_str(), hex(join(lefted)).c_str());
        }
    std::printf("wide ops by limbs: %u evaluations, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

namespace {

// ---- the x87 extended float. A value is 10 bytes: the 64-bit significand with its integer bit, then sign and 15-bit exponent.
Bytes f80(bool sign, unsigned exp_field, std::uint64_t mant) {
    Bytes v(10);
    for (int i = 0; i < 8; ++i) v[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(mant >> (8 * i));
    const unsigned se = (sign ? 0x8000u : 0u) | exp_field;
    v[8] = static_cast<std::uint8_t>(se);
    v[9] = static_cast<std::uint8_t>(se >> 8);
    return v;
}
// A double as the extended float it equals (every double is one), made here from the fields.
Bytes f80_of(double d) {
    std::uint64_t bits;
    std::memcpy(&bits, &d, 8);
    const bool sign = (bits >> 63) != 0;
    const unsigned e = static_cast<unsigned>((bits >> 52) & 0x7FF);
    const std::uint64_t frac = bits & ((std::uint64_t{1} << 52) - 1);
    if (e == 0x7FF) return f80(sign, 0x7FFF, (std::uint64_t{1} << 63) | (frac << 11));
    if (e == 0) {
        if (frac == 0) return f80(sign, 0, 0);
        int shift = 0;
        std::uint64_t m = frac;
        while (!(m >> 63)) m <<= 1, ++shift;
        return f80(sign, static_cast<unsigned>(63 - shift - 1074 + 16383), m);   // (the leading bit of the fraction, at 63 - shift, weighs 2^-1074 per unit)
    }
    return f80(sign, e - 1023 + 16383, (std::uint64_t{1} << 63) | (frac << 11));
}
std::uint64_t f80_mant(const Bytes& v) { return as_u64(v); }
unsigned f80_exp(const Bytes& v) { return (v[8] | (v[9] << 8)) & 0x7FFF; }
bool f80_sign(const Bytes& v) { return (v[9] & 0x80) != 0; }
bool f80_is_nan(const Bytes& v) { return f80_exp(v) == 0x7FFF && (f80_mant(v) << 1) != 0; }
bool f80_is_inf(const Bytes& v) { return f80_exp(v) == 0x7FFF && (f80_mant(v) << 1) == 0; }
bool f80_is_zero(const Bytes& v) { return f80_exp(v) == 0 && f80_mant(v) == 0; }

Bytes fop(unsigned op, const Bytes& a, const Bytes& c = {}, std::size_t out = 10) {
    Bytes r(out, 0xEE);
    bool z = false;
    CB_CHECK(Emu::evaluateWideOp(op, r.data(), out, a.data(), a.size(), c.data(), c.size(), z));
    return r;
}

// ---- a small unsigned big integer (32-bit limbs) for the exact reference of the float operations
using Big = std::vector<std::uint32_t>;
Big big_of(std::uint64_t v) { return {static_cast<std::uint32_t>(v), static_cast<std::uint32_t>(v >> 32)}; }
void big_trim(Big& a) {
    while (!a.empty() && a.back() == 0) a.pop_back();
}
Big big_add(Big a, const Big& b) {
    if (a.size() < b.size()) a.resize(b.size(), 0);
    std::uint64_t carry = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t s = std::uint64_t{a[i]} + (i < b.size() ? b[i] : 0) + carry;
        a[i] = static_cast<std::uint32_t>(s);
        carry = s >> 32;
    }
    if (carry) a.push_back(static_cast<std::uint32_t>(carry));
    return a;
}
int big_cmp(Big a, Big b) {
    big_trim(a);
    big_trim(b);
    if (a.size() != b.size()) return a.size() < b.size() ? -1 : 1;
    for (std::size_t i = a.size(); i-- > 0;)
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    return 0;
}
Big big_sub(Big a, const Big& b) {   // a >= b
    std::int64_t borrow = 0;
    for (std::size_t i = 0; i < a.size(); ++i) {
        std::int64_t d = std::int64_t{a[i]} - (i < b.size() ? b[i] : 0) - borrow;
        borrow = d < 0 ? 1 : 0;
        if (d < 0) d += (std::int64_t{1} << 32);
        a[i] = static_cast<std::uint32_t>(d);
    }
    big_trim(a);
    return a;
}
Big big_shl(const Big& a, unsigned bits) {
    Big r(a.size() + bits / 32 + 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i) {
        const std::uint64_t v = std::uint64_t{a[i]} << (bits % 32);
        r[i + bits / 32] |= static_cast<std::uint32_t>(v);
        r[i + bits / 32 + 1] |= static_cast<std::uint32_t>(v >> 32);
    }
    big_trim(r);
    return r;
}
Big big_mul(const Big& a, const Big& b) {
    Big r(a.size() + b.size() + 1, 0);
    for (std::size_t i = 0; i < a.size(); ++i) {
        std::uint64_t carry = 0;
        for (std::size_t j = 0; j < b.size() || carry; ++j) {
            const std::uint64_t t = r[i + j] + (j < b.size() ? std::uint64_t{a[i]} * b[j] : 0) + carry;
            r[i + j] = static_cast<std::uint32_t>(t);
            carry = t >> 32;
        }
    }
    big_trim(r);
    return r;
}
unsigned big_bits(Big a) {
    big_trim(a);
    if (a.empty()) return 0;
    return static_cast<unsigned>(32 * (a.size() - 1)) + (32 - static_cast<unsigned>(std::countl_zero(a.back())));
}
bool big_bit(const Big& a, unsigned i) { return i / 32 < a.size() && ((a[i / 32] >> (i % 32)) & 1) != 0; }
// q = a / b by shift and subtract, with the remainder
void big_divmod(const Big& a, const Big& b, Big& q, Big& r) {
    q.assign(a.size(), 0);
    r.clear();
    for (unsigned i = big_bits(a); i-- > 0;) {
        r = big_shl(r, 1);
        if (big_bit(a, i)) r = big_add(r, big_of(1));
        if (big_cmp(r, b) >= 0) {
            r = big_sub(r, b);
            q[i / 32] |= 1u << (i % 32);
        }
    }
    big_trim(q);
    big_trim(r);
}
// value = big * 2^scale (+ a little if sticky) as the extended float of nearest even; only for results in the normal range.
Bytes f80_round(bool sign, Big v, long scale, bool sticky) {
    big_trim(v);
    const unsigned bits = big_bits(v);
    const long exp = scale + static_cast<long>(bits) - 1;   // the leading bit's weight
    std::uint64_t mant = 0;
    bool guard = false, rest = sticky;
    if (bits <= 64) {
        for (unsigned i = 0; i < bits; ++i)
            if (big_bit(v, i)) mant |= std::uint64_t{1} << (i + 64 - bits);
    } else {
        for (unsigned i = 0; i < 64; ++i)
            if (big_bit(v, bits - 64 + i)) mant |= std::uint64_t{1} << i;
        guard = big_bit(v, bits - 65);
        for (unsigned i = 0; i + 65 < bits; ++i)
            if (big_bit(v, i)) rest = true;
    }
    long e = exp;
    if (guard && (rest || (mant & 1))) {
        if (++mant == 0) mant = std::uint64_t{1} << 63, ++e;
    }
    return f80(sign, static_cast<unsigned>(e + 16383), mant);
}
Big big_of_mant(const Bytes& v) { return big_of(f80_mant(v)); }
long f80_unbiased(const Bytes& v) { return static_cast<long>(f80_exp(v)) - 16383; }   // (the exponent of the leading bit)

Bytes random_f80(std::mt19937_64& rng, long exp_low, long exp_high) {
    const std::uint64_t mant = (std::uint64_t{1} << 63) | (rng() & ~(std::uint64_t{1} << 63));
    const long e = exp_low + static_cast<long>(rng() % static_cast<std::uint64_t>(exp_high - exp_low + 1));
    return f80(rng() & 1, static_cast<unsigned>(e + 16383), (rng() % 5 == 0) ? (mant & 0xFFFFFFFF00000000ull) | (std::uint64_t{1} << 63) : mant);
}

}  // namespace

// The extended float's arithmetic against exact integer models (a bigint of its own here), the host's doubles where an operation is exact in a double, and the rounding
// decisions written out.
CB_TEST("engine.pcode_cpu.x87_extended_float_operations_round_as_the_processor_does", [] {
    std::mt19937_64 rng(20261126);
    unsigned checked = 0, different = 0;
    const auto note = [&](const char* what, const Bytes& got, const Bytes& want) {
        ++checked;
        if (got != want && ++different <= 10) std::printf("  x87 %s: got %s, want %s\n", what, hex(got).c_str(), hex(want).c_str());
    };
    // add, subtract, multiply, divide: random normal values, rounded from the exact result by the model above
    for (int k = 0; k < 4000; ++k) {
        const long span = k % 3 == 0 ? 3 : k % 3 == 1 ? 70 : 200;   // (how far apart the exponents may be)
        const Bytes a = random_f80(rng, -100, -100 + span), b = random_f80(rng, -100, -100 + span);
        const bool sa = f80_sign(a), sb = f80_sign(b);
        // add / subtract: align to the smaller exponent
        for (const bool subtract : {false, true}) {
            const bool sbe = subtract ? !sb : sb;
            const long ea = f80_unbiased(a), eb = f80_unbiased(b), low = std::min(ea, eb);
            const Big ma = big_shl(big_of_mant(a), static_cast<unsigned>(ea - low)), mb = big_shl(big_of_mant(b), static_cast<unsigned>(eb - low));
            Bytes want;
            if (sa == sbe) want = f80_round(sa, big_add(ma, mb), low - 63, false);
            else if (big_cmp(ma, mb) == 0) want = f80(false, 0, 0);
            else if (big_cmp(ma, mb) > 0) want = f80_round(sa, big_sub(ma, mb), low - 63, false);
            else want = f80_round(sbe, big_sub(mb, ma), low - 63, false);
            note(subtract ? "sub" : "add", fop(subtract ? gh::CPUI_FLOAT_SUB : gh::CPUI_FLOAT_ADD, a, b), want);
        }
        // multiply
        {
            const Big p = big_mul(big_of_mant(a), big_of_mant(b));
            note("mult", fop(gh::CPUI_FLOAT_MULT, a, b), f80_round(sa != sb, p, f80_unbiased(a) + f80_unbiased(b) - 126, false));
        }
        // divide: the quotient to 130 bits and the remainder
        {
            Big q, r;
            big_divmod(big_shl(big_of_mant(a), 130), big_of_mant(b), q, r);
            note("div", fop(gh::CPUI_FLOAT_DIV, a, b), f80_round(sa != sb, q, f80_unbiased(a) - f80_unbiased(b) - 130, !r.empty()));
        }
        // square root of a positive value: the largest root whose square does not pass the value
        {
            Bytes p = a;
            p[9] &= 0x7F;
            const long e2 = f80_unbiased(p) - 63;
            const bool odd = (e2 & 1) != 0;
            const Big value = big_shl(big_of_mant(p), 132 + (odd ? 1u : 0u));
            Big root = {0};
            for (long bit = 100; bit >= 0; --bit) {
                Big cand = root;
                cand.resize(std::max<std::size_t>(cand.size(), static_cast<std::size_t>(bit / 32 + 1)), 0);
                cand[static_cast<std::size_t>(bit / 32)] |= 1u << (bit % 32);
                if (big_cmp(big_mul(cand, cand), value) <= 0) root = cand;
            }
            const bool exact = big_cmp(big_mul(root, root), value) == 0;
            note("sqrt", fop(gh::CPUI_FLOAT_SQRT, p), f80_round(false, root, (e2 - (odd ? 1 : 0)) / 2 - 66, !exact));
        }
    }
    // operations that are exact in a double: the host's own arithmetic is the answer
    for (int k = 0; k < 3000; ++k) {
        const auto small = [&](int exp_span) {   // a value with at most 26 significant bits
            const std::uint64_t m = rng() & ((std::uint64_t{1} << 26) - 1);
            return std::ldexp(static_cast<double>(m | 1), static_cast<int>(rng() % static_cast<std::uint64_t>(exp_span)) - 20) * ((rng() & 1) ? -1.0 : 1.0);
        };
        const double x = small(20), y = small(20);
        note("add exact", fop(gh::CPUI_FLOAT_ADD, f80_of(x), f80_of(y)), f80_of(x + y));
        note("sub exact", fop(gh::CPUI_FLOAT_SUB, f80_of(x), f80_of(y)), f80_of(x - y));
        note("mult exact", fop(gh::CPUI_FLOAT_MULT, f80_of(x), f80_of(y)), f80_of(x * y));
        note("div exact", fop(gh::CPUI_FLOAT_DIV, f80_of(x * y), f80_of(y)), f80_of(x));
        note("sqrt exact", fop(gh::CPUI_FLOAT_SQRT, f80_of(std::fabs(x * x))), f80_of(std::fabs(x)));
        note("neg", fop(gh::CPUI_FLOAT_NEG, f80_of(x)), f80_of(-x));
        note("abs", fop(gh::CPUI_FLOAT_ABS, f80_of(x)), f80_of(std::fabs(x)));
        // narrowing to a double and back is the identity on a double; widening is exact
        note("to double", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80_of(x), {}, 8), [&] { Bytes d(8); std::memcpy(d.data(), &x, 8); return d; }());
        Bytes d(8);
        std::memcpy(d.data(), &x, 8);
        note("from double", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, d, {}, 10), f80_of(x));
        const float xf = static_cast<float>(x);
        Bytes fb(4);
        std::memcpy(fb.data(), &xf, 4);
        note("to float", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80_of(static_cast<double>(xf)), {}, 4), fb);
        note("from float", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, fb, {}, 10), f80_of(static_cast<double>(xf)));
        // a double that needs rounding going to a float, through the extended format (one rounding: from the exact value)
        const double wide_double = x * 1.000000059604644775390625 + y * 1e-12;
        const float rounded = static_cast<float>(wide_double);
        Bytes rb(4);
        std::memcpy(rb.data(), &rounded, 4);
        note("round to float", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80_of(wide_double), {}, 4), rb);
        // rounding to whole numbers (a quarter is exact)
        const double q = static_cast<double>(static_cast<std::int64_t>(rng() % 4000001) - 2000000) / 4.0;
        note("floor", fop(gh::CPUI_FLOAT_FLOOR, f80_of(q)), f80_of(std::floor(q)));
        note("ceil", fop(gh::CPUI_FLOAT_CEIL, f80_of(q)), f80_of(std::ceil(q)));
        note("round", fop(gh::CPUI_FLOAT_ROUND, f80_of(q)), f80_of(std::floor(q + 0.5)));
        // comparisons
        const auto flag = [&](unsigned op, const Bytes& l, const Bytes& r) { return fop(op, l, r, 1)[0]; };
        const Bytes fx = f80_of(x), fy = f80_of(y);
        note("less", {flag(gh::CPUI_FLOAT_LESS, fx, fy)}, {static_cast<std::uint8_t>(x < y)});
        note("lessequal", {flag(gh::CPUI_FLOAT_LESSEQUAL, fx, fy)}, {static_cast<std::uint8_t>(x <= y)});
        note("equal", {flag(gh::CPUI_FLOAT_EQUAL, fx, fy)}, {static_cast<std::uint8_t>(x == y)});
        note("notequal", {flag(gh::CPUI_FLOAT_NOTEQUAL, fx, fy)}, {static_cast<std::uint8_t>(x != y)});
    }
    // integers: every size converts exactly and truncates back; a double's truncation is the host's
    for (int k = 0; k < 3000; ++k) {
        const std::size_t n = 1 + rng() % 8;
        std::uint64_t v = rng();
        if (n < 8) v &= (std::uint64_t{1} << (8 * n)) - 1;
        const Bytes in = from_u64(v, n);
        const Bytes ext = fop(gh::CPUI_FLOAT_INT2FLOAT, in);
        note("int to float", fop(gh::CPUI_FLOAT_TRUNC, ext, {}, n), in);
        // (the value as the host's signed number)
        const std::int64_t sv = n == 8 ? static_cast<std::int64_t>(v) : (static_cast<std::int64_t>(v << (64 - 8 * n)) >> (64 - 8 * n));
        if (std::llabs(sv) < (std::int64_t{1} << 52)) note("int to float exact", ext, f80_of(static_cast<double>(sv)));
    }
    for (int k = 0; k < 3000; ++k) {
        const double x = (static_cast<double>(static_cast<std::int64_t>(rng() % 2000001) - 1000000) / 7.0);
        note("trunc", fop(gh::CPUI_FLOAT_TRUNC, f80_of(x), {}, 8), from_u64(static_cast<std::uint64_t>(static_cast<std::int64_t>(x)), 8));
        note("trunc 4", fop(gh::CPUI_FLOAT_TRUNC, f80_of(x), {}, 4), from_u64(static_cast<std::uint64_t>(static_cast<std::int64_t>(x)), 4));
    }
    // truncation to integers wider than 8 bytes (the x87's own result is 10 bytes wide here): the integer part from the significand by a bigint, the range checked at each size
    for (int k = 0; k < 3000; ++k) {
        const std::uint64_t m = (rng() & ((std::uint64_t{1} << 53) - 1)) | (std::uint64_t{1} << 52);   // a 53-bit significand
        const int e = static_cast<int>(rng() % 100);                                                    // the integer is m * 2^(e - 52), up to 2^100
        const bool negative = (rng() & 1) != 0;
        const double x = std::ldexp(static_cast<double>(m), e - 52) * (negative ? -1.0 : 1.0);
        Big integer = big_of(m);
        if (e >= 52) integer = big_shl(integer, static_cast<unsigned>(e - 52));
        else
            for (int i = 0; i < 52 - e; ++i) {   // (shifted right a bit at a time)
                for (std::size_t j = 0; j < integer.size(); ++j) integer[j] = (integer[j] >> 1) | (j + 1 < integer.size() ? integer[j + 1] << 31 : 0);
                big_trim(integer);
            }
        for (const std::size_t n : {10u, 12u, 16u}) {
            Bytes want(n, 0);
            for (std::size_t i = 0; i < n; ++i)
                if (i / 4 < integer.size()) want[i] = static_cast<std::uint8_t>(integer[i / 4] >> (8 * (i % 4)));
            const bool fits = e < static_cast<int>(8 * n) - 1 || (e == static_cast<int>(8 * n) - 1 && negative && m == (std::uint64_t{1} << 52));
            if (!fits || (big_bits(integer) > 8 * n - 1 && !(negative && big_bits(integer) == 8 * n))) {
                std::fill(want.begin(), want.end(), std::uint8_t{0});
                want[n - 1] = 0x80;
            } else if (negative) {
                unsigned carry = 1;
                for (std::uint8_t& b : want) {
                    const unsigned s = static_cast<std::uint8_t>(~b) + carry;
                    b = static_cast<std::uint8_t>(s);
                    carry = s >> 8;
                }
            }
            note("trunc wide", fop(gh::CPUI_FLOAT_TRUNC, f80_of(x), {}, n), want);
        }
    }
    // an integer of 9 bytes to a half-precision float (what AArch64's UCVTF takes): the half's rounding to nearest even and its overflow to infinity
    {
        const auto half = [&](std::uint64_t v) { return fop(gh::CPUI_FLOAT_INT2FLOAT, Bytes{static_cast<std::uint8_t>(v), static_cast<std::uint8_t>(v >> 8), static_cast<std::uint8_t>(v >> 16), 0, 0, 0, 0, 0, 0}, {}, 2); };
        note("half of 0", half(0), {0x00, 0x00});
        note("half of 1", half(1), {0x00, 0x3C});
        note("half of 65504", half(65504), {0xFF, 0x7B});
        note("half of 65519 stays finite", half(65519), {0xFF, 0x7B});
        note("half of 65520 ties to infinity", half(65520), {0x00, 0x7C});
        note("half of 2049 ties to even", half(2049), {0x00, 0x68});
        note("half of 2051 ties to even", half(2051), {0x02, 0x68});
        note("half of 2050", half(2050), {0x01, 0x68});
    }
    // round-to-even decisions at the 64-bit boundary, written out: 1 + 2^-64 is a tie, 1 + 2^-63 + 2^-64 a tie to the even one above, a hair over a tie goes up
    const Bytes one = f80(false, 16383, std::uint64_t{1} << 63);
    const Bytes two_m64 = f80(false, 16383 - 64, std::uint64_t{1} << 63);
    note("tie to even (down)", fop(gh::CPUI_FLOAT_ADD, one, two_m64), one);
    note("tie to even (up)", fop(gh::CPUI_FLOAT_ADD, f80(false, 16383, (std::uint64_t{1} << 63) | 1), two_m64), f80(false, 16383, (std::uint64_t{1} << 63) | 2));
    note("over a tie", fop(gh::CPUI_FLOAT_ADD, one, f80(false, 16383 - 64, (std::uint64_t{1} << 63) | 1)), f80(false, 16383, (std::uint64_t{1} << 63) | 1));
    note("under a tie", fop(gh::CPUI_FLOAT_ADD, one, f80(false, 16383 - 65, 0xFFFFFFFFFFFFFFFFull)), one);
    note("tiny subtract", fop(gh::CPUI_FLOAT_SUB, one, f80(false, 16383 - 200, std::uint64_t{1} << 63)), one);
    note("cancel", fop(gh::CPUI_FLOAT_SUB, one, one), f80(false, 0, 0));
    note("cancel to the next", fop(gh::CPUI_FLOAT_SUB, f80(false, 16383, (std::uint64_t{1} << 63) | 1), one), f80(false, 16383 - 63, std::uint64_t{1} << 63));
    // the limits: overflow to infinity, underflow through the denormals to zero, the smallest denormal exactly
    const Bytes big_one = f80(false, 0x7FFE, std::uint64_t{1} << 63);
    note("overflow", fop(gh::CPUI_FLOAT_MULT, big_one, f80(false, 16383 + 1, std::uint64_t{1} << 63)), f80(false, 0x7FFF, std::uint64_t{1} << 63));
    note("overflow negative", fop(gh::CPUI_FLOAT_ADD, f80(true, 0x7FFE, ~std::uint64_t{0}), f80(true, 0x7FFE, ~std::uint64_t{0})), f80(true, 0x7FFF, std::uint64_t{1} << 63));
    const Bytes tiny = f80(false, 1, std::uint64_t{1} << 63);   // the smallest normal, 2^-16382
    note("to denormal", fop(gh::CPUI_FLOAT_MULT, tiny, f80(false, 16383 - 1, std::uint64_t{1} << 63)), f80(false, 0, std::uint64_t{1} << 62));
    note("smallest denormal", fop(gh::CPUI_FLOAT_MULT, tiny, f80(false, 16383 - 63, std::uint64_t{1} << 63)), f80(false, 0, 1));
    note("half the smallest denormal ties to zero", fop(gh::CPUI_FLOAT_MULT, f80(false, 0, 1), f80(false, 16383 - 1, std::uint64_t{1} << 63)), f80(false, 0, 0));
    note("three halves of it ties up to two", fop(gh::CPUI_FLOAT_MULT, f80(false, 0, 3), f80(false, 16383 - 1, std::uint64_t{1} << 63)), f80(false, 0, 2));
    note("denormal operands", fop(gh::CPUI_FLOAT_ADD, f80(false, 0, 1), f80(false, 0, 2)), f80(false, 0, 3));
    note("denormal carries into normal", fop(gh::CPUI_FLOAT_ADD, f80(false, 0, 0x7FFFFFFFFFFFFFFFull), f80(false, 0, 1)), f80(false, 1, std::uint64_t{1} << 63));
    note("denormal to double is zero", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80(false, 0, 1), {}, 8), Bytes(8, 0));
    // a double's denormal widens exactly (2^-1074 is 2^-1074 in the extended format)
    {
        const double d = std::ldexp(1.0, -1074);
        Bytes db(8);
        std::memcpy(db.data(), &d, 8);
        note("double denormal widens", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, db, {}, 10), f80(false, static_cast<unsigned>(16383 - 1074), std::uint64_t{1} << 63));
        note("and narrows back", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80(false, static_cast<unsigned>(16383 - 1074), std::uint64_t{1} << 63), {}, 8), db);
        note("half of it ties to zero", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80(false, static_cast<unsigned>(16383 - 1075), std::uint64_t{1} << 63), {}, 8), Bytes(8, 0));
        note("overflow to a double", fop(gh::CPUI_FLOAT_FLOAT2FLOAT, f80(false, 16383 + 1024, std::uint64_t{1} << 63), {}, 8), Bytes{0, 0, 0, 0, 0, 0, 0xF0, 0x7F});
    }
    // the specials: every pair of zero, one, infinity and NaN of either sign, against the rules of the arithmetic
    {
        const Bytes quiet = f80(false, 0x7FFF, (std::uint64_t{1} << 63) | (std::uint64_t{1} << 62) | 5);
        const Bytes signalling = f80(false, 0x7FFF, (std::uint64_t{1} << 63) | 7);
        std::vector<Bytes> specials = {f80(false, 0, 0), f80(true, 0, 0), one, f80(true, 16383, std::uint64_t{1} << 63), f80(false, 0x7FFF, std::uint64_t{1} << 63), f80(true, 0x7FFF, std::uint64_t{1} << 63), quiet, signalling};
        for (const unsigned op : {static_cast<unsigned>(gh::CPUI_FLOAT_ADD), static_cast<unsigned>(gh::CPUI_FLOAT_SUB), static_cast<unsigned>(gh::CPUI_FLOAT_MULT), static_cast<unsigned>(gh::CPUI_FLOAT_DIV)})
            for (const Bytes& x : specials)
                for (const Bytes& y : specials) {
                    const Bytes r = fop(op, x, y);
                    ++checked;
                    const bool nan_in = f80_is_nan(x) || f80_is_nan(y);
                    const bool inf_x = f80_is_inf(x), inf_y = f80_is_inf(y), zero_x = f80_is_zero(x), zero_y = f80_is_zero(y);
                    const bool sx = f80_sign(x), sy = f80_sign(op == gh::CPUI_FLOAT_SUB ? y : y) != (op == gh::CPUI_FLOAT_SUB);
                    bool bad = false;
                    if (nan_in) {
                        bad = !f80_is_nan(r) || ((f80_mant(r) >> 62) & 1) == 0;   // (quiet)
                        if (f80_is_nan(x) && !f80_is_nan(y)) bad = bad || (f80_mant(r) & ~(std::uint64_t{1} << 62)) != (f80_mant(x) & ~(std::uint64_t{1} << 62));
                    } else if (op == gh::CPUI_FLOAT_ADD || op == gh::CPUI_FLOAT_SUB) {
                        if (inf_x && inf_y && sx != sy) bad = !(f80_is_nan(r) && f80_sign(r));   // (the indefinite NaN)
                        else if (inf_x || inf_y) bad = !f80_is_inf(r) || f80_sign(r) != (inf_x ? sx : sy);
                        else if (zero_x && zero_y) bad = !f80_is_zero(r) || f80_sign(r) != (sx && sy);
                    } else if (op == gh::CPUI_FLOAT_MULT) {
                        if ((inf_x && zero_y) || (zero_x && inf_y)) bad = !(f80_is_nan(r) && f80_sign(r));
                        else if (inf_x || inf_y) bad = !f80_is_inf(r) || f80_sign(r) != (sx != f80_sign(y));
                        else if (zero_x || zero_y) bad = !f80_is_zero(r) || f80_sign(r) != (sx != f80_sign(y));
                    } else {
                        if ((zero_x && zero_y) || (inf_x && inf_y)) bad = !(f80_is_nan(r) && f80_sign(r));
                        else if (inf_x || zero_y) bad = !f80_is_inf(r) || f80_sign(r) != (sx != f80_sign(y));
                        else if (zero_x || inf_y) bad = !f80_is_zero(r) || f80_sign(r) != (sx != f80_sign(y));
                    }
                    if (bad && ++different <= 10) std::printf("  x87 special op %u: %s with %s gave %s\n", op, hex(x).c_str(), hex(y).c_str(), hex(r).c_str());
                }
        note("sqrt of a negative", fop(gh::CPUI_FLOAT_SQRT, f80(true, 16383, std::uint64_t{1} << 63)), f80(true, 0x7FFF, (std::uint64_t{3} << 62)));
        note("sqrt of minus zero", fop(gh::CPUI_FLOAT_SQRT, f80(true, 0, 0)), f80(true, 0, 0));
        note("nan test", fop(gh::CPUI_FLOAT_NAN, quiet, {}, 1), {1});
        note("not a nan", fop(gh::CPUI_FLOAT_NAN, one, {}, 1), {0});
        note("nan is unordered", fop(gh::CPUI_FLOAT_LESS, quiet, one, 1), {0});
        note("nan is not equal", fop(gh::CPUI_FLOAT_NOTEQUAL, quiet, quiet, 1), {1});
        note("zeros are equal", fop(gh::CPUI_FLOAT_EQUAL, f80(false, 0, 0), f80(true, 0, 0), 1), {1});
        note("truncating a nan", fop(gh::CPUI_FLOAT_TRUNC, quiet, {}, 4), from_u64(0x80000000u, 4));
        note("truncating too much", fop(gh::CPUI_FLOAT_TRUNC, f80(false, 16383 + 40, std::uint64_t{1} << 63), {}, 4), from_u64(0x80000000u, 4));
        note("truncating the most negative", fop(gh::CPUI_FLOAT_TRUNC, f80(true, 16383 + 31, std::uint64_t{1} << 63), {}, 4), from_u64(0x80000000u, 4));
        note("two nans", fop(gh::CPUI_FLOAT_ADD, f80(false, 0x7FFF, (std::uint64_t{1} << 63) | (std::uint64_t{1} << 62) | 3), quiet), quiet);
    }
    std::printf("x87 extended float: %u evaluations, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

#if LDBL_MANT_DIG == 64
// Where the host's long double is the x87's own (GCC and clang on x86; MSVC's is a double), its arithmetic is the oracle: every operation on random finite values, those
// of far-apart and of nearly equal exponents and the denormals, bit for bit.
CB_TEST("engine.pcode_cpu.x87_extended_float_operations_match_the_hosts_long_double", [] {
    std::mt19937_64 rng(20261201);
    unsigned checked = 0, different = 0;
    const auto to_bytes = [](long double x) {
        Bytes b(10, 0);
        std::memcpy(b.data(), &x, 10);
        return b;
    };
    const auto to_long_double = [](const Bytes& b) {
        long double x = 0;
        std::memcpy(&x, b.data(), 10);
        return x;
    };
    for (int k = 0; k < 300000; ++k) {
        long double x, y;
        if (k % 4 == 3) {   // denormals, and values near the bottom of the range
            const auto denormal = [&] {
                Bytes b(10, 0);
                const std::uint64_t m = (rng() >> (rng() % 40)) & 0x7FFFFFFFFFFFFFFFull;
                for (int i = 0; i < 8; ++i) b[static_cast<std::size_t>(i)] = static_cast<std::uint8_t>(m >> (8 * i));
                b[8] = static_cast<std::uint8_t>(rng() % 3);
                if (b[8]) b[7] |= 0x80;
                b[9] = static_cast<std::uint8_t>(rng() & 0x80);
                return b;
            };
            x = to_long_double(denormal());
            y = to_long_double(rng() % 2 ? denormal() : f80(rng() & 1, static_cast<unsigned>(16383 - 1 - rng() % 70), (std::uint64_t{1} << 63) | (rng() >> 1)));
        } else {
            const auto make = [&] {
                const long e = (rng() % 7 == 0) ? static_cast<long>(rng() % 32000) - 16000 : static_cast<long>(rng() % 400) - 200;
                return std::ldexp(static_cast<long double>((rng() | (std::uint64_t{1} << 63))), static_cast<int>(e) - 63) * ((rng() & 1) ? -1.0L : 1.0L);
            };
            x = make();
            y = k % 5 == 0 ? x * (1.0L + std::ldexp(1.0L, -static_cast<int>(rng() % 70))) : make();
        }
        const Bytes X = to_bytes(x), Y = to_bytes(y);
        const auto note = [&](const char* what, const Bytes& got, long double want) {
            ++checked;
            if (got != to_bytes(want) && ++different <= 10) std::printf("  long double %s: x=%s y=%s got %s want %s\n", what, hex(X).c_str(), hex(Y).c_str(), hex(got).c_str(), hex(to_bytes(want)).c_str());
        };
        note("add", fop(gh::CPUI_FLOAT_ADD, X, Y), x + y);
        note("sub", fop(gh::CPUI_FLOAT_SUB, X, Y), x - y);
        note("mult", fop(gh::CPUI_FLOAT_MULT, X, Y), x * y);
        if (y != 0) note("div", fop(gh::CPUI_FLOAT_DIV, X, Y), x / y);
        if (k % 4 != 3) {
            const long double p = std::fabs(x);
            note("sqrt", fop(gh::CPUI_FLOAT_SQRT, to_bytes(p)), std::sqrt(p));
            ++checked;
            const double d = static_cast<double>(x);
            if (fop(gh::CPUI_FLOAT_FLOAT2FLOAT, X, {}, 8) != Bytes(reinterpret_cast<const std::uint8_t*>(&d), reinterpret_cast<const std::uint8_t*>(&d) + 8) && ++different <= 10) std::printf("  long double to double\n");
            ++checked;
            const float f = static_cast<float>(x);
            if (fop(gh::CPUI_FLOAT_FLOAT2FLOAT, X, {}, 4) != Bytes(reinterpret_cast<const std::uint8_t*>(&f), reinterpret_cast<const std::uint8_t*>(&f) + 4) && ++different <= 10) std::printf("  long double to float\n");
        }
    }
    std::printf("x87 against the host's long double: %u evaluations, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});
#endif

namespace {

std::filesystem::path languages_dir() {
    if (const std::optional<std::string> env = fs::get_env("CB_LANGUAGES_DIR"); env && !env->empty()) return fs::from_text(*env);
    return config::current().checkout_root() / "src" / "decompile" / "engine" / "languages";
}

std::unique_ptr<Emu> x86_64() {
    return std::make_unique<Emu>(languages_dir() / "x86-64.sla", std::vector<std::pair<std::string, int>>{{"longMode", 1}, {"addrsize", 2}, {"bit64", 1}, {"opsize", 1}});
}

// One instruction from `code` at 0x1000 (the registers and memory are the caller's).
Emu::Status run_one(Emu& emu, const Bytes& code) {
    emu.write(0x1000, code.data(), code.size());
    emu.setPc(0x1000);
    return emu.step();
}

Bytes reg_wide(const Emu& emu, const char* name) {
    const Emu::RegRef ref = emu.regRef(name);
    Bytes v(ref.size);
    emu.readRegWide(ref, v.data());
    return v;
}
void set_wide(Emu& emu, const char* name, const Bytes& v) {
    const Emu::RegRef ref = emu.regRef(name);
    CB_CHECK_EQ(ref.size, static_cast<unsigned>(v.size()));
    emu.writeRegWide(ref, v.data());
}
Bytes first_bytes(const Bytes& v, std::size_t n) { return Bytes(v.begin(), v.begin() + static_cast<std::ptrdiff_t>(n)); }

}  // namespace

// Real instructions of x86-64.sla whose p-code works on 16-byte values: the widening multiply and the divide of a 128-bit dividend, SSE's moves and logic and a 128-bit shift.
CB_TEST("engine.pcode_cpu.wide_instructions_of_x86_64_run_on_the_emulator", [] {
    std::mt19937_64 rng(20261127);
    const std::unique_ptr<Emu> emu_ptr = x86_64();
    Emu& emu = *emu_ptr;
    unsigned checked = 0, different = 0;
    const auto note = [&](const char* what, const Bytes& got, const Bytes& want) {
        ++checked;
        if (got != want && ++different <= 10) std::printf("  x86-64 %s: got %s, want %s\n", what, hex(got).c_str(), hex(want).c_str());
    };
    for (int k = 0; k < 400; ++k) {
        const std::uint64_t a = rng() >> (rng() % 64), c = (rng() >> (rng() % 64)) | 1, d = rng() >> (rng() % 64);
        // MUL rcx: rdx:rax = rax * rcx
        emu.setReg("RAX", a);
        emu.setReg("RCX", c);
        emu.setReg("RDX", d);
        CB_CHECK(run_one(emu, {0x48, 0xF7, 0xE1}) == Emu::Status::Ok);
        const Big product = big_mul(big_of(a), big_of(c));
        Bytes want = from_u64(0, 16);
        for (std::size_t i = 0; i < 16; ++i)
            if (i / 4 < product.size()) want[i] = static_cast<std::uint8_t>(product[i / 4] >> (8 * (i % 4)));
        Bytes got = from_u64(emu.reg("RAX"), 8);
        const Bytes high = from_u64(emu.reg("RDX"), 8);
        got.insert(got.end(), high.begin(), high.end());
        note("mul", got, want);
        note("mul carry", {static_cast<std::uint8_t>(emu.reg("CF"))}, {static_cast<std::uint8_t>(emu.reg("RDX") != 0)});
        // DIV rcx: rax = rdx:rax / rcx, rdx = the remainder (rdx below rcx, or it faults)
        const std::uint64_t hi = d % c, lo = rng();
        emu.setReg("RAX", lo);
        emu.setReg("RCX", c);
        emu.setReg("RDX", hi);
        CB_CHECK(run_one(emu, {0x48, 0xF7, 0xF1}) == Emu::Status::Ok);
        Big dividend = big_add(big_shl(big_of(hi), 64), big_of(lo)), q, r;
        big_divmod(dividend, big_of(c), q, r);
        note("div quotient", from_u64(emu.reg("RAX"), 8), from_u64(q.size() ? q[0] | (q.size() > 1 ? std::uint64_t{q[1]} << 32 : 0) : 0, 8));
        note("div remainder", from_u64(emu.reg("RDX"), 8), from_u64(r.size() ? r[0] | (r.size() > 1 ? std::uint64_t{r[1]} << 32 : 0) : 0, 8));
        // IMUL rcx (one operand, signed): the 128-bit signed product, from the unsigned one by the signs
        const std::uint64_t sa = rng() >> (rng() % 64), sc = rng() >> (rng() % 64);
        emu.setReg("RAX", sa);
        emu.setReg("RCX", sc);
        CB_CHECK(run_one(emu, {0x48, 0xF7, 0xE9}) == Emu::Status::Ok);
        const bool na = (sa >> 63) != 0, nc = (sc >> 63) != 0;
        const Big magnitude = big_mul(big_of(na ? ~sa + 1 : sa), big_of(nc ? ~sc + 1 : sc));
        Bytes signed_want = from_u64(0, 16);
        for (std::size_t i = 0; i < 16; ++i)
            if (i / 4 < magnitude.size()) signed_want[i] = static_cast<std::uint8_t>(magnitude[i / 4] >> (8 * (i % 4)));
        if (na != nc) {   // (negate in 128 bits)
            unsigned carry = 1;
            for (std::uint8_t& b : signed_want) {
                const unsigned s = static_cast<std::uint8_t>(~b) + carry;
                b = static_cast<std::uint8_t>(s);
                carry = s >> 8;
            }
        }
        Bytes signed_got = from_u64(emu.reg("RAX"), 8);
        const Bytes signed_high = from_u64(emu.reg("RDX"), 8);
        signed_got.insert(signed_got.end(), signed_high.begin(), signed_high.end());
        note("imul", signed_got, signed_want);
    }
    // SSE: moves, logic, a 64-bit lane add and a whole-register shift on random registers and memory
    for (int k = 0; k < 200; ++k) {
        const Bytes x = random_value(rng, 16), y = random_value(rng, 16);
        set_wide(emu, "XMM0", x);
        set_wide(emu, "XMM1", y);
        const auto xor_want = [&] { Bytes r(16); for (int i = 0; i < 16; ++i) r[static_cast<std::size_t>(i)] = x[static_cast<std::size_t>(i)] ^ y[static_cast<std::size_t>(i)]; return r; }();
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0xEF, 0xC1}) == Emu::Status::Ok);   // pxor xmm0, xmm1
        note("pxor", reg_wide(emu, "XMM0"), xor_want);
        set_wide(emu, "XMM0", x);
        const auto or_want = [&] { Bytes r(16); for (int i = 0; i < 16; ++i) r[static_cast<std::size_t>(i)] = x[static_cast<std::size_t>(i)] | y[static_cast<std::size_t>(i)]; return r; }();
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0xEB, 0xC1}) == Emu::Status::Ok);   // por xmm0, xmm1
        note("por", reg_wide(emu, "XMM0"), or_want);
        set_wide(emu, "XMM0", x);
        Bytes add_want(16);
        for (int lane = 0; lane < 2; ++lane) {
            const std::uint64_t s = as_u64(Bytes(x.begin() + 8 * lane, x.begin() + 8 * lane + 8)) + as_u64(Bytes(y.begin() + 8 * lane, y.begin() + 8 * lane + 8));
            for (int i = 0; i < 8; ++i) add_want[static_cast<std::size_t>(8 * lane + i)] = static_cast<std::uint8_t>(s >> (8 * i));
        }
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0xD4, 0xC1}) == Emu::Status::Ok);   // paddq xmm0, xmm1
        note("paddq", reg_wide(emu, "XMM0"), add_want);
        // movdqa xmm0, [rcx] and movdqa [rcx], xmm0 through memory
        emu.write(0x4000, y.data(), 16);
        emu.setReg("RCX", 0x4000);
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x6F, 0x01}) == Emu::Status::Ok);
        note("movdqa load", reg_wide(emu, "XMM0"), y);
        emu.setReg("RCX", 0x5000);
        set_wide(emu, "XMM0", x);
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x7F, 0x01}) == Emu::Status::Ok);
        Bytes stored(16);
        emu.read(0x5000, stored.data(), 16);
        note("movdqa store", stored, x);
        // pslldq xmm0, n: the register shifted left by n bytes
        const std::uint8_t n = static_cast<std::uint8_t>(rng() % 18);
        set_wide(emu, "XMM0", x);
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x73, 0xF8, n}) == Emu::Status::Ok);
        Bytes shifted(16, 0);
        for (std::size_t i = n; i < 16; ++i) shifted[i] = x[i - n];
        note("pslldq", reg_wide(emu, "XMM0"), shifted);
    }
    std::printf("x86-64 wide instructions: %u checks, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

// x87 instructions of the description: loads and stores of 10-byte values (a wide LOAD and STORE), arithmetic on the extended registers.
CB_TEST("engine.pcode_cpu.x87_instructions_of_x86_64_run_on_the_emulator", [] {
    std::mt19937_64 rng(20261128);
    const std::unique_ptr<Emu> emu_ptr = x86_64();
    Emu& emu = *emu_ptr;
    unsigned checked = 0, different = 0;
    const auto note = [&](const char* what, const Bytes& got, const Bytes& want) {
        ++checked;
        if (got != want && ++different <= 10) std::printf("  x87 %s: got %s, want %s\n", what, hex(got).c_str(), hex(want).c_str());
    };
    emu.setReg("RCX", 0x4000);
    emu.setReg("RDX", 0x5000);
    for (int k = 0; k < 200; ++k) {
        const auto small = [&] { return std::ldexp(static_cast<double>((rng() & 0xFFFFFF) | 1), static_cast<int>(rng() % 16) - 12) * ((rng() & 1) ? -1.0 : 1.0); };
        const double x = small(), y = small();
        const Bytes fx = f80_of(x), fy = f80_of(y);
        emu.write(0x4000, fx.data(), 10);
        emu.write(0x4010, fy.data(), 10);
        const auto two = [&](const Bytes& op, double want) {   // fld [rcx]; fld [rcx+16]; <op>; fstp [rdx]
            CB_CHECK(run_one(emu, {0xDB, 0x29}) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, {0xDB, 0x69, 0x10}) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, op) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, {0xDB, 0x3A}) == Emu::Status::Ok);
            Bytes stored(10);
            emu.read(0x5000, stored.data(), 10);
            return std::make_pair(stored, f80_of(want));
        };
        {
            auto [got, want] = two({0xDE, 0xC1}, x + y);   // faddp st1, st0: st1 = x + y
            note("faddp", got, want);
        }
        {
            auto [got, want] = two({0xDE, 0xE9}, x - y);   // fsubp st1, st0: st1 = st1 - st0
            note("fsubp", got, want);
        }
        {
            auto [got, want] = two({0xDE, 0xC9}, x * y);   // fmulp
            note("fmulp", got, want);
        }
        {
            const double q = x * y;   // x * y / y == x when the product is exact (24-bit mantissas)
            emu.write(0x4000, f80_of(q).data(), 10);
            auto [got, want] = two({0xDE, 0xF9}, q / y);   // fdivp st1, st0: st1 = st1 / st0
            note("fdivp", got, want);
        }
        {
            emu.write(0x4000, f80_of(std::fabs(x * x)).data(), 10);
            CB_CHECK(run_one(emu, {0xDB, 0x29}) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, {0xD9, 0xFA}) == Emu::Status::Ok);   // fsqrt
            CB_CHECK(run_one(emu, {0xDB, 0x3A}) == Emu::Status::Ok);
            Bytes stored(10);
            emu.read(0x5000, stored.data(), 10);
            note("fsqrt", stored, f80_of(std::fabs(x)));
        }
        {   // fld qword [rcx] widens a double, fstp qword [rdx] narrows it
            Bytes d(8);
            std::memcpy(d.data(), &x, 8);
            emu.write(0x4000, d.data(), 8);
            CB_CHECK(run_one(emu, {0xDD, 0x01}) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, {0xDD, 0x1A}) == Emu::Status::Ok);
            Bytes stored(8);
            emu.read(0x5000, stored.data(), 8);
            note("fld/fstp qword", stored, d);
        }
        {   // fild qword [rcx]: an integer becomes the float, fistp qword [rdx] turns it back
            const std::int64_t n = static_cast<std::int64_t>(rng()) >> (rng() % 63);
            Bytes nb(8);
            std::memcpy(nb.data(), &n, 8);
            emu.write(0x4000, nb.data(), 8);
            CB_CHECK(run_one(emu, {0xDF, 0x29}) == Emu::Status::Ok);
            CB_CHECK(run_one(emu, {0xDF, 0x3A}) == Emu::Status::Ok);
            Bytes stored(8);
            emu.read(0x5000, stored.data(), 8);
            note("fild/fistp qword", stored, nb);
        }
    }
    std::printf("x87 instructions: %u checks, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

// Memory hooks see a wide access as accesses of at most 8 bytes in address order, each valued in the space's byte order.
CB_TEST("engine.pcode_cpu.a_wide_access_reaches_the_memory_hooks_in_pieces_of_8_bytes_in_address_order", [] {
    struct Access {
        std::string kind;
        std::uint64_t address;
        unsigned size;
        std::uint64_t value;
    };
    // little endian: movdqa xmm0, [rcx] (16 bytes), movdqa [rcx], xmm0, fld tbyte [rcx] (10 bytes), fstp tbyte [rcx]
    {
        const std::unique_ptr<Emu> emu_ptr = x86_64();
        Emu& emu = *emu_ptr;
        std::vector<Access> log;
        Bytes memory(32);
        for (std::size_t i = 0; i < memory.size(); ++i) memory[i] = static_cast<std::uint8_t>(0x10 + i);
        emu.onRead([&](std::string_view space, std::uint64_t address, unsigned size, std::uint64_t& value) {
            if (space != "ram" || address < 0x4000) return false;   // (the instruction's own bytes come from the page map)
            value = 0;
            for (unsigned i = 0; i < size; ++i) value |= std::uint64_t{memory[address - 0x4000 + i]} << (8 * i);
            log.push_back({"read", address, size, value});
            return true;
        });
        emu.onWrite([&](std::string_view space, std::uint64_t address, unsigned size, std::uint64_t value) {
            if (space != "ram" || address < 0x4000) return false;
            for (unsigned i = 0; i < size; ++i) memory[address - 0x4000 + i] = static_cast<std::uint8_t>(value >> (8 * i));
            log.push_back({"write", address, size, value});
            return true;
        });
        emu.setReg("RCX", 0x4000);
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x6F, 0x01}) == Emu::Status::Ok);   // movdqa xmm0, [rcx]
        CB_CHECK_EQ(log.size(), std::size_t{2});
        if (log.size() == 2) {
            CB_CHECK_EQ(log[0].address, std::uint64_t{0x4000});
            CB_CHECK_EQ(log[0].size, 8u);
            CB_CHECK_EQ(log[0].value, std::uint64_t{0x1716151413121110ull});
            CB_CHECK_EQ(log[1].address, std::uint64_t{0x4008});
            CB_CHECK_EQ(log[1].size, 8u);
            CB_CHECK_EQ(log[1].value, std::uint64_t{0x1F1E1D1C1B1A1918ull});
        }
        CB_CHECK_EQ(hex(reg_wide(emu, "XMM0")), std::string("1f1e1d1c1b1a19181716151413121110"));
        log.clear();
        emu.setReg("RCX", 0x4010);
        set_wide(emu, "XMM0", from_u64(0x0807060504030201ull, 8).size() ? Bytes{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16} : Bytes());
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x7F, 0x01}) == Emu::Status::Ok);   // movdqa [rcx], xmm0
        CB_CHECK_EQ(log.size(), std::size_t{2});
        if (log.size() == 2) {
            CB_CHECK_EQ(log[0].address, std::uint64_t{0x4010});
            CB_CHECK_EQ(log[0].value, std::uint64_t{0x0807060504030201ull});
            CB_CHECK_EQ(log[1].address, std::uint64_t{0x4018});
            CB_CHECK_EQ(log[1].value, std::uint64_t{0x100F0E0D0C0B0A09ull});
        }
        CB_CHECK_EQ(memory[0x10], std::uint8_t{1});
        CB_CHECK_EQ(memory[0x1F], std::uint8_t{16});
        // ten bytes: 8, then 2
        log.clear();
        emu.setReg("RCX", 0x4000);
        CB_CHECK(run_one(emu, {0xDB, 0x29}) == Emu::Status::Ok);   // fld tbyte [rcx]
        CB_CHECK_EQ(log.size(), std::size_t{2});
        if (log.size() == 2) {
            CB_CHECK_EQ(log[0].address, std::uint64_t{0x4000});
            CB_CHECK_EQ(log[0].size, 8u);
            CB_CHECK_EQ(log[1].address, std::uint64_t{0x4008});
            CB_CHECK_EQ(log[1].size, 2u);
            CB_CHECK_EQ(log[1].value, std::uint64_t{0x1918});
        }
        // a hook that declines leaves the piece to the page map
        emu.onRead([&](std::string_view, std::uint64_t address, unsigned, std::uint64_t&) { return address == 12345; });
        const Bytes ones(16, 0x5A);
        emu.write(0x4000, ones.data(), 16);
        CB_CHECK(run_one(emu, {0x66, 0x0F, 0x6F, 0x01}) == Emu::Status::Ok);
        CB_CHECK_EQ(hex(reg_wide(emu, "XMM0")), std::string("5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a5a"));
    }
    // big endian: lvx vr0, 0, r3 on the PowerPC description: the first piece is the high half of the register
    {
        sl::Emulator& emu = engine::pcode_cpu::emulator_named("ppc_64_isa_altivec_be.sla");
        std::vector<Access> log;
        emu.clearMemory();
        emu.onRead([&](std::string_view space, std::uint64_t address, unsigned size, std::uint64_t& value) {
            if (space != "ram" || address < 0x4000) return false;
            value = 0;
            for (unsigned i = 0; i < size; ++i) value = (value << 8) | (0x20 + (address - 0x4000 + i));   // (big endian: the first byte is the most significant)
            log.push_back({"read", address, size, value});
            return true;
        });
        emu.onWrite({});
        emu.setReg("r3", 0x4000);
        const std::uint32_t lvx = 0x7C0018CE | (0u << 21) | (0u << 16) | (3u << 11);   // lvx vr0, 0, r3
        const Bytes code = {static_cast<std::uint8_t>(lvx >> 24), static_cast<std::uint8_t>(lvx >> 16), static_cast<std::uint8_t>(lvx >> 8), static_cast<std::uint8_t>(lvx)};
        CB_CHECK(run_one(emu, code) == Emu::Status::Ok);
        CB_CHECK_EQ(log.size(), std::size_t{2});
        if (log.size() == 2) {
            CB_CHECK_EQ(log[0].address, std::uint64_t{0x4000});
            CB_CHECK_EQ(log[0].size, 8u);
            CB_CHECK_EQ(log[0].value, std::uint64_t{0x2021222324252627ull});
            CB_CHECK_EQ(log[1].address, std::uint64_t{0x4008});
            CB_CHECK_EQ(log[1].value, std::uint64_t{0x28292A2B2C2D2E2Full});
        }
        // the register as a value, least significant byte first: the last byte of memory comes first
        CB_CHECK_EQ(hex(reg_wide(emu, "vs32")), std::string("2021222324252627" "28292a2b2c2d2e2f"));
        const Bytes raw = [&] { Bytes b(16); const Emu::RegRef r = emu.regRef("vs32"); emu.readRegs(r, b.data(), 16); return b; }();
        CB_CHECK_EQ(raw[0], std::uint8_t{0x20});   // (readRegs: the register space is big endian too, so the bytes are in memory order)
        CB_CHECK_EQ(raw[15], std::uint8_t{0x2F});
        emu.onRead({});
    }
});

// PowerPC's description: AltiVec logic and the 128-bit products of the 64-bit multiplies, on random values.
CB_TEST("engine.pcode_cpu.wide_instructions_of_the_powerpc_description_run_on_the_emulator", [] {
    sl::Emulator& emu = engine::pcode_cpu::emulator_named("ppc_64_isa_altivec_be.sla");
    emu.onRead({});
    emu.onWrite({});
    std::mt19937_64 rng(20261129);
    unsigned checked = 0, different = 0;
    const auto code_of = [](std::uint32_t w) { return Bytes{static_cast<std::uint8_t>(w >> 24), static_cast<std::uint8_t>(w >> 16), static_cast<std::uint8_t>(w >> 8), static_cast<std::uint8_t>(w)}; };
    const auto note = [&](const char* what, const Bytes& got, const Bytes& want) {
        ++checked;
        if (got != want && ++different <= 10) std::printf("  ppc %s: got %s, want %s\n", what, hex(got).c_str(), hex(want).c_str());
    };
    for (int k = 0; k < 300; ++k) {
        const Bytes x = random_value(rng, 16), y = random_value(rng, 16);
        set_wide(emu, "vs33", x);
        set_wide(emu, "vs34", y);
        Bytes want(16);
        for (std::size_t i = 0; i < 16; ++i) want[i] = x[i] ^ y[i];
        CB_CHECK(run_one(emu, code_of(0x10000000u | (0u << 21) | (1u << 16) | (2u << 11) | 1220)) == Emu::Status::Ok);   // vxor vr0, vr1, vr2
        note("vxor", reg_wide(emu, "vs32"), want);
        for (std::size_t i = 0; i < 16; ++i) want[i] = x[i] | y[i];
        CB_CHECK(run_one(emu, code_of(0x10000000u | (0u << 21) | (1u << 16) | (2u << 11) | 1156)) == Emu::Status::Ok);   // vor
        note("vor", reg_wide(emu, "vs32"), want);
        // mulhdu r3, r4, r5: the high 64 bits of the unsigned 128-bit product; mulld the low
        const std::uint64_t a = rng() >> (rng() % 64), c = rng() >> (rng() % 64);
        emu.setReg("r4", a);
        emu.setReg("r5", c);
        CB_CHECK(run_one(emu, code_of(0x7C642812u)) == Emu::Status::Ok);
        const Big product = big_mul(big_of(a), big_of(c));
        const auto limb64 = [&](std::size_t i) { return (i * 2 < product.size() ? std::uint64_t{product[i * 2]} : 0) | (i * 2 + 1 < product.size() ? std::uint64_t{product[i * 2 + 1]} << 32 : 0); };
        note("mulhdu", from_u64(emu.reg("r3"), 8), from_u64(limb64(1), 8));
        // the signed and the multiply-add forms (16-byte temporaries; the reference is the bit-serial one on each step the description writes)
        const auto ref = [](unsigned op, std::size_t rn, const Bytes& x, const Bytes& y = {}) {
            Bytes r;
            CB_CHECK(reference(op, rn, x, x.size(), y, y.size(), r));
            return r;
        };
        const std::uint64_t third = rng() >> (rng() % 64);
        emu.setReg("r6", third);
        const Bytes ba = from_u64(a, 8), bc = from_u64(c, 8), bt = from_u64(third, 8);
        const Bytes sa = ref(gh::CPUI_INT_SEXT, 16, ba), sc = ref(gh::CPUI_INT_SEXT, 16, bc), st = ref(gh::CPUI_INT_SEXT, 16, bt);
        const Bytes za = ref(gh::CPUI_INT_ZEXT, 16, ba), zc = ref(gh::CPUI_INT_ZEXT, 16, bc), zt = ref(gh::CPUI_INT_ZEXT, 16, bt);
        const auto high = [&](const Bytes& v) { return ref(gh::CPUI_SUBPIECE, 8, v, Bytes{8}); };
        CB_CHECK(run_one(emu, code_of(0x7C000000u | (3u << 21) | (4u << 16) | (5u << 11) | (73u << 1))) == Emu::Status::Ok);   // mulhd r3, r4, r5
        note("mulhd", from_u64(emu.reg("r3"), 8), high(ref(gh::CPUI_INT_MULT, 16, sa, sc)));
        CB_CHECK(run_one(emu, code_of(0x10000000u | (3u << 21) | (4u << 16) | (5u << 11) | (6u << 6) | 48)) == Emu::Status::Ok);   // maddhd r3, r4, r5, r6
        note("maddhd", from_u64(emu.reg("r3"), 8), high(ref(gh::CPUI_INT_ADD, 16, ref(gh::CPUI_INT_MULT, 16, sa, sc), st)));
        CB_CHECK(run_one(emu, code_of(0x10000000u | (3u << 21) | (4u << 16) | (5u << 11) | (6u << 6) | 49)) == Emu::Status::Ok);   // maddhdu
        note("maddhdu", from_u64(emu.reg("r3"), 8), high(ref(gh::CPUI_INT_ADD, 16, ref(gh::CPUI_INT_MULT, 16, za, zc), zt)));
        CB_CHECK(run_one(emu, code_of(0x10000000u | (3u << 21) | (4u << 16) | (5u << 11) | (6u << 6) | 51)) == Emu::Status::Ok);   // maddld
        note("maddld", from_u64(emu.reg("r3"), 8), ref(gh::CPUI_SUBPIECE, 8, ref(gh::CPUI_INT_ADD, 16, ref(gh::CPUI_INT_MULT, 16, sa, sc), st), Bytes{0}));
        if (c != 0) {
            CB_CHECK(run_one(emu, code_of(0x7C000000u | (3u << 21) | (4u << 16) | (5u << 11) | (777u << 1))) == Emu::Status::Ok);   // modsd
            note("modsd", from_u64(emu.reg("r3"), 8), ref(gh::CPUI_SUBPIECE, 8, ref(gh::CPUI_INT_SREM, 16, sa, sc), Bytes{0}));
            CB_CHECK(run_one(emu, code_of(0x7C000000u | (3u << 21) | (4u << 16) | (5u << 11) | (265u << 1))) == Emu::Status::Ok);   // modud
            note("modud", from_u64(emu.reg("r3"), 8), ref(gh::CPUI_SUBPIECE, 8, ref(gh::CPUI_INT_REM, 16, za, zc), Bytes{0}));
        }
        // a 32-byte shift and a splat, in a big-endian register: vsldoi vD,vA,vB,SHB takes bytes SHB.. of the pair vA:vB, vspltb copies one byte (counted from the top) to all
        {
            const unsigned shb = static_cast<unsigned>(rng() % 16);
            set_wide(emu, "vs33", x);
            set_wide(emu, "vs34", y);
            CB_CHECK(run_one(emu, code_of(0x10000000u | (0u << 21) | (1u << 16) | (2u << 11) | (shb << 6) | 44)) == Emu::Status::Ok);
            Bytes pair(32);   // (value order: vB is the low half)
            std::copy(y.begin(), y.end(), pair.begin());
            std::copy(x.begin(), x.end(), pair.begin() + 16);
            Bytes shifted = ref(gh::CPUI_INT_LEFT, 32, pair, Bytes{static_cast<std::uint8_t>(8 * shb)});
            note("vsldoi", reg_wide(emu, "vs32"), Bytes(shifted.begin() + 16, shifted.end()));
            const unsigned lane = static_cast<unsigned>(rng() % 16);
            CB_CHECK(run_one(emu, code_of(0x10000000u | (0u << 21) | (lane << 16) | (2u << 11) | 524)) == Emu::Status::Ok);   // vspltb vs32, vs34, lane
            note("vspltb", reg_wide(emu, "vs32"), Bytes(16, y[15 - lane]));
        }
        // a store to memory writes the register most significant byte first (stvx vs32, 0, r3), and a load reads it back
        {
            const Bytes v = random_value(rng, 16);
            set_wide(emu, "vs32", v);
            emu.setReg("r3", 0x7000);
            CB_CHECK(run_one(emu, code_of(0x7C0001CEu | (0u << 21) | (3u << 11))) == Emu::Status::Ok);   // stvx vs32, 0, r3
            Bytes memory(16);
            emu.read(0x7000, memory.data(), 16);
            Bytes reversed(v.rbegin(), v.rend());
            note("stvx", memory, reversed);
        }
    }
    std::printf("PowerPC wide instructions: %u checks, %u differ\n", checked, different);
    CB_CHECK_EQ(different, 0u);
});

// A user operation with a wide value reaches the wide hook whole (and the narrow one as the low 8 bytes); a wide result is the hook's bytes.
CB_TEST("engine.pcode_cpu.a_user_operation_with_wide_values_reaches_the_wide_hook", [] {
    // (an emulator of its own: the thread's shared one has the PowerPC adapter's lane operations bound, which are served before any hook)
    sl::Emulator emu(config::current().checkout_root() / "src" / "decompile" / "engine" / "languages" / "ppc_64_isa_altivec_be.sla", {});
    const std::uint32_t vaddubm = 0x10000000u | (0u << 21) | (1u << 16) | (2u << 11) | 512;   // vaddubs vr0, vr1, vr2: a named operation (no p-code) in this description
    const Bytes code = {static_cast<std::uint8_t>(vaddubm >> 24), static_cast<std::uint8_t>(vaddubm >> 16), static_cast<std::uint8_t>(vaddubm >> 8), static_cast<std::uint8_t>(vaddubm)};
    const Bytes x = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16}, y(16, 1);
    set_wide(emu, "vs33", x);
    set_wide(emu, "vs34", y);
    set_wide(emu, "vs32", Bytes(16, 0));
    // without a hook: counted as unimplemented, the output zero
    CB_CHECK(run_one(emu, code) == Emu::Status::Ok);
    CB_CHECK_EQ(hex(reg_wide(emu, "vs32")), std::string(32, '0'));
    CB_CHECK(!emu.unimplementedOps().empty());
    // the narrow hook sees the low 8 bytes of each argument
    std::vector<std::uint64_t> seen;
    emu.onUserOp([&](sl::Emulator&, const std::string&, const std::vector<std::uint64_t>& args, std::uint64_t& result) {
        seen = args;
        result = 0x1122334455667788ull;
        return true;
    });
    CB_CHECK(run_one(emu, code) == Emu::Status::Ok);
    CB_CHECK_EQ(seen.size(), std::size_t{2});
    if (seen.size() == 2) {
        CB_CHECK_EQ(seen[0], std::uint64_t{0x0807060504030201ull});
        CB_CHECK_EQ(seen[1], std::uint64_t{0x0101010101010101ull});
    }
    CB_CHECK_EQ(hex(reg_wide(emu, "vs32")), std::string("00000000000000001122334455667788"));
    // the wide hook: whole values in, whole value out (here a lane-wise add of bytes, which the description leaves to the host)
    std::string seen_name;
    emu.onUserOpWide([&](sl::Emulator&, const std::string& name, const std::vector<Bytes>& args, Bytes& result) {
        seen_name = name;
        if (args.size() != 2 || args[0].size() != 16 || args[1].size() != 16 || result.size() != 16) return false;
        for (std::size_t i = 0; i < 16; ++i) result[i] = static_cast<std::uint8_t>(std::min(255, args[0][i] + args[1][i]));
        return true;
    });
    CB_CHECK(run_one(emu, code) == Emu::Status::Ok);
    CB_CHECK_EQ(seen_name, std::string("vectorAddUnsignedByteSaturate"));
    Bytes want(16);
    for (std::size_t i = 0; i < 16; ++i) want[i] = static_cast<std::uint8_t>(std::min(255, x[i] + y[i]));
    CB_CHECK_EQ(hex(reg_wide(emu, "vs32")), hex(want));
    emu.onUserOpWide({});
    emu.onUserOp({});
});

// A run of blocks gives what single steps give when instructions with wide values are in it (they leave the flat blocks and run by the wide path in the middle of one).
CB_TEST("engine.pcode_cpu.blocks_with_wide_instructions_run_as_single_steps_do", [] {
    std::mt19937_64 rng(20261130);
    const Bytes program = {0x66, 0x0F, 0xEF, 0xC1,         // pxor xmm0, xmm1
                           0x48, 0x01, 0xC8,               // add rax, rcx
                           0x66, 0x0F, 0xD4, 0xC1,         // paddq xmm0, xmm1
                           0x48, 0xF7, 0xE1,               // mul rcx
                           0x66, 0x0F, 0x6F, 0x12,         // movdqa xmm2, [rdx]
                           0x66, 0x0F, 0xEB, 0xD0,         // por xmm2, xmm0
                           0x48, 0xFF, 0xC0,               // inc rax
                           0x66, 0x0F, 0x7F, 0x12,         // movdqa [rdx], xmm2
                           0x48, 0x29, 0xC8,               // sub rax, rcx
                           0x90};                          // nop
    const unsigned count = 10;
    unsigned different = 0;
    for (int k = 0; k < 60; ++k) {
        const std::unique_ptr<Emu> single_ptr = x86_64(), blocks_ptr = x86_64();
        Emu* both[2] = {single_ptr.get(), blocks_ptr.get()};
        const Bytes x = random_value(rng, 16), y = random_value(rng, 16), memory = random_value(rng, 16);
        const std::uint64_t a = rng(), c = rng();
        for (Emu* emu : both) {
            emu->write(0x1000, program.data(), program.size());
            emu->write(0x6000, memory.data(), 16);
            set_wide(*emu, "XMM0", x);
            set_wide(*emu, "XMM1", y);
            emu->setReg("RAX", a);
            emu->setReg("RCX", c);
            emu->setReg("RDX", 0x6000);
            emu->setPc(0x1000);
        }
        Emu::setBlocksOverride(0);
        for (unsigned i = 0; i < count; ++i) CB_CHECK(single_ptr->step() == Emu::Status::Ok);
        Emu::setBlocksOverride(1);
        std::uint64_t done = 0;
        while (done < count) {
            const Emu::BlockResult r = blocks_ptr->runBlocks(count - done);
            CB_CHECK(r.status == Emu::Status::Ok);
            done += r.executed;
            if (r.executed == 0) break;
        }
        Emu::setBlocksOverride(-1);
        CB_CHECK_EQ(done, std::uint64_t{count});
        for (const char* name : {"XMM0", "XMM1", "XMM2"}) {
            if (reg_wide(*single_ptr, name) != reg_wide(*blocks_ptr, name)) ++different;
        }
        for (const char* name : {"RAX", "RCX", "RDX"})
            if (single_ptr->reg(name) != blocks_ptr->reg(name)) ++different;
        if (single_ptr->pc() != blocks_ptr->pc()) ++different;
        Bytes m1(16), m2(16);
        single_ptr->read(0x6000, m1.data(), 16);
        blocks_ptr->read(0x6000, m2.data(), 16);
        if (m1 != m2) ++different;
        if (single_ptr->steps() != blocks_ptr->steps()) ++different;
    }
    CB_CHECK_EQ(different, 0u);
});

// What an instruction with a wide value costs on the emulator, for whoever moves a lane's vector instructions here: instructions per second of a loop of each kind, narrow ones
// beside them (nothing is asserted but that they ran; the figures are printed).
CB_TEST("engine.pcode_cpu.bench_wide_instructions", [] {
    struct Case {
        const char* name;
        Bytes code;   // a straight run of copies of one instruction, then a jump back (jmp rel8)
    };
    const std::vector<Case> cases = {
        {"add rax, rcx (narrow)", {0x48, 0x01, 0xC8}},          {"pxor xmm0, xmm1", {0x66, 0x0F, 0xEF, 0xC1}},         {"paddq xmm0, xmm1", {0x66, 0x0F, 0xD4, 0xC1}},
        {"movdqa xmm0, [rcx]", {0x66, 0x0F, 0x6F, 0x01}},       {"movdqa [rcx], xmm0", {0x66, 0x0F, 0x7F, 0x01}},       {"mul rcx", {0x48, 0xF7, 0xE1}},
        {"div rcx", {0x48, 0xF7, 0xF1}},                        {"pslldq xmm0, 4", {0x66, 0x0F, 0x73, 0xF8, 0x04}},     {"fadd st0, st1", {0xD8, 0xC1}},
        {"fmul st0, st1", {0xD8, 0xC9}},                        {"fld tbyte [rcx]; fstp st0", {0xDB, 0x29, 0xDD, 0xD8}}};
    const std::unique_ptr<Emu> emu_ptr = x86_64();
    Emu& emu = *emu_ptr;
    Emu::setBlocksOverride(-1);
    for (const Case& c : cases) {
        Bytes program;
        for (int i = 0; i < 16; ++i) program.insert(program.end(), c.code.begin(), c.code.end());
        program.push_back(0xEB);   // jmp back to the start
        program.push_back(static_cast<std::uint8_t>(-static_cast<int>(program.size() + 1)));
        if (program.size() > 128) {   // (rel8 reaches 128 bytes back: a near jump otherwise)
            program.pop_back();
            program.pop_back();
            program.push_back(0xE9);
            const std::int32_t back = -static_cast<std::int32_t>(program.size() + 4);
            for (int i = 0; i < 4; ++i) program.push_back(static_cast<std::uint8_t>(back >> (8 * i)));
        }
        emu.write(0x1000, program.data(), program.size());
        emu.setReg("RCX", 0x4000);
        emu.setReg("RAX", 7);
        emu.setReg("RDX", 1);
        set_wide(emu, "XMM0", Bytes(16, 3));
        set_wide(emu, "XMM1", Bytes(16, 5));
        const Bytes one = f80_of(1.5);
        emu.write(0x4000, one.data(), 10);
        std::uint64_t executed = 0;
        double seconds = 0;
        for (int round = 0; round < 2; ++round) {   // (the first round decodes and warms the caches: only the second is timed)
            emu.setPc(0x1000);
            executed = 0;
            const auto start = std::chrono::steady_clock::now();
            while (executed < 300000) {
                const Emu::BlockResult r = emu.runBlocks(10000);
                if (r.status != Emu::Status::Ok) break;
                executed += r.executed;
                if (c.name[0] == 'd') emu.setReg("RDX", 1);   // (a divide wants the high half under the divisor)
            }
            seconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - start).count();
        }
        std::printf("bench wide: %-28s %9llu instructions, %6.1f ns each\n", c.name, static_cast<unsigned long long>(executed), seconds * 1e9 / static_cast<double>(executed ? executed : 1));
        CB_CHECK(executed >= 300000);
    }
});

// Wide register access: readRegWide and writeRegWide are value order, readRegs and writeRegs memory order, in both byte orders.
CB_TEST("engine.pcode_cpu.wide_registers_read_and_write_as_values", [] {
    for (const char* which : {"x86", "ppc"}) {
        std::unique_ptr<Emu> owned;
        Emu* emu = nullptr;
        if (std::string(which) == "x86") {
            owned = x86_64();
            emu = owned.get();
        } else {
            emu = &engine::pcode_cpu::emulator_named("ppc_64_isa_altivec_be.sla");
        }
        const char* name = std::string(which) == "x86" ? "XMM3" : "vs35";
        const Emu::RegRef ref = emu->regRef(name);
        CB_CHECK_EQ(ref.size, 16u);
        Bytes value(16);
        for (std::size_t i = 0; i < 16; ++i) value[i] = static_cast<std::uint8_t>(0xA0 + i);
        emu->writeRegWide(ref, value.data());
        Bytes back(16);
        emu->readRegWide(ref, back.data());
        CB_CHECK_EQ(hex(back), hex(value));
        // the low 8 bytes as the narrow call reads them
        CB_CHECK_EQ(emu->reg(ref), std::uint64_t{0xA7A6A5A4A3A2A1A0ull});
        Bytes raw(16);
        emu->readRegs(ref, raw.data(), 16);
        if (std::string(which) == "x86") CB_CHECK_EQ(hex(raw), hex(value));
        else CB_CHECK_EQ(raw[0], std::uint8_t{0xAF});   // (big endian: the most significant byte is first in memory)
        emu->setReg(ref, 0x0102030405060708ull);   // (the narrow write of a wide register: the low 8 bytes set, the rest zero)
        emu->readRegWide(ref, back.data());
        CB_CHECK_EQ(hex(back), std::string("00000000000000000102030405060708"));
    }
});

}  // namespace cb
