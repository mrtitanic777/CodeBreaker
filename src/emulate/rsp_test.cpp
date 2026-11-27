// Tests of the RSP interpreter (emulate/rsp) on small microcode assembled by decompile/mips (Variant::Rsp): the scalar unit and its delay
// slots, a vector load / multiply-accumulate / store, the flags, the divide unit, the DMA and the RDP's hand-off, and how a run ends.
#include "emulate/rsp.hpp"

#include <string>
#include <vector>

#include "core/testing.hpp"
#include "decompile/mips.hpp"

namespace cb::emulate::rsp {
namespace {

namespace m = decompile::mips;

// SP memory (DMEM, IMEM) with `lines` assembled into IMEM from its start, and an RDRAM of 64 KB.
struct Machine {
    Bytes sp = Bytes(2 * kMemSize, 0);
    Bytes rdram = Bytes(0x10000, 0);
    explicit Machine(const std::vector<std::string>& lines) {
        std::uint32_t at = 0;
        for (const std::string& line : lines) {
            const Bytes w = m::assemble(line, 0x04001000 + at, {}, m::Variant::Rsp);
            std::copy(w.begin(), w.end(), sp.begin() + kMemSize + at);
            at += static_cast<std::uint32_t>(w.size());
        }
    }
    std::uint16_t dmem16(std::uint32_t a) const { return static_cast<std::uint16_t>(sp[a] << 8 | sp[a + 1]); }
    void put16(std::uint32_t a, std::uint16_t v) {
        sp[a] = static_cast<std::uint8_t>(v >> 8);
        sp[a + 1] = static_cast<std::uint8_t>(v);
    }
};

}  // namespace

CB_TEST("emulate.rsp.scalar_code_its_delay_slots_and_break", [] {
    Machine mc({"addiu $t0, $zero, 5",        // a loop: t1 += t0, t0 -= 1 until t0 is 0
                "addiu $t1, $zero, 0",
                "addu $t1, $t1, $t0",
                "addiu $t0, $t0, -1",
                "bne $t0, $zero, 0x04001008",
                "sw $t1, 0x100($zero)",       // (the delay slot runs each time)
                "break"});
    Rsp sp(mc.sp, mc.rdram);
    const RunResult r = sp.run(0);
    CB_CHECK(r.broke);
    CB_CHECK(!r.fault);
    CB_CHECK_EQ(sp.gpr(9), 15u);
    CB_CHECK_EQ(mc.dmem16(0x102), 15);
    CB_CHECK((sp.status() & kHalt) && (sp.status() & kBroke));
});

CB_TEST("emulate.rsp.vectors_load_multiply_accumulate_and_store", [] {
    Machine mc({"lqv $v1[0], 0($zero)",          // v1 = the 8 lanes at DMEM 0
                "lqv $v2[0], 16($zero)",         // v2 = the 8 at DMEM 16
                "vmudh $v3, $v1, $v2[0]",        // v3 = v1 * v2's lane 0 (integer)
                "vaddc $v4, $v1, $v2",           // unsigned add: VCO's carries
                "vlt $v5, $v1, $v2",             // the smaller of each, VCC's flags
                "sqv $v3[0], 32($zero)",
                "sqv $v5[0], 48($zero)",
                "cfc2 $t0, $vco",
                "cfc2 $t1, $vcc",
                "break"});
    for (unsigned i = 0; i < 8; ++i) {
        mc.put16(i * 2, static_cast<std::uint16_t>(i + 1));                 // 1 2 3 ... 8
        mc.put16(16 + i * 2, static_cast<std::uint16_t>(i == 7 ? 0xFFFF : 3));   // 3 3 3 3 3 3 3 -1
    }
    Rsp sp(mc.sp, mc.rdram);
    CB_CHECK(sp.run(0).broke);
    for (unsigned i = 0; i < 8; ++i) CB_CHECK_EQ(mc.dmem16(32 + i * 2), (i + 1) * 3);
    // VADDC: only 8 + 0xFFFF carries out of 16 bits; VLT then clears VCO, so the read of VCO after VLT is 0
    CB_CHECK_EQ(sp.gpr(8), 0u);
    // VLT: v1 < v2 for 1 and 2 (lanes 0, 1); lane 7 compares 8 with -1 (signed): not less
    CB_CHECK_EQ(sp.gpr(9), 0x03u);
    CB_CHECK_EQ(mc.dmem16(48), 1);
    CB_CHECK_EQ(mc.dmem16(48 + 14), 0xFFFF);
    CB_CHECK_EQ(sp.vr(4)[7], 7);   // 8 + 0xFFFF, the low 16 bits
});

CB_TEST("emulate.rsp.fractions_clamp_and_the_accumulator_keeps_48_bits", [] {
    Machine mc({"lqv $v1[0], 0($zero)",
                "vmulf $v2, $v1, $v1",           // (x * x * 2 + 0x8000) >> 16
                "vmacf $v3, $v1, $v1",           // and again onto the accumulator: twice as much, clamped
                "break"});
    mc.put16(0, 0x4000);   // 0.5
    mc.put16(2, 0x8000);   // -1.0: -1 * -1 = 1.0 does not fit a signed fraction
    Rsp sp(mc.sp, mc.rdram);
    CB_CHECK(sp.run(0).broke);
    CB_CHECK_EQ(sp.vr(2)[0], 0x2000);   // 0.25
    CB_CHECK_EQ(sp.vr(2)[1], 0x7FFF);   // clamped
    CB_CHECK_EQ(sp.vr(3)[0], 0x4000);   // 0.5
    CB_CHECK_EQ(sp.acc(0) >> 16, 0x4000u);
});

CB_TEST("emulate.rsp.the_divide_unit", [] {
    CB_CHECK_EQ(Rsp::reciprocals()[0], 0xFFFF);
    CB_CHECK_EQ(Rsp::reciprocals()[1], 0xFF00);
    CB_CHECK_EQ(Rsp::inverse_roots()[0], 0xFFFF);
    Machine mc({"lqv $v1[0], 0($zero)",
                "vrcp $v2[0], $v1[0]",           // 1 / 2: the reciprocal of lane 0 (= 2), its low half into v2's lane 0, its high half into DIV_OUT
                "vrcph $v2[1], $v1[0]",          // DIV_OUT into lane 1
                "vrcp $v3[0], $v1[1]",           // 1 / 0
                "break"});
    mc.put16(0, 2);
    mc.put16(2, 0);
    Rsp sp(mc.sp, mc.rdram);
    CB_CHECK(sp.run(0).broke);
    // the result is about 2^31 / x: 1/2 is 0x3FFFE000 (the table's first entry saturates), its low half in lane 0, its high half in lane 1
    CB_CHECK_EQ(sp.vr(2)[0], 0xE000);
    CB_CHECK_EQ(sp.vr(2)[1], 0x3FFF);
    CB_CHECK_EQ(sp.vr(3)[0], 0xFFFF);   // 1/0 is the largest: 0x7FFFFFFF
});

CB_TEST("emulate.rsp.dma_and_the_rdp_hand_off", [] {
    Machine mc({"addiu $t0, $zero, 0x200",
                "mtc0 $t0, $0",                  // SP_MEM_ADDR: DMEM 0x200
                "addiu $t0, $zero, 0x1000",
                "mtc0 $t0, $1",                  // SP_DRAM_ADDR: RDRAM 0x1000
                "addiu $t0, $zero, 15",
                "mtc0 $t0, $2",                  // SP_RD_LEN: 16 bytes in
                "addiu $t0, $zero, 0x200",
                "mtc0 $t0, $0",
                "addiu $t0, $zero, 0x2000",
                "mtc0 $t0, $1",
                "addiu $t0, $zero, 15",
                "mtc0 $t0, $3",                  // SP_WR_LEN: the same 16 bytes out to 0x2000
                "addiu $t0, $zero, 2",
                "mtc0 $t0, $11",                 // DPC_STATUS: set XBUS (the list is in DMEM)
                "addiu $t0, $zero, 0x200",
                "mtc0 $t0, $8",                  // DPC_START
                "addiu $t0, $zero, 0x210",
                "mtc0 $t0, $9",                  // DPC_END: the RDP's list is handed over
                "break"});
    for (unsigned i = 0; i < 16; ++i) mc.rdram[0x1000 + i] = static_cast<std::uint8_t>(0xA0 + i);
    Rsp sp(mc.sp, mc.rdram);
    std::vector<std::uint32_t> lists;
    bool xbus = false;
    sp.on_dpc = [&](std::uint32_t start, std::uint32_t end, bool x) {
        lists.push_back(start);
        lists.push_back(end);
        xbus = x;
    };
    CB_CHECK(sp.run(0).broke);
    CB_CHECK_EQ(mc.sp[0x200], 0xA0);
    CB_CHECK_EQ(mc.sp[0x20F], 0xAF);
    CB_CHECK_EQ(mc.rdram[0x2000], 0xA0);
    CB_CHECK_EQ(mc.rdram[0x200F], 0xAF);
    CB_CHECK(lists == (std::vector<std::uint32_t>{0x200, 0x210}));
    CB_CHECK(xbus);
});

CB_TEST("emulate.rsp.a_run_ends_at_a_fault_or_the_step_limit", [] {
    Machine spin({"j 0x04001000", "nop"});
    Rsp a(spin.sp, spin.rdram);
    const RunResult r = a.run(0, 1000);
    CB_CHECK(!r.broke);
    CB_CHECK_EQ(*r.fault, std::string("step limit"));
    Machine bad({"nop"});
    bad.sp[kMemSize + 4] = 0x00;   // mult $zero, $zero ... : not an RSP instruction
    bad.sp[kMemSize + 7] = 0x18;
    Rsp b(bad.sp, bad.rdram);
    const RunResult f = b.run(0);
    CB_CHECK(f.fault && f.fault->starts_with("not an RSP instruction"));
});

}  // namespace cb::emulate::rsp
