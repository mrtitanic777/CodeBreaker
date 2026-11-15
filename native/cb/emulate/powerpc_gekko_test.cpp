// cb-unity: alone (file-local names that other files in a batch also use)
// The PowerPC interpreter as the Nintendo consoles use it (emulate/powerpc.hpp): Gekko's paired singles with the values worked out by
// hand, the quantized loads and stores, the quantization registers, and the memory-mapped hardware registers that are logged instead of
// being memory. The Xbox 360's use of the same interpreter (opcode 4 is VMX128 there) must not change.
#include <bit>
#include <cmath>
#include <string>
#include <vector>

#include "cb/build/ppc_asm.hpp"
#include "cb/core/testing.hpp"
#include "cb/decompile/ppc.hpp"
#include "cb/emulate/powerpc.hpp"

namespace pp = cb::emulate::powerpc;
namespace dp = cb::decompile::ppc;

namespace {

constexpr std::uint32_t kBase = 0x80003100;

double as_double(std::uint64_t bits) { return std::bit_cast<double>(bits); }
std::uint64_t as_bits(double x) { return std::bit_cast<std::uint64_t>(x); }

std::uint32_t word(const std::string& line) {
    const cb::Bytes b = dp::assemble(line, kBase);
    return static_cast<std::uint32_t>(b[0]) << 24 | static_cast<std::uint32_t>(b[1]) << 16 | static_cast<std::uint32_t>(b[2]) << 8 | b[3];
}

pp::Machine gekko() {
    pp::Machine m;
    m.paired_singles = true;
    m.pc = kBase;
    return m;
}

void set_pair(pp::Machine& m, unsigned reg, double ps0, double ps1) {
    m.f[reg] = as_bits(ps0);
    m.ps1[reg] = as_bits(ps1);
}

}  // namespace

CB_TEST("emulate.powerpc_gekko.paired_arithmetic", [] {
    pp::Machine m = gekko();
    set_pair(m, 2, 1.5, 2.5);
    set_pair(m, 3, 0.25, 0.5);
    pp::step(m, word("ps_add f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 1.75);
    CB_CHECK_EQ(as_double(m.ps1[1]), 3.0);
    CB_CHECK_EQ(m.pc, kBase + 4);

    pp::step(m, word("ps_sub f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 1.25);
    CB_CHECK_EQ(as_double(m.ps1[1]), 2.0);

    // ps_mul frD,frA,frC: each half of A times the same half of C
    pp::step(m, word("ps_mul f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 0.375);
    CB_CHECK_EQ(as_double(m.ps1[1]), 1.25);

    // ps_madd frD,frA,frC,frB: A * C + B
    set_pair(m, 2, 2.0, 4.0);
    set_pair(m, 3, 3.0, 5.0);
    set_pair(m, 4, 1.0, 10.0);
    pp::step(m, word("ps_madd f1,f2,f3,f4"));
    CB_CHECK_EQ(as_double(m.f[1]), 7.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 30.0);
    pp::step(m, word("ps_nmsub f1,f2,f3,f4"));   // -(A * C - B)
    CB_CHECK_EQ(as_double(m.f[1]), -5.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), -10.0);

    // ps_muls0 uses C's first half for both; ps_sum0 adds A's first and B's second half and keeps C's second
    pp::step(m, word("ps_muls0 f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 6.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 12.0);
    pp::step(m, word("ps_sum0 f1,f2,f3,f4"));
    CB_CHECK_EQ(as_double(m.f[1]), 12.0);   // a0 + b1 = 2 + 10
    CB_CHECK_EQ(as_double(m.ps1[1]), 5.0);  // c1

    // the merges pick one half of each
    set_pair(m, 2, 1.0, 2.0);
    set_pair(m, 3, 3.0, 4.0);
    pp::step(m, word("ps_merge00 f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 1.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 3.0);
    pp::step(m, word("ps_merge01 f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 1.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 4.0);
    pp::step(m, word("ps_merge10 f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 2.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 3.0);
    pp::step(m, word("ps_merge11 f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 2.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 4.0);

    // moves and sign changes act on both halves
    set_pair(m, 3, -3.0, 4.0);
    pp::step(m, word("ps_abs f1,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 3.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 4.0);
    pp::step(m, word("ps_neg f1,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), 3.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), -4.0);
    pp::step(m, word("ps_mr f1,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), -3.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 4.0);

    // ps_cmpu0 compares the first halves into a condition field: A < B sets LT (8), A > B GT (4), equal EQ (2)
    set_pair(m, 2, 1.0, 9.0);
    set_pair(m, 3, 2.0, 0.0);
    pp::step(m, word("ps_cmpu0 cr3,f2,f3"));
    CB_CHECK_EQ((m.cr >> 16) & 0xF, 8u);
    pp::step(m, word("ps_cmpu1 cr3,f2,f3"));
    CB_CHECK_EQ((m.cr >> 16) & 0xF, 4u);
});

CB_TEST("emulate.powerpc_gekko.single_precision_rounding", [] {
    pp::Machine m = gekko();
    set_pair(m, 2, 1.0, 1.0);
    set_pair(m, 3, 3.0, 3.0);
    pp::step(m, word("ps_div f1,f2,f3"));
    CB_CHECK_EQ(as_double(m.f[1]), static_cast<double>(1.0f / 3.0f));   // a result is rounded to single
    CB_CHECK(as_double(m.f[1]) != 1.0 / 3.0);
});

CB_TEST("emulate.powerpc_gekko.quantized_loads_and_stores", [] {
    pp::Machine m = gekko();
    m.r[3] = 0x1000;
    // floats: the default quantization (type 0)
    m.write(0x1000, 4, 0x3F800000);   // 1.0f
    m.write(0x1004, 4, 0x40000000);   // 2.0f
    pp::step(m, word("psq_l f1,0(r3),0,0"));
    CB_CHECK_EQ(as_double(m.f[1]), 1.0);
    CB_CHECK_EQ(as_double(m.ps1[1]), 2.0);
    // W = 1: one value, and the second half is 1.0
    pp::step(m, word("psq_l f2,4(r3),1,0"));
    CB_CHECK_EQ(as_double(m.f[2]), 2.0);
    CB_CHECK_EQ(as_double(m.ps1[2]), 1.0);
    // an update form leaves the address in rA
    pp::step(m, word("psq_lu f4,8(r3),0,0"));
    CB_CHECK_EQ(m.r[3], 0x1008u);
    m.r[3] = 0x1000;

    // signed 16-bit values with a scale of 2: the loaded value is the integer divided by 4
    m.gqr[2] = 0x02070000;   // load scale 2, load type 7 (s16)
    m.write(0x1010, 2, 100);
    m.write(0x1012, 2, 0xFFFC);   // -4
    m.r[3] = 0x1010;
    pp::step(m, word("psq_l f5,0(r3),0,2"));
    CB_CHECK_EQ(as_double(m.f[5]), 25.0);
    CB_CHECK_EQ(as_double(m.ps1[5]), -1.0);

    // stores truncate and saturate: unsigned 8-bit with no scale
    m.gqr[1] = 4;   // store type 4 (u8), scale 0
    set_pair(m, 1, 200.7, 300.0);
    m.r[3] = 0x1020;
    pp::step(m, word("psq_st f1,0(r3),0,1"));
    CB_CHECK_EQ(m.read(0x1020, 1), 200u);
    CB_CHECK_EQ(m.read(0x1021, 1), 255u);
    // W = 1 stores only the first value
    m.write(0x1031, 1, 0x77);
    pp::step(m, word("psq_st f1,16(r3),1,1"));
    CB_CHECK_EQ(m.read(0x1030, 1), 200u);
    CB_CHECK_EQ(m.read(0x1031, 1), 0x77u);
    // floats go out as singles
    m.gqr[0] = 0;
    set_pair(m, 6, 0.5, -2.0);
    m.r[3] = 0x1040;
    pp::step(m, word("psq_st f6,0(r3),0,0"));
    CB_CHECK_EQ(m.read(0x1040, 4), 0x3F000000u);
    CB_CHECK_EQ(m.read(0x1044, 4), 0xC0000000u);
    // the indexed form
    m.r[3] = 0x1000;
    m.r[4] = 0x40;
    pp::step(m, word("psq_lx f7,r3,r4,0,0"));
    CB_CHECK_EQ(as_double(m.f[7]), 0.5);
    CB_CHECK_EQ(as_double(m.ps1[7]), -2.0);
});

CB_TEST("emulate.powerpc_gekko.quantization_registers_are_special_registers", [] {
    pp::Machine m = gekko();
    m.r[3] = 0x12345678;
    pp::step(m, word("mtspr 914,r3"));   // GQR2
    CB_CHECK_EQ(m.gqr[2], 0x12345678u);
    pp::step(m, word("mfspr r4,914"));
    CB_CHECK_EQ(m.r[4], 0x12345678u);
});

CB_TEST("emulate.powerpc_gekko.lfs_fills_both_halves", [] {
    pp::Machine m = gekko();
    m.r[3] = 0x1000;
    m.write(0x1000, 4, 0x40490FDB);   // pi as a single
    pp::step(m, word("lfs f1,0(r3)"));
    CB_CHECK_EQ(as_double(m.f[1]), as_double(m.ps1[1]));
    CB_CHECK(std::abs(as_double(m.f[1]) - 3.14159274) < 1e-6);
});

CB_TEST("emulate.powerpc_gekko.hardware_registers_are_logged", [] {
    pp::Machine m = gekko();
    m.set_devices({{0xCC000000, 0xCC010000}});
    m.r[3] = 0xCC002000;
    m.r[5] = 0xABCD;
    pp::step(m, word("lwz r4,0(r3)"));
    CB_CHECK_EQ(m.r[4], 0u);
    pp::step(m, word("lwz r4,0(r3)"));
    CB_CHECK_EQ(m.r[4], 1u);   // a read answers how many times the register was read before
    pp::step(m, word("stw r5,4(r3)"));
    pp::step(m, word("sth r5,8(r3)"));
    CB_CHECK_EQ(m.io().size(), 4u);
    CB_CHECK_EQ(m.io()[0].address, 0xCC002000u);
    CB_CHECK(!m.io()[0].write);
    CB_CHECK_EQ(m.io()[2].address, 0xCC002004u);
    CB_CHECK(m.io()[2].write);
    CB_CHECK_EQ(m.io()[2].value, 0xABCDu);
    CB_CHECK_EQ(m.io()[3].size, 2u);
    CB_CHECK(m.writes().empty());      // a hardware register is not memory
    m.r[3] = 0x2000;
    pp::step(m, word("stw r5,0(r3)"));   // outside the device range: memory as before
    CB_CHECK_EQ(m.io().size(), 4u);
    CB_CHECK_EQ(m.read(0x2000, 4), 0xABCDu);
});

CB_TEST("emulate.powerpc_gekko.run_logs_hardware_and_stops_without_the_flag", [] {
    // lis r3,0xCC00; lwz r4,0x2000(r3); stw r4,0x2004(r3); mr r3,r4; blr
    cb::Bytes code;
    for (const char* line : {"lis r3,0xCC00", "lwz r4,8192(r3)", "stw r4,8196(r3)", "mr r3,r4", "blr"}) {
        const cb::Bytes b = dp::assemble(line, kBase + static_cast<std::uint32_t>(code.size()));
        code.insert(code.end(), b.begin(), b.end());
    }
    const std::vector<pp::Region> regions = {{kBase, code}};
    pp::RunOptions options;
    options.devices = {{0xCC000000, 0xCC010000}};
    options.paired_singles = true;
    const pp::RunResult r = pp::run(regions, kBase, {}, options);
    CB_CHECK_EQ(r.exit_text(), std::string("ret"));
    CB_CHECK_EQ(r.machine->io().size(), 2u);
    CB_CHECK(r.writes.empty());

    // the same opcode 4 word is VMX128 on the Xbox 360: not run
    cb::Bytes vector = dp::assemble("ps_add f1,f2,f3", kBase);
    const std::vector<pp::Region> vector_regions = {{kBase, vector}};
    const pp::RunResult v = pp::run(vector_regions, kBase, {}, {});
    CB_CHECK_EQ(v.exit_text(), std::string("exception:unsupported"));
});
