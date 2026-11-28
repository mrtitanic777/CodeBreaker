// cb-unity: alone
// emulate/cpu/pcode_cpu_v810_unit_test.cpp -- the V810 on the one emulator (pcode_cpu::run on v810exact.sla), the retired own core's unit tests: flags, the 64-bit multiply and
// the divide's remainder, loads and stores of every size, branches, calls, floating point, the bit-string instructions, the stops and the device log, every expected value
// as it was. Programs are written in the assembler's own syntax.
#include "emulate/v810.hpp"
#include "emulate/cpu/pcode_cpu_v810.hpp"

#include <string>

#include "source/processors.hpp"
#include "core/testing.hpp"

namespace cb::emulate::v810 {
namespace {

constexpr std::uint32_t kRom = 0x07000000, kWork = 0x05000000;

// A machine with the program at 0x07000000, 64 KB of work RAM at 0x05000000 and a logged device at 0x02000000.
struct Rig {
    Bus bus{0x07FFFFFF};
    RunResult result;

    RunResult run(const std::string& program, RunOptions options = {}) {
        const source::processors::Image image = source::processors::assemble_described(source::processors::find("v810"), ".org 0x07000000\n" + program + "\n        jmp [r31]\n", ".");
        bus.memory(kRom, 0x07FFFFFF, image.data, false, false, 0x10000);
        bus.memory(kWork, 0x05FFFFFF, {}, true, false, 0x10000);
        bus.device(0x02000000, 0x02FFFFFF, [](std::uint32_t, unsigned) -> std::uint32_t { return 0xA5; });
        result = emulate::pcode_cpu::run(bus, kRom, options);
        return result;
    }
    std::uint32_t r(unsigned n) const { return result.regs.r[n]; }
};

}  // namespace

CB_TEST("emulate.pcode_cpu.v810_the_flags_of_the_arithmetic", [] {
    Rig m;
    m.run(R"(
        movhi 0x7fff,r0,r6
        ori 0xffff,r6,r6
        add 0x1,r6
        stsr psw,r10
        mov -0x1,r7
        add 0x1,r7
        stsr psw,r11
        mov 0x1,r8
        cmp 0x2,r8
        stsr psw,r12
        mov 0x3,r9
        shl 0x1f,r9
        stsr psw,r13
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(6), 0x80000000u);
    CB_CHECK_EQ(m.r(10), psw::S | psw::OV);
    CB_CHECK_EQ(m.r(7), 0u);
    CB_CHECK_EQ(m.r(11), psw::Z | psw::CY);
    CB_CHECK_EQ(m.r(8), 1u);                         // (cmp does not write)
    CB_CHECK_EQ(m.r(12), psw::S | psw::CY);
    CB_CHECK_EQ(m.r(9), 0x80000000u);                // 3 << 31 keeps the low bit's move; the bit shifted out last was 1
    CB_CHECK_EQ(m.r(13), psw::S | psw::CY);
    CB_CHECK_EQ(m.r(0), 0u);
});

CB_TEST("emulate.pcode_cpu.v810_multiply_leaves_the_high_word_in_r30_and_divide_the_remainder", [] {
    Rig m;
    m.run(R"(
        movhi 0x1,r0,r6
        mov r6,r7
        mul r6,r7
        mov r30,r10
        stsr psw,r11
        mov 0x7,r8
        mov 0x2,r9
        div r9,r8
        movhi 0x8000,r0,r12
        mov -0x1,r13
        div r13,r12
        mov -0x7,r14
        mov 0x2,r15
        divu r15,r14
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(7), 0u);
    CB_CHECK_EQ(m.r(10), 1u);
    CB_CHECK((m.r(11) & psw::OV) != 0);
    CB_CHECK_EQ(m.r(8), 3u);
    CB_CHECK_EQ(m.r(12), 0x80000000u);               // INT_MIN / -1 stays INT_MIN
    CB_CHECK_EQ(m.r(14), 0x7FFFFFFCu);               // 0xFFFFFFF9 / 2
    CB_CHECK_EQ(m.r(30), 1u);
});

CB_TEST("emulate.pcode_cpu.v810_loads_and_stores_of_each_size", [] {
    Rig m;
    m.run(R"(
        movhi 0x500,r0,r6
        mov -0x2,r7
        st.b r7,0x0[r6]
        ld.b 0x0[r6],r8
        in.b 0x0[r6],r9
        movhi 0x1234,r0,r10
        ori 0x5678,r10,r10
        st.w r10,0x4[r6]
        ld.h 0x4[r6],r11
        ld.h 0x6[r6],r12
        ld.w 0x5[r6],r13
        movhi 0x200,r0,r14
        st.h r10,0x28[r14]
        in.b 0x28[r14],r15
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(8), 0xFFFFFFFEu);
    CB_CHECK_EQ(m.r(9), 0xFEu);
    CB_CHECK_EQ(m.r(11), 0x5678u);
    CB_CHECK_EQ(m.r(12), 0x1234u);
    CB_CHECK_EQ(m.r(13), 0x12345678u);               // (an unaligned word access ignores the low bits)
    CB_CHECK_EQ(m.r(15), 0xA5u);
    const std::span<std::uint8_t> work = m.bus.bytes(kWork);
    CB_CHECK_EQ(work[0], 0xFE);
    CB_CHECK((Bytes(work.begin() + 4, work.begin() + 8) == Bytes{0x78, 0x56, 0x34, 0x12}));
    CB_CHECK_EQ(m.result.io.size(), 2u);
    CB_CHECK(m.result.io[0].kind == DeviceAccess::Kind::Write);
    CB_CHECK(m.result.io[0].address == 0x02000028u && m.result.io[0].size == 2 && m.result.io[0].value == 0x5678u);
    CB_CHECK(m.result.io[1].kind == DeviceAccess::Kind::Read && m.result.io[1].value == 0xA5u);
});

CB_TEST("emulate.pcode_cpu.v810_branches_calls_and_returns", [] {
    Rig m;
    m.run(R"(
        mov 0x5,r6
        mov 0x0,r7
    loop:
        add r6,r7
        add -0x1,r6
        bne loop
        mov r31,r20
        .byte 0x00, 0xac, 0x08, 0x00   ; jal fn (the description's assembler does not take the 26-bit displacement of a jal yet)
        mov r20,r31
        br done
    fn:
        mov 0x9,r8
        jmp [r31]
    done:
        setf 0x2,r9
        cmp r7,r7
        setf 0x2,r10
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(7), 15u);
    CB_CHECK_EQ(m.r(6), 0u);
    CB_CHECK_EQ(m.r(8), 9u);
    CB_CHECK_EQ(m.r(9), 1u);                         // (Z is still set by the loop's last add, which made r6 zero)
    CB_CHECK_EQ(m.r(10), 1u);
});

CB_TEST("emulate.pcode_cpu.v810_single_precision_floating_point", [] {
    Rig m;
    m.run(R"(
        movhi 0x3fc0,r0,r6
        movhi 0x4010,r0,r7
        addf.s r6,r7
        cvt.sw r7,r8
        trnc.sw r7,r9
        mov 0x3,r10
        cvt.ws r10,r11
        movhi 0x3f80,r0,r12
        movhi 0x0,r0,r13
        divf.s r13,r12
        stsr psw,r14
        cmpf.s r6,r7
        stsr psw,r15
    )");
    CB_CHECK_EQ(m.result.exit(), "return");
    CB_CHECK_EQ(m.r(7), 0x40700000u);                // 2.25 + 1.5
    CB_CHECK_EQ(m.r(8), 4u);                         // 3.75 rounded
    CB_CHECK_EQ(m.r(9), 3u);                         // truncated
    CB_CHECK_EQ(m.r(11), 0x40400000u);
    CB_CHECK_EQ(m.r(12), 0x3F800000u);               // dividing by zero leaves the operand and sets the flag
    CB_CHECK((m.r(14) & psw::FZD) != 0);
    CB_CHECK_EQ(m.r(15) & psw::Z, 0u);               // 3.75 is not 1.5
    CB_CHECK_EQ(m.r(15) & psw::S, 0u);
});

CB_TEST("emulate.pcode_cpu.v810_bit_strings", [] {
    {
        Rig m;
        m.bus.memory(kWork, 0x05FFFFFF, {}, true, false, 0x10000);   // (the rig adds its own after this one; the first range wins)
        RunOptions o;
        o.r[30] = kWork;
        o.r[29] = kWork + 0x10;
        o.r[28] = 40;
        o.r[27] = 0;
        o.r[26] = 4;
        std::span<std::uint8_t> work = m.bus.bytes(kWork);
        for (int k = 0; k < 4; ++k) work[k] = 0xFF;
        work[4] = 0xFF;
        m.run("        movbsu\n", o);
        CB_CHECK_EQ(m.result.exit(), "return");
        CB_CHECK((Bytes(work.begin() + 0x10, work.begin() + 0x18) == Bytes{0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0x0F, 0x00, 0x00}));
        CB_CHECK_EQ(m.r(28), 0u);
        CB_CHECK_EQ(m.r(30), kWork + 4);
        CB_CHECK_EQ(m.r(27), 8u);
        CB_CHECK_EQ(m.r(29), kWork + 0x14);
        CB_CHECK_EQ(m.r(26), 12u);
    }
    {
        Rig m;
        m.bus.memory(kWork, 0x05FFFFFF, {}, true, false, 0x10000);
        m.bus.bytes(kWork)[1] = 0x01;   // bit 8
        RunOptions o;
        o.r[30] = kWork;
        o.r[28] = 32;
        m.run("        sch1bsu\n        stsr psw,r10\n", o);
        CB_CHECK_EQ(m.r(29), 8u);
        CB_CHECK_EQ(m.r(28), 24u);
        CB_CHECK_EQ(m.r(27), 8u);
        CB_CHECK_EQ(m.r(10) & psw::Z, 0u);
    }
});

CB_TEST("emulate.pcode_cpu.v810_xb_xh_rev_and_the_system_registers", [] {
    Rig m;
    m.run(R"(
        movhi 0x1234,r0,r6
        ori 0x5678,r6,r6
        mov r6,r7
        xb r7
        mov r6,r8
        xh r8
        mov r6,r9
        rev r9,r10
        ldsr r6,eipc
        stsr eipc,r11
        sei
        stsr psw,r12
        cli
        stsr psw,r13
    )");
    CB_CHECK_EQ(m.r(7), 0x12347856u);
    CB_CHECK_EQ(m.r(8), 0x56781234u);
    CB_CHECK_EQ(m.r(10), 0x1E6A2C48u);               // 0x12345678 with its bits reversed
    CB_CHECK_EQ(m.r(11), 0x12345678u);
    CB_CHECK((m.r(12) & psw::ID) != 0);
    CB_CHECK((m.r(13) & psw::ID) == 0);
});

CB_TEST("emulate.pcode_cpu.v810_stops_and_limits", [] {
    {
        Rig m;
        m.run("        .half 0x6c00\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: undefined instruction at 07000000");
    }
    {
        Rig m;
        m.run("        trap 0x5\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: trap 5 at 07000000");
    }
    {
        Rig m;
        m.run("        mov 0x5,r6\n        mov 0x0,r7\n        div r7,r6\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: divide by zero at 07000004");
    }
    {
        Rig m;
        m.run("        halt\n");
        CB_CHECK_EQ(m.result.exit(), "stopped: halt at 07000000");
    }
    {
        Rig m;
        RunOptions o;
        o.max_steps = 100;
        m.run("    spin:\n        br spin\n", o);
        CB_CHECK_EQ(m.result.exit(), "limit");
        CB_CHECK_EQ(m.result.steps, 100u);
    }
});

}  // namespace cb::emulate::v810
