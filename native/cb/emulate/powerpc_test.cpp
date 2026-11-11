// cb/emulate/powerpc_test.cpp -- the PowerPC interpreter: integer, rotate, compare, branch, load/store and floating
// instructions, memory (pages, limits, shared images, fills), running a function (externs, absent code, the branch log,
// a shadow) and the run record.
//
// Instruction words are encoded by hand below and every expected value is worked out by hand from the Power ISA (not
// recorded from this code). The old harness (state/work/typed-port/emulate.powerpc: random single steps and programs made
// by the Python module) is the reference for the rest.
//
// DIVERGES from the Python module:
//  - filled(kind, size) gives exactly `size` bytes; the Python gave whole words (size 5 gave 8 bytes).
//  - A load that straddles 4 GB reports its "where" address wrapped to 32 bits (the Python gave a number past 2**32).
//  - A run's path is in order of first arrival (the Python held a set).
//  - Stop is cb::emulate::powerpc::Fault carrying a StopReason; a Machine that cannot hold its regions throws it from the
//    constructor, as the Python raised Stop there.
#include "cb/emulate/powerpc.hpp"

#include <bit>
#include <cmath>

#include "cb/core/testing.hpp"

namespace p = cb::emulate::powerpc;
using cb::Bytes;

namespace {

std::uint32_t D(unsigned op, unsigned rt, unsigned ra, std::int32_t imm) {
    return op << 26 | rt << 21 | ra << 16 | (static_cast<std::uint32_t>(imm) & 0xFFFF);
}
std::uint32_t X(unsigned op, unsigned rt, unsigned ra, unsigned rb, unsigned xo, unsigned rc = 0) {
    return op << 26 | rt << 21 | ra << 16 | rb << 11 | xo << 1 | rc;
}
std::uint32_t B(std::int32_t li, bool lk = false) { return 18u << 26 | (static_cast<std::uint32_t>(li) & 0x03FFFFFC) | (lk ? 1u : 0u); }
std::uint32_t BC(unsigned bo, unsigned bi, std::int32_t bd) { return 16u << 26 | bo << 21 | bi << 16 | (static_cast<std::uint32_t>(bd) & 0xFFFC); }
constexpr std::uint32_t kBlr = 0x4E800020;
std::uint32_t LI(unsigned r, std::int32_t v) { return D(14, r, 0, v); }
std::uint32_t MTCTR(unsigned r) { return X(31, r, 9, 0, 467); }

p::Region code(std::uint32_t at, std::initializer_list<std::uint32_t> words) {
    p::Region r;
    r.address = at;
    for (std::uint32_t w : words) {
        for (int s = 24; s >= 0; s -= 8) r.data.push_back(static_cast<std::uint8_t>(w >> s));
    }
    return r;
}

constexpr std::uint32_t kBase = 0x10000000;

// One instruction on a fresh machine with registers given; the machine afterwards.
struct One {
    p::Machine m;
    One() { m.pc = kBase; }
    void run(std::uint32_t w) { p::step(m, w); }
};

p::RunResult run_words(std::initializer_list<std::uint32_t> words, std::initializer_list<std::uint64_t> args = {},
                       const p::RunOptions& o = {}) {
    const p::Region r = code(kBase, words);
    const std::vector<std::uint64_t> a(args);
    return p::run(std::span<const p::Region>(&r, 1), kBase, a, o);
}

}  // namespace

CB_TEST("emulate.powerpc.helpers", [] {
    CB_CHECK_EQ(p::mask(0, 63), ~std::uint64_t{0});
    CB_CHECK_EQ(p::mask(56, 63), 0xFFu);
    CB_CHECK_EQ(p::mask(0, 7), 0xFF00000000000000ull);
    CB_CHECK_EQ(p::mask(60, 3), 0xF00000000000000Full);   // wraps when mb > me
    CB_CHECK_EQ(p::rotl64(0x8000000000000001ull, 1), 3u);
    CB_CHECK_EQ(p::rotl64(5, 64), 5u);
    CB_CHECK_EQ(p::rotl32(0x80000001ull, 1), 0x0000000300000003ull);   // the word's result in both halves
    CB_CHECK_EQ(p::sign_extend(0x8000, 16), 0xFFFFFFFFFFFF8000ull);
    CB_CHECK_EQ(p::sign_extend(0x7FFF, 16), 0x7FFFu);
    CB_CHECK_EQ(p::to_bits(1.0), 0x3FF0000000000000ull);
    CB_CHECK_EQ(p::to_bits(p::round_to_single(0.1)), p::to_bits(static_cast<double>(0.1f)));
    CB_CHECK(std::isinf(p::round_to_single(1e300)));
});

CB_TEST("emulate.powerpc.add_and_immediates", [] {
    One a;
    a.m.r[4] = 100;
    a.run(D(14, 3, 4, -1));                       // addi r3,r4,-1
    CB_CHECK_EQ(a.m.r[3], 99u);
    CB_CHECK_EQ(a.m.pc, kBase + 4);
    a.run(D(14, 5, 0, 0x1234));                   // addi r5,0,$1234: ra = 0 means the number 0
    CB_CHECK_EQ(a.m.r[5], 0x1234u);
    a.run(D(15, 6, 0, -2));                       // addis r6,0,-2: -2 << 16
    CB_CHECK_EQ(a.m.r[6], 0xFFFFFFFFFFFE0000ull);
    a.m.r[7] = ~0ull;
    a.m.r[8] = 1;
    a.run(X(31, 9, 7, 8, 266));                   // add r9,r7,r8: wraps at 64 bits
    CB_CHECK_EQ(a.m.r[9], 0u);
    a.run(X(31, 10, 8, 7, 40));                   // subf r10,r8,r7 = r7 - r8
    CB_CHECK_EQ(a.m.r[10], ~0ull - 1);
});

CB_TEST("emulate.powerpc.carry_chain", [] {
    One a;
    a.m.r[3] = ~0ull;
    a.m.r[4] = 1;
    a.run(X(31, 5, 3, 4, 10));                    // addc: carry out
    CB_CHECK_EQ(a.m.r[5], 0u);
    CB_CHECK(a.m.ca);
    a.m.r[6] = 7;
    a.run(X(31, 7, 6, 0, 202));                   // addze r7,r6: 7 + carry
    CB_CHECK_EQ(a.m.r[7], 8u);
    CB_CHECK(!a.m.ca);
    a.run(D(8, 8, 6, 5));                         // subfic r8,r6,5 = 5 - 7: borrow, so no carry
    CB_CHECK_EQ(a.m.r[8], ~1ull);
    CB_CHECK(!a.m.ca);
    a.run(D(12, 9, 6, -1));                       // addic r9,r6,-1: 7 + (2^64 - 1) carries
    CB_CHECK_EQ(a.m.r[9], 6u);
    CB_CHECK(a.m.ca);
});

CB_TEST("emulate.powerpc.multiply_and_divide", [] {
    One a;
    a.m.r[3] = 0xFFFFFFFFull;                     // -1 as a word
    a.m.r[4] = 5;
    a.run(X(31, 5, 3, 4, 235));                   // mullw: -5 sign-extended to 64 bits
    CB_CHECK_EQ(a.m.r[5], static_cast<std::uint64_t>(-5));
    a.run(X(31, 6, 3, 4, 75));                    // mulhw: the high word of -5 is -1
    CB_CHECK_EQ(a.m.r[6], 0xFFFFFFFFull);
    a.run(X(31, 7, 3, 4, 11));                    // mulhwu: 0xFFFFFFFF * 5 >> 32 = 4
    CB_CHECK_EQ(a.m.r[7], 4u);
    a.m.r[8] = static_cast<std::uint64_t>(-7);
    a.m.r[9] = 2;
    a.run(X(31, 10, 8, 9, 489));                  // divd r10 = r8 / r9 = -3 (toward zero)... (rt = ra / rb)
    CB_CHECK_EQ(a.m.r[10], static_cast<std::uint64_t>(-3));
    a.m.r[9] = 0;
    a.run(X(31, 10, 8, 9, 489));                  // a division by zero leaves 0
    CB_CHECK_EQ(a.m.r[10], 0u);
    a.m.r[11] = 0xFFFFFFFF80000000ull;            // INT_MIN / -1 as words
    a.m.r[12] = 0xFFFFFFFFull;
    a.run(X(31, 13, 11, 12, 491));
    CB_CHECK_EQ(a.m.r[13], 0u);
    a.m.r[3] = 1ull << 63;
    a.m.r[4] = 4;
    a.run(X(31, 5, 3, 4, 9));                     // mulhdu: 2^63 * 4 >> 64 = 2
    CB_CHECK_EQ(a.m.r[5], 2u);
    a.run(X(31, 5, 3, 4, 73));                    // mulhd: (-2^63) * 4 >> 64 = -2
    CB_CHECK_EQ(a.m.r[5], static_cast<std::uint64_t>(-2));
});

CB_TEST("emulate.powerpc.logical_and_record", [] {
    One a;
    a.m.r[3] = 0xF0F0;
    a.m.r[4] = 0x0FF0;
    a.run(X(31, 3, 5, 4, 28, 1));                 // and. r5,r3,r4 (rs = r3 in the rt field): 0x00F0, CR0 positive
    CB_CHECK_EQ(a.m.r[5], 0xF0u);
    CB_CHECK_EQ(a.m.cr >> 28, 4u);
    a.run(X(31, 3, 6, 4, 316));                   // xor
    CB_CHECK_EQ(a.m.r[6], 0xFF00u);
    a.run(X(31, 3, 7, 3, 124));                   // nor r7,r3,r3 = ~r3
    CB_CHECK_EQ(a.m.r[7], ~0xF0F0ull);
    a.run(D(25, 3, 8, 0x1));                      // oris r8,r3,1
    CB_CHECK_EQ(a.m.r[8], 0x1F0F0u);
    a.m.r[3] = 0;
    a.run(D(28, 3, 9, 0xFF));                     // andi. r9,r3,$FF = 0: CR0 equal
    CB_CHECK_EQ(a.m.cr >> 28, 2u);
    a.m.r[3] = 1ull << 63;
    a.run(X(31, 3, 10, 3, 28, 1));                // and. with a 64-bit negative: CR0 less than
    CB_CHECK_EQ(a.m.cr >> 28, 8u);
});

CB_TEST("emulate.powerpc.shifts_and_counts", [] {
    One a;
    a.m.r[3] = 0x80000001;
    a.m.r[4] = 4;
    a.run(X(31, 3, 5, 4, 24));                    // slw: (r3 << 4) as a word
    CB_CHECK_EQ(a.m.r[5], 0x10u);
    a.run(X(31, 3, 6, 4, 536));                   // srw
    CB_CHECK_EQ(a.m.r[6], 0x08000000u);
    a.run(X(31, 3, 7, 4, 792));                   // sraw: the word is negative, shift keeps the sign
    CB_CHECK_EQ(a.m.r[7], 0xFFFFFFFFF8000000ull);
    CB_CHECK(a.m.ca);                              // a one bit was shifted out of a negative number
    a.run(X(31, 3, 8, 3, 824));                    // srawi by the rb field = 3: shift by 3
    CB_CHECK_EQ(a.m.r[8], 0xFFFFFFFFF0000000ull);
    a.m.r[9] = 1;
    a.m.r[10] = 40;
    a.run(X(31, 9, 11, 10, 27));                   // sld r11 = r9 << 40
    CB_CHECK_EQ(a.m.r[11], 1ull << 40);
    a.m.r[10] = 70;
    a.run(X(31, 9, 11, 10, 27));                   // a count of 64 or more clears
    CB_CHECK_EQ(a.m.r[11], 0u);
    a.m.r[12] = 0x00FF000000000000ull;
    a.run(X(31, 12, 13, 0, 58));                   // cntlzd
    CB_CHECK_EQ(a.m.r[13], 8u);
    a.run(X(31, 12, 13, 0, 26));                   // cntlzw of the low word 0: 32
    CB_CHECK_EQ(a.m.r[13], 32u);
    a.m.r[14] = 0x80;
    a.run(X(31, 14, 15, 0, 954));                  // extsb
    CB_CHECK_EQ(a.m.r[15], 0xFFFFFFFFFFFFFF80ull);
});

CB_TEST("emulate.powerpc.rotates", [] {
    One a;
    a.m.r[3] = 0x12345678;
    // rlwinm r4,r3,8,0,31 (rotate left 8, whole word): mb = 0, me = 31, sh = 8
    a.run(21u << 26 | 3u << 21 | 4u << 16 | 8u << 11 | 0u << 6 | 31u << 1);
    CB_CHECK_EQ(a.m.r[4], 0x34567812u);
    // clrlwi r5,r3,16 = rlwinm r5,r3,0,16,31
    a.run(21u << 26 | 3u << 21 | 5u << 16 | 0u << 11 | 16u << 6 | 31u << 1);
    CB_CHECK_EQ(a.m.r[5], 0x5678u);
    // rldicl r6,r3,0,32 (clear the high 32 bits): op 30, mb = 32 (mb5 = 1, mb0-4 = 0), sh = 0
    a.m.r[3] = 0xAAAABBBB12345678ull;
    a.run(30u << 26 | 3u << 21 | 6u << 16 | 0u << 11 | 0u << 6 | 1u << 5 | 0u << 2);
    CB_CHECK_EQ(a.m.r[6], 0x12345678u);
    // rlwimi r7,r3,0,24,31: insert the low byte into r7
    a.m.r[7] = 0xFFFFFF00FFFFFF00ull;
    a.run(20u << 26 | 3u << 21 | 7u << 16 | 0u << 11 | 24u << 6 | 31u << 1);
    CB_CHECK_EQ(a.m.r[7], 0xFFFFFF00FFFFFF78ull);
});

CB_TEST("emulate.powerpc.compares_set_cr_fields", [] {
    One a;
    a.m.r[3] = static_cast<std::uint64_t>(-1);
    a.m.r[4] = 1;
    a.run(X(31, 0, 3, 4, 0));                     // cmpw cr0,r3,r4: signed words: -1 < 1
    CB_CHECK_EQ(a.m.cr >> 28, 8u);
    a.run(X(31, 0, 3, 4, 32));                    // cmplw: 0xFFFFFFFF > 1
    CB_CHECK_EQ(a.m.cr >> 28, 4u);
    a.run(X(31, 1 << 2, 4, 4, 0));                // cmpw cr1,r4,r4: equal in field 1
    CB_CHECK_EQ((a.m.cr >> 24) & 0xF, 2u);
    a.run(D(11, 2 << 2 | 1, 3, 0));               // cmpdi cr2,r3,0 (64-bit): less than
    CB_CHECK_EQ((a.m.cr >> 20) & 0xF, 8u);
    a.run(D(10, 3 << 2, 4, 1));                   // cmplwi cr3,r4,1: equal
    CB_CHECK_EQ((a.m.cr >> 16) & 0xF, 2u);
});

CB_TEST("emulate.powerpc.branches_and_link", [] {
    One a;
    a.run(B(0x40, true));                          // bl +0x40
    CB_CHECK_EQ(a.m.pc, kBase + 0x40);
    CB_CHECK_EQ(a.m.lr, kBase + 4);
    One b;
    b.m.ctr = 2;
    b.run(BC(16, 0, -8));                          // bdnz -8: ctr 2 -> 1, taken
    CB_CHECK_EQ(b.m.pc, kBase - 8);
    CB_CHECK_EQ(b.m.ctr, 1u);
    b.m.pc = kBase;
    b.run(BC(16, 0, -8));                          // ctr 1 -> 0: not taken
    CB_CHECK_EQ(b.m.pc, kBase + 4);
    One c;
    c.m.set_cr(0, 4);                              // greater than
    c.run(BC(12, 1, 16));                          // beq... no: bit 1 is "gt": branch if true: taken
    CB_CHECK_EQ(c.m.pc, kBase + 16);
    c.m.pc = kBase;
    c.run(BC(4, 1, 16));                           // branch if gt is false: not taken
    CB_CHECK_EQ(c.m.pc, kBase + 4);
    One d;
    d.m.lr = 0x20000003;
    d.run(kBlr);                                   // blr: the low two bits are dropped
    CB_CHECK_EQ(d.m.pc, 0x20000000u);
    One e;
    e.m.ctr = 0x30000000;
    e.run(0x4E800421);                             // bctrl
    CB_CHECK_EQ(e.m.pc, 0x30000000u);
    CB_CHECK_EQ(e.m.lr, kBase + 4);
});

CB_TEST("emulate.powerpc.loads_and_stores", [] {
    One a;
    a.m.r[3] = 0x60001000;
    a.m.r[4] = 0x1122334455667788ull;
    a.run(D(36, 4, 3, 0));                         // stw r4,0(r3)
    CB_CHECK_EQ(a.m.read(0x60001000, 4), 0x55667788u);
    a.run(D(38, 4, 3, 4));                         // stb
    CB_CHECK_EQ(a.m.read(0x60001004, 1), 0x88u);
    a.run(D(62, 4, 3, 8));                         // std r4,8(r3)
    CB_CHECK_EQ(a.m.read(0x60001008, 8), 0x1122334455667788ull);
    a.run(D(34, 5, 3, 4));                         // lbz
    CB_CHECK_EQ(a.m.r[5], 0x88u);
    a.run(D(42, 6, 3, 8));                         // lha: 0x1122 positive
    CB_CHECK_EQ(a.m.r[6], 0x1122u);
    a.run(D(40, 7, 3, 2));                         // lhz of 0x7788
    CB_CHECK_EQ(a.m.r[7], 0x7788u);
    a.run(D(58, 8, 3, 8));                         // ld
    CB_CHECK_EQ(a.m.r[8], 0x1122334455667788ull);
    a.run(D(58, 9, 3, 8 | 2));                     // lwa: sign-extended 0x11223344
    CB_CHECK_EQ(a.m.r[9], 0x11223344u);
    a.m.r[10] = 0x60002000;
    a.run(D(37, 4, 10, 16));                       // stwu: the base register is updated
    CB_CHECK_EQ(a.m.r[10], 0x60002010u);
    CB_CHECK_EQ(a.m.read(0x60002010, 4), 0x55667788u);
    a.m.r[11] = 0x60001000;
    a.m.r[12] = 0;
    a.run(X(31, 13, 11, 12, 534));                 // lwbrx r13,r11,r12: byte-reversed
    CB_CHECK_EQ(a.m.r[13], 0x88776655u);
    a.run(X(31, 4, 11, 12, 662));                  // stwbrx
    CB_CHECK_EQ(a.m.read(0x60001000, 4), 0x88776655u);
    // lmw / stmw
    a.m.r[29] = 1;
    a.m.r[30] = 2;
    a.m.r[31] = 3;
    a.run(D(47, 29, 3, 0x100));
    a.m.r[29] = a.m.r[30] = a.m.r[31] = 0;
    a.run(D(46, 29, 3, 0x100));
    CB_CHECK_EQ(a.m.r[31], 3u);
    CB_CHECK_EQ(a.m.r[29], 1u);
});

CB_TEST("emulate.powerpc.memory_is_big_endian_across_pages", [] {
    p::Machine m;
    m.write(0x60000FFE, 4, 0xAABBCCDD);
    CB_CHECK_EQ(m.read(0x60000FFE, 4), 0xAABBCCDDu);
    CB_CHECK_EQ(m.read(0x60000FFF, 1), 0xBBu);
    CB_CHECK_EQ(m.read(0x60001000, 2), 0xCCDDu);
    CB_CHECK_EQ(m.page_count(), 2u);
    CB_CHECK_EQ(m.writes().size(), 4u);
    // the stack is not the function's output
    m.write(p::kStackLow + 8, 4, 1);
    CB_CHECK_EQ(m.writes().size(), 4u);
});

CB_TEST("emulate.powerpc.memory_limit_and_shared_pages", [] {
    p::Machine m;
    for (std::uint32_t i = 0; i < p::kMaxPages; ++i) m.write(0x1000000 + i * 0x1000, 1, 1);
    CB_CHECK_EQ(m.page_count(), p::kMaxPages);
    try {
        (void)m.read(0x40000000, 4);
        CB_CHECK(false);
    } catch (const p::Fault& f) {
        CB_CHECK(f.reason() == p::StopReason::Memory);
    }
    // a shared image is read without being copied, and copied when written
    p::Region r;
    r.address = 0x50000000;
    r.data = {1, 2, 3, 4};
    const p::PageImage image = p::PageImage::of(std::span<const p::Region>(&r, 1));
    CB_CHECK_EQ(image.size(), 1u);
    p::Machine s({}, image);
    CB_CHECK_EQ(s.read(0x50000000, 4), 0x01020304u);
    CB_CHECK_EQ(s.page_count(), 0u);
    s.write(0x50000001, 1, 9);
    CB_CHECK_EQ(s.page_count(), 1u);
    CB_CHECK_EQ(s.read(0x50000000, 4), 0x01090304u);
    CB_CHECK_EQ(image.find(0x50000)->at(1), 2);        // the shared page is untouched
    CB_CHECK_EQ(s.writes().at(0x50000001).first, 2);   // the byte before is the shared one
    // memory nobody gave is zeros, and reading it creates the page
    p::Machine z;
    CB_CHECK_EQ(z.read(0x70000000, 8), 0u);
    CB_CHECK_EQ(z.page_count(), 1u);
});

CB_TEST("emulate.powerpc.fills", [] {
    const Bytes ptr = p::filled(p::Fill::Pointers, 8);
    CB_CHECK_EQ(ptr.size(), 8u);
    CB_CHECK_EQ(ptr[0], 0x60);
    CB_CHECK_EQ(ptr[2], 0x01);                          // 0x60000100
    CB_CHECK_EQ(ptr[7], 0x04);                          // 0x60000104
    const Bytes counts = p::filled(p::Fill::Counts, 8);
    CB_CHECK_EQ(counts[3], 1);
    CB_CHECK_EQ(counts[7], 2);
    const Bytes text = p::filled(p::Fill::Text, 48);
    CB_CHECK_EQ(text[0], 'A');
    CB_CHECK_EQ(text[22], 'A' + 22);
    CB_CHECK_EQ(text[23], 0);
    CB_CHECK_EQ(text[24], 'A');
    CB_CHECK(p::filled(p::Fill::None).empty());
    CB_CHECK(p::fill_from_text("counts") == p::Fill::Counts);
    CB_CHECK(!p::fill_from_text("bogus").has_value());
    // a run with a fill reads it behind a pointer
    p::RunOptions o;
    o.fill = p::Fill::Counts;
    const auto r = run_words({D(32, 3, 3, 4), kBlr}, {p::kArgsBase}, o);   // lwz r3,4(r3): the second word is 2
    CB_CHECK_EQ(r.ret, 2u);
});

CB_TEST("emulate.powerpc.floating_point", [] {
    One a;
    a.m.f[1] = p::to_bits(1.5);
    a.m.f[2] = p::to_bits(2.25);
    a.run(63u << 26 | 3u << 21 | 1u << 16 | 2u << 11 | 21u << 1);   // fadd f3,f1,f2
    CB_CHECK_EQ(p::to_double(a.m.f[3]), 3.75);
    a.run(63u << 26 | 3u << 21 | 1u << 16 | 2u << 11 | 20u << 1);   // fsub
    CB_CHECK_EQ(p::to_double(a.m.f[3]), -0.75);
    a.run(63u << 26 | 3u << 21 | 1u << 16 | 0u << 11 | 2u << 6 | 25u << 1);   // fmul f3,f1,f2 (the c field is bits 6-10)
    CB_CHECK_EQ(p::to_double(a.m.f[3]), 3.375);
    a.run(63u << 26 | 3u << 21 | 1u << 16 | 2u << 11 | 18u << 1);   // fdiv
    CB_CHECK_EQ(p::to_double(a.m.f[3]), 1.5 / 2.25);
    a.m.f[4] = p::to_bits(0.0);
    a.run(63u << 26 | 3u << 21 | 1u << 16 | 4u << 11 | 18u << 1);   // fdiv by zero: +infinity
    CB_CHECK(std::isinf(p::to_double(a.m.f[3])));
    a.run(59u << 26 | 3u << 21 | 1u << 16 | 2u << 11 | 21u << 1);   // fadds: rounded to single
    CB_CHECK_EQ(p::to_double(a.m.f[3]), 3.75);
    a.run(63u << 26 | 3u << 21 | 0u << 16 | 1u << 11 | 40u << 1);   // fneg f3,f1
    CB_CHECK_EQ(p::to_double(a.m.f[3]), -1.5);
    a.run(63u << 26 | 3u << 21 | 0u << 16 | 3u << 11 | 264u << 1);  // fabs f3,f3
    CB_CHECK_EQ(p::to_double(a.m.f[3]), 1.5);
    a.run(63u << 26 | 3u << 21 | 0u << 16 | 1u << 11 | 815u << 1);  // fctidz f3,f1: 1.5 toward zero
    CB_CHECK_EQ(a.m.f[3], 1u);
    a.run(63u << 26 | 3u << 21 | 0u << 16 | 1u << 11 | 814u << 1);  // fctid: 1.5 to even = 2
    CB_CHECK_EQ(a.m.f[3], 2u);
    a.m.f[5] = p::to_bits(std::nan(""));
    a.run(63u << 26 | 3u << 21 | 0u << 16 | 5u << 11 | 15u << 1);   // fctiwz of NaN: the most negative word
    CB_CHECK_EQ(a.m.f[3], 0x80000000u);
    a.run(63u << 26 | 0u << 21 | 1u << 16 | 2u << 11 | 0u << 1);    // fcmpu cr0,f1,f2: less than
    CB_CHECK_EQ(a.m.cr >> 28, 8u);
    a.run(63u << 26 | 0u << 21 | 1u << 16 | 5u << 11 | 0u << 1);    // against a NaN: unordered
    CB_CHECK_EQ(a.m.cr >> 28, 1u);
});

CB_TEST("emulate.powerpc.floating_loads_and_stores", [] {
    One a;
    a.m.r[3] = 0x60003000;
    a.m.f[1] = p::to_bits(0.1);
    a.run(D(54, 1, 3, 0));                          // stfd
    CB_CHECK_EQ(a.m.read(0x60003000, 8), p::to_bits(0.1));
    a.run(D(52, 1, 3, 16));                         // stfs: rounded to single
    CB_CHECK_EQ(a.m.read(0x60003010, 4), std::bit_cast<std::uint32_t>(0.1f));
    a.run(D(48, 2, 3, 16));                         // lfs: widened
    CB_CHECK_EQ(p::to_double(a.m.f[2]), static_cast<double>(0.1f));
    a.run(D(50, 3, 3, 0));                          // lfd
    CB_CHECK_EQ(a.m.f[3], p::to_bits(0.1));
});

CB_TEST("emulate.powerpc.special_registers", [] {
    One a;
    a.m.r[3] = 0x1234;
    a.run(X(31, 3, 8, 0, 467));                     // mtlr r3
    CB_CHECK_EQ(a.m.lr, 0x1234u);
    a.run(X(31, 4, 8, 0, 339));                     // mflr r4
    CB_CHECK_EQ(a.m.r[4], 0x1234u);
    a.m.r[5] = ~0ull;
    a.run(X(31, 5, 9, 0, 467));                     // mtctr: 64 bits
    CB_CHECK_EQ(a.m.ctr, ~0ull);
    a.m.cr = 0xA5A5A5A5;
    a.run(X(31, 6, 0, 0, 19));                      // mfcr
    CB_CHECK_EQ(a.m.r[6], 0xA5A5A5A5u);
    a.m.r[7] = 0xFFFFFFFF;
    a.run(X(31, 7, 0x80 >> 7 << 0, 0, 144) | (0x01u << 12));   // mtcrf with the last field only
    CB_CHECK_EQ(a.m.cr & 0xF, 0xFu);
});

CB_TEST("emulate.powerpc.faults", [] {
    const auto stops = [](std::uint32_t w, p::StopReason why) {
        One a;
        try {
            a.run(w);
            return false;
        } catch (const p::Fault& f) {
            return f.reason() == why;
        }
    };
    CB_CHECK(stops(0x44000002, p::StopReason::Syscall));                // sc
    CB_CHECK(stops(0x10000000, p::StopReason::Unsupported));            // a vector instruction
    CB_CHECK(stops(D(3, 31, 0, 0), p::StopReason::Trap));               // twi 31,r0,0: always
    One a;
    a.run(D(3, 0, 0, 0));                                                // twi 0: never
    CB_CHECK_EQ(a.m.pc, kBase + 4);
    One b;
    try {
        b.run(0x44000002);
    } catch (const p::Fault&) {
    }
    CB_CHECK_EQ(b.m.pc, kBase);                                          // a fault leaves pc at the instruction
    CB_CHECK_EQ(std::string(p::to_text(p::StopReason::Memory)), "memory");
});

// ---- running a function ------------------------------------------------------------------------------------------

CB_TEST("emulate.powerpc.run_returns_and_counts", [] {
    const auto r = run_words({X(31, 3, 3, 4, 266), kBlr}, {40, 2});   // add r3,r3,r4 ; blr
    CB_CHECK(r.exit == p::Exit::Ret);
    CB_CHECK_EQ(r.exit_text(), "ret");
    CB_CHECK_EQ(r.ret, 42u);
    CB_CHECK_EQ(r.steps, 2u);
    CB_CHECK(r.saved);
    CB_CHECK(!r.truncated);
    CB_CHECK(r.writes.empty());
    CB_CHECK(r.machine != nullptr);
});

CB_TEST("emulate.powerpc.run_limit", [] {
    p::RunOptions o;
    o.max_steps = 10;
    const auto r = run_words({B(0)}, {}, o);   // b . : forever
    CB_CHECK(r.exit == p::Exit::None);
    CB_CHECK(r.truncated);
    CB_CHECK_EQ(r.steps, 10u);
    CB_CHECK_EQ(r.exit_text(), "none");
});

CB_TEST("emulate.powerpc.run_stops_with_a_reason", [] {
    const auto invalid = run_words({LI(3, 1)});                                // runs off the end into zeros
    CB_CHECK_EQ(invalid.exit_text(), "exception:invalid");
    const auto sc = run_words({0x44000002});
    CB_CHECK_EQ(sc.exit_text(), "exception:syscall");
    const auto aligned = run_words({LI(3, 0x1001), MTCTR(3), 0x4E800420});     // bctr to an odd address (the low bits drop)
    CB_CHECK_EQ(aligned.exit_text(), "exception:invalid");
    const auto vector = run_words({0x10000000});
    CB_CHECK_EQ(vector.exit_text(), "exception:unsupported");
    CB_CHECK(vector.stopped == p::StopReason::Unsupported);
});

CB_TEST("emulate.powerpc.run_registers_at_entry_and_saved", [] {
    const auto good = run_words({D(37, 1, 1, -16), D(14, 1, 1, 16), kBlr});    // stwu r1 ... addi r1: the stack pointer comes back
    CB_CHECK(good.saved);
    const auto bad = run_words({LI(14, 5), kBlr});                              // clobbers r14
    CB_CHECK(!bad.saved);
    const auto stack = run_words({D(37, 1, 1, -16), D(14, 1, 1, 16), kBlr});
    CB_CHECK(stack.writes.empty());                                             // stack stores are not output
    const auto r = run_words({D(14, 3, 13, 0), kBlr});                          // r13: the per-thread data pointer
    CB_CHECK_EQ(r.ret, 0x7E000000u);
});

CB_TEST("emulate.powerpc.run_records_output_writes", [] {
    const auto r = run_words({D(15, 4, 0, 0x6000), D(14, 5, 0, 0x55), D(38, 5, 4, 3), D(38, 0, 4, 9), kBlr});
    // stb r5,3(r4) writes 0x55 at 0x60000003; stb r0,9(r4): r0 holds 0 and the byte was 0: no change, not reported
    CB_CHECK_EQ(r.writes.size(), 1u);
    CB_CHECK_EQ(r.writes[0].address, 0x60000003u);
    CB_CHECK_EQ(r.writes[0].before, 0);
    CB_CHECK_EQ(r.writes[0].after, 0x55);
});

CB_TEST("emulate.powerpc.run_calls_out_through_externs", [] {
    p::ExternTable ext{{0x10010000, "alpha"}, {0x10010040, "beta"}};
    p::RunOptions o;
    o.externs = &ext;
    o.stub_ret = 0x1111;
    // r3 = 7 ; bl alpha ; bl beta ; blr -- but blr must find lr: save it in r31 across the calls
    const auto r = run_words({X(31, 31, 8, 0, 339), LI(3, 7), B(0x10010000 - (kBase + 8), true), B(0x10010040 - (kBase + 12), true),
                              X(31, 31, 8, 0, 467), kBlr},
                             {}, o);
    CB_CHECK_EQ(r.exit_text(), "ret");
    CB_CHECK_EQ(r.calls.size(), 2u);
    CB_CHECK_EQ(r.calls[0].name, "alpha");
    CB_CHECK_EQ(r.calls[0].args[0], 7u);                 // r3 at the call
    CB_CHECK_EQ(r.calls[1].name, "beta");
    CB_CHECK_EQ(r.calls[1].args[0], 0x1111u);            // the stand-in's answer, seen by the next call
    CB_CHECK_EQ(r.ret, 0x1111u);
    CB_CHECK_EQ(r.steps, 6u);                            // the calls themselves are not steps
});

CB_TEST("emulate.powerpc.run_call_limit_truncates", [] {
    p::ExternTable ext{{0x10010000, "alpha"}};
    p::RunOptions o;
    o.externs = &ext;
    const auto r = run_words({LI(0, 450), MTCTR(0), B(0x10010000 - (kBase + 8), true), BC(16, 0, -8), kBlr}, {}, o);
    CB_CHECK(r.truncated);
    CB_CHECK_EQ(r.calls.size(), p::kMaxCalls);
});

CB_TEST("emulate.powerpc.run_absent_code_is_a_call_out", [] {
    p::RunOptions o;
    o.absent = p::AddressRange{0x10020000, 0x10030000};
    const auto r = run_words({X(31, 12, 8, 0, 339), B(0x10020040 - (kBase + 4), true), X(31, 12, 8, 0, 467), kBlr}, {}, o);
    CB_CHECK_EQ(r.exit_text(), "ret");
    CB_CHECK_EQ(r.calls.size(), 1u);
    CB_CHECK_EQ(r.calls[0].name, "@10020040");
    const auto none = run_words({X(31, 12, 8, 0, 339), B(0x10020040 - (kBase + 4), true), X(31, 12, 8, 0, 467), kBlr});    // without `absent` it is just invalid code
    CB_CHECK_EQ(none.exit_text(), "exception:invalid");
});

CB_TEST("emulate.powerpc.run_path_and_branch_log", [] {
    p::RunOptions o;
    o.span = p::AddressRange{kBase, kBase + 0x40};
    o.branches = true;
    // cmpwi r3,5 ; beq +8 ; li r3,1 ; blr
    const auto r = run_words({D(11, 0, 3, 5), BC(12, 2, 8), LI(3, 1), kBlr}, {5}, o);
    CB_CHECK_EQ(r.path.size(), 3u);                       // cmpwi, beq, blr (the li was skipped)
    CB_CHECK_EQ(r.path[0], kBase);
    CB_CHECK_EQ(r.path[2], kBase + 12);
    CB_CHECK(r.branches.has_value());
    CB_CHECK_EQ(r.branches->size(), 1u);
    const p::BranchEvent& e = (*r.branches)[0];
    CB_CHECK_EQ(e.address, kBase + 4);
    CB_CHECK(e.taken);
    CB_CHECK(e.compare.has_value());
    CB_CHECK_EQ(e.compare->size, 4u);
    CB_CHECK_EQ(e.compare->a, 5u);
    CB_CHECK_EQ(e.compare->b, 5u);
    CB_CHECK(!e.compare->a_from.has_value());
    // the same with the other input: the branch falls through
    const auto s = run_words({D(11, 0, 3, 5), BC(12, 2, 8), LI(3, 1), kBlr}, {6}, o);
    CB_CHECK(!(*s.branches)[0].taken);
    CB_CHECK_EQ(s.path.size(), 4u);
    // a compare of loaded values knows where they came from
    const auto l = run_words({D(32, 4, 3, 8), D(11, 0, 4, 0), BC(12, 2, 8), LI(3, 1), kBlr}, {p::kArgsBase}, o);
    CB_CHECK_EQ(l.branches->size(), 1u);
    CB_CHECK(l.branches->front().compare->a_from == p::kArgsBase + 8);
    // with branches off there is no log
    CB_CHECK(!run_words({kBlr}).branches.has_value());
});

CB_TEST("emulate.powerpc.run_shadow_sees_each_step_and_call", [] {
    struct Log : p::Shadow {
        std::vector<std::uint32_t> steps;
        int calls = 0;
        void step(p::Machine& m, std::uint32_t pc) override {
            steps.push_back(pc);
            CB_CHECK_EQ(m.pc, pc);
        }
        void after_call(p::Machine&, bool known, std::uint64_t ret) override {
            ++calls;
            CB_CHECK(known);
            CB_CHECK_EQ(ret, 9u);
        }
    } log;
    p::ExternTable ext{{0x10010000, "alpha"}};
    p::RunOptions o;
    o.externs = &ext;
    o.stub_ret = 9;
    o.shadow = &log;
    run_words({X(31, 31, 8, 0, 339), B(0x10010000 - (kBase + 4), true), X(31, 31, 8, 0, 467), kBlr}, {}, o);
    CB_CHECK_EQ(log.steps.size(), 4u);
    CB_CHECK_EQ(log.calls, 1);
});

CB_TEST("emulate.powerpc.run_args_fargs_and_shared", [] {
    p::RunOptions o;
    o.fargs = {2.5};
    const auto r = run_words({kBlr}, {1, 2, 3, 4, 5, 6, 7, 8, 9}, o);   // a ninth argument is dropped
    CB_CHECK_EQ(r.ret, 1u);
    CB_CHECK_EQ(r.fret, p::to_bits(2.5));
    CB_CHECK_EQ(r.machine->r[10], 8u);
    CB_CHECK_EQ(r.machine->r[11], 0u);
    // pages prepared once and used by a run
    p::Region data;
    data.address = 0x60000000;
    data.data = {0, 0, 0, 77};
    p::RunOptions s;
    s.shared = p::PageImage::of(std::span<const p::Region>(&data, 1));
    const auto q = run_words({D(32, 3, 3, 0), kBlr}, {p::kArgsBase}, s);
    CB_CHECK_EQ(q.ret, 77u);
});

CB_TEST("emulate.powerpc.machine_too_big_for_its_limit", [] {
    p::Region big;
    big.address = 0x10000000;
    big.data.assign((p::kMaxPages + 1) * p::kPageSize, 1);
    try {
        p::Machine m(std::span<const p::Region>(&big, 1));
        CB_CHECK(false);
    } catch (const p::Fault& f) {
        CB_CHECK(f.reason() == p::StopReason::Memory);
    }
});

CB_TEST("emulate.powerpc.record", [] {
    p::ExternTable ext{{0x10010000, "alpha"}};
    p::RunOptions o;
    o.externs = &ext;
    o.span = p::AddressRange{kBase, kBase + 0x20};
    o.stub_ret = 11;
    const p::Region r = code(kBase, {D(15, 4, 0, 0x6000), LI(5, 0x55), D(38, 5, 4, 3), X(31, 12, 8, 0, 339), B(0x10010000 - (kBase + 16), true),
                                     X(31, 12, 8, 0, 467), kBlr});
    const std::vector<std::uint64_t> args{11};
    const cb::emulate::Run run = p::recorded(std::span<const p::Region>(&r, 1), kBase, args, o);
    CB_CHECK_EQ(run.ended(), "ret");
    CB_CHECK(run.endian == cb::emulate::Endian::Big);
    CB_CHECK(!run.truncated);
    CB_CHECK_EQ(std::get<std::uint64_t>(run.results.at("r3")), 11u);
    CB_CHECK_EQ(run.writes.size(), 1u);
    CB_CHECK_EQ(run.writes[0].address, 0x60000003u);
    CB_CHECK_EQ(run.writes[0].size, 1u);
    CB_CHECK_EQ(run.writes[0].value, 0x55u);
    CB_CHECK_EQ(run.writes[0].old.value_or(9), 0u);
    CB_CHECK_EQ(run.calls.size(), 1u);
    CB_CHECK_EQ(run.calls[0].name.value_or(""), "alpha");
    CB_CHECK_EQ(run.calls[0].args->size(), 8u);
    CB_CHECK(run.preserved.has_value());
    CB_CHECK_EQ(run.preserved->size(), 19u);              // r14..r31 and r1
    CB_CHECK(run.kept());
    CB_CHECK_EQ(run.path.size(), 7u);
    std::string order;
    for (const auto& kv : run.results) order += kv.first;
    CB_CHECK_EQ(order, "r3f1");
    CB_CHECK_EQ(std::any_cast<const p::RunResult&>(run.raw).steps, 7u);
    // a function that does not hand r14 back is not kept
    const p::Region bad = code(kBase, {LI(14, 1), kBlr});
    CB_CHECK(!p::recorded(std::span<const p::Region>(&bad, 1), kBase).kept());
});
