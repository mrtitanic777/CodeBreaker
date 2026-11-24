// cb-unity: alone (file-local names that other files in a batch also use)
// engine/pcode_vector against a real implementation: AArch64 Advanced SIMD and x86-64 SSE instructions run on Unicorn (QEMU's translator), each with random operands (edge values
// mixed in), and the register it leaves compared with what the library computes for the same operands. This is a check on how the library reads the manuals, apart from the
// exact models of pcode_vector_test.cpp, which were written by the same hand. Instructions Unicorn's CPU model lacks are counted and named, not skipped silently.
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <functional>
#include <random>
#include <string>
#include <vector>

#include "core/testing.hpp"
#include "core/unicorn_api.hpp"
#include "engine/pcode_vector.hpp"

namespace cb {
namespace {

namespace vec = engine::vec;
using vec::Byte;
using Bytes = std::vector<Byte>;
using Sign = vec::Sign;

struct Engine {
    uc_engine* uc = nullptr;
    Engine(uc_arch arch, uc_mode mode) {
        const auto& u = emu::unicorn::api();
        emu::unicorn::check(u.uc_open(arch, mode, &uc), "uc_open");
        emu::unicorn::check(u.uc_mem_map(uc, 0x10000, 0x10000, UC_PROT_ALL), "uc_mem_map");
    }
    ~Engine() { emu::unicorn::api().uc_close(uc); }
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
};

std::mt19937_64 g_rng(0xa64);
std::uint64_t edge(unsigned ew) {
    const unsigned w = 8 * ew;
    const std::uint64_t mask = vec::laneMask(ew);
    switch (g_rng() % 10) {
        case 0: return 0;
        case 1: return mask;
        case 2: return std::uint64_t{1} << (w - 1);
        case 3: return (std::uint64_t{1} << (w - 1)) - 1;
        case 4: return 1;
        case 5: return (std::uint64_t{1} << (g_rng() % w)) & mask;
        default: return g_rng() & mask;
    }
}
Bytes operand(unsigned ew) {
    Bytes v(16);
    for (std::size_t i = 0; i < 16; i += ew) vec::store(v.data() + i, ew, edge(ew));
    return v;
}
std::string hex(const Bytes& v) {
    std::string s;
    char b[4];
    for (std::size_t i = v.size(); i-- > 0;) {
        std::snprintf(b, sizeof b, "%02x", v[i]);
        s += b;
    }
    return s;
}

// ---- AArch64: Rd = v0, Rn = v1, Rm = v2
struct A64Case {
    std::string name;
    std::uint32_t word;                                                 // the encoding with Rd = 0, Rn = 1, Rm = 2 (the model sees the same three registers' values)
    unsigned ew;                                                        // the lane width the operands are made of
    std::function<Bytes(const Bytes& d, const Bytes& n, const Bytes& m)> model;   // what v0 holds afterwards
};
unsigned g_a64Missing = 0, g_a64Lacks = 0, g_fails = 0;

void runA64(Engine& e, const A64Case& c, int iterations) {
    const auto& u = emu::unicorn::api();
    std::uint8_t code[4];
    for (int i = 0; i < 4; ++i) code[i] = static_cast<std::uint8_t>(c.word >> (8 * i));
    emu::unicorn::check(u.uc_mem_write(e.uc, 0x10000, code, 4), "uc_mem_write");
    for (int iter = 0; iter < iterations; ++iter) {
        const unsigned ew = c.ew;
        const Bytes d = operand(ew), n = operand(ew), m = operand(ew);
        const Bytes regs[3] = {d, n, m};
        for (int r = 0; r < 3; ++r) emu::unicorn::check(u.uc_reg_write(e.uc, UC_ARM64_REG_Q0 + r, regs[r].data()), "uc_reg_write");
        std::uint64_t pc = 0x10000;
        u.uc_reg_write(e.uc, UC_ARM64_REG_PC, &pc);
        const uc_err err = u.uc_emu_start(e.uc, 0x10000, 0x10004, 0, 1);
        if (err != UC_ERR_OK) {
            if (c.name == "sdot" || c.name == "udot" || c.name == "usdot" || c.name == "sqrdmlah" || c.name == "sqrdmlsh") {   // (Unicorn's CPU lacks these extensions: undefined instructions)
                ++g_a64Lacks;
                return;
            }
            if (++g_a64Missing <= 1000) std::printf("  %s %08x: Unicorn error %d\n", c.name.c_str(), c.word, static_cast<int>(err));
            return;
        }
        Bytes got(16), want = c.model(d, n, m);
        emu::unicorn::check(u.uc_reg_read(e.uc, UC_ARM64_REG_Q0, got.data()), "uc_reg_read");
        want.resize(16, 0);
        if (got != want && ++g_fails <= 25)
            std::printf("  %s %08x ew=%u: d=%s n=%s m=%s\n      unicorn %s\n      library %s\n", c.name.c_str(), c.word, ew, hex(d).c_str(), hex(n).c_str(), hex(m).c_str(), hex(got).c_str(),
                        hex(want).c_str());
    }
}

std::vector<A64Case> a64Cases() {
    std::vector<A64Case> cases;
    const auto sizeBits = [](unsigned ew) { return ew == 1 ? 0u : ew == 2 ? 1u : ew == 4 ? 2u : 3u; };
    // three registers, the same lane width: 0 Q U 01110 size 1 Rm opcode 1 Rn Rd
    const auto same = [&](const char* name, unsigned u, unsigned opcode, unsigned ew, std::function<void(Byte*, const Bytes&, const Bytes&, const Bytes&, unsigned)> f) {
        const std::uint32_t word = 0x4e200400u | (u << 29) | (sizeBits(ew) << 22) | (2u << 16) | (opcode << 11) | (1u << 5);
        cases.push_back({std::string(name), word, ew, [f, ew](const Bytes& d, const Bytes& n, const Bytes& m) {
                             Bytes out(16);
                             f(out.data(), d, n, m, ew);
                             return out;
                         }});
    };
    using F = std::function<void(Byte*, const Bytes&, const Bytes&, const Bytes&, unsigned)>;
    const auto bin = [](auto fn) { return F([fn](Byte* o, const Bytes&, const Bytes& n, const Bytes& m, unsigned ew) { fn(o, n.data(), m.data(), 16, ew); }); };
    for (unsigned ew : {1u, 2u, 4u, 8u}) {
        same("add", 0, 0x10, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::add(o, a, b, n, w); }));
        same("sub", 1, 0x10, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::sub(o, a, b, n, w); }));
        for (unsigned s = 0; s < 2; ++s) {
            const Sign sg = s ? Sign::Unsigned : Sign::Signed;
            same(s ? "uqadd" : "sqadd", s, 0x01, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::addSat(o, a, b, n, w, sg); }));
            same(s ? "uqsub" : "sqsub", s, 0x05, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::subSat(o, a, b, n, w, sg); }));
            if (ew != 8) {
                same(s ? "uhadd" : "shadd", s, 0x00, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::halvingAdd(o, a, b, n, w, sg, false); }));
                same(s ? "urhadd" : "srhadd", s, 0x02, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::halvingAdd(o, a, b, n, w, sg, true); }));
                same(s ? "uhsub" : "shsub", s, 0x04, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::halvingSub(o, a, b, n, w, sg); }));
                same(s ? "umax" : "smax", s, 0x0c, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::max(o, a, b, n, w, sg); }));
                same(s ? "umin" : "smin", s, 0x0d, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::min(o, a, b, n, w, sg); }));
                same(s ? "uabd" : "sabd", s, 0x0e, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::absDiff(o, a, b, n, w, sg); }));
                same(s ? "umaxp" : "smaxp", s, 0x14, ew, bin([s](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::pairwise(o, a, b, n, w, s ? vec::Pair::MaxUnsigned : vec::Pair::MaxSigned); }));
                same(s ? "uminp" : "sminp", s, 0x15, ew, bin([s](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::pairwise(o, a, b, n, w, s ? vec::Pair::MinUnsigned : vec::Pair::MinSigned); }));
            }
            same(s ? "cmhi" : "cmgt", s, 0x06, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::compare(o, a, b, n, w, vec::Cmp::Gt, sg); }));
            same(s ? "cmhs" : "cmge", s, 0x07, ew, bin([sg](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::compare(o, a, b, n, w, vec::Cmp::Ge, sg); }));
            // register shifts, with and without rounding and saturation (the count is the signed low byte of each lane of Rm; Rn is shifted)
            const struct { const char* name; unsigned opcode; bool round, sat; } regShifts[] = {{s ? "ushl" : "sshl", 0x08, false, false}, {s ? "uqshl" : "sqshl", 0x09, false, true},
                                                                                              {s ? "urshl" : "srshl", 0x0a, true, false}, {s ? "uqrshl" : "sqrshl", 0x0b, true, true}};
            for (const auto& rs : regShifts)
                same(rs.name, s, rs.opcode, ew, F([sg, rs](Byte* o, const Bytes&, const Bytes& n, const Bytes& m, unsigned w) { vec::shiftNeon(o, n.data(), m.data(), 16, w, sg, rs.round, rs.sat); }));
        }
        if (ew != 8) {
            same("mul", 0, 0x13, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::mul(o, a, b, n, w); }));
            same("mla", 0, 0x12, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes& m, unsigned w) {
                     Bytes p(16);
                     vec::mul(p.data(), n.data(), m.data(), 16, w);
                     vec::add(o, d.data(), p.data(), 16, w);
                 }));
            same("mls", 1, 0x12, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes& m, unsigned w) {
                     Bytes p(16);
                     vec::mul(p.data(), n.data(), m.data(), 16, w);
                     vec::sub(o, d.data(), p.data(), 16, w);
                 }));
            same("saba", 0, 0x0f, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes& m, unsigned w) {
                     Bytes p(16);
                     vec::absDiff(p.data(), n.data(), m.data(), 16, w, Sign::Signed);
                     vec::add(o, d.data(), p.data(), 16, w);
                 }));
            same("uaba", 1, 0x0f, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes& m, unsigned w) {
                     Bytes p(16);
                     vec::absDiff(p.data(), n.data(), m.data(), 16, w, Sign::Unsigned);
                     vec::add(o, d.data(), p.data(), 16, w);
                 }));
        }
        if (ew == 2 || ew == 4) {
            same("sqdmulh", 0, 0x16, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::mulDoublingHigh(o, a, b, n, w, false); }));
            same("sqrdmulh", 1, 0x16, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::mulDoublingHigh(o, a, b, n, w, true); }));
            // SQRDMLAH / SQRDMLSH (ARMv8.1): 0 Q 1 01110 size 0 Rm 1000 S 1 Rn Rd
            for (unsigned sub = 0; sub < 2; ++sub) {
                const std::uint32_t word = 0x6e008400u | (sizeBits(ew) << 22) | (2u << 16) | (sub << 11) | (1u << 5);
                cases.push_back({sub ? "sqrdmlsh" : "sqrdmlah", word, ew, [ew, sub](const Bytes& d, const Bytes& n, const Bytes& m) {
                                     Bytes out(16);
                                     vec::mulDoublingHighAcc(out.data(), d.data(), n.data(), m.data(), 16, ew, sub != 0);
                                     return out;
                                 }});
            }
        }
        same("addp", 0, 0x17, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::pairwise(o, a, b, n, w, vec::Pair::Add); }));
        same("cmeq", 1, 0x11, ew, bin([](Byte* o, const Byte* a, const Byte* b, std::size_t n, unsigned w) { vec::compare(o, a, b, n, w, vec::Cmp::Eq, Sign::Unsigned); }));
        same("cmtst", 0, 0x11, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes& m, unsigned w) {
                 Bytes t(16);
                 vec::bitAnd(t.data(), n.data(), m.data(), 16);
                 vec::compare(o, t.data(), Bytes(16, 0).data(), 16, w, vec::Cmp::Ne, Sign::Unsigned);
             }));
    }
    // two registers, one: 0 Q U 01110 size 10000 opcode 10 Rn Rd
    const auto misc = [&](const char* name, unsigned u, unsigned opcode, unsigned ew, F f, unsigned sizeOverride = 99) {
        const std::uint32_t word = 0x4e200800u | (u << 29) | ((sizeOverride == 99 ? sizeBits(ew) : sizeOverride) << 22) | (opcode << 12) | (1u << 5);
        cases.push_back({std::string(name), word, ew, [f, ew](const Bytes& d, const Bytes& n, const Bytes&) {
                             Bytes out(16);
                             f(out.data(), d, n, n, ew);
                             return out;
                         }});
    };
    for (unsigned ew : {1u, 2u, 4u, 8u}) {
        misc("abs", 0, 0x0b, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::abs(o, n.data(), 16, w, false); }));
        misc("neg", 1, 0x0b, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::neg(o, n.data(), 16, w, false); }));
        misc("sqabs", 0, 0x07, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::abs(o, n.data(), 16, w, true); }));
        misc("sqneg", 1, 0x07, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::neg(o, n.data(), 16, w, true); }));
        if (ew != 8) {
            misc("cls", 0, 0x04, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::bitOp(o, n.data(), 16, w, vec::BitOp::CountLeadingSigns); }));
            misc("clz", 1, 0x04, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::bitOp(o, n.data(), 16, w, vec::BitOp::CountLeadingZeros); }));
            misc("saddlp", 0, 0x02, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::addLongPairwise(o, nullptr, n.data(), 16, w, Sign::Signed); }));
            misc("uaddlp", 1, 0x02, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::addLongPairwise(o, nullptr, n.data(), 16, w, Sign::Unsigned); }));
            misc("sadalp", 0, 0x06, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes&, unsigned w) { vec::addLongPairwise(o, d.data(), n.data(), 16, w, Sign::Signed); }));
            misc("uadalp", 1, 0x06, ew, F([](Byte* o, const Bytes& d, const Bytes& n, const Bytes&, unsigned w) { vec::addLongPairwise(o, d.data(), n.data(), 16, w, Sign::Unsigned); }));
        }
        if (ew <= 4) {   // REV64, REV32, REV16: the lanes of the element size reversed inside each group
            misc("rev64", 0, 0x00, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::reverseInGroups(o, n.data(), 16, w, 8); }));
            if (ew <= 2) misc("rev32", 1, 0x00, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::reverseInGroups(o, n.data(), 16, w, 4); }));
            if (ew == 1) misc("rev16", 0, 0x01, ew, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::reverseInGroups(o, n.data(), 16, w, 2); }));
        }
    }
    misc("cnt", 0, 0x05, 1, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::bitOp(o, n.data(), 16, w, vec::BitOp::Popcount); }));
    misc("rbit", 1, 0x05, 1, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned w) { vec::bitOp(o, n.data(), 16, w, vec::BitOp::ReverseBits); }), 1);
    misc("not", 1, 0x05, 1, F([](Byte* o, const Bytes&, const Bytes& n, const Bytes&, unsigned) { vec::bitNot(o, n.data(), 16); }));
    // permutes: 0 Q 001110 size 0 Rm 0 opcode 10 Rn Rd
    for (unsigned ew : {1u, 2u, 4u, 8u}) {
        const struct { const char* name; unsigned opc; int kind; bool flag; } perm[] = {{"uzp1", 1, 0, false}, {"uzp2", 5, 0, true}, {"trn1", 2, 1, false}, {"trn2", 6, 1, true}, {"zip1", 3, 2, false}, {"zip2", 7, 2, true}};
        for (const auto& p : perm) {
            const std::uint32_t word = 0x4e000800u | (sizeBits(ew) << 22) | (2u << 16) | (p.opc << 12) | (1u << 5);
            cases.push_back({p.name, word, ew, [p, ew](const Bytes&, const Bytes& n, const Bytes& m) {
                                 Bytes out(16);
                                 if (p.kind == 0) vec::unzip(out.data(), n.data(), m.data(), 16, ew, p.flag);
                                 else if (p.kind == 1) vec::transpose(out.data(), n.data(), m.data(), 16, ew, p.flag);
                                 else vec::zip(out.data(), n.data(), m.data(), 16, ew, p.flag);
                                 return out;
                             }});
        }
    }
    for (unsigned idx : {1u, 5u, 15u}) {   // EXT
        const std::uint32_t word = 0x6e000000u | (2u << 16) | (idx << 11) | (1u << 5);
        cases.push_back({"ext", word, 1, [idx](const Bytes&, const Bytes& n, const Bytes& m) {
                             Bytes out(16);
                             vec::extract(out.data(), n.data(), m.data(), 16, idx);
                             return out;
                         }});
    }
    // TBL, one to two registers (Rn = v1, v2 as the table, Rm = v2 in the one register form)
    cases.push_back({"tbl1", 0x4e000000u | (2u << 16) | (1u << 5), 1, [](const Bytes&, const Bytes& n, const Bytes& m) {
                         Bytes idx(16);
                         for (std::size_t i = 0; i < 16; ++i) idx[i] = static_cast<Byte>(m[i] % 24);
                         Bytes out(16);
                         vec::tableLookup(out.data(), n.data(), 16, m.data(), 16, vec::Lookup{});
                         return out;
                     }});
    // dot products: SDOT, UDOT, USDOT (vector), accumulating into v0
    cases.push_back({"sdot", 0x4e809400u | (2u << 16) | (1u << 5), 1, [](const Bytes& d, const Bytes& n, const Bytes& m) {
                         Bytes out(16);
                         vec::multiplyAdd(out.data(), d.data(), n.data(), m.data(), 16, 1, 4, Sign::Signed, Sign::Signed, false);
                         return out;
                     }});
    cases.push_back({"udot", 0x6e809400u | (2u << 16) | (1u << 5), 1, [](const Bytes& d, const Bytes& n, const Bytes& m) {
                         Bytes out(16);
                         vec::multiplyAdd(out.data(), d.data(), n.data(), m.data(), 16, 1, 4, Sign::Unsigned, Sign::Unsigned, false);
                         return out;
                     }});
    cases.push_back({"usdot", 0x4e809c00u | (2u << 16) | (1u << 5), 1, [](const Bytes& d, const Bytes& n, const Bytes& m) {
                         Bytes out(16);
                         vec::multiplyAdd(out.data(), d.data(), n.data(), m.data(), 16, 1, 4, Sign::Unsigned, Sign::Signed, false);
                         return out;
                     }});
    // across lanes: ADDV, SMAXV, UMINV
    for (unsigned ew : {1u, 2u, 4u}) {
        const struct { const char* name; unsigned u, opc; vec::Reduce op; Sign sg; } acr[] = {{"addv", 0, 0x1b, vec::Reduce::Add, Sign::Unsigned}, {"smaxv", 0, 0x0a, vec::Reduce::Max, Sign::Signed},
                                                                                           {"uminv", 1, 0x1a, vec::Reduce::Min, Sign::Unsigned}, {"saddlv", 0, 0x03, vec::Reduce::AddLong, Sign::Signed},
                                                                                           {"uaddlv", 1, 0x03, vec::Reduce::AddLong, Sign::Unsigned}};
        for (const auto& a : acr) {
            if (ew == 4 && (a.opc == 0x03)) continue;   // (the 4S long form has its own encoding; the widths below cover the rule)
            const std::uint32_t word = 0x4e300800u | (a.u << 29) | (sizeBits(ew) << 22) | (a.opc << 12) | (1u << 5);
            cases.push_back({a.name, word, ew, [a, ew](const Bytes&, const Bytes& n, const Bytes&) {
                                 Bytes out(16, 0);
                                 vec::store(out.data(), a.op == vec::Reduce::AddLong ? 2 * ew : ew, vec::reduce(n.data(), 16, ew, a.op, a.sg));
                                 return out;
                             }});
        }
    }
    // multiply long: SMULL / UMULL (low halves, Q = 0): 0 Q U 01110 size 1 Rm 1100 00 Rn Rd; the result is 128 bits
    for (unsigned ew : {1u, 2u, 4u})
        for (unsigned s = 0; s < 2; ++s) {
            const std::uint32_t word = 0x0e20c000u | (s << 29) | (sizeBits(ew) << 22) | (2u << 16) | (1u << 5);
            cases.push_back({s ? "umull" : "smull", word, ew, [ew, s](const Bytes&, const Bytes& n, const Bytes& m) {
                                 Bytes out(16);
                                 vec::mulWiden(out.data(), n.data(), m.data(), 8, ew, s ? Sign::Unsigned : Sign::Signed, s ? Sign::Unsigned : Sign::Signed);
                                 return out;
                             }});
        }
    // narrowing: XTN, SQXTN, UQXTN, SQXTUN (the low half of v0, Q = 0)
    for (unsigned ew : {2u, 4u, 8u}) {
        const struct { const char* name; unsigned u, opc; vec::Narrow mode; } nar[] = {{"xtn", 0, 0x12, vec::Narrow::Truncate}, {"sqxtn", 0, 0x14, vec::Narrow::SignedToSigned},
                                                                                       {"uqxtn", 1, 0x14, vec::Narrow::UnsignedToUnsigned}, {"sqxtun", 1, 0x12, vec::Narrow::SignedToUnsigned}};
        for (const auto& nr : nar) {
            const std::uint32_t word = 0x0e200800u | (nr.u << 29) | (sizeBits(ew) << 22) | (nr.opc << 12) | (1u << 5);   // the size field is the narrow lane's: one less
            const std::uint32_t fixed = (word & ~(3u << 22)) | ((sizeBits(ew) - 1) << 22);
            cases.push_back({nr.name, fixed, ew, [nr, ew](const Bytes&, const Bytes& n, const Bytes&) {
                                 Bytes out(16, 0);
                                 vec::narrow(out.data(), n.data(), 16, ew, nr.mode);
                                 return out;
                             }});
        }
    }
    // shifts by an immediate: 0 Q U 011110 immh immb opcode 1 Rn Rd; right shifts encode (2 esize - shift), left ones (esize + shift)
    for (unsigned ew : {1u, 2u, 4u, 8u}) {
        const unsigned w = 8 * ew;
        for (unsigned shift : {1u, 2u, w / 2, w - 1, w}) {
            if (shift == 0 || shift > w) continue;
            const unsigned right = 2 * w - shift;   // immh:immb of a right shift
            for (unsigned s = 0; s < 2; ++s) {
                const Sign sg = s ? Sign::Unsigned : Sign::Signed;
                const auto word = [&](unsigned immhb, unsigned u, unsigned opc) { return 0x4f000400u | (u << 29) | (immhb << 16) | (opc << 11) | (1u << 5); };
                cases.push_back({s ? "ushr" : "sshr", word(right, s, 0x00), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftRightImm(o.data(), n.data(), 16, ew, sg, shift, false); return o; }});
                cases.push_back({s ? "urshr" : "srshr", word(right, s, 0x04), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftRightImm(o.data(), n.data(), 16, ew, sg, shift, true); return o; }});
                cases.push_back({s ? "usra" : "ssra", word(right, s, 0x02), ew, [=](const Bytes& d, const Bytes& n, const Bytes&) {
                                     Bytes t(16), o(16);
                                     vec::shiftRightImm(t.data(), n.data(), 16, ew, sg, shift, false);
                                     vec::add(o.data(), d.data(), t.data(), 16, ew);
                                     return o;
                                 }});
                cases.push_back({s ? "ursra" : "srsra", word(right, s, 0x06), ew, [=](const Bytes& d, const Bytes& n, const Bytes&) {
                                     Bytes t(16), o(16);
                                     vec::shiftRightImm(t.data(), n.data(), 16, ew, sg, shift, true);
                                     vec::add(o.data(), d.data(), t.data(), 16, ew);
                                     return o;
                                 }});
            }
            cases.push_back({"sri", 0x4f000400u | (1u << 29) | (right << 16) | (0x08u << 11) | (1u << 5), ew, [=](const Bytes& d, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftInsert(o.data(), d.data(), n.data(), 16, ew, false, shift); return o; }});
        }
        for (unsigned shift : {0u, 1u, w / 2, w - 1}) {
            const unsigned left = w + shift;
            const auto word = [&](unsigned u, unsigned opc) { return 0x4f000400u | (u << 29) | (left << 16) | (opc << 11) | (1u << 5); };
            cases.push_back({"shl", word(0, 0x0a), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftImm(o.data(), n.data(), 16, ew, vec::Shift::Left, shift, vec::Count::Saturate); return o; }});
            cases.push_back({"sli", word(1, 0x0a), ew, [=](const Bytes& d, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftInsert(o.data(), d.data(), n.data(), 16, ew, true, shift); return o; }});
            cases.push_back({"sqshl#", word(0, 0x0e), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftLeftSatImm(o.data(), n.data(), 16, ew, Sign::Signed, shift, false); return o; }});
            cases.push_back({"uqshl#", word(1, 0x0e), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftLeftSatImm(o.data(), n.data(), 16, ew, Sign::Unsigned, shift, false); return o; }});
            cases.push_back({"sqshlu#", word(1, 0x0c), ew, [=](const Bytes&, const Bytes& n, const Bytes&) { Bytes o(16); vec::shiftLeftSatImm(o.data(), n.data(), 16, ew, Sign::Signed, shift, true); return o; }});
        }
    }
    return cases;
}

}  // namespace

CB_TEST("engine.pcode_vector.aarch64_integer_simd_matches_unicorn", [] {
    g_fails = 0;
    g_a64Missing = 0;
    Engine e(UC_ARCH_ARM64, UC_MODE_ARM);
    std::uint64_t cpacr = 3u << 20;   // FP and Advanced SIMD enabled
    emu::unicorn::api().uc_reg_write(e.uc, UC_ARM64_REG_CPACR_EL1, &cpacr);
    const std::vector<A64Case> cases = a64Cases();
    for (const A64Case& c : cases) runA64(e, c, 40);
    std::printf("  aarch64: %zu encodings, %u of them executed by Unicorn (the other %u -- dot products and SQRDMLAH/SQRDMLSH -- are undefined on its CPU and checked against the model only), %u differences\n",
                cases.size(), static_cast<unsigned>(cases.size()) - g_a64Lacks, g_a64Lacks, g_fails);
    CB_CHECK_EQ(g_fails, 0u);
    CB_CHECK_EQ(g_a64Missing, 0u);
});

namespace {

// ---- AArch64 floating point lanes: the result register and the cumulative exception flags (FPSR) after one instruction, with the default FPCR (nearest even, no flush, no DN)
std::uint64_t floatLane(unsigned ew) {
    const unsigned fb = ew == 2 ? 10 : ew == 4 ? 23 : 52, eb = ew == 2 ? 5 : ew == 4 ? 8 : 11;
    const std::uint64_t sign = (g_rng() & 1) ? std::uint64_t{1} << (eb + fb) : 0, expMax = (std::uint64_t{1} << eb) - 1, frac = (std::uint64_t{1} << fb) - 1, bias = expMax >> 1;
    switch (g_rng() % 14) {
        case 0: return sign;
        case 1: return sign | (expMax << fb);
        case 2: return sign | (expMax << fb) | (std::uint64_t{1} << (fb - 1)) | (g_rng() & (frac >> 2));
        case 3: return sign | (expMax << fb) | std::max<std::uint64_t>(1, g_rng() & (frac >> 1));
        case 4: return sign | (g_rng() & frac);
        case 5: return sign | ((expMax - 1) << fb) | frac;
        case 6: return sign | ((bias + (g_rng() % 8)) << fb) | (g_rng() & frac);
        case 7: return sign | ((bias - (g_rng() % 8)) << fb) | (g_rng() & frac);
        case 8: return sign | (std::uint64_t{1} << fb) | (g_rng() & 3);
        case 9: return sign | ((expMax - 1 - (g_rng() % 3)) << fb) | (g_rng() & frac);
        default: return g_rng() & ((std::uint64_t{1} << (eb + fb + 1)) - 1);
    }
}
Bytes floatOperand(unsigned ew) {
    Bytes v(16);
    for (std::size_t i = 0; i < 16; i += ew) vec::store(v.data() + i, ew, floatLane(ew));
    return v;
}
struct A64FpCase {
    std::string name;
    std::uint32_t word;
    unsigned ew;                                                                             // the float lane width
    std::function<Bytes(const Bytes& d, const Bytes& n, const Bytes& m, unsigned& flags)> model;
    bool intOperands = false;                                                                // scvtf, ucvtf: the source lanes are integers
};
unsigned g_fpFails = 0;

void runA64Fp(Engine& e, const A64FpCase& c, int iterations) {
    const auto& u = emu::unicorn::api();
    std::uint8_t code[4];
    for (int i = 0; i < 4; ++i) code[i] = static_cast<std::uint8_t>(c.word >> (8 * i));
    emu::unicorn::check(u.uc_mem_write(e.uc, 0x10000, code, 4), "uc_mem_write");
    for (int iter = 0; iter < iterations; ++iter) {
        const Bytes d = floatOperand(c.ew), n = c.intOperands ? operand(c.ew) : floatOperand(c.ew), m = floatOperand(c.ew);
        const Bytes regs[3] = {d, n, m};
        for (int r = 0; r < 3; ++r) emu::unicorn::check(u.uc_reg_write(e.uc, UC_ARM64_REG_Q0 + r, regs[r].data()), "uc_reg_write");
        std::uint64_t pc = 0x10000, fpsr = 0, fpcr = 0;
        u.uc_reg_write(e.uc, UC_ARM64_REG_PC, &pc);
        u.uc_reg_write(e.uc, UC_ARM64_REG_FPSR, &fpsr);
        u.uc_reg_write(e.uc, UC_ARM64_REG_FPCR, &fpcr);
        const uc_err err = u.uc_emu_start(e.uc, 0x10000, 0x10004, 0, 1);
        if (err != UC_ERR_OK) {
            std::printf("  %s %08x: Unicorn error %d\n", c.name.c_str(), c.word, static_cast<int>(err));
            ++g_fpFails;
            return;
        }
        Bytes got(16);
        std::uint64_t gotFpsr = 0;
        emu::unicorn::check(u.uc_reg_read(e.uc, UC_ARM64_REG_Q0, got.data()), "uc_reg_read");
        u.uc_reg_read(e.uc, UC_ARM64_REG_FPSR, &gotFpsr);
        unsigned flags = 0;
        Bytes want = c.model(d, n, m, flags);
        want.resize(16, 0);
        // FPSR: IOC 1, DZC 2, OFC 4, UFC 8, IXC 16 (the input denormal bit, IDC, is only set under flush-to-zero)
        const unsigned wantFpsr = ((flags & vec::kInvalid) ? 1u : 0u) | ((flags & vec::kDivideByZero) ? 2u : 0u) | ((flags & vec::kOverflow) ? 4u : 0u) |
                                  ((flags & vec::kUnderflow) ? 8u : 0u) | ((flags & vec::kInexact) ? 16u : 0u);
        if ((got != want || (gotFpsr & 0x1fu) != wantFpsr) && ++g_fpFails <= 25)
            std::printf("  %s %08x ew=%u: d=%s n=%s m=%s\n      unicorn %s fpsr %02llx\n      library %s fpsr %02x\n", c.name.c_str(), c.word, c.ew, hex(d).c_str(), hex(n).c_str(), hex(m).c_str(),
                        hex(got).c_str(), static_cast<unsigned long long>(gotFpsr & 0x1f), hex(want).c_str(), wantFpsr);
    }
}

std::vector<A64FpCase> a64FpCases() {
    std::vector<A64FpCase> cases;
    using Model = std::function<Bytes(const Bytes&, const Bytes&, const Bytes&, unsigned&)>;
    vec::FpEnv arm;
    arm.nan = vec::NanRule::Arm;
    arm.minmax = vec::MinMax::ArmPropagate;
    vec::FpEnv armNum = arm;
    armNum.minmax = vec::MinMax::ArmNumber;
    for (unsigned sz = 0; sz < 2; ++sz) {
        const unsigned ew = sz ? 8u : 4u;
        const std::uint32_t szBit = sz << 22;
        const auto three = [&](const char* name, std::uint32_t base, Model m) { cases.push_back({name, base | szBit | (2u << 16) | (1u << 5), ew, std::move(m)}); };
        const auto two = [&](const char* name, std::uint32_t base, Model m, bool ints = false) { cases.push_back({name, base | szBit | (1u << 5), ew, std::move(m), ints}); };
        const auto op2 = [ew](vec::Fp2 op, vec::FpEnv env) {
            return Model([ew, op, env](const Bytes&, const Bytes& n, const Bytes& m, unsigned& f) { Bytes o(16); vec::fp2(o.data(), n.data(), m.data(), 16, ew, op, env, f); return o; });
        };
        three("fadd", 0x4e20d400, op2(vec::Fp2::Add, arm));
        three("fsub", 0x4ea0d400, op2(vec::Fp2::Sub, arm));
        three("fmul", 0x6e20dc00, op2(vec::Fp2::Mul, arm));
        three("fmulx", 0x4e20dc00, op2(vec::Fp2::MulExtended, arm));
        three("fdiv", 0x6e20fc00, op2(vec::Fp2::Div, arm));
        three("fmax", 0x4e20f400, op2(vec::Fp2::Max, arm));
        three("fmin", 0x4ea0f400, op2(vec::Fp2::Min, arm));
        three("fmaxnm", 0x4e20c400, op2(vec::Fp2::Max, armNum));
        three("fminnm", 0x4ea0c400, op2(vec::Fp2::Min, armNum));
        three("fabd", 0x6ea0d400, op2(vec::Fp2::AbsDiff, arm));
        three("fmla", 0x4e20cc00, Model([ew, arm](const Bytes& d, const Bytes& n, const Bytes& m, unsigned& f) { Bytes o(16); vec::fp3(o.data(), n.data(), m.data(), d.data(), 16, ew, vec::Fp3::MulAdd, arm, f); return o; }));
        three("fmls", 0x4ea0cc00, Model([ew, arm](const Bytes& d, const Bytes& n, const Bytes& m, unsigned& f) {   // (the first product operand is negated first: a NaN keeps the flipped sign)
                  Bytes neg(16), o(16);
                  vec::fp1(neg.data(), n.data(), 16, ew, vec::Fp1::Neg, arm, f);
                  vec::fp3(o.data(), neg.data(), m.data(), d.data(), 16, ew, vec::Fp3::MulAdd, arm, f);
                  return o;
              }));
        const struct { const char* name; std::uint32_t base; vec::FpCmp cmp; bool swap; } cmps[] = {{"fcmeq", 0x4e20e400, vec::FpCmp::Eq, false}, {"fcmge", 0x6e20e400, vec::FpCmp::Ge, false},
                                                                                                    {"fcmgt", 0x6ea0e400, vec::FpCmp::Gt, false}};
        for (const auto& c : cmps)
            three(c.name, c.base, Model([ew, c, arm](const Bytes&, const Bytes& n, const Bytes& m, unsigned& f) {
                      Bytes o(16);
                      vec::fpCompare(o.data(), n.data(), m.data(), 16, ew, c.cmp, c.cmp != vec::FpCmp::Eq, arm, f);
                      return o;
                  }));
        const auto op1 = [ew](vec::Fp1 op, vec::FpEnv env) {
            return Model([ew, op, env](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) { Bytes o(16); vec::fp1(o.data(), n.data(), 16, ew, op, env, f); return o; });
        };
        two("fsqrt", 0x6ea1f800, op1(vec::Fp1::Sqrt, arm));
        two("fabs", 0x4ea0f800, op1(vec::Fp1::Abs, arm));
        two("fneg", 0x6ea0f800, op1(vec::Fp1::Neg, arm));
        const struct { const char* name; std::uint32_t base; vec::Round mode; vec::Fp1 op; } rints[] = {
            {"frintn", 0x4e218800, vec::Round::NearestEven, vec::Fp1::RoundToInt},   {"frintm", 0x4e219800, vec::Round::TowardNegative, vec::Fp1::RoundToInt},
            {"frintp", 0x4ea18800, vec::Round::TowardPositive, vec::Fp1::RoundToInt}, {"frintz", 0x4ea19800, vec::Round::TowardZero, vec::Fp1::RoundToInt},
            {"frinta", 0x6e218800, vec::Round::NearestAway, vec::Fp1::RoundToInt},   {"frintx", 0x6e219800, vec::Round::NearestEven, vec::Fp1::RoundToIntExact},
            {"frinti", 0x6ea19800, vec::Round::NearestEven, vec::Fp1::RoundToInt}};
        for (const auto& r : rints) {
            vec::FpEnv env = arm;
            env.round = r.mode;
            two(r.name, r.base, op1(r.op, env));
        }
        const struct { const char* name; std::uint32_t base; vec::Round mode; bool isSigned; } cvts[] = {
            {"fcvtns", 0x4e21a800, vec::Round::NearestEven, true},   {"fcvtnu", 0x6e21a800, vec::Round::NearestEven, false}, {"fcvtms", 0x4e21b800, vec::Round::TowardNegative, true},
            {"fcvtmu", 0x6e21b800, vec::Round::TowardNegative, false}, {"fcvtas", 0x4e21c800, vec::Round::NearestAway, true},  {"fcvtau", 0x6e21c800, vec::Round::NearestAway, false},
            {"fcvtps", 0x4ea1a800, vec::Round::TowardPositive, true}, {"fcvtpu", 0x6ea1a800, vec::Round::TowardPositive, false}, {"fcvtzs", 0x4ea1b800, vec::Round::TowardZero, true},
            {"fcvtzu", 0x6ea1b800, vec::Round::TowardZero, false}};
        for (const auto& v : cvts) {
            vec::FpEnv env = arm;
            env.round = v.mode;
            two(v.name, v.base, Model([ew, env, v](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) {
                    Bytes o(16);
                    vec::floatToInt(o.data(), n.data(), 16, ew, ew, v.isSigned ? Sign::Signed : Sign::Unsigned, 0, vec::IntOverflow::Saturate, env, f);
                    return o;
                }));
        }
        two("scvtf", 0x4e21d800, Model([ew, arm](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) { Bytes o(16); vec::intToFloat(o.data(), n.data(), 16, ew, ew, Sign::Signed, 0, arm, f); return o; }), true);
        two("ucvtf", 0x6e21d800, Model([ew, arm](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) { Bytes o(16); vec::intToFloat(o.data(), n.data(), 16, ew, ew, Sign::Unsigned, 0, arm, f); return o; }), true);
    }
    // FCVTL (Q = 0: the low half of the source widened) and FCVTN (the destination's low half): half <-> single (sz 0) and single <-> double (sz 1)
    for (unsigned sz = 0; sz < 2; ++sz) {
        const unsigned narrowEw = sz ? 4u : 2u, wideEw = sz ? 8u : 4u;
                cases.push_back({"fcvtl", 0x0e217800u | (sz << 22) | (1u << 5), narrowEw, [=](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) {
                             Bytes o(16);
                             vec::floatConvert(o.data(), n.data(), 8, narrowEw, wideEw, arm, f);
                             return o;
                         }});
        cases.push_back({"fcvtn", 0x0e216800u | (sz << 22) | (1u << 5), wideEw, [=](const Bytes&, const Bytes& n, const Bytes&, unsigned& f) {
                             Bytes o(16, 0);
                             vec::floatConvert(o.data(), n.data(), 16, wideEw, narrowEw, arm, f);
                             return o;
                         }});
    }
    return cases;
}

}  // namespace

CB_TEST("engine.pcode_vector.aarch64_float_simd_matches_unicorn", [] {
    g_fpFails = 0;
    Engine e(UC_ARCH_ARM64, UC_MODE_ARM);
    std::uint64_t cpacr = 3u << 20;
    emu::unicorn::api().uc_reg_write(e.uc, UC_ARM64_REG_CPACR_EL1, &cpacr);
    const std::vector<A64FpCase> cases = a64FpCases();
    for (const A64FpCase& c : cases) runA64Fp(e, c, 300);
    std::printf("  aarch64 float: %zu encodings run on Unicorn, %u differences\n", cases.size(), g_fpFails);
    CB_CHECK_EQ(g_fpFails, 0u);
});

namespace {

// ---- x86-64 SSE: xmm0 is the destination and a source, xmm1 the other source
struct X86Case {
    std::string name;
    Bytes code;   // the instruction's bytes (with ModRM C1: xmm0, xmm1)
    unsigned ew;
    std::function<Bytes(const Bytes& dst, const Bytes& src)> model;
};
unsigned g_x86Missing = 0;

void runX86(Engine& e, const X86Case& c, int iterations) {
    const auto& u = emu::unicorn::api();
    emu::unicorn::check(u.uc_mem_write(e.uc, 0x10000, c.code.data(), c.code.size()), "uc_mem_write");
    for (int iter = 0; iter < iterations; ++iter) {
        const Bytes d = operand(c.ew), s = operand(c.ew);
        emu::unicorn::check(u.uc_reg_write(e.uc, UC_X86_REG_XMM0, d.data()), "uc_reg_write");
        emu::unicorn::check(u.uc_reg_write(e.uc, UC_X86_REG_XMM1, s.data()), "uc_reg_write");
        std::uint64_t rip = 0x10000;
        u.uc_reg_write(e.uc, UC_X86_REG_RIP, &rip);
        const uc_err err = u.uc_emu_start(e.uc, 0x10000, 0x10000 + c.code.size(), 0, 1);
        if (err != UC_ERR_OK) {
            if (++g_x86Missing <= 100) std::printf("  %s: Unicorn error %d\n", c.name.c_str(), static_cast<int>(err));
            return;
        }
        Bytes got(16), want = c.model(d, s);
        emu::unicorn::check(u.uc_reg_read(e.uc, UC_X86_REG_XMM0, got.data()), "uc_reg_read");
        if (got != want && ++g_fails <= 25)
            std::printf("  %s ew=%u: dst=%s src=%s\n      unicorn %s\n      library %s\n", c.name.c_str(), c.ew, hex(d).c_str(), hex(s).c_str(), hex(got).c_str(), hex(want).c_str());
    }
}

std::vector<X86Case> x86Cases() {
    std::vector<X86Case> cases;
    using M = std::function<Bytes(const Bytes&, const Bytes&)>;
    const auto add = [&](const char* name, std::initializer_list<unsigned> op, unsigned ew, M model) {
        Bytes code = {0x66};
        for (unsigned b : op) code.push_back(static_cast<Byte>(b));
        code.push_back(0xc1);
        cases.push_back({name, code, ew, std::move(model)});
    };
    const auto lane2 = [](auto fn) {
        return M([fn](const Bytes& d, const Bytes& s) {
            Bytes o(16);
            fn(o.data(), d.data(), s.data());
            return o;
        });
    };
    add("paddb", {0x0f, 0xfc}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::add(o, a, b, 16, 1); }));
    add("paddw", {0x0f, 0xfd}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::add(o, a, b, 16, 2); }));
    add("paddd", {0x0f, 0xfe}, 4, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::add(o, a, b, 16, 4); }));
    add("paddq", {0x0f, 0xd4}, 8, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::add(o, a, b, 16, 8); }));
    add("psubb", {0x0f, 0xf8}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::sub(o, a, b, 16, 1); }));
    add("psubq", {0x0f, 0xfb}, 8, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::sub(o, a, b, 16, 8); }));
    add("paddsb", {0x0f, 0xec}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::addSat(o, a, b, 16, 1, Sign::Signed); }));
    add("paddsw", {0x0f, 0xed}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::addSat(o, a, b, 16, 2, Sign::Signed); }));
    add("paddusb", {0x0f, 0xdc}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::addSat(o, a, b, 16, 1, Sign::Unsigned); }));
    add("paddusw", {0x0f, 0xdd}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::addSat(o, a, b, 16, 2, Sign::Unsigned); }));
    add("psubsb", {0x0f, 0xe8}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::subSat(o, a, b, 16, 1, Sign::Signed); }));
    add("psubsw", {0x0f, 0xe9}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::subSat(o, a, b, 16, 2, Sign::Signed); }));
    add("psubusb", {0x0f, 0xd8}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::subSat(o, a, b, 16, 1, Sign::Unsigned); }));
    add("psubusw", {0x0f, 0xd9}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::subSat(o, a, b, 16, 2, Sign::Unsigned); }));
    add("pmullw", {0x0f, 0xd5}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::mul(o, a, b, 16, 2); }));
    add("pmulhw", {0x0f, 0xe5}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::mulHigh(o, a, b, 16, 2, Sign::Signed); }));
    add("pmulhuw", {0x0f, 0xe4}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::mulHigh(o, a, b, 16, 2, Sign::Unsigned); }));
    add("pmuludq", {0x0f, 0xf4}, 4, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::mulEvenOdd(o, a, b, 16, 4, Sign::Unsigned, false); }));
    add("pmaddwd", {0x0f, 0xf5}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::multiplyAdd(o, nullptr, a, b, 16, 2, 4, Sign::Signed, Sign::Signed, false); }));
    add("psadbw", {0x0f, 0xf6}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::sumAbsDiff(o, a, b, 16); }));
    add("pavgb", {0x0f, 0xe0}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::halvingAdd(o, a, b, 16, 1, Sign::Unsigned, true); }));
    add("pavgw", {0x0f, 0xe3}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::halvingAdd(o, a, b, 16, 2, Sign::Unsigned, true); }));
    add("pminub", {0x0f, 0xda}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::min(o, a, b, 16, 1, Sign::Unsigned); }));
    add("pmaxub", {0x0f, 0xde}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::max(o, a, b, 16, 1, Sign::Unsigned); }));
    add("pminsw", {0x0f, 0xea}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::min(o, a, b, 16, 2, Sign::Signed); }));
    add("pmaxsw", {0x0f, 0xee}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::max(o, a, b, 16, 2, Sign::Signed); }));
    for (unsigned k = 0; k < 3; ++k) {
        const unsigned ew = 1u << k;
        add("pcmpeq", {0x0f, 0x74 + k}, ew, lane2([ew](Byte* o, const Byte* a, const Byte* b) { vec::compare(o, a, b, 16, ew, vec::Cmp::Eq, Sign::Unsigned); }));
        add("pcmpgt", {0x0f, 0x64 + k}, ew, lane2([ew](Byte* o, const Byte* a, const Byte* b) { vec::compare(o, a, b, 16, ew, vec::Cmp::Gt, Sign::Signed); }));
    }
    const auto pack = [&](const char* name, unsigned op, unsigned ewIn, vec::Narrow mode) {
        add(name, {0x0f, op}, ewIn, lane2([ewIn, mode](Byte* o, const Byte* a, const Byte* b) {
                Byte t[16];
                vec::narrow(t, a, 16, ewIn, mode);
                vec::narrow(t + 8, b, 16, ewIn, mode);
                std::memcpy(o, t, 16);
            }));
    };
    pack("packsswb", 0x63, 2, vec::Narrow::SignedToSigned);
    pack("packuswb", 0x67, 2, vec::Narrow::SignedToUnsigned);
    pack("packssdw", 0x6b, 4, vec::Narrow::SignedToSigned);
    struct Unpack {
        const char* n;
        unsigned lo, hi, ew;
    };
    const Unpack unp[] = {{"punpckbw", 0x60, 0x68, 1}, {"punpckwd", 0x61, 0x69, 2}, {"punpckdq", 0x62, 0x6a, 4}, {"punpckqdq", 0x6c, 0x6d, 8}};
    for (const Unpack& u : unp) {
        add(u.n, {0x0f, u.lo}, u.ew, lane2([u](Byte* o, const Byte* a, const Byte* b) { vec::zip(o, a, b, 16, u.ew, false); }));
        add(u.n, {0x0f, u.hi}, u.ew, lane2([u](Byte* o, const Byte* a, const Byte* b) { vec::zip(o, a, b, 16, u.ew, true); }));
    }
    struct ShiftOp {
        const char* n;
        unsigned op, ew;
        vec::Shift sh;
    };
    const ShiftOp shifts[] = {{"psllw", 0xf1, 2, vec::Shift::Left},           {"pslld", 0xf2, 4, vec::Shift::Left},           {"psllq", 0xf3, 8, vec::Shift::Left},
                              {"psrlw", 0xd1, 2, vec::Shift::RightLogical},   {"psrld", 0xd2, 4, vec::Shift::RightLogical},   {"psrlq", 0xd3, 8, vec::Shift::RightLogical},
                              {"psraw", 0xe1, 2, vec::Shift::RightArithmetic}, {"psrad", 0xe2, 4, vec::Shift::RightArithmetic}};
    for (const ShiftOp& sh : shifts)   // the count is the low 64 bits of the source register
        add(sh.n, {0x0f, sh.op}, sh.ew, M([sh](const Bytes& d, const Bytes& s) {
                Bytes o(16);
                vec::shiftImm(o.data(), d.data(), 16, sh.ew, sh.sh, vec::load(s.data(), 8), vec::Count::Saturate);
                return o;
            }));
    // SSSE3
    add("pshufb", {0x0f, 0x38, 0x00}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) {
            vec::Lookup mode;
            mode.indexMask = 0x0f;
            mode.bit7Zero = true;
            vec::tableLookup(o, a, 16, b, 16, mode);
        }));
    add("phaddw", {0x0f, 0x38, 0x01}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::pairwise(o, a, b, 16, 2, vec::Pair::Add); }));
    add("phaddd", {0x0f, 0x38, 0x02}, 4, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::pairwise(o, a, b, 16, 4, vec::Pair::Add); }));
    add("phaddsw", {0x0f, 0x38, 0x03}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) {
            Byte ev[16], od[16];
            vec::unzip(ev, a, b, 16, 2, false);
            vec::unzip(od, a, b, 16, 2, true);
            vec::addSat(o, ev, od, 16, 2, Sign::Signed);
        }));
    add("phsubw", {0x0f, 0x38, 0x05}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) {
            Byte ev[16], od[16];
            vec::unzip(ev, a, b, 16, 2, false);
            vec::unzip(od, a, b, 16, 2, true);
            vec::sub(o, ev, od, 16, 2);
        }));
    add("pmaddubsw", {0x0f, 0x38, 0x04}, 1, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::multiplyAdd(o, nullptr, a, b, 16, 1, 2, Sign::Unsigned, Sign::Signed, true); }));
    add("pmulhrsw", {0x0f, 0x38, 0x0b}, 2, lane2([](Byte* o, const Byte* a, const Byte* b) { vec::mulHighRoundHalfUp(o, a, b, 16, 2); }));
    for (unsigned k = 0; k < 3; ++k) {
        const unsigned ew = 1u << k;
        add("pabs", {0x0f, 0x38, 0x1c + k}, ew, M([ew](const Bytes&, const Bytes& s) {
                Bytes o(16);
                vec::abs(o.data(), s.data(), 16, ew, false);
                return o;
            }));
    }
    for (unsigned imm : {1u, 4u, 8u, 15u}) {
        const Bytes code = {0x66, 0x0f, 0x3a, 0x0f, 0xc1, static_cast<Byte>(imm)};
        cases.push_back({"palignr", code, 1, M([imm](const Bytes& d, const Bytes& s) {
                             Bytes o(16);
                             vec::extract(o.data(), s.data(), d.data(), 16, imm);
                             return o;
                         })});
    }
    return cases;
}

}  // namespace

CB_TEST("engine.pcode_vector.x86_sse_integer_simd_matches_unicorn", [] {
    g_fails = 0;
    g_x86Missing = 0;
    Engine e(UC_ARCH_X86, UC_MODE_64);
    const std::vector<X86Case> cases = x86Cases();
    for (const X86Case& c : cases) runX86(e, c, 40);
    std::printf("  x86-64: %zu encodings run on Unicorn, %u not executed by it, %u differences\n", cases.size(), g_x86Missing, g_fails);
    CB_CHECK_EQ(g_fails, 0u);
    CB_CHECK_EQ(g_x86Missing, 0u);
});

}  // namespace cb
